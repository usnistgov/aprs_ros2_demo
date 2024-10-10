#!/usr/bin/env python3
import rclpy
from aprs_vision.motoman_conveyer import MotomanConveyer

def main(args=None):
    rclpy.init(args=args)

    motoman_conveyer_node = MotomanConveyer()

    try:
        rclpy.spin(motoman_conveyer_node)
    except KeyboardInterrupt:
        motoman_conveyer_node.destroy_node()

if __name__ == '__main__':
    main()