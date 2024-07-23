#!/usr/bin/env python3
import rclpy
from aprs_vision.vision_annotator import VisionAnnotator

def main(args=None):
    rclpy.init(args=args)

    vision_annotator_node = VisionAnnotator()

    try:
        rclpy.spin(vision_annotator_node)
    except KeyboardInterrupt:
        vision_annotator_node.destroy_node()
    finally:
        rclpy.shutdown()

if __name__ == '__main__':
    main()
