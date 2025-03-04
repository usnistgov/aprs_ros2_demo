from launch import LaunchDescription
from launch.actions import OpaqueFunction

import os

from launch_ros.actions import Node

from moveit_configs_utils import MoveItConfigsBuilder

from ament_index_python.packages import get_package_share_directory

def launch_setup(context, *args, **kwargs):
    
    urdf = os.path.join(get_package_share_directory("fanuc_description"), "urdf/fanuc.urdf.xacro")

    moveit_config = (
        MoveItConfigsBuilder("fanuc", package_name="fanuc_moveit_config")
        .robot_description(urdf)
        .robot_description_semantic(file_path="config/fanuc.srdf")
        .trajectory_execution(file_path="config/controllers.yaml")
        .planning_pipelines(pipelines=["ompl"])
        .moveit_cpp(file_path="config/moveit_cpp.yaml")
        .to_moveit_configs()
    )

    # MoveItPy node
    moveit_py_test = Node(
        package="robot_commander_py",
        executable="test_moveit.py",
        output="screen",
        parameters=[
            moveit_config.to_dict(),
        ],
    )

    nodes_to_start = [
        moveit_py_test
    ]

    return nodes_to_start


def generate_launch_description():
    declared_arguments = []

    return LaunchDescription(declared_arguments + [OpaqueFunction(function=launch_setup)])