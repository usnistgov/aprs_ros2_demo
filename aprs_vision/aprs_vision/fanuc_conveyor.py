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

    grid_hsv_lower = (0, 0, 10)
    grid_hsv_upper = (255, 255, 140)
    generate_map_area = 5

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
    
    