#!/usr/bin/env python3
import rclpy
from aprs_vision.antvision_publisher import AntVisionPublisher

def main(args=None):
    rclpy.init(args=args)
    antvision_publisher_node = AntVisionPublisher()
    try:
        rclpy.spin(antvision_publisher_node)
    except KeyboardInterrupt:
        antvision_publisher_node.destroy_node()
    finally:
        rclpy.shutdown()

if __name__ == '__main__':
    main()
