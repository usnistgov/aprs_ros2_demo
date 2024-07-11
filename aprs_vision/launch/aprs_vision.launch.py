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

    use_rviz = LaunchConfiguration("rviz")

    vision_annotater = Node(
        package='aprs_vision',
        executable='vision_annotater_node.py',
        output='screen',
    )

    antvision_node = Node(
        package='aprs_vision',
        executable='antvision_publisher_node.py',
        output='screen'
    )

    rviz_config_file = PathJoinSubstitution(
        [FindPackageShare("aprs_vision"), "config", "vision.rviz"]
    )

    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        output="log",
        arguments=["-d", rviz_config_file],
        condition=IfCondition(use_rviz)
    )
    
    nodes_to_start = [
        vision_annotater,
        antvision_node,
        rviz_node,
    ]

    return nodes_to_start


def generate_launch_description():
    declared_arguments = []

    declared_arguments.append(DeclareLaunchArgument("rviz", default_value="false",description="start rviz node?"))

    return LaunchDescription(declared_arguments + [OpaqueFunction(function=launch_setup)])
