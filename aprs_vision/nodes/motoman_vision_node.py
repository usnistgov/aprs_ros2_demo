#!/usr/bin/env python3
import rclpy
from aprs_vision.motoman_table import MotomanTable

def main(args=None):
    rclpy.init(args=args)

    motoman_vision_node = MotomanTable()

    try:
        rclpy.spin(motoman_vision_node)
    except KeyboardInterrupt:
        motoman_vision_node.destroy_node()

if __name__ == '__main__':
    main()
