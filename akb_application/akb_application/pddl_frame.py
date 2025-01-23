import customtkinter as ctk
from tkinter import ttk
from tkinter.ttk import Separator
from tkinter import NORMAL, DISABLED

from typing import Optional

from rclpy.node import Node
from rclpy.action import ActionClient
from rclpy.action.client import ClientGoalHandle
from rclpy.task import Future
from plansys2_msgs.msg import Plan, PlanItem, ActionExecutionInfo
from aprs_interfaces.srv import GeneratePlan, ClearCurrentState
from aprs_interfaces.action import ExecutePlan
from aprs_interfaces.action._execute_plan import ExecutePlan_FeedbackMessage

from akb_application.settings import *

class PDDLFrame(ctk.CTkFrame):
    def __init__(self, frame, node: Node, row: int, col: int):
        super().__init__(frame, fg_color="transparent")
        
        self.grid(row=row, column=col, sticky="N")
        
        self.node = node
        self.plan_received = ctk.BooleanVar(value=False)
        self.reset_occurred = ctk.BooleanVar(value=False)
        self.execution_started = ctk.BooleanVar(value=False)
        self.execution_complete_keys = ctk.StringVar(value="")
        
        title = ctk.CTkLabel(self, text="PDDL Panel", font=ctk.CTkFont(FONT_FAMILY, TITLE_FONT_SIZE, weight=TITLE_FONT_WEIGHT))
        title.grid(row=0, column=0, columnspan=3, pady=5)
        
        Separator(self, orient='horizontal').grid(row=1, column=0, columnspan=3, padx=10, sticky="WE")
        
        self.plan_frame = PlanFrame(self, row=3, column=0, columnspan=3)
        
        self.plan_button = PlanButton(self, node, self.plan_received, row=2, column=0)
        self.execute_button = ExecuteButton(self, node, self.execution_started, self.execution_complete_keys, row=2, column=1)
        self.reset_button = ResetButton(self, node, self.reset_occurred, row=2, column=2)

        self.plan_received.trace_add('write', self.update_plan)
        self.reset_occurred.trace_add('write', self.reset)
        self.execution_started.trace_add('write', self.execution_starting)
        self.execution_complete_keys.trace_add('write', self.update_plan_frame_results)

        
    def update_plan(self, *args):
        if self.plan_received.get():
            self.execute_button.set_plan(self.plan_button.get_plan_items())

            self.plan_frame.new_plan(self.plan_button.get_plan_items())

            self.plan_button.configure(state=DISABLED)
            self.reset_button.configure(state=NORMAL)

            self.plan_received.set(False)


    def reset(self, *args):
        if self.reset_occurred.get():
            self.plan_frame.clear_frame()
            self.plan_frame.plan_rows.clear()
            self.plan_frame.no_plan_label.grid(column=0, row=2, columnspan=5)

            self.execute_button.set_plan([])

            self.reset_button.configure(state=DISABLED)
            self.plan_button.configure(state=NORMAL)
            self.reset_occurred.set(False)
    
    def execution_starting(self, *args):
        if self.execution_started.get():
            for key in self.plan_frame.plan_rows.keys():
                self.plan_frame.plan_row_in_progress(key)
    
    def update_plan_frame_results(self, *args):
        self.node.get_logger().info("Inside update plan frame")
        if len(self.execution_complete_keys.get())==0:
            return
        results_seperated = self.execution_complete_keys.get().split("|")
        for key in results_seperated:
            self.plan_frame.plan_row_complete(key)
        self.plan_frame.update_progress(len(results_seperated))

    # def executing(self, *args):
    #     if self.execution_started.get():
    #         pass
    
