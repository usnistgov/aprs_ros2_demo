import os
from typing import Optional

from PIL import Image

import cv2

import customtkinter as ctk

from ament_index_python.packages import get_package_share_directory

from aprs_vision.stream_handler import StreamHandler, StreamException

class LiveImage(ctk.CTkLabel):
    streams = {
        "fanuc_table": "http://192.168.1.104/mjpg/video.mjpg",
        "fanuc_conveyor": "http://192.168.1.108/mjpg/video.mjpg",
        "motoman_table": "http://192.168.1.110/mjpg/video.mjpg",
        "motoman_conveyor": "http://192.168.1.107/mjpg/video.mjpg",
        "teach_table": "http://192.168.1.105/mjpg/video.mjpg",
    }

    def __init__(self, frame, detection_area: str, height=200):
        super().__init__(frame)

        self.height = height 
        self.width = height
        self.detection_area = detection_area
        
        self.font = ctk.CTkFont("Roboto", 26, weight="bold")

        # Get calibration file
        share_path = get_package_share_directory('aprs_vision')
        calibration_filepath = os.path.join(share_path, 'config', f'{detection_area}_calibration.npz')

        # Connect to stream and set width
        try:
            self.stream = StreamHandler(self.streams[detection_area], calibration_filepath)
            shape = self.stream.read_frame().shape
            self.width = int(shape[1] * self.height / shape[0])
            self.image_height = shape[0]

        except StreamException as e:
            self.stream = None
            self.image_height = 100
            self.configure(width=self.width, height=self.height)
            print(f'Issue with {self.detection_area} camera: {e}')
        
        self.update_image()

    def get_image(self):
        if self.stream is None:
            return None

        try:
            cv_image = self.stream.read_frame()
            cv_image = cv2.cvtColor(cv_image, cv2.COLOR_BGR2RGB)

            return cv_image
            
        except StreamException as e:
            print(f'Issue with {self.detection_area} camera: {e}')
            return None

    def update_image(self):
        cv_image = self.get_image()
        
        if cv_image is None:
            area = self.detection_area.title().replace("_", " ")
            self.configure(text=f"{area}\nimage not found", fg_color="#C2C2C2", font=self.font)
        else:
            self.configure(text="", image=ctk.CTkImage(Image.fromarray(cv_image), size=(self.width, self.height)), fg_color="transparent")
            self.after(50, self.update_image)

    def get_shape(self) -> tuple[int, int]:
        return (self.width, self.height)
    
    def get_image_height(self) -> int:
        return self.image_height