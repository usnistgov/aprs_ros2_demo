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
from moveit.core.robot_state import RobotState, robotStateToRobotStateMsg
from moveit.core.robot_model import RobotModel, JointModelGroup
from moveit.core.robot_trajectory import RobotTrajectory

from moveit.planning import MoveItPy, PlanningComponent, PlanningSceneMonitor, PlanRequestParameters, TrajectoryExecutionManager

from moveit_msgs.msg import MoveItErrorCodes, CollisionObject
from moveit_msgs.msg import PlanningScene as PlanningSceneMsg
from moveit_msgs.srv import GetCartesianPath

from geometry_msgs.msg import TransformStamped, Pose, PoseStamped

from aprs_interfaces.msg import Trays, Tray, SlotInfo
from aprs_interfaces.srv import PneumaticGripperControl

from robot_commander_py.utils import (
    PlanningSceneObjectInfo, 
    build_collision_object, 
    convert_transform_to_pose,
    convert_transform_stamped_to_pose_stamped, 
    build_robot_pose,
    multiply_pose
)

class RobotCommander(Node):
    def __init__(self):
        super().__init__('robot_commander')

        # TODO: remove mentions of fanuc and replace with a robot variable, same with fanuc arm and replace with a group_name variable

        # Initialize MoveItPy
        self.moveit_py = MoveItPy(node_name="moveit_py", name_space='fanuc')

        # Initialize planning components
        self.planning_component: PlanningComponent = self.moveit_py.get_planning_component("fanuc_arm")
        self.planning_scene_monitor: PlanningSceneMonitor = self.moveit_py.get_planning_scene_monitor()
        self.robot_model: RobotModel = self.moveit_py.get_robot_model()
        self.joint_group: JointModelGroup = self.robot_model.get_joint_model_group('fanuc_arm')
        self.trajectory_execution_manager: TrajectoryExecutionManager = self.moveit_py.get_trajectory_execution_manager()

        # TF 
        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)

        # Flags
        self.planning_scene_ready = False

        # Variables
        self.trays_info: dict[str, Optional[Trays]] = {'table': None, 'conveyor': None}
        self.previous_trays_info: dict[str, Trays] = {}
        self.collision_objects: list[CollisionObject] = []

        # Services
        self._compute_cartesian_path_client = self.create_client(GetCartesianPath, "/fanuc/compute_cartesian_path")
        self.gripper_client = self.create_client(PneumaticGripperControl, '/fanuc/actuate_gripper')

        # Subscribers
        self.create_subscription(Trays, '/fanuc/conveyor_trays_info', self.conveyor_trays_info_cb, qos_profile_default)

        # Publisher
        self.planning_scene_pub = self.create_publisher(PlanningSceneMsg, '/fanuc/planning_scene_1', qos_profile_default)

        # Timer
        self.planning_scene_timer = self.create_timer(0.2, self.update_planning_scene_timer_cb)

    def plan_and_execute(self) -> bool:
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
        single_plan_parameters.planning_time = 10.0

        # Generate plan
        plan: MotionPlanResponse = self.planning_component.plan(single_plan_parameters=single_plan_parameters)
        plan_result: MoveItErrorCodes = plan.error_code

        if not plan_result.val == MoveItErrorCodes.SUCCESS:
            self.get_logger().error(f'Unable to plan trajectory. Error code: {plan_result.val}')
            return False
            
        execution: ExecutionStatus =  self.moveit_py.execute(plan.trajectory, controllers=[])
        
        if not execution.status == "SUCCEEDED":
            self.get_logger().error(f'Unable to complete trajectory. Error: {execution.status}')
            return False
                
        return True
    
    def cartesian_plan_and_execute(self, pose: Pose) -> bool:
        #TODO: remove hard coding of fanuc_arm for group name
        result = self.plan_cartesian_trajectory("fanuc_arm", pose)
        
        if result.fraction < 1.0:
            self.get_logger().warn(f'Unable to fully compute cartesian path, fraction is {result.fraction}')
            return False
        
        with self.planning_scene_monitor.read_write() as scene:
            scene: PlanningScene
            trajectory = RobotTrajectory(self.robot_model)            
            trajectory.set_robot_trajectory_msg(scene.current_state, result.solution)
            trajectory.joint_model_group_name = 'fanuc_arm'

        # Retime trajectory
        if not trajectory.apply_totg_time_parameterization(1.0, 1.0):
            self.get_logger().warn('Unable to retime trajectory')

        execution: ExecutionStatus =  self.moveit_py.execute(trajectory, controllers=[])
            
        if not execution.status == "SUCCEEDED":
            self.get_logger().error(f'Unable to complete trajectory. Error: {execution.status}')
            return False
        
        return True

    def move_to_named_configuration(self, configuration) -> bool:
        if not configuration in self.planning_component.named_target_states:
            return False
        
        self.planning_component.set_goal_state(configuration_name=configuration)

        return self.plan_and_execute()
    
    def move_to_defined_pose(self, pose: Pose) -> bool:

        with self.planning_scene_monitor.read_write() as scene:
            scene: PlanningScene
            self.planning_component.set_start_state(robot_state=scene.current_state)

        goal_pose_stamped = PoseStamped()
        goal_pose_stamped.header.frame_id = 'world'
        goal_pose_stamped.header.stamp = self.get_clock().now().to_msg()
        goal_pose_stamped.pose = pose

        # TODO: fix hardcoding of fanuc_tool0
        
        self.planning_component.set_goal_state(pose_stamped_msg=goal_pose_stamped, pose_link="fanuc_tool0")

        return self.plan_and_execute()

    def pick_from_slot(self, slot_name: str) -> bool:
        try:
            transform = self.tf_buffer.lookup_transform('world', slot_name, Time())
        except Exception as e:
            self.get_logger().error(str(e))
            return False
        
        part_on_conveyor = transform.transform.translation.y < 0
        robot_above_table = self.get_joint_value('joint_1') > 0 

        if (part_on_conveyor and robot_above_table) or (not part_on_conveyor and not robot_above_table):
            self.move_to_named_configuration('conveyor_home')

        # Set variables for testing:
        # TODO: recieve parameters from yaml file

        pick_offset = 0.0
        above_slot_offset = 0.1
        
        # Set goal state to above transform

        above_slot_pose: Pose
        above_slot_pose = convert_transform_to_pose(transform.transform)

        above_slot_pose.position.z += above_slot_offset

        above_slot_robot_pose = build_robot_pose(above_slot_pose)

        self.cartesian_plan_and_execute(above_slot_robot_pose)

        # Set goal state to pick position

        pick_pose: Pose
        pick_pose = convert_transform_to_pose(transform.transform)

        pick_pose.position.z += pick_offset

        robot_pick_pose = build_robot_pose(pick_pose)

        self.cartesian_plan_and_execute(robot_pick_pose)

        # Close gripper to grab gear

        self.actuate_gripper(True)

        # Move back to above slot

        self.cartesian_plan_and_execute(above_slot_robot_pose)

        return True
    
    def place_in_slot(self, slot_name: str) -> bool:
        try:
            transform = self.tf_buffer.lookup_transform('world', slot_name, Time())
        except Exception as e:
            self.get_logger().error(e)
            return False
        
        # TODO: remove hardcoding on joint_1

        part_on_conveyor = transform.transform.translation.y < 0
        robot_above_table = self.get_joint_value('joint_1') > 0 

        if (part_on_conveyor and robot_above_table) or (not part_on_conveyor and not robot_above_table):
            self.move_to_named_configuration('conveyor_home')

        # Set variables for testing:
        # TODO: recieve parameters from yaml file

        place_offset = 0.01
        above_slot_offset = 0.1
        
        # Set goal state to above transform

        above_slot_pose: Pose
        above_slot_pose = convert_transform_to_pose(transform.transform)

        above_slot_pose.position.z += above_slot_offset

        above_slot_robot_pose = build_robot_pose(above_slot_pose)

        self.cartesian_plan_and_execute(above_slot_robot_pose)

        # Set goal state to pick position

        pick_pose: Pose
        pick_pose = convert_transform_to_pose(transform.transform)

        pick_pose.position.z += place_offset

        robot_pick_pose = build_robot_pose(pick_pose)

        self.cartesian_plan_and_execute(robot_pick_pose)

        # Close gripper to grab gear

        self.actuate_gripper(False)

        # Move back to above slot

        self.cartesian_plan_and_execute(above_slot_robot_pose)

        return True

    def get_joint_value(self, joint_name: str) -> float:
        active_joints: list[str] = self.joint_group.active_joint_model_names

        if not joint_name in active_joints:
            self.get_logger().error(f'{joint_name} does not exist in active joints.')
            raise Exception(f'{joint_name} does not exist in active joints.')

        with self.planning_scene_monitor.read_only() as scene:
            scene: PlanningScene
            state: RobotState = scene.current_state

            # TODO: remove group name hard coding
            current_state = state.get_joint_group_positions("fanuc_arm")
                
            return current_state[active_joints.index(joint_name)]

    def add_trays_to_planning_scene(self, trays: Trays):
        objects: list[PlanningSceneObjectInfo] = []
        self.collision_objects.clear()

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
    
        self.collision_objects = [build_collision_object(obj) for obj in objects]
            
        with self.planning_scene_monitor.read_write() as scene:
            scene: PlanningScene

            for collision_obj, planning_obj in zip(self.collision_objects, objects):
                scene.apply_collision_object(collision_obj, planning_obj.color)
            
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

        # with self.planning_scene_monitor.read_only() as scene:
        #     scene: PlanningScene
            
        #     plan_msg: PlanningSceneMsg = scene.planning_scene_message

        #     plan_msg.world.collision_objects = self.collision_objects

        #     self.planning_scene_pub.publish(plan_msg)

    def plan_cartesian_trajectory(self, group_name: str, goal_pose: Pose, vsf=1.0, asf=1.0, avoid_collisions=True) -> GetCartesianPath.Response:
        request = GetCartesianPath.Request()

        request.header.frame_id = 'world'
        request.header.stamp = self.get_clock().now().to_msg()
        
        with self.planning_scene_monitor.read_write() as scene:
            scene: PlanningScene
            request.start_state = robotStateToRobotStateMsg(scene.current_state)
            
        request.group_name = group_name

        request.waypoints = [goal_pose]
        request.max_step = 0.1
        request.avoid_collisions = True
        request.max_velocity_scaling_factor = vsf
        request.max_acceleration_scaling_factor = asf
        request.avoid_collisions = avoid_collisions

        future = self._compute_cartesian_path_client.call_async(request)

        while not future.done():
            sleep(0.1)

        return future.result() # type: ignore
    
    def actuate_gripper(self, enable: bool) -> PneumaticGripperControl.Response:
        req = PneumaticGripperControl.Request()

        req.enable = enable

        future = self.gripper_client.call_async(req)

        while not future.done():
            sleep(0.1)

        return future.result() #type: ignore