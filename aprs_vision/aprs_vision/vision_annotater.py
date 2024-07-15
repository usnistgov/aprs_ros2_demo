from numpy import square
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import TransformStamped, Pose
from tf2_ros.transform_broadcaster import TransformBroadcaster
from aprs_interfaces.msg import Object, Objects, KitTray, PartTray, SlotInfo, Trays
from typing import Optional
from math import sqrt

class VisionAnnotater(Node):
    def __init__(self):
        super().__init__('object_tf_broadcaster')
        self.tf_broadcaster = TransformBroadcaster(self)
        self.transforms = []
        self.trays_info = Trays()
        self.part_thresh = 0.07
        self.kit_thresh = 0.050
        self.tf_publish_timer = self.create_timer(0.1, self.publish_frames)
        self.trays_publish_timer = self.create_timer(0.1, self.publish_trays)
        self.objects_sub = self.create_subscription(Objects, 'antvision_objects', self.objects_callback, 10)
        self.trays_publisher = self.create_publisher(Trays, 'trays_info', 10)
        self.slot_offsets: dict[int, dict[str, tuple[float, float]]] = {
            Object.S2L2_KIT_TRAY: {
                'lg_1': (-0.028, -0.055),
                'lg_2': (-0.028, 0.055),
                'sg_1': (0.055, -0.04),
                'sg_2': (0.055, 0.04),
            },
            Object.LARGE_GEAR_TRAY: {
                'slot_1': (0.0, 0.055),
                'slot_2': (0.0, -0.055),
            },
            Object.MEDIUM_GEAR_TRAY: {
                'slot_1': (0.038, -0.038),
                'slot_2': (0.038, 0.038),
                'slot_3': (-0.038, -0.038),
                'slot_4': (-0.038, 0.038),
            },
            Object.M2L1_KIT_TRAY: {
                'lg_1': (0.0, 0.04),
                'mg_1': (0.04, -0.055),
                'mg_2': (-0.04, -0.055),
            },
            Object.SMALL_GEAR_TRAY: {
                'slot_1': (0.028, -0.028),
                'slot_2': (0.028, 0.028),
                'slot_3': (-0.028, -0.028),
                'slot_4': (-0.028, 0.028),
            },
        }
        self.raw_object_data : Optional[list[Object]]  = None
    
    def objects_callback(self, msg: Objects ):
        if not self.raw_object_data:
            self.raw_object_data = []
        
        self.raw_object_data.clear()

        for obj in msg.objects:
            self.raw_object_data.append(obj)

        self.build_frames()
        self.build_trays_msg()

    def publish_trays(self):
        self.trays_publisher.publish(self.trays_info)

    def publish_frames(self):
        self.tf_broadcaster.sendTransform(self.transforms)

    def calculate_distance(self, p1: Pose, p2: Pose):
        return sqrt(square(p1.position.x - p2.position.x) + square(p1.position.y - p2.position.y))

    def build_kit_tray_msg(self, kit_tray_obj: Object) -> KitTray:
        if not self.raw_object_data:
            return KitTray()
        
        kit_tray_msg = KitTray()
        kit_tray_msg.identifier = kit_tray_obj.object_identifier
        kit_tray_msg.name = kit_tray_obj.name

        part_poses = [p.pose_stamped.pose for p in self.raw_object_data if p.object_type == Object.PART]

        a = self.slot_offsets[kit_tray_msg.identifier]

        for name, (x_offset, y_offset) in self.slot_offsets[kit_tray_msg.identifier].items():
            slot_info = SlotInfo()
            slot_info.name = f"{kit_tray_msg.name}_{name}"
            slot_info.number = int(name[-1])

            slot_pose = kit_tray_obj.pose_stamped.pose
            slot_pose.position.x += x_offset
            slot_pose.position.y += y_offset

            distance = [self.calculate_distance(slot_pose, p) for p in part_poses]

            if not distance:
                slot_info.occupied = False
                if "lg" in slot_info.name:
                    kit_tray_msg.large_gear_slots.append(slot_info) #type: ignore
                elif "mg" in slot_info.name:
                    kit_tray_msg.medium_gear_slots.append(slot_info) #type: ignore
                elif "sg" in slot_info.name:
                    kit_tray_msg.small_gear_slots.append(slot_info) #type: ignore
                continue
            
            if min(distance) < self.kit_thresh:
                slot_info.occupied = True
            else:
                slot_info.occupied = False

            if "lg" in slot_info.name:
                kit_tray_msg.large_gear_slots.append(slot_info) #type: ignore
            elif "mg" in slot_info.name:
                kit_tray_msg.medium_gear_slots.append(slot_info) #type: ignore
            elif "sg" in slot_info.name:
                kit_tray_msg.small_gear_slots.append(slot_info) #type: ignore

        return kit_tray_msg

    def build_part_tray_msg(self, part_tray_obj: Object) -> PartTray:
        if not self.raw_object_data:
            return PartTray()
        
        part_tray_msg = PartTray()
        part_tray_msg.identifier = part_tray_obj.object_identifier
        part_tray_msg.name = part_tray_obj.name

        part_poses = [p.pose_stamped.pose for p in self.raw_object_data if p.object_type == Object.PART]

        a = self.slot_offsets[part_tray_msg.identifier]

        for name, (x_offset, y_offset) in self.slot_offsets[part_tray_msg.identifier].items():
            slot_info = SlotInfo()
            slot_info.name = f"{part_tray_msg.name}_{name}"
            slot_info.number = int(name[-1])

            slot_pose = part_tray_obj.pose_stamped.pose
            slot_pose.position.x += x_offset
            slot_pose.position.y += y_offset

            distance = [self.calculate_distance(slot_pose, p) for p in part_poses]

            if not distance:
                slot_info.occupied = False
                part_tray_msg.slots.append(slot_info)
                continue

            # self.get_logger().info(str(min(distance)))
            # self.get_logger().info(slot_info.name)

            if min(distance) < self.part_thresh:
                slot_info.occupied = True
            else:
                slot_info.occupied = False

            part_tray_msg.slots.append(slot_info) #type: ignore

        # self.get_logger().info("\n")
        return part_tray_msg

    def build_trays_msg(self):
        if not self.raw_object_data:
            return
        
        self.trays_info.kit_trays = []
        self.trays_info.part_trays = []
        
        for object in self.raw_object_data:
            object: Object
            if object.object_type == Object.KIT_TRAY:
                self.trays_info.kit_trays.append(self.build_kit_tray_msg(object)) #type: ignore
            elif object.object_type == Object.PART_TRAY:
                self.trays_info.part_trays.append(self.build_part_tray_msg(object)) #type: ignore

    def create_pose(self, x, y):
        pose = Pose()
        pose.position.x = x
        pose.position.y = y
        pose.position.z = 0.03
        pose.orientation.x = 0.0
        pose.orientation.y = 0.0
        pose.orientation.z = 0.0
        pose.orientation.w = 1.0
        return pose

    def build_frames(self):
        self.transforms.clear()
        
        if not self.raw_object_data:
            return

        for object in self.raw_object_data:
            object: Object
            self.generate_transform(object.pose_stamped.header.frame_id, object.name, object.pose_stamped.pose)

            slot_info = self.slot_offsets.get(object.object_identifier)
            if slot_info:
                for slot_name, (x_offset, y_offset) in slot_info.items():
                    slot_frame_id = f"{object.name}_{slot_name}"
                    self.generate_transform(object.name, slot_frame_id, self.create_pose(x_offset, y_offset))

    def generate_transform(self, parent_frame, child_frame, initial_pose: Pose):
        t = TransformStamped()

        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = parent_frame
        t.child_frame_id = child_frame

        t.transform.translation.x = initial_pose.position.x
        t.transform.translation.y = initial_pose.position.y
        t.transform.translation.z = initial_pose.position.z
        t.transform.rotation.x = initial_pose.orientation.x
        t.transform.rotation.y = initial_pose.orientation.y
        t.transform.rotation.z = initial_pose.orientation.z
        t.transform.rotation.w = initial_pose.orientation.w
        
        self.transforms.append(t)