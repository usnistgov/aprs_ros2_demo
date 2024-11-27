from rclpy.node import Node
from aprs_interfaces.msg import Trays
from aprs_interfaces.srv import LocateTrays
from rclpy.qos import qos_profile_default
import customtkinter as ctk
from aprs_vision.stream_handler import StreamHandler
from PIL import Image
from ament_index_python.packages import get_package_share_directory
import os
import cv2

class SimpleGuiClass(Node):
    def __init__(self):
        super().__init__("simple_gui")

        self.main_window = ctk.CTk()

        motoman_trays_sub = self.create_subscription(
            Trays,
            "/motoman/table_trays_info",
            self.motoman_trays_cb_,
            qos_profile_default
        )

        video_stream = "http://192.168.1.110/mjpg/video.mjpg"

        # Stream Handler
        share_path = get_package_share_directory('aprs_vision')
        calibration_filepath = os.path.join(share_path, 'config', 'motoman_table_calibration.npz')

        self.stream_handler = StreamHandler(video_stream, calibration_filepath)
        self.img_max_height = 400

        self.create_timer(0.05, self.update_img)

        self.motoman_canvas = TrayCanvas(self.main_window)
        self.motoman_canvas.pack(padx=10)

        self.motoman_img = LiveImage(self.main_window)
        self.motoman_img.pack(padx=10)

    def update_img(self):
        cv_image = self.stream_handler.read_frame()
        cv_image = cv2.cvtColor(cv_image, cv2.COLOR_BGR2RGB)
        width = int(cv_image.shape[1] * self.img_max_height / cv_image.shape[0])
        self.motoman_img.current_image = ctk.CTkImage(Image.fromarray(cv_image), size=(width, self.img_max_height))

        height_in_inches = cv_image.shape[0] / 30
        height_in_meters = height_in_inches * 0.0254
        self.motoman_canvas.conversion_factor = 400 / height_in_meters
        self.motoman_canvas.width = width
        '''
        cv_image = self.bridge.imgmsg_to_cv2(msg, "rgb8")
        width = int(cv_image.shape[1] * self.img_max_height / cv_image.shape[0])
        
        self.most_recent_imgs[vision_system] = ctk.CTkImage(Image.fromarray(cv_image), size=(width, self.img_max_height))
        if vision_system == self.vision_selection.get():
            self.live_image_label.current_image = self.live_image_label[vision_system]

        height_in_inches = cv_image.shape[0] / 30
        height_in_meters = height_in_inches * 0.0254
        self.visualization_canvases[vision_system].conversion_factor = 400 / height_in_meters
        self.visualization_canvases[vision_system].width = width'''

    def motoman_trays_cb_(self, msg: Trays):
        all_trays = msg.kit_trays + msg.part_trays
        self.motoman_canvas.all_trays = all_trays


def main():
    print('Hi from aprs_application.')


if __name__ == '__main__':
    main()
