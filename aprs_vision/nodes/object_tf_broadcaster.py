#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from aprs_vision.tf_broadcaster import TFBroadcaster
from aprs_interfaces.msg import Objects

class ObjectTFBroadcaster(Node):
    def __init__(self):
        super().__init__('object_tf_broadcaster')
        self.tf_broadcaster = TFBroadcaster("objects_tf_broadcaster")
        self.subscription = self.create_subscription(Objects, 'object_locations', self.listener_callback, 10)

    def listener_callback(self, msg):
        for object_pose in msg.objects:
            self.tf_broadcaster.generate_transform(object_pose.pose_stamped.header.frame_id, object_pose.name, object_pose.pose_stamped.pose)

        self.tf_broadcaster.send_transforms()
        self.get_logger().info("Published Transforms")

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
