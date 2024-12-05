import customtkinter as ctk
import tkinter as tk
from copy import copy
from functools import partial

from rclpy.node import Node
from rclpy.task import Future
from aprs_interfaces.srv import LocateTrays, MoveToNamedPose, PneumaticGripperControl
from example_interfaces.srv import Trigger
from ament_index_python.packages import get_package_share_directory

class LocateTraysFrame(ctk.CTkFrame):
    srv_names = {
        "fanuc_table": "fanuc/locate_trays_on_table",
        # "fanuc_conveyor": "fanuc/locate_trays_on_conveyor",
        "teach_table": "teach/locate_trays_on_table",
        "motoman_table": "motoman/locate_trays_on_table",
        # "motoman_conveyor": "motoman/locate_trays_on_conveyor",
    }
    def __init__(self, frame, node: Node):
        super().__init__(frame, height=300, width=433, fg_color="#EBEBEB")
        self.grid_rowconfigure([i for i in range(3)], weight=1)
        self.grid_columnconfigure([i for i in range(2)], weight=1)

        self.font = ("", 15)
        self.disabled_color = "#404040"

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
        # for detection_area in ["All", "fanuc_table", "fanuc_conveyor", "teach_table", "motoman_table", "motoman_conveyor"]:
        for detection_area in ["All", "fanuc_table", "teach_table", "motoman_table"]:
            self.checkboxes[detection_area].grid(row=checkbox_count//2 + 1, column=checkbox_count%2, pady=5, padx=3, sticky="w")
            checkbox_count += 1

        self.locate_trays_button = ctk.CTkButton(self, text="Locate trays in selected areas", command=self.button_command_cb, font=self.font, state=tk.DISABLED, fg_color=self.disabled_color)
        self.locate_trays_button.grid(row = 0, column = 0, columnspan=2, pady=10)
        
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
        self.locate_trays_button.configure(state=tk.NORMAL, fg_color="#3B8ED0")

    def deactivate_all(self):
        for checkbox in self.checkboxes.values():
            checkbox.configure(state=tk.DISABLED)
        self.locate_trays_button.configure(state=tk.DISABLED, fg_color=self.disabled_color)

    def reactivate_button_timeout(self):
        for detection_area, future in self.service_request_futures.items():
            if future.done():
                continue
            self.checkboxes[detection_area].configure(text_color="red")
            future.cancel()
        
        self.activate_all()
    
    def check_box_trace_cb(self):
        if any([checkbox_selected.get() for checkbox_selected in self.selected_detection_areas.values()]):
            self.locate_trays_button.configure(state=tk.NORMAL, fg_color="#3B8ED0")
        else:
            self.locate_trays_button.configure(state=tk.DISABLED, fg_color=self.disabled_color)

class InitializePlanningSceneFrame(ctk.CTkFrame):
    srv_names = {
        "fanuc": "fanuc/initialize_planning_scene",
        "motoman": "motoman/initialize_planning_scene"}
    def __init__(self, frame, node: Node):
        super().__init__(frame, height=300, width=433, fg_color="#EBEBEB")
        self.rowconfigure([i for i in range(4)], weight=1)
        self.rowconfigure([i for i in range(3)], weight=1)

        self.font = ("", 15)
        self.disabled_color = "#404040"

        self.successfully_locate_trays = True
        self.service_request_futures: dict[str, Future] = {}

        self.num_services_done: int
        self.timeout_id: str

        self.node = node
        self.clients = {robot: self.node.create_client(Trigger, srv_name) for robot, srv_name in self.srv_names.items()}
        self.selected_robots = {robot: ctk.BooleanVar(value=True) for robot in list(self.srv_names.keys())}

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

        self.initialize_button = ctk.CTkButton(self, text="Initialize Planning Scene", command=self.button_command_cb, font=self.font, state=tk.NORMAL)
        self.initialize_button.grid(row = 0, column = 0, columnspan=2, pady=10)
        
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
        self.initialize_button.configure(state=tk.NORMAL, fg_color="#3B8ED0")

    def deactivate_all(self):
        for checkbox in self.checkboxes.values():
            checkbox.configure(state=tk.DISABLED)
        self.initialize_button.configure(state=tk.DISABLED, fg_color=self.disabled_color)

    def reactivate_button_timeout(self):
        for robot, future in self.service_request_futures.items():
            if future.done():
                continue
            self.checkboxes[robot].configure(text_color="red")
            future.cancel()
        
        self.activate_all()
    
    def check_box_trace_cb(self):
        if any([checkbox_selected.get() for checkbox_selected in self.selected_robots.values()]):
            self.initialize_button.configure(state=tk.NORMAL, fg_color="#3B8ED0")
        else:
            self.initialize_button.configure(state=tk.DISABLED, fg_color=self.disabled_color)

class RobotFunctionsFrame(ctk.CTkFrame):
    srv_names = {
        "fanuc": "fanuc/initialize_planning_scene",
        "motoman": "motoman/initialize_planning_scene"}
    def __init__(self, frame, node: Node):
        super().__init__(frame, height=300, width=433, fg_color="#EBEBEB")
        self.node = node
        self.font = ("", 15)

        self.actuate_gripper_clients = {
            "fanuc": self.node.create_client(PneumaticGripperControl, "fanuc/actuate_gripper"),
            "motoman": self.node.create_client(PneumaticGripperControl, "motoman/actuate_gripper")
        }

        self.move_to_named_pose_clients = {
            "fanuc": self.node.create_client(MoveToNamedPose, "fanuc/move_to_named_pose"),
            "motoman": self.node.create_client(MoveToNamedPose, "motoman/move_to_named_pose")
        }

        self.named_poses = {
            "fanuc": self.get_named_positions("fanuc"),
            "motoman": self.get_named_positions("motoman")
        }

        self.selected_robot = ctk.StringVar(value="fanuc")
        self.robot_switch = ctk.CTkSwitch(self, text="Fanuc", command=self.update_robot_switch,
                                          variable=self.selected_robot, onvalue="motoman", offvalue="fanuc", height=40, width=135, font=self.font, fg_color="#3B8ED0", progress_color="#3B8ED0")
        
        self.robot_switch.grid(column=0, row=0, columnspan = 2, padx=10, pady=10)
        
        self.open_gripper_button = ctk.CTkButton(self, text="Open Gripper", command=partial(self.actuate_gripper_button_cb, False), font=self.font)
        self.open_gripper_button.grid(column=0, row=2, padx=10, pady=10, columnspan=2)
        self.open_gripper_button = ctk.CTkButton(self, text="Close Gripper", command=partial(self.actuate_gripper_button_cb, True), font=self.font)
        self.open_gripper_button.grid(column=0, row=3, padx=10, pady=10, columnspan=2)

        self.selected_named_position = ctk.StringVar(value=self.named_poses[self.selected_robot.get()][0])
        self.named_pose_dropdown = ctk.CTkOptionMenu(
            self, 
            variable=self.selected_named_position, 
            values=self.named_poses[self.selected_robot.get()], 
            font=self.font, 
            dropdown_font=self.font,
            anchor="center"
        )
        self.named_pose_dropdown.grid(column=1, row=1, padx=10, pady=10)
        self.move_to_named_pose_button = ctk.CTkButton(self, text="Move to Pose:", command=self.move_to_named_pose_button_cb, font=self.font)
        self.move_to_named_pose_button.grid(column=0, row=1, padx=10, pady=10)
    
    def actuate_gripper_button_cb(self, enable: bool):
        if not self.actuate_gripper_clients[self.selected_robot.get()].service_is_ready():
            self.node.get_logger().error(f"Acuate gripper service not ready for {self.selected_robot.get()}")
            return

        gripper_request = PneumaticGripperControl.Request()
        gripper_request.enable = enable

        future = self.actuate_gripper_clients[self.selected_robot.get()].call_async(gripper_request)

        future.add_done_callback(self.gripper_done_cb)
    
    def gripper_done_cb(self, future: Future):
        result: PneumaticGripperControl.Response = future.result() # type: ignore
        if result.success:
            self.node.get_logger().info(f"Acuated gripper for {self.selected_robot.get()}")
        else:
            self.node.get_logger().error(f"Unable to actuate gripper for {self.selected_robot.get()}")
    
    def move_to_named_pose_button_cb(self):
        if not self.move_to_named_pose_clients[self.selected_robot.get()].service_is_ready():
            self.node.get_logger().error(f"Move to named pose service not ready for {self.selected_robot.get()}")
            return

        move_to_named_pose_request = MoveToNamedPose.Request()
        move_to_named_pose_request.name = self.selected_named_position.get()

        self.move_to_named_pose_button.configure(state=tk.DISABLED)

        future = self.move_to_named_pose_clients[self.selected_robot.get()].call_async(move_to_named_pose_request)

        future.add_done_callback(self.move_to_named_pose_done_cb)
    
    def move_to_named_pose_done_cb(self, future: Future):
        result: MoveToNamedPose.Response = future.result() # type: ignore
        if result.success:
            self.node.get_logger().info(f"Moved to pose {self.selected_named_position.get()} for {self.selected_robot.get()}")
        else:
            self.node.get_logger().error(f"Could not move to pose {self.selected_named_position.get()} for {self.selected_robot.get()}")
        self.move_to_named_pose_button.configure(state=tk.NORMAL)

    def update_robot_switch(self):
        self.robot_switch.configure(text=self.selected_robot.get().capitalize())
        self.selected_named_position.set(self.named_poses[self.selected_robot.get()][0])
        self.named_pose_dropdown.configure(values=self.named_poses[self.selected_robot.get()])

    def get_named_positions(self, robot_name: str):
        moveit_package = get_package_share_directory(f'{robot_name}_moveit_config')
        srdf_file_path = moveit_package + f"/config/{robot_name}.srdf"
        found_named_positions = []
        with open(srdf_file_path, "+r") as f:
            for line in f:
                if "group_state" in line and " name" in line:
                    found_name = ""
                    inside_quotes = False
                    for c in line[line.find(" name"):]:
                        if c == '"':
                            if inside_quotes == False:
                                inside_quotes = True
                                continue
                            else:
                                break
                        if inside_quotes:
                            found_name += c
                    found_named_positions.append(found_name)
        return found_named_positions