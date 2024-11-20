#!/usr/bin/env python3
import rclpy
from aprs_vision.motoman_conveyor import MotomanConveyor

def main(args=None):
    rclpy.init(args=args)

    motoman_conveyor_node = MotomanConveyor()

    try:
        rclpy.spin(motoman_conveyor_node)
    except KeyboardInterrupt:
        motoman_conveyor_node.destroy_node()

if __name__ == '__main__':
    main()