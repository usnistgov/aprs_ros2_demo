#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from aprs_vision.tf_broadcaster import TFBroadcaster
from aprs_interfaces.msg import Object, Objects

class ObjectTFBroadcaster(Node):
    def __init__(self):
        super().__init__('object_tf_broadcaster')
        self.tf_broadcaster = TFBroadcaster("objects_tf_broadcaster")
        self.subscription = self.create_subscription(Objects, 'object_locations', self.listener_callback, 10)

    def listener_callback(self, msg: Objects):
        for object in msg.objects:
            object: Object
            self.tf_broadcaster.generate_transform(object.pose_stamped.header.frame_id, object.name, object.pose_stamped.pose)

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