class PlanRow(ctk.CTkFrame):
    def __init__(self, frame, robot: str, gear_size: str, area: str, action: str, slot: str, row, column, columnspan: int):
        super().__init__(frame, fg_color="transparent")

        self.robot_label = ctk.CTkLabel(self, text=robot, width=ROBOT_LABEL_WIDTH, font=ctk.CTkFont(FONT_FAMILY, TEXT_FONT_SIZE, weight=TEXT_FONT_WEIGHT))
        self.gear_size_label = ctk.CTkLabel(self, text=gear_size, width=GEAR_SIZE_LABEL_WIDTH, font=ctk.CTkFont(FONT_FAMILY, TEXT_FONT_SIZE, weight=TEXT_FONT_WEIGHT))
        self.area_label = ctk.CTkLabel(self, text=area, width=AREA_LABEL_WIDTH, font=ctk.CTkFont(FONT_FAMILY, TEXT_FONT_SIZE, weight=TEXT_FONT_WEIGHT))
        self.action_label = ctk.CTkLabel(self, text=action, width=ACTION_LABEL_WIDTH, font=ctk.CTkFont(FONT_FAMILY, TEXT_FONT_SIZE, weight=TEXT_FONT_WEIGHT))
        self.slot_label = ctk.CTkLabel(self, text=slot, width=SLOT_LABEL_WIDTH, font=ctk.CTkFont(FONT_FAMILY, TEXT_FONT_SIZE, weight=TEXT_FONT_WEIGHT))

        self.robot_label.grid(row=0, column=0)
        self.action_label.grid(row=0, column=1)
        self.gear_size_label.grid(row=0, column=2)
        self.area_label.grid(row=0, column=3)
        self.slot_label.grid(row=0, column=4)

        self.grid(row=row, column=column, columnspan=columnspan)
    
    def change_color(self, color):
        self.configure(fg_color=color)


class PlanFrame(ctk.CTkFrame):
    ROBOT_COLUMN = 0
    ACTION_COLUMN = 1
    SLOT_COLUMN = 2

    def __init__(self, frame, row, column, columnspan):
        super().__init__(frame, fg_color="transparent", width=sum([ROBOT_LABEL_WIDTH, ACTION_LABEL_WIDTH, SLOT_LABEL_WIDTH, GEAR_SIZE_LABEL_WIDTH, AREA_LABEL_WIDTH]))

        self.total_progress_label: Optional[ctk.CTkLabel] = None
        self.total_progress_bar: Optional[ctk.CTkProgressBar] = None

        robot_label = ctk.CTkLabel(self, text="Robot", width=ROBOT_LABEL_WIDTH, font=ctk.CTkFont(FONT_FAMILY, TEXT_FONT_SIZE, weight="bold"))
        gear_size_label = ctk.CTkLabel(self, text="Gear Size", width=GEAR_SIZE_LABEL_WIDTH, font=ctk.CTkFont(FONT_FAMILY, TEXT_FONT_SIZE, weight="bold"))
        area_label = ctk.CTkLabel(self, text="Area", width=AREA_LABEL_WIDTH, font=ctk.CTkFont(FONT_FAMILY, TEXT_FONT_SIZE, weight="bold"))
        action_label = ctk.CTkLabel(self, text="Action", width=ACTION_LABEL_WIDTH, font=ctk.CTkFont(FONT_FAMILY, TEXT_FONT_SIZE, weight="bold"))
        slot_label = ctk.CTkLabel(self, text="Slot", width=SLOT_LABEL_WIDTH, font=ctk.CTkFont(FONT_FAMILY, TEXT_FONT_SIZE, weight="bold"))
        
        robot_label.grid(row=0, column=0)
        action_label.grid(row=0, column=1)
        gear_size_label.grid(row=0, column=2)
        area_label.grid(row=0, column=3)
        slot_label.grid(row=0, column=4)

        self.plan_rows: dict[str, PlanRow] = {}

        sep = ttk.Separator(self, orient='horizontal')
        sep.grid(column=0, row=1, columnspan=5, sticky="ew",)

        self.no_plan_label = ctk.CTkLabel(self, text="NO PLAN CREATED YET", font=ctk.CTkFont(FONT_FAMILY, TEXT_FONT_SIZE, weight=TEXT_FONT_WEIGHT))
        self.no_plan_label.grid(column=0, row=2, columnspan=5)

        self.grid(row=row, column=column, columnspan=columnspan)


    def clear_frame(self):
        for row in self.plan_rows.items():
            row.grid_forget()
        self.plan_rows = {}
        
        if self.total_progress_label is not None:
            self.total_progress_label.grid_forget()

        if self.total_progress_bar is not None:
            self.total_progress_bar.grid_forget()

        self.total_progress_label = None
        self.total_progress_bar = None

    def new_plan(self, plan_items: list[PlanItem]):
        current_row = 2

        self.no_plan_label.grid_forget()

        for item in plan_items:
            action = item.action
            filtered_action = action.replace("(", "").replace(")", "")
            split_action = filtered_action.split(" ")
            
            robot = split_action[0].split("_")[0].capitalize()
            gear_size = split_action[2]
            area = split_action[3].capitalize().replace("_", " ")
            action_type = split_action[0].split("_")[1].capitalize()
            slot = split_action[1]

            key = "_".join(split_action[:2])

            self.plan_rows[key]= PlanRow(self, robot, gear_size, area, action_type, slot, current_row, 0, 5)
            current_row+=1

        self.total_progress_label = ctk.CTkLabel(self, text="Total progress: 0%")
        self.total_progress_label.grid(row=current_row, column=0, columnspan=5, pady=3, sticky="s")

        self.total_progress_bar = ctk.CTkProgressBar(self)
        self.total_progress_bar.set(0.0)
        self.total_progress_bar.grid(row=current_row+1, column=0, columnspan=5, pady=0, sticky="s")
        
    
    def plan_row_in_progress(self, key):
        self.plan_rows[key].configure(fg_color=YELLOW)

    def plan_row_complete(self, key):
        self.plan_rows[key].configure(fg_color=GREEN)
    
    def plan_row_failed(self, key):
        self.plan_rows[key].configure(fg_color=RED)

    def update_progress(self, value):
        if self.total_progress_bar is not None:
            self.total_progress_bar.set(value/len(self.plan_rows))

        if self.total_progress_label is not None:
            self.total_progress_label.configure(text=f"Total progress: {int(value/len(self.plan_rows) * 100)}%")


class PlanButton(ctk.CTkButton):
    def __init__(self, frame, node: Node, plan_recieved: ctk.BooleanVar, row: int, column: int):
        super().__init__(
            frame,
            text='Generate Plan',
            command=self.command_cb,
            font=ctk.CTkFont(FONT_FAMILY, BUTTON_FONT_SIZE, weight=BUTTON_FONT_WEIGHT),
            fg_color=BLUE,
            hover_color=DARK_BLUE
        )

        self.node = node
        self.plan_items: list[PlanItem] = []

        self.plan_received = plan_recieved

        self.client = self.node.create_client(GeneratePlan, PDDL_NAMES['plan'])
        
        self.grid(row=row, column=column, padx=10, pady=10)
    
    def command_cb(self, *args):
        self.plan_items = []

        self.configure(fg_color=YELLOW, state=DISABLED)
        
        request = GeneratePlan.Request()
        
        if not self.client.service_is_ready():
            self.node.get_logger().error(f"Generate plan service not ready.")
            self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.after(3000, self.reset)
            return

        future = self.client.call_async(request)

        future.add_done_callback(self.srv_done_cb)
        
    def srv_done_cb(self, future: Future):
        result: GeneratePlan.Response = future.result() # type: ignore
        
        if result.success:
            self.configure(fg_color=GREEN, hover_color=DARK_GREEN, state=NORMAL)

            for item in result.plan.items:
                self.plan_items.append(item)
            
            self.plan_received.set(value=True)
        else:
            self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.node.get_logger().error(f'Unable to generate plan.')
            
        self.after(3000, self.reset)
        
    def reset(self, *args):
        self.configure(fg_color=BLUE, hover_color=DARK_BLUE, state=NORMAL)
    
    def get_plan_items(self) -> list[PlanItem]:
        return self.plan_items
        
