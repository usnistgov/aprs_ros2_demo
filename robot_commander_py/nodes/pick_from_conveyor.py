#!/usr/bin/env python3

from time import sleep

import threading

import rclpy

from rclpy.executors import MultiThreadedExecutor

from robot_commander_py.robot_commander import RobotCommander

def run(executor: MultiThreadedExecutor, stop_event: threading.Event):
    while not stop_event.is_set():
        executor.spin_once()

def main(args=None):
    rclpy.init(args=args)

    commander = RobotCommander()

    executor = MultiThreadedExecutor()

    executor.add_node(commander)

    executor.spin()

    # stop_event = threading.Event()

    # spin_thread = threading.Thread(target=run, args=(executor, stop_event))
    # spin_thread.start()

    # while not commander.planning_scene_ready:
    #     sleep(0.1)

    # sleep(1)

    # # commander.move_to_named_configuration('home')

    # if not commander.pick_from_slot('m2l1_kit_tray_02_mg_1'):
    #     return 

    # if not commander.place_in_slot('m2l1_kit_tray_02_mg_2'):
    #     return
    
    # stop_event.set()

    # spin_thread.join()

if __name__ == '__main__':
    main()