#!/usr/bin/env python3

from conveyor_interfaces.srv import EnableConveyor, SetConveyorState
import rclpy
import threading
from rclpy.client import Client
from rclpy.executors import MultiThreadedExecutor
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup
from aprs_vision.stream_handler import StreamHandler
import os
from ament_index_python.packages import get_package_share_directory

from rclpy.node import Node

from aprs_interfaces.srv import MoveConveyor

import cv2
from cv2.typing import MatLike

from typing import Optional

import numpy as np

import time

class DemoConveyorControl(Node):

    toward_motoman = 1
    toward_fanuc = 0
    base_speed = 30.0

    def __init__(self):
        super().__init__('Conveyor_Control_Client')

        self.result_image: Optional[MatLike] = None
        self.rect_image: MatLike = np.zeros((100,100))
        self.fanuc_frame: Optional[MatLike] = None
        self.motoman_frame: Optional[MatLike] = None

        # self.enable_complete = False
        # self.state_complete = False

        group = MutuallyExclusiveCallbackGroup()

        self.move_conveyor_to_fanuc_srv = self.create_service(MoveConveyor,"/conveyor/move_to_fanuc", self.move_fanuc_cb, callback_group=group)
        self.move_conveyor_to_motoman_srv = self.create_service(MoveConveyor,"/conveyor/move_to_motoman", self.move_motoman_cb, callback_group=group)

        self.enable_client = self.create_client(EnableConveyor,'/conveyor/enable')
        self.set_state_client = self.create_client(SetConveyorState,'/conveyor/set_state')

        while not self.enable_client.wait_for_service(timeout_sec=1.0):
            self.get_logger().warn('service not available, waiting again...')

        share_path = get_package_share_directory('aprs_vision')
        calibration_filepath_fanuc = os.path.join(share_path, 'config', 'fanuc_conveyor_calibration.npz')
        calibration_filepath_motoman = os.path.join(share_path, 'config', 'motoman_conveyor_calibration.npz')

        self.fanuc_conveyor = StreamHandler('http://192.168.1.108/mjpg/video.mjpg', calibration_filepath_fanuc)
        self.motoman_conveyor = StreamHandler('http://192.168.1.107/mjpg/video.mjpg', calibration_filepath_motoman)

        self.get_frame()

        timer_group = MutuallyExclusiveCallbackGroup()

        self.frame_timer = self.create_timer(timer_period_sec=0.02, callback=self.get_frame, callback_group=timer_group)

        while self.fanuc_frame is None or self.motoman_frame is None:
            self.get_logger().info('Unable to connect to cameras')

        current_location = self.find_conveyor()
        if current_location == self.toward_fanuc:
            self.get_logger().info('The conveyor is located at the fanuc')
        elif current_location == self.toward_motoman:
            self.get_logger().info('The conveyor is located at the motoman')
        elif current_location == -1:
            self.get_logger().info('The conveyor is located at neither robot')

    # def send_enable_request(self, enabled: bool):
    #     self.req_enable.enable = enabled
    #     return self.cli_enable.call_async(self.req_enable)

    # def send_state_request(self, speed: float, direction: int):
    #     self.req_state.direction = direction
    #     self.req_state.speed = speed
    #     return self.cli_state.call_async(self.req_state)

    def get_frame(self):
        self.fanuc_frame = self.fanuc_conveyor.read_frame()
        self.motoman_frame = self.motoman_conveyor.read_frame()

    def finish_movement(self, movement_direction: int) -> bool:
        thresh = self.image_thresholding(movement_direction)

        if thresh is None:
            return False   

        edge_rectangle = thresh[0:thresh.shape[0], 0:100]
        top_rectangle = thresh[0:20, 0:100]
        bottom_rectangle= thresh[thresh.shape[0]-20:thresh.shape[0], 0:100]

        self.rect_image = cv2.rectangle(self.result_image, (0,0), (100, thresh.shape[0]), (255,0,0), 3) #type: ignore

        if cv2.countNonZero(edge_rectangle) > 10000 and cv2.countNonZero(bottom_rectangle) > 100 and cv2.countNonZero(top_rectangle) > 100:
            return True
        
        return False
    
    def fine_tune_movement(self, location: int) -> bool:
        thresh = self.image_thresholding(location)

        if thresh is None:
            return False   

        edge_rectangle = thresh[0:thresh.shape[0], 0:25]
        top_rectangle = thresh[0:20, 0:25]
        bottom_rectangle= thresh[thresh.shape[0]-20:thresh.shape[0], 0:25]

        self.rect_image = cv2.rectangle(self.result_image, (0,0), (25, thresh.shape[0]), (255,0,0), 3) #type: ignore

        if cv2.countNonZero(edge_rectangle) > 10000 and cv2.countNonZero(bottom_rectangle) > 100 and cv2.countNonZero(top_rectangle) > 100:
            return True
        
        return False

    def determine_adjustment_action(self, movement_direction: int) -> int:

        thresh = self.image_thresholding(movement_direction)

        if thresh is None:
            return -1        

        edge_rectangle_small = thresh[0:thresh.shape[0], 0:25]
        top_rectangle_small = thresh[0:20, 0:25]
        bottom_rectangle_small = thresh[thresh.shape[0]-20:thresh.shape[0], 0:25]

        edge_rectangle_large = thresh[0:thresh.shape[0], 0:200]
        top_rectangle_large = thresh[0:20, 0:200]
        bottom_rectangle_large = thresh[thresh.shape[0]-20:thresh.shape[0], 0:200]

        if cv2.countNonZero(edge_rectangle_small) > 10000 and cv2.countNonZero(bottom_rectangle_small) > 100 and cv2.countNonZero(top_rectangle_small) > 100:
            return 0

        if cv2.countNonZero(edge_rectangle_large) > 10000 and cv2.countNonZero(bottom_rectangle_large) > 100 and cv2.countNonZero(top_rectangle_large) > 100:
            return 1
        
        return 2

    def image_thresholding(self, location: int) -> Optional[MatLike]:
        if self.motoman_frame is None or self.fanuc_frame is None:
                return None
            
        if location == self.toward_motoman:
            self.result_image = self.motoman_frame.copy()
        elif location == self.toward_fanuc:
            self.result_image = self.fanuc_frame.copy()

        if self.result_image is None:
            return None

        hsv = cv2.cvtColor(self.result_image, cv2.COLOR_BGR2HSV) 

        return cv2.inRange(hsv, (0, 0, 0), (255, 255, 180)) #type: ignore
        
    def move_motoman_cb(self, request: MoveConveyor.Request, response: MoveConveyor.Response) -> MoveConveyor.Response:
            return self.move_conveyor(self.toward_motoman,response)
    
    def move_fanuc_cb(self, request: MoveConveyor.Request, response: MoveConveyor.Response) -> MoveConveyor.Response:
            return self.move_conveyor(self.toward_fanuc,response)
    
    def move_conveyor(self, direction: int, response: MoveConveyor.Response) -> MoveConveyor.Response:
        
        resp= self.enable_conveyor(True)
        if not resp.success:
            self.get_logger().warn(resp.message)

        resp= self.set_conveyor_state(self.base_speed, direction)
        if not resp.success:
            self.get_logger().warn(resp.message)

        current_time = time.time()

        while (time.time() - current_time) < 10:

            if self.finish_movement(direction):
                break

        if (time.time() - current_time) >= 10:
            response.success = False
            response.message = 'Conveyor movement timeout'

            resp= self.enable_conveyor(False)
            if not resp.success:
                self.get_logger().warn(resp.message)

            return response

        if direction == self.toward_fanuc:
            opposite_direction = self.toward_motoman
        else:
            opposite_direction = self.toward_fanuc


        resp= self.set_conveyor_state(0.0, direction)
        if not resp.success:
            self.get_logger().warn(resp.message)

        time.sleep(1.0)

        current_state = self.determine_adjustment_action(direction)

        location = direction

        if current_state == -1:
            response.success = False
            response.message = "Lost contact with camera"

            return response

        elif current_state == 0:
            response.success = True
            response.message ="Conveyor successfully moved"

            return response
        
        elif current_state == 1:
            if direction == self.toward_motoman:
                direction = opposite_direction

        elif current_state == 2:
            if direction == self.toward_fanuc:
                direction = opposite_direction

        resp= self.set_conveyor_state(self.base_speed/6, direction)
        if not resp.success:
            self.get_logger().warn(resp.message)

        current_time = time.time()

        while (time.time() - current_time) < 5:

            if self.fine_tune_movement(location):
                break

        if (time.time() - current_time) >= 5:
            response.success = False
            response.message = 'Conveyor movement timeout'

            resp= self.enable_conveyor(False)
            if not resp.success:
                self.get_logger().warn(resp.message)

            return response

        resp= self.enable_conveyor(False)
        if not resp.success:
            self.get_logger().warn(resp.message)

        response.success = True
        response.message = "Conveyor successfully moved"

        return response

    def enable_conveyor(self, enable: bool) -> EnableConveyor.Response:
        req = EnableConveyor.Request()
        req.enable = enable

        future = self.enable_client.call_async(req)

        while not future.done():
            time.sleep(0.1)

        result: EnableConveyor.Response = future.result() # type: ignore
        
        return result
    
    def set_conveyor_state(self, speed: float, direction: int) -> SetConveyorState.Response:
        req = SetConveyorState.Request()
        req.speed = speed
        req.direction = direction

        future = self.set_state_client.call_async(req)

        while not future.done():
            time.sleep(0.1)

        result: SetConveyorState.Response = future.result() # type: ignore
        
        return result
    
    def find_conveyor(self) -> int:
        if self.motoman_frame is None or self.fanuc_frame is None:
            return False
        
        hsv_fanuc = cv2.cvtColor(self.fanuc_frame, cv2.COLOR_BGR2HSV) 
        hsv_motoman = cv2.cvtColor(self.motoman_frame, cv2.COLOR_BGR2HSV) 

        thresh_fanuc = cv2.inRange(hsv_fanuc, (0, 0, 0), (255, 255, 180)) #type: ignore
        thresh_motoman = cv2.inRange(hsv_motoman, (0, 0, 0), (255, 255, 180)) #type: ignore

        edge_rectangle_fanuc = thresh_fanuc[0:thresh_fanuc.shape[0], 0:25]
        edge_rectangle_motoman = thresh_motoman[0:thresh_motoman.shape[0], 0:25]
        top_rectangle_fanuc = thresh_fanuc[0:20, 0:100]
        bottom_rectangle_fanuc= thresh_fanuc[thresh_fanuc.shape[0]-20:thresh_fanuc.shape[0], 0:100]
        top_rectangle_motoman = thresh_motoman[0:20, 0:100]
        bottom_rectangle_motoman= thresh_motoman[thresh_motoman.shape[0]-20:thresh_motoman.shape[0], 0:100]

        if cv2.countNonZero(edge_rectangle_fanuc) > 10000 & cv2.countNonZero(bottom_rectangle_fanuc) > 10:
            if cv2.countNonZero(top_rectangle_fanuc) > 10:
                return self.toward_fanuc
            
        if cv2.countNonZero(edge_rectangle_motoman) > 10000 & cv2.countNonZero(bottom_rectangle_motoman) > 10:
            if cv2.countNonZero(top_rectangle_motoman) > 10:
                return self.toward_motoman
            
        return -1

def main(args=None):
    rclpy.init(args=args)

    conveyor_control = DemoConveyorControl()

    executor = MultiThreadedExecutor()
    executor.add_node(conveyor_control)

    executor.spin()

    # spin_thread = threading.Thread(target=executor.spin)
    # spin_thread.start()

    # try:
    #     while conveyor_control.result_image is None:
    #         time.sleep(0.1)
    #     while True:
    #         cv2.imshow('fanuc', conveyor_control.fanuc_frame)
    #         cv2.imshow('motoman', conveyor_control.motoman_frame)
    #         cv2.waitKey(1)

    # except KeyboardInterrupt:
    #     cv2.destroyAllWindows()
    #     pass

if __name__ == '__main__':
    main()