import random
from functools import partial
from typing import Optional

import customtkinter as ctk
from tkinter import DISABLED, NORMAL
from tkinter.ttk import Separator

from rclpy.node import Node
from rclpy.task import Future

from aprs_interfaces.srv import PneumaticGripperControl, MoveToNamedPose, Pick, Place
from aprs_interfaces.msg import Trays, Tray, SlotInfo

from akb_application.settings import *

class ControlPanel(ctk.CTkFrame):
    def __init__(self, frame, node: Node, row: int, col: int, select_mode_on: ctk.BooleanVar, selected_canvas_slot: ctk.StringVar):
        super().__init__(frame, fg_color="transparent")

        self.node = node
        
        self.grid(row=row, column=col, sticky='N', padx=20, pady=20)
        
        self.robot_selection = ctk.StringVar(value="fanuc")

        self.open_gripper_pressed = ctk.BooleanVar(value=False)
        self.open_gripper_pressed.trace_add('write', self.update_held_gear)

        self.select_mode_on = select_mode_on
        self.selected_canvas_slot = selected_canvas_slot
        
        # Widgets
        ctk.CTkLabel(self, text="Control Panel", font=ctk.CTkFont(FONT_FAMILY, TITLE_FONT_SIZE, weight=TITLE_FONT_WEIGHT)).grid(row=0, column=0, columnspan=4)
        Separator(self, orient='horizontal').grid(row=1, column=0, columnspan=2, sticky="WE")
        RobotSelectionSwitch(self, row=2, column=0, robot=self.robot_selection)
        ActuateGripperButton(self, node, row=3, column=0, enable=False, robot=self.robot_selection, open_gripper_pressed=self.open_gripper_pressed)
        ActuateGripperButton(self, node, row=4, column=0, enable=True, robot=self.robot_selection)
        MoveToPoseFrame(self, node, row=5, column=0, robot=self.robot_selection)
        self.pick_place_frame = PickPlaceFrame(self, node, row=3, column=1, robot=self.robot_selection, select_mode_on=self.select_mode_on, selected_canvas_slot=self.selected_canvas_slot)
        
    def update_held_gear(self, *args):
        if self.open_gripper_pressed.get() == True:
            self.pick_place_frame.held_part[self.robot_selection.get()].set("None")
            self.open_gripper_pressed.set(False)

class RobotSelectionSwitch(ctk.CTkFrame):
    def __init__(self, frame, row: int, column: int, robot: ctk.StringVar):
        super().__init__(frame, fg_color="transparent")
        
        self.robot = robot
        
        self.fanuc_label = ctk.CTkLabel(self, text='Fanuc', font=ctk.CTkFont(FONT_FAMILY, SWITCH_FONT_SIZE, weight=SWITCH_FONT_WEIGHT), text_color=PURPLE)
        self.motoman_label = ctk.CTkLabel(self, text='Motoman', font=ctk.CTkFont(FONT_FAMILY, SWITCH_FONT_SIZE, weight=SWITCH_FONT_WEIGHT), text_color=LIGHT_GRAY)
        
        switch = ctk.CTkSwitch(
            self,
            text="",
            onvalue='motoman',
            offvalue='fanuc',
            variable=self.robot,
            width=36,
            progress_color=PURPLE,
            fg_color=PURPLE,
            button_color=DARK_PURPLE,
            button_hover_color=DARK_PURPLE,
            command=self.switch_command_cb)
        
        self.grid(row=row, column=column, columnspan=2, pady=5)
        
        self.fanuc_label.grid(row=0, column=0)
        switch.grid(row=0, column=1, padx=10)
        self.motoman_label.grid(row=0, column=2)
    
    def switch_command_cb(self, *args):
        if self.robot.get() == "fanuc":
            self.fanuc_label.configure(text_color=PURPLE)
            self.motoman_label.configure(text_color=LIGHT_GRAY)
        elif self.robot.get() == "motoman":
            self.fanuc_label.configure(text_color=LIGHT_GRAY)
            self.motoman_label.configure(text_color=PURPLE)
            
class ActuateGripperButton(ctk.CTkButton):
    def __init__(self, frame, node: Node, row: int, column: int, enable: bool, robot: ctk.StringVar, open_gripper_pressed: Optional[ctk.BooleanVar]=None):
        super().__init__(
            frame,
            text=f'{"Close" if enable else "Open"} Gripper',
            command=self.command_cb,
            font=ctk.CTkFont(FONT_FAMILY, BUTTON_FONT_SIZE, weight=BUTTON_FONT_WEIGHT),
            fg_color=PURPLE,
            hover_color=DARK_PURPLE
        )
        
        self.node = node
        self.enable = enable
        self.robot = robot

        self.open_gripper_pressed: Optional[ctk.BooleanVar] = open_gripper_pressed
        
        self.clients = {k: self.node.create_client(PneumaticGripperControl, v) for k,v in ACTUATE_GRIPPER_NAMES.items()}
        
        self.grid(row=row, column=column, padx=10, pady=5, sticky="WE")
        
        self.reset()
    
    def command_cb(self, *args):
        self.configure(fg_color=YELLOW, state=DISABLED)
        
        print(f'{"Closing" if self.enable else "Opening"} the {self.robot.get().lower()}\'s gripper')
        
        gripper_request = PneumaticGripperControl.Request()
        gripper_request.enable = self.enable
        
        if not self.clients[self.robot.get()].service_is_ready():
            self.node.get_logger().error(f"Gripper service not ready for {self.robot.get()}")
            self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.after(3000, self.reset)
            return

        future = self.clients[self.robot.get()].call_async(gripper_request)

        future.add_done_callback(self.srv_done_cb)
        
    def srv_done_cb(self, future: Future):
        result: PneumaticGripperControl.Response = future.result() # type: ignore
        
        if result.success:
            self.configure(fg_color=GREEN, hover_color=DARK_GREEN, state=NORMAL)
            if not self.enable and self.open_gripper_pressed is not None:
                self.open_gripper_pressed.set(True)
        else:
            self.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.node.get_logger().error(f'Unable to {"close" if self.enable else "open"} gripper for {self.robot.get()}')
        
        self.after(3000, self.reset)
        
    def reset(self, *args):
        self.configure(fg_color=PURPLE, hover_color=DARK_PURPLE, state=NORMAL)
        
    
class MoveToPoseFrame(ctk.CTkFrame):
    def __init__(self, frame, node: Node, row: int, column: int, robot: ctk.StringVar):
        super().__init__(frame, fg_color="transparent")
        
        self.robot = robot
        self.node = node
        
        self.selected_pose = ctk.StringVar()
        
        self.selected_pose.trace_add("write", self.pose_selection_trace_cb)
        self.robot.trace_add("write", self.robot_trace_cb)
        
        self.button = ctk.CTkButton(
            self,
            text=f'Move To Pose',
            command=self.button_command_cb,
            state=DISABLED,
            font=ctk.CTkFont(FONT_FAMILY, BUTTON_FONT_SIZE, weight=BUTTON_FONT_WEIGHT),
            fg_color=PURPLE,
            hover_color=DARK_PURPLE
        )
        
        self.menu = ctk.CTkOptionMenu(
            self, 
            variable=self.selected_pose, 
            values=NAMED_POSES[self.robot.get()], 
            font=ctk.CTkFont(FONT_FAMILY, DROP_DOWN_FONT_SIZE, weight=DROP_DOWN_FONT_WEIGHT), 
            dropdown_font=ctk.CTkFont(FONT_FAMILY, DROP_DOWN_FONT_SIZE, weight=DROP_DOWN_FONT_WEIGHT),
            fg_color=PURPLE,
            button_color=PURPLE,
            button_hover_color=DARK_PURPLE,
            anchor="center"
        )
        
        self.clients = {k: self.node.create_client(MoveToNamedPose, v) for k,v in MOVE_TO_POSE_NAMES.items()}       
        
        self.grid(row=row, column=column, pady=5)
        self.menu.grid(row=0, column=0, padx=10)
        self.button.grid(row=0, column=1, padx=10)
        
    def button_command_cb(self, *args):
        if self.selected_pose.get() is None:
            print("No pose selected")
            return
        
        print(f"Moving the {self.robot.get().lower()} to {self.selected_pose.get()}")
        
        self.button.configure(fg_color=YELLOW, state=DISABLED)
        
        request = MoveToNamedPose.Request()
        request.name = self.selected_pose.get()
        
        if not self.clients[self.robot.get()].service_is_ready():
            self.node.get_logger().error(f"Gripper service not ready for {self.robot.get()}")
            self.button.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.after(3000, self.reset)
            return

        future = self.clients[self.robot.get()].call_async(request)

        future.add_done_callback(self.srv_done_cb)
    
    def srv_done_cb(self, future: Future):
        result: MoveToNamedPose.Response = future.result() # type: ignore
        
        if result.success:
            self.button.configure(fg_color=GREEN, hover_color=DARK_GREEN, state=NORMAL)
        else:
            self.button.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.node.get_logger().error(f'Unable to move the {self.robot.get()} to {self.selected_pose.get()}')
        
        self.after(3000, self.reset)
    
    def pose_selection_trace_cb(self, *args):
        if self.selected_pose.get() == "":
            self.button.configure(state=DISABLED)
        else:
            self.button.configure(state=NORMAL)
            
    def robot_trace_cb(self, *args):
        self.menu.configure(values=NAMED_POSES[self.robot.get()])
        self.selected_pose.set("")
        
    def reset(self, *args):
        self.button.configure(fg_color=PURPLE, hover_color=DARK_PURPLE, state=NORMAL)
        
class PickPlaceFrame(ctk.CTkFrame):
    def __init__(self, frame, node: Node, row: int, column: int, robot: ctk.StringVar, select_mode_on: ctk.BooleanVar, selected_canvas_slot: ctk.StringVar):
        super().__init__(frame, fg_color="transparent")
        
        self.robot = robot
        self.node = node

        self.select_mode_on = select_mode_on
        self.selected_canvas_slot = selected_canvas_slot
        
        self.pick_slot = ctk.StringVar()
        self.place_slot = ctk.StringVar()

        self.pick_slot.trace_add("write", self.activate_deactivate_pick_button)
        self.place_slot.trace_add("write", self.activate_deactivate_place_button)

        self.held_part = {robot: ctk.StringVar(value="None") for robot in ["fanuc", "motoman"]}

        self.current_available_slots = {areas: [] for areas in TRAYS_INFO_NAMES.keys()}
        self.reachable_occupied: dict[str: list[SlotInfo]] = {robot: [] for robot in REACHABLE_AREAS.keys()} # type:ignore
        self.reachable_unoccupied: dict[str: dict[str: list[SlotInfo]]] = {robot: {} for robot in REACHABLE_AREAS.keys()} # type:ignore
        
        self.robot.trace_add("write", self.robot_trace_cb)
        self.held_part["fanuc"].trace_add("write", self.held_part_trace_cb)
        self.held_part["motoman"].trace_add("write", self.held_part_trace_cb)
        
        self.most_recent_tray_infos = {k: Trays() for k in TRAYS_INFO_NAMES.keys()}
        for area, topic in TRAYS_INFO_NAMES.items():
            self.node.create_subscription(Trays, topic, partial(self.trays_cb, area), 10)

        self.pick_button = ctk.CTkButton(
            self,
            text=f'Pick',
            command=self.pick_button_command_cb,
            state=DISABLED,
            font=ctk.CTkFont(FONT_FAMILY, BUTTON_FONT_SIZE, weight=BUTTON_FONT_WEIGHT),
            fg_color=PURPLE,
            hover_color=DARK_PURPLE,
        )
        
        self.pick_selection_menu = ctk.CTkOptionMenu(
            self, 
            variable=self.pick_slot, 
            values=[], 
            font=ctk.CTkFont(FONT_FAMILY, DROP_DOWN_FONT_SIZE, weight=DROP_DOWN_FONT_WEIGHT), 
            dropdown_font=ctk.CTkFont(FONT_FAMILY, DROP_DOWN_FONT_SIZE, weight=DROP_DOWN_FONT_WEIGHT),
            fg_color=PURPLE,
            button_color=PURPLE,
            button_hover_color=DARK_PURPLE,
            anchor="center",
            width=250
        )
        
        self.place_button = ctk.CTkButton(
            self,
            text=f'Place',
            command=self.place_button_command_cb,
            state=DISABLED,
            font=ctk.CTkFont(FONT_FAMILY, BUTTON_FONT_SIZE, weight=BUTTON_FONT_WEIGHT),
            fg_color=PURPLE,
            hover_color=DARK_PURPLE
        )
        
        self.place_selection_menu = ctk.CTkOptionMenu(
            self, 
            variable=self.place_slot, 
            values=[], 
            font=ctk.CTkFont(FONT_FAMILY, DROP_DOWN_FONT_SIZE, weight=DROP_DOWN_FONT_WEIGHT), 
            dropdown_font=ctk.CTkFont(FONT_FAMILY, DROP_DOWN_FONT_SIZE, weight=DROP_DOWN_FONT_WEIGHT),
            fg_color=PURPLE,
            button_color=PURPLE,
            button_hover_color=DARK_PURPLE,
            anchor="center",
            width=250
        )
        
        self.holding_label = ctk.CTkLabel(
            self,
            text=f'Holding: {self.held_part[self.robot.get()].get()}',
            font=ctk.CTkFont(FONT_FAMILY, TITLE_FONT_SIZE, weight=TITLE_FONT_WEIGHT),
            text_color=GRAY
        )

        # self.select_switch = ctk.CTkSwitch(
        #     self,
        #     text="Canvas select mode: " + ("ON" if self.select_mode_on.get() else "OFF"),
        #     text_color=GREEN if self.select_mode_on.get() else RED,
        #     onvalue=True,
        #     offvalue=False,
        #     variable=self.select_mode_on,
        #     width=36,
        #     progress_color=PURPLE,
        #     fg_color=PURPLE,
        #     button_color=DARK_PURPLE,
        #     button_hover_color=DARK_PURPLE,
        #     command=self.select_switch_switched
        # )
        
        self.pick_clients = {k: self.node.create_client(Pick, v) for k,v in PICK_NAMES.items()}
        self.place_clients = {k: self.node.create_client(Place, v) for k,v in PLACE_NAMES.items()}        
        
        self.grid(row=row, column=column, rowspan=3, sticky="WE")
        
        self.pick_selection_menu.grid(row=0, column=0, padx=10, pady=5)
        self.pick_button.grid(row=0, column=1, padx=10, pady=5)
        
        self.place_selection_menu.grid(row=1, column=0, padx=10, pady=5)
        self.place_button.grid(row=1, column=1, padx=10, pady=5)
                                  
        self.holding_label.grid(row=2, column=0, columnspan=2, padx=10, pady=5)

        # self.select_switch.grid(row=3, column=0, columnspan=2, pady=10)

        # self.pick_slot.trace_add('write', self.check_select_mode)
        # self.place_slot.trace_add('write', self.check_select_mode)

        # self.selected_canvas_slot.trace_add('write', self.select_slot_clicked)

        self.after(1000, self.update_pick_place_options)
    
    def select_switch_switched(self):
        self.select_switch.configure(
            text="Canvas select mode " + ("ON" if self.select_mode_on.get() else "OFF"),
            text_color=GREEN if self.select_mode_on.get() else RED
        )
    
    def activate_deactivate_pick_button(self, *args):
        if self.pick_slot.get()=="":
            self.pick_button.configure(state=DISABLED)
        else:
            self.pick_button.configure(state=NORMAL)

    def activate_deactivate_place_button(self, *args):
        if self.place_slot.get()=="":
            self.place_button.configure(state=DISABLED)
        else:
            self.place_button.configure(state=NORMAL)
    
    def trays_cb(self, detection_area, msg: Trays):
        self.most_recent_tray_infos[detection_area] = msg
    
    def update_pick_place_options(self, *args):
        self.update_options_for_robot("fanuc")
        self.update_options_for_robot("motoman")
        self.after(1000, self.update_pick_place_options)
    
    def update_options_for_robot(self, robot: str):
        all_reachable_trays = sum([self.most_recent_tray_infos[detection_area].kit_trays + self.most_recent_tray_infos[detection_area].part_trays for detection_area in REACHABLE_AREAS[robot]], [])# type:ignore
        current_available_slots = sum([tray.slots for tray in all_reachable_trays], [])
        self.reachable_occupied[robot] = sorted([slot.name for slot in current_available_slots if slot.occupied])
        unoccupied_slots: list[SlotInfo] = [slot for slot in current_available_slots if not slot.occupied]

        sizes = ["small", "medium", "large"]
        self.reachable_unoccupied[robot] = {size: [] for size in sizes}
        for slot in unoccupied_slots:
            self.reachable_unoccupied[robot][sizes[slot.size-1]].append(slot.name)
        for size in sizes:
            self.reachable_unoccupied[robot][size] = sorted(self.reachable_unoccupied[robot][size])

        if self.robot.get() == robot:
            if self.held_part[robot].get() == "None":
                if sorted(self.pick_selection_menu.cget("values")) != sorted(self.reachable_occupied[robot]):
                    self.pick_selection_menu.configure(values=self.reachable_occupied[robot])
                    self.place_selection_menu.configure(values=[])
                    if self.place_slot.get() != "":
                        self.place_slot.set("")
            else:
                gear_size = self.held_part[robot].get().lower().split(" ")[0]
                if sorted(self.place_selection_menu.cget("values")) != sorted(self.reachable_unoccupied[robot][gear_size]):
                    self.place_selection_menu.configure(values=self.reachable_unoccupied[robot][gear_size])
                    self.pick_selection_menu.configure(values=[])
                    if self.pick_slot.get() != "":
                        self.pick_slot.set("")
    
    def request_pick_srv(self, slot_name):
        self.pick_button.configure(fg_color=YELLOW, state=DISABLED)
        
        request = Pick.Request()
        request.frame_name = slot_name
        
        if not self.pick_clients[self.robot.get()].service_is_ready():
            self.node.get_logger().error(f"Pick service not ready for {self.robot.get()}")
            self.pick_button.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.after(3000, self.reset_pick_button)
            return

        future = self.pick_clients[self.robot.get()].call_async(request)

        future.add_done_callback(partial(self.pick_srv_done_cb, slot_name))
    
    def request_place_srv(self, slot_name):
        self.place_button.configure(fg_color=YELLOW, state=DISABLED)

        request = Place.Request()
        request.frame_name = slot_name

        if not self.place_clients[self.robot.get()].service_is_ready():
            self.node.get_logger().error(f"Place service is not ready for {self.robot.get()}")
            self.pick_button.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.after(3000, self.reset_place_button)
            return
        future = self.place_clients[self.robot.get()].call_async(request)

        future.add_done_callback(self.place_srv_done_cb)
    
    def pick_button_command_cb(self, *args):
        if self.pick_slot.get() is None or self.pick_slot.get() == "":
            print("No slot selected")
            return
        
        self.request_pick_srv(self.pick_slot.get())
        
    def place_button_command_cb(self, *args):
        if self.place_slot.get() is None or self.place_slot.get() == "":
            print("No slot selected")
            return
        
        self.request_place_srv(self.place_slot.get())
    
    def pick_srv_done_cb(self, slot_name, future: Future):
        result: Pick.Response = future.result() # type: ignore
        
        if result.success:
            self.pick_button.configure(state=DISABLED)
            self.place_button.configure(state=NORMAL)
            self.pick_button.configure(fg_color=GREEN, hover_color=DARK_GREEN)
            if "small" in slot_name or "sg" in slot_name:
                self.held_part[self.robot.get()].set('Small Gear')
            elif "medium" in slot_name or "mg" in slot_name:
                self.held_part[self.robot.get()].set('Medium Gear')
            else:
                self.held_part[self.robot.get()].set('Large Gear')
        else:
            self.pick_button.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.node.get_logger().error(f'Unable to pick {slot_name} using {self.robot.get()}. Status: {result.status}')
        self.after(3000, self.reset_pick_button)
            
    def place_srv_done_cb(self, future: Future):
        result: Place.Response = future.result() # type:ignore

        if result.success:
            self.held_part[self.robot.get()].set("None")
            self.place_button.configure(state=DISABLED)
            self.pick_button.configure(state=NORMAL)
            self.place_button.configure(fg_color=GREEN, hover_color=DARK_GREEN)
        else:
            self.place_button.configure(fg_color=RED, hover_color=DARK_RED, state=NORMAL)
            self.node.get_logger().error(f'Unable to place gear')
        self.after(3000, self.reset_place_button)
                    
    def update_held_label_and_menu(self):
        if self.held_part[self.robot.get()].get().lower().startswith("small"):
            text_color = YELLOW
        elif self.held_part[self.robot.get()].get().lower().startswith("medium"):
            text_color = BLUE
        elif self.held_part[self.robot.get()].get().lower().startswith("large"):
            text_color = GREEN
        else:
            text_color = GRAY
            
        self.holding_label.configure(
            text=f'Holding: {self.held_part[self.robot.get()].get()}',
            text_color=text_color
        )

        if self.held_part[self.robot.get()].get() == "None":
            self.pick_selection_menu.configure(values=["select"]+self.reachable_occupied[self.robot.get()])
            self.place_selection_menu.configure(values=[])
            self.pick_slot.set("")
            self.place_slot.set("")
        else:
            holding_size = self.held_part[self.robot.get()].get().lower().split(" ")[0]
            self.place_selection_menu.configure(values=["select"]+self.reachable_unoccupied[self.robot.get()][holding_size])
            self.pick_selection_menu.configure(values=[])
            self.pick_slot.set("")
            self.place_slot.set("")
    
    def robot_trace_cb(self, *args):
        self.reset_pick_button()
        self.reset_place_button()
        self.update_held_label_and_menu()
        
    def held_part_trace_cb(self, *args):
        self.update_held_label_and_menu()
        
    def reset_pick_button(self, *args):
        self.pick_button.configure(fg_color=PURPLE, hover_color=DARK_PURPLE)
        
    def reset_place_button(self, *args):
        self.place_button.configure(fg_color=PURPLE, hover_color=DARK_PURPLE)
    
    # def check_select_mode(self, *args):
    #     if [self.pick_slot.get(), self.place_slot.get()].count('')==2:
    #         return
    #     if 'select' in [self.pick_slot.get(), self.place_slot.get()]:
    #         self.select_mode_on.set(True)
    #     else:
    #         self.select_mode_on.set(False)
    
    # def select_slot_clicked(self, *args):
    #     if not self.select_mode_on.get() or self.selected_canvas_slot.get() == "":
    #         return
        
    #     held_part = self.held_part[self.robot.get()].get()
    #     split_info = self.selected_canvas_slot.get().split("|")
    #     area = split_info[0]
    #     slot = split_info[1]

    #     if area not in REACHABLE_AREAS[self.robot.get()]:
    #         return

    #     if held_part == "None": # pick
    #         if slot in self.reachable_occupied[self.robot.get()]:
    #             self.request_pick_srv(slot)
    #     else: # place
    #         held_part_size = held_part.split(" ")[0].lower()
    #         if slot in self.reachable_unoccupied[self.robot.get()][held_part_size]:
    #             self.request_place_srv(slot)
        
    #     self.selected_canvas_slot.set("")