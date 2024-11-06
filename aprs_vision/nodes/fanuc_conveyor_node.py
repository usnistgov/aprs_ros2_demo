#!/usr/bin/env python3
import rclpy
from aprs_vision.fanuc_conveyor import FanucConveyor

def main(args=None):
    rclpy.init(args=args)

    fanuc_conveyor_node = FanucConveyor()

    try:
        rclpy.spin(fanuc_conveyor_node)
    except KeyboardInterrupt:
        fanuc_conveyor_node.destroy_node()

if __name__ == '__main__':
    main()