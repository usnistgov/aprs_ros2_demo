#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Pose
from aprs_interfaces.msg import Object, Objects, KitTray, PartTray, SlotInfo, Trays
from typing import Optional
from math import sqrt
from numpy import square

class VisionAnnotator(Node):
    def __init__(self):
        super().__init__('vision_annotator')
        self.fanuc_trays_info = Trays()
        self.motoman_trays_info = Trays()
        self.teach_trays_info = Trays()

        self.part_thresh = 0.07
        self.kit_thresh = 0.050

        self.fanuc_objects_sub = self.create_subscription(Objects, 'fanuc_vision_objects', self.fanuc_objects_callback, 10)
        self.motoman_objects_sub = self.create_subscription(Objects, 'motoman_vision_objects', self.motoman_objects_callback, 10)
        self.teach_objects_sub = self.create_subscription(Objects, 'teach_vision_objects', self.teach_objects_callback, 10)

        self.fanuc_trays_publisher = self.create_publisher(Trays, 'fanuc_trays_info', 10)
        self.motoman_trays_publisher = self.create_publisher(Trays, 'motoman_trays_info', 10)
        self.teach_trays_publisher = self.create_publisher(Trays, 'teach_trays_info', 10)

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

        self.trays_publish_timer = self.create_timer(0.1, self.publish_trays)
    
    def fanuc_objects_callback(self, msg: Objects ):
        if not self.fanuc_raw_object_data:
            self.fanuc_raw_object_data = []
        
        self.fanuc_raw_object_data.clear()

        for obj in msg.objects:
            self.fanuc_raw_object_data.append(obj)

        self.build_trays_msg("FANUC")

    def motoman_objects_callback(self, msg: Objects ):
        if not self.motoman_raw_object_data:
            self.motoman_raw_object_data = []
        
        self.motoman_raw_object_data.clear()

        for obj in msg.objects:
            self.motoman_raw_object_data.append(obj)

        self.build_trays_msg("MOTOMAN")

    def teach_objects_callback(self, msg: Objects ):
        if not self.teach_raw_object_data:
            self.teach_raw_object_data = []
        
        self.teach_raw_object_data.clear()

        for obj in msg.objects:
            self.teach_raw_object_data.append(obj)

        self.build_trays_msg("TEACH")

    def publish_trays(self):
        self.fanuc_trays_publisher.publish(self.fanuc_trays_info)
        self.teach_trays_publisher.publish(self.teach_trays_info)
        self.motoman_trays_publisher.publish(self.motoman_trays_info)

    def calculate_distance(self, p1: Pose, p2: Pose):
        return sqrt(square(p1.position.x - p2.position.x) + square(p1.position.y - p2.position.y))

    def build_kit_tray_msg(self, kit_tray_obj: Object, source: str) -> KitTray:

        if source == "FANUC":
            raw_object_data = self.fanuc_raw_object_data
        elif source == "MOTOMAN":
            raw_object_data = self.motoman_raw_object_data
        elif source =="TEACH":
            raw_object_data = self.teach_raw_object_data
                
        if not raw_object_data:
            return KitTray()
        
        kit_tray_msg = KitTray()
        kit_tray_msg.identifier = kit_tray_obj.object_identifier
        kit_tray_msg.name = kit_tray_obj.name

        part_poses = [p.pose_stamped.pose for p in raw_object_data if p.object_type == Object.PART]

        a = self.slot_offsets[kit_tray_msg.identifier]

        for name, (x_offset, y_offset) in self.slot_offsets[kit_tray_msg.identifier].items():
            slot_info = SlotInfo()
            slot_info.name = f"{kit_tray_msg.name}_{name}"
            slot_info.number = int(name[-1])

            slot_pose = kit_tray_obj.pose_stamped.pose
            slot_pose.position.x += x_offset
            slot_pose.position.y += y_offset

            distance = [self.calculate_distance(slot_pose, p) for p in part_poses]

            if not distance:
                slot_info.occupied = False
                if "lg" in slot_info.name:
                    kit_tray_msg.large_gear_slots.append(slot_info) #type: ignore
                elif "mg" in slot_info.name:
                    kit_tray_msg.medium_gear_slots.append(slot_info) #type: ignore
                elif "sg" in slot_info.name:
                    kit_tray_msg.small_gear_slots.append(slot_info) #type: ignore
                continue
            
            if min(distance) < self.kit_thresh:
                slot_info.occupied = True
            else:
                slot_info.occupied = False

            if "lg" in slot_info.name:
                kit_tray_msg.large_gear_slots.append(slot_info) #type: ignore
            elif "mg" in slot_info.name:
                kit_tray_msg.medium_gear_slots.append(slot_info) #type: ignore
            elif "sg" in slot_info.name:
                kit_tray_msg.small_gear_slots.append(slot_info) #type: ignore

        return kit_tray_msg

    def build_part_tray_msg(self, part_tray_obj: Object, source: str) -> PartTray:

        if source == "FANUC":
            raw_object_data = self.fanuc_raw_object_data
        elif source == "MOTOMAN":
            raw_object_data = self.motoman_raw_object_data
        elif source =="TEACH":
            raw_object_data = self.teach_raw_object_data
    
        if not raw_object_data:
            return PartTray()
        
        part_tray_msg = PartTray()
        part_tray_msg.identifier = part_tray_obj.object_identifier
        part_tray_msg.name = part_tray_obj.name

        part_poses = [p.pose_stamped.pose for p in raw_object_data if p.object_type == Object.PART]

        a = self.slot_offsets[part_tray_msg.identifier]

        for name, (x_offset, y_offset) in self.slot_offsets[part_tray_msg.identifier].items():
            slot_info = SlotInfo()
            slot_info.name = f"{part_tray_msg.name}_{name}"
            slot_info.number = int(name[-1])

            slot_pose = part_tray_obj.pose_stamped.pose
            slot_pose.position.x += x_offset
            slot_pose.position.y += y_offset

            distance = [self.calculate_distance(slot_pose, p) for p in part_poses]

            if not distance:
                slot_info.occupied = False
                part_tray_msg.slots.append(slot_info)
                continue

            if min(distance) < self.part_thresh:
                slot_info.occupied = True
            else:
                slot_info.occupied = False

            part_tray_msg.slots.append(slot_info) #type: ignore

        return part_tray_msg

    def build_trays_msg(self, source: str):

        if source == "FANUC":
            raw_object_data = self.fanuc_raw_object_data
        elif source == "MOTOMAN":
            raw_object_data = self.motoman_raw_object_data
        elif source =="TEACH":
            raw_object_data = self.teach_raw_object_data
    
        if not raw_object_data:
            return
        
        trays_info = Trays()
        trays_info.kit_trays = []
        trays_info.part_trays = []
        
        for object in raw_object_data:
            object: Object
            if object.object_type == Object.KIT_TRAY:
                trays_info.kit_trays.append(self.build_kit_tray_msg(object, source)) #type: ignore
            elif object.object_type == Object.PART_TRAY:
                trays_info.part_trays.append(self.build_part_tray_msg(object, source)) #type: ignore

        if source == "FANUC":
            self.fanuc_trays_info.kit_trays = trays_info.kit_trays
            self.fanuc_trays_info.part_trays = trays_info.part_trays
        elif source == "MOTOMAN":
            self.motoman_trays_info.kit_trays = trays_info.kit_trays
            self.motoman_trays_info.part_trays = trays_info.part_trays
        elif source =="TEACH":
            self.teach_trays_info.kit_trays = trays_info.kit_trays
            self.teach_trays_info.part_trays = trays_info.part_trays