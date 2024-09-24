#!/usr/bin/env python3

import rclpy
from aprs_vision.teach_table import TeachTable

def main(args=None):
    rclpy.init(args=args)

    teach_table_vision_node = TeachTable()

    try:
        rclpy.spin(teach_table_vision_node)
    except KeyboardInterrupt:
        teach_table_vision_node.destroy_node()

if __name__ == '__main__':
    main()