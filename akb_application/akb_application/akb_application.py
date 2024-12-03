import rclpy
from rclpy.node import Node

from akb_application.app import App

class AKBApplication(Node):
    def __init__(self):
        super().__init__("akb_application")

        self.root = App()

def main(args=None):
    rclpy.init(args=args)

    akb_app = AKBApplication()

    try:
        akb_app.root.mainloop()
    except KeyboardInterrupt:
        pass

if __name__ == '__main__':
    main()
