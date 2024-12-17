#!/usr/bin/env python3

from typing import Optional

import cv2
import numpy as np
from scipy.interpolate import griddata
from rclpy.node import Node
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
            raise CameraException('Unable to connect to camera stream')
        
        if self.camera_name == "fanuc_conveyor" or self.camera_name =="motoman_conveyor":
            self.frame = cv2.rotate(self.frame,cv2.ROTATE_180)

        # Variables
        self.rotation_angle = 0
        self.start_corner: Optional[tuple[int, int]] = None
        self.end_corner: Optional[tuple[int, int]] = None
        self.low_h = self.background_hsv_bounds[self.camera_name]['lower'][0]
        self.low_s = self.background_hsv_bounds[self.camera_name]['lower'][1]
        self.low_v = self.background_hsv_bounds[self.camera_name]['lower'][2]
        self.high_h = self.background_hsv_bounds[self.camera_name]['upper'][0]
        self.high_s = self.background_hsv_bounds[self.camera_name]['upper'][1]
        self.high_v = self.background_hsv_bounds[self.camera_name]['upper'][2]
        self.minimum_contour_area = 10
        self.rectangle_x: Optional[int] = None
        self.rectangle_y: Optional[int] = None

        self.hsv_window = ""

    def rotation_trackbar_cb_(self, val: int):
        """ Callback for rotation trackbar

        Args:
            val (int): Trackbar value
        """
        self.rotation_angle = val/5

    def draw_grid(self, img: MatLike, spacing: int, color: tuple[int, int, int]=(0, 255, 0), thickness: int=2) -> MatLike:
        """ Draws grid onto an image

        Args:
            img (MatLike): Image to draw grid on.
            spacing (int): Distance between gridlines in px.
            color (tuple[int, int, int], optional): BGR color for gridlines.
            thickness (int, optional): Gridline thickness. Defaults to 2.

        Returns:
            MatLike: Image with gridlines
        """

        h, w, _ = img.shape

        rows = int(h/spacing)
        cols = int(w/spacing)

        for i in range(0, rows + 1):
            y = int(i * h / rows)
            cv2.line(img, (0, y), (w, y), color, thickness)

        for j in range(0, cols + 1):
            x = int(j * w / cols)
            cv2.line(img, (x, 0), (x, h), color, thickness)

        return img

    def rectangle_corners_mouse_cb_(self, event: int, x: int, y: int, _, __):
        """ Callback for mouse events and location when drawing rectangles 

        Args:
            event (int): cv2 mouse event.
            x (int): x position of cursor
            y (int): y position of cursor
        """
        self.rectangle_x = x
        self.rectangle_y = y

        if event == cv2.EVENT_MBUTTONDOWN:
            self.start_corner = (x, y)
        if event == cv2.EVENT_MBUTTONUP:
            self.end_corner = (x, y)

    def low_h_cb_(self, val: int):
        """ Callback for low hue slider

        Args:
            val (int): Value of the trackbar
        """
        self.low_h = val
        self.low_h = min(self.high_h - 1, self.low_h)
        cv2.setTrackbarPos(self.low_H_name, self.hsv_window, self.low_h)

    def high_h_cb_(self, val: int):
        """ Callback for high hue slider

        Args:
            val (int): Value of the trackbar
        """
        self.high_h = val
        self.high_h = max(self.high_h, self.low_h + 1)
        cv2.setTrackbarPos(self.high_H_name, self.hsv_window, self.high_h)

    def low_s_cb_(self, val: int):
        """ Callback for low saturation slider

        Args:
            val (int): Value of the trackbar
        """
        self.low_s = val
        self.low_s = min(self.high_s - 1, self.low_s)
        cv2.setTrackbarPos(self.low_S_name, self.hsv_window, self.low_s)

    def high_s_cb_(self, val: int):
        """ Callback for high saturation slider

        Args:
            val (int): Value of the trackbar
        """
        self.high_s = val
        self.high_s = max(self.high_s, self.low_s + 1)
        cv2.setTrackbarPos(self.high_S_name, self.hsv_window, self.high_s)

    def low_v_cb_(self, val: int):
        """ Callback for low value slider

        Args:
            val (int): Value of the trackbar
        """
        self.low_v = val
        self.low_v = min(self.high_v - 1, self.low_v)
        cv2.setTrackbarPos(self.low_V_name, self.hsv_window, self.low_v)

    def high_v_cb_(self, val: int):
        """ Callback for high value slider

        Args:
            val (int): Value of the trackbar
        """
        self.high_v = val
        self.high_v = max(self.high_v, self.low_v + 1)
        cv2.setTrackbarPos(self.high_V_name, self.hsv_window, self.high_v)

    def contour_area_slider_cb_(self, val: int):
        """ Callback for the minimum contour area slider

        Args:
            val (int): Value of the slider
        """
        self.minimum_contour_area = val

    def set_rotation(self, frame: MatLike) -> tuple[MatLike, float]:
        """ Allow user to rotate image to align holes on table with perpendicular grid
        
        Args:
            frame (MatLike): Input image
        
        Raises:
            CalibrationException: Upon user quit
        
        Returns:
            tuple[MatLike, float]: Rotated image, angle
        """
        rotation_window = "Use slider to set rotation for the image. Press \'c\' to continue, Press \'q\' to quit"

        cv2.namedWindow(rotation_window, cv2.WINDOW_NORMAL)
        cv2.resizeWindow(rotation_window , 800, 800)

        angle_trackbar = 'Rotation angle'
        cv2.createTrackbar(angle_trackbar, rotation_window, 0, 50, self.rotation_trackbar_cb_)
        cv2.setTrackbarMin(angle_trackbar, rotation_window, -50)

        while True:
            rotation_matrix = cv2.getRotationMatrix2D((frame.shape[1]/2, frame.shape[0]/2), self.rotation_angle, 1)
            rotated_img = cv2.warpAffine(frame, rotation_matrix, (frame.shape[1], frame.shape[0]))
            grid_img = rotated_img.copy()
            self.draw_grid(grid_img, 30, (0, 153, 254), 2)

            cv2.imshow(rotation_window, grid_img)

            key = cv2.waitKey(1) & 0xFF
            if key == ord('c'):
                cv2.destroyWindow(rotation_window)
                return (rotated_img, self.rotation_angle)
            if key == ord('q'):
                raise CalibrationException('User quit')

    def select_region(self, frame: MatLike, selection_type: str, color:tuple[int, int, int]=(0,255,0))-> tuple[tuple[int, int], tuple[int, int]]:
        """ Function to select rectangular region on image.
        
        Args:
            frame (MatLike): Input Image
            type (str): descriptor only truly used for naming window. Expected to be "row", or "column"
            color (tuple[int, int, int], optional): Color of rectangle drawn on image. Defaults to (0,255,0) [Green].
        
        Raises:
            CalibrationException: Attempting to continue without both corners of rectangle selected
            CalibrationException: User quit
        
        Returns:
            tuple[tuple[int, int], tuple[int, int]]: Start Corner of Rectangle, End Corner of Rectangle
        """
        
        self.start_corner = None
        self.end_corner = None
        box_window = f'Use the middle mouse button to draw a rectangle over a single {selection_type}'
        cv2.namedWindow(box_window, cv2.WINDOW_NORMAL)
        cv2.resizeWindow(box_window , 800, 800)

        cv2.setMouseCallback(box_window, self.rectangle_corners_mouse_cb_)

        while True:
            if self.start_corner is not None:
                rectangle_image = frame.copy()

                if len(rectangle_image.shape) < 3:
                    rectangle_image = cv2.cvtColor(rectangle_image, cv2.COLOR_GRAY2BGR)

                if self.end_corner is None:
                    if self.rectangle_x is not None and self.rectangle_y is not None:
                        rectangle_end_point = (self.rectangle_x, self.rectangle_y)
                    else:
                        rectangle_end_point = self.start_corner
                else:
                    rectangle_end_point = self.end_corner

                cv2.rectangle(rectangle_image, self.start_corner, rectangle_end_point, color, 3)
                cv2.imshow(box_window, rectangle_image)
            else:
                cv2.imshow(box_window, frame)


            key = cv2.waitKey(30) & 0xFF
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

    def set_hsv_threshold(self, frame: MatLike) -> MatLike:
        """ Allow user to adjust hsv thresholding so that all holes properly appear in the threshold.
        
        Args:
            frame (Matlike): Input cropped image
        
        Raises:
            CalibrationException: User quit
        
        Returns:
            MatLike: thresholded image
        """
        frame = cv2.GaussianBlur(frame, (5,5), 0)
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        self.hsv_window = 'Use sliders to set HSV thresholding values to that only holes are visible in white'
        cv2.namedWindow(self.hsv_window, cv2.WINDOW_NORMAL)
        cv2.resizeWindow(self.hsv_window , 800, 800)

        cv2.createTrackbar(self.low_H_name, self.hsv_window, self.low_h, self.max_value_H, self.low_h_cb_)
        cv2.createTrackbar(self.high_H_name, self.hsv_window, self.high_h, self.max_value_H, self.high_h_cb_)
        cv2.createTrackbar(self.low_S_name, self.hsv_window, self.low_s, self.max_value, self.low_s_cb_)
        cv2.createTrackbar(self.high_S_name, self.hsv_window, self.high_s, self.max_value, self.high_s_cb_)
        cv2.createTrackbar(self.low_V_name, self.hsv_window, self.low_v, self.max_value, self.low_v_cb_)
        cv2.createTrackbar(self.high_V_name, self.hsv_window, self.high_v, self.max_value, self.high_v_cb_)

        while True:

            frame_threshold_image = cv2.inRange(frame, (self.low_h, self.low_s, self.low_v), (self.high_h, self.high_s, self.high_v)) #type: ignore
            cv2.imshow(self.hsv_window, frame_threshold_image)

            key = cv2.waitKey(30) & 0xFF
            if key == ord('c'):
                cv2.destroyWindow(self.hsv_window)
                return frame_threshold_image
            if key == ord('q'):
                raise CalibrationException('User quit')

    def remove_excess_detected_points(self, frame: MatLike)-> MatLike:
        """ Allow user to draw rectangles over unwanted points to remove them. Also includes a slider that allows the user to 
        adjust the minimum contour size to ensure all points are included and any unnecessary points are not.
        
        Args:
            frame (MatLike): HSV thresholded image
        
        Raises:
            CalibrationException: User quit
        
        Returns:
            MatLike: Input image, but with selected points removed
        """

        self.start_corner = None
        self.end_corner = None
        remove_window = 'Use the middle mouse button to draw rectanlges over points that need to be removed'
        cv2.namedWindow(remove_window, cv2.WINDOW_NORMAL)
        cv2.resizeWindow(remove_window, 800, 800)

        cv2.createTrackbar('Minimum Contour Area', remove_window, self.minimum_contour_area, 30, self.contour_area_slider_cb_)
        cv2.setMouseCallback(remove_window, self.rectangle_corners_mouse_cb_)

        while True:
            remove_img = frame.copy()
            if len(remove_img.shape) < 3:
                remove_img = cv2.cvtColor(remove_img, cv2.COLOR_GRAY2BGR)
            if self.start_corner is not None:

                if self.end_corner is None:
                    if self.rectangle_x is not None and self.rectangle_y is not None:
                        rectangle_end_point = (self.rectangle_x, self.rectangle_y)
                    else:
                        rectangle_end_point = self.start_corner

                    cv2.rectangle(remove_img, self.start_corner, rectangle_end_point, (0, 255, 0), 2)
                    # TODO: incorporate select region
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
            cv2.drawContours(remove_img, filtered_contours, -1, (0, 0, 255), -1) #type: ignore
            cv2.imshow(remove_window, remove_img)

            key = cv2.waitKey(30) & 0xFF
            if key == ord('c'):
                cv2.destroyWindow(remove_window)
                return frame
            if key == ord('q'):
                raise CalibrationException('User quit')

    def detect_contours(self, frame: MatLike, column_start: tuple[int, int], column_end: tuple[int, int], row_start: tuple[int, int], row_end: tuple[int, int])-> tuple[list[MatLike], list[MatLike], list[MatLike]]:
        """ Detects contours in image, selected row, and selected column greater than the minimum contour area
        
        Args:
            frame (MatLike): HSV thresholded image with excess points removed
            column_start (tuple[int, int]): start point for selected column
            column_end (tuple[int, int]): end point for selected column
            row_start (tuple[int, int]): start point for selected row
            row_end (tuple[int, int]): end point for selected row
        
        Raises:
            CalibrationException: Not enough holes detected
            CalibrationException: Too many holes detected
        
        Returns:
            tuple[list[MatLike], list[MatLike], list[MatLike]]: All contours in image, all contours in selected column, and all columns 
            in selected row greater than the minimum contour area set earlier
        """
        
        all_contours, _ = cv2.findContours(frame, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        selected_column_region = frame[column_start[1]:column_end[1], column_start[0]:column_end[0]]
        contours_in_selected_column, _ = cv2.findContours(selected_column_region, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        selected_row_region = frame[row_start[1]:row_end[1], row_start[0]:row_end[0]]
        contours_in_selected_row, _ = cv2.findContours(selected_row_region, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        all_contours = [c for c in all_contours if cv2.contourArea(c) >= self.minimum_contour_area]
        contours_in_selected_column = [c for c in contours_in_selected_column if cv2.contourArea(c) >= self.minimum_contour_area]
        contours_in_selected_row = [c for c in contours_in_selected_row if cv2.contourArea(c) >= self.minimum_contour_area]

        num_holes = len(all_contours)
        num_rows = len(contours_in_selected_column)
        num_columns = len(contours_in_selected_row)

        if num_holes < num_rows * num_columns:
            raise CalibrationException(f'Not enough holes detected! Minimum Contour Area: {self.minimum_contour_area}')
        elif num_holes > num_rows * num_columns:
            raise CalibrationException(f'Too many holes detected. Minimum Contour Area: {self.minimum_contour_area}')

        return (all_contours, contours_in_selected_column, contours_in_selected_row)

    def sort_all_holes(self, frame: MatLike, column_start_y: int, all_holes: list[MatLike], holes_in_column: list[MatLike])-> list[MatLike]:
        """Sort all holes in image from left to right and top to bottom
        
        Args:
            frame (MatLike): HSV thresholded image with excess points removed
            column_start_y (int): y-coordinate of the starting point of the selected column
            all_holes (list[MatLike]): list of all holes(contours) in the image
            holes_in_column (list[MatLike]): list of all holes(contours) in the selected column
        
        Raises:
            CalibrationException: Not able to properly sort holes
        
        Returns:
            list[MatLike]: Sorted centerpoints of all contours in image, sorted from left to right and top to bottom
        """
        
        # Find center of contours and sort
        center_points = []
        for hole in all_holes:
            # Calculate moments for each contour
            moments = cv2.moments(hole)

            # Calculate center of contour
            if moments["m00"] != 0:
                c_x = int(moments["m10"] / moments["m00"])
                c_y = int(moments["m01"] / moments["m00"])

                center_points.append((c_x, c_y))

        row_y = []
        for contour in holes_in_column:
            # Calculate moments for each contour
            moments = cv2.moments(contour)

            # Calculate center of contour
            if moments["m00"] != 0:
                row_y.append( int(moments["m01"] / moments["m00"]))

        row_y.sort()

        # Loop through each detected row and sort points in each row from left to right
        sorted_points = []

        for i in range(len(holes_in_column)):
            working_points = []

            if i == 0:
                lower_bound = 0
                upper_bound = column_start_y + row_y[i] + (row_y[i+1] - row_y[i])/2
            elif i == len(holes_in_column) - 1:
                lower_bound = column_start_y + row_y[i] - (row_y[i] - row_y[i-1])/2
                upper_bound = frame.shape[0]
            else:
                lower_bound = column_start_y + row_y[i] - (row_y[i] - row_y[i-1])/2
                upper_bound = column_start_y + row_y[i] + (row_y[i+1] - row_y[i])/2

            for x, y in center_points:
                if lower_bound <= y <= upper_bound:
                    working_points.append((x, y))

            sorted_points += sorted(working_points, key=lambda k: [k[0]])

            # Check to see if all points were sorted
        if not len(sorted_points) == len(center_points):
            raise CalibrationException('Not able to properly sort holes!')

        return sorted_points

    def create_mappings(self, sorted_holes: list[MatLike], num_rows: int, num_columns: int)-> tuple[MatLike, MatLike]:
        """Create mappings necessary for the calibration file. This mapping assigns pixel coordinates so that one inch in the image
        is 30 pixels, ie the distance between two holes in both x and y is assigned to be 30 pixels.
        
        Args:
            sorted_holes (list[MatLike]): List of all sorted centerpoints
            num_rows (int): number of rows of holes in image
            num_columns (int): number of columns of holes in image
        
        Returns:
            tuple[MatLike, MatLike]: Mapping for x coordinates, mapping for y coordinates
        """
        
        actual_points = []
        # Create grid, assigning each hole distance a certain number of pixels
        for i in range(num_rows):
            x = i * self.pixels_per_inch
            for j in range(num_columns):
                y = j * self.pixels_per_inch
                actual_points.append([x, y])

        grid_x, grid_y = np.mgrid[0:(num_rows-1)*self.pixels_per_inch, 0:(num_columns-1)*self.pixels_per_inch]

        destination = np.array(actual_points)
        source = np.array(sorted_holes)

        grid_z = griddata(destination, source, (grid_x, grid_y), method='cubic')
        map_x = np.append([], [ar[:,0] for ar in grid_z]).reshape((num_rows-1)*self.pixels_per_inch,(num_columns-1)*self.pixels_per_inch).astype('float32')
        map_y = np.append([], [ar[:,1] for ar in grid_z]).reshape((num_rows-1)*self.pixels_per_inch,(num_columns-1)*self.pixels_per_inch).astype('float32')

        return (map_x, map_y)

    def get_frame(self):
        return self.frame
