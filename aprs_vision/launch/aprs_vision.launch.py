import os

from launch import LaunchDescription
from launch.actions import (
    OpaqueFunction,
)

from launch_ros.actions import Node


def launch_setup(context, *args, **kwargs):

    object_tf_broadcaster = Node(
        package='aprs_vision',
        executable='object_tf_broadcaster.py',
        output='screen',
    )

    antvision_node = Node(
        package='aprs_vision',
        executable='antvision_publisher.py',
        output='screen'
    )
    
    nodes_to_start = [
        object_tf_broadcaster,
        antvision_node
    ]

    return nodes_to_start


def generate_launch_description():
    declared_arguments = []

    return LaunchDescription(declared_arguments + [OpaqueFunction(function=launch_setup)])
