from typing import Optional
from functools import partial

import customtkinter as ctk
from tkinter.ttk import Separator

from rclpy.node import Node
from rclpy.time import Time, Duration
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup
from rclpy.task import Future

from controller_manager_msgs.srv import ListControllers
from controller_manager_msgs.msg import ControllerState

from std_msgs.msg import Bool

from akb_application.settings import *


class StatusPanel(ctk.CTkFrame):
    def __init__(self, frame, node: Node, row: int, col: int):
        super().__init__(frame, fg_color="transparent")

        self.node = node
        
        # Widgets
        title = ctk.CTkLabel(self, text="Status Panel", font=ctk.CTkFont(FONT_FAMILY, TITLE_FONT_SIZE, weight=TITLE_FONT_WEIGHT))

        horizontal_sep = Separator(self, orient='horizontal')
        vertical_sep = Separator(self, orient='vertical')

        RobotStatusFrame(self, self.node, "fanuc", row=2, col=0)
        RobotStatusFrame(self, self.node, "motoman", row=2, col=2)

        # Layout
        self.grid(row=row, column=col, sticky="N", padx=20, pady=20)

        self.grid_columnconfigure((0, 2), weight=1)

        title.grid(row=0, column=0, columnspan=3)
        horizontal_sep.grid(row=1, column=0, columnspan=3, sticky="WE")
        vertical_sep.grid(row=2, column=1, sticky="NS")


class RobotStatusFrame(ctk.CTkFrame):
    def __init__(self, frame, node: Node, robot_name: str, row:int, col: int):
        super().__init__(master=frame, fg_color="transparent")
        self.node = node

        self.robot_name = robot_name

        # Layout
        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure((2, 3, 4), weight=1)
        self.grid(row=row, column=col, sticky="EW")

        # Variables for controller state
        self.statuses = {name: ctk.BooleanVar(value=False) for name in ['state', 'command', 'gripper']}

        # Timing
        self.last_update_time: dict[str, Optional[Time]] = {s: None for s in ['state', 'command', 'gripper']}

        self.list_controller_client = self.node.create_client(ListControllers, f"/{robot_name}/controller_manager/list_controllers")

        self.after_call: Optional[str] = None

        # ROS Subscriptions
        # topic_names = CONTROLLER_STATUS_NAMES[robot_name]

        # state_cb_group = MutuallyExclusiveCallbackGroup()
        # command_cb_group = MutuallyExclusiveCallbackGroup()
        # gripper_cb_group = MutuallyExclusiveCallbackGroup()

        # self.node.create_subscription(Bool, topic_names["state"], self.state_cb, 1, callback_group=state_cb_group)
        # self.node.create_subscription(Bool, topic_names["command"], self.command_cb, 1, callback_group=command_cb_group)
        # self.node.create_subscription(Bool, topic_names["gripper"], self.gripper_cb, 1, callback_group=gripper_cb_group)

        # self.state_after: Optional[str] = None
        # self.command_after: Optional[str] = None
        # self.gripper_after: Optional[str] = None

        # Widgets
        font = ctk.CTkFont(FONT_FAMILY, TITLE_FONT_SIZE, TITLE_FONT_WEIGHT)

        robot_name_label = ctk.CTkLabel(self, text=f"{robot_name.capitalize()}:", font=font)
        robot_name_label.grid(row=0, column=0)

        bar = Separator(self, orient="horizontal")
        bar.grid(row=1, column=0, sticky="WE", pady=2, padx=5)

        ControllerStatus(self, "State", self.statuses['state'], row=2)
        ControllerStatus(self, "Command", self.statuses['command'], row=3)
        ControllerStatus(self, "Gripper", self.statuses['gripper'], row=4)

        self.call_list_robot_controllers()
        
    def call_list_robot_controllers(self):
        if not self.list_controller_client.service_is_ready():
            self.after(250, self.call_list_robot_controllers)
            return

        list_request = ListControllers.Request()

        self.future = self.list_controller_client.call_async(list_request)
        self.future.add_done_callback(self.list_controllers_done)

        self.after_call = self.after(1000, self.list_controllers_timeout)
    
    def list_controllers_timeout(self):
        for status in self.statuses.values():
            status.set(False)
        
        self.list_controller_client.remove_pending_request(self.future)
        self.call_list_robot_controllers()
    
    def list_controllers_done(self, future: Future):
        if self.after_call is not None:
            self.after_cancel(self.after_call)
            self.after_call = None

        result: ListControllers.Response = future.result() # type: ignore

        for controller in result.controller:
            controller: ControllerState
            match(controller.name):
                case 'joint_trajectory_controller':
                    state = controller.state.lower()=="active"
                    if self.statuses['state'].get() != state:
                        self.statuses['state'].set(state)
                case 'joint_state_broadcaster':
                    state = controller.state.lower()=="active"
                    if self.statuses['command'].get() != state:
                        self.statuses['command'].set(state)
                case 'pneumatic_gripper_controller':
                    state = controller.state.lower()=="active"
                    if self.statuses['gripper'].get() != state:
                        self.statuses['gripper'].set(state)
                case _:
                    self.node.get_logger().error("Unknown driver found with name: " + controller.name)
        self.after(250, self.call_list_robot_controllers)

class ControllerStatus(ctk.CTkFrame):
    def __init__(self, frame, controller_name: str, status: ctk.BooleanVar, row):
        super().__init__(frame, fg_color="transparent")

        # Add trace to status variable
        self.status = status
        self.last_status = status.get()
        self.status.trace_add('write', self.update_label)

        # Layout
        self.grid_columnconfigure((0,1), weight=1)
        self.grid(row=row, column=0, padx=10, pady=2, sticky="WE")

        # Labels
        name_font = ctk.CTkFont(FONT_FAMILY, TEXT_FONT_SIZE, weight=TEXT_FONT_WEIGHT)
        status_font = ctk.CTkFont(FONT_FAMILY, STATUS_FONT_SIZE, weight=STATUS_FONT_WEIGHT)

        controller_name_label = ctk.CTkLabel(self, text=f"{controller_name}:", font=name_font, width=80)
        self.status_label = ctk.CTkLabel(self, text="INACTIVE", text_color="white", fg_color=RED, font=status_font, corner_radius=5, width=80)
        
        # Add widgets to frame
        controller_name_label.grid(row=0, column=0,padx=5, sticky="WE")
        self.status_label.grid(row=0, column=1, padx=5, sticky="E")
        
    def update_label(self, *args):
        if self.status.get():
            self.status_label.configure(text="ACTIVE", fg_color=GREEN)
        else:
            self.status_label.configure(text="INACTIVE", fg_color=RED)
        