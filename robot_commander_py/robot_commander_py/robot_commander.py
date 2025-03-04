from rclpy.node import Node

from moveit.core.planning_interface import MotionPlanResponse
from moveit.core.controller_manager import ExecutionStatus

from moveit.planning import MoveItPy, PlanningComponent, PlanningSceneMonitor, PlanRequestParameters

from moveit_msgs.msg import MoveItErrorCodes

class RobotCommander(Node):
    def __init__(self):
        super().__init__('robot_commander')

        # Initialize MoveItPy
        self.moveit_py = MoveItPy(node_name="moveit_py", name_space='fanuc')

        # Initialize planning components
        self.planning_component: PlanningComponent = self.moveit_py.get_planning_component("fanuc_arm")

        self.get_logger().info("Finished configuration")

    def plan_and_execute(self):
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