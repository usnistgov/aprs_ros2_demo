#!/usr/bin/env python3

import os
import cv2
import math
import numpy as np

from cv2.typing import MatLike
from typing import Optional

from rclpy.node import Node
from rclpy.qos import qos_profile_default

from ament_index_python.packages import get_package_share_directory

from tf2_ros.transform_broadcaster import TransformBroadcaster

from aprs_interfaces.srv import LocateTrays

from aprs_interfaces.msg import Trays, Tray, SlotInfo
from aprs_vision.slot_offsets import SlotOffsets
from aprs_vision.stream_handler import StreamHandler
from geometry_msgs.msg import TransformStamped, Point, Quaternion, PoseStamped

class DetectionException(Exception):
    pass

class DetectionArea(Node):    
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
    
    tray_height = 0.015
    gear_height = 0.016
    conversion_factor = 0.8466 # (px/mm)

    def __init__(self):
        super().__init__("detection_area", namespace='fanuc')

        # Declare and get parameters
        self.declare_parameter('robot_name', '')
        self.declare_parameter('location', '')
        self.declare_parameter('base_frame', '')
        self.declare_parameter('video_stream', '')

        self.declare_parameter('publish_frames', False)

        self.declare_parameter('rotation_from_base_frame', 0.0)

        self.declare_parameter('table_origin.x', 0.0)
        self.declare_parameter('table_origin.y', 0.0)
        self.declare_parameter('table_origin.z', 0.0)

        self.robot_name = self.get_parameter('robot_name').get_parameter_value().string_value
        self.location = self.get_parameter('location').get_parameter_value().string_value
        self.base_frame = self.get_parameter('base_frame').get_parameter_value().string_value
        video_stream = self.get_parameter('video_stream').get_parameter_value().string_value

        self.publish_frames = self.get_parameter('publish_frames').get_parameter_value().bool_value

        self.angle_offset = self.get_parameter('rotation_from_base_frame').get_parameter_value().double_value

        self.table_origin = Point(
            x=self.get_parameter('table_origin.x').get_parameter_value().double_value,
            y=self.get_parameter('table_origin.y').get_parameter_value().double_value,
            z=self.get_parameter('table_origin.z').get_parameter_value().double_value
        )

        # Stream Handler
        share_path = get_package_share_directory('aprs_vision')
        calibration_filepath = os.path.join(share_path, 'config', f'{self.robot_name}_{self.location}_calibration.npz')

        self.stream_handler = StreamHandler(video_stream, calibration_filepath)
        self.current_frame: Optional[MatLike] = None
  
        # ArUco
        self.aruco_detector = cv2.aruco.ArucoDetector(cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_5X5_50))
        
        # ROS Messages
        self.trays_info: Optional[Trays] = None

        # ROS Services
        self.locate_trays_srv = self.create_service(LocateTrays, f'locate_trays_on_{self.location}', self.locate_trays_cb)

        # ROS Topics
        self.trays_info_pub = self.create_publisher(Trays, f'{self.location}_trays_info', qos_profile_default)
    
        # TF Broadcaster
        self.tf_broadcaster = TransformBroadcaster(self)
        self.transforms: list[TransformStamped] = []

        # ROS Timers
        self.publish_timer = self.create_timer(timer_period_sec=1, callback=self.publish)
        self.read_frame_timer = self.create_timer(timer_period_sec=0.1, callback=self.get_frame)

        self.get_logger().info('Connected to camera')

    def locate_trays_cb(self, _, response: LocateTrays.Response) -> LocateTrays.Response:
        if self.current_frame is None:
            response.message = 'Unable to connect to camera'
            response.success = False
            return response
        
        try:
            no_background = self.remove_background(self.current_frame)

            self.trays_info = self.detect_trays(no_background)

        except DetectionException as e:
            response.success = False
            response.message = str(e)
            return response

        response.success = True

        if len(self.trays_info.kit_trays) == 0 and len(self.trays_info.part_trays) == 0:
            response.message = 'No trays found'
            return response
        
        response.message = 'Located trays'

        return response

    def get_frame(self):
        self.current_frame = self.stream_handler.read_frame()

    def publish(self):
        # Publish trays info
        if self.trays_info is not None:
            self.trays_info_pub.publish(self.trays_info)

        # Update timestamps and send TF transforms
        for t in self.transforms:
            t.header.stamp = self.get_clock().now().to_msg()
        
        if self.transforms:
            self.tf_broadcaster.sendTransform(self.transforms)
    
    def remove_background(self, frame: MatLike) -> MatLike:
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        thresh = cv2.inRange(hsv, (0, 0, 0), (255, 255, 150)) #type: ignore

        canvas = np.zeros((frame.shape[0], frame.shape[1]), dtype=np.uint8)

        contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        contours = [c for c in contours if cv2.contourArea(c) > 10000]

        cv2.drawContours(canvas, contours, -1, color=255, thickness=cv2.FILLED) # type: ignore

        return cv2.bitwise_and(frame, frame, mask=canvas)

    def classify_tray(self,contour: MatLike,image: MatLike)-> tuple[int, int, tuple[float, float], float]:
        """Takes in a contour of a tray and the rectified image of the workspace and returns the tray identifier, as well as the 
        tray id, center and rotation of the tray based on the aruco marker.

        Args:
            contour (MatLike): Tray contour
            image (MatLike): Rectified Image

        Raises:
            DetectionException: issue with detection

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
            raise DetectionException("Detected object without fiducial")            

        if len(marker_ids) > 1:
            raise DetectionException("Detected multiple fiducials on one tray")
        
        tray_id = marker_ids[0][0]
        
        corners = np.reshape(corners[0],(4,2))

        # Calculate marker center and angle of rotation
        marker_center = (float(corners[0][0] + (corners[2][0]-corners[0][0])/2), float(corners[0][1] + (corners[2][1]-corners[0][1])/2))
        angle = math.atan2(corners[0][1] - corners[1][1],corners[0][0]-corners[1][0])

        # Use area to classify tray identifier
        tray_area = cv2.contourArea(contour) * self.conversion_factor**2

        if tray_area < 20000:
            raise DetectionException('Detected a partial tray')

        identifier = min({k: abs(v - tray_area) for (k,v) in self.tray_areas.items()}.items(), key=lambda x: x[1])[0]

        return (identifier, tray_id, marker_center, angle)

    def detect_trays(self, table_image: MatLike) -> Trays:       
        # Clear list of transforms
        self.transforms.clear()

        # Publish transform from robot base to image base        
        image_base_frame = f'{self.robot_name}_{self.location}_image_base_frame'
        
        if self.publish_frames:
            image_origin = Point(
                x=self.table_origin.x,
                y=self.table_origin.y,
                z=self.table_origin.z,
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
            identifier, tray_id, (tray_x, tray_y), theta = self.classify_tray(contour,table_image)

            # Build tray message
            tray_msg = Tray()
            tray_msg.identifier = identifier
            tray_msg.name = f'{self.tray_names[identifier]}_{tray_id:02}'

            tray_center = Point(
                x=(tray_x * self.conversion_factor) / 1000,
                y=(tray_y * self.conversion_factor) / 1000, 
                z=-self.tray_height
            )

            tray_msg.tray_pose = self.generate_pose(image_base_frame, tray_center, 0.0, math.pi, theta)

            # Publish TF frame from image base frame to tray fiducial center
            if self.publish_frames:
                self.transforms.append(
                    self.generate_transform(image_base_frame, tray_msg.name, tray_center, 0.0, math.pi, theta)
                )
            
            beta = theta + math.pi
        
            # Build slot messages
            for slot_name, (x_off, y_off) in SlotOffsets.offsets[tray_msg.identifier].items():
                slot_info = self.build_slot_info_message(tray_msg.identifier, tray_msg.name, slot_name, x_off, y_off)
                
                x_off_px = (x_off*1000) / self.conversion_factor
                y_off_px = (y_off*1000) / self.conversion_factor

                slot_x = int(tray_x + (x_off_px*math.cos(beta)) - (-y_off_px*math.sin(beta)))
                slot_y = int(tray_y + (x_off_px*math.sin(beta)) + (-y_off_px*math.cos(beta)))

                slot_info.occupied = self.check_occupied(table_image, (slot_x, slot_y))

                tray_msg.slots.append(slot_info)

            # Adds tray_msg to corresponding list in tray_info    
            if identifier in DetectionArea.part_tray_types:
                trays_info.part_trays.append(tray_msg)
            elif identifier in DetectionArea.kit_tray_types:
                trays_info.kit_trays.append(tray_msg)

        return trays_info

    def build_slot_info_message(self, identifier: int, tray_name: str, slot_name: str, x_off: float, y_off: float) -> SlotInfo:
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

    def check_occupied(self, image: MatLike, slot_center: tuple[int, int]) -> bool:
        offset = 15
        gear_hsv_lower = (0, 83, 0)
        gear_hsv_upper = (147, 255, 255)

        try:
            square = image[slot_center[1]-offset:slot_center[1]+offset, slot_center[0]-offset:slot_center[0]+offset]
        
            gear = cv2.inRange(cv2.cvtColor(square, cv2.COLOR_BGR2HSV), gear_hsv_lower, gear_hsv_upper) # type: ignore

        except cv2.error:
            raise DetectionException("Slot outside of detection area")

        if cv2.countNonZero(gear) > ((offset**2) * 0.5):
            return True

        return False   
         
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
    
    def quaternion_from_euler(self, roll: float, pitch: float, yaw: float) -> Quaternion:
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