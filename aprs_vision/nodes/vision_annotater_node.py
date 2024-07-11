#!/usr/bin/env python3
import rclpy
from aprs_vision.vision_annotater import VisionAnnotater

def main(args=None):
    rclpy.init(args=args)

    vision_annotater_node = VisionAnnotater()

    try:
        rclpy.spin(vision_annotater_node)
    except KeyboardInterrupt:
        vision_annotater_node.destroy_node()
    finally:
        rclpy.shutdown()

if __name__ == '__main__':
    main()
