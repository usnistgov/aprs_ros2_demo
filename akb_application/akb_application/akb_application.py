import threading

import rclpy

from rclpy.node import Node
from rclpy.qos import qos_profile_default
from rclpy.executors import MultiThreadedExecutor

from aprs_interfaces.msg import Trays

from akb_application.app import App

class AKBApplication(Node):
    def __init__(self):
        super().__init__("akb_application")

        self.root = App(self)

def main(args=None):
    rclpy.init(args=args)

    akb_app = AKBApplication()

    executor = MultiThreadedExecutor()
    executor.add_node(akb_app)

    spin_thread = threading.Thread(target=executor.spin)
    spin_thread.start()

    akb_app.root.mainloop()

    executor.shutdown()

if __name__ == '__main__':
    main()
