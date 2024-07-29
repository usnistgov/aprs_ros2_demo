#!/usr/bin/env python3

import math
from typing import Optional

from rclpy.node import Node

from aprs_vision.vision_read import VisionUtility

from example_interfaces.srv import Trigger
from aprs_interfaces.msg import Object, Objects


class VisionPublisher(Node):
    object_name_mappings = {
        Object.SMALL_GEAR: 'small_gear',
        Object.MEDIUM_GEAR: 'medium_gear',
        Object.LARGE_GEAR: 'large_gear',
        Object.SMALL_GEAR_TRAY: 'small_gear_tray',
        Object.MEDIUM_GEAR_TRAY: 'medium_gear_tray',
        Object.LARGE_GEAR_TRAY: 'large_gear_tray',
        Object.M2L1_KIT_TRAY: 'm2l1_kit_tray',
        Object.S2L2_KIT_TRAY: 's2l2_kit_tray',
    }

    object_identifier_mappings = {
        'sku_part_small_gear': Object.SMALL_GEAR,
        'sku_part_medium_gear': Object.MEDIUM_GEAR,
        'sku_part_large_gear': Object.LARGE_GEAR,
        'sku_small_gear_vessel': Object.SMALL_GEAR_TRAY,
        'sku_medium_gear_vessel': Object.MEDIUM_GEAR_TRAY,
        'sku_large_gear_vessel': Object.LARGE_GEAR_TRAY,
        'sku_kit_m2l1_vessel': Object.M2L1_KIT_TRAY,
        'sku_kit_s2l2_vessel': Object.S2L2_KIT_TRAY,
    }

    object_type_mappings = {
        'KT': Object.KIT_TRAY,
        'PT': Object.PART_TRAY,
        'P': Object.PART
    }

    def __init__(self):
        super().__init__('vision_publisher')
        self.fanuc_vision_object_publisher_ = self.create_publisher(Objects, 'fanuc_vision_objects', 10)
        self.motoman_vision_object_publisher_ = self.create_publisher(Objects, 'motoman_vision_objects', 10)
        self.teach_vision_object_publisher_ = self.create_publisher(Objects, 'teach_vision_objects', 10)

        self.update_vision_data_srv = self.create_service(Trigger, 'update_vision_data', self.update_vision_data_callback)
        self.object_counters = {}

        self.fanuc_objects: Optional[Objects] = None
        self.motoman_objects: Optional[Objects] = None
        self.teach_objects: Optional[Objects] = None

        self.fanuc_vision_utility_ = VisionUtility("CONFIG_FANUC")
        self.motoman_vision_utility_ = VisionUtility("CONFIG_MOTOMAN")
        self.teach_table_vision_utility_ = VisionUtility("CONFIG_TEACH")

        self.read_from_vision_system()

        self.timer = self.create_timer(0.5, self.publish)

    def read_from_vision_system(self):            
        self.fanuc_objects = self.build_objects_msg(self.fanuc_vision_utility_.get_object_locations(), "fanuc_base_link")
        
        self.motoman_objects = self.build_objects_msg(self.motoman_vision_utility_.get_object_locations(), "motoman_base_link")

        self.teach_objects = self.build_objects_msg(self.teach_table_vision_utility_.get_object_locations(), "teach_base_link")

    def publish(self):
        if self.fanuc_objects:
            self.fanuc_vision_object_publisher_.publish(self.fanuc_objects)
        
        if self.motoman_objects:
            self.motoman_vision_object_publisher_.publish(self.motoman_objects)
        
        if self.teach_objects:
            self.teach_vision_object_publisher_.publish(self.teach_objects)

    def build_objects_msg(self, object_locations: list[tuple[str, float, float, float, str]], base_link: str) -> Objects:
        objects_msg = Objects()

        for name, rotation, x, y, type_ in object_locations:
            object = Object()

            object.object_identifier = self.get_object_identifier(name)
            object.name = self.generate_unique_name(object.object_identifier)
            object.object_type = self.get_object_type(type_)

            object.pose_stamped.header.frame_id = base_link
            object.pose_stamped.pose.position.x = x
            object.pose_stamped.pose.position.y = y
            
            if object.object_type == Object.PART:
                object.pose_stamped.pose.position.z = 0.03
            else:
                object.pose_stamped.pose.position.z = 0.0
            
            quaternion = self.euler_to_quaternion(0.0, 0.0, rotation)
            object.pose_stamped.pose.orientation.x = quaternion[0]
            object.pose_stamped.pose.orientation.y = quaternion[1]
            object.pose_stamped.pose.orientation.z = quaternion[2]
            object.pose_stamped.pose.orientation.w = quaternion[3]

            objects_msg.objects.append(object) # type: ignore

        return objects_msg

    def update_vision_data_callback(self, _, response: Trigger.Response):
        self.read_from_vision_system()

        response.success = True
        response.message = "Vision Data Updated Successfully"
        
        self.get_logger().info('Vision Data Updated.')
        
        return response

    def generate_unique_name(self, object_identifier: int) -> str:
        identifier = self.object_name_mappings.get(object_identifier)
        
        if identifier not in self.object_counters:
            self.object_counters[identifier] = 1
        else:
            self.object_counters[identifier] += 1
        
        return f'{identifier}_{self.object_counters[identifier]}'

    def get_object_type(self, object_type) -> int:
        return self.object_type_mappings.get(object_type) or -1

    def get_object_identifier(self, name: str) -> int:
        return self.object_identifier_mappings.get(name) or -1

    def euler_to_quaternion(self, roll: float, pitch: float, yaw: float) -> tuple[float, float, float, float]:
        qx = math.sin(roll/2) * math.cos(pitch/2) * math.cos(yaw/2) - math.cos(roll/2) * math.sin(pitch/2) * math.sin(yaw/2)
        qy = math.cos(roll/2) * math.sin(pitch/2) * math.cos(yaw/2) + math.sin(roll/2) * math.cos(pitch/2) * math.sin(yaw/2)
        qz = math.cos(roll/2) * math.cos(pitch/2) * math.sin(yaw/2) - math.sin(roll/2) * math.sin(pitch/2) * math.cos(yaw/2)
        qw = math.cos(roll/2) * math.cos(pitch/2) * math.cos(yaw/2) + math.sin(roll/2) * math.sin(pitch/2) * math.sin(yaw/2)
        
        return (qx, qy, qz, qw)