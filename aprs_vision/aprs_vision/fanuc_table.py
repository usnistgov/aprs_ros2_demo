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

class FanucTable(VisionTable):
    table_origin = Point(x=228.591, y=228.287, z= -25.4)
    tray_height = 0.0015
    gear_height = 0.005

    video_stream = "http://192.168.1.104/mjpg/video.mjpg"
    map_x_image = 'fanuc_table_map_x.npy'
    map_y_image = 'fanuc_table_map_y.npy'
    background_image = 'fanuc_table_background.jpg'
    publish_frames = True

    grid_hsv_lower = (0, 0, 1)
    grid_hsv_upper = (255, 255, 120)
    generate_map_area = 5

    angle_offset = 0
    suffix = 'fanuc_table'
    vision_location = "table_vision"

    conversion_factor = 0.8466 # 30 pixels is 25.4 mm so 1 pixel is .8466 mm

    background_threshold = 20
    gear_detection_values = {
        SlotInfo.LARGE: GearDetection((40, 30, 123), (85, 165, 215), 60),
        SlotInfo.MEDIUM: GearDetection((6, 100, 140), (24, 162, 255), 44),
        SlotInfo.SMALL: GearDetection((18, 80, 170), (45, 135, 255), 30)
    }

    base_frame = 'fanuc_base_link'

    def __init__(self):
        super().__init__("table_vision", "fanuc")
