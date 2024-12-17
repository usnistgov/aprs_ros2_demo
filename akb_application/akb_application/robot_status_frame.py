from typing import Optional

import customtkinter as ctk
from tkinter import ttk

from rclpy.node import Node
from rclpy.time import Time, Duration

from std_msgs.msg import Bool


class StatusFrame(ctk.CTkFrame):
    def __init__(self, frame, node: Node, row: int, col: int):
        super().__init__(frame, fg_color="transparent")
        
        self.grid_columnconfigure((0, 2), weight=1)
        self.grid(row=row, column=col, sticky="SE", padx=20, pady=20)

        font = ctk.CTkFont("Roboto", 18, weight="bold")

        title = ctk.CTkLabel(self, text="Robot Status", font=font)

        title.grid(row=0, column=0, columnspan=3)

        sep = ttk.Separator(self, orient='horizontal')
        sep.grid(row=1, column=0, columnspan=3, sticky="WE")
        
        self.node = node

        self.fanuc_status_frame = RobotStatusFrame(self, self.node, "fanuc")
        self.fanuc_status_frame.grid(row=2, column=0, sticky="WE")

        sep = ttk.Separator(self, orient='vertical')
        sep.grid(row=2, column=1)

        self.motoman_status_frame = RobotStatusFrame(self, self.node, "motoman")
        self.motoman_status_frame.grid(row=2, column=2, sticky="WE")


class RobotStatusFrame(ctk.CTkFrame):
    def __init__(self, frame, node: Node, robot_name: str):
        super().__init__(frame, fg_color="transparent")
        self.node = node

        # Layout
        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure((2, 3, 4), weight=1)

        # Variables for controller state
        self.state_status = ctk.BooleanVar(value=False)
        self.command_status = ctk.BooleanVar(value=False)
        self.gripper_status = ctk.BooleanVar(value=False)

        # Timing
        self.last_update_time: dict[str, Optional[Time]] = {s: None for s in ['state', 'command', 'gripper']}

        # ROS Subscriptions
        topic_names = {
            "state": f"{robot_name}/joint_state_broadcaster_status",
            "command": f"{robot_name}/joint_trajectory_controller_status",
            "gripper": f"{robot_name}/pneumatic_gripper_status"
        }

        self.node.create_subscription(Bool, topic_names["state"], self.state_cb, 1)
        self.node.create_subscription(Bool, topic_names["command"], self.command_cb, 1)
        self.node.create_subscription(Bool, topic_names["gripper"], self.gripper_cb, 1)

        # Widgets
        font = ctk.CTkFont("Roboto", 16, weight="bold")

        robot_name_label = ctk.CTkLabel(self, text=f"{robot_name.capitalize()}:", font=font)
        robot_name_label.grid(row=0, column=0)

        bar = ttk.Separator(self, orient="horizontal")
        bar.grid(row=1, column=0, sticky="WE", pady=2, padx=5)

        ControllerStatus(self, "State", self.state_status, row=2)
        ControllerStatus(self, "Command", self.command_status, row=3)
        ControllerStatus(self, "Gripper", self.gripper_status, row=4)

        self.update()
        
    def state_cb(self, msg: Bool):
        self.last_update_time['state'] = self.node.get_clock().now()
        self.state_status.set(msg.data)
        
    def command_cb(self, msg: Bool):
        self.last_update_time['command'] = self.node.get_clock().now()
        self.command_status.set(msg.data)
        
    def gripper_cb(self, msg: Bool):
        self.last_update_time['gripper'] = self.node.get_clock().now()
        self.gripper_status.set(msg.data)

    def update(self, *args):
        # Reset status to INACTIVE if topic is not published for 3 seconds
        for key, time in self.last_update_time.items():
            if time is None:
                continue

            if (self.node.get_clock().now() - time).nanoseconds > 3E9:
                if key == 'state':
                    self.state_status.set(False)
                elif key == 'command':
                    self.command_status.set(False)
                elif key == 'gripper':
                    self.gripper_status.set(False)

        self.after(100, self.update)
        

class ControllerStatus(ctk.CTkFrame):
    def __init__(self, frame, controller_name: str, status: ctk.BooleanVar, row):
        super().__init__(frame, fg_color="transparent")

        # Add trace to status variable
        self.status = status
        self.status.trace_add('write', self.update_label)

        # Layout
        self.grid_columnconfigure((0,1), weight=1)
        self.grid(row=row, column=0, padx=10, pady=2, sticky="WE")

        # Labels
        name_font = ctk.CTkFont("Roboto", 16)
        status_font = ctk.CTkFont("Roboto", 14, weight="bold")

        controller_name_label = ctk.CTkLabel(self, text=f"{controller_name}:", font=name_font, width=80)
        self.status_label = ctk.CTkLabel(self, text="INACTIVE", text_color="white", fg_color="red", font=status_font, width=80, corner_radius=5)
        
        # Add widgets to frame
        controller_name_label.grid(row=0, column=0,padx=5, sticky="WE")
        self.status_label.grid(row=0, column=1, padx=5, sticky="E")
        
    def update_label(self, *args):
        if self.status.get():
            self.status_label.configure(text="ACTIVE", fg_color="green")
        else:
            self.status_label.configure(text="INACTIVE", fg_color="red")
        