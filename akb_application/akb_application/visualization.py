from typing import Optional

import customtkinter as ctk
from tkinter.ttk import Separator
from tkinter import NORMAL, DISABLED

from rclpy.node import Node
from rclpy.client import Client
from rclpy.task import Future

from aprs_interfaces.srv import LocateTrays
from example_interfaces.srv import Trigger

from akb_application.live_image import LiveImage
from akb_application.tray_canvas import TrayCanvas
from akb_application.settings import *

class VisualizationFrame(ctk.CTkFrame):
    def __init__(self, frame, node: Node, row: int, col: int, select_mode_on: ctk.BooleanVar, selected_canvas_slot: ctk.StringVar, areas_updated: dict[str, ctk.BooleanVar]):
        super().__init__(frame, fg_color="transparent")

        self.node = node

        self.select_mode_on = select_mode_on
        self.selected_canvas_slot = selected_canvas_slot

        self.areas_updated: dict[str, ctk.BooleanVar] = areas_updated

        # Variables                
        self.dropdown_selection = ctk.StringVar(value=DETECTION_AREAS[0])

        # Widgets
        image_title = ctk.CTkLabel(self, text="Live Image", font=ctk.CTkFont(FONT_FAMILY, TITLE_FONT_SIZE, weight=TITLE_FONT_WEIGHT))
        canvas_title = ctk.CTkLabel(self, text="Visualization", font=ctk.CTkFont(FONT_FAMILY, TITLE_FONT_SIZE, weight=TITLE_FONT_WEIGHT))

        drop_down = ctk.CTkOptionMenu(
            self,
            values=DETECTION_AREAS,
            variable=self.dropdown_selection,
            font=ctk.CTkFont(FONT_FAMILY, DROP_DOWN_FONT_SIZE, weight=DROP_DOWN_FONT_WEIGHT),
            dropdown_font=ctk.CTkFont(FONT_FAMILY, DROP_DOWN_FONT_SIZE, weight=DROP_DOWN_FONT_WEIGHT),
            width=300,
            command=self.dropdown_cb
        )

        self.live_images = {key: LiveImage(self, key, height=500) for key in DETECTION_AREAS}
        self.canvases = {key: TrayCanvas(self, node, TRAYS_INFO_NAMES[key], *self.live_images[key].get_shape(), self.live_images[key].get_image_height(), self.select_mode_on, self.selected_canvas_slot) for key in DETECTION_AREAS}

        update_selected_button = UpdateSelectedTraysButton(self, node, self.dropdown_selection, self.areas_updated)
        update_all_button = UpdateAllTraysButton(self, node, self.areas_updated)

        # Layout
        self.grid(row=row, column=col, sticky="N")
        self.grid_columnconfigure((0,1,2,3), weight=1)

        image_title.grid(row=0, column=0, columnspan=2, pady=5)
        Separator(self, orient='horizontal').grid(row=1, column=0, columnspan=2, padx=20, sticky="WE")
        
        canvas_title.grid(row=0, column=2, columnspan=2, pady=5)
        Separator(self, orient='horizontal').grid(row=1, column=2, columnspan=2, padx=20, sticky="WE")
        
        drop_down.grid(row=3, column=0, columnspan=2, padx=20, pady=10)
        update_selected_button.grid(row=3, column=2, padx=20, pady=10)
        update_all_button.grid(row=3, column=3, padx=20, pady=10)

        self.live_images[self.dropdown_selection.get()].continue_updating.set(True)
        self.live_images[self.dropdown_selection.get()].grid(row=2, column=0, columnspan=2, padx=20, pady=10)
        self.canvases[self.dropdown_selection.get()].grid(row=2, column=2, columnspan=2, padx=20, pady=10)
        self.current_area = self.dropdown_selection.get()
            
    def dropdown_cb(self, *args):
        for live_image in self.live_images.values():
            live_image.continue_updating.set(False)

        self.live_images[self.current_area].grid_remove()
        self.canvases[self.current_area].grid_remove()
       
        self.live_images[self.dropdown_selection.get()].continue_updating.set(True)
        self.live_images[self.dropdown_selection.get()].grid(row=2, column=0, columnspan=2, padx=20, pady=20)
        self.canvases[self.dropdown_selection.get()].grid(row=2, column=2, columnspan=2, padx=20, pady=20)
        self.current_area = self.dropdown_selection.get()

