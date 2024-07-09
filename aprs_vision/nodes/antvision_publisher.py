#!/usr/bin/env python3
from numpy import object0
import rclpy
from rclpy.node import Node

from typing import Optional

from aprs_vision.antvision_read import AntVisionUtility
from aprs_interfaces.msg import Object, Objects

import math
from time import sleep

class ObjectLocationNode(Node):
    def __init__(self):
        super().__init__('object_location_node')
        self.publisher_ = self.create_publisher(Objects, 'object_locations', 10)
        self.object_counters = {}
        self.objects: Optional[Objects] = None
        self.build_objects_msg()
        self.timer = self.create_timer(0.5, self.publish)

    def publish(self):
        if self.objects:
            self.publisher_.publish(self.objects)

    def build_objects_msg(self):
        object_locations = AntVisionUtility().get_object_locations()
        if not object_locations:
            return

        self.objects = Objects()
        for name, rotation, x, y, type_ in object_locations:
            object = Object()
            object.object_identifier = self.get_object_identifier(name)
            object.name = self.generate_unique_name(object.object_identifier )
            object.object_type = self.get_object_type(type_)

            object.pose_stamped.header.frame_id = "fanuc_base_link"
            object.pose_stamped.pose.position.x = x
            object.pose_stamped.pose.position.y = y
            if object.object_type == Object.PART:
                object.pose_stamped.pose.position.z = 0.03
            else:
                object.pose_stamped.pose.position.z = 0.0
            
            quaternion = self.euler_to_quaternion(0, 0, rotation)
            object.pose_stamped.pose.orientation.x = quaternion[0]
            object.pose_stamped.pose.orientation.y = quaternion[1]
            object.pose_stamped.pose.orientation.z = quaternion[2]
            object.pose_stamped.pose.orientation.w = quaternion[3]
            
            self.objects.objects.append(object) # type: ignore

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

    def euler_to_quaternion(self, roll, pitch, yaw):
        qx = math.sin(roll/2) * math.cos(pitch/2) * math.cos(yaw/2) - math.cos(roll/2) * math.sin(pitch/2) * math.sin(yaw/2)
        qy = math.cos(roll/2) * math.sin(pitch/2) * math.cos(yaw/2) + math.sin(roll/2) * math.cos(pitch/2) * math.sin(yaw/2)
        qz = math.cos(roll/2) * math.cos(pitch/2) * math.sin(yaw/2) - math.sin(roll/2) * math.sin(pitch/2) * math.cos(yaw/2)
        qw = math.cos(roll/2) * math.cos(pitch/2) * math.cos(yaw/2) + math.sin(roll/2) * math.sin(pitch/2) * math.sin(yaw/2)
        return [qx, qy, qz, qw]

def main(args=None):
    rclpy.init(args=args)
    node = ObjectLocationNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.destroy_node()
    finally:
        rclpy.shutdown()

if __name__ == '__main__':
    main()
