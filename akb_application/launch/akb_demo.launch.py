import os
import yaml
import xacro
import rclpy.logging
from launch import LaunchDescription
from launch.actions import (
    DeclareLaunchArgument,
    IncludeLaunchDescription,
    OpaqueFunction,
)
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch.conditions import IfCondition

from ament_index_python.packages import get_package_share_directory, PackageNotFoundError


def launch_setup(context, *args, **kwargs):
    akb_application = Node(
        package='akb_application',
        executable='akb_application',
        output='screen'
    )

    fanuc_bringup = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [FindPackageShare("fanuc_description"), "/launch", "/fanuc_bringup.launch.py"]
        )
    )

    motoman_bringup = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [FindPackageShare("motoman_description"), "/launch", "/motoman_bringup.launch.py"]
        )
    )

    robot_commander = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [FindPackageShare("robot_commander"), "/launch", "/robot_commander.launch.py"]
        )
    )

    pddl = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [FindPackageShare("aprs_pddl"), "/launch", "/aprs_pddl.launch.py"]
        )
    )

    task_planner = Node(
        package='aprs_pddl',
        executable='task_planner_node',
        output='screen'
    )

    vision_system = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [FindPackageShare("aprs_vision"), "/launch", "/aprs_vision.launch.py"]
        )
    )

    nodes_to_start = [
        akb_application,
        fanuc_bringup,
        motoman_bringup,
        robot_commander,
        pddl,
        task_planner,
        vision_system
    ]

    return nodes_to_start


def generate_launch_description():
    declared_arguments = []

    return LaunchDescription(declared_arguments + [OpaqueFunction(function=launch_setup)])