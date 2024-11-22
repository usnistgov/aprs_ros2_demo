#!/usr/bin/env python3

import os
import cv2
import math
import numpy as np
from numpy.lib.npyio import NpzFile
from scipy.interpolate import griddata

from cv2.typing import MatLike
from typing import Optional

from rclpy.node import Node
from rclpy.qos import qos_profile_default

from ament_index_python.packages import get_package_share_directory

from tf2_ros.transform_broadcaster import TransformBroadcaster

from aprs_vision.gear_detection import GearDetection
from aprs_vision.calibration_tool import CalibrationTool
from aprs_interfaces.srv import GenerateGridMaps, LocateTrays

from example_interfaces.srv import Trigger
from sensor_msgs.msg import Image
from aprs_interfaces.msg import Trays, Tray, SlotInfo, PixelCenter, SlotPixel, PixelSlotInfo
from aprs_vision.slot_offsets import SlotOffsets
from geometry_msgs.msg import TransformStamped, Point, Quaternion, PoseStamped
    
class VisionTable(Node):    
    tray_names = {
        Tray.SMALL_GEAR_TRAY: 'small_gear_tray',
        Tray.MEDIUM_GEAR_TRAY: 'medium_gear_tray',
        Tray.LARGE_GEAR_TRAY: 'large_gear_tray',
        Tray.M2L1_KIT_TRAY: 'm2l1_kit_tray',
        Tray.S2L2_KIT_TRAY: 's2l2_kit_tray'
    }

    tray_areas = {
        Tray.SMALL_GEAR_TRAY: 24827,
        Tray.MEDIUM_GEAR_TRAY: 33823,
        Tray.LARGE_GEAR_TRAY: 28207,
        Tray.S2L2_KIT_TRAY: 36986,
        Tray.M2L1_KIT_TRAY: 29865
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

    grid_hsv_lower: tuple[int,int,int]
    grid_hsv_upper: tuple[int,int,int]
    calibrate_rows: int
    calibrate_columns: int
    generate_map_area: float
    angle_offset: float
    suffix: str
    vision_location: str

    # Variables Consistent For All Locations
    top_left_x = 0
    top_left_y = 0
    bottom_left_x = 0
    bottom_left_y = 0
    top_right_x = 0
    top_right_y = 0
    bottom_right_x = 0
    bottom_right_y = 0

    clicked = 0
    refpt = []
    rows = []
    columns = []
    pixels_in_one_inch = 30

    counter = 0
    erase_counter = 0
    input_mode = 0
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_origin = (20,30)
    font_scale = 1
    font_color = (255,255,255)
    font_thickness = 2


    gear_detection_values: dict[int, GearDetection]

    base_frame = ''

    def __init__(self, node_name: str, ns: str):
        super().__init__(node_name, namespace=ns)

        self.capture = cv2.VideoCapture(self.video_stream)
        self.aruco_detector = cv2.aruco.ArucoDetector(cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_5X5_50))

        share_path = get_package_share_directory('aprs_vision')

        # self.map_x = np.load(os.path.join(share_path, 'config', self.map_x_image))
        # self.map_y = np.load(os.path.join(share_path, 'config', self.map_y_image))
        calibration_filepath = os.path.join(share_path, 'config', f'{self.suffix}_calibration.npz')
        if os.path.exists(calibration_filepath):
            calibration_file_load:NpzFile = np.load(calibration_filepath)
        else:
            self.calibration_necessary = True
        self.base_background = cv2.imread(os.path.join(share_path, 'config', self.background_image))

        self.slot_pixel_centers: dict[str, tuple[int, int]] = {}
        self.current_frame: Optional[MatLike] = None

        # ROS Messages
        self.trays_info: Optional[Trays] = None
        self.current_image_msg: Optional[Image] = None

        # ROS Services
        self.locate_trays_srv = self.create_service(LocateTrays, f'{self.vision_location}/locate_trays', self.locate_trays_cb)
        self.update_trays_info_srv = self.create_service(Trigger, f'{self.vision_location}/update_slots', self.update_slots_cb)
        self.calibrate_maps_srv = self.create_service(GenerateGridMaps, f'{self.vision_location}/calibrate_maps', self.calibrate_maps_cb)

        # ROS Topics
        self.trays_info_pub = self.create_publisher(Trays, f'{self.vision_location}/trays_info', qos_profile_default)
        self.raw_image_pub = self.create_publisher(Image, f'{self.vision_location}/raw_image', 10)
        self.detected_trays_image_pub = self.create_publisher(Image, f'{self.vision_location}/detected_trays_image', qos_profile_default)
    
        # TF Broadcaster
        self.tf_broadcaster = TransformBroadcaster(self)
        self.transforms: list[TransformStamped] = []

        # ROS Timers
        self.publish_timer = self.create_timer(timer_period_sec=1, callback=self.publish)
        self.read_frame_timer = self.create_timer(timer_period_sec=0.1, callback=self.read_frame)

    def locate_trays_cb(self, request: LocateTrays.Request, response: LocateTrays.Response) -> LocateTrays.Response:
        if self.trays_info is not None:
            if request.overwrite:
                self.trays_info = None
            else: 
                response.message = "Trays Already Found. Unable to perform without overwriting current data!"
                response.success = False
                return response
        
        if self.current_frame is None:
            response.message = "Unable to connect to camera"
            response.success = False
            return response
        
        frame = self.current_frame.copy()

        # Function to determine the location of the trays provided an initial image
        original_img = frame.copy()

        frame = self.remove_background(frame)
        self.trays_info = self.detect_trays(frame)

        response.success = True
        response.message = "Located trays"


        # try:
        # # Remove table backgroud
        #     frame = self.remove_background(frame)

        # # # Remove and replace and large gears present
        # #     frame = self.remove_and_replace_gears(frame, gear_size=SlotInfo.LARGE)
        
        # # # Remove and replace any medium gears present
        # #     frame = self.remove_and_replace_gears(frame, gear_size=SlotInfo.MEDIUM)

        # # # # Remove and replace any small gears present
        # #     frame = self.remove_and_replace_gears(frame,gear_size=SlotInfo.SMALL)

        #     self.trays_info = self.detect_trays(frame)

        # # # Check if slots are occupied 
        # #     self.update_slots(original_img)

        #     response.success = True
        #     response.message = "Located trays"
        # except Exception as e:
        #     response.success = False
        #     response.message = f"Unable to detect trays properly. Exception: ({e})" 
            # cv2.imshow('Error Reason', frame)
            # cv2.waitKey(0)

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

        if self.full_frame is None:
            response.message = "Unable to connect to camera"
            response.success = False
            return response
        if not os.path.exists(request.filepath):
            response.message = "Provided filepath does not exist"
            response.success = False
            return response
        
        tool = CalibrationTool(self.full_frame)
        ret = tool.run_calibration()
        if ret is None:
            response.success = False
            response.message = "Unable to Calibrate Map"
            return response

        self.rotation, self.start_corner, self.end_corner, self.map_x, self.map_y = ret

        calibration_file_name = os.path.join(request.filepath, f'{self.suffix}_calibration')

        np.savez(
            calibration_file_name, 
            rotation=np.array(self.rotation), 
            start_corner=self.start_corner, 
            end_corner=self.end_corner,
            map_x=self.map_x,
            map_y=self.map_y
        )

        response.success = True
        response.message = "Map Calibrated"
        if request.save_background_image:
            cv2.imwrite(f'{request.filepath}{self.background_image}',self.rectify_frame(self.full_frame))

        return response

    def publish(self):
        # Publish Raw Image
        if self.current_image_msg is not None:
            self.raw_image_pub.publish(self.current_image_msg)

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

    def read_frame(self):
        ret, self.current_frame = self.capture.read()

        if ret:
            self.full_frame = self.current_frame.copy()
            # self.current_frame = self.rectify_frame(self.current_frame)
        # Save current frame as ROS message for publishing
            self.current_image_msg = self.build_img_msg_from_mat(self.current_frame)
        else:
            self.current_frame = None

    def rectify_frame(self, frame: MatLike) -> MatLike:
        if not self.rotation == 0:
            rotation_matrix = cv2.getRotationMatrix2D((frame.shape[1] / 2, frame.shape[0] / 2), self.rotation, 1)
            frame = cv2.warpAffine(frame, rotation_matrix, (frame.shape[1], frame.shape[0]))
        offset = -10
        
        frame = frame[
            self.start_corner[1] - offset:self.end_corner[1] + offset,
            self.start_corner[0] - offset:self.end_corner[0] + offset
        ]

        frame = cv2.remap(frame, self.map_x, self.map_y, cv2.INTER_CUBIC)

        return frame
    
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
        if (gear_size == SlotInfo.LARGE):
            cv2.imwrite('hsv.jpg',hsv)

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

    def classify_tray(self,contour: MatLike,image: MatLike)-> tuple[int, int, tuple[float, float], float]:
        """Takes in a contour of a tray and the rectified image of the workspace and returns the tray identifier, as well as the 
        tray id, center and rotation of the tray based on the aruco marker.

        Args:
            contour (MatLike): Tray contour
            image (MatLike): Rectified Image

        Raises:
            Exception: _description_
            Exception: _description_

        Returns:
            tuple[int, int, tuple[float, float], float]: identifier, id, center, rotation(radians)
        """
        # Use contours and mask to isolate tray in image
        canvas = np.zeros((image.shape[0], image.shape[1]), dtype=np.uint8)
        cv2.drawContours(canvas, [contour], -1, color=255, thickness=-1) # type: ignore
        just_contour = cv2.bitwise_and(image, image, mask=canvas)

        # Look for fiducial marker and check to make sure one and only one is found
        corners, marker_ids, _ = self.aruco_detector.detectMarkers(just_contour)

        if marker_ids is None:
            raise Exception("Detected tray without fiducial")            

        if len(marker_ids) > 1:
            raise Exception("Detected multiple fiducials on one tray")
        
        tray_id = marker_ids[0][0]
        
        corners = np.reshape(corners[0],(4,2))

        # Calculate marker center and angle of rotation
        marker_center = (corners[0][0] + (corners[2][0]-corners[0][0])/2, corners[0][1] + (corners[2][1]-corners[0][1])/2)
        angle = math.atan2(corners[0][1] - corners[1][1],corners[0][0]-corners[1][0])

        # Use area to classify tray identifier
        tray_area = cv2.contourArea(contour) * self.conversion_factor**2

        identifier = min({k: abs(v - tray_area) for (k,v) in self.tray_areas.items()}.items(), key=lambda x: x[1])[0]

        return (identifier, tray_id, marker_center, angle)

    def detect_trays(self, table_image: MatLike) -> Trays:       
        # Clear list of transforms
        self.transforms.clear()

        # Publish transform from robot base to image base        
        image_base_frame = f'image_base_{self.suffix}'
        if self.publish_frames:
            image_origin = Point(
                x=self.table_origin.x/1000,
                y=self.table_origin.y/1000,
                z=self.table_origin.z/1000,
            )

            self.transforms.append(
                self.generate_transform(self.base_frame, image_base_frame, image_origin, 0.0, math.pi, self.angle_offset)
            )

        # Detect contours
        tray_contours, _ = cv2.findContours(
            cv2.cvtColor(table_image, cv2.COLOR_BGR2GRAY),
            cv2.RETR_EXTERNAL,
            cv2.CHAIN_APPROX_NONE)

        # Build trays_info message
        trays_info = Trays()  
        for contour in tray_contours:
            # Ignore small contours
            if cv2.contourArea(contour) < 200:
                continue

            # Classify tray based on contours
            identifier, tray_id, (x, y), theta = self.classify_tray(contour,table_image)

            # Build tray message
            tray_msg = Tray()
            tray_msg.identifier = identifier
            tray_msg.name = f'{self.tray_names[identifier]}_{tray_id:02}'

            tray_center = Point(
                x=(x * self.conversion_factor) / 1000,
                y=(y * self.conversion_factor) / 1000, 
                z=-self.tray_height
            )

            tray_msg.tray_pose = self.generate_pose(image_base_frame, tray_center, 0.0, math.pi, theta)

            # Publish TF frame from image base frame to tray fiducial center
            if self.publish_frames:
                self.transforms.append(
                    self.generate_transform(image_base_frame, tray_msg.name, tray_center, 0.0, math.pi, theta)
                )
            
            # Build slot messages
            for slot_name, (x_off, y_off) in SlotOffsets.offsets[tray_msg.identifier].items():
                tray_msg.slots.append(
                    self.build_slot_info_message(tray_msg.identifier, tray_msg.name, slot_name, x_off, y_off)
                )

        # Adds tray_msg to corresponding list in tray_info    
        if identifier in VisionTable.part_tray_types:
            trays_info.part_trays.append(tray_msg)
        elif identifier in VisionTable.kit_tray_types:
            trays_info.kit_trays.append(tray_msg)

        return trays_info

    def build_slot_info_message(self, identifier: int, tray_name: str, slot_name: str, x_off: float, y_off: float):
        # Create slot info for each slot
        slot_info = SlotInfo()
        slot_info.name = f"{tray_name}_{slot_name}"

        if "sg" in slot_info.name or identifier == Tray.SMALL_GEAR_TRAY:
            slot_info.size = SlotInfo.SMALL
        elif "mg" in slot_info.name or identifier == Tray.MEDIUM_GEAR_TRAY:
            slot_info.size = SlotInfo.MEDIUM
        elif "lg" in slot_info.name or identifier == Tray.LARGE_GEAR_TRAY:
            slot_info.size = SlotInfo.LARGE

        # Generate tf transform for slot
        slot_center = Point(
            x= x_off,
            y= y_off, 
            z= self.gear_height
        )
        slot_info.slot_pose = self.generate_pose(tray_name, slot_center, 0.0, 0.0, 0.0)
        if self.publish_frames:
            self.transforms.append(self.generate_transform(tray_name, slot_info.name, slot_center, 0.0, 0.0, 0.0))


        return slot_info
    
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
         
    def generate_grid_maps(self, frame: MatLike, filepath: str) -> Optional[bool]:
        # The point for calibrate trays for the fanuc conveyor system lines up with the tape on the table

        # Create Copy for Calibrating and then Create Black Box for Displaying Instructions
        self.calibration_image = frame.copy()
        self.original_copy = frame.copy()
        self.calibration_image[0:40,0:1000] = (0,0,0)

        # Provide First Step Instruction
        self.text_over_image('Select Top Left Point', self.calibration_image)

        # Create Window and allow mouse callback for selecting four corners
        while True:
            cv2.namedWindow('window', cv2.WINDOW_NORMAL)
            cv2.resizeWindow('window', 1000, 1000)
            cv2.imshow('window',self.calibration_image)
            cv2.createTrackbar("Angle", "window", 0, 30, self.slider_rotation)
            cv2.setTrackbarMin('Angle','window', -30)
            cv2.setMouseCallback('window',self.click_input_on_image)
            key = cv2.waitKey(0) & 0xFF
            if key == ord('r'):
                self.refpt.clear()
                self.calibration_image = self.original_copy
                self.calibration_image[0:40,0:1000] = (0,0,0)
                self.text_over_image('Select Top Left Point', self.calibration_image)
                cv2.imshow('window', self.calibration_image)
                self.counter = 0
            elif  key == ord('c'):
                break

        # Assign output from mouse callback to easily callable variables
        top_left_x = self.refpt[0][0]
        top_left_y = self.refpt[0][1]
        top_right_x = self.refpt[1][0]
        top_right_y = self.refpt[1][1]
        bottom_right_x = self.refpt[2][0]
        bottom_right_y = self.refpt[2][1]

        angle = math.atan2(top_right_y-top_left_y,top_right_x - top_left_x)
        (h,w) = frame.shape[:2]
        (cX, cY) = (w // 2, h // 2)
        M_rot = cv2.getRotationMatrix2D((cX,cY), math.degrees(angle), 1.0)
        rotated = cv2.warpAffine(frame, M_rot, (w,h))

        print(angle)

        cv2.imshow('window', rotated)
        cv2.waitKey(0)

        self.refpt.clear()

        offset = 5

        # Create corners points using previous values
        top_left = (top_left_x + offset, top_left_y + offset)
        top_right = (top_right_x - offset, top_right_y + offset)
        bottom_right = (bottom_right_x - offset, bottom_right_y - offset)

        # Black out everything from image that is not the active region
        fanuc_table_corners = np.array([top_right, bottom_right, (top_left_x, bottom_right_y), top_left])

        maskImage = np.zeros(frame.shape, dtype=np.uint8)
        cv2.drawContours(maskImage, [fanuc_table_corners], 0, (255, 255, 255), -1)

        active_region = cv2.bitwise_and(frame, maskImage)

        # Detect optical table holes 
        blur = cv2.GaussianBlur(active_region,(5,5),0)

        hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)

        cv2.imwrite('hsv.jpg', hsv)

        threshold = cv2.inRange(hsv, self.grid_hsv_lower, self.grid_hsv_upper) # type: ignore

        corners = np.array([top_right, bottom_right, (top_left_x, bottom_right_y), top_left])

        mask2 = np.zeros(threshold.shape, dtype=np.uint8)
        
        cv2.drawContours(mask2, [corners], -1, 255, -1) # type: ignore
    
        just_holes = cv2.bitwise_and(threshold, mask2)
        cv2.drawContours(just_holes,[corners],-1,0,2) # type: ignore

        self.just_holes_calibration = just_holes.copy()
        self.text_over_image('Select Points to Erase using middle mouse button',self.just_holes_calibration)
        self.just_holes_original = self.just_holes_calibration

        # Mouse callback on window to allow for a right-click drag to remove any excess points
        cv2.namedWindow('window', cv2.WINDOW_GUI_NORMAL)
        cv2.setMouseCallback('window',self.draw_box_on_image)
        cv2.createTrackbar('Minimum Size of Contours', 'window', round(self.generate_map_area), 30, self.slider_change)
        cv2.imshow('window', self.just_holes_calibration)
        cv2.waitKey(0)

        # Save adjusted image into just_holes and remove any lingering instructions. Add new instruction for next callback

        just_holes = self.just_holes_original
        just_holes[0:40,0:1000] = 0
        self.text_over_image('Please Select a Single Column',just_holes)

        # Mouse callback for selecting a single column
        while True:
            cv2.setMouseCallback('window', self.select_column)
            cv2.imshow('window', just_holes)
            key = cv2.waitKey(0) & 0xFF
            if key == ord('r'):
                self.rows.clear()
                self.text_over_image('Please Select a Single Column', just_holes)
                cv2.imshow('window', just_holes)
            elif key == ord('c'):
                break

        just_holes[0:40,0:1000] = 0
        self.text_over_image('Please Select a Single Row', just_holes)

        # Mouse callback for selecting a single row
        while True:
            cv2.imshow('window', just_holes)
            cv2.setMouseCallback('window', self.select_row)
            key = cv2.waitKey(0) & 0xFF
            if key == ord('r'):
                self.columns.clear()
                self.text_over_image('Please Select a Single Row', self.just_holes_calibration)
                cv2.imshow('window', self.just_holes_calibration)
            elif key == ord('c'):
                break
            
        cv2.destroyAllWindows()
        just_holes[0:40,0:1000] = 0

        # Find all contours, all contours within the selected column, and all contours within the selected row
        self.contours, _ = cv2.findContours(just_holes, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        rows_contours, _ = cv2.findContours(just_holes[self.rows[0][1]:self.rows[1][1],self.rows[0][0]:self.rows[1][0]], cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        columns_contours, _ = cv2.findContours(just_holes[self.columns[0][1]:self.columns[1][1],self.columns[0][0]:self.columns[1][0]], cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        # Filter Contours Based on Area
        total_contours=0
        filtered_contours = []
        for contour in self.contours:
            area = cv2.contourArea(contour)
            if area >= self.generate_map_area:
                filtered_contours.append(contour)
                total_contours = total_contours+1

        filtered_row_contours = []
        for contour in rows_contours:
            area = cv2.contourArea(contour)
            if area >= self.generate_map_area:
                filtered_row_contours.append(contour)

        filtered_column_contours = []
        for contour in columns_contours:
            area = cv2.contourArea(contour)
            if area >= self.generate_map_area:
                filtered_column_contours.append(contour)


        center_points = []
        # Calculate centerpoints for every detected contour
        for contour in filtered_contours:
            # Calculate moments for each contour
            M = cv2.moments(contour)

            # Calculate center of contour
            if M["m00"] != 0:
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])

                center_points.append((cX, cY))

        # Calculate centerpoints for the detected contours in the first column
        row_points = []
        for row in filtered_row_contours:
            # Calculate moments for each contour
            M = cv2.moments(row)

            # Calculate center of contour
            if M["m00"] != 0:
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])

                row_points.append((cX, cY))
        row_points.sort(key=lambda point: (point[1]))

        # Count the total number of rows and columns based on detected contours
        rows = len(filtered_row_contours)
        columns = len(filtered_column_contours)

        print(f"Found: {total_contours}  Rows: {rows} Columns: {columns}")

        # Check to see if the found number of contours matches the expected number of contours
        if not len(filtered_contours) == rows * columns:
            self.get_logger().error("Not able to detect all holes")
            return False


        # Loop through each detected row and sort points in each row from left to right
        sorted_points = []

        for i in range(rows):
            working_points = []
            if i != rows-1:
                dist = (row_points[i+1][1] - row_points[i][1])/2
            else:
                dist = (row_points[i][1] - row_points[i-1][1])/2

            for point in center_points:
                if row_points[i][1] - dist + self.rows[0][1]< point[1] < row_points[i][1] + dist + self.rows[0][1]:
                    working_points.append(point)
                     
            sorted_points += sorted(working_points, key=lambda k: [k[0]])

        # Check to see if all points were sorted
        if not len(sorted_points) == len(center_points):
            self.get_logger().error("Not able to properly sort holes")
            return False
        
        actual_points = []

        # Create grid, assigning each hole distance a certain number of pixels
        for i in range(rows):
            x = (i * self.pixels_in_one_inch)
            for j in range(columns):
                y = (j * self.pixels_in_one_inch)
                actual_points.append([x, y])

        grid_x, grid_y = np.mgrid[0:rows*self.pixels_in_one_inch, 0:columns*self.pixels_in_one_inch]

        destination = np.array(actual_points)
        source = np.array(sorted_points)

        grid_z = griddata(destination, source, (grid_x, grid_y), method='cubic')
        map_x = np.append([], [ar[:,0] for ar in grid_z]).reshape(rows*self.pixels_in_one_inch,columns*self.pixels_in_one_inch)
        map_y = np.append([], [ar[:,1] for ar in grid_z]).reshape(rows*self.pixels_in_one_inch,columns*self.pixels_in_one_inch)
        self.map_x = map_x.astype('float32')
        self.map_y = map_y.astype('float32')

        np.save(f"{filepath}{self.map_x_image}", self.map_x)
        np.save(f"{filepath}{self.map_y_image}", self.map_y)

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
    
    def generate_pose(self, parent_frame: str, pt: Point, roll: float, pitch: float, yaw: float) -> PoseStamped:
        p = PoseStamped()

        p.header.stamp = self.get_clock().now().to_msg()
        p.header.frame_id = parent_frame

        p.pose.position.x = pt.x
        p.pose.position.y = pt.y
        p.pose.position.z = pt.z
        p.pose.orientation = self.quaternion_from_euler(roll, pitch, yaw)

        return p
            
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
    
    def click_input_on_image(self,event,x,y,flags,param):
        if event == cv2.EVENT_LBUTTONDOWN:
            # print(f"The point selected is x: {x}, y: {y}")
            self.refpt.append((x,y))
            self.calibration_image[0:40,0:1000] = (0,0,0)
            if self.erase_counter == 0:
                pass
            elif self.erase_counter % 2 == 0:
                self.just_holes_calibration[0:40,0:1000] = 0
                self.text_over_image('Select Top Left of Point to Erase', self.just_holes_calibration)
                cv2.imshow('window', self.just_holes_calibration)
                self.erase_counter = self.erase_counter + 1
            else:
                self.just_holes_calibration[0:40,0:1000] = 0
                self.text_over_image('Select Bottom Right of Point to Erase', self.just_holes_calibration)
                cv2.imshow('window', self.just_holes_calibration)
                self.erase_counter = self.erase_counter + 1
            if self.counter == 0:
                cv2.circle(self.calibration_image, (x,y), 10, (255,255,255), -1)
                self.text_over_image('Select Top Right Point or press r to reset', self.calibration_image)
                cv2.imshow('window', self.calibration_image)
            elif self.counter == 1:
                cv2.circle(self.calibration_image, (x,y), 10, (255,255,255), -1)
                self.text_over_image('Select Bottom Right Point or press r to reset', self.calibration_image)
                cv2.imshow('window', self.calibration_image)

            self.counter = self.counter + 1
            if self.counter == 3:
                cv2.circle(self.calibration_image, (x,y), 10, (255,255,255), -1)
                self.text_over_image("Press c to progress or press r to reset", self.calibration_image)
                cv2.imshow('window', self.calibration_image)

    def draw_box_on_image(self,event,x,y,flags,param):
        rectangle_image = self.just_holes_calibration.copy()
        if self.refpt:
            rectangle_image = self.just_holes_calibration.copy()
            cv2.rectangle(rectangle_image, (self.refpt[0][0],self.refpt[0][1]), (x,y), (120,120,120), 1)
            cv2.imshow('window', rectangle_image)
        if event == cv2.EVENT_MBUTTONDOWN:
            self.refpt.append((x,y))
        if event == cv2.EVENT_MBUTTONUP:
            cv2.rectangle(self.just_holes_calibration, (self.refpt[0][0],self.refpt[0][1]), (x,y), 0, -1) #type: ignore
            cv2.rectangle(self.just_holes_original, (self.refpt[0][0],self.refpt[0][1]), (x,y), 0, -1) #type: ignore
            cv2.imshow('window', self.just_holes_calibration)
            self.refpt.clear()

    def select_column(self,event,x,y,flags,param):
        rectangle_image = self.just_holes_original.copy()
        rectangle_image[0:40,0:1000] = 0
        if event == cv2.EVENT_MBUTTONDOWN:
            self.rows.append((x,y))
            self.clicked = 1
        if event == cv2.EVENT_MBUTTONUP:
            self.rows.append((x,y))
            self.clicked = 0
            rectangle_image[0:40,0:1000] = 0
            cv2.rectangle(rectangle_image, (self.rows[0][0], self.rows[0][1]), (self.rows[1][0], self.rows[1][1]), 120, 2)
            self.text_over_image('Press c to continue or press r to place rectangle again.',rectangle_image)
            cv2.imshow('window', rectangle_image)
        if self.clicked == 1:
            rectangle_image = self.just_holes_original.copy()
            cv2.rectangle(rectangle_image, (self.rows[0][0],self.rows[0][1]), (x,y), 120, 2)
            cv2.imshow('window', rectangle_image)

    def select_row(self,event,x,y,flags,param):
        rectangle_image = self.just_holes_original.copy()
        rectangle_image[0:40,0:1000] = 0
        if event == cv2.EVENT_MBUTTONDOWN:
            self.columns.append((x,y))
            self.clicked = 1
        if event == cv2.EVENT_MBUTTONUP:
            self.columns.append((x,y))
            self.clicked = 0
            rectangle_image[0:40,0:1000] = 0
            cv2.rectangle(rectangle_image, (self.columns[0][0], self.columns[0][1]), (self.columns[1][0], self.columns[1][1]), 120, 2)
            self.text_over_image('Press c to continue or press r to place rectangle again',rectangle_image)
            cv2.imshow('window', rectangle_image)                
        if self.clicked == 1:
            rectangle_image = self.just_holes_original.copy()
            cv2.rectangle(rectangle_image, (self.columns[0][0],self.columns[0][1]), (x,y), 120, 2)
            cv2.imshow('window', rectangle_image)
    
    def slider_rotation(self, val):
        self.rotate_image(val, self.calibration_image)

    def rotate_image(self, angle, image):
        rotation_matrix = cv2.getRotationMatrix2D((image.shape[1] / 2, image.shape[0] / 2), angle, 1)
        rotated_image = cv2.warpAffine(image, rotation_matrix, (image.shape[1], image.shape[0]))
        cv2.imshow("window", rotated_image)

    def slider_change(self, val):
        contour_image = self.just_holes_original.copy()
        contours, _ = cv2.findContours(contour_image[50:,:], cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        filtered_contours = []
        for contour in contours:
            area = cv2.contourArea(contour)
            if area >= val:
                filtered_contours.append(contour)
        self.generate_map_area = val

        colored_image = cv2.cvtColor(contour_image,cv2.COLOR_GRAY2BGR)
        cv2.drawContours(colored_image[50:,:],filtered_contours,-1,(0,255,0),-1) #type: ignore
        cv2.imshow('window', colored_image)
        self.just_holes_calibration = colored_image.copy()

    def text_over_image(self, text, image):
        image = cv2.putText(
        image,
        text, 
        self.font_origin,
        self.font,
        self.font_scale,
        self.font_color,
        self.font_thickness,
        cv2.LINE_AA
        )     
    
    




