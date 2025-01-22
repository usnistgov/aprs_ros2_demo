import customtkinter as ctk

from rclpy.node import Node


from akb_application.control_panel import ControlPanel
from akb_application.status_panel import StatusPanel
from akb_application.visualization import VisualizationFrame
from akb_application.pddl_frame import PDDLFrame

class App(ctk.CTk):
    def __init__(self, node: Node):
        super().__init__()

        ctk.set_appearance_mode("light")
        
        self.title("Automated Kit Building Demo")

        self.select_mode_on = ctk.BooleanVar(value=False)
        self.selected_canvas_slot = ctk.StringVar(value="")

        # Widgets
        VisualizationFrame(self, node, row=0, col=0, select_mode_on=self.select_mode_on, selected_canvas_slot=self.selected_canvas_slot)
        PDDLFrame(self, node,row=0, col=1)
        ControlPanel(self, node, row=1, col=0, select_mode_on=self.select_mode_on, selected_canvas_slot=self.selected_canvas_slot)
        StatusPanel(self, node, row=1, col=1)
        