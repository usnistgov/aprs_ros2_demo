#!/usr/bin/env python3
import rclpy
from rclpy.node import Node

from std_msgs.msg import Float64MultiArray



class MinimalPublisher(Node):

    def __init__(self):
        super().__init__('minimal_publisher')
        self.publisher_ = self.create_publisher(Float64MultiArray, '/forward_position_controller/commands', 10)
        # self.joint_messages = [[1.54,0.0,0.0,0.0,0.0,0.0],[1.54,0.0,0.0,1.54,0.0,0.0]]
        self.joint_messages = [[1.54,0.0,0.0,0.0,0.0,0.0]]
        
        # timer_period = 0.5  # seconds
        # self.timer = self.create_timer(timer_period, self.timer_callback)
        # self.i = 0


    # def timer_callback(self):
    #     msg = Float64MultiArray()
    
    #     msg.data = [1.779624972, 0.723229535, -1.1779227, 0.007574729, -0.421392295, -3.22257593]

    #     self.publisher_.publish(msg)

    def publish_message(self):
        for joint_value in self.joint_messages:

            joint_move = Float64MultiArray()

            joint_move.data = joint_value

            self.publisher_.publish(joint_move)

            self.get_logger().info(f'Publishing: "{joint_move.data}"')

            rclpy.spin_once(self, timeout_sec = 0.1)

def main(args=None):
    rclpy.init(args=args)

    minimal_publisher = MinimalPublisher()
    
    # rclpy.spin(minimal_publisher)
    minimal_publisher.publish_message()

    minimal_publisher.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()