from typing import Optional

from copy import deepcopy

from time import sleep

from rclpy.node import Node
from rclpy.time import Time
from rclpy.qos import qos_profile_default

from tf2_ros import TransformException # type: ignore
from tf2_ros import Buffer
from tf2_ros import TransformListener

from moveit.core.planning_interface import MotionPlanResponse
from moveit.core.controller_manager import ExecutionStatus
from moveit.core.planning_scene import PlanningScene
from moveit.core.robot_state import RobotState

from moveit.planning import MoveItPy, PlanningComponent, PlanningSceneMonitor, PlanRequestParameters

from moveit_msgs.msg import MoveItErrorCodes, ObjectColor

from geometry_msgs.msg import TransformStamped, Pose

from aprs_interfaces.msg import Trays, Tray, SlotInfo

from robot_commander_py.utils import (
    PlanningSceneObjectInfo, 
    build_collision_object, 
    convert_transform_to_pose, 
    multiply_pose
)

class RobotCommander(Node):
    def __init__(self):
        super().__init__('robot_commander')

        # Initialize MoveItPy
        self.moveit_py = MoveItPy(node_name="moveit_py", name_space='fanuc')

        # Initialize planning components
        self.planning_component: PlanningComponent = self.moveit_py.get_planning_component("fanuc_arm")
        self.planning_scene_monitor: PlanningSceneMonitor = self.moveit_py.get_planning_scene_monitor()

        # TF 
        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)

        # Flags
        self.planning_scene_ready = False

        # Variables
        self.trays_info: dict[str, Optional[Trays]] = {'table': None, 'conveyor': None}
        self.previous_trays_info: dict[str, Trays] = {}

        # Subscribers
        self.create_subscription(Trays, '/fanuc/conveyor_trays_info', self.conveyor_trays_info_cb, qos_profile_default)

        # Timer
        self.planning_scene_timer = self.create_timer(0.2, self.update_planning_scene_timer_cb)

    def plan_and_execute(self):
        # Set start state to current state
        with self.planning_scene_monitor.read_only() as scene:
            scene: PlanningScene

            self.planning_component.set_start_state(robot_state=scene.current_state)

        # Create the plan request parameters 
        single_plan_parameters = PlanRequestParameters(self.moveit_py, "fanuc_arm")
        single_plan_parameters.max_acceleration_scaling_factor = 1.0
        single_plan_parameters.max_velocity_scaling_factor = 1.0
        single_plan_parameters.planning_pipeline = "ompl"
        single_plan_parameters.planner_id = "RRTConnectkConfigDefault"

        # Generate plan
        plan: MotionPlanResponse = self.planning_component.plan(single_plan_parameters=single_plan_parameters)
        plan_result: MoveItErrorCodes = plan.error_code

        if plan_result.val == MoveItErrorCodes.SUCCESS:
            
            execution: ExecutionStatus =  self.moveit_py.execute(plan.trajectory, controllers=[])
            
            if not execution.status == "SUCCEEDED":
                self.get_logger().error(f'Unable to complete trajectory. Error: {execution.status}')
                
        else:
            self.get_logger().error(f'Unable to plan trajectory. Error code: {plan_result.val}')

    def add_trays_to_planning_scene(self, trays: Trays):
        objects: list[PlanningSceneObjectInfo] = []

        all_trays: list[Tray] = trays.kit_trays + trays.part_trays # type: ignore
        
        for tray in all_trays:
            try:
                transform = self.tf_buffer.lookup_transform('world', tray.tray_pose.header.frame_id, Time())    
            except Exception as e:
                self.get_logger().error(f'{e}')
                transform = TransformStamped()

            tray_world_pose = multiply_pose(convert_transform_to_pose(transform.transform), tray.tray_pose.pose)

            objects.append(PlanningSceneObjectInfo(tray.name, tray.identifier, tray_world_pose))

            for slot in tray.slots:
                slot: SlotInfo

                slot_world_pose = multiply_pose(tray_world_pose, slot.slot_pose.pose)
                
                if not slot.occupied:
                    continue

                objects.append(PlanningSceneObjectInfo(slot.name, slot.size, slot_world_pose))
        
        
        objects.append(PlanningSceneObjectInfo('table', PlanningSceneObjectInfo.TABLE, Pose()))

        conveyor_belt_pose = Pose()
        conveyor_belt_pose.position.x = -0.3937
        conveyor_belt_pose.position.y = -0.0762
        conveyor_belt_pose.position.z = 0.0625

        objects.append(PlanningSceneObjectInfo('conveyor', PlanningSceneObjectInfo.CONVEYOR, conveyor_belt_pose))

        for object in objects:      
            collision_object = build_collision_object(object)
            
            with self.planning_scene_monitor.read_write() as scene:
                scene: PlanningScene
                scene.apply_collision_object(collision_object, object.color)

            self.planning_scene_monitor.process_collision_object(collision_object)
            sleep(0.5)
            
        self.get_logger().info("Planning scene updated")
            

    def conveyor_trays_info_cb(self, msg: Trays):
        self.trays_info['conveyor'] = msg

    def update_planning_scene_timer_cb(self, *args):
        if self.trays_info['conveyor'] is None:
            return
        
        if not self.planning_scene_ready or self.trays_info['conveyor'] != self.previous_trays_info['conveyor']:
            with self.planning_scene_monitor.read_write() as scene:
                scene: PlanningScene
                scene.remove_all_collision_objects()

            self.add_trays_to_planning_scene(self.trays_info['conveyor'])
            self.previous_trays_info['conveyor'] = deepcopy(self.trays_info['conveyor'])
            self.planning_scene_ready = True