from aprs_vision.vision_table import VisionTable
from aprs_vision.gear_detection import GearDetection
from aprs_interfaces.msg import SlotInfo
from geometry_msgs.msg import Point
import math

class FanucTable(VisionTable):
    table_origin = Point(x=228.591, y=228.287, z=-0.01)
    tray_height = 0.017
    gear_height = 0.02

    video_stream = "http://192.168.1.104/mjpg/video.mjpg"
    map_x_image = 'fanuc_table_map_x.npy'
    map_y_image = 'fanuc_table_map_y.npy'
    background_image = 'fanuc_table_background.jpg'
    publish_frames = True

    top_left_x = 417
    top_left_y = 180
    bottom_left_x = 428
    bottom_left_y = 944
    top_right_x = 1043
    top_right_y = 179
    bottom_right_x = 1035
    bottom_right_y = 955
    grid_hsv_lower = (100, 0, 0)
    grid_hsv_upper = (255, 255, 120)
    calibrate_rows = 24
    calibrate_columns = 19
    generate_map_area = 10

    angle_offset = 0
    suffix = 'fanuc'

    conversion_factor = 0.8466 # 30 pixels is 25.4 mm so 1 pixel is .8466 mm

    background_threshold = 20
    gear_detection_values = {
        SlotInfo.LARGE: GearDetection((40, 30, 123), (85, 165, 215), 60),
        SlotInfo.MEDIUM: GearDetection((6, 100, 140), (24, 162, 255), 44),
        SlotInfo.SMALL: GearDetection((18, 80, 170), (45, 135, 255), 30)
    }

    base_frame = 'fanuc_base_link'

    def __init__(self):
        super().__init__("fanuc_table", "fanuc_vision")