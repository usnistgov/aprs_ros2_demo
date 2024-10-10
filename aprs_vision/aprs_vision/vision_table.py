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

from aprs_vision.gear_detection import GearDetection
from aprs_interfaces.srv import GenerateGridMaps

from pathlib import Path

from example_interfaces.srv import Trigger
from sensor_msgs.msg import Image
from aprs_interfaces.msg import Trays, Tray, SlotInfo, PixelCenter, SlotPixel, PixelSlotInfo
from aprs_vision.slot_offsets import SlotOffsets
from geometry_msgs.msg import TransformStamped, Point, Quaternion
    
class VisionTable(Node):    
    tray_names = {
        Tray.SMALL_GEAR_TRAY: 'small_gear_tray',
        Tray.MEDIUM_GEAR_TRAY: 'medium_gear_tray',
        Tray.LARGE_GEAR_TRAY: 'large_gear_tray',
        Tray.M2L1_KIT_TRAY: 'm2l1_kit_tray',
        Tray.S2L2_KIT_TRAY: 's2l2_kit_tray'
    }

    kit_tray_types = [Tray.S2L2_KIT_TRAY, Tray.M2L1_KIT_TRAY]
    part_tray_types = [Tray.SMALL_GEAR_TRAY, Tray.MEDIUM_GEAR_TRAY, Tray.LARGE_GEAR_TRAY]

    table_origin: Point
    tray_height: float
    gear_height: float

    video_stream = ''
    map_x_image = ''
    map_y_image = ''
    background_image = ''
    background_threshold: int
    conversion_factor: float
    publish_frames: bool

    top_left_x: int
    top_left_y: int
    bottom_left_x: int
    bottom_left_y: int
    top_right_x: int
    top_right_y: int
    bottom_right_x: int
    bottom_right_y: int
    grid_hsv_lower: tuple[int,int,int]
    grid_hsv_upper: tuple[int,int,int]
    calibrate_rows: int
    calibrate_columns: int
    generate_map_area: float
    angle_offset: float
    suffix: str


    gear_detection_values: dict[int, GearDetection]

    base_frame = ''

    def __init__(self, node_name: str, ns: str):
        super().__init__(node_name, namespace=ns)

        self.capture = cv2.VideoCapture(self.video_stream)

        share_path = get_package_share_directory('aprs_vision')

        self.map_x = np.load(os.path.join(share_path, 'config', self.map_x_image))
        self.map_y = np.load(os.path.join(share_path, 'config', self.map_y_image))
        self.base_background = cv2.imread(os.path.join(share_path, 'config', self.background_image))

        self.slot_pixel_centers: dict[str, tuple[int, int]] = {}
        self.slot_pixel_center_pub: Optional[SlotPixel] = None
        self.current_frame: Optional[MatLike] = None

        # ROS Messages
        self.trays_info: Optional[Trays] = None
        self.current_image_msg: Optional[Image] = None

        # ROS Services
        self.locate_trays_srv = self.create_service(Trigger, 'locate_trays', self.locate_trays_cb)
        self.update_trays_info_srv = self.create_service(Trigger, 'update_slots', self.update_slots_cb)
        self.calibrate_maps_srv = self.create_service(GenerateGridMaps, 'calibrate_maps', self.calibrate_maps_cb)

        # ROS Topics
        self.trays_info_pub = self.create_publisher(Trays, 'trays_info', qos_profile_default)
        self.raw_image_pub = self.create_publisher(Image, 'raw_image', 10)
        self.detected_trays_image_pub = self.create_publisher(Image, 'detected_trays_image', qos_profile_default)
        self.slot_centers_pub = self.create_publisher(SlotPixel,'slot_pixel_centers', qos_profile_default)
    
        # TF Broadcaster
        self.tf_broadcaster = TransformBroadcaster(self)
        self.transforms: list[TransformStamped] = []

        # ROS Timers
        self.publish_timer = self.create_timer(timer_period_sec=1, callback=self.publish)
        self.read_frame_timer = self.create_timer(timer_period_sec=0.1, callback=self.read_frame)

    def locate_trays_cb(self, _, response: Trigger.Response) -> Trigger.Response:
        if self.trays_info is not None:
            self.trays_info = None
            self.slot_pixel_center_pub = None
        
        if self.current_frame is None:
            response.message = "Unable to connect to camera"
            response.success = False
            return response
        
        frame = self.current_frame.copy()
        
        # Save current frame as ROS message for publishing
        self.current_image_msg = self.build_img_msg_from_mat(frame)

        # Function to determine the location of the trays provided an initial image
        frame = self.rectify_frame(frame)

        original_img = frame.copy()

        # Remove table backgroud
        try:
            cv2.imshow('window', frame)
            cv2.waitKey(0)
            frame = self.remove_background(frame)
            cv2.imshow('window', frame)
            cv2.waitKey(0)

        # Remove and replace and large gears present
            frame = self.remove_and_replace_gears(frame, gear_size=SlotInfo.LARGE)
        
        # Remove and replace any medium gears present
            frame = self.remove_and_replace_gears(frame, gear_size=SlotInfo.MEDIUM)

        # # Remove and replace any small gears present
            frame = self.remove_and_replace_gears(frame,gear_size=SlotInfo.SMALL)

            trays_on_table = self.detect_trays(frame)

        # Fill tray info message
            self.trays_info = self.determine_tray_info(trays_on_table)
            self.slot_centers_pub.publish(self.slot_pixel_center_pub)

        # Check if slots are occupied 
            self.update_slots(original_img)

            response.success = True
            response.message = "Located trays"
        except:
            response.success = False
            response.message = "Unable to detect trays properly. Is the robot arm in the way?"
            cv2.imshow('Error Reason', frame)
            cv2.waitKey(0)

        return response
    
    def update_slots_cb(self, _, response: Trigger.Response) -> Trigger.Response:

        if self.current_frame is None:
            response.message = "Unable to connect to camera"
            response.success = False
            return response

        if self.update_slots(self.rectify_frame(self.current_frame)):
        
            response.success = True
            response.message = "Updated slots"
        else:
            response.message = False
            response.message = "Unable to update slots"

        return response
    
    def calibrate_maps_cb(self, request: GenerateGridMaps.Request, response: GenerateGridMaps.Response) -> GenerateGridMaps.Response:
        # TODO: Generate and save new maps for calibration

        if self.current_frame is None:
            response.message = "Unable to connect to camera"
            response.success = False
            return response
        if not os.path.exists(request.filepath):
            response.message = "Provided filepath does not exist"
            response.success = False
            return response

        if self.generate_grid_maps(self.current_frame,request.filepath) is False:
            response.success = False
            response.message = "Unable to Calibrate Map"
        else:
            response.success = True
            response.message = "Map Calibrated"
            if request.save_background_image:
                cv2.imwrite(f'{request.filepath}{self.background_image}',cv2.remap(self.current_frame, self.map_x, self.map_y, cv2.INTER_CUBIC)[:-30,:-30])

        return response

    def publish(self):
        # Publish Raw Image
        if self.current_image_msg is not None:
            self.raw_image_pub.publish(self.current_image_msg)

        # Publish trays info
        if self.trays_info is not None:
            self.trays_info_pub.publish(self.trays_info)
            self.slot_centers_pub.publish(self.slot_pixel_center_pub)

        # Update timestamps and send TF transforms
        for t in self.transforms:
            t.header.stamp = self.get_clock().now().to_msg()
        
        if self.transforms:
            self.tf_broadcaster.sendTransform(self.transforms)

        # Publish annotated image
        # self.detected_trays_image_pub.publish(self.no_gears_msg)

    def read_frame(self):
        ret, current_frame = self.capture.read()

        if ret:
            self.current_frame = current_frame
        else:
            self.current_frame = None

    def rectify_frame(self, frame: MatLike) -> MatLike:
        return cv2.remap(frame, self.map_x, self.map_y, cv2.INTER_CUBIC)[:-30,:-30]
    
    def remove_background(self, frame: MatLike) -> MatLike:
        delta = cv2.subtract(self.base_background, frame)

        gray = cv2.cvtColor(delta, cv2.COLOR_BGR2GRAY)

        _, mask = cv2.threshold(gray, self.background_threshold, 255, cv2.THRESH_BINARY)

        canvas = np.zeros(gray.shape, dtype=np.uint8)

        contours,_ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        
        filtered_contours = []
        for c in contours:
            if cv2.contourArea(c) > 100:
                filtered_contours.append(c)

        cv2.drawContours(canvas, filtered_contours, -1, color=255, thickness=cv2.FILLED) # type: ignore

        return cv2.bitwise_and(frame, frame, mask=canvas)
    
    def remove_and_replace_gears(self, frame: MatLike, gear_size: int) -> MatLike:
        detection_params = self.gear_detection_values[gear_size]

        # cv2.imwrite(f"gear_image{gear_size}", frame)

        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        gears = cv2.inRange(hsv, detection_params.hsv_lower, detection_params.hsv_upper) # type: ignore
    
        contours, _ = cv2.findContours(gears, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        for c in contours:
            if cv2.contourArea(c) < 200:
                continue

            M = cv2.moments(c)
        
            if M["m00"] != 0:
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])

            start_x = int(cX - detection_params.px_radius)
            start_y = int(cY - detection_params.px_radius)

            cv2.circle(frame, (cX, cY), detection_params.px_radius, (0, 0, 0), -1)

            square = frame[start_y:start_y + 2*detection_params.px_radius, start_x:start_x + 2*detection_params.px_radius]

            _, mask = cv2.threshold(cv2.cvtColor(square, cv2.COLOR_BGR2GRAY), 0, 255, cv2.THRESH_BINARY)

            avg_color = cv2.mean(square, mask)

            cv2.circle(frame, (cX, cY), detection_params.px_radius + 3, avg_color, -1)
        
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
                if angle > 0:
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

            # Use aspect ratio to classify tray type
            if aspect_ratio < 0.6:
                trays_on_table[Tray.LARGE_GEAR_TRAY].append(((cX,cY), angle))              

            elif aspect_ratio < 0.8:
                if abs(angle-90) >= 90:
                    angle = angle + 90
                else:
                    angle = angle - 90
                trays_on_table[Tray.S2L2_KIT_TRAY].append(((cX,cY), angle))

            elif aspect_ratio < 0.9:
                trays_on_table[Tray.M2L1_KIT_TRAY].append(((cX,cY), angle))

            else:
                if (height*width > 27000):
                    trays_on_table[Tray.MEDIUM_GEAR_TRAY].append(((cX,cY), angle))
                else:
                    trays_on_table[Tray.SMALL_GEAR_TRAY].append(((cX,cY), angle))

        # Sort trays_on_table lists based on location
        for identifier, trays in trays_on_table.items():
            if not len(trays) > 1:
                continue

            trays_on_table[identifier] = sorted(trays, key=lambda k: [k[0][0] , k[0][1]])

        return trays_on_table
    
    def determine_tray_info(self, trays_on_table: dict[int, list[tuple[tuple[int, int], float]]]) -> Trays:       
        # Clear list of transforms
        self.transforms.clear()
        
        # Fill the trays info msg
        tray_info = Trays()
        pixel_tray_info = SlotPixel()
        image_base = f'image_base_{self.suffix}'
        image_center = Point(
            x=self.table_origin.x/1000,
            y=self.table_origin.y/1000,
            z=self.table_origin.z/1000,
        )
        self.transforms.append(self.generate_transform(self.base_frame, image_base, image_center, 0.0, math.pi, self.angle_offset))

        for identifier, trays in trays_on_table.items():
            for i, ((x,y), angle) in enumerate(trays):
                # Gather info for publishing TF frames
                tray_msg = Tray()
                tray_msg.identifier = identifier
                tray_msg.name = f'{VisionTable.tray_names[identifier]}_{i}'

                pixel_tray_msg = PixelCenter()
                pixel_tray_msg.identifier = identifier
                pixel_tray_msg.name = f'{VisionTable.tray_names[identifier]}_{i}'
                pixel_tray_msg.x = x
                pixel_tray_msg.y = y
                pixel_tray_msg.angle = angle

                if self.publish_frames:
                    tray_center = Point(
                        x=(x * self.conversion_factor) / 1000,
                        y=(y * self.conversion_factor) / 1000, 
                        z=self.tray_height
                    )

                    theta = math.radians(-angle) + math.pi

                    # Publish TF frame
                    tray_frame_name = f'{tray_msg.name}_{self.suffix}'
                    self.transforms.append(self.generate_transform(image_base, tray_frame_name, tray_center, 0.0, math.pi, theta))
 
                for slot_name, (x_off, y_off) in SlotOffsets.offsets[tray_msg.identifier].items():
                    # Create slot info for each slot
                    slot_info = SlotInfo()
                    pixel_slot_info = PixelSlotInfo()
                    slot_info.name = f"{tray_msg.name}_{slot_name}"
                    pixel_slot_info.name = f"{tray_msg.name}_{slot_name}"

                    if "sg" in slot_info.name or identifier == Tray.SMALL_GEAR_TRAY:
                        slot_info.size = SlotInfo.SMALL
                        pixel_slot_info.size = SlotInfo.SMALL
                    elif "mg" in slot_info.name or identifier == Tray.MEDIUM_GEAR_TRAY:
                        slot_info.size = SlotInfo.MEDIUM
                        pixel_slot_info.size = SlotInfo.MEDIUM
                    elif "lg" in slot_info.name or identifier == Tray.LARGE_GEAR_TRAY:
                        slot_info.size = SlotInfo.LARGE
                        pixel_slot_info.size = SlotInfo.LARGE

                    # Generate tf transform for slot
                    if self.publish_frames:
                        slot_center_tray = Point(
                            x= x_off,
                            y= y_off, 
                            z= self.gear_height
                        )
                        slot_frame_name = f'{slot_info.name}_{self.suffix}'
                        self.transforms.append(self.generate_transform(tray_frame_name, slot_frame_name, slot_center_tray, 0.0, 0.0, 0.0))

                    # Store pixel coordinates for center of slot
                
                    x_px = 1000 * (x_off/self.conversion_factor)
                    y_px = 1000 * (y_off/self.conversion_factor)

                    length = math.sqrt(x_px **2 + y_px**2)

                    alpha = math.radians(angle)

                    beta = math.atan2(y_off, -x_off)

                    gamma = beta - alpha

                    # print(f'slot: {slot_info.name} alpha: {math.degrees(alpha)} beta: {math.degrees(beta)} gamma: {math.degrees(gamma)}')

                    self.slot_pixel_centers[slot_info.name] = (
                        int(x - length * math.cos(gamma)),
                        int(y - length * math.sin(gamma))
                    )
                    pixel_slot_info.slot_center_x = int(x - length * math.cos(gamma))
                    pixel_slot_info.slot_center_y = int(y - length * math.sin(gamma))

                    tray_msg.slots.append(slot_info)
                    pixel_tray_msg.slots.append(pixel_slot_info)

                if identifier in VisionTable.part_tray_types:
                    tray_info.part_trays.append(tray_msg)
                    pixel_tray_info.part_trays.append(pixel_tray_msg)
                elif identifier in VisionTable.kit_tray_types:
                    tray_info.kit_trays.append(tray_msg)
                    pixel_tray_info.kit_trays.append(pixel_tray_msg)   
            self.slot_pixel_center_pub = pixel_tray_info
        
        return tray_info
    
    def update_slots(self, frame: MatLike) -> bool:
        if self.trays_info is None:
            return False
        
        for kit_tray in self.trays_info.kit_trays:
            kit_tray: Tray
            print(kit_tray.name)
            for slot in kit_tray.slots:
                slot: SlotInfo
                slot.occupied = self.check_occupied(frame, slot)

        for part_tray in self.trays_info.part_trays:
            part_tray: Tray
            print(part_tray.name)
            for slot in part_tray.slots:
                slot: SlotInfo
                slot.occupied = self.check_occupied(frame, slot)

        return True

    def check_occupied(self, original_img: MatLike, slot: SlotInfo) -> bool:
        detection_params = self.gear_detection_values[slot.size]
            
        start_x = int(self.slot_pixel_centers[slot.name][0] - detection_params.px_radius)
        start_y = int(self.slot_pixel_centers[slot.name][1] - detection_params.px_radius)

        square = original_img[start_y:start_y + 2*detection_params.px_radius, start_x:start_x + 2*detection_params.px_radius]

        gear = cv2.inRange(cv2.cvtColor(square, cv2.COLOR_BGR2HSV), detection_params.hsv_lower, detection_params.hsv_upper) # type: ignore
        if cv2.countNonZero(gear) > detection_params.px_area:
            return True

        return False   
         
    # TODO: Fix
    def generate_grid_maps(self, frame: MatLike, filepath: str) -> Optional[bool]:
        offset = 15

        # Corners are manually deduced from location of screw heads in table
        top_left = (self.top_left_x + offset, self.top_left_y + offset)
        top_right = (self.top_right_x - offset, self.top_right_y + offset)
        bottom_right = (self.bottom_right_x - offset, self.bottom_right_y - offset)
        bottom_left = (self.bottom_left_x + offset, self.bottom_left_y - offset)

        # Black out everything from image that is not the active region
        fanuc_table_corners = np.array([top_right, bottom_right, bottom_left, top_left])

        maskImage = np.zeros(frame.shape, dtype=np.uint8)
        cv2.drawContours(maskImage, [fanuc_table_corners], 0, (255, 255, 255), -1)

        active_region = cv2.bitwise_and(frame, maskImage)

        # Detect optical table holes 
        blur = cv2.GaussianBlur(active_region,(5,5),0)

        hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)

        cv2.imwrite('hsv.jpg', hsv)

        threshold = cv2.inRange(hsv, self.grid_hsv_lower, self.grid_hsv_upper) # type: ignore

        offset = 25

        top_left = (self.top_left_x + offset, self.top_left_y + offset)
        top_right = (self.top_right_x - offset, self.top_right_y + offset)
        bottom_right = (self.bottom_right_x - offset, self.bottom_right_y - offset)
        bottom_left = (self.bottom_left_x + offset, self.bottom_left_y - offset)

        corners = np.array([top_right, bottom_right, bottom_left, top_left])

        mask2 = np.zeros(threshold.shape, dtype=np.uint8)
        
        cv2.drawContours(mask2, [corners], 0, 255, -1) # type: ignore

    
        just_holes = cv2.bitwise_and(threshold, mask2)

        contours, _ = cv2.findContours(just_holes, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        filtered_contours = []
        for contour in contours:
            area = cv2.contourArea(contour)
            if area >= self.generate_map_area:
                filtered_contours.append(contour)

        rows = self.calibrate_rows
        columns = self.calibrate_columns

        # cv2.drawContours(just_holes,filtered_contours,-1,120,2)
        # cv2.imshow('window', just_holes)
        # cv2.waitKey(0)

        if not len(filtered_contours) == rows * columns:
            self.get_logger().error("Not able to detect all holes")
            return False

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

            center_y += 25

        if not len(sorted_points) == len(center_points):
            self.get_logger().error("Not able to properly sort holes")
            return False
        
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

        np.save(f"{filepath}{self.map_x_image}", map_x_32)
        np.save(f"{filepath}{self.map_y_image}", map_y_32)

        return True
    def generate_transform(self, parent_frame: str, child_frame: str, pt: Point, roll: float, pitch: float, yaw: float) -> TransformStamped:
        t = TransformStamped()

        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = parent_frame
        t.child_frame_id = child_frame

        t.transform.translation.x = pt.x
        t.transform.translation.y = pt.y
        t.transform.translation.z = pt.z
        t.transform.rotation = self.quaternion_from_euler(roll, pitch, yaw)

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




