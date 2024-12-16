import customtkinter as ctk

from tkinter import ttk
from rclpy.node import Node
from std_msgs.msg import Bool


class RobotStatusFrame(ctk.CTkFrame):
    def __init__(self, frame, node: Node):
        super().__init__(frame, height=300, width=600, fg_color="#EBEBEB")
        self.rowconfigure(0, weight=1)
        self.columnconfigure((0,1,2), weight=1)

        self.node = node

        self.fanuc_status_frame = RobotStatusSubFrame(self, self.node, "fanuc")
        self.fanuc_status_frame.grid(row=0, column=0, padx=2)

        sep = ttk.Separator(self, orient='vertical')
        sep.grid(row=0, column=1, sticky="ns")

        self.motoman_status_frame = RobotStatusSubFrame(self, self.node, "motoman")
        self.motoman_status_frame.grid(row=0, column=2, padx=2)

class RobotStatusSubFrame(ctk.CTkFrame):
    def __init__(self, frame, node: Node, robot_name: str):
        super().__init__(frame, height=300, width=290, fg_color="#EBEBEB")
        self.node = node

        self.rowconfigure((0,1,2,3), weight=1)
        self.columnconfigure((0,1), weight=1)

        self.joint_state_broadcaster_active = False
        self.joint_trajectory_controller_active = False
        self.pneumatic_gripper_active = False

        self.joint_state_broadcaster_sub = self.node.create_subscription(Bool, f"{robot_name}/joint_state_boradcaster_status", self.joint_state_broadcaster_cb, 1)
        self.joint_trajectory_controller_sub = self.node.create_subscription(Bool, f"{robot_name}/joint_trajectory_controller_status", self.joint_trajectory_controller_cb, 1)
        self.pneumatic_gripper_sub = self.node.create_subscription(Bool, f"{robot_name}/pneumatic_gripper_status", self.pneumatic_gripper_status_cb, 1)

        ctk.CTkLabel(self, text= f"{robot_name.capitalize()} controllers:", height=15).grid(row=0, column=0, columnspan=2, pady=10)

        self.joint_state_broadcaster_label = ctk.CTkLabel(self, text="Joint State Broadcaster:", height=15)
        self.joint_state_broadcaster_label.grid(row=1, column=0, pady=2, padx=2)
        self.joint_state_broadcaster_status_label = ctk.CTkLabel(self, text="INACTIVE", text_color="red", height=15)
        self.joint_state_broadcaster_status_label.grid(row=1, column=1, pady=2, padx=2)

        self.joint_trajectory_controller_label = ctk.CTkLabel(self, text="Joint Trajectory Controller:", height=15)
        self.joint_trajectory_controller_label.grid(row=2, column=0, pady=2, padx=2)
        self.joint_trajectory_controller_status_label = ctk.CTkLabel(self, text="INACTIVE", text_color="red", height=15)
        self.joint_trajectory_controller_status_label.grid(row=2, column=1, pady=2, padx=2)

        self.pneumatic_gripper_label = ctk.CTkLabel(self, text="Pneumatic Gripper:", height=15)
        self.pneumatic_gripper_label.grid(row=3, column=0, pady=2, padx=2)
        self.pneumatic_gripper_status_label = ctk.CTkLabel(self, text="INACTIVE", text_color="red", height=15)
        self.pneumatic_gripper_status_label.grid(row=3, column=1, pady=2, padx=2)

    def joint_state_broadcaster_cb(self, msg: Bool):
        if self.joint_state_broadcaster_active != msg.data:
            self.joint_state_broadcaster_active = msg.data
            self.joint_state_broadcaster_status_label.configure(text=("ACTIVE" if self.joint_state_broadcaster_active else "INACTIVE"), text_color=("green" if self.joint_state_broadcaster_active else "red"))
        
    def joint_trajectory_controller_cb(self, msg: Bool):
        if self.joint_trajectory_controller_active != msg.data:
            self.joint_trajectory_controller_active = msg.data
            self.joint_trajectory_controller_status_label.configure(text=("ACTIVE" if self.joint_trajectory_controller_active else "INACTIVE"), text_color=("green" if self.joint_trajectory_controller_active else "red"))
        
    def pneumatic_gripper_status_cb(self, msg: Bool):
        if self.pneumatic_gripper_active != msg.data:
            self.pneumatic_gripper_active = msg.data
            self.pneumatic_gripper_status_label.configure(text=("ACTIVE" if self.pneumatic_gripper_active else "INACTIVE"), text_color=("green" if self.pneumatic_gripper_active else "red"))