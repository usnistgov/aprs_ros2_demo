#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from example_interfaces.srv import Trigger
import socket
import struct
import time

CONFIG = {
        "port":12123,
        "host_ip":"192.168.1.34"
        }

class GripperUtility:

    def __init__(self, config=CONFIG, DEBUG=False):
        self.config = config
        self.open_msg = struct.pack("!i", 0)
        self.close_msg = struct.pack("!i", 1)
    
    def send_message(self, msg):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((self.config['host_ip'], self.config['port']))
        for _ in range(3):
            time.sleep(0.1)
            s.send(msg)
        s.close()
        return
    
    def open_gripper(self):
        self.send_message(self.open_msg)
        return
    
    def close_gripper(self):
        self.send_message(self.close_msg)
        return

class GripperControl(Node):
    def __init__(self):
        super().__init__('gripper_control')
        self.srv = self.create_service(Trigger, 'gripper_open', self.gripper_open_callback)
        self.srv = self.create_service(Trigger, 'gripper_close', self.gripper_close_callback)
        self.gripper_util = GripperUtility()

    def gripper_open_callback(self, request, response):
        self.gripper_util.open_gripper()
        response.success = True
        response.message = "Gripper Opened Successfully"
        self.get_logger().info('Gripper Opened.')

        return response
    
    def gripper_close_callback(self, request, response):
        self.gripper_util.close_gripper()
        response.success = True
        response.message = "Gripper Closed Successfully"
        self.get_logger().info('Gripper Closed.')

        return response
    
def main(args=None):
    rclpy.init(args=args)

    gripper_control = GripperControl()
    try:
        rclpy.spin(gripper_control)
    except KeyboardInterrupt:
        pass

    gripper_control.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()