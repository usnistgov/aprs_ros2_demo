import customtkinter as ctk
import tkinter as tk
from tkinter import ttk

from copy import copy
from typing import Optional

from rclpy.node import Node
from rclpy.action import ActionClient
from rclpy.action.client import ClientGoalHandle
from rclpy.task import Future
from plansys2_msgs.msg import Plan, PlanItem, ActionExecutionInfo
from aprs_interfaces.msg import RobotChangeover
from aprs_interfaces.srv import GeneratePlan, ClearCurrentState
from aprs_interfaces.action import ExecutePlan
from aprs_interfaces.action._execute_plan import ExecutePlan_FeedbackMessage

class ActionProgressBar:
    def __init__(self, progress_bar: ctk.CTkProgressBar, row: int):
        self.progress_bar = progress_bar
        self.row = row

class PDDLFrame(ctk.CTkFrame):
    def __init__(self, frame, node: Node):
        super().__init__(frame, height=1100, width=600, fg_color="#EBEBEB")
        self.grid_columnconfigure((0,1,2), weight=1)
        self.grid_rowconfigure([i for i in range(5)], weight=1)
        self.grid_propagate(False)
        self.node = node

        self.font = ("", 15)

        self.generate_plan_client = self.node.create_client(GeneratePlan, "generate_pddl_plan")
        self.clear_plan_client = self.node.create_client(ClearCurrentState, "clear_pddl_current_state")
        self.execute_plan_client = ActionClient(self.node, ExecutePlan, "/execute_pddl_plan")

        self.changeover_publisher = self.node.create_publisher(RobotChangeover, "robot_changeover_requested", 10)
        
        self.generate_plan_button = ctk.CTkButton(self, text="Generate Plan", command=self.generate_plan_button_cb, state=tk.NORMAL)
        self.generate_plan_button.grid(row=0, column=0, pady=15)

        self.execute_plan_button = ctk.CTkButton(self, text="Execute Plan", command=self.execute_plan_button_cb, state=tk.DISABLED)
        self.execute_plan_button.grid(row=0, column=1, pady=15)
        
        self.clear_plan_button = ctk.CTkButton(self, text="Reset", command=self.clear_plan_button_cb, state=tk.NORMAL)
        self.clear_plan_button.grid(row=0, column=2, pady=15)

        self.pddl_info_frame = ctk.CTkFrame(self, width=550, height=800, fg_color="#EBEBEB")
        self.pddl_info_frame.grid(row=1, column=0, columnspan=3)
        
        self.selected_robot = ctk.StringVar(value="fanuc")
        self.robot_switch = ctk.CTkSwitch(self, text="Fanuc", command=self.update_robot_switch,
                                          variable=self.selected_robot, onvalue="motoman", offvalue="fanuc", height=40, width=135, font=self.font, fg_color="#3B8ED0", progress_color="#3B8ED0")
        self.robot_switch.grid(column=0, row=2, columnspan = 3, padx=10, pady=10)

        self.changeover_button = ctk.CTkButton(self, text="Changeover", command=self.robot_change_over)
        self.changeover_button.grid(column=0, row=3, columnspan=3, padx=10, pady=10)
        
        self.total_progress_label = ctk.CTkLabel(self, text="Total_progress: 0.0%")
        self.total_progress_label.grid(row=4, column=0, columnspan=3, sticky="s")
        self.total_progress_bar = ctk.CTkProgressBar(self)
        self.total_progress_bar.grid(row=5, column=0, columnspan=3, sticky="s")
        self.total_progress_bar.set(0.0)

        self.action_labels: dict[str, ctk.CTkLabel] = {}
        self.action_args_labels: dict[str, ctk.CTkLabel] = {}
        self.progress_bars: dict[str, ActionProgressBar] = {}
        self.pddl_info_frame_current_row = 0

        self.plan: Optional[Plan] = None

    def update_robot_switch(self):
        self.robot_switch.configure(text=self.selected_robot.get().capitalize())
    
    def robot_change_over(self):
        changeover_msg = RobotChangeover()
        if self.selected_robot.get() == "motoman":
            changeover_msg.motoman = True
            changeover_msg.fanuc = False
        else:
            changeover_msg.motoman = False
            changeover_msg.fanuc = True

        self.changeover_publisher.publish(changeover_msg)

    def clear_pddl_info_frame(self):
        w=[]
        for widget in self.pddl_info_frame.children.values():
            widget.grid_forget()
        self.action_labels.clear()
        self.action_args_labels.clear()
        self.progress_bars.clear()
        
        
    
    def clear_plan_button_cb(self):
        self.clear_pddl_info_frame()
        if not self.clear_plan_client.service_is_ready():
            ctk.CTkLabel(self.pddl_info_frame, text="Service not ready", text_color="red").grid(column=0, row=0)
            return

        clear_plan_request = ClearCurrentState.Request()

        future = self.clear_plan_client.call_async(clear_plan_request)

        future.add_done_callback(self.clear_plan_done_client)

    def clear_plan_done_client(self, future: Future):
        if future.cancelled():
            return
        
        response: ClearCurrentState.Response = future.result() # type: ignore

        if response.success:
            self.generate_plan_button.configure(state=tk.NORMAL)
            self.execute_plan_button.configure(state=tk.DISABLED)
            self.total_progress_label.configure(text="Total_progress: 0.0%")
            self.total_progress_bar.set(0.0)
        else:
            self.clear_pddl_info_frame()
            ctk.CTkLabel(self.pddl_info_frame, text="Service not ready", text_color="red").grid(column=0, row=0)
    
    def generate_plan_button_cb(self):
        self.clear_pddl_info_frame()
        if not self.generate_plan_client.service_is_ready():
            ctk.CTkLabel(self.pddl_info_frame, text="Service not ready", text_color="red").grid(column=0, row=0)
            return

        generate_plan_request = GeneratePlan.Request()

        future = self.generate_plan_client.call_async(generate_plan_request)

        future.add_done_callback(self.generate_plan_done_cb)
    
    def generate_plan_done_cb(self, future: Future):
        if future.cancelled():
            return
        
        response: GeneratePlan.Response = future.result() # type: ignore

        if response.success:
            # self.generate_plan_button.configure(state=tk.DISABLED)
            plan_items: list[PlanItem] = response.plan.items # type: ignore
            for item in plan_items:
                filtered_action = item.action.replace("(", "").replace(")", "")
                seperated_action = filtered_action.split(" ")
                action = seperated_action[0]
                action_args = "  ".join(seperated_action[1:])
                self.action_labels[action] = ctk.CTkLabel(self.pddl_info_frame, text=action, height=15)
                self.action_labels[action].grid(column=0, row=self.pddl_info_frame_current_row, sticky="w", pady=0)
                self.pddl_info_frame_current_row += 1

                self.progress_bars[action + "_" + seperated_action[1]] = ActionProgressBar(ctk.CTkProgressBar(self.pddl_info_frame), copy(self.pddl_info_frame_current_row - 1))
                self.progress_bars[action + "_" + seperated_action[1]].progress_bar.set(0.0)

                self.action_args_labels[action] = ctk.CTkLabel(self.pddl_info_frame, text=action_args, height=15)
                self.action_args_labels[action].grid(column=0, row=self.pddl_info_frame_current_row, sticky="w", padx=3, pady=0)
                self.pddl_info_frame_current_row += 1

                sep = ttk.Separator(self.pddl_info_frame, orient='horizontal')
                sep.grid(column=0, row=self.pddl_info_frame_current_row, columnspan=2, sticky="ew")
                self.pddl_info_frame_current_row += 1
            self.execute_plan_button.configure(state=tk.NORMAL)
            self.plan = response.plan
        else:
            self.clear_pddl_info_frame()
            ctk.CTkLabel(self.pddl_info_frame, text="Unable to plan", text_color="red").grid(column=0, row=0)

    def execute_plan_button_cb(self):
        self.generate_plan_button.configure(state=tk.DISABLED)
        self.execute_plan_button.configure(state=tk.DISABLED, fg_color="yellow")
        if not self.execute_plan_client.server_is_ready():
            self.node.get_logger().error("Execute action server not ready")
            return
        
        if self.plan is None:
            self.clear_pddl_info_frame()
            ctk.CTkLabel(self.pddl_info_frame, text="Plan not set yet", text_color="red").grid(column=0, row=0)
            return

        self.node.get_logger().info("Executing plan")

        goal = ExecutePlan.Goal()
        goal.plan = self.plan

        try:
            send_goal_future = self.execute_plan_client.send_goal_async(goal, feedback_callback=self.execute_plan_feedback_callback)
            
            send_goal_future.add_done_callback(self.goal_response_callback)
            

            for k, bar in self.progress_bars.items():
                self.node.get_logger().info("gridding progress bar for " + k)
                bar.progress_bar.grid(column=1, row=bar.row, rowspan=2)
                
            self.execute_plan_button.configure(state=tk.DISABLED)
            self.generate_plan_button.configure(state=tk.NORMAL)
        except:
            self.node.get_logger().info("Could not execute plan")
        
    def goal_response_callback(self, future: Future):
        goal_handle: ClientGoalHandle = future.result() # type: ignore
        if not goal_handle.accepted:
            self.node.get_logger().info('Plan execution not accepted')
            self.execute_plan_button.configure(state=tk.NORMAL, fg_color="blue")
            return

        self.execute_plan_button.configure(fg_color="orange")
        self.node.get_logger().info('Goal accepted :)')

        result_future: Future = goal_handle.get_result_async()
        result_future.add_done_callback(self.get_result_callback)

    def get_result_callback(self, future: Future):
        result: ExecutePlan.Result = future.result().result # type: ignore

        if result.success:
            self.execute_plan_button.configure(fg_color="green")
        else:
            self.execute_plan_button.configure(fg_color="red")
        
        self.generate_plan_button.configure(state=tk.NORMAL)
    
    def execute_plan_feedback_callback(self, feedback: ExecutePlan_FeedbackMessage):
        action_execution_infos: list[ActionExecutionInfo] = feedback.feedback.action_execution_status # type: ignore
        
        if len(action_execution_infos) > 0:
            total_progress = 0
            for info in action_execution_infos:
                self.progress_bars[info.action + "_" + info.arguments[0]].progress_bar.set(info.completion) # type: ignore
                total_progress+=info.completion
            
            total_percentage = total_progress/len(action_execution_infos)
            self.total_progress_bar.set(total_percentage)
            self.total_progress_label.configure(text=f"Total progress: {int(total_percentage * 100)}%")