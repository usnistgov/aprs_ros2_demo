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

    while not commander.planning_scene_ready:
        sleep(0.1)

    commander.pick_from_slot('m2l1_kit_tray_02_lg_1')

    # Wait for planning scene to be ready

    # Move the robot to conveyor configuration

    # Move to above gear

    # Move to gear

    # Close gripper

    # Move to above gear

if __name__ == '__main__':
    main()