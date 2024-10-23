#!/usr/bin/env python3
import rclpy
from aprs_vision.fanuc_conveyer import FanucConveyer

def main(args=None):
    rclpy.init(args=args)

    fanuc_conveyer_node = FanucConveyer()

    try:
        rclpy.spin(fanuc_conveyer_node)
    except KeyboardInterrupt:
        fanuc_conveyer_node.destroy_node()

if __name__ == '__main__':
    main()