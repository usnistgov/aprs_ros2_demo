
import rclpy
from rclpy.node import Node
from rclpy.executors import MultiThreadedExecutor

from time import sleep

from aprs_interfaces.srv import LocateTrays, PneumaticGripperControl, MoveToNamedPose
from std_msgs.msg import Bool
from example_interfaces.srv import Trigger

from akb_application.settings import *

class MinimalGripperService(Node):
    def __init__(self, name, srv):
        super().__init__(name)
        self.srv = self.create_service(PneumaticGripperControl, srv, self.cb)

    def cb(self, request: PneumaticGripperControl.Request, response: PneumaticGripperControl.Response):
        response.success = True
        
        sleep(0.2)

        return response

class MinimalTrayService(Node):
    def __init__(self, name, srv):
        super().__init__(name)
        self.srv = self.create_service(LocateTrays, srv, self.cb)

    def cb(self, _, response: LocateTrays.Response):
        response.success = True
        
        sleep(0.2)

        return response
    
class MinimalMoveToPoseService(Node):
    def __init__(self, name, robot, srv):
        super().__init__(name)
        self.srv = self.create_service(MoveToNamedPose, srv, self.cb)
        self.robot = robot

    def cb(self, request: MoveToNamedPose.Request, response: MoveToNamedPose.Response):
        if request.name in NAMED_POSES[self.robot]:
            response.success = True
        else:
            response.success = False
        
        sleep(0.2)

        return response

class MinimalPlanningSceneService(Node):
    def __init__(self, name, srv):
        super().__init__(name)
        self.srv = self.create_service(Trigger, srv, self.cb)

    def cb(self, _, response: Trigger.Response):
        response.success = True
        
        sleep(0.2)

        return response

class MinimalPublisher(Node):

    def __init__(self, name, topic, value):
        super().__init__(name)
        self.pub = self.create_publisher(Bool, topic, 1)
        self.value = value
        
        self.create_timer(1, self.cb)

    def cb(self):
        msg = Bool()
        msg.data = self.value
        
        self.pub.publish(msg)


def main(args=None):
    rclpy.init(args=args)

    executor = MultiThreadedExecutor()
    
    for area, srv in LOCATE_TRAYS_NAMES.items():
        executor.add_node(MinimalTrayService(f'{area}_server', srv))
        
    for robot, srv in ACTUATE_GRIPPER_NAMES.items():
        executor.add_node(MinimalGripperService(f'{robot}_gripper_server', srv))
        
    for robot, srv in MOVE_TO_POSE_NAMES.items():
        executor.add_node(MinimalMoveToPoseService(f'{robot}_move_to_pose_server', robot, srv))
        
    for robot, srv in PLANNING_SCENE_NAMES.items():
        executor.add_node(MinimalPlanningSceneService(f'{robot}_planning_scene_server', srv))
    
    for robot in ['fanuc', 'motoman']:
        for status_type, topic in CONTROLLER_STATUS_NAMES[robot].items():
            name = f'{robot}_{status_type}_publisher'
            executor.add_node(MinimalPublisher(name, topic, True))
    
    try:
        executor.spin()
    except KeyboardInterrupt:
        executor.shutdown()

if __name__ == '__main__':
    main()