import customtkinter as ctk
import tkinter as tk
from copy import copy
from functools import partial

from rclpy.node import Node
from rclpy.task import Future
from aprs_interfaces.srv import LocateTrays
from example_interfaces.srv import Trigger

class LocateTraysFrame(ctk.CTkFrame):
    srv_names = {
        "fanuc_table": "fanuc/locate_trays_on_table",
        "fanuc_conveyor": "fanuc/locate_trays_on_conveyor",
        "teach_table": "teach/locate_trays_on_table",
        "motoman_table": "motoman/locate_trays_on_table",
        "motoman_conveyor": "motoman/locate_trays_on_conveyor",
    }
    def __init__(self, frame, node: Node):
        super().__init__(frame, height=800, width=600)
        self.rowconfigure([i for i in range(4)], weight=1)
        self.rowconfigure([i for i in range(3)], weight=1)

        self.font = ("", 15)

        self.successfully_locate_trays = True
        self.service_request_futures: dict[str, Future] = {}

        self.num_services_done: int
        self.timeout_id: str

        self.node = node
        self.clients = {detection_area: self.node.create_client(LocateTrays, srv_name) for detection_area, srv_name in self.srv_names.items()}
        self.selected_detection_areas = {detection_area: ctk.BooleanVar(value=False) for detection_area in list(self.srv_names.keys()) + ["All"]}

        self.checkboxes: dict[str, ctk.CTkCheckBox] = {}
        checkbox_names = list(self.srv_names.keys()) + ["All"]
        for detection_area in checkbox_names:
            cb_text = detection_area.replace("_", " ").capitalize()
            self.checkboxes[detection_area] = ctk.CTkCheckBox(
                self,
                text=cb_text, 
                variable=self.selected_detection_areas[detection_area], 
                onvalue=True, 
                offvalue=False, 
                height=1, 
                width=20, 
                font=self.font, 
                command=self.check_box_trace_cb
            )
        
        checkbox_count = 0
        for detection_area in ["All", "fanuc_table", "fanuc_conveyor", "teach_table", "motoman_table", "motoman_conveyor"]:
            self.checkboxes[detection_area].grid(row=checkbox_count//3 + 1, column=checkbox_count%3, pady=5, padx=3, sticky="w")
            checkbox_count += 1

        self.locate_trays_button = ctk.CTkButton(self, text="Locate trays in selected areas", command=self.button_command_cb, font=self.font, state=tk.DISABLED)
        self.locate_trays_button.grid(row = 0, column = 1, pady=10)
        
        self.selected_detection_areas["All"].trace_add("write", self.select_all_detection_areas)

    def select_all_detection_areas(self, _, __, ___):
        if self.selected_detection_areas["All"].get():
            for checkbox_selected in self.selected_detection_areas.values():
                checkbox_selected.set(True)
            for detection_area, checkbox in self.checkboxes.items():
                if detection_area != "All":
                    checkbox.configure(state=tk.DISABLED)
        else:
            for checkbox in self.checkboxes.values():
                checkbox.configure(state=tk.NORMAL)

    def button_command_cb(self):
        self.num_services_done = 0

        self.service_request_futures.clear()

        self.deactivate_all()

        for detection_area, checkbox_selected in self.selected_detection_areas.items():
            if detection_area == "All":
                continue

            if checkbox_selected.get():
                if not self.clients[detection_area].service_is_ready():
                    self.node.get_logger().error(f"Service not ready for locate_trays {detection_area}")
                    self.checkboxes[detection_area].configure(text_color="red")
                    continue
                
                request = LocateTrays.Request()
                
                self.checkboxes[detection_area].configure(text_color="orange")

                future = self.clients[detection_area].call_async(request)

                future.add_done_callback(partial(self.service_done_cb, detection_area))

                self.service_request_futures[detection_area] = future
        
        if len(self.service_request_futures) == 0:
            self.activate_all()
        else:
            self.timeout_id = self.after(5000, self.reactivate_button_timeout)

    def service_done_cb(self, detection_area: str, future: Future):
        if future.cancelled():
            return
        response: LocateTrays.Response = future.result() # type: ignore
        if response.success:
            self.checkboxes[detection_area].configure(text_color="green")
            self.node.get_logger().info(f"Locate trays successful for detection area: {detection_area}. Response message: {response.message}")
        else:
            self.checkboxes[detection_area].configure(text_color="red")
            self.node.get_logger().warn(f"Locate trays unsuccessful for detection area: {detection_area}. Response message: {response.message}")
        self.num_services_done += 1

        if self.num_services_done == len(self.service_request_futures):
            self.after_cancel(self.timeout_id)
            self.activate_all()

    def activate_all(self):
        for checkbox in self.checkboxes.values():
            checkbox.configure(state=tk.NORMAL)
        self.locate_trays_button.configure(state=tk.NORMAL)

    def deactivate_all(self):
        for checkbox in self.checkboxes.values():
            checkbox.configure(state=tk.DISABLED)
        self.locate_trays_button.configure(state=tk.DISABLED)

    def reactivate_button_timeout(self):
        for detection_area, future in self.service_request_futures.items():
            if future.done():
                continue
            self.checkboxes[detection_area].configure(text_color="red")
            future.cancel()
        
        self.activate_all()
    
    def check_box_trace_cb(self):
        all_bools = [checkbox_selected.get() for checkbox_selected in self.selected_detection_areas.values()]
        self.locate_trays_button.configure(state = (tk.NORMAL if any(all_bools) else tk.DISABLED))

class InitializePlanningSceneFrame(ctk.CTkFrame):
    srv_names = {
        "fanuc": "fanuc/initialize_planning_scene",
        "motoman": "motoman/initialize_planning_scene"}
    def __init__(self, frame, node: Node):
        super().__init__(frame, height=800, width=600)
        self.rowconfigure([i for i in range(4)], weight=1)
        self.rowconfigure([i for i in range(3)], weight=1)

        self.font = ("", 15)

        self.successfully_locate_trays = True
        self.service_request_futures: dict[str, Future] = {}

        self.num_services_done: int
        self.timeout_id: str

        self.node = node
        self.clients = {robot: self.node.create_client(Trigger, srv_name) for robot, srv_name in self.srv_names.items()}
        self.selected_robots = {robot: ctk.BooleanVar(value=False) for robot in list(self.srv_names.keys())}

        self.checkboxes: dict[str, ctk.CTkCheckBox] = {}
        checkbox_names = list(self.srv_names.keys())
        for robot in checkbox_names:
            cb_text = robot.capitalize()
            self.checkboxes[robot] = ctk.CTkCheckBox(
                self,
                text=cb_text, 
                variable=self.selected_robots[robot], 
                onvalue=True, 
                offvalue=False, 
                height=1, 
                width=20, 
                font=self.font, 
                command=self.check_box_trace_cb
            )
        
        self.checkboxes["fanuc"].grid(row=1, column=0, pady=5, padx=3, sticky="w")
        self.checkboxes["motoman"].grid(row=1, column=1, pady=5, padx=3, sticky="w")

        self.locate_trays_button = ctk.CTkButton(self, text="Initialize Planning Scene", command=self.button_command_cb, font=self.font, state=tk.DISABLED)
        self.locate_trays_button.grid(row = 0, column = 0, columnspan=2, pady=10)
        
    def button_command_cb(self):
        self.num_services_done = 0

        self.service_request_futures.clear()

        self.deactivate_all()

        for robot, checkbox_selected in self.selected_robots.items():
            if checkbox_selected.get():
                if not self.clients[robot].service_is_ready():
                    self.node.get_logger().error(f"Service not ready for {robot}")
                    self.checkboxes[robot].configure(text_color="red")
                    continue
                
                request = Trigger.Request()
                
                self.checkboxes[robot].configure(text_color="orange")

                future = self.clients[robot].call_async(request)

                future.add_done_callback(partial(self.service_done_cb, robot))

                self.service_request_futures[robot] = future
        
        if len(self.service_request_futures) == 0:
            self.activate_all()
        else:
            self.timeout_id = self.after(5000, self.reactivate_button_timeout)

    def service_done_cb(self, robot: str, future: Future):
        if future.cancelled():
            return
        response: Trigger.Response = future.result() # type: ignore
        if response.success:
            self.checkboxes[robot].configure(text_color="green")
            self.node.get_logger().info(response.message)
        else:
            self.checkboxes[robot].configure(text_color="red")
            self.node.get_logger().warn(response.message)
        self.num_services_done += 1

        if self.num_services_done == len(self.service_request_futures):
            self.after_cancel(self.timeout_id)
            self.activate_all()

    def activate_all(self):
        for checkbox in self.checkboxes.values():
            checkbox.configure(state=tk.NORMAL)
        self.locate_trays_button.configure(state=tk.NORMAL)

    def deactivate_all(self):
        for checkbox in self.checkboxes.values():
            checkbox.configure(state=tk.DISABLED)
        self.locate_trays_button.configure(state=tk.DISABLED)

    def reactivate_button_timeout(self):
        for robot, future in self.service_request_futures.items():
            if future.done():
                continue
            self.checkboxes[robot].configure(text_color="red")
            future.cancel()
        
        self.activate_all()
    
    def check_box_trace_cb(self):
        all_bools = [checkbox_selected.get() for checkbox_selected in self.selected_robots.values()]
        self.locate_trays_button.configure(state = (tk.NORMAL if any(all_bools) else tk.DISABLED))
