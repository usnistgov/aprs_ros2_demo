import os
from launch import LaunchDescription
from launch.actions import (
    DeclareLaunchArgument,
    OpaqueFunction,
)
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch.substitutions import PathJoinSubstitution, LaunchConfiguration
from launch.conditions import IfCondition

def launch_setup(context, *args, **kwargs):

    fanuc_vision = Node(
        package='aprs_vision',
        executable='fanuc_vision_node.py',
        output='screen',
    )

    teach_table_vision = Node(
        package='aprs_vision',
        executable='teach_table_vision_node.py',
        output='screen'
    )
    
    nodes_to_start = [
        fanuc_vision,
        teach_table_vision,
    ]

    return nodes_to_start


def generate_launch_description():
    declared_arguments = []

    return LaunchDescription(declared_arguments + [OpaqueFunction(function=launch_setup)])