class UpdateSelectedTraysButton(ctk.CTkButton):
    def __init__(self, frame, node: Node, selected_area: ctk.StringVar, areas_updated: dict[str, ctk.BooleanVar]):
        super().__init__(
            frame,
            text="Update",
            font=ctk.CTkFont(FONT_FAMILY, BUTTON_FONT_SIZE, weight=BUTTON_FONT_WEIGHT),
            fg_color=BLUE,
            hover_color=DARK_BLUE,
            command=self.button_cb
        )
        
        self.node = node
        self.selected_area = selected_area
        self.locate_trays_client: Optional[Client] = None
        self.areas_updated = areas_updated
        
        self.count = 0
        
        self.planning_scene_clients = {
            robot: self.node.create_client(Trigger, srv) for robot, srv in PLANNING_SCENE_NAMES.items()
        }
        
        self.selected_area.trace_add('write', self.dropdown_changed)
    
    def dropdown_changed(self, *args):
        srv_name = LOCATE_TRAYS_NAMES[self.selected_area.get()]
        
        self.locate_trays_client = self.node.create_client(LocateTrays, srv_name)
        self.configure(fg_color=BLUE, hover_color=DARK_BLUE, state=NORMAL)
    
    def button_cb(self, *args):
        # Locate Trays
        srv_name = LOCATE_TRAYS_NAMES[self.selected_area.get()]
        
        self.count = 0
        
        if self.locate_trays_client is None:
            self.locate_trays_client = self.node.create_client(LocateTrays, srv_name)
        if self.locate_trays_client.srv_name != srv_name:
            self.locate_trays_client = self.node.create_client(LocateTrays, srv_name)
            
        if not self.locate_trays_client.service_is_ready():
            self.node.get_logger().error(f"Locate trays service not ready for {self.selected_area.get()}")
            self.configure(fg_color=RED, hover_color=DARK_RED)
            return
        
        request = LocateTrays.Request()
        
        future = self.locate_trays_client.call_async(request)
        
        self.configure(fg_color=YELLOW, state=DISABLED)
        
        future.add_done_callback(self.locate_trays_done_cb)
        
    def locate_trays_done_cb(self, future: Future):
        response: LocateTrays.Response = future.result() # type: ignore
        
        if response.success:
            if self.selected_area.get() == 'teach_table':
                self.configure(fg_color=GREEN, hover_color=DARK_GREEN, state=NORMAL)
                self.areas_updated[self.selected_area.get()].set(True)
            else:
                self.call_initialize_planning_scene()
                
        else:
            self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.node.get_logger().warn(f"{response.message}")
    
    def call_initialize_planning_scene(self):
        for robot, client in self.planning_scene_clients.items():
            if not client.service_is_ready():
                self.node.get_logger().error(f"Initialize planning scene not ready for {robot}")
                self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
                return
        
            request = Trigger.Request()
            
            future = client.call_async(request)
            
            future.add_done_callback(self.initalize_planning_scene_done_cb)
    
    def initalize_planning_scene_done_cb(self, future: Future):
        response: LocateTrays.Response = future.result() # type: ignore

        if response.success:
            self.areas_updated[self.selected_area.get()].set(True)
            self.count += 1
            if self.count == 2:
                self.configure(fg_color=GREEN, hover_color=DARK_GREEN, state=NORMAL)
        else:
            self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.node.get_logger().warn(f"{response.message}")
        

class UpdateAllTraysButton(ctk.CTkButton):
    def __init__(self, frame, node: Node, areas_updated: dict[str, ctk.BooleanVar]):
        super().__init__(
            frame,
            text="Update All",
            font=ctk.CTkFont(FONT_FAMILY, BUTTON_FONT_SIZE, weight=BUTTON_FONT_WEIGHT),
            fg_color=BLUE,
            hover_color=DARK_BLUE,
            command=self.button_cb
        )
        
        self.node = node

        self.areas_updated = areas_updated

        self.clients = {k: self.node.create_client(LocateTrays, v) for k,v in LOCATE_TRAYS_NAMES.items()}
        
        self.planning_scene_clients = {
            robot: self.node.create_client(Trigger, srv) for robot, srv in PLANNING_SCENE_NAMES.items()
        }

        self.locate_count = 0
        self.planning_scene_count = 0
    
    def button_cb(self, *args):
        self.configure(fg_color=YELLOW, state=DISABLED)
        self.locate_count = 0
        
        for area, client in self.clients.items():
            
            if not client.service_is_ready():
                self.node.get_logger().error(f"Locate trays service not ready for {area}")
                self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
                return
        
            request = LocateTrays.Request()
            
            future = client.call_async(request)
            
            future.add_done_callback(self.locate_trays_done_cb)
        
    def locate_trays_done_cb(self, future: Future):
        response: LocateTrays.Response = future.result() # type: ignore
        
        if response.success:
            self.locate_count += 1
        else:
            self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.node.get_logger().warn(f"{response.message}")
        
        if self.locate_count == len(LOCATE_TRAYS_NAMES):
            self.areas_updated["teach_table"].set(True)
            self.planning_scene_count = 0
            self.after(3000, self.call_initialize_planning_scene)

    def call_initialize_planning_scene(self):
        for robot, client in self.planning_scene_clients.items():
            if not client.service_is_ready():
                self.node.get_logger().error(f"Initialize planning scene not ready for {robot}")
                self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
                return
        
            request = Trigger.Request()
            
            future = client.call_async(request)
            
            future.add_done_callback(self.initalize_planning_scene_done_cb)
    
    def initalize_planning_scene_done_cb(self, future: Future):
        response: LocateTrays.Response = future.result() # type: ignore
        
        if response.success:
            self.planning_scene_count += 1
            if self.planning_scene_count == 2:
                self.configure(fg_color=GREEN, hover_color=DARK_GREEN, state=NORMAL)
                for area in DETECTION_AREAS:
                    if "teach" not in area:
                        self.areas_updated[area].set(True)
        else:
            self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.node.get_logger().warn(f"{response.message}")