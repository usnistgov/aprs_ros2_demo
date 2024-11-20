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

    robot_state_publisher = Node(
        package='robot_state_publisher',
        namespace='motoman',
        executable='robot_state_publisher',
        output='both',
        parameters=[
            robot_description
        ],
    )

    rviz_config_file = PathJoinSubstitution(
        [FindPackageShare("motoman_description"), "config", "motoman.rviz"]
    )

    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        output="log",
        namespace="motoman",
        arguments=["-d", rviz_config_file],
    )
    

    nodes_to_start = [
        robot_state_publisher,
        rviz_node
    ]

    return nodes_to_start


def generate_launch_description():
    declared_arguments = []

    return LaunchDescription(declared_arguments + [OpaqueFunction(function=launch_setup)])