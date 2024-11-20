import cv2
import numpy as np
from aprs_vision.vision_table import VisionTable
from aprs_vision.gear_detection import GearDetection
from aprs_interfaces.msg import SlotInfo
from cv2.typing import MatLike
from typing import Optional
from scipy.interpolate import griddata

class TeachTable(VisionTable):
    video_stream = "http://192.168.1.105/mjpg/video.mjpg"
    map_x_image = 'teach_table_map_x.npy'
    map_y_image = 'teach_table_map_y.npy'
    background_image = 'teach_table_background.jpg'
    publish_frames = False

    grid_hsv_lower = (0, 0, 130)
    grid_hsv_upper = (255, 50, 255)
    generate_map_area = 5

    suffix = 'teach'
    vision_location = 'table_vision'

    conversion_factor = 0.8466 # 30 pixels is 25.4 mm so 1 pixel is .8466 mm

    gear_detection_values = {
        SlotInfo.LARGE: GearDetection((40, 90, 123), (83, 186, 255), 60),
        SlotInfo.MEDIUM: GearDetection((0, 100, 230), (20, 175, 255), 44),
        SlotInfo.SMALL: GearDetection((10, 10, 205), (45, 145, 255), 30)
    }

    def __init__(self):
        super().__init__("table_vision", "teach")

    def remove_background(self, frame: MatLike) -> MatLike:
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # cv2.imwrite('hsv.jpg', hsv)

        no_background = cv2.inRange(hsv, (0, 0, 135), (255, 255, 255)) # type: ignore      
        
        canvas = np.zeros(no_background.shape, dtype=np.uint8)

        contours,_ = cv2.findContours(no_background, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        filtered_contours = []
        for c in contours:
            if cv2.contourArea(c) > 100:
                filtered_contours.append(c)

        cv2.drawContours(canvas, filtered_contours, -1, color=255, thickness=cv2.FILLED) # type: ignore

        return cv2.bitwise_and(frame, frame, mask=canvas)