import customtkinter as ctk
import tkinter as tk

from typing import Optional

from rclpy.node import Node
from rclpy.action import ActionClient
from rclpy.action.client import ClientGoalHandle
from rclpy.task import Future
from plansys2_msgs.msg import Plan, PlanItem
from aprs_interfaces.srv import GeneratePlan
from aprs_interfaces.action import ExecutePlan
from aprs_interfaces.action._execute_plan import ExecutePlan_FeedbackMessage

class PDDLFrame(ctk.CTkFrame):
    def __init__(self, frame, node: Node):
        super().__init__(frame, height=1100, width=600, fg_color="#EBEBEB")
        self.grid_columnconfigure(0, weight=1)
        self.node = node
        self.generate_plan_client = self.node.create_client(GeneratePlan, "generate_pddl_plan")
        
        self.execute_plan_client = ActionClient(self.node, ExecutePlan, "/execute_pddl_plan")

        self.generate_plan_button = ctk.CTkButton(self, text="Generate Plan", command=self.generate_plan_button_cb, state=tk.NORMAL)
        self.generate_plan_button.grid(row=0)
        self.plan_text = ctk.CTkLabel(self, text="", height=200, width=550)
        self.plan_text.grid(row=1)

        self.execute_plan_button = ctk.CTkButton(self, text="Execute Plan", command=self.execute_plan_button_cb, state=tk.DISABLED)
        self.execute_plan_button.grid(row=2)
        self.feedback_text = ctk.CTkLabel(self, text="", height=200, width=550)
        self.feedback_text.grid(row=3)

        self.plan: Optional[Plan] = None
    
    def generate_plan_button_cb(self):
        if not self.generate_plan_client.service_is_ready():
            self.plan_text.configure(text="Service not ready", text_color="red")
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
            self.plan_text.configure(text="• "+"\n• ".join([item.action for item in plan_items]), text_color="green")
            self.execute_plan_button.configure(state=tk.NORMAL)
            self.plan = response.plan
        else:
            self.plan_text.configure(text=f"No plan found\n{response.status}", text_color="red")

    def execute_plan_button_cb(self):
        self.execute_plan_button.configure(state=tk.DISABLED, fg_color="yellow")
        if not self.execute_plan_client.server_is_ready():
            self.feedback_text.configure(text="Execute action server not ready", text_color="red")
            return
        
        if self.plan is None:
            self.feedback_text.configure(text="Plan not set yet", text_color="red")

        self.node.get_logger().info("Executing plan")

        goal = ExecutePlan.Goal()
        goal.plan = self.plan

        try:
            send_goal_future = self.execute_plan_client.send_goal_async(goal, feedback_callback=self.execute_plan_feedback_callback)
            
            send_goal_future.add_done_callback(self.goal_response_callback)
            
            self.execute_plan_button.configure(state=tk.DISABLED)
            self.plan_text.configure(text="")
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
            self.feedback_text.configure(text=f"Execution finished\nResult: {result.status}")
        else:
            self.execute_plan_button.configure(fg_color="red")
            self.feedback_text.configure(text=f"Unable to execute plan\nResult: {result.status}")
        
        self.generate_plan_button.configure(state=tk.NORMAL)
    
    def execute_plan_feedback_callback(self, feedback: ExecutePlan_FeedbackMessage):
        self.feedback_text.configure(text=feedback.feedback.current_action, text_color="black")
