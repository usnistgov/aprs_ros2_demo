import customtkinter as ctk
import tkinter as tk

from rclpy.node import Node

from akb_application.live_image import LiveImage
from akb_application.tray_canvas import TrayCanvas

class App(ctk.CTk):
    def __init__(self, node: Node):
        super().__init__()

        self.node = node

        self.geometry("1400x1000")

        self.fanuc_table_image = LiveImage(self, "fanuc_table", height=400)

        self.fanuc_table_image.pack(pady=20, padx=20)

        w, h = self.fanuc_table_image.get_shape()
        img_h = self.fanuc_table_image.get_image_height()

        self.fanuc_trays = TrayCanvas(self, self.node, "/fanuc/table_trays_info", w, h, img_h)

        self.fanuc_trays.pack(pady=20, padx=20)