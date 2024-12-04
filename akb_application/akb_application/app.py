import customtkinter as ctk
import tkinter as tk

from rclpy.node import Node

from akb_application.live_image import LiveImage
from akb_application.tray_canvas import TrayCanvas
from akb_application.service_handlers import LocateTraysFrame, InitializePlanningSceneFrame
from akb_application.pddl_frame import PDDLFrame

class App(ctk.CTk):
    def __init__(self, node: Node):
        super().__init__()

        ctk.set_appearance_mode("light")

        self.node = node
        self.geometry("2000x1100")

        self.grid_columnconfigure((0,1,2), weight=1)
        self.grid_rowconfigure((0,1), weight=1)

        self.font = ("", 18)

        self.visualization_frame = ctk.CTkFrame(self, height=800, width=1300, fg_color="#EBEBEB")
        self.visualization_frame.grid_columnconfigure((0,1,2), weight=1)
        self.visualization_frame.grid_rowconfigure([i for i in range(4)], weight=1)

        ctk.CTkLabel(self.visualization_frame, text="Fanuc Table Image", font=self.font, fg_color="#EBEBEB", anchor="s").grid(column=0, row=0, pady=2)
        self.fanuc_table_image = LiveImage(self.visualization_frame, "fanuc_table", height=400)
        self.fanuc_table_image.grid(column=0, row=1)
        ctk.CTkLabel(self.visualization_frame, text="Fanuc Table Tray Visualization", font=self.font, fg_color="#EBEBEB", anchor="s").grid(column=0, row=2, pady=2)
        w, h = self.fanuc_table_image.get_shape()
        img_h = self.fanuc_table_image.get_image_height()
        self.fanuc_trays = TrayCanvas(self.visualization_frame, self.node, "/fanuc/table_trays_info", w, h, img_h)
        self.fanuc_trays.grid(column=0, row=3, padx=5)

        ctk.CTkLabel(self.visualization_frame, text="Motoman Table Image", font=self.font, fg_color="#EBEBEB", anchor="s").grid(column=1, row=0, pady=2)
        self.motoman_table_image = LiveImage(self.visualization_frame, "motoman_table", height=400)
        self.motoman_table_image.grid(column=1, row=1)
        ctk.CTkLabel(self.visualization_frame, text="Motoman Table Tray Visualization", font=self.font, fg_color="#EBEBEB", anchor="s").grid(column=1, row=2, pady=2)
        w, h = self.motoman_table_image.get_shape()
        img_h = self.motoman_table_image.get_image_height()
        self.motoman_trays = TrayCanvas(self.visualization_frame, self.node, "/motoman/table_trays_info", w, h, img_h)
        self.motoman_trays.grid(column=1, row=3, padx=5)

        ctk.CTkLabel(self.visualization_frame, text="Teach Table Image", font=self.font, fg_color="#EBEBEB", anchor="s").grid(column=2, row=0, pady=2)
        self.teach_table_image = LiveImage(self.visualization_frame, "teach_table", height=400)
        self.teach_table_image.grid(column=2, row=1)
        ctk.CTkLabel(self.visualization_frame, text="Teach Table Tray Visualization", font=self.font, fg_color="#EBEBEB", anchor="s").grid(column=2, row=2, pady=2)
        w, h = self.teach_table_image.get_shape()
        img_h = self.teach_table_image.get_image_height()
        self.teach_trays = TrayCanvas(self.visualization_frame, self.node, "/teach/table_trays_info", w, h, img_h)
        self.teach_trays.grid(column=2, row=3, padx=5)
        

        self.locate_trays_frame = LocateTraysFrame(self, self.node)
        self.planning_scene_frame = InitializePlanningSceneFrame(self, self.node)
        self.pddl_frame = PDDLFrame(self, self.node)
    
        self.visualization_frame.grid(row=0, column=0, columnspan=2)
        self.locate_trays_frame.grid(row=1, column=0)
        self.planning_scene_frame.grid(row=1, column=1)
        self.pddl_frame.grid(row=0, column=2, rowspan=2)

        # self.fanuc_table_image = LiveImage(self, "fanuc_table", height=400)

        # self.fanuc_table_image.pack(pady=20, padx=20)

        # w, h = self.fanuc_table_image.get_shape()
        # img_h = self.fanuc_table_image.get_image_height()

        # self.fanuc_trays = TrayCanvas(self, self.node, "/fanuc/table_trays_info", w, h, img_h)

        # self.fanuc_trays.pack(pady=20, padx=20)

        # LocateTraysFrame(self, node).pack(pady=20, padx=20)
        # InitializePlanningSceneFrame(self, node).pack(pady=35)

        # PDDLFrame(self, self.node).pack(pady=20, padx=20)
