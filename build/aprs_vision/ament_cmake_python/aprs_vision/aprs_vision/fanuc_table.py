#!/usr/bin/env python3

import cv2
import math
import numpy as np
from scipy.interpolate import griddata

from cv2.typing import MatLike
from typing import Optional

from rclpy.node import Node
from rclpy.qos import qos_profile_default

from example_interfaces.srv import Trigger
from sensor_msgs.msg import Image
from aprs_interfaces.msg import Trays


class FanucTable(Node):    
    def __init__(self):
        super().__init__(node_name='fanuc_vision_node')

        self.capture = cv2.VideoCapture("http://192.168.1.104/mjpg/video.mjpg")

        # ROS Services
        self.update_trays_info_srv = self.create_service(Trigger, 'update_vision_data', self.update_trays)
        self.calibrate_maps_srv = self.create_service(Trigger, 'calibrate_maps', self.calibrate_maps)

        # ROS Topics
        self.trays_info_pub = self.create_publisher(Trays, 'trays_info', qos_profile_default)
        self.raw_image_pub = self.create_publisher(Image, 'raw_image', qos_profile_default)
        self.detected_trays_image_pub = self.create_publisher(Image, 'detected_trays_image', qos_profile_default)

        # ROS Timers
        self.publish_timer = self.create_timer(timer_period_sec=0.1, callback=self.publish)

    def update_trays(self, req: Trigger.Request):
        # Update tray info msg
        pass

    def calibrate_maps(self, req: Trigger.Request):
        # Generate and save new maps for calibration
        pass

    def publish(self):
        # Publish to all topics

        # Get most recent frame for raw image 
        raw_image = self.get_most_recent_frame()

        if raw_image is not None:
            raw_image_msg = self.build_img_msg_from_mat(raw_image)
            self.raw_image_pub.publish(raw_image_msg)
            self.get_logger().info("Publishing image")

    def get_most_recent_frame(self) -> Optional[MatLike]:
        ret, frame = self.capture.read()

        if not ret:
            self.get_logger().error("Unable to connect to stream")
            return None
        
        return frame

    def generate_grid_maps(self, frame: MatLike) -> MatLike:
        offset = 15

        # Corners are manually deduced from location of screw heads in table
        top_left = (417 + offset, 180 + offset)
        top_right = (1043 - offset, 179 + offset)
        bottom_right = (1035 - offset, 955 - offset)
        bottom_left = (428 + offset, 944 - offset)

        # Black out everything from image that is not the active region
        fanuc_table_corners = np.array([top_right, bottom_right, bottom_left, top_left])

        maskImage = np.zeros(frame.shape, dtype=np.uint8)
        cv2.drawContours(maskImage, [fanuc_table_corners], 0, (255, 255, 255), -1)

        active_region = cv2.bitwise_and(frame, maskImage)

        # Detect optical table holes 
        blur = cv2.GaussianBlur(active_region,(5,5),0)

        hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)

        threshold = cv2.inRange(hsv, (100, 0, 0), (255, 255, 120))

        offset = 25

        top_left = (417 + offset, 180 + offset)
        top_right = (1043 - offset, 179 + offset)
        bottom_right = (1035 - offset, 955 - offset)
        bottom_left = (428 + offset, 944 - offset)

        corners = np.array([top_right, bottom_right, bottom_left, top_left])

        mask2 = np.zeros(threshold.shape, dtype=np.uint8)
        cv2.drawContours(mask2, [corners], 0, 255, -1)

        just_holes = cv2.bitwise_and(threshold, mask2)

        contours, _ = cv2.findContours(just_holes, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        filtered_contours = []
        for contour in contours:
            area = cv2.contourArea(contour)
            if area >= 10:
                filtered_contours.append(contour)

        rows = 24
        columns = 19

        if not len(filtered_contours) == rows * columns:
            self.get_logger().error("Not able to detect all holes")
            return

        center_points = []

        for contour in filtered_contours:
            # Calculate moments for each contour
            M = cv2.moments(contour)

            # Calculate center of contour
            if M["m00"] != 0:
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])

                center_points.append((cX, cY))

        center_y = 210
        sorted_points = []
        working_points = []

        for i in range(rows):
            for point in center_points:
                if center_y - 18 <= point[1] <= center_y + 18:
                    working_points.append(point)
                
            sorted_points += sorted(working_points, key=lambda k: [k[0]])

            working_points.clear()

            center_y += 31

        if not len(sorted_points) == len(center_points):
            self.get_logger().error("Not able to properly sort holes")
            return
        
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

        np.save("src/vision_testing/map_x.npy", map_x_32)
        np.save("src/vision_testing/map_y.npy", map_y_32)

    def rectify_frame(self, frame: MatLike) -> MatLike:

        map_x = np.load("src/vision_testing/map_x.npy")
        map_y = np.load("src/vision_testing/map_y.npy")

        return cv2.remap(frame, map_x, map_y, cv2.INTER_CUBIC)[:-30,:-30]
    
    def remove_background(self, frame: MatLike, background: MatLike) -> MatLike:
        delta = cv2.subtract(background, frame)

        gray = cv2.cvtColor(delta, cv2.COLOR_BGR2GRAY)

        _, mask = cv2.threshold(gray, 12, 255, cv2.THRESH_BINARY)

        canvas = np.zeros(gray.shape, dtype=np.uint8)

        contours,_ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        filtered_contours = []
        for c in contours:
            if cv2.contourArea(c) > 40:
                filtered_contours.append(c)

        cv2.drawContours(canvas, filtered_contours, -1, color=255, thickness=cv2.FILLED)

        return cv2.bitwise_and(frame, frame, mask=canvas)
    
    def remove_and_replace_gears(self, frame: MatLike, gear_size="large") -> MatLike:
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # Remove and replace large green gears
        if gear_size == "large":
            hsv_lower = (40, 30, 123)
            hsv_upper = (85, 165, 215)
            radius = 60
        elif gear_size == "medium":
            hsv_lower = (6, 76, 140)
            hsv_upper = (24, 162, 255)
            radius = 44
        else:
            return frame

        gears = cv2.inRange(hsv, hsv_lower, hsv_upper)
    
        contours, _ = cv2.findContours(gears, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        for c in contours:
            if cv2.contourArea(c) < 200:
                continue

            M = cv2.moments(c)
        
            if M["m00"] != 0:
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])

            start_x = int(cX - radius)
            start_y = int(cY - radius)

            cv2.circle(frame, (cX, cY), radius, (0, 0, 0), -1)

            square = frame[start_y:start_y + 2*radius, start_x:start_x + 2*radius]

            _, mask = cv2.threshold(cv2.cvtColor(square, cv2.COLOR_BGR2GRAY), 0, 255, cv2.THRESH_BINARY)

            avg_color = cv2.mean(square, mask)

            cv2.circle(frame, (cX, cY), radius + 3, avg_color, -1)
        
        return frame
            
    def detect_trays(self, table_image: MatLike, original: MatLike) -> MatLike:
        gray = cv2.cvtColor(table_image, cv2.COLOR_BGR2GRAY)

        contours, _ = cv2.findContours(gray, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        canvas = np.zeros(table_image.shape, dtype=np.uint8)
        canvas.fill(255)

        for contour in contours:
            if cv2.contourArea(contour) < 200:
                continue

            peri = cv2.arcLength(contour, True)
            poly = cv2.approxPolyDP(contour, 0.01 * peri, True)

            rect = cv2.minAreaRect(poly)

            (_, _), (width, height), angle = rect

            if width < height:
                angle += 90

            angle = math.radians(angle - 90)

            aspect_ratio = min(width, height) / max(width, height)

            if aspect_ratio < 0.6:
                tray_type = "large_part_tray"
            elif aspect_ratio < 0.9:
                tray_type = "m2l1_kit_tray"
            else:
                tray_type = "medium_part_tray"

            tray_color = {
                "medium_part_tray": (255, 50, 0),
                "large_part_tray": (180, 0, 100),
                 "m2l1_kit_tray": (19, 69, 139)
            }

            box = cv2.boxPoints(rect)
            box = box.astype(np.int32)

            cv2.drawContours(canvas, [poly], -1, tray_color[tray_type], -1) 
            # cv2.drawContours(table_image, [box], -1, (0, 0, 255), 1) 

            M = cv2.moments(box)

            # Calculate center of contour
            if M["m00"] != 0:
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])

            # Calculate slot locations
            slot_offsets = []

            if tray_type == "medium_part_tray":
                # Draw circles at each of the slot locations based on roation
                slot_offsets = (
                    [(40, -40), "medium"],
                    [(40, 40), "medium"],
                    [(-40, -40), "medium"],
                    [(-40, 40), "medium"]
                )
            elif tray_type == "large_part_tray":
                # Draw circles at each of the slot locations based on roation
                slot_offsets = (
                    [(0, 55), "large"],
                    [(0, -55), "large"]
                )
            elif tray_type == "m2l1_kit_tray":
                # Draw circles at each of the slot locations based on roation
                slot_offsets = (
                    [(0, -40), "large"],
                    [(-40, 53), "medium"],
                    [(40, 53), "medium"]
                )
                
            for (x_off, y_off), size in slot_offsets:
                # 30 pixels is 25.4 mm so 1 pixel is .8466 mm
                conv_factor = 0.8466

                slot = (
                    int(x_off/conv_factor * math.cos(angle) + y_off/conv_factor * math.cos(angle - math.pi/2) + cX),
                    int(x_off/conv_factor * math.sin(angle) + y_off/conv_factor * math.sin(angle - math.pi/2) + cY)
                )

                if size == "medium":
                    radius = 42
                    color = (28, 172, 255)
                    hsv_lower = (6, 76, 140)
                    hsv_upper = (24, 162, 255)
                elif size == "large":
                    radius = 55
                    color = (0, 255, 0)
                    hsv_lower = (40, 30, 123)
                    hsv_upper = (85, 165, 215)

                # return original

                start_x = int(slot[0] - radius)
                start_y = int(slot[1] - radius)

                square = original[start_y:start_y + 2*radius, start_x:start_x + 2*radius]

                gear = cv2.inRange(cv2.cvtColor(square, cv2.COLOR_BGR2HSV), hsv_lower, hsv_upper)

                if cv2.countNonZero(gear) > 100:
                    cv2.circle(canvas, slot, radius, color, -1)
                    cv2.circle(canvas, slot, 5, (0, 0, 0), -1)
                # else:
                #     cv2.circle(table_image, slot, radius, (150, 150, 150), -1)

            length = 30

            x_end = (int(length * math.cos(angle) + cX), int(length * math.sin(angle) + cY))
            y_end = (int(length * math.cos(angle - math.pi/2) + cX), int(length * math.sin(angle - math.pi/2) + cY))

            cv2.arrowedLine(canvas, (cX, cY), x_end, (0, 0, 255), 2)
            cv2.arrowedLine(canvas, (cX, cY), y_end, (0, 255, 0), 2)
            cv2.circle(canvas, (cX, cY), 3, (0, 0, 0), -1)

        return cv2.addWeighted(original, 0.6, canvas, 0.4, 0)
    
    def build_img_msg_from_mat(self, mat: MatLike) -> Image:
        img_msg = Image()
        
        img_msg.height = mat.shape[0]
        img_msg.width = mat.shape[1]

        numpy_type_to_cvtype = {
            'uint8': '8U',
            'int8': '8S',
            'uint16': '16U',
            'int16': '16S',
            'int32': '32S',
            'float32': '32F',
            'float64': '64F'
        }

        if len(mat.shape) < 3:
            n_channels = 1
        else:
            n_channels = mat.shape[2]

        # img_msg.encoding = f'{numpy_type_to_cvtype[mat.dtype.name]}C{n_channels}'
        img_msg.encoding = 'bgr8'

        if mat.dtype.byteorder == '>':
            img_msg.is_bigendian = True
        
        img_msg.data.frombytes(mat.tobytes())
        
        img_msg.step = len(img_msg.data) // img_msg.height

        return img_msg




