#!/usr/bin/env python3
import rclpy
from aprs_vision.vision_broadcaster import VisionBroadcaster

def main(args=None):
    rclpy.init(args=args)
    vision_broadcaster_node = VisionBroadcaster()
    try:
        rclpy.spin(vision_broadcaster_node)
    except KeyboardInterrupt:
        vision_broadcaster_node.destroy_node()
    finally:
        rclpy.shutdown()

if __name__ == '__main__':
    main()