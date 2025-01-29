import customtkinter as ctk

from rclpy.node import Node

from akb_application.settings import DETECTION_AREAS

from akb_application.control_panel import ControlPanel
from akb_application.status_panel import StatusPanel
from akb_application.visualization import VisualizationFrame
from akb_application.pddl_frame import PDDLFrame

class App(ctk.CTk):
    def __init__(self, node: Node):
        super().__init__()

        self.grid_columnconfigure((0,1),weight=1)
        self.grid_rowconfigure((0,1),weight=1)

        ctk.set_appearance_mode("light")

        self.geometry("2022x894")

        self.resizable(False, False)

        self.node = node
        self.title("Automated Kit Building Demo")

        areas_updated = {area: ctk.BooleanVar(value=False) for area in DETECTION_AREAS}

        self.select_mode_on = ctk.BooleanVar(value=False)
        self.selected_canvas_slot = ctk.StringVar(value="")

        # Widgets
        VisualizationFrame(self, node, row=0, col=0, select_mode_on=self.select_mode_on, selected_canvas_slot=self.selected_canvas_slot, areas_updated=areas_updated)
        PDDLFrame(self, node,row=0, col=1, areas_updated=areas_updated)
        ControlPanel(self, node, row=1, col=0, select_mode_on=self.select_mode_on, selected_canvas_slot=self.selected_canvas_slot)
        StatusPanel(self, node, row=1, col=1)
        