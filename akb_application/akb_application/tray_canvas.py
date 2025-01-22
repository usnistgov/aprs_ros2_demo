import math

from typing import Optional
from copy import copy

from rclpy.node import Node
from rclpy.qos import qos_profile_default

from aprs_vision.conversions import euler_from_quaternion
from aprs_interfaces.msg import Trays, Tray, SlotInfo
from akb_application.canvas_tool_tip import CanvasTooltip

import customtkinter as ctk
import tkinter as tk

from akb_application.settings import *

class Gear:
    def __init__(self, frame_name: str, c_x: int, c_y: int, radius: float):
        self.frame_name = frame_name
        self.c_x = c_x
        self.c_y = c_y
        self.radius = radius

class TrayCanvas(ctk.CTkCanvas):
    tray_corners_ = {
        Tray.SMALL_GEAR_TRAY: [
            (-0.08, 0.08),
            (0.08, 0.08),
            (0.08, -0.08),
            (-0.08, -0.08)
        ],
        Tray.MEDIUM_GEAR_TRAY: [
            (-0.098, 0.098),
            (0.098, 0.098),
            (0.098, -0.098),
            (-0.098, -0.098)
        ],
        Tray.LARGE_GEAR_TRAY: [
            (-0.105, 0.113),
            (0.105, 0.113),
            (0.105, -0.025),
            (-0.105, -0.025),
        ],
        Tray.M2L1_KIT_TRAY: [
            (-0.108, 0.043), # Top left corner
            (0.108, 0.043), # Top right corner
            (0.108, -0.054), # Bottom right corner
            (0.019, -0.128), # Very bottom right corner
            (-0.019, -0.128), # Very bottom left corner
            (-0.108, -0.054), # Bottom left corner
        ],
        Tray.S2L2_KIT_TRAY: [
            (-0.105, 0.113), # Top left corner
            (0.105, 0.113), # Top right corner
            (0.105, 0.0), # Middle right coner
            (0.067, -0.08), # Bottom right corner
            (-0.067,  -0.08), # Bottom left corner
            (-0.105, 0.0), # Middle left coner
        ],
        -1: [
            (-0.02, 0.02),
            (0.02, 0.02),
            (0.02, -0.02),
            (-0.02, -0.02)
        ]
    }

    gear_radii_ = {
        SlotInfo.SMALL: 0.032,
        SlotInfo.MEDIUM: 0.04,
        SlotInfo.LARGE: 0.05}
    
    tray_colors_ = {
        Tray.SMALL_GEAR_TRAY: "#3b3a3a",
        Tray.MEDIUM_GEAR_TRAY: "#3b3a3a",
        Tray.LARGE_GEAR_TRAY: "#3b3a3a",
        Tray.M2L1_KIT_TRAY: "#3b3a3a",
        Tray.S2L2_KIT_TRAY: "#3b3a3a"
    }

    fiducial_square_measurements = (0.04, 0.04)
    radius = 0.03
    
    def __init__(self, frame, node: Node, topic: str, width: int, height: int, img_height: int, select_mode_on: ctk.BooleanVar, selected_canvas_slot: ctk.StringVar):
        super().__init__(frame, bg="#C2C2C2", highlightthickness=0, height=height, width=width)

        self.node = node

        self.select_mode_on = select_mode_on
        self.selected_canvas_slot = selected_canvas_slot

        self.topic = topic

        self.gears_present: list[Gear] = []
        
        actual_height = (img_height / 30.0) * 0.0254
        self.conversion_factor = height / actual_height

        self.node.create_subscription(Trays, topic, self.trays_info_cb, qos_profile_default)

        self.trays_info: Optional[Trays] = None
        self.previous_trays_info: Optional[Trays] = None

        self.gear_map: dict[int, str] = {}

        self.update_rate = 1000 #ms
        
        self.update_canvas()

        if "teach" not in topic:
            self.bind('<Button-1>', self.canvas_clicked)

    def trays_info_cb(self, msg: Trays):
        self.trays_info = msg
    
    def update_canvas(self):
        if self.trays_info is None:
            pass
        elif self.trays_info != self.previous_trays_info:
            self.delete("all")

            self.gears_present.clear()

            self.previous_trays_info = copy(self.trays_info)
            all_trays: list[Tray] = self.trays_info.kit_trays + self.trays_info.part_trays # type: ignore
            for tray in all_trays:
                if tray.identifier not in TrayCanvas.tray_corners_.keys():
                    continue
                
                x = tray.tray_pose.pose.position.x
                y = tray.tray_pose.pose.position.y
                _, __, yaw = euler_from_quaternion(tray.tray_pose.pose.orientation)

                tray_rotation = -yaw

                self.draw_tray(tray.identifier, (x, y), tray_rotation, tray.name)

                for slot in tray.slots:
                    slot: SlotInfo
                    
                    x_off = slot.slot_pose.pose.position.x
                    y_off = slot.slot_pose.pose.position.y
                    
                    self.draw_gear(slot.size, (x, y), tray_rotation, x_off, y_off, slot.name, slot.occupied)
        
        self.after(self.update_rate, self.update_canvas)
    
    def draw_tray(self, identifier: int, center: tuple[float, float], rotation: float, tray_name: str):
        corners = TrayCanvas.tray_corners_[identifier]
        
        rounded_corners = self.round_points(corners)
        
        rotated_points = self.rotate_points(rounded_corners, rotation)
        
        # Handle canvas axis is defined from top left
        flipped_points = [(x, -y) for x, y in rotated_points]
        
        translated_points = self.translate_points(center, flipped_points)
        
        canvas_points = self.get_canvas_points(translated_points)
        
        tray_polygon = self.create_polygon(canvas_points, fill=DARK_GRAY, smooth=True, splinesteps=32)
        # tooltip = CanvasTooltip(self, tray_polygon, text=tray_name)
    
    def draw_gear(self, size: int, tray_center: tuple[float, float], tray_rotation: float, x_off: float, y_off: float, gear_name: str, occupied: bool):
        slot_center = [(x_off, y_off)]

        rotated_points = self.rotate_points(slot_center, tray_rotation)
        flipped_points = [(x, -y) for x, y in rotated_points]

        translated_points = self.translate_points(tray_center, flipped_points)
        
        canvas_points = self.get_canvas_points(translated_points)

        cx = canvas_points[0]
        cy = canvas_points[1]

        radius = self.gear_radii_[size] * self.conversion_factor

        gear_oval = self.create_oval(cx - radius, cy - radius, cx + radius, cy + radius, fill=(GREEN if occupied else GRAY), activefill=(DARK_GREEN if occupied else GRAY))
        
        self.gear_map[gear_oval] = gear_name

        self.gears_present.append(Gear(gear_name, cx, cy, radius))

        self.tag_bind(gear_oval, sequence="<Enter>", func=self.slot_press_cb)
        
        # tooltip = CanvasTooltip(self, gear_oval, text=gear_name)

    def round_points(self, points: list[tuple[float, float]]) -> list[tuple[float, float]]:
        rounded_points = []
        for i in range(len(points)):
            p1 = points[i]
            p2 = points[(i+1)%len(points)]
            angle = math.atan2((p2[1] - p1[1]), (p2[0] - p1[0]))
            
            # Add original point
            rounded_points.append(p1)
            
            # Add two points between i and i+1 point 
            p3 = (p1[0] + self.radius * math.cos(angle), p1[1] + self.radius * math.sin(angle))
            p4 = (p2[0] - self.radius * math.cos(angle), p2[1] - self.radius * math.sin(angle))
                
            rounded_points.append(p3)
            rounded_points.append(p3)
            rounded_points.append(p4)
            rounded_points.append(p4)

        return rounded_points
    
    def translate_points(self, center: tuple[float, float], points) -> list[tuple[float, float]]:
        return [(p[0] + center[0], p[1] + center[1]) for p in points]
    
    def rotate_points(self, points: list[tuple[float, float]], angle: float) -> list[tuple[float, float]]:
        rotated_points = []
        for x,y in points:
            x_r = x * math.cos(angle) - y * math.sin(angle)
            y_r = x * math.sin(angle) + y * math.cos(angle)
            rotated_points.append((x_r, y_r))
        
        return rotated_points

    def get_canvas_points(self, points: list[tuple[float, float]]) -> list[float]:
        canvas_points = []
        for x, y in points:
            canvas_points.append(x*self.conversion_factor)
            canvas_points.append(y*self.conversion_factor)
        
        return canvas_points
    
    def slot_press_cb(self, event: tk.Event):
        item = self.find_closest(event.x, event.y)
        try:
            print(self.gear_map[item[0]])
        except KeyError:
            pass
    
    def dist(self, p_1, p_2):
        return math.sqrt(sum([(p_1[i]-p_2[i])**2 for i in range(len(p_1))]))
    
    def canvas_clicked(self, event: tk.Event):
        if not self.select_mode_on.get():
            return
        else:
            area = ""
            for key, topic in TRAYS_INFO_NAMES.items():
                if topic == self.topic:
                    area = key
            for gear in self.gears_present:
                if self.dist((event.x, event.y), (gear.c_x, gear.c_y)) <= gear.radius:
                    self.selected_canvas_slot.set(area+"|"+gear.frame_name)