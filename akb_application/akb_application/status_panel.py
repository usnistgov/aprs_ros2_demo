from typing import Optional
from functools import partial

import customtkinter as ctk
from tkinter.ttk import Separator

from rclpy.node import Node
from rclpy.time import Time, Duration
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup

from std_msgs.msg import Bool

from akb_application.settings import *


class StatusPanel(ctk.CTkFrame):
    def __init__(self, frame, node: Node, row: int, col: int):
        super().__init__(frame, fg_color="transparent")

        self.node = node
        
        # Widgets
        title = ctk.CTkLabel(self, text="Status Panel", font=ctk.CTkFont(FONT_FAMILY, TITLE_FONT_SIZE, weight=TITLE_FONT_WEIGHT))

        horizontal_sep = Separator(self, orient='horizontal')
        vertical_sep = Separator(self, orient='horizontal')

        RobotStatusFrame(self, self.node, "fanuc", row=2, col=0)
        RobotStatusFrame(self, self.node, "motoman", row=2, col=2)

        # Layout
        self.grid(row=row, column=col, sticky="N", padx=20, pady=20)

        self.grid_columnconfigure((0, 2), weight=1)

        title.grid(row=0, column=0, columnspan=3)
        horizontal_sep.grid(row=1, column=0, columnspan=3, sticky="WE")
        vertical_sep.grid(row=2, column=1)


class RobotStatusFrame(ctk.CTkFrame):
    def __init__(self, frame, node: Node, robot_name: str, row:int, col: int):
        super().__init__(master=frame, fg_color="transparent")
        self.node = node

        # Layout
        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure((2, 3, 4), weight=1)
        self.grid(row=row, column=col, sticky="EW")

        # Variables for controller state
        self.statuses = {name: ctk.BooleanVar(value=False) for name in ['state', 'command', 'gripper']}

        # Timing
        self.last_update_time: dict[str, Optional[Time]] = {s: None for s in ['state', 'command', 'gripper']}

        # ROS Subscriptions
        topic_names = CONTROLLER_STATUS_NAMES[robot_name]

        state_cb_group = MutuallyExclusiveCallbackGroup()
        command_cb_group = MutuallyExclusiveCallbackGroup()
        gripper_cb_group = MutuallyExclusiveCallbackGroup()

        self.node.create_subscription(Bool, topic_names["state"], self.state_cb, 1, callback_group=state_cb_group)
        self.node.create_subscription(Bool, topic_names["command"], self.command_cb, 1, callback_group=command_cb_group)
        self.node.create_subscription(Bool, topic_names["gripper"], self.gripper_cb, 1, callback_group=gripper_cb_group)

        self.state_after: Optional[str] = None
        self.command_after: Optional[str] = None
        self.gripper_after: Optional[str] = None

        # Widgets
        font = ctk.CTkFont(FONT_FAMILY, TITLE_FONT_SIZE, TITLE_FONT_WEIGHT)

        robot_name_label = ctk.CTkLabel(self, text=f"{robot_name.capitalize()}:", font=font)
        robot_name_label.grid(row=0, column=0)

        bar = Separator(self, orient="horizontal")
        bar.grid(row=1, column=0, sticky="WE", pady=2, padx=5)

        ControllerStatus(self, "State", self.statuses['state'], row=2)
        ControllerStatus(self, "Command", self.statuses['command'], row=3)
        ControllerStatus(self, "Gripper", self.statuses['gripper'], row=4)

        # self.update()
        
    def state_cb(self, msg: Bool):
        if self.statuses['state'].get() != msg.data:
            self.statuses['state'].set(msg.data)
        if self.state_after is not None:
            self.after_cancel(self.state_after)
        self.state_after = self.after(3000, partial(self.update_test, 'state'))
        
    def command_cb(self, msg: Bool):
        if self.statuses['command'].get() != msg.data:
            self.statuses['command'].set(msg.data)
        if self.command_after is not None:
            self.after_cancel(self.command_after)
        self.command_after = self.after(3000, partial(self.update_test, 'command'))
        
    def gripper_cb(self, msg: Bool):
        if self.statuses['gripper'].get() != msg.data:
            self.statuses['gripper'].set(msg.data)
        if self.gripper_after is not None:
            self.after_cancel(self.gripper_after)
        self.gripper_after = self.after(3000, partial(self.update_test, 'gripper'))

    def update_test(self, key: str):
        if self.statuses[key].get():
            self.statuses[key].set(False)
    
    # def update(self, *args):
    #     # Reset status to INACTIVE if topic is not published for 3 seconds
    #     current_time = self.node.get_clock().now()

    #     for key, last_update in self.last_update_time.items():
    #         if last_update is None:
    #             continue

    #         duration = (current_time - last_update).nanoseconds/1E9
            
    #         if  duration >= 3:
    #             self.node.get_logger().info(f'Time since last update for {key} is {duration}')
    #             if key == 'state':
    #                 if self.state_status.get():
    #                     self.state_status.set(False)
    #             elif key == 'command':
    #                 if self.command_status.get():
    #                     self.command_status.set(False)
    #             elif key == 'gripper':
    #                 if self.gripper_status.get():
    #                     self.gripper_status.set(False)

    #     self.after(500, self.update)
        

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
        