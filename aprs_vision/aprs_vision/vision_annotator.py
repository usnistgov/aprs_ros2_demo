#!/usr/bin/env python3

from math import sqrt

from rclpy.node import Node

from aprs_vision.slot_offsets import SlotOffsets

from geometry_msgs.msg import Pose
from aprs_interfaces.msg import Object, Objects, Tray, Trays, SlotInfo

class VisionAnnotator(Node):
    def __init__(self):
        super().__init__('vision_annotator')
        self.fanuc_trays_info = Trays()
        self.motoman_trays_info = Trays()
        self.teach_trays_info = Trays()

        self.slot_distance_threshold = 0.07

        self.fanuc_objects_sub = self.create_subscription(Objects, 'fanuc_vision_objects', self.fanuc_objects_callback, 10)
        self.motoman_objects_sub = self.create_subscription(Objects, 'motoman_vision_objects', self.motoman_objects_callback, 10)
        self.teach_objects_sub = self.create_subscription(Objects, 'teach_vision_objects', self.teach_objects_callback, 10)

        self.fanuc_trays_publisher = self.create_publisher(Trays, 'fanuc_trays_info', 10)
        self.motoman_trays_publisher = self.create_publisher(Trays, 'motoman_trays_info', 10)
        self.teach_trays_publisher = self.create_publisher(Trays, 'teach_trays_info', 10)

        self.trays_publish_timer = self.create_timer(0.1, self.publish_trays)
    
    def fanuc_objects_callback(self, msg: Objects):
        self.fanuc_trays_info = self.build_trays_msg(msg)

    def motoman_objects_callback(self, msg: Objects):
        self.motoman_trays_info = self.build_trays_msg(msg)

    def teach_objects_callback(self, msg: Objects):
        self.teach_trays_info = self.build_trays_msg(msg)

    def publish_trays(self):
        self.fanuc_trays_publisher.publish(self.fanuc_trays_info)
        self.teach_trays_publisher.publish(self.teach_trays_info)
        self.motoman_trays_publisher.publish(self.motoman_trays_info)

    def build_tray_msg(self, tray_object: Object, part_poses: list[Pose]) -> Tray:        
        tray_msg = Tray()
        tray_msg.identifier = tray_object.object_identifier
        tray_msg.name = tray_object.name

        # Fill slot info
        for name, (x_offset, y_offset) in SlotOffsets.offsets[tray_msg.identifier].items():
            slot_info = SlotInfo()
            slot_info.name = f"{tray_msg.name}_{name}"

            slot_pose = tray_object.pose_stamped.pose
            slot_pose.position.x += x_offset
            slot_pose.position.y += y_offset
           
            all_distances = [self.calculate_distance(slot_pose, p) for p in part_poses]
            
            try:
                slot_info.occupied = min(all_distances) < self.slot_distance_threshold
            except ValueError:
                slot_info.occupied = False
            
            if "sg" in slot_info.name:
                slot_info.size = SlotInfo.SMALL
            elif "mg" in slot_info.name:
                slot_info.size = SlotInfo.MEDIUM
            elif "lg" in slot_info.name:
                slot_info.size = SlotInfo.LARGE
            
            tray_msg.slots.append(slot_info) #type: ignore

        return tray_msg

    def build_trays_msg(self, objects_msg: Objects) -> Trays:        
        trays_info = Trays()
        trays_info.kit_trays = []
        trays_info.part_trays = []
        
        objects: list[Object] = objects_msg.objects #type: ignore
        
        part_poses = [p.pose_stamped.pose for p in objects if p.object_type == Object.PART]
        
        for object in objects:
            object: Object
            if object.object_type == Object.KIT_TRAY:
                trays_info.kit_trays.append(self.build_tray_msg(object, part_poses)) #type: ignore
            elif object.object_type == Object.PART_TRAY:
                trays_info.part_trays.append(self.build_tray_msg(object, part_poses)) #type: ignore
            
        return trays_info
            
    def calculate_distance(self, p1: Pose, p2: Pose):
        return sqrt((p1.position.x - p2.position.x)**2 + (p1.position.y - p2.position.y)**2)