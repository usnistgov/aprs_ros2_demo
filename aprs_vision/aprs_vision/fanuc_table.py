#!/usr/bin/env python3

import os
import cv2
import math
import numpy as np
from scipy.interpolate import griddata

from cv2.typing import MatLike
from typing import Optional

from rclpy.node import Node
from rclpy.qos import qos_profile_default

from ament_index_python.packages import get_package_share_directory

from tf2_ros.transform_broadcaster import TransformBroadcaster

from example_interfaces.srv import Trigger
from sensor_msgs.msg import Image
from aprs_interfaces.msg import Trays, Tray, SlotInfo
from aprs_vision.slot_offsets import SlotOffsets
from geometry_msgs.msg import TransformStamped, Point, Quaternion


class FanucTable(Node):    
    tray_names = {
        Tray.SMALL_GEAR_TRAY: 'small_gear_tray',
        Tray.MEDIUM_GEAR_TRAY: 'medium_gear_tray',
        Tray.LARGE_GEAR_TRAY: 'large_gear_tray',
        Tray.M2L1_KIT_TRAY: 'm2l1_kit_tray',
        Tray.S2L2_KIT_TRAY: 's2l2_kit_tray'
    }

    kit_tray_types = [Tray.S2L2_KIT_TRAY, Tray.M2L1_KIT_TRAY]
    part_tray_types = [Tray.SMALL_GEAR_TRAY, Tray.MEDIUM_GEAR_TRAY, Tray.LARGE_GEAR_TRAY]

    table_origin = Point(x=228.591, y=228.287, z=-0.01)
    tray_height = 0.017 # TODO: find real value
    gear_height = 0.02 # TODO: find real value

    def __init__(self):
        super().__init__(node_name='fanuc_vision_node')

        self.capture = cv2.VideoCapture("http://192.168.1.104/mjpg/video.mjpg")

        self.conversion_factor = 0.8466 # 30 pixels is 25.4 mm so 1 pixel is .8466 mm

        share_path = get_package_share_directory('aprs_vision')

        self.map_x = np.load(os.path.join(share_path, 'config', 'map_x.npy'))
        self.map_y = np.load(os.path.join(share_path, 'config', 'map_y.npy'))
        self.base_background = cv2.imread(os.path.join(share_path, 'config', 'background.jpg'))

        self.slot_pixel_centers: dict[str, tuple[int, int]] = {}

        # ROS Messages
        self.trays_info: Optional[Trays] = None
        self.current_frame: Optional[Image] = None

        # ROS Services
        self.locate_trays_srv = self.create_service(Trigger, 'locate_trays', self.locate_trays_cb)
        self.update_trays_info_srv = self.create_service(Trigger, 'update_slots', self.update_slots_cb)
        self.calibrate_maps_srv = self.create_service(Trigger, 'calibrate_maps', self.calibrate_maps_cb)

        # ROS Topics
        self.trays_info_pub = self.create_publisher(Trays, 'trays_info', qos_profile_default)
        self.raw_image_pub = self.create_publisher(Image, 'raw_image', 10)
        self.detected_trays_image_pub = self.create_publisher(Image, 'detected_trays_image', qos_profile_default)
    
        # TF Broadcaster
        self.tf_broadcaster = TransformBroadcaster(self)
        self.transforms: list[TransformStamped] = []

        # ROS Timers
        self.publish_timer = self.create_timer(timer_period_sec=1, callback=self.publish)

    def locate_trays_cb(self, _, response: Trigger.Response) -> Trigger.Response:
        if self.trays_info is not None:
            response.message = "Trays already located"
            response.success = False
            return response
        
        frame = self.get_most_recent_frame()

        if frame is None:
            response.message = "Unable to connect to camera"
            response.success = False
            return response
        
        # Save current frame as ROS message for publishing
        self.current_frame = self.build_img_msg_from_mat(frame)

        # Function to determine the location of the trays provided an initial image
        frame = self.rectify_frame(frame)

        original_img = frame.copy()

        # Remove table backgroud
        frame = self.remove_background(frame)

        # Remove and replace and large gears present
        frame = self.remove_and_replace_gears(frame, gear_size="large")
        
        # Remove and replace any medium gears present
        frame = self.remove_and_replace_gears(frame, gear_size="medium")

        trays_on_table = self.detect_trays(frame)

        # Fill tray info message
        self.trays_info = self.determine_tray_info(trays_on_table)

        # Check if slots are occupied 
        self.update_slots(original_img)

        response.success = True
        response.message = "Located trays"

        return response
    
    def update_slots_cb(self, _, response: Trigger.Response) -> Trigger.Response:
        frame = self.get_most_recent_frame()

        if frame is None:
            response.message = "Unable to connect to camera"
            response.success = False
            return response

        self.update_slots(frame)
        
        response.success = True
        response.message = "Updated slots"

        return response
    
    def calibrate_maps_cb(self, _, response: Trigger.Response) -> Trigger.Response:
        # TODO: Generate and save new maps for calibration

        response.success = False
        response.message = "Not yet implemented"

        return response

    def publish(self):
        # Publish Raw Image
        if self.current_frame is not None:
            self.raw_image_pub.publish(self.current_frame)

        # Publish trays info
        if self.trays_info is not None:
            self.trays_info_pub.publish(self.trays_info)

        # Update timestamps and send TF transforms
        for t in self.transforms:
            t.header.stamp = self.get_clock().now().to_msg()
        
        if self.transforms:
            self.tf_broadcaster.sendTransform(self.transforms)

        # Publish annotated image
        # self.detected_trays_image_pub.publish(self.no_gears_msg)

    def get_most_recent_frame(self) -> Optional[MatLike]:
        ret, frame = self.capture.read()

        if not ret:
            self.get_logger().error("Unable to connect to stream")
            return None
        
        return frame

    def rectify_frame(self, frame: MatLike) -> MatLike:
        return cv2.remap(frame, self.map_x, self.map_y, cv2.INTER_CUBIC)[:-30,:-30]
    
    def remove_background(self, frame: MatLike) -> MatLike:
        delta = cv2.subtract(self.base_background, frame)

        gray = cv2.cvtColor(delta, cv2.COLOR_BGR2GRAY)

        _, mask = cv2.threshold(gray, 12, 255, cv2.THRESH_BINARY)

        canvas = np.zeros(gray.shape, dtype=np.uint8)

        contours,_ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        filtered_contours = []
        for c in contours:
            if cv2.contourArea(c) > 40:
                filtered_contours.append(c)

        cv2.drawContours(canvas, filtered_contours, -1, color=255, thickness=cv2.FILLED) # type: ignore

        return cv2.bitwise_and(frame, frame, mask=canvas)
    
    def remove_and_replace_gears(self, frame: MatLike, gear_size="large") -> MatLike:
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # Remove and replace large green gears
        #TODO Add small gears to if statement
        if gear_size == "large":
            hsv_lower = (40, 30, 123)
            hsv_upper = (85, 165, 215)
            radius = 60
        elif gear_size == "medium":
            hsv_lower = (6, 76, 140)
            hsv_upper = (24, 162, 255)
            radius = 44
        else:
            return frame

        gears = cv2.inRange(hsv, hsv_lower, hsv_upper) # type: ignore
    
        contours, _ = cv2.findContours(gears, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        for c in contours:
            if cv2.contourArea(c) < 200:
                continue

            M = cv2.moments(c)
        
            if M["m00"] != 0:
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])

            start_x = int(cX - radius)
            start_y = int(cY - radius)

            cv2.circle(frame, (cX, cY), radius, (0, 0, 0), -1)

            square = frame[start_y:start_y + 2*radius, start_x:start_x + 2*radius]

            _, mask = cv2.threshold(cv2.cvtColor(square, cv2.COLOR_BGR2GRAY), 0, 255, cv2.THRESH_BINARY)

            avg_color = cv2.mean(square, mask)

            cv2.circle(frame, (cX, cY), radius + 3, avg_color, -1)
        
        return frame
            
    def detect_trays(self, table_image: MatLike) -> dict[int, list[tuple[tuple[int, int], float]]]:
        # Determine basic tray info for all trays
        trays_on_table = {
            Tray.SMALL_GEAR_TRAY: [],
            Tray.MEDIUM_GEAR_TRAY: [],
            Tray.LARGE_GEAR_TRAY: [],
            Tray.M2L1_KIT_TRAY: [],
            Tray.S2L2_KIT_TRAY:[]
        }
        
        tray_contours, _ = cv2.findContours(
            cv2.cvtColor(table_image, cv2.COLOR_BGR2GRAY),
            cv2.RETR_EXTERNAL,
            cv2.CHAIN_APPROX_NONE)
        
        for contour in tray_contours:
            # Ignore contours under a size threshold
            if cv2.contourArea(contour) < 200:
                continue

            # Approximate the contour as a polygon
            peri = cv2.arcLength(contour, True)
            poly = cv2.approxPolyDP(contour, 0.01 * peri, True)

            # Fit a rotated rect bounding box to the poly
            rect = cv2.minAreaRect(poly)
            (_, _), (width, height), angle = rect

            if width > height:
                angle = (90 - angle)
            else:
                angle *= -1

            # print(f'height:  {height}  width: {width} angle:  {angle}')      

            aspect_ratio = min(width, height) / max(width, height)

            # Calculate center of contour
            box = cv2.boxPoints(rect)
            box = box.astype(np.int32)

            M = cv2.moments(box)
            if M["m00"] != 0:
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])

            # TODO: Update classifier to handle small gear tray and s2l2 kit tray

            # Use aspect ratio to classify tray type
            if aspect_ratio < 0.6:
                trays_on_table[Tray.LARGE_GEAR_TRAY].append(((cX,cY), angle))                

            elif aspect_ratio < 0.9:
                trays_on_table[Tray.M2L1_KIT_TRAY].append(((cX,cY), angle))
            else:
                trays_on_table[Tray.MEDIUM_GEAR_TRAY].append(((cX,cY), angle))

        # Sort trays_on_table lists based on location
        for identifier, trays in trays_on_table.items():
            if not len(trays) > 1:
                continue

            trays_on_table[identifier] = sorted(trays, key=lambda k: [k[0][1] , k[0][0]])

        return trays_on_table
    
    def determine_tray_info(self, trays_on_table: dict[int, list[tuple[tuple[int, int], float]]]) -> Trays:       
        # Fill the trays info msg
        tray_info = Trays()

        # Clear list of transforms
        self.transforms.clear()

        for identifier, trays in trays_on_table.items():
            for i, ((x,y), angle) in enumerate(trays):
                # Gather info for publishing TF frames
                tray_msg = Tray()
                tray_msg.identifier = identifier
                tray_msg.name = f'{FanucTable.tray_names[identifier]}_{i}'

                tray_center = Point(
                    x=(FanucTable.table_origin.x - (x * self.conversion_factor)) / 1000,
                    y=(FanucTable.table_origin.y + (y * self.conversion_factor)) / 1000, 
                    z=(FanucTable.table_origin.z + self.tray_height)
                )

                theta = math.radians(angle) + math.pi

                # Publish TF frame
                self.transforms.append(self.generate_transform('fanuc_base_link', tray_msg.name, tray_center, theta))
 
                for slot_name, (x_off, y_off) in SlotOffsets.offsets[tray_msg.identifier].items():
                    # Create slot info for each slot
                    slot_info = SlotInfo()
                    slot_info.name = f"{tray_msg.name}_{slot_name}"

                    if "sg" in slot_info.name or identifier == Tray.SMALL_GEAR_TRAY:
                        slot_info.size = SlotInfo.SMALL
                    elif "mg" in slot_info.name or identifier == Tray.MEDIUM_GEAR_TRAY:
                        slot_info.size = SlotInfo.MEDIUM
                    elif "lg" in slot_info.name or identifier == Tray.LARGE_GEAR_TRAY:
                        slot_info.size = SlotInfo.LARGE

                    # Generate tf transform for slot
                    slot_center_tray = Point(
                        x= -x_off,
                        y= y_off, 
                        z= self.gear_height
                    )

                    self.transforms.append(self.generate_transform(tray_msg.name, slot_info.name, slot_center_tray, 0.0))

                    # Store pixel coordinates for center of slot
                
                    x_px = 1000 * (x_off/self.conversion_factor)
                    y_px = 1000 * (y_off/self.conversion_factor)

                    length = math.sqrt(x_px **2 + y_px**2)

                    alpha = math.radians(angle)

                    beta = math.atan2(y_off, x_off)

                    gamma = beta - alpha

                    # print(f'slot: {slot_info.name} alpha: {math.degrees(alpha)} beta: {math.degrees(beta)} gamma: {math.degrees(gamma)}')

                    self.slot_pixel_centers[slot_info.name] = (
                        int(x - length * math.cos(gamma)),
                        int(y - length * math.sin(gamma))
                    )

                    tray_msg.slots.append(slot_info)

                if identifier in FanucTable.part_tray_types:
                    tray_info.part_trays.append(tray_msg)
                elif identifier in FanucTable.kit_tray_types:
                    tray_info.kit_trays.append(tray_msg)   
        
        return tray_info
    
    def update_slots(self, frame: MatLike) -> bool:
        if self.trays_info is None:
            return False
        
        for kit_tray in self.trays_info.kit_trays:
            kit_tray: Tray
            for slot in kit_tray.slots:
                slot: SlotInfo
                slot.occupied = self.check_occupied(frame, slot)

        for part_tray in self.trays_info.part_trays:
            part_tray: Tray
            for slot in part_tray.slots:
                slot: SlotInfo
                slot.occupied = self.check_occupied(frame, slot)

        return True

    def check_occupied(self, orignal_img: MatLike, slot: SlotInfo) -> bool:
        if slot.size == SlotInfo.SMALL:
            #TODO: Add radius and hsv values for small gear
            return False
        elif slot.size == SlotInfo.MEDIUM:
            radius = 42
            hsv_lower = (6, 76, 140)
            hsv_upper = (24, 162, 255)
        elif slot.size == SlotInfo.LARGE:
            radius = 55
            hsv_lower = (40, 30, 123)
            hsv_upper = (85, 165, 215)
            
        start_x = int(self.slot_pixel_centers[slot.name][0] - radius)
        start_y = int(self.slot_pixel_centers[slot.name][1] - radius)

        square = orignal_img[start_y:start_y + 2*radius, start_x:start_x + 2*radius]

        gear = cv2.inRange(cv2.cvtColor(square, cv2.COLOR_BGR2HSV), hsv_lower, hsv_upper) # type: ignore

        if cv2.countNonZero(gear) > 100:
            return True

        return False        
    
    def generate_grid_maps(self, frame: MatLike) -> Optional[MatLike]:
        offset = 15

        # Corners are manually deduced from location of screw heads in table
        top_left = (417 + offset, 180 + offset)
        top_right = (1043 - offset, 179 + offset)
        bottom_right = (1035 - offset, 955 - offset)
        bottom_left = (428 + offset, 944 - offset)

        # Black out everything from image that is not the active region
        fanuc_table_corners = np.array([top_right, bottom_right, bottom_left, top_left])

        maskImage = np.zeros(frame.shape, dtype=np.uint8)
        cv2.drawContours(maskImage, [fanuc_table_corners], 0, (255, 255, 255), -1)

        active_region = cv2.bitwise_and(frame, maskImage)

        # Detect optical table holes 
        blur = cv2.GaussianBlur(active_region,(5,5),0)

        hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)

        threshold = cv2.inRange(hsv, (100, 0, 0), (255, 255, 120)) # type: ignore

        offset = 25

        top_left = (417 + offset, 180 + offset)
        top_right = (1043 - offset, 179 + offset)
        bottom_right = (1035 - offset, 955 - offset)
        bottom_left = (428 + offset, 944 - offset)

        corners = np.array([top_right, bottom_right, bottom_left, top_left])

        mask2 = np.zeros(threshold.shape, dtype=np.uint8)
        
        cv2.drawContours(mask2, [corners], 0, 255, -1) # type: ignore

        just_holes = cv2.bitwise_and(threshold, mask2)

        contours, _ = cv2.findContours(just_holes, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        filtered_contours = []
        for contour in contours:
            area = cv2.contourArea(contour)
            if area >= 10:
                filtered_contours.append(contour)

        rows = 24
        columns = 19

        if not len(filtered_contours) == rows * columns:
            self.get_logger().error("Not able to detect all holes")
            return None

        center_points = []

        for contour in filtered_contours:
            # Calculate moments for each contour
            M = cv2.moments(contour)

            # Calculate center of contour
            if M["m00"] != 0:
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])

                center_points.append((cX, cY))

        center_y = 210
        sorted_points = []
        working_points = []

        for i in range(rows):
            for point in center_points:
                if center_y - 18 <= point[1] <= center_y + 18:
                    working_points.append(point)
                
            sorted_points += sorted(working_points, key=lambda k: [k[0]])

            working_points.clear()

            center_y += 31

        if not len(sorted_points) == len(center_points):
            self.get_logger().error("Not able to properly sort holes")
            return
        
        actual_points = []

        for i in range(rows):
            x = (i * 30)
            for j in range(columns):
                y = (j * 30)
                actual_points.append([x, y])

        grid_x, grid_y = np.mgrid[0:rows*30, 0:columns*30]

        destination = np.array(actual_points)
        source = np.array(sorted_points)

        grid_z = griddata(destination, source, (grid_x, grid_y), method='cubic')
        map_x = np.append([], [ar[:,0] for ar in grid_z]).reshape(rows*30,columns*30)
        map_y = np.append([], [ar[:,1] for ar in grid_z]).reshape(rows*30,columns*30)
        map_x_32 = map_x.astype('float32')
        map_y_32 = map_y.astype('float32')

        np.save("src/vision_testing/map_x.npy", map_x_32)
        np.save("src/vision_testing/map_y.npy", map_y_32)

    def generate_transform(self, parent_frame: str, child_frame: str, pt: Point, rotation: float) -> TransformStamped:
        t = TransformStamped()

        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = parent_frame
        t.child_frame_id = child_frame

        t.transform.translation.x = pt.x
        t.transform.translation.y = pt.y
        t.transform.translation.z = pt.z
        t.transform.rotation = self.quaternion_from_euler(0.0, 0.0, rotation)

        return t
            
    def build_img_msg_from_mat(self, mat: MatLike) -> Image:
        img_msg = Image()
        
        img_msg.height = mat.shape[0]
        img_msg.width = mat.shape[1]

        img_msg.encoding = 'bgr8'

        if mat.dtype.byteorder == '>':
            img_msg.is_bigendian = True
        
        img_msg.data.frombytes(mat.tobytes())
        
        img_msg.step = len(img_msg.data) // img_msg.height

        return img_msg
    
    def quaternion_from_euler(self, roll, pitch, yaw):
        cy = math.cos(yaw * 0.5)
        sy = math.sin(yaw * 0.5)
        cp = math.cos(pitch * 0.5)
        sp = math.sin(pitch * 0.5)
        cr = math.cos(roll * 0.5)
        sr = math.sin(roll * 0.5)

        q = Quaternion()
       
        q.x = cy * cp * sr - sy * sp * cr
        q.y = sy * cp * sr + cy * sp * cr
        q.z = sy * cp * cr - cy * sp * sr
        q.w = cy * cp * cr + sy * sp * sr

        return q




