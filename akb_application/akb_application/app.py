import customtkinter as ctk

from akb_application.live_image import LiveImage
from akb_application.tray_canvas import TrayCanvas

class App(ctk.CTk):
    def __init__(self):
        super().__init__()

        self.geometry("1400x1000")

        self.grid_columnconfigure(0, weight=1)

        self.detection_area = ctk.StringVar()
        drop_down = ctk.CTkOptionMenu(self, variable=self.detection_area, values=['fanuc_table', 'fanuc_conveyor'])

        drop_down.grid(row=0, column=0, padx=20, pady=20, sticky="ew")

        self.detection_area.trace_add("write", self.select_detection_area)

        self.image_frame = ctk.CTkFrame(self, width=400, height=800, fg_color="#EBEBEB")

        self.fanuc_table_image = LiveImage(self.image_frame, "fanuc_table", height=400)
        w, h = self.fanuc_table_image.get_shape()
        img_h = self.fanuc_table_image.get_image_height()
        
        self.fanuc_trays = TrayCanvas(self.image_frame, w, h, img_h)

        self.image_frame.grid(row=1, column=0, padx=20, pady=20, sticky="ew")

        # self.fanuc_table_image.grid(row=0, column=0, padx=20, pady=20, sticky="ew")
        # self.fanuc_trays.grid(row=1, column=0, padx=20, pady=20, sticky="ew", columnspan=1)

        # self.fanuc_conveyor_image = LiveImage(self, "fanuc_conveyor")
        # self.fanuc_conveyor_image.pack(pady=20, padx=20)

        # self.motoman_table_image = LiveImage(self, "motoman_table")
        # self.motoman_table_image.pack(pady=20, padx=20)

        # self.motoman_conveyor_image = LiveImage(self, "motoman_conveyor")
        # self.motoman_conveyor_image.pack(pady=20, padx=20)

        # self.teach_table_image = LiveImage(self, "teach_table")
        # self.teach_table_image.pack(pady=20, padx=20)
    
    def select_detection_area(self, _, __, ___):
        self.image_frame.grid_columnconfigure((0, 1), weight=1)

        for _, widget in self.image_frame.children.items():
            widget.grid_forget()

        if self.detection_area.get() == "fanuc_table":
            self.fanuc_table_image.grid(row=0, column=0, padx=20, pady=20, sticky="ew")
            self.fanuc_trays.grid(row=0, column=1, padx=20, pady=20, sticky="ew")