import cv2
import numpy as np
from numpy.typing import ArrayLike
from scipy.interpolate import griddata

from typing import Optional
from cv2.typing import MatLike

class CalibrationTool():
    grid_hsv_lower = (0, 0, 10)
    grid_hsv_upper = (255, 255, 125)
    pixels_per_inch = 30
    minimum_contour_area = 10
    clicked = 0
    
    def __init__(self, full_frame: MatLike):
        self.current_frame = full_frame
        self.drawing_rectangle = False

        self.column_start: Optional[tuple[int, int]] = None
        self.column_end: Optional[tuple[int, int]] = None
        self.row_start: Optional[tuple[int, int]] = None
        self.row_end: Optional[tuple[int, int]] = None
        
    def rotation_change(self, val):
        self.rotate_image(val/5)

    def rotate_image(self, angle):
        self.angle = angle
        self.rotation_matrix = cv2.getRotationMatrix2D((self.current_frame.shape[1] / 2, self.current_frame.shape[0] / 2), angle, 1)
        self.rotated_image = cv2.warpAffine(self.current_frame, self.rotation_matrix, (self.current_frame.shape[1], self.current_frame.shape[0]))
        grid_image = self.rotated_image.copy()
        self.draw_grid(grid_image, 15, 20, (100, 100, 100), 1)
        cv2.imshow('Image Rotation', grid_image)

    def mouse_callback(self, event,x,y,flags,param):
        if event == cv2.EVENT_MBUTTONDOWN:
            self.start_corner = (x,y)
            self.drawing_rectangle = True
        if self.drawing_rectangle:
            rectangle_image = self.current_frame.copy()
            cv2.rectangle(rectangle_image, self.start_corner, (x,y), (0, 0, 255), 3)
            cv2.imshow('Corner Selection', rectangle_image)
        if event == cv2.EVENT_MBUTTONUP:
            self.end_corner = (x,y)
            self.drawing_rectangle = False

    def draw_grid(self, img, rows, cols, color=(0, 255, 0), thickness=1):
        """Draws a grid over the image."""

        h, w, _ = img.shape

        for i in range(0, rows + 1):
            y = int(i * h / rows)
            cv2.line(img, (0, y), (w, y), color, thickness)

        for j in range(0, cols + 1):
            x = int(j * w / cols)
            cv2.line(img, (x, 0), (x, h), color, thickness)

        return img

    def contour_area_slider(self, val):
        contour_image = self.just_holes_original.copy()
        contours, _ = cv2.findContours(contour_image, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        filtered_contours = []
        for contour in contours:
            area = cv2.contourArea(contour)
            if area >= val:
                filtered_contours.append(contour)
        self.minimum_contour_area = val

        colored_image = cv2.cvtColor(contour_image,cv2.COLOR_GRAY2BGR)
        cv2.drawContours(colored_image,filtered_contours,-1,(0,255,0),-1) #type: ignore
        cv2.imshow('Remove Excess Points', colored_image)
        self.just_holes_calibration = colored_image.copy()

    def draw_box_on_image(self,event,x,y,flags,param):
        rectangle_image = self.just_holes_calibration.copy()
        if event == cv2.EVENT_MBUTTONDOWN:
            self.start_point = (x,y)
            self.drawing_rectangle = True
        if event == cv2.EVENT_MBUTTONUP:
            cv2.rectangle(self.just_holes_calibration, self.start_point, (x,y), 0, -1) #type: ignore
            cv2.rectangle(self.just_holes_original, self.start_point, (x,y), 0, -1) #type: ignore
            cv2.imshow('Remove Excess Points', self.just_holes_calibration)
            self.drawing_rectangle = False
        if self.drawing_rectangle:
            rectangle_image = self.just_holes_calibration.copy()
            cv2.rectangle(rectangle_image, self.start_point, (x,y), (120,120,120), 1)
            cv2.imshow('Remove Excess Points', rectangle_image)
    
    def select_column(self,event,x,y,flags,param):
        rectangle_image = self.current_frame.copy()
        
        if event == cv2.EVENT_MBUTTONDOWN:
            self.column_start = (x, y)

        if self.column_start is not None and self.column_end is None:
            rectangle_image = self.current_frame.copy()
            cv2.rectangle(rectangle_image, self.column_start, (x,y), 120, 2)
            cv2.imshow('Select Columns and Rows', rectangle_image)
            
        if event == cv2.EVENT_MBUTTONUP and self.column_start is not None:
            self.column_end = (x, y)
            cv2.rectangle(rectangle_image, self.column_start, self.column_end, 120, 2)
            cv2.imshow('Select Columns and Rows', rectangle_image)

    def select_row(self,event,x,y,flags,param):
        rectangle_image = self.current_frame.copy()
        
        if event == cv2.EVENT_MBUTTONDOWN:
            self.row_start = (x, y)

        if self.row_start is not None and self.row_end is None:
            rectangle_image = self.current_frame.copy()
            cv2.rectangle(rectangle_image, self.row_start, (x,y), 120, 2)
            cv2.imshow('Select Columns and Rows', rectangle_image)
            
        if event == cv2.EVENT_MBUTTONUP and self.row_start is not None:
            self.row_end = (x, y)
            cv2.rectangle(rectangle_image, self.row_start, self.row_end, 120, 2)
            cv2.imshow('Select Columns and Rows', rectangle_image)

    def run_calibration(self)-> Optional[tuple[float, tuple[int, int], tuple[int, int], MatLike, MatLike]]:
        # Rotate image so grid of holes is perpendicular
        self.rotated_image = self.current_frame
        cv2.namedWindow('Image Rotation', cv2.WINDOW_NORMAL)
        cv2.resizeWindow('Image Rotation' , 800, 800)
        cv2.imshow('Image Rotation', self.current_frame)
        cv2.createTrackbar('Angle','Image Rotation',0,50,self.rotation_change)
        cv2.setTrackbarMin('Angle', 'Image Rotation', -50)
        rotation_set = False
        while not rotation_set:
            key = cv2.waitKey(0) & 0xFF
            if key == ord('c'):
                rotation_set = True
        
        self.current_frame = self.rotated_image
        rectangle_set = False
        cv2.destroyAllWindows()

        # Determine cropping region
        cv2.namedWindow('Corner Selection', cv2.WINDOW_NORMAL)
        cv2.resizeWindow('Corner Selection' , 800, 800)
        cv2.imshow('Corner Selection', self.current_frame)
        cv2.setMouseCallback('Corner Selection',self.mouse_callback)
        while not rectangle_set:
            key = cv2.waitKey(0) & 0xFF
            if key == ord('c'):
                rectangle_set = True
        
        # Crop image 
        offset = -10
        
        self.current_frame = self.current_frame[
            self.start_corner[1] - offset:self.end_corner[1] + offset,
            self.start_corner[0] - offset:self.end_corner[0] + offset
        ]

        cropped_image = self.current_frame.copy()
        
        cv2.destroyAllWindows()

        # Threshold image to remove everything but holes 
        blur = cv2.GaussianBlur(self.current_frame,(5,5),0)
        hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)
        threshold = cv2.inRange(hsv, self.grid_hsv_lower, self.grid_hsv_upper) # type: ignore

        self.just_holes_calibration = threshold.copy()
        self.just_holes_original = self.just_holes_calibration

        # Mouse callback on window to allow for a middle-click drag to remove any excess points
        cv2.namedWindow('Remove Excess Points', cv2.WINDOW_NORMAL)
        cv2.resizeWindow('Remove Excess Points' , 800, 800)
        cv2.setMouseCallback('Remove Excess Points',self.draw_box_on_image)
        cv2.createTrackbar("Minimum contour area", 'Remove Excess Points', self.minimum_contour_area, 30, self.contour_area_slider)
        cv2.imshow('Remove Excess Points', self.just_holes_calibration)
        
        excess_points_removed = False
        while not excess_points_removed:
            key = cv2.waitKey(0) & 0xFF
            if key == ord('c'):
                excess_points_removed = True

        cv2.destroyAllWindows()

        self.current_frame = self.just_holes_original

        # Select region for columns and rows
        cv2.namedWindow('Select Columns and Rows', cv2.WINDOW_NORMAL)
        cv2.resizeWindow('Select Columns and Rows' , 800, 800)
        while True:
            cv2.setMouseCallback('Select Columns and Rows', self.select_column)
            cv2.imshow('Select Columns and Rows', self.current_frame)
            key = cv2.waitKey(0) & 0xFF
            if key == ord('r'):
                self.column_start = None
                self.column_end = None
                
            elif key == ord('c'):
                break

        # Mouse callback for selecting a single row
        while True:
            cv2.imshow('Select Columns and Rows', self.current_frame)
            cv2.setMouseCallback('Select Columns and Rows', self.select_row)
            key = cv2.waitKey(0) & 0xFF
            if key == ord('r'):
                self.row_start = None
                self.row_end = None

            elif key == ord('c'):
                break
            
        cv2.destroyAllWindows()

        # Detect contours 
        all_contours, _ = cv2.findContours(self.current_frame, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        if self.column_start is not None and self.column_end is not None:
            selected_column_region = self.current_frame[self.column_start[1]:self.column_end[1], self.column_start[0]:self.column_end[0]]
        else:
            print("something bad happened")
            return None
        
        contours_in_selected_column, _ = cv2.findContours(selected_column_region, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        
        if self.row_start is not None and self.row_end is not None:
            selected_row_region = self.current_frame[self.row_start[1]:self.row_end[1], self.row_start[0]:self.row_end[0]]
        else:
            print("something bad happened")
            return None
        
        contours_in_selected_row, _ = cv2.findContours(selected_row_region, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        # Count num contours
        all_contours = [c for c in all_contours if cv2.contourArea(c) > self.minimum_contour_area]
        contours_in_selected_column = [c for c in contours_in_selected_column if cv2.contourArea(c) > self.minimum_contour_area]

        num_holes = len(all_contours)
        num_rows = len(contours_in_selected_column)
        num_columns = len([c for c in contours_in_selected_row if cv2.contourArea(c) > self.minimum_contour_area])

        print(f'rows: {num_rows}, columns: {num_columns}, num_holes: {num_holes}"')

        if not num_rows * num_columns == num_holes:
            print(f"I fucked up :(")
            return None

        # Find center of contours and sort
        center_points = []
        for contour in all_contours:
            # Calculate moments for each contour
            M = cv2.moments(contour)

            # Calculate center of contour
            if M["m00"] != 0:
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])

                center_points.append((cX, cY))
        
        row_y = []
        for contour in contours_in_selected_column:
            # Calculate moments for each contour
            M = cv2.moments(contour)

            # Calculate center of contour
            if M["m00"] != 0:
                row_y.append( int(M["m01"] / M["m00"]))

        row_y.sort()

        # Loop through each detected row and sort points in each row from left to right
        sorted_points = []

        for i in range(num_rows):
            working_points = []

            if i == 0:
                lower_bound = 0
                upper_bound = self.column_start[1] + row_y[i] + (row_y[i+1] - row_y[i])/2 
            elif i == num_rows - 1:
                lower_bound = self.column_start[1] + row_y[i] - (row_y[i] - row_y[i-1])/2 
                upper_bound = self.current_frame.shape[1]
            else:
                lower_bound = self.column_start[1] + row_y[i] - (row_y[i] - row_y[i-1])/2 
                upper_bound = self.column_start[1] + row_y[i] + (row_y[i+1] - row_y[i])/2 

            for x, y in center_points:
                if lower_bound <= y <= upper_bound:
                    working_points.append((x, y))
                     
            sorted_points += sorted(working_points, key=lambda k: [k[0]])

        # Check to see if all points were sorted
        if not len(sorted_points) == len(center_points):
            # self.get_logger().error("Not able to properly sort holes")
            print("Not able to properly sort holes")
            return None

        actual_points = []
        # Create grid, assigning each hole distance a certain number of pixels
        for i in range(num_rows):
            x = (i * self.pixels_per_inch)
            for j in range(num_columns):
                y = (j * self.pixels_per_inch)
                actual_points.append([x, y])

        grid_x, grid_y = np.mgrid[0:(num_rows-1)*self.pixels_per_inch, 0:(num_columns-1)*self.pixels_per_inch]

        destination = np.array(actual_points)
        source = np.array(sorted_points)

        grid_z = griddata(destination, source, (grid_x, grid_y), method='cubic')
        map_x = np.append([], [ar[:,0] for ar in grid_z]).reshape((num_rows-1)*self.pixels_per_inch,(num_columns-1)*self.pixels_per_inch).astype('float32')
        map_y = np.append([], [ar[:,1] for ar in grid_z]).reshape((num_rows-1)*self.pixels_per_inch,(num_columns-1)*self.pixels_per_inch).astype('float32')

        cv2.imshow("Remapped", cv2.remap(cropped_image, map_x, map_y, cv2.INTER_CUBIC))
        cv2.waitKey(0)

        return (self.angle, self.start_corner, self.end_corner, map_x, map_y)
