import customtkinter as ctk
from rclpy.node import Node
from tkinter import ttk
import tkinter as tk
from PIL import Image, ImageTk
from sensor_msgs.msg import Image as ImageMsg
import rclpy
from rclpy.qos import qos_profile_default
from cv_bridge import CvBridge
from typing import Optional
from cv2.typing import MatLike
from functools import partial
from time import time
from queue import Queue
from example_interfaces.srv import Trigger

FRAMEWIDTH=1200
FRAMEHEIGHT=750
FAR_LEFT_COLUMN = 1
LEFT_COLUMN=2
MIDDLE_COLUMN = 3
RIGHT_COLUMN = 4
FAR_RIGHT_COLUMN = 5

class DemoControlWindow(Node):
    def __init__(self):
        super().__init__("aprs_demo_gui")
        
        ctk.set_appearance_mode("light")  # Modes: system (default), light, dark
        ctk.set_default_color_theme("blue")  # Themes: blue (default), dark-blue, green
        
        self.main_window = ctk.CTk()
        
        self.main_window.title("Demo Control")

        self.main_window.grid_rowconfigure(0, weight=1)
        self.main_window.grid_rowconfigure(100, weight=1)
        self.main_window.grid_columnconfigure(0, weight=1)
        self.main_window.grid_columnconfigure(6, weight=1)
        
        s = ttk.Style()
        s.theme_use('clam')
        s.configure('TNotebook', font='Arial Bold')
        
        self.main_window.geometry("1200x950")
        
        # VISION VARIABLES
        self.bridge = CvBridge()
        self.fanuc_image_update_var = ctk.IntVar(value=1)
        self.teach_image_update_var = ctk.IntVar(value=1)
        self.most_recent_fanuc_time = -100
        self.most_recent_teach_time = -100
        self.fanuc_image = None
        self.teach_image = None
        
        self.notebook = ttk.Notebook(self.main_window)
        
        self.vision_frame = ctk.CTkFrame(self.notebook, width=FRAMEWIDTH, height=FRAMEHEIGHT)
        self.vision_frame.pack(fill='both', expand=True)
        self.notebook.add(self.vision_frame, text="Run Demo")
        self.setup_vision_tab()
        
        self.notebook.grid(pady=10, column=LEFT_COLUMN, columnspan = 2, sticky=tk.E+tk.W+tk.N+tk.S)
        
        # ROS2 SUBSCRIBERS
        self.fanuc_image_subscriber = self.create_subscription(
            ImageMsg, 
            '/fanuc_vision/raw_image',
            self.fanuc_image_cb,
            qos_profile_default)
        
        self.teach_table_image_subscriber = self.create_subscription(
            ImageMsg, 
            '/teach_table_vision/raw_image',
            self.teach_table_image_cb,
            qos_profile_default)
        
        # ROS2 SERVICE CLIENTS
        self.locate_fanuc_trays_client = self.create_client(
            Trigger,
            "/fanuc_vision/locate_trays"
        )
        self.update_fanuc_slots_client = self.create_client(
            Trigger,
            "/fanuc_vision/update_slots"
        )
        self.locate_teach_table_trays_client = self.create_client(
            Trigger,
            "/teach_table_vision/locate_trays"
        )
        self.update_teach_table_slots_client = self.create_client(
            Trigger,
            "/teach_table_vision/update_slots"
        )
        
        vision_connection_timer = self.create_timer(0.5, self.vision_connection_cb)
        
    # VISION FUNCTIONS
    def setup_vision_tab(self):
        self.vision_frame.grid_rowconfigure(0, weight=1)
        self.vision_frame.grid_rowconfigure(100, weight=1)
        self.vision_frame.grid_columnconfigure(0, weight=1)
        self.vision_frame.grid_columnconfigure(10, weight=1)
        
        # Headers
        fanuc_header_label = ctk.CTkLabel(self.vision_frame, text="FANUC")
        fanuc_header_label.grid(column = LEFT_COLUMN, row=1)
        teach_table_header_label = ctk.CTkLabel(self.vision_frame, text="TEACH TABLE")
        teach_table_header_label.grid(column = RIGHT_COLUMN, row=1)
        
        # Status Labels
        fanuc_status_header_label = ctk.CTkLabel(self.vision_frame, text="Status:")
        fanuc_status_header_label.grid(column = LEFT_COLUMN, row=2)
        self.fanuc_status_label = ctk.CTkLabel(self.vision_frame, text="Not Connected", text_color = "red")
        self.fanuc_status_label.grid(column=LEFT_COLUMN, row=3)
        
        teach_status_header_label = ctk.CTkLabel(self.vision_frame, text="Status:")
        teach_status_header_label.grid(column = RIGHT_COLUMN, row=2)
        self.teach_table_status_label = ctk.CTkLabel(self.vision_frame, text="Not Connected", text_color = "red")
        self.teach_table_status_label.grid(column=RIGHT_COLUMN, row=3)
        
        # Image Labels
        self.fanuc_image_label = ctk.CTkLabel(self.vision_frame, text="")
        self.fanuc_image_label.grid(column=LEFT_COLUMN, row=4)
        
        self.teach_table_image_label = ctk.CTkLabel(self.vision_frame, text="")
        self.teach_table_image_label.grid(column=RIGHT_COLUMN, row=4)
        
        # Service buttons
        self.locate_trays_fanuc_vision_button = ctk.CTkButton(self.vision_frame, text="Locate fanuc trays", command=self.locate_fanuc_trays)
        self.locate_trays_fanuc_vision_button.grid(column=LEFT_COLUMN, row=5)
        self.update_slots_fanuc_vision_button = ctk.CTkButton(self.vision_frame, text="Update fanuc slots", command=self.update_fanuc_slots, state=tk.DISABLED)
        self.update_slots_fanuc_vision_button.grid(column=LEFT_COLUMN, row=6)
        
        self.locate_trays_teach_table_vision_button = ctk.CTkButton(self.vision_frame, text="Locate teach table trays", command=self.locate_teach_table_trays)
        self.locate_trays_teach_table_vision_button.grid(column=RIGHT_COLUMN, row=5)
        self.update_slots_teach_table_vision_button = ctk.CTkButton(self.vision_frame, text="Update teach table slots", command=self.update_teach_table_slots, state=tk.DISABLED)
        self.update_slots_teach_table_vision_button.grid(column=RIGHT_COLUMN, row=6)
        
        # Map canvases
        self.fanuc_canvas = tk.Canvas(self.vision_frame, width = 300, height=300, bd = 0, highlightthickness=0)
        self.fanuc_canvas.grid(row = 7,column = LEFT_COLUMN, sticky = "we", padx=50)
        
        self.teach_table_canvas = tk.Canvas(self.vision_frame, width = 300, height=300, bd = 0, highlightthickness=0)
        self.teach_table_canvas.grid(row = 7,column = RIGHT_COLUMN, sticky = "we", padx=50)
        
        self.fanuc_image_update_var.trace_add("write", self.fanuc_image_update)
        self.teach_image_update_var.trace_add("write", self.teach_image_update)
        
    
    def fanuc_image_cb(self, msg: ImageMsg):
        # fanuc_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding="passthrough")
        # # print(fanuc_image.shape)
        # self.fanuc_image.put(ctk.CTkImage(Image.fromarray(fanuc_image), size=(375, 211)))
        # self.fanuc_image_update()
        self.most_recent_fanuc_time = time()
        self.fanuc_image = msg
        self.fanuc_image_update_var.set((self.fanuc_image_update_var.get()+1)%2)
    
    def fanuc_image_update(self, _, __, ___):
        cv_image = self.bridge.imgmsg_to_cv2(self.fanuc_image, "rgb8")
        frame_to_show = Image.fromarray(cv_image)
        self.fanuc_image_label.configure(image=ctk.CTkImage(frame_to_show, size=(375, 211)))
    
    def teach_table_image_cb(self, msg: ImageMsg):
        # teach_table_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding="passthrough")
        # # print(teach_table_image.shape)
        # self.teach_image.put(ctk.CTkImage(Image.fromarray(teach_table_image), size=(375, 211)))
        # self.teach_image_update()
        self.most_recent_teach_time = time()
        self.teach_image = msg
        self.teach_image_update_var.set((self.teach_image_update_var.get()+1)%2)
    
    def teach_image_update(self, _, __, ___):
        cv_image = self.bridge.imgmsg_to_cv2(self.teach_image, "rgb8")
        frame_to_show = Image.fromarray(cv_image)
        self.teach_table_image_label.configure(image=ctk.CTkImage(frame_to_show, size=(375, 211)))
    
    def vision_connection_cb(self):
        current_time = time()
        if current_time - self.most_recent_fanuc_time > 5.0:
            self.fanuc_status_label.configure(text="Not Connected", text_color = "red")
        else:
            self.fanuc_status_label.configure(text="Connected", text_color = "green")
        
        if current_time - self.most_recent_teach_time > 5.0:
            self.teach_table_status_label.configure(text="Not Connected", text_color = "red")
        else:
            self.teach_table_status_label.configure(text="Connected", text_color = "green")
    
    def locate_fanuc_trays(self):
        request = Trigger.Request()
        future = self.locate_fanuc_trays_client.call_async(request)

        start = time()
        while not future.done():
            pass
            if time()-start >= 5.0:
                self.get_logger().warn("Unable to locate Fanuc trays. Be sure that the Fanuc is not blocking the vision system")
                return

        self.locate_trays_fanuc_vision_button.configure(state=tk.DISABLED)
        self.update_slots_fanuc_vision_button.configure(state=tk.NORMAL)
    
    def update_fanuc_slots(self):
        request = Trigger.Request()
        future = self.update_fanuc_slots_client.call_async(request)

        start = time()
        while not future.done():
            pass
            if time()-start >= 5.0:
                self.get_logger().warn("Unable to update fanuc slots. Be sure that the Fanuc is not blocking the vision system")
                return
    
    def locate_teach_table_trays(self):
        request = Trigger.Request()
        future = self.locate_teach_table_trays_client.call_async(request)

        start = time()
        while not future.done():
            pass
            if time()-start >= 5.0:
                self.get_logger().warn("Unable to locate teach table trays. Be sure that there is nothing blocking the teach table")
                return
            
        self.locate_trays_teach_table_vision_button.configure(state=tk.DISABLED)
        self.update_slots_teach_table_vision_button.configure(state=tk.NORMAL)
    
    def update_teach_table_slots(self):
        request = Trigger.Request()
        future = self.update_teach_table_slots_client.call_async(request)

        start = time()
        while not future.done():
            pass
            if time()-start >= 5.0:
                self.get_logger().warn("Unable to update teach table slots. Be sure that there is nothing blocking the teach table")
                return