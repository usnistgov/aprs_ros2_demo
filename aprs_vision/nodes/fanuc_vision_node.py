#!/usr/bin/env python3
import rclpy
from aprs_vision.fanuc_table import FanucTable

def main(args=None):
    rclpy.init(args=args)

    fanuc_vision_node = FanucTable()

    try:
        rclpy.spin(fanuc_vision_node)
    except KeyboardInterrupt:
        fanuc_vision_node.destroy_node()

if __name__ == '__main__':
    main()
