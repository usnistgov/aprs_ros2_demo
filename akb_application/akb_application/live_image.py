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

    def __init__(self, frame: ctk.CTk, detection_area: str):
        super().__init__(frame, text="Image not found", fg_color="#C2C2C2", font=("UbuntuMono",50))
        
        self.height = 100 
        self.width = 100

        self.configure(width=self.width, height=self.height)
        
        share_path = get_package_share_directory('aprs_vision')
        calibration_filepath = os.path.join(share_path, 'config', f'{detection_area}_calibration.npz')

        try:
            self.stream = StreamHandler(self.streams[detection_area], calibration_filepath)
        except StreamException:
            self.stream = None
        
        self.update_image()

    def get_image(self):
        if self.stream is None:
            return None

        try:
            cv_image = self.stream.read_frame()
            cv_image = cv2.cvtColor(cv_image, cv2.COLOR_BGR2RGB)

            self.width = int(cv_image.shape[1] * self.height / cv_image.shape[0])

            return cv_image
            
        except StreamException:
            return None

    def update_image(self):
        cv_image = self.get_image()
        
        if cv_image is None:
            self.configure(text="Image not found", fg_color="#C2C2C2")
        else:
            self.configure(text="", image=ctk.CTkImage(Image.fromarray(cv_image), size=(self.width, self.height)), fg_color="transparent")
        
        self.after(50, self.update_image)