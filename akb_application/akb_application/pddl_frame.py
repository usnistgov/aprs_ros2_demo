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
        
        self.plan_received = ctk.BooleanVar(value=False)
        self.reset_occurred = ctk.BooleanVar(value=False)
        self.execution_started = ctk.BooleanVar(value=False)
        
        title = ctk.CTkLabel(self, text="PDDL Panel", font=ctk.CTkFont(FONT_FAMILY, TITLE_FONT_SIZE, weight=TITLE_FONT_WEIGHT))
        title.grid(row=0, column=0, columnspan=3, pady=5)
        
        Separator(self, orient='horizontal').grid(row=1, column=0, columnspan=3, padx=10, sticky="WE")
        
        self.plan_frame = PlanFrame(self, row=3, column=0, columnspan=3)
        
        self.plan_button = PlanButton(self, node, self.plan_received, row=2, column=0)

        self.plan_received.trace_add('write', self.update_plan)
        self.reset_occurred.trace_add('write', self.reset)
        self.execution_started.trace_add('write', self.executing)

        # self.execute_button = ExecuteButton(self, node, row=2, column=1, plan_frame = self.plan_frame)
        self.reset_button = ResetButton(self, node, self.reset_occurred, row=2, column=2)

    def update_plan(self, *args):
        if self.plan_received.get():
            self.plan_frame.new_plan(self.plan_button.get_plan_items())

            self.plan_button.configure(state=DISABLED)
            self.reset_button.configure(state=NORMAL)

            self.plan_received.set(False)


    def reset(self, *args):
        if self.reset_occurred.get():
            self.plan_frame.clear_frame()
            self.plan_frame.plan_rows.clear()
            self.plan_frame.no_plan_label.grid(column=0, row=2, columnspan=5)

            self.reset_button.configure(state=DISABLED)
            self.plan_button.configure(state=NORMAL)
            self.reset_occurred.set(False)

    def executing(self, *args):
        if self.execution_started.get():
            pass
    
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

        self.plan_rows: list[PlanRow] = []

        sep = ttk.Separator(self, orient='horizontal')
        sep.grid(column=0, row=1, columnspan=5, sticky="ew",)

        self.no_plan_label = ctk.CTkLabel(self, text="NO PLAN CREATED YET", font=ctk.CTkFont(FONT_FAMILY, TEXT_FONT_SIZE, weight=TEXT_FONT_WEIGHT))
        self.no_plan_label.grid(column=0, row=2, columnspan=5)

        self.grid(row=row, column=column, columnspan=columnspan)


    def clear_frame(self):
        for row in self.plan_rows:
            row.grid_forget()
        
        
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

            self.plan_rows.append(PlanRow(self, robot, gear_size, area, action_type, slot, current_row, 0, 5))
            current_row+=1

    def update_label(self, key):
        self.robot_labels[key].configure(text_color="green")
        self.slot_labels[key].configure(text_color="green")
        self.action_labels[key].configure(text_color="green")

    def update_progress(self, value):
        if self.total_progress_bar is not None:
            self.total_progress_bar.set(value)

        if self.total_progress_label is not None:
            self.total_progress_label.configure(text=f"Total progress: {int(value * 100)}%")


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

        # self.configure(fg_color=YELLOW, state=DISABLED)
        
        plan_item = PlanItem()
        plan_item.duration = 5.0
        plan_item.action = "(motoman_place medium_gear_tray_03_slot_2 medium motoman_conveyor)"
        plan_item.time = 1.5
        self.plan_items.append(plan_item)

        plan_item = PlanItem()
        plan_item.duration = 7.0
        plan_item.action = "(fanuc_pick s2l2_kit_tray_lg_1 large fanuc_table)"
        plan_item.time = 1.8
        self.plan_items.append(plan_item)
        self.plan_received.set(value=True)
        
        # self.node.get_logger().info(str(self.parent_frame.plan))

        # self.plan_frame.new_plan()
        # request = GeneratePlan.Request()
        
        # if not self.client.service_is_ready():
        #     self.node.get_logger().error(f"Generate plan service not ready.")
        #     self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
        #     self.after(3000, self.reset)
        #     return

        # future = self.client.call_async(request)

        # future.add_done_callback(self.srv_done_cb)
        
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
        # self.configure(fg_color=YELLOW, state=DISABLED)

        # request = ClearCurrentState.Request()
        
        # if not self.client.service_is_ready():
        #     self.node.get_logger().error(f"Clears service not ready.")
        #     self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
        #     self.after(3000, self.reset)
        #     return

        # future = self.client.call_async(request)

        # future.add_done_callback(self.srv_done_cb)
        
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
    def __init__(self, frame, node: Node, row: int, column: int, plan_frame: PlanFrame):
        super().__init__(
            frame,
            text='Execute',
            command=self.command_cb,
            font=ctk.CTkFont(FONT_FAMILY, BUTTON_FONT_SIZE, weight=BUTTON_FONT_WEIGHT),
            fg_color=BLUE,
            hover_color=DARK_BLUE
        )
        
        self.plan_frame = plan_frame

        self.already_set_keys: list[str] = []

        self.parent_frame: PDDLFrame =frame

        self.node = node
        self.client = ActionClient(self.node, ExecutePlan, PDDL_NAMES['execute'])
        
        self.grid(row=row, column=column, padx=10, pady=10)
    
    def command_cb(self, *args):
        self.configure(fg_color=YELLOW, state=DISABLED)
        
        if self.parent_frame.plan is None:
            self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.node.get_logger().error('No plan generated')
            self.after(3000, self.reset)
            return

        goal = ExecutePlan.Goal()
        goal.plan = self.parent_frame.plan

        try:
            send_goal_future = self.client.send_goal_async(goal, feedback_callback=self.feedback_callback)
            send_goal_future.add_done_callback(self.goal_response_callback)
             
        except:
            self.node.get_logger().error("Unable to send plan goal")
    
    def feedback_callback(self, feedback: ExecutePlan_FeedbackMessage):
        action_execution_infos: list[ActionExecutionInfo] = feedback.feedback.action_execution_status # type: ignore
        
        if len(action_execution_infos) > 0:
            total_progress = sum([info.completion for info in action_execution_infos])
            for info in action_execution_infos:
                key = info.action + "_" + info.arguments[0] # type: ignore
                if key in self.already_set_keys:
                    continue
                if info.completion == 1:
                    self.plan_frame.update_label(key)
                    self.already_set_keys.append(key)
            self.plan_frame.update_progress(total_progress/len(action_execution_infos))

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
        
        self.after(3000, self.reset)
    
    def reset(self, *args):
        self.configure(fg_color=BLUE, hover_color=DARK_BLUE, state=NORMAL)
        self.already_set_keys.clear()
