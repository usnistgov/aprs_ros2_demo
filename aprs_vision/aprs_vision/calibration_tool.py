#!/usr/bin/env python3

import cv2
import numpy as np

from scipy.interpolate import griddata

from rclpy.node import Node

from typing import Optional
from cv2.typing import MatLike

class CameraException(Exception):
    pass

class CalibrationException(Exception):
    pass

class CalibrationTool(Node):       
    camera_ip_address = {
        'fanuc_table': '192.168.1.104',
        'fanuc_conveyor': '192.168.1.108',
        'motoman_table': '192.168.1.110',
        'motoman_conveyor': '192.168.1.107',
        'teach_table': '192.168.1.105',
    }
    
    background_hsv_bounds = {
        'fanuc_table': {
            'lower': (0, 0, 1), 
            'upper': (255, 255, 120)
        },
        'fanuc_conveyor': {
            'lower': (0, 0, 10), 
            'upper': (255, 255, 140)
        },
        'motoman_table': {
            'lower': (0, 0, 1), 
            'upper': (255, 255, 61)
        },
        'motoman_conveyor': {
            'lower': (0, 0, 10), 
            'upper': (255, 255, 125)
        },
        'teach_table': {
            'lower': (0, 0, 110), 
            'upper': (255, 255, 255)
        },
    }
    
    pixels_per_inch = 30
    crop_offset = -10
    max_value = 255
    max_value_H = 360//2   
    low_H_name = 'Low H'
    low_S_name = 'Low S'
    low_V_name = 'Low V'
    high_H_name = 'High H'
    high_S_name = 'High S'
    high_V_name = 'High V' 
    
    def __init__(self):
        super().__init__('calibration_tool')
        
        # Connect to camera stream
        self.declare_parameter('camera_name', "")
        
        self.camera_name = self.get_parameter('camera_name').get_parameter_value().string_value
        
        if self.camera_name == '':
            raise CameraException('Camera name parameter not set')
        
        if self.camera_name not in self.camera_ip_address.keys():
            raise CameraException(f'[{self.camera_name}] not a valid option')
        
        stream = f'http://{self.camera_ip_address[self.camera_name]}/mjpg/video.mjpg'
        
        ret, self.frame = cv2.VideoCapture(stream).read()
        
        if not ret:
            raise CameraException(f'Unable to connect to camera stream')
        
        # Variables
        self.rotation_angle = 0
        self.start_corner: Optional[tuple[int, int]] = None
        self.end_corner: Optional[tuple[int, int]] = None
        self.low_H = self.background_hsv_bounds[self.camera_name]['lower'][0]
        self.low_S = self.background_hsv_bounds[self.camera_name]['lower'][1]
        self.low_V = self.background_hsv_bounds[self.camera_name]['lower'][2]
        self.high_H = self.background_hsv_bounds[self.camera_name]['upper'][0]
        self.high_S = self.background_hsv_bounds[self.camera_name]['upper'][1]
        self.high_V = self.background_hsv_bounds[self.camera_name]['upper'][2]
        self.minimum_contour_area = 10
        
    def rotation_trackbar_cb_(self, val):
        self.rotation_angle = val/5
    
    def rectangle_corners_mouse_cb_(self, event,x,y,flags,param):
        self.rectangle_x = x
        self.rectangle_y = y
        if event == cv2.EVENT_MBUTTONDOWN:
            self.start_corner = (x,y)
        if event == cv2.EVENT_MBUTTONUP:
            self.end_corner = (x,y)

    def low_H_cb_(self,val):
        self.low_H = val
        self.low_H = min(self.high_H - 1, self.low_H)
        cv2.setTrackbarPos(self.low_H_name, self.hsv_window, self.low_H)

    def high_H_cb_(self,val):
        self.high_H = val
        self.high_H = max(self.high_H, self.low_H + 1)
        cv2.setTrackbarPos(self.high_H_name, self.hsv_window, self.high_H)

    def low_S_cb_(self,val):
        self.low_S = val
        self.low_S = min(self.high_S - 1, self.low_S)
        cv2.setTrackbarPos(self.low_S_name, self.hsv_window, self.low_S)

    def high_S_cb_(self,val):
        self.high_S = val
        self.high_S = max(self.high_S, self.low_S + 1)
        cv2.setTrackbarPos(self.high_S_name, self.hsv_window, self.high_S)

    def low_V_cb_(self,val):
        self.low_V = val
        self.low_V = min(self.high_V - 1, self.low_V)
        cv2.setTrackbarPos(self.low_V_name, self.hsv_window, self.low_V)

    def high_V_cb_(self,val):
        self.high_V = val
        self.high_V = max(self.high_V, self.low_V + 1)
        cv2.setTrackbarPos(self.high_V_name, self.hsv_window, self.high_V)

    def contour_area_slider_cb_(self, val):
        self.minimum_contour_area = val

    def set_rotation(self, frame: MatLike) -> MatLike:
        rotation_window = "Use slider to set rotation for the image. Press \'c\' to continue, Press \'q\' to quit"
                
        cv2.namedWindow(rotation_window, cv2.WINDOW_NORMAL)
        cv2.resizeWindow(rotation_window , 800, 800)
        
        angle_trackbar = 'Rotation angle'
        cv2.createTrackbar(angle_trackbar, rotation_window, 0, 50, self.rotation_trackbar_cb_)
        cv2.setTrackbarMin(angle_trackbar, rotation_window, -50)
        
        while True:
            rotation_matrix = cv2.getRotationMatrix2D((frame.shape[1]/2, frame.shape[0]/2), self.rotation_angle, 1)
            rotated_img = cv2.warpAffine(frame, rotation_matrix, (frame.shape[1], frame.shape[0]))

            # TODO: add grid to rotation image
            
            cv2.imshow(rotation_window, rotated_img)
            
            key = cv2.waitKey(1) & 0xFF
            if key == ord('c'):
                cv2.destroyWindow(rotation_window)
                return rotated_img
            if key == ord('q'):
                raise CalibrationException('User quit')
            
    def select_region(self, frame: MatLike, type: str)-> tuple[tuple[int, int], tuple[int, int]]:
        self.start_corner = None
        self.end_corner = None
        box_window = f'Use the middle mouse button to draw a rectangle over a single {type}'
        cv2.namedWindow(box_window, cv2.WINDOW_NORMAL)
        cv2.resizeWindow(box_window , 800, 800)
        
        cv2.setMouseCallback(box_window, self.rectangle_corners_mouse_cb_)
        
        while True:
            if self.start_corner is not None:
                rectangle_image = frame.copy()
                if self.end_corner is None:
                    rectangle_end_point = (self.rectangle_x, self.rectangle_y)
                else:
                    rectangle_end_point = self.end_corner
                
                cv2.rectangle(rectangle_image, self.start_corner, rectangle_end_point, 120, 3)
                cv2.imshow(box_window, rectangle_image) 
            else:
                cv2.imshow(box_window, frame)
            
            
            key = cv2.waitKey(1) & 0xFF
            if key == ord('c'):
                if self.end_corner is None or self.start_corner is None:
                    raise CalibrationException('Both corners were not selected for region!')
                cv2.destroyWindow(box_window)
                break
            if key == ord('r'):
                self.start_corner = None
                self.end_corner = None
            if key == ord('q'):
                raise CalibrationException('User quit')
        
        return (self.start_corner, self.end_corner)
    
    # def set_crop_region(self, frame: MatLike)-> MatLike:
    #     corner_window = 'Use the middle mouse button to draw a rectangle over the desired calibration region'
    #     cv2.namedWindow(corner_window, cv2.WINDOW_NORMAL)
    #     cv2.resizeWindow(corner_window , 800, 800)
        
    #     cv2.setMouseCallback(corner_window, self.rectangle_corners_mouse_cb_)
        
    #     while True:
    #         if self.start_corner is not None:
    #             rectangle_image = frame.copy()
    #             if self.end_corner is None:
    #                 rectangle_end_point = (self.rectangle_x, self.rectangle_y)
    #             else:
    #                 rectangle_end_point = self.end_corner
                
    #             cv2.rectangle(rectangle_image, self.start_corner, rectangle_end_point, (0, 0, 255), 3)
    #             cv2.imshow(corner_window, rectangle_image) 
    #         else:
    #             cv2.imshow(corner_window, frame)
            
            
    #         key = cv2.waitKey(1) & 0xFF
    #         if key == ord('c'):
    #             if self.end_corner is None or self.start_corner is None:
    #                 raise CalibrationException('No corners were selected for cropping region!')
    #             cv2.destroyWindow(corner_window)
    #             break
    #         if key == ord('r'):
    #             self.start_corner = None
    #             self.end_corner = None
    #         if key == ord('q'):
    #             raise CalibrationException('User quit')
        
    #     # Crop image        
    #     cropped_img = frame[
    #         self.start_corner[1] - self.crop_offset:self.end_corner[1] + self.crop_offset,
    #         self.start_corner[0] - self.crop_offset:self.end_corner[0] + self.crop_offset
    #     ]

    #     return cropped_img
    
    def set_hsv_threshold(self, frame) -> MatLike:
        frame = cv2.GaussianBlur(frame, (5,5), 0)
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        self.hsv_window = 'Use sliders to set HSV thresholding values to that only holes are visible in white'
        cv2.namedWindow(self.hsv_window, cv2.WINDOW_NORMAL)
        cv2.resizeWindow(self.hsv_window , 800, 800)
        
        cv2.createTrackbar(self.low_H_name, self.hsv_window, self.low_H, self.max_value_H, self.low_H_cb_)
        cv2.createTrackbar(self.high_H_name, self.hsv_window, self.high_H, self.max_value_H, self.high_H_cb_)
        cv2.createTrackbar(self.low_S_name, self.hsv_window, self.low_S, self.max_value, self.low_S_cb_)
        cv2.createTrackbar(self.high_S_name, self.hsv_window, self.high_S, self.max_value, self.high_S_cb_)
        cv2.createTrackbar(self.low_V_name, self.hsv_window, self.low_V, self.max_value, self.low_V_cb_)
        cv2.createTrackbar(self.high_V_name, self.hsv_window, self.high_V, self.max_value, self.high_V_cb_)

        while True:
    
            frame_threshold_image = cv2.inRange(frame, (self.low_H, self.low_S, self.low_V), (self.high_H, self.high_S, self.high_V)) #type: ignore
            cv2.imshow(self.hsv_window, frame_threshold_image)
            
            key = cv2.waitKey(30) & 0xFF
            if key == ord('c'):
                cv2.destroyWindow(self.hsv_window)
                return frame_threshold_image
            if key == ord('q'):
                raise CalibrationException('User quit')
    
    def remove_excess_detected_points(self, frame: MatLike)-> MatLike:
        self.start_corner = None
        self.end_corner = None
        remove_window = 'Use the middle mouse button to draw rectanlges over points that need to be removed'
        cv2.namedWindow(remove_window, cv2.WINDOW_NORMAL)
        cv2.resizeWindow(remove_window, 800, 800)

        cv2.createTrackbar('Minimum Contour Area', remove_window, self.minimum_contour_area, 30, self.contour_area_slider_cb_)
        cv2.setMouseCallback(remove_window, self.rectangle_corners_mouse_cb_)

        while True:
            remove_img = frame.copy()
            if self.start_corner is not None:
                if self.end_corner is None:
                    cv2.rectangle(remove_img, self.start_corner, (self.rectangle_x, self.rectangle_y), 120, 2)
                else:
                    cv2.rectangle(frame, self.start_corner, self.end_corner, 0, -1)
                    self.start_corner = None
                    self.end_corner = None

            contours, _ = cv2.findContours(frame, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
            filtered_contours = []
            for contour in contours:
                area = cv2.contourArea(contour)
                if area >= self.minimum_contour_area:
                    filtered_contours.append(contour)
            colored_image = cv2.cvtColor(remove_img,cv2.COLOR_GRAY2BGR)
            cv2.drawContours(colored_image,filtered_contours,-1,(0,255,0),-1) #type: ignore
            cv2.imshow(remove_window, colored_image)

            key = cv2.waitKey(30) & 0xFF
            if key == ord('c'):
                cv2.destroyWindow(remove_window)
                return frame
            if key == ord('q'):
                raise CalibrationException('User quit')

    def get_frame(self):
        return self.frame
        
        
        
        
        
        
        
        
    # def rotation_change(self, val):
    #     self.rotate_image(val/5)

    # def rotate_image(self, angle):
    #     self.angle = angle
    #     self.rotation_matrix = cv2.getRotationMatrix2D((self.current_frame.shape[1] / 2, self.current_frame.shape[0] / 2), angle, 1)
    #     self.rotated_image = cv2.warpAffine(self.current_frame, self.rotation_matrix, (self.current_frame.shape[1], self.current_frame.shape[0]))
    #     grid_image = self.rotated_image.copy()
    #     self.draw_grid(grid_image, 15, 20, (100, 100, 100), 1)
    #     cv2.imshow('Image Rotation', grid_image)


    # def draw_grid(self, img, rows, cols, color=(0, 255, 0), thickness=1):
    #     """Draws a grid over the image."""

    #     h, w, _ = img.shape

    #     for i in range(0, rows + 1):
    #         y = int(i * h / rows)
    #         cv2.line(img, (0, y), (w, y), color, thickness)

    #     for j in range(0, cols + 1):
    #         x = int(j * w / cols)
    #         cv2.line(img, (x, 0), (x, h), color, thickness)

    #     return img

    # def contour_area_slider(self, val):
    #     contour_image = self.just_holes_original.copy()
    #     contours, _ = cv2.findContours(contour_image, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
    #     filtered_contours = []
    #     for contour in contours:
    #         area = cv2.contourArea(contour)
    #         if area >= val:
    #             filtered_contours.append(contour)
    #     self.minimum_contour_area = val

    #     colored_image = cv2.cvtColor(contour_image,cv2.COLOR_GRAY2BGR)
    #     cv2.drawContours(colored_image,filtered_contours,-1,(0,255,0),-1) #type: ignore
    #     cv2.imshow('Remove Excess Points', colored_image)
    #     self.just_holes_calibration = colored_image.copy()

    # def draw_box_on_image(self,event,x,y,flags,param):
    #     rectangle_image = self.just_holes_calibration.copy()
    #     if event == cv2.EVENT_MBUTTONDOWN:
    #         self.start_point = (x,y)
    #         self.drawing_rectangle = True
    #     if event == cv2.EVENT_MBUTTONUP:
    #         cv2.rectangle(self.just_holes_calibration, self.start_point, (x,y), 0, -1) #type: ignore
    #         cv2.rectangle(self.just_holes_original, self.start_point, (x,y), 0, -1) #type: ignore
    #         cv2.imshow('Remove Excess Points', self.just_holes_calibration)
    #         self.drawing_rectangle = False
    #     if self.drawing_rectangle:
    #         rectangle_image = self.just_holes_calibration.copy()
    #         cv2.rectangle(rectangle_image, self.start_point, (x,y), (120,120,120), 1)
    #         cv2.imshow('Remove Excess Points', rectangle_image)
    
    # def select_column(self,event,x,y,flags,param):
    #     rectangle_image = self.current_frame.copy()
        
    #     if event == cv2.EVENT_MBUTTONDOWN:
    #         self.column_start = (x, y)

    #     if self.column_start is not None and self.column_end is None:
    #         rectangle_image = self.current_frame.copy()
    #         cv2.rectangle(rectangle_image, self.column_start, (x,y), 120, 2)
    #         cv2.imshow('Select Columns and Rows', rectangle_image)
            
    #     if event == cv2.EVENT_MBUTTONUP and self.column_start is not None:
    #         self.column_end = (x, y)
    #         cv2.rectangle(rectangle_image, self.column_start, self.column_end, 120, 2)
    #         cv2.imshow('Select Columns and Rows', rectangle_image)

    # def select_row(self,event,x,y,flags,param):
    #     rectangle_image = self.current_frame.copy()
        
    #     if event == cv2.EVENT_MBUTTONDOWN:
    #         self.row_start = (x, y)

    #     if self.row_start is not None and self.row_end is None:
    #         rectangle_image = self.current_frame.copy()
    #         cv2.rectangle(rectangle_image, self.row_start, (x,y), 120, 2)
    #         cv2.imshow('Select Columns and Rows', rectangle_image)
            
    #     if event == cv2.EVENT_MBUTTONUP and self.row_start is not None:
    #         self.row_end = (x, y)
    #         cv2.rectangle(rectangle_image, self.row_start, self.row_end, 120, 2)
    #         cv2.imshow('Select Columns and Rows', rectangle_image)
            
    

    # def run_calibration(self)-> Optional[tuple[float, tuple[int, int], tuple[int, int], MatLike, MatLike]]:
    #     pass
        # # Rotate image so grid of holes is perpendicular
        # self.rotated_image = self.current_frame
        # cv2.namedWindow('Image Rotation', cv2.WINDOW_NORMAL)
        # cv2.resizeWindow('Image Rotation' , 800, 800)
        # cv2.imshow('Image Rotation', self.current_frame)
        # cv2.createTrackbar('Angle','Image Rotation',0,50,self.rotation_change)
        # cv2.setTrackbarMin('Angle', 'Image Rotation', -50)
        # rotation_set = False
        # while not rotation_set:
        #     key = cv2.waitKey(0) & 0xFF
        #     if key == ord('c'):
        #         rotation_set = True
        
        # self.current_frame = self.rotated_image
        # rectangle_set = False
        # cv2.destroyAllWindows()

        # # Determine cropping region
        # cv2.namedWindow('Corner Selection', cv2.WINDOW_NORMAL)
        # cv2.resizeWindow('Corner Selection' , 800, 800)
        # cv2.imshow('Corner Selection', self.current_frame)
        # cv2.setMouseCallback('Corner Selection',self.mouse_callback)
        # while not rectangle_set:
        #     key = cv2.waitKey(0) & 0xFF
        #     if key == ord('c'):
        #         rectangle_set = True
        
        # # Crop image 
        # offset = -10
        
        # self.current_frame = self.current_frame[
        #     self.start_corner[1] - offset:self.end_corner[1] + offset,
        #     self.start_corner[0] - offset:self.end_corner[0] + offset
        # ]

        # cropped_image = self.current_frame.copy()
        
        # cv2.destroyAllWindows()

        # # Threshold image to remove everything but holes 
        # blur = cv2.GaussianBlur(self.current_frame,(5,5),0)
        # hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)
        
        # cv2.imwrite('hsv.jpg', hsv)
        
        # return None
        
        # threshold = cv2.inRange(hsv, self.background_hsv_bounds[self.camera_name]['lower'], self.background_hsv_bounds[self.camera_name]['upper']) #type: ignore

        # self.just_holes_calibration = threshold.copy()
        # self.just_holes_original = self.just_holes_calibration

        # # Mouse callback on window to allow for a middle-click drag to remove any excess points
        # cv2.namedWindow('Remove Excess Points', cv2.WINDOW_NORMAL)
        # cv2.resizeWindow('Remove Excess Points' , 800, 800)
        # cv2.setMouseCallback('Remove Excess Points',self.draw_box_on_image)
        # cv2.createTrackbar("Minimum contour area", 'Remove Excess Points', self.minimum_contour_area, 30, self.contour_area_slider)
        # cv2.imshow('Remove Excess Points', self.just_holes_calibration)
        
        # excess_points_removed = False
        # while not excess_points_removed:
        #     key = cv2.waitKey(0) & 0xFF
        #     if key == ord('c'):
        #         excess_points_removed = True

        # cv2.destroyAllWindows()

        # self.current_frame = self.just_holes_original

        # # Select region for columns and rows
        # cv2.namedWindow('Select Columns and Rows', cv2.WINDOW_NORMAL)
        # cv2.resizeWindow('Select Columns and Rows' , 800, 800)
        # while True:
        #     cv2.setMouseCallback('Select Columns and Rows', self.select_column)
        #     cv2.imshow('Select Columns and Rows', self.current_frame)
        #     key = cv2.waitKey(0) & 0xFF
        #     if key == ord('r'):
        #         self.column_start = None
        #         self.column_end = None
                
        #     elif key == ord('c'):
        #         break

        # # Mouse callback for selecting a single row
        # while True:
        #     cv2.imshow('Select Columns and Rows', self.current_frame)
        #     cv2.setMouseCallback('Select Columns and Rows', self.select_row)
        #     key = cv2.waitKey(0) & 0xFF
        #     if key == ord('r'):
        #         self.row_start = None
        #         self.row_end = None

        #     elif key == ord('c'):
        #         break
            
        # cv2.destroyAllWindows()

        # # Detect contours 
        # all_contours, _ = cv2.findContours(self.current_frame, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        # if self.column_start is not None and self.column_end is not None:
        #     selected_column_region = self.current_frame[self.column_start[1]:self.column_end[1], self.column_start[0]:self.column_end[0]]
        # else:
        #     print("something bad happened")
        #     return None
        
        # contours_in_selected_column, _ = cv2.findContours(selected_column_region, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        
        # if self.row_start is not None and self.row_end is not None:
        #     selected_row_region = self.current_frame[self.row_start[1]:self.row_end[1], self.row_start[0]:self.row_end[0]]
        # else:
        #     print("something bad happened")
        #     return None
        
        # contours_in_selected_row, _ = cv2.findContours(selected_row_region, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        # # Count num contours
        # all_contours = [c for c in all_contours if cv2.contourArea(c) > self.minimum_contour_area]
        # contours_in_selected_column = [c for c in contours_in_selected_column if cv2.contourArea(c) > self.minimum_contour_area]

        # num_holes = len(all_contours)
        # num_rows = len(contours_in_selected_column)
        # num_columns = len([c for c in contours_in_selected_row if cv2.contourArea(c) > self.minimum_contour_area])

        # print(f'rows: {num_rows}, columns: {num_columns}, num_holes: {num_holes}"')

        # if not num_rows * num_columns == num_holes:
        #     print(f"I fucked up :(")
        #     return None

        # # Find center of contours and sort
        # center_points = []
        # for contour in all_contours:
        #     # Calculate moments for each contour
        #     M = cv2.moments(contour)

        #     # Calculate center of contour
        #     if M["m00"] != 0:
        #         cX = int(M["m10"] / M["m00"])
        #         cY = int(M["m01"] / M["m00"])

        #         center_points.append((cX, cY))
        
        # row_y = []
        # for contour in contours_in_selected_column:
        #     # Calculate moments for each contour
        #     M = cv2.moments(contour)

        #     # Calculate center of contour
        #     if M["m00"] != 0:
        #         row_y.append( int(M["m01"] / M["m00"]))

        # row_y.sort()

        # # Loop through each detected row and sort points in each row from left to right
        # sorted_points = []

        # for i in range(num_rows):
        #     working_points = []

        #     if i == 0:
        #         lower_bound = 0
        #         upper_bound = self.column_start[1] + row_y[i] + (row_y[i+1] - row_y[i])/2 
        #     elif i == num_rows - 1:
        #         lower_bound = self.column_start[1] + row_y[i] - (row_y[i] - row_y[i-1])/2 
        #         upper_bound = self.current_frame.shape[1]
        #     else:
        #         lower_bound = self.column_start[1] + row_y[i] - (row_y[i] - row_y[i-1])/2 
        #         upper_bound = self.column_start[1] + row_y[i] + (row_y[i+1] - row_y[i])/2 

        #     for x, y in center_points:
        #         if lower_bound <= y <= upper_bound:
        #             working_points.append((x, y))
                     
        #     sorted_points += sorted(working_points, key=lambda k: [k[0]])

        # # Check to see if all points were sorted
        # if not len(sorted_points) == len(center_points):
        #     # self.get_logger().error("Not able to properly sort holes")
        #     print("Not able to properly sort holes")
        #     return None

        # actual_points = []
        # # Create grid, assigning each hole distance a certain number of pixels
        # for i in range(num_rows):
        #     x = (i * self.pixels_per_inch)
        #     for j in range(num_columns):
        #         y = (j * self.pixels_per_inch)
        #         actual_points.append([x, y])

        # grid_x, grid_y = np.mgrid[0:(num_rows-1)*self.pixels_per_inch, 0:(num_columns-1)*self.pixels_per_inch]

        # destination = np.array(actual_points)
        # source = np.array(sorted_points)

        # grid_z = griddata(destination, source, (grid_x, grid_y), method='cubic')
        # map_x = np.append([], [ar[:,0] for ar in grid_z]).reshape((num_rows-1)*self.pixels_per_inch,(num_columns-1)*self.pixels_per_inch).astype('float32')
        # map_y = np.append([], [ar[:,1] for ar in grid_z]).reshape((num_rows-1)*self.pixels_per_inch,(num_columns-1)*self.pixels_per_inch).astype('float32')

        # cv2.imshow("Remapped", cv2.remap(cropped_image, map_x, map_y, cv2.INTER_CUBIC))
        # cv2.waitKey(0)

        # return (self.angle, self.start_corner, self.end_corner, map_x, map_y)
