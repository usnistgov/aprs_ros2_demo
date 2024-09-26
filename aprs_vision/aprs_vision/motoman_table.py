from aprs_vision.vision_table import VisionTable
from aprs_vision.gear_detection import GearDetection
from aprs_interfaces.msg import SlotInfo
from geometry_msgs.msg import Point

class MotomanTable(VisionTable):
    #TODO Fill in real values
    # table_origin = 
    # tray_height = 
    # gear_height = 

    video_stream = "http://192.168.1.107/mjpg/video.mjpg"
    map_x_image = 'motoman_table_map_x.npy'
    map_y_image = 'motoman_table_map_y.npy'
    background_image = 'motoman_table_background.jpg'
    publish_frames = True

    #TODO Fill in real values
    # top_left_x = 417
    # top_left_y = 180
    # bottom_left_x = 428
    # bottom_left_y = 944
    # top_right_x = 1043
    # top_right_y = 179
    # bottom_right_x = 1035
    # bottom_right_y = 955
    # grid_hsv_lower = (100, 0, 0)
    # grid_hsv_upper = (255, 255, 120)
    # calibrate_rows = 24
    # calibrate_columns = 19
    # generate_map_area = 10

    conversion_factor = 0.8466 # 30 pixels is 25.4 mm so 1 pixel is .8466 mm

    #TODO Fill in real values
    # background_threshold = 12
    # gear_detection_values = {
    #     SlotInfo.LARGE: GearDetection((40, 30, 123), (85, 165, 215), 60),
    #     SlotInfo.MEDIUM: GearDetection((6, 100, 140), (24, 162, 255), 44),
    #     SlotInfo.SMALL: GearDetection((18, 80, 170), (45, 135, 255), 30)
    # }

    base_frame = 'motoman_base_link'

    def __init__(self):
        super().__init__("motoman_table", "motoman_vision")