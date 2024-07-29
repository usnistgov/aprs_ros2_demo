#!/usr/bin/env python3
import rclpy
from aprs_vision.vision_publisher import VisionPublisher

def main(args=None):
    rclpy.init(args=args)
    
    vision_publisher_node = VisionPublisher()
    
    try:
        rclpy.spin(vision_publisher_node)
    except KeyboardInterrupt:
        vision_publisher_node.destroy_node()
    finally:
        rclpy.shutdown()

if __name__ == '__main__':
    main()