class ResetButton(ctk.CTkButton):
    def __init__(self, frame, node: Node, reset_occurred: ctk.BooleanVar, row: int, column: int):
        super().__init__(
            frame,
            text='Reset',
            command=self.command_cb,
            font=ctk.CTkFont(FONT_FAMILY, BUTTON_FONT_SIZE, weight=BUTTON_FONT_WEIGHT),
            fg_color=BLUE,
            hover_color=DARK_BLUE,
            state=DISABLED,
        )
    
        self.node = node
        self.reset_occurred = reset_occurred
        self.client = self.node.create_client(ClearCurrentState, PDDL_NAMES['clear'])
        
        self.grid(row=row, column=column, padx=10, pady=10)
    
    def command_cb(self, *args):
        self.reset_occurred.set(True)
        self.configure(fg_color=YELLOW, state=DISABLED)

        request = ClearCurrentState.Request()
        
        if not self.client.service_is_ready():
            self.node.get_logger().error(f"Clears service not ready.")
            self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.after(3000, self.reset)
            return

        future = self.client.call_async(request)

        future.add_done_callback(self.srv_done_cb)
        
    def srv_done_cb(self, future: Future):
        result: GeneratePlan.Response = future.result() # type: ignore
        
        if result.success:
            self.configure(fg_color=GREEN, hover_color=DARK_GREEN, state=NORMAL)
        else:
            self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.node.get_logger().error(f'Unable to reset.')
        
        self.after(3000, self.reset)
        
    def reset(self, *args):
        self.configure(fg_color=BLUE, hover_color=DARK_BLUE, state=NORMAL)
        
class ExecuteButton(ctk.CTkButton):
    def __init__(self, frame, node: Node, execution_started: ctk.BooleanVar, execution_complete_keys: ctk.StringVar, row: int, column: int):
        super().__init__(
            frame,
            text='Execute',
            command=self.command_cb,
            font=ctk.CTkFont(FONT_FAMILY, BUTTON_FONT_SIZE, weight=BUTTON_FONT_WEIGHT),
            fg_color=BLUE,
            hover_color=DARK_BLUE
        )
        
        self.execution_started = execution_started
        self.execution_complete_keys = execution_complete_keys
        self.plan: Optional[Plan] = None

        self.already_set_keys: list[str] = []

        self.node = node
        self.client = ActionClient(self.node, ExecutePlan, PDDL_NAMES['execute'])
        
        self.grid(row=row, column=column, padx=10, pady=10)

    def set_plan(self, plan_items: list[PlanItem]):
        if plan_items:
            self.plan = Plan()
            self.plan.items = plan_items
        else:
            self.plan = None
    
    def command_cb(self, *args):
        if self.plan is None:
            self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.node.get_logger().error("No plan set")
            self.after(3000, self.reset)
        
        self.configure(fg_color=YELLOW, state=DISABLED)

        self.execution_started.set(True)

        goal = ExecutePlan.Goal()
        goal.plan = self.plan

        try:
            send_goal_future = self.client.send_goal_async(goal, feedback_callback=self.feedback_callback)
            send_goal_future.add_done_callback(self.goal_response_callback)
             
        except:
            self.node.get_logger().error("Unable to send plan goal")
    
    def feedback_callback(self, feedback: ExecutePlan_FeedbackMessage):
        action_execution_infos: list[ActionExecutionInfo] = feedback.feedback.action_execution_status # type: ignore
        
        keys = []
        for info in action_execution_infos:
            if info.completion == 1:
                keys.append(info.action + "_" + info.arguments[0]) # type: ignore
        complete_keys = "|".join(keys)
        if self.execution_complete_keys.get()!= complete_keys:
            self.execution_complete_keys.set(complete_keys)
        # if len(action_execution_infos) > 0:
        #     num_complete = sum([info.completion for info in action_execution_infos])
        #     if self.execution_complete_keys.get() != num_complete:
        #         self.execution_count.set(num_complete)

    def goal_response_callback(self, future: Future):
        goal_handle: ClientGoalHandle = future.result() # type: ignore
        if not goal_handle.accepted:
            self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.node.get_logger().error("Plan goal not accepted")
            self.after(3000, self.reset)
            return

        result_future: Future = goal_handle.get_result_async()
        result_future.add_done_callback(self.get_result_callback)
        
    def get_result_callback(self, future: Future):
        result: ExecutePlan.Result = future.result().result # type: ignore

        if result.success:
            self.configure(fg_color=GREEN, hover_color=DARK_GREEN, state=NORMAL)
        else:
            self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
        self.execution_started.set(False)
        self.after(3000, self.reset)
    
    def reset(self, *args):
        self.configure(fg_color=BLUE, hover_color=DARK_BLUE, state=NORMAL)
        self.already_set_keys.clear()
