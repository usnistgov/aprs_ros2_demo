import rclpy
from rclpy.node import Node

from std_msgs.msg import Float64MultiArray


class MinimalPublisher(Node):

    def __init__(self):
        super().__init__('minimal_publisher')
        self.publisher_ = self.create_publisher(Float64MultiArray, '/forward_position_controller/commands', 10)
        
        timer_period = 0.5  # seconds
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.i = 0

    def timer_callback(self):
        msg = Float64MultiArray()
    
        msg.data = [0.0,0.100388,-0.0833605,0.00523022,-1.50248,-3.14152]
        
        self.publisher_.publish(msg)

def main(args=None):
    rclpy.init(args=args)

    minimal_publisher = MinimalPublisher()
    
    rclpy.spin(minimal_publisher)

    minimal_publisher.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()