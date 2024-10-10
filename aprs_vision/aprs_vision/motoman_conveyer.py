import os
import cv2
import math
import numpy as np


from aprs_vision.vision_table import VisionTable
from aprs_vision.gear_detection import GearDetection
from aprs_interfaces.msg import SlotInfo
from geometry_msgs.msg import Point
from scipy.interpolate import griddata
from rclpy.qos import qos_profile_default

from cv2.typing import MatLike
from typing import Optional

class MotomanConveyer(VisionTable):
    #TODO Fill in real values
    table_origin = Point(x=407.026, y=400.365, z=-0.01)
    tray_height = 0.017
    gear_height = 0.02

    video_stream = "http://192.168.1.107/mjpg/video.mjpg"
    map_x_image = 'motoman_conveyer_map_x.npy'
    map_y_image = 'motoman_conveyer_map_y.npy'
    background_image = 'motoman_conveyer_background.jpg'
    publish_frames = True

    top_left_x = 389
    top_left_y = 10
    bottom_left_x = 386
    bottom_left_y = 308
    top_right_x = 849
    top_right_y = 19
    bottom_right_x = 858
    bottom_right_y = 305
    grid_hsv_lower = (0, 0, 10)
    grid_hsv_upper = (255, 255, 146)
    calibrate_rows = 18
    calibrate_columns = 32
    generate_map_area = 0.1

    angle_offset = math.pi
    suffix = 'motoman_conveyer'

    conversion_factor = 0.8466 # 30 pixels is 25.4 mm so 1 pixel is .8466 mm

    background_threshold = 12
    gear_detection_values = {
        SlotInfo.LARGE: GearDetection((40, 30, 123), (85, 165, 215), 60),
        SlotInfo.MEDIUM: GearDetection((6, 100, 140), (24, 162, 255), 44),
        SlotInfo.SMALL: GearDetection((18, 80, 170), (45, 135, 255), 30)
    }

    base_frame = 'motoman_base_link'

    def __init__(self):
        super().__init__("motoman_conveyer", "motoman_conveyer_vision")
    
    def generate_grid_maps(self, frame: MatLike, filepath: str) -> Optional[bool]:
        vertical_offset = 28

        cv2.imshow('window', frame)
        cv2.waitKey(0)

        # Corners are manually deduced from location of screw heads in table
        top_left = (self.top_left_x, self.top_left_y + vertical_offset)
        top_right = (self.top_right_x, self.top_right_y + vertical_offset)
        bottom_right = (self.bottom_right_x, self.bottom_right_y)
        bottom_left = (self.bottom_left_x, self.bottom_left_y)

        # Black out everything from image that is not the active region
        fanuc_table_corners = np.array([top_right, bottom_right, bottom_left, top_left])

        maskImage = np.zeros(frame.shape, dtype=np.uint8)
        cv2.drawContours(maskImage, [fanuc_table_corners], 0, (255, 255, 255), -1)

        active_region = cv2.bitwise_and(frame, maskImage)

        cv2.imshow('window', active_region)
        cv2.waitKey(0)

        # Detect optical table holes 
        blur = cv2.GaussianBlur(active_region,(5,5),0)

        hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)

        cv2.imwrite('hsv.jpg', hsv)

        threshold = cv2.inRange(hsv, self.grid_hsv_lower, self.grid_hsv_upper) # type: ignore

        cv2.imshow('window', threshold)
        cv2.waitKey(0)

        top_offset = 3
        offset = 10

        top_left = (self.top_left_x + offset, self.top_left_y + top_offset + vertical_offset)
        top_right = (self.top_right_x - offset, self.top_right_y + top_offset + vertical_offset)
        bottom_right = (self.bottom_right_x - offset, self.bottom_right_y - offset)
        bottom_left = (self.bottom_left_x + offset, self.bottom_left_y - offset)

        corners = np.array([top_right, bottom_right, bottom_left, top_left])

        mask2 = np.zeros(threshold.shape, dtype=np.uint8)
        
        cv2.drawContours(mask2, [corners], 0, 255, -1) # type: ignore

    
        just_holes = cv2.bitwise_and(threshold, mask2)
        
        cv2.rectangle(just_holes,(519,168),(525,175),(0,0,0),-1)
        cv2.imshow('window', just_holes)
        cv2.waitKey(0)
        cv2.rectangle(just_holes,(484,239),(488,242),(0,0,0),-1)
        cv2.imshow('window', just_holes)
        cv2.waitKey(0)
        cv2.rectangle(just_holes,(428,256),(436,260),(0,0,0),-1)
        cv2.imshow('window', just_holes)
        cv2.waitKey(0)

        contours, _ = cv2.findContours(just_holes, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        filtered_contours = []
        for contour in contours:
            area = cv2.contourArea(contour)
            if area >= self.generate_map_area:
                filtered_contours.append(contour)

        rows = self.calibrate_rows
        columns = self.calibrate_columns

        cv2.drawContours(just_holes,filtered_contours,-1,120,2) # type: ignore
        cv2.imshow('window', just_holes)
        cv2.waitKey(0)

        if not len(filtered_contours) == rows * columns:
            self.get_logger().error("Not able to detect all holes")
            return False

        center_points = []

        for contour in filtered_contours:
            # Calculate moments for each contour
            M = cv2.moments(contour)

            # Calculate center of contour
            if M["m00"] != 0:
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])

                center_points.append((cX, cY))

        center_y = 53
        sorted_points = []
        working_points = []

        for i in range(rows):
            for point in center_points:
                if center_y - 7 <= point[1] <= center_y + 7:
                    working_points.append(point)
                
            sorted_points += sorted(working_points, key=lambda k: [k[0]])

            start = (389,center_y - 7)
            end = (849,center_y - 7)
            start_1 = (389,center_y + 7)
            end_1 = (849,center_y + 7)

            cv2.line(just_holes,start,end,255,2)
            cv2.line(just_holes,start_1,end_1,255,2)

            working_points.clear()

            center_y += 14

        cv2.imshow('window', just_holes)
        cv2.waitKey(0)

        if not len(sorted_points) == len(center_points):
            self.get_logger().error("Not able to properly sort holes")
            return False
        
        actual_points = []

        for i in range(rows):
            x = (i * 30)
            for j in range(columns):
                y = (j * 30)
                actual_points.append([x, y])

        grid_x, grid_y = np.mgrid[0:rows*30, 0:columns*30]

        destination = np.array(actual_points)
        source = np.array(sorted_points)

        grid_z = griddata(destination, source, (grid_x, grid_y), method='cubic')
        map_x = np.append([], [ar[:,0] for ar in grid_z]).reshape(rows*30,columns*30)
        map_y = np.append([], [ar[:,1] for ar in grid_z]).reshape(rows*30,columns*30)
        map_x_32 = map_x.astype('float32')
        map_y_32 = map_y.astype('float32')

        np.save(f"{filepath}{self.map_x_image}", map_x_32)
        np.save(f"{filepath}{self.map_y_image}", map_y_32)

        return True
    