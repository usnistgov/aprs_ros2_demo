#!/usr/bin/env python3

from typing import Optional

import threading
from time import sleep

import rclpy
import rclpy.action
import rclpy.duration
from rclpy.node import Node
from rclpy.action import ActionClient
from rclpy.executors import MultiThreadedExecutor
from rclpy.duration import Duration

from control_msgs.action import FollowJointTrajectory

from sensor_msgs.msg import JointState
from trajectory_msgs.msg import JointTrajectoryPoint

class RobotMoveTest(Node):

    def __init__(self):
        super().__init__('robot_move_test')
        self.subscription = self.create_subscription(
            JointState,
            '/motoman/joint_states',
            self.joint_state_cb_,
            10)
        
        self.action_client = ActionClient(self, FollowJointTrajectory, "/motoman/joint_trajectory_controller/follow_joint_trajectory")

        self.current_positions: Optional[dict[str, float]] = None

    def joint_state_cb_(self, msg: JointState):
        self.current_positions = {}
        
        for joint_name, position in zip(msg.name, msg.position):
            self.current_positions[joint_name] = position
    
    def move_robot(self) -> Optional[FollowJointTrajectory.Goal]:
        if self.current_positions is None:
            return None
    
        goal = FollowJointTrajectory.Goal()
        goal.trajectory.joint_names = [f"joint_{i}" for i in "sleurbt"]

        # Create trajectory points
        point_1 = JointTrajectoryPoint()
        
        for joint_name in goal.trajectory.joint_names:
            point_1.positions.append(self.current_positions[joint_name])
        
        point_1.time_from_start = Duration(seconds=20).to_msg()
        point_1.positions[0] = 1.57
        point_1.positions[1] = 0.0
        point_1.positions[2] = 0.0
        point_1.positions[3] = -1.57
        point_1.positions[4] = 0.0
        point_1.positions[5] = -1.57
        point_1.positions[6] = 0.0
        
        goal.trajectory.points.append(point_1)

        # point_2 = JointTrajectoryPoint()
        
        # for joint_name in goal.trajectory.joint_names:
        #     point_2.positions.append(self.current_positions[joint_name])
        
        # point_2.time_from_start = Duration(seconds=10).to_msg()
        # point_2.positions[0] -= 0.25
        # point_2.positions[1] -= 0.25
        
        # goal.trajectory.points.append(point_2)

        return goal


def main(args=None):
    rclpy.init(args=args)

    node = RobotMoveTest()

    executor = MultiThreadedExecutor()
    executor.add_node(node)

    spin_thread = threading.Thread(target=executor.spin)
    spin_thread.start()

    while node.current_positions is None:
        sleep(0.1)
    
    goal = node.move_robot()

    if goal is not None:
        future = node.action_client.send_goal_async(goal)

        while not future.done():
            sleep(0.1)

        goal_handle: rclpy.action.client.ClientGoalHandle = future.result() # type: ignore
        
        result: FollowJointTrajectory.Result = goal_handle.get_result().result

        node.get_logger().info(f'Error Code: {result.error_code}')
        node.get_logger().info(f'Error String: {result.error_string}')

    else:
        node.get_logger().error("Unable to create goal")
    
    executor.shutdown()

if __name__ == '__main__':
    main()