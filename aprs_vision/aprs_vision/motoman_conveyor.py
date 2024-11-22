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

class MotomanConveyor(VisionTable):
    #TODO Fill in real values
    table_origin = Point(x=-165.1, y=746.125, z=-0.01)
    tray_height = 0.017
    gear_height = 0.02

    video_stream = "http://192.168.1.107/mjpg/video.mjpg"
    map_x_image = 'motoman_conveyor_map_x.npy'
    map_y_image = 'motoman_conveyor_map_y.npy'
    background_image = 'motoman_conveyor_background.jpg'
    publish_frames = True

    grid_hsv_lower = (0, 0, 10)
    grid_hsv_upper = (255, 255, 125)
    generate_map_area = 5

    angle_offset = math.pi
    suffix = 'motoman_conveyor'
    vision_location = 'conveyor_vision'

    conversion_factor = 0.8466 # 30 pixels is 25.4 mm so 1 pixel is .8466 mm

    background_threshold = 15
    gear_detection_values = {
        SlotInfo.LARGE: GearDetection((47, 112, 144), (77, 195, 212), 60),
        SlotInfo.MEDIUM: GearDetection((10, 147, 215), (37, 188, 255), 44),
        SlotInfo.SMALL: GearDetection((17, 69, 179), (33, 148, 255), 30)
    }

    base_frame = 'motoman_base_link'

    def __init__(self):
        super().__init__("conveyor_vision", "motoman")
    
    