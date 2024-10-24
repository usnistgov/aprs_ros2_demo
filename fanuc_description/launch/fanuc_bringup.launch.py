import os
import xacro
from launch import LaunchDescription
from launch.actions import (
    OpaqueFunction,
)
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

from ament_index_python.packages import get_package_share_directory

from moveit_configs_utils import MoveItConfigsBuilder

def launch_setup(context, *args, **kwargs):

    urdf = os.path.join(get_package_share_directory('fanuc_description'), 'urdf', 'fanuc.urdf.xacro')
        
    doc = xacro.process_file(urdf)

    robot_description = {"robot_description": doc.toxml()}

    robot_controllers = PathJoinSubstitution([FindPackageShare("fanuc_description"), "config", "fanuc_controllers.yaml",])

    robot_state_publisher = Node(
        package='robot_state_publisher',
        namespace='fanuc',
        executable='robot_state_publisher',
        output='both',
        parameters=[
            robot_description
        ],
    )

    control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        namespace='fanuc',
        parameters=[robot_controllers],
        output="both",
        remappings=[
            ("~/robot_description", "/fanuc/robot_description"),
        ],
    )

    joint_state_broadcaster = Node(
        package="controller_manager",
        executable="spawner",
        namespace='fanuc',
        arguments=['joint_state_broadcaster', '-c', '/fanuc/controller_manager'],
    )

    joint_trajectory_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=['joint_trajectory_controller', '-c', '/fanuc/controller_manager'],
    )

    pneumatic_gripper_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=['pneumatic_gripper_controller', '-c', '/fanuc/controller_manager'],
    )

    rviz_config_file = PathJoinSubstitution(
        [FindPackageShare("fanuc_description"), "config", "fanuc.rviz"]
    )

    moveit_config = (
        MoveItConfigsBuilder("fanuc", package_name="fanuc_moveit_config")
        .robot_description(urdf)
        .robot_description_semantic(file_path="config/fanuc.srdf")
        .trajectory_execution(file_path="config/controllers.yaml")
        .planning_pipelines(pipelines=["ompl"])
        .to_moveit_configs()
    )

    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        output="log",
        namespace="fanuc",
        arguments=["-d", rviz_config_file],
        parameters=[
            moveit_config.to_dict(),
        ],
    )
    

    nodes_to_start = [
        control_node,
        robot_state_publisher,
        joint_state_broadcaster,
        joint_trajectory_controller,
        pneumatic_gripper_controller,
        # forward_position_controller,
        # fanuc_gripper_control,
        rviz_node
    ]

    return nodes_to_start


def generate_launch_description():
    declared_arguments = []

    return LaunchDescription(declared_arguments + [OpaqueFunction(function=launch_setup)])