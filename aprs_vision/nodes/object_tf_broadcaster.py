#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from aprs_vision.tf_broadcaster import TFBroadcaster
from aprs_interfaces.msg import Object, Objects
from geometry_msgs.msg import Pose

class ObjectTFBroadcaster(Node):
    def __init__(self):
        super().__init__('object_tf_broadcaster')
        self.tf_broadcaster = TFBroadcaster("objects_tf_broadcaster")
        self.subscription = self.create_subscription(Objects, 'object_locations', self.listener_callback, 10)
        self.slot_offsets = {
            Object.S2L2_KIT_TRAY: {
                'lg_1': self.create_pose(-0.054000, -0.028000),
                'lg_2': self.create_pose(0.054000, -0.028000),
                'sg_1': self.create_pose(-0.037, 0.055000),
                'sg_2': self.create_pose(0.037, 0.055000),
            },
            Object.LARGE_GEAR_TRAY: {
                'slot_1': self.create_pose(0.059000, 0),
                'slot_2': self.create_pose(-0.059000, 0),
            },
            Object.MEDIUM_GEAR_TRAY: {
                'slot_1': self.create_pose(-0.037500, 0.037500),
                'slot_2': self.create_pose(0.037500, 0.037500),
                'slot_3': self.create_pose(-0.037500, -0.037500),
                'slot_4': self.create_pose(0.037500, -0.037500),
            },
            Object.M2L1_KIT_TRAY: {
                'lg_1': self.create_pose(0.038970, 0.007460),
                'mg_1': self.create_pose(-0.051710, 0.038460),
                'mg_2': self.create_pose(-0.051710, -0.038460),
            },
            Object.SMALL_GEAR_TRAY: {
                'slot_1': self.create_pose(-0.026500, 0.026500),
                'slot_2': self.create_pose(0.026500, 0.026500),
                'slot_3': self.create_pose(-0.026500, -0.026500),
                'slot_4': self.create_pose(0.026500, -0.026500),
            },
        }

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
    
    def listener_callback(self, msg):
        for object in msg.objects:
            object: Object
            self.tf_broadcaster.generate_transform(object.pose_stamped.header.frame_id, object.name, object.pose_stamped.pose)

            slot_info = self.slot_offsets.get(object.object_identifier)
            if slot_info:
                for slot_name, slot_pose in slot_info.items():
                    slot_frame_id = f"{object.name}_{slot_name}"
                    self.tf_broadcaster.generate_transform(object.name, slot_frame_id, slot_pose)

        self.tf_broadcaster.send_transforms()

def main(args=None):
    rclpy.init(args=args)

    node = ObjectTFBroadcaster()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.destroy_node()
    finally:
        rclpy.shutdown()

if __name__ == '__main__':
    main()
