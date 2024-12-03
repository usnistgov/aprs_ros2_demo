import customtkinter as ctk

from akb_application.live_image import LiveImage

class App(ctk.CTk):
    def __init__(self):
        super().__init__()

        self.geometry("400x1000")

        self.fanuc_table_image = LiveImage(self, "fanuc_table")
        self.fanuc_table_image.pack(pady=20, padx=20)

        self.fanuc_conveyor_image = LiveImage(self, "fanuc_conveyor")
        self.fanuc_conveyor_image.pack(pady=20, padx=20)

        self.motoman_table_image = LiveImage(self, "motoman_table")
        self.motoman_table_image.pack(pady=20, padx=20)

        self.motoman_conveyor_image = LiveImage(self, "motoman_conveyor")
        self.motoman_conveyor_image.pack(pady=20, padx=20)

        self.teach_table_image = LiveImage(self, "teach_table")
        self.teach_table_image.pack(pady=20, padx=20)