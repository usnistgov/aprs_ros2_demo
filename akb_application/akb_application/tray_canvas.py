import math

from typing import Optional
from functools import partial

from rclpy.node import Node
from rclpy.task import Future
from rclpy.qos import qos_profile_default

from aprs_vision.conversions import euler_from_quaternion
from aprs_interfaces.msg import Trays, Tray, SlotInfo
from aprs_interfaces.srv import Pick, Place
from akb_application.canvas_tool_tip import CanvasTooltip
from sensor_msgs.msg import JointState


from customtkinter import CTkCanvas

class Gear:
    def __init__(self, frame_name: str, c_x: int, c_y: int, radius: float, occupied: bool):
        self.frame_name = frame_name
        self.c_x = c_x
        self.c_y = c_y
        self.radius = radius
        self.occupied = occupied

class TrayCanvas(CTkCanvas):
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
    
    def __init__(self, frame, node: Node, topic: str, width: int, height: int, img_height: int, robot: Optional[str]=None):
        super().__init__(frame, bd = 0, highlightthickness=0, height=height, width=width)

        self.node = node

        self.robot: Optional[str] = None
        
        actual_height = (img_height / 30.0) * 0.0254
        self.conversion_factor = height / actual_height

        self.node.create_subscription(Trays, topic, self.trays_info_cb, qos_profile_default)

        self.trays_info: Optional[Trays] = None

        self.update_rate = 1000 #ms

        if self.robot is not None:
            self.node.create_subscription(JointState, f"{self.robot}/joint_states", self.joint_states_cb, qos_profile_default)
            self.pick_client = self.node.create_client(Pick, f"{robot}/pick_from_slot")
            self.place_client = self.node.create_client(Place, f"{robot}/place_in_slot")
            self.actively_picking = False
            self.actively_placing = False
            self.gripper_open = False

            self.held_gear_type: Optional[str] = None
            self.gears_present: list[Gear] = []
            self.bind('<Button-1>', self.tray_canvas_clicked)
        
        self.update_canvas()

    def trays_info_cb(self, msg: Trays):
        self.trays_info = msg
    
    def joint_states_cb(self, msg: JointState):
        for i in range(len(msg.name)):
            if "finger" in msg.name[i]:
                self.gripper_open = msg.position[i] != 0
                return
    
    def update_canvas(self):
        self.delete("all")

        if self.trays_info is not None:
            all_trays: list[Tray] = self.trays_info.kit_trays + self.trays_info.part_trays # type: ignore
            self.gears_present.clear()
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
                    
                    # if not slot.occupied:
                    #     continue
                    
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
        
        tray_polygon = self.create_polygon(canvas_points, fill=TrayCanvas.tray_colors_[identifier], smooth=True, splinesteps=32)
        tooltip = CanvasTooltip(self, tray_polygon, text=tray_name)
    
    def draw_gear(self, size: int, tray_center: tuple[float, float], tray_rotation: float, x_off: float, y_off: float, gear_name: str, occupied: bool):
        slot_center = [(x_off, y_off)]

        rotated_points = self.rotate_points(slot_center, tray_rotation)
        flipped_points = [(x, -y) for x, y in rotated_points]

        translated_points = self.translate_points(tray_center, flipped_points)
        
        canvas_points = self.get_canvas_points(translated_points)

        cx = canvas_points[0]
        cy = canvas_points[1]

        radius = self.gear_radii_[size] * self.conversion_factor

        gear_oval = self.create_oval(cx - radius, cy - radius, cx + radius, cy + radius, fill=("#40bd42" if occupied else "#616161"))
        self.gears_present.append(Gear(gear_name, cx, cy, radius, occupied))
        tooltip = CanvasTooltip(self, gear_oval, text=gear_name)

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
    
    def dist(self, p_1, p_2):
        return math.sqrt(sum([(p_1[i]-p_2[i])**2 for i in range(len(p_1))]))
    
    def tray_canvas_clicked(self, event):
        if self.robot is None:
            return
        
        if self.gripper_open:
            for gear in self.gears_present:
                if self.dist((event.x, event.y), (gear.c_x, gear.c_y)) < gear.radius:
                    if not gear.occupied:
                        self.node.get_logger().error("Tried to pick from unoccupied slot")
                        return
                    self.pick_gear(gear.frame_name)
        
        else:
            for gear in self.gears_present:
                if self.dist((event.x, event.y), (gear.c_x, gear.c_y)) < gear.radius:
                    if not gear.occupied:
                        self.node.get_logger().error("Tried to place in occupied slot")
                        return
                    self.place(gear.frame_name)


    
    def pick_gear(self, frame_name):
        if not self.pick_client.service_is_ready():
            self.node.get_logger().error(f"Pick service not ready for {self.robot}")
            return
        
        if self.actively_picking or self.actively_placing:
            self.node.get_logger().error(f"Already picking or placing with {self.robot}")
            return
        
        pick_request = Pick.Request()
        pick_request.frame_name = frame_name

        self.actively_picking = True

        future = self.pick_client.call_async(pick_request)

        future.add_done_callback(partial(self.pick_gear_done_cb, frame_name))

    def pick_gear_done_cb(self, frame_name: str, future: Future):
        result: Pick.Response = future.result()
        self.actively_picking = False
        if result.success:
            self.node.get_logger().info(f"Successfully picked gear with {self.robot} on frame {frame_name}")
            if "small" in frame_name or "sg" in frame_name:
                self.held_gear_type = "small"
            elif "medium" in frame_name or "mg" in frame_name:
                self.held_gear_type = "medium"
            else:
                self.held_gear_type = "large"
        else:
            self.node.get_logger().error(f"{self.robot} could not pick from frame {frame_name}")
    
    def place_gear(self, frame_name):
        if not self.pick_client.service_is_ready():
            self.node.get_logger().error(f"Place service not ready for {self.robot}")
            return
        
        if self.actively_picking or self.actively_placing:
            self.node.get_logger().error(f"Already picking or placing with {self.robot}")
            return
        
        if self.held_gear_type == "small" and "small" not in frame_name and "sg" not in frame_name:
            self.node.get_logger().error(f"{self.robot} is holding a small gear. Cannot place in not small slot.")
            return
        elif self.held_gear_type == "medium" and "medium" not in frame_name and "mg" not in frame_name:
            self.node.get_logger().error(f"{self.robot} is holding a medium gear. Cannot place in not medium slot.")
            return
        elif self.held_gear_type == "large" and "large" not in frame_name and "lg" not in frame_name:
            self.node.get_logger().error(f"{self.robot} is holding a large gear. Cannot place in not large slot.")
            return
        
        place_request = Place.Request()
        place_request.frame_name = frame_name

        self.actively_placing = True

        future = self.place_client.call_async(place_request)

        future.add_done_callback(partial(self.place_gear_done_cb, frame_name))

    def place_gear_done_cb(self, frame_name: str, future: Future):
        result: Place.Response = future.result()
        self.actively_placing = False
        if result.success:
            self.node.get_logger().info(f"Successfully place gear with {self.robot} on frame {frame_name}")
            self.held_gear_type = None
        else:
            self.node.get_logger().error(f"{self.robot} could not place from frame {frame_name}")