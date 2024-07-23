#!/usr/bin/env python3

from rclpy.node import Node

from tf2_ros.transform_broadcaster import TransformBroadcaster

from aprs_vision.slot_offsets import SlotOffsets

from geometry_msgs.msg import TransformStamped, Pose
from aprs_interfaces.msg import Object, Objects

class VisionBroadcaster(Node):
    def __init__(self):
        super().__init__('vision_tf_broadcaster')
        self.tf_broadcaster = TransformBroadcaster(self)
        self.motoman_transforms: list[TransformStamped] = []
        self.fanuc_transforms: list[TransformStamped] = []

        self.fanuc_objects_sub = self.create_subscription(Objects, 'fanuc_vision_objects', self.fanuc_objects_callback, 10)
        self.motoman_objects_sub = self.create_subscription(Objects, 'motoman_vision_objects', self.motoman_objects_callback, 10)

        self.tf_publish_timer = self.create_timer(0.1, self.publish_frames)
    
    def fanuc_objects_callback(self, msg: Objects):
        self.fanuc_transforms = self.build_frames(msg)

    def motoman_objects_callback(self, msg: Objects):
        self.motoman_transforms = self.build_frames(msg)

    def publish_frames(self):
        if self.fanuc_transforms:
            for transform in self.fanuc_transforms: # Update timestamps
                transform.header.stamp = self.get_clock().now().to_msg()
            
            self.tf_broadcaster.sendTransform(self.fanuc_transforms)

        if self.motoman_transforms:
            for transform in self.motoman_transforms: # Update timestamps
                transform.header.stamp = self.get_clock().now().to_msg()
            
            self.tf_broadcaster.sendTransform(self.motoman_transforms)

    def build_frames(self, objects_msg: Objects) -> list[TransformStamped]:
        transforms: list[TransformStamped] = []

        objects: list[Object] = objects_msg.objects #type: ignore
        
        tray_types = (Object.KIT_TRAY, Object.PART_TRAY)

        for object in objects:
            if object.object_type not in tray_types: #don't publish part transforms
                continue
            
            transforms.append(self.generate_transform(
                object.pose_stamped.header.frame_id, 
                object.name, 
                object.pose_stamped.pose))

            slot_info = SlotOffsets.offsets.get(object.object_identifier) or {}
            
            for slot_name, (x_offset, y_offset) in slot_info.items():
                slot_frame_id = f"{object.name}_{slot_name}"

                slot_pose = Pose()
                slot_pose.position.x = x_offset
                slot_pose.position.y = y_offset

                transforms.append(self.generate_transform(object.name, slot_frame_id, slot_pose))
        
        return transforms
                
    def generate_transform(self, parent_frame, child_frame, initial_pose: Pose) -> TransformStamped:
        t = TransformStamped()

        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = parent_frame
        t.child_frame_id = child_frame

        t.transform.translation.x = initial_pose.position.x
        t.transform.translation.y = initial_pose.position.y
        t.transform.translation.z = initial_pose.position.z
        t.transform.rotation = initial_pose.orientation
        
        return t