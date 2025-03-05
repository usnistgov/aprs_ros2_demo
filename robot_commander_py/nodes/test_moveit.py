#!/usr/bin/env python3

from time import sleep

import threading

import rclpy

from rclpy.executors import MultiThreadedExecutor

from robot_commander_py.robot_commander import RobotCommander

def main(args=None):
    rclpy.init(args=args)

    commander = RobotCommander()

    executor = MultiThreadedExecutor()

    executor.add_node(commander)

    # executor.spin()

    spin_thread = threading.Thread(target=executor.spin)
    spin_thread.start()

    sleep(1)

    commander.planning_component.set_goal_state(configuration_name="home")

    commander.plan_and_execute()

    commander.planning_component.set_goal_state(configuration_name="above_table")

    commander.plan_and_execute()

    executor.shutdown()

if __name__ == '__main__':
    main()