import rclpy
from rclpy.node import Node

from typing import Optional
from geometry_msgs.msg import Pose

from aprs_vision.antvision_read import AntVisionUtility
from aprs_interfaces.msg import Object, Objects, KitTray, PartTray, SlotInfo, Trays

import math
from time import sleep

class TeachTablePublisher(Node):
    def __init__(self):
        super().__init__('teach_table_state_publisher')
        self.teach_table_state_publisher_ = self.create_publisher(Trays, 'teach_table_state', 10)
        self.trays: Optional[Trays] = None
        self.objects: Optional[Objects] = None
        self.trays_info = Trays()
        self.timer = self.create_timer(0.5, self.publish)
        self.build_objects()
        self.build_trays_msg()
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

    def publish(self):
        if self.trays:
            self.teach_table_state_publisher_.publish(self.trays_info)

    def calculate_distance(self, p1: Pose, p2: Pose):
        return math.sqrt(math.square(p1.position.x - p2.position.x) + math.square(p1.position.y - p2.position.y))
    
    def build_kit_tray_msg(self, kit_tray_obj: Object) -> KitTray:
        if not self.objects:
            return KitTray()
        
        kit_tray_msg = KitTray()
        kit_tray_msg.identifier = kit_tray_obj.object_identifier
        kit_tray_msg.name = kit_tray_obj.name

        part_poses = [p.pose_stamped.pose for p in self.objects if p.object_type == Object.PART]

        # for p in self.objects:
        #     if p.object_type == Object.PART:
        #         part_poses.append(p.pose)


        # not used anywhere
        # a = self.slot_offsets[kit_tray_msg.identifier]

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

    def build_objects(self):

        # list of trays
        self.trays_info.kit_trays = []
        self.trays_info.part_trays = []

        # reading socket
        tray_locations = AntVisionUtility("CONFIG_TEACH").get_object_locations()
        if not tray_locations:
            return
        
        # defining objects
        self.objects = Objects()
        for name, rotation, x, y, type_ in tray_locations:
            object = Object()
            object.object_identifier = self.get_object_identifier(name)
            object.name = self.generate_unique_name(object.object_identifier )
            object.object_type = self.get_object_type(type_)

            if object.object_identifier == Object.LARGE_GEAR_TRAY:
                x_offset = +0.007
                y_offset = -0.008
            elif object.object_identifier == Object.MEDIUM_GEAR_TRAY:
                x_offset = 0.006
                y_offset = -0.011
            elif object.object_identifier == Object.M2L1_KIT_TRAY:
                x_offset = 0.003
                y_offset = 0.003
            else:
                x_offset = 0.0 
                y_offset = 0.0

            object.pose_stamped.header.frame_id = "fanuc_base_link"
            object.pose_stamped.pose.position.x = x + x_offset
            object.pose_stamped.pose.position.y = y + y_offset
            if object.object_type == Object.PART:
                object.pose_stamped.pose.position.z = 0.03
            else:
                object.pose_stamped.pose.position.z = 0.0
            
            quaternion = self.euler_to_quaternion(0.0, 0.0, rotation)
            object.pose_stamped.pose.orientation.x = quaternion[0]
            object.pose_stamped.pose.orientation.y = quaternion[1]
            object.pose_stamped.pose.orientation.z = quaternion[2]
            object.pose_stamped.pose.orientation.w = quaternion[3]
            
            self.objects.objects.append(object) # type: ignore

    def build_part_tray_msg(self, part_tray_obj: Object) -> PartTray:
        if not self.objects:
            return PartTray()
        
        part_tray_msg = PartTray()
        part_tray_msg.identifier = part_tray_obj.object_identifier
        part_tray_msg.name = part_tray_obj.name

        part_poses = [p.pose_stamped.pose for p in self.raw_object_data if p.object_type == Object.PART]

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

            # self.get_logger().info(str(min(distance)))
            # self.get_logger().info(slot_info.name)

            if min(distance) < self.part_thresh:
                slot_info.occupied = True
            else:
                slot_info.occupied = False

            part_tray_msg.slots.append(slot_info) #type: ignore

        # self.get_logger().info("\n")
        return part_tray_msg
    
    def build_trays_msg(self):
        if self.objects:
            for object in self.objects:
                object: Object
                if object.object_type == Object.KIT_TRAY:
                    self.trays_info.kit_trays.append(self.build_kit_tray_msg(object)) #type: ignore
                elif object.object_type == Object.PART_TRAY:
                    self.trays_info.part_trays.append(self.build_part_tray_msg(object)) #type: ignore

    def get_object_type(self, type_) -> int:
        if type_ == 'KT':
            return Object.KIT_TRAY
        elif type_ == 'PT':
            return Object.PART_TRAY
        elif type_ == 'P':
            return Object.PART
        else:
            return -1
        
    def get_object_identifier(self, name):
        mapping = {
            'sku_part_small_gear': Object.SMALL_GEAR,
            'sku_part_medium_gear': Object.MEDIUM_GEAR,
            'sku_part_large_gear': Object.LARGE_GEAR,
            'sku_small_gear_vessel': Object.SMALL_GEAR_TRAY,
            'sku_medium_gear_vessel': Object.MEDIUM_GEAR_TRAY,
            'sku_large_gear_vessel': Object.LARGE_GEAR_TRAY,
            'sku_kit_m2l1_vessel': Object.M2L1_KIT_TRAY,
            'sku_kit_s2l2_vessel': Object.S2L2_KIT_TRAY,
        }
        return mapping.get(name)
    
    def generate_unique_name(self, object_identifier) -> str:
        mappings = {
            Object.SMALL_GEAR: 'small_gear',
            Object.MEDIUM_GEAR: 'medium_gear',
            Object.LARGE_GEAR: 'large_gear',
            Object.SMALL_GEAR_TRAY: 'small_gear_tray',
            Object.MEDIUM_GEAR_TRAY: 'medium_gear_tray',
            Object.LARGE_GEAR_TRAY: 'large_gear_tray',
            Object.M2L1_KIT_TRAY: 'm2l1_kit_tray',
            Object.S2L2_KIT_TRAY: 's2l2_kit_tray',
        }
        object_identifier = mappings.get(object_identifier)
        if object_identifier not in self.object_counters:
            self.object_counters[object_identifier] = 1
        else:
            self.object_counters[object_identifier] += 1
        return f'{object_identifier}_{self.object_counters[object_identifier]}'
    

    

    

    

