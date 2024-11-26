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

    urdf = os.path.join(get_package_share_directory('motoman_description'), 'urdf', 'motoman.urdf.xacro')
        
    doc = xacro.process_file(urdf)

    robot_description = {"robot_description": doc.toxml()}

    robot_controllers = PathJoinSubstitution([FindPackageShare("motoman_description"), "config", "motoman_controllers.yaml",])

    control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        namespace='motoman',
        parameters=[
            robot_controllers],
        output="both",
        remappings=[
            ("~/robot_description", "/motoman/robot_description"),
        ],
    )
    
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        namespace='motoman',
        output='both',
        parameters=[
            robot_description
        ],
    )

    joint_state_broadcaster = Node(
        package="controller_manager",
        executable="spawner",
        namespace='motoman',
        arguments=['joint_state_broadcaster', '-c', '/motoman/controller_manager']
    )
    
    joint_trajectory_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=['joint_trajectory_controller', '-c', '/motoman/controller_manager'],
    )

    pneumatic_gripper_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=['pneumatic_gripper_controller', '-c', '/motoman/controller_manager']
    )
    
    nodes_to_start = [
        control_node,
        robot_state_publisher,
        joint_state_broadcaster,
        joint_trajectory_controller,
        pneumatic_gripper_controller,
    ]

    return nodes_to_start


def generate_launch_description():
    declared_arguments = []

    return LaunchDescription(declared_arguments + [OpaqueFunction(function=launch_setup)])