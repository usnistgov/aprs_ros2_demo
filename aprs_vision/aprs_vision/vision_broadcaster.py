#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import TransformStamped, Pose
from tf2_ros.transform_broadcaster import TransformBroadcaster
from aprs_interfaces.msg import Object, Objects
from typing import Optional

class VisionBroadcaster(Node):
    def __init__(self):
        super().__init__('vision_tf_broadcaster')
        self.tf_broadcaster = TransformBroadcaster(self)
        self.transforms = []

        self.fanuc_objects_sub = self.create_subscription(Objects, 'fanuc_vision_objects', self.fanuc_objects_callback, 10)
        self.motoman_objects_sub = self.create_subscription(Objects, 'motoman_vision_objects', self.motoman_objects_callback, 10)
        self.teach_objects_sub = self.create_subscription(Objects, 'teach_vision_objects', self.teach_objects_callback, 10)
        
        self.slot_offsets: dict[int, dict[str, tuple[float, float]]] = {
            Object.S2L2_KIT_TRAY: {
                'lg_1': (-0.028, -0.055),
                'lg_2': (-0.028, 0.055),
                'sg_1': (0.055, -0.04),
                'sg_2': (0.055, 0.04),
            },
            Object.LARGE_GEAR_TRAY: {
                'slot_1': (0.0, 0.055),
                'slot_2': (0.0, -0.055),
            },
            Object.MEDIUM_GEAR_TRAY: {
                'slot_1': (0.038, -0.038),
                'slot_2': (0.038, 0.038),
                'slot_3': (-0.038, -0.038),
                'slot_4': (-0.038, 0.038),
            },
            Object.M2L1_KIT_TRAY: {
                'lg_1': (0.0, 0.04),
                'mg_1': (0.04, -0.055),
                'mg_2': (-0.04, -0.055),
            },
            Object.SMALL_GEAR_TRAY: {
                'slot_1': (0.028, -0.028),
                'slot_2': (0.028, 0.028),
                'slot_3': (-0.028, -0.028),
                'slot_4': (-0.028, 0.028),
            },
        }
        
        self.fanuc_raw_object_data : Optional[list[Object]]  = None
        self.motoman_raw_object_data : Optional[list[Object]]  = None
        self.teach_raw_object_data : Optional[list[Object]]  = None

        self.tf_publish_timer = self.create_timer(0.1, self.publish_frames)
    
    def fanuc_objects_callback(self, msg: Objects ):
        if not self.fanuc_raw_object_data:
            self.fanuc_raw_object_data = []
        
        self.fanuc_raw_object_data.clear()

        for obj in msg.objects:
            self.fanuc_raw_object_data.append(obj)

        self.build_frames("FANUC")
    
    def motoman_objects_callback(self, msg: Objects ):
        if not self.motoman_raw_object_data:
            self.motoman_raw_object_data = []
        
        self.motoman_raw_object_data.clear()

        for obj in msg.objects:
            self.motoman_raw_object_data.append(obj)

        self.build_frames("MOTOMAN")

    def teach_objects_callback(self, msg: Objects ):
        if not self.teach_raw_object_data:
            self.teach_raw_object_data = []
        
        self.teach_raw_object_data.clear()

        for obj in msg.objects:
            self.teach_raw_object_data.append(obj)

        self.build_frames("TEACH")

    def publish_frames(self):
        self.tf_broadcaster.sendTransform(self.transforms)

    def create_pose(self, x, y):
        pose = Pose()
        pose.position.x = x
        pose.position.y = y
        pose.position.z = 0.03
        pose.orientation.x = 0.0
        pose.orientation.y = 0.0
        pose.orientation.z = 0.0
        pose.orientation.w = 1.0
        return pose

    def build_frames(self, source: str):
        self.transforms.clear()

        if source == "FANUC":
            raw_object_data = self.fanuc_raw_object_data
        elif source == "MOTOMAN":
            raw_object_data = self.motoman_raw_object_data
        elif source =="TEACH":
            raw_object_data = self.teach_raw_object_data
                
        if not raw_object_data:
            return

        for object in raw_object_data:
            if object.object_type == Object.PART:
                continue
            object: Object
            self.generate_transform(object.pose_stamped.header.frame_id, object.name, object.pose_stamped.pose)

            slot_info = self.slot_offsets.get(object.object_identifier)
            if slot_info:
                for slot_name, (x_offset, y_offset) in slot_info.items():
                    slot_frame_id = f"{object.name}_{slot_name}"
                    self.generate_transform(object.name, slot_frame_id, self.create_pose(x_offset, y_offset))

    def generate_transform(self, parent_frame, child_frame, initial_pose: Pose):
        t = TransformStamped()

        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = parent_frame
        t.child_frame_id = child_frame

        t.transform.translation.x = initial_pose.position.x
        t.transform.translation.y = initial_pose.position.y
        t.transform.translation.z = initial_pose.position.z
        t.transform.rotation.x = initial_pose.orientation.x
        t.transform.rotation.y = initial_pose.orientation.y
        t.transform.rotation.z = initial_pose.orientation.z
        t.transform.rotation.w = initial_pose.orientation.w
        
        self.transforms.append(t)