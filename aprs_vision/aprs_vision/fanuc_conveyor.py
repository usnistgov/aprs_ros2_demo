import cv2
import numpy as np

from aprs_vision.vision_table import VisionTable
from aprs_vision.gear_detection import GearDetection
from aprs_interfaces.msg import SlotInfo
from geometry_msgs.msg import Point
from scipy.interpolate import griddata
from rclpy.qos import qos_profile_default

from cv2.typing import MatLike
from typing import Optional

class FanucConveyor(VisionTable):
    table_origin = Point(x=279.4, y=-758.825, z=38.1)
    tray_height = 0.0015
    gear_height = 0.005

    video_stream = "http://192.168.1.108/mjpg/video.mjpg"
    map_x_image = 'fanuc_conveyor_map_x.npy'
    map_y_image = 'fanuc_conveyor_map_y.npy'
    background_image = 'fanuc_conveyor_background.jpg'
    publish_frames = True

    # top_left_x = 718
    # top_left_y = 41
    # bottom_left_x = 722
    # bottom_left_y = 307
    # top_right_x = 1148
    # top_right_y = 58
    # bottom_right_x = 1158
    # bottom_right_y = 308
    top_left_x = 0
    top_left_y = 0
    bottom_left_x = 0
    bottom_left_y = 0
    top_right_x = 0
    top_right_y = 0
    bottom_right_x = 0
    bottom_right_y = 0
    grid_hsv_lower = (0, 0, 10)
    grid_hsv_upper = (255, 255, 140)
    calibrate_rows = 18
    calibrate_columns = 32
    generate_map_area = 5
    tray_detection_lower = (0,51,0)
    tray_detection_upper = (180,255,255)
    click = 0
    refpt = []
    rows = []
    columns = []
    clicked = 0
    pixels_in_one_inch = 30

    angle_offset = 0
    suffix = 'fanuc_conveyor'
    vision_location = 'conveyor_vision'

    conversion_factor = 0.8466 # 30 pixels is 25.4 mm so 1 pixel is .8466 mm

    background_threshold = 15
    gear_detection_values = {
        SlotInfo.LARGE: GearDetection((47, 112, 144), (77, 195, 212), 60),
        SlotInfo.MEDIUM: GearDetection((10, 147, 215), (37, 188, 255), 44),
        SlotInfo.SMALL: GearDetection((17, 69, 179), (33, 148, 255), 30)
    }

    base_frame = 'fanuc_base_link'

    def __init__(self):
        super().__init__("conveyor_vision", "fanuc")
    
    def generate_grid_maps(self, frame: MatLike, filepath: str) -> Optional[bool]:
        # The point for calibrate trays for the fanuc conveyor system lines up with the tape on the table

        self.calibration_image = frame.copy()
        self.calibration_image[0:40,0:500] = (0,0,0)

        self.counter = 0
        self.erase_counter = 0
        self.input_mode = 0
        self.font = cv2.FONT_HERSHEY_SIMPLEX
        self.font_origin = (20,30)
        self.font_scale = 1
        self.font_color = (255,255,255)
        self.font_thickness = 2

        self.text_over_image('Select Top Left Point', self.calibration_image)

        cv2.namedWindow('window', cv2.WINDOW_NORMAL)
        cv2.resizeWindow('window', 800, 800)
        cv2.imshow('window',self.calibration_image)
        cv2.setMouseCallback('window',self.click_input_on_image)
        cv2.waitKey(0)
        cv2.destroyAllWindows()

        top_left_x = self.refpt[0][0]
        top_left_y = self.refpt[0][1]
        bottom_left_x = self.refpt[1][0]
        bottom_left_y = self.refpt[1][1]
        top_right_x = self.refpt[2][0]
        top_right_y = self.refpt[2][1]
        bottom_right_x = self.refpt[3][0]
        bottom_right_y = self.refpt[3][1]

        self.refpt.clear()

        # Corners are manually deduced from location of screw heads in table
        top_left = (top_left_x, top_left_y)
        top_right = (top_right_x, top_right_y)
        bottom_right = (bottom_right_x, bottom_right_y)
        bottom_left = (bottom_left_x, bottom_left_y)

        # Black out everything from image that is not the active region
        fanuc_table_corners = np.array([top_right, bottom_right, bottom_left, top_left])

        maskImage = np.zeros(frame.shape, dtype=np.uint8)
        cv2.drawContours(maskImage, [fanuc_table_corners], 0, (255, 255, 255), -1)

        active_region = cv2.bitwise_and(frame, maskImage)

        # Detect optical table holes 
        blur = cv2.GaussianBlur(active_region,(5,5),0)

        hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)

        threshold = cv2.inRange(hsv, self.grid_hsv_lower, self.grid_hsv_upper) # type: ignore

        top_left = (top_left_x, top_left_y)
        top_right = (top_right_x, top_right_y)
        bottom_right = (bottom_right_x, bottom_right_y)
        bottom_left = (bottom_left_x, bottom_left_y)

        corners = np.array([top_right, bottom_right, bottom_left, top_left])

        mask2 = np.zeros(threshold.shape, dtype=np.uint8)
        
        cv2.drawContours(mask2, [corners], -1, 255, -1) # type: ignore
    
        just_holes = cv2.bitwise_and(threshold, mask2)
        cv2.drawContours(just_holes,[corners],-1,0,2) # type: ignore

        self.just_holes_calibration = just_holes.copy()
        self.text_over_image('Select Points to Erase using Right Click',self.just_holes_calibration)
        # self.erase_counter = 1

        cv2.namedWindow('window', cv2.WINDOW_NORMAL)
        cv2.resizeWindow('window', 800, 800)
        cv2.imshow('window', self.just_holes_calibration)
        cv2.setMouseCallback('window',self.draw_box_on_image)
        cv2.waitKey(0)

        just_holes = self.just_holes_calibration
        cv2.imwrite('intermediate_image.jpg', just_holes)

        just_holes[0:40,0:800] = 0

        self.text_over_image('Please Select a Single Column',just_holes)
        cv2.setMouseCallback('window', self.select_row)
        cv2.imshow('window', just_holes)
        cv2.resizeWindow('window', 800, 800)
        cv2.waitKey(0)
        just_holes[0:40,0:800] = 0
        self.text_over_image('Please Select a Single Row', just_holes)
        cv2.imshow('window', just_holes)
        cv2.setMouseCallback('window', self.select_column)
        cv2.waitKey(0)
        cv2.destroyAllWindows()
        just_holes[0:40,0:800] = 0

        contours, _ = cv2.findContours(just_holes, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        rows_contours, _ = cv2.findContours(just_holes[self.rows[0][1]:self.rows[1][1],self.rows[0][0]:self.rows[1][0]], cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        columns_contours, _ = cv2.findContours(just_holes[self.columns[0][1]:self.columns[1][1],self.columns[0][0]:self.columns[1][0]], cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        i=0
        filtered_contours = []
        for contour in contours:
            area = cv2.contourArea(contour)
            if area >= self.generate_map_area:
                filtered_contours.append(contour)
                i = i+1
        center_points = []

        for contour in filtered_contours:
            # Calculate moments for each contour
            M = cv2.moments(contour)

            # Calculate center of contour
            if M["m00"] != 0:
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])

                center_points.append((cX, cY))

        row_points = []
        for row in rows_contours:
            # Calculate moments for each contour
            M = cv2.moments(row)

            # Calculate center of contour
            if M["m00"] != 0:
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])

                row_points.append((cX, cY))
        row_points.sort(key=lambda point: (point[1]))

        
        # rows = sum(1 for x,y in center_points if top_left_x < x < top_left_x + 45)
        # columns = sum(1 for x,y in center_points if bottom_left_y - 10 < y < bottom_left_y + 10)

        rows = len(rows_contours)
        columns = len(columns_contours)

        print(f"Found: {i}  Rows: {rows} Columns: {columns}")

        if not len(filtered_contours) == rows * columns:
            self.get_logger().error("Not able to detect all holes")
            return False

        # center_y_left = 56
        # center_y_right = 72
        sorted_points = []
        working_points = []

        for i in range(rows):
            if i != rows-1:
                dist = (row_points[i+1][1] - row_points[i][1])/2
            else:
                dist = (row_points[i][1] - row_points[i-1][1])/2
            # pt1 = (row_points[i][0] + self.rows[0][0], row_points[i][1] - round(dist) + self.rows[0][1])
            # pt2 = (row_points[i][0] + 1300 + self.rows[0][0],row_points[i][1] - round(dist) + self.rows[0][1])
            # pt3 = (row_points[i][0] + self.rows[0][0], row_points[i][1] + round(dist) + self.rows[0][1])
            # pt4 = (row_points[i][0] + 1300 + self.rows[0][0],row_points[i][1] + round(dist) + self.rows[0][1])
            # cv2.line(just_holes,pt1,pt2,120,2) #type: ignore
            # cv2.line(just_holes,pt3,pt4,255,2) #type: ignore
            
            # cv2.namedWindow('window')
            # cv2.resizeWindow('window', 800, 800)
            # cv2.imshow('window', just_holes)
            # cv2.waitKey(0)
            for point in center_points:
                if row_points[i][1] - dist + self.rows[0][1]< point[1] < row_points[i][1] + dist + self.rows[0][1]:
                    working_points.append(point)

                # start = (top_left_x,center_y_left - 7)
                # end = (top_right_x,center_y_right - 7)
                # start_1 = (top_left_x,center_y_left + 7)
                # end_1 = (top_right_x,center_y_right + 7)
                # slope_upper = (end[1]-start[1])/(end[0]-start[0])
                # slope_lower = (end_1[1]-start_1[1])/(end_1[0]-start_1[0])
                # y_int_upper = -slope_upper * start[0] + start[1]
                # y_int_lower = -slope_lower * start_1[0] + start_1[1]
                # if slope_upper * point[0] + y_int_upper <= point[1] <= slope_lower * point[0] + y_int_lower:
                #     working_points.append(point)
                
                
            sorted_points += sorted(working_points, key=lambda k: [k[0]])

            # cv2.line(just_holes,start,end,255,2)
            # cv2.line(just_holes,start_1,end_1,255,2)

            working_points.clear()

        # print(f"Sorted Points:  {len(sorted_points)}  Center Points:  {len(center_points)}")

        if not len(sorted_points) == len(center_points):
            self.get_logger().error("Not able to properly sort holes")
            return False
        
        actual_points = []

        for i in range(rows):
            x = (i * self.pixels_in_one_inch)
            for j in range(columns):
                y = (j * self.pixels_in_one_inch)
                actual_points.append([x, y])

        grid_x, grid_y = np.mgrid[0:rows*self.pixels_in_one_inch, 0:columns*self.pixels_in_one_inch]

        destination = np.array(actual_points)
        source = np.array(sorted_points)

        grid_z = griddata(destination, source, (grid_x, grid_y), method='cubic')
        map_x = np.append([], [ar[:,0] for ar in grid_z]).reshape(rows*self.pixels_in_one_inch,columns*self.pixels_in_one_inch)
        map_y = np.append([], [ar[:,1] for ar in grid_z]).reshape(rows*self.pixels_in_one_inch,columns*self.pixels_in_one_inch)
        self.map_x = map_x.astype('float32')
        self.map_y = map_y.astype('float32')

        np.save(f"{filepath}{self.map_x_image}", self.map_x)
        np.save(f"{filepath}{self.map_y_image}", self.map_y)

        return True
    
    def remove_background(self, frame: MatLike) -> MatLike:
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        trays = cv2.inRange(hsv, self.tray_detection_lower, self.tray_detection_upper ) # type: ignore
        
        return cv2.bitwise_and(frame,frame,mask=trays)
    
    def click_input_on_image(self,event,x,y,flags,param):
        if event == cv2.EVENT_LBUTTONDOWN:
            # print(f"The point selected is x: {x}, y: {y}")
            self.refpt.append((x,y))
            self.calibration_image[0:40,0:800] = (0,0,0)
            if self.erase_counter == 0:
                pass
            elif self.erase_counter % 2 == 0:
                self.just_holes_calibration[0:40,0:800] = 0
                self.text_over_image('Select Top Left of Point to Erase', self.just_holes_calibration)
                cv2.imshow('window', self.just_holes_calibration)
                self.erase_counter = self.erase_counter + 1
            else:
                self.just_holes_calibration[0:40,0:800] = 0
                self.text_over_image('Select Bottom Right of Point to Erase', self.just_holes_calibration)
                cv2.imshow('window', self.just_holes_calibration)
                self.erase_counter = self.erase_counter + 1
            if self.counter == 0:
                self.text_over_image('Select Bottom Left Point', self.calibration_image)
                cv2.imshow('window', self.calibration_image)
            elif self.counter == 1:
                self.text_over_image('Select Top Right Point', self.calibration_image)
                cv2.imshow('window', self.calibration_image)
            elif self.counter == 2:
                self.text_over_image('Select Bottom Right Point', self.calibration_image)
                cv2.imshow('window', self.calibration_image)

            self.counter = self.counter + 1
            if self.counter == 4:
                self.text_over_image("Select any key to progress", self.calibration_image)
                cv2.imshow('window', self.calibration_image)

    def draw_box_on_image(self,event,x,y,flags,param):
        rectangle_image = self.just_holes_calibration.copy()
        if self.refpt:
            rectangle_image = self.just_holes_calibration.copy()
            cv2.rectangle(rectangle_image, (self.refpt[0][0],self.refpt[0][1]), (x,y), 120, 1)
            cv2.imshow('window', rectangle_image)
        if event == cv2.EVENT_RBUTTONDOWN:
            self.refpt.append((x,y))
        if event == cv2.EVENT_RBUTTONUP:
            cv2.rectangle(self.just_holes_calibration, (self.refpt[0][0],self.refpt[0][1]), (x,y), 0, -1) #type: ignore
            cv2.imshow('window', self.just_holes_calibration)
            self.refpt.clear()

    def select_row(self,event,x,y,flags,param):
        rectangle_image = self.just_holes_calibration.copy()
        rectangle_image[0:40,0:800] = 0
        if event == cv2.EVENT_RBUTTONDOWN:
            self.rows.append((x,y))
            self.clicked = 1
        if event == cv2.EVENT_RBUTTONUP:
            self.rows.append((x,y))
            self.clicked = 0
            rectangle_image[0:40,0:800] = 0
            self.text_over_image('Press any key to continue',rectangle_image)
            cv2.imshow('window', rectangle_image)
        if self.clicked == 1:
            rectangle_image = self.just_holes_calibration.copy()
            cv2.rectangle(rectangle_image, (self.rows[0][0],self.rows[0][1]), (x,y), 120, 2)
            cv2.imshow('window', rectangle_image)

    def select_column(self,event,x,y,flags,param):
        rectangle_image = self.just_holes_calibration.copy()
        rectangle_image[0:40,0:800] = 0
        if event == cv2.EVENT_RBUTTONDOWN:
            self.columns.append((x,y))
            self.clicked = 1
        if event == cv2.EVENT_RBUTTONUP:
            self.columns.append((x,y))
            self.clicked = 0
            rectangle_image[0:40,0:800] = 0
            self.text_over_image('Press any key to continue',rectangle_image)
            cv2.imshow('window', rectangle_image)
        if self.clicked == 1:
            rectangle_image = self.just_holes_calibration.copy()
            cv2.rectangle(rectangle_image, (self.columns[0][0],self.columns[0][1]), (x,y), 120, 2)
            cv2.imshow('window', rectangle_image)

    def text_over_image(self, text, image):
        image = cv2.putText(
        image,
        text, 
        self.font_origin,
        self.font,
        self.font_scale,
        self.font_color,
        self.font_thickness,
        cv2.LINE_AA
        )     
    
    