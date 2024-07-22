#!/usr/bin/env python3
import rclpy
from aprs_vision.tp import TeachTablePublisher

def main(args=None):
    rclpy.init(args=args)
    teach_table_publisher_node = TeachTablePublisher()
    try:
        rclpy.spin(teach_table_publisher_node)
    except KeyboardInterrupt:
        teach_table_publisher_node.destroy_node()
    finally:
        rclpy.shutdown()

if __name__ == '__main__':
    main()