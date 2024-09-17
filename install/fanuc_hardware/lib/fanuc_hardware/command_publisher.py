#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from example_interfaces.srv import Trigger
from std_msgs.msg import Float64MultiArray
import time



class MinimalPublisher(Node):

    def __init__(self):
        super().__init__('minimal_publisher')
        self.publisher_ = self.create_publisher(Float64MultiArray, '/forward_position_controller/commands', 10)

        self.gripper_open_client = self.create_client(Trigger, 'gripper_open')
        while not self.gripper_open_client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('service not available, waiting again...')

        self.gripper_close_client = self.create_client(Trigger, 'gripper_close')
        while not self.gripper_close_client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('service not available, waiting again...')

        self.req = Trigger.Request()

        self.joint_message = [0.0,0.0,0.0,0.0,0.0,0.0]

    def publish_message(self):
        joint_move = Float64MultiArray()
        joint_move.data = self.joint_message
        self.publisher_.publish(joint_move)
        self.get_logger().info(f'Publishing: "{joint_move.data}"')
        rclpy.spin_once(self, timeout_sec = 0.1)

def main(args=None):
    rclpy.init(args=args)

    minimal_publisher = MinimalPublisher()

    minimal_publisher.joint_message = [1.775558355,0.54021431,-1.0296919,-0.001850049,-0.555136875,-0.06736971]
    minimal_publisher.publish_message()

    minimal_publisher.joint_message = [1.775558355,0.71111695,-1.19036691,-0.0025307274,-0.394479318,-0.066601764]
    minimal_publisher.publish_message()

    time.sleep(5.0)

    future_close = minimal_publisher.gripper_close_client.call_async(minimal_publisher.req)

    minimal_publisher.joint_message = [1.775558355,0.54021431,-1.0296919,-0.001850049,-0.555136875,-0.06736971]
    minimal_publisher.publish_message()

    minimal_publisher.joint_message = [1.73300723,0.913784583,-0.88924525,-0.00057595865,-0.695758053,-0.025935593]
    minimal_publisher.publish_message()

    time.sleep(3.0)

    future_open = minimal_publisher.gripper_open_client.call_async(minimal_publisher.req)

    minimal_publisher.joint_message = [1.891500577,-0.181374616,-0.15594517,-0.0028099801,-1.42918031,-0.184394035]
    minimal_publisher.publish_message()

    minimal_publisher.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()