import os
import xacro

from launch import LaunchDescription
from launch.actions import OpaqueFunction
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

from ament_index_python.packages import get_package_share_directory

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