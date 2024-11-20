import os
import math
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

    motoman_vision = Node(
        package='aprs_vision',
        executable='motoman_vision_node.py',
        output='screen'
    )

    motoman_conveyor_vision = Node(
        package='aprs_vision',
        executable='motoman_conveyor_node.py',
        output='screen'
    )

    fanuc_conveyor_vision = Node(
        package='aprs_vision',
        executable='fanuc_conveyor_node.py',
        output='screen'
    )

    fanuc_base_static_transform = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        arguments=[
            "--x" , f"{30 * .0254}", 
            "--y" , f"{7 * .0254}",
            "--frame-id" , "world",
            "--child-frame-id" , "fanuc_base_link",
        ]
        # extarguments=[
        #     {"FRAME_ID": 'world'},
        #     {'CHILD_FRAME_ID': 'fanuc_base_link'},
        #     {'X': 0},
        #     {'Y': 0},
        #     {'Z': 0},
        # ]

    )

    motoman_base_static_transform = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        arguments=[
            "--x" , f"{77.5 * .0254}", 
            "--y" , f"{6.5 * .0254}",
            "--yaw" , f"{math.pi}",
            "--frame-id" , "world",
            "--child-frame-id" , "motoman_base_link",
        ]
        # arguments=[
        #     {"FRAME_ID": 'world'},
        #     {'CHILD_FRAME_ID': 'motoman_base_link'},
        #     {'X': 0},
        #     {'Y': 0},
        #     {'Z': 0},
        # ]

    )
    
    nodes_to_start = [
        fanuc_vision,
        teach_table_vision,
        motoman_vision,
        motoman_conveyor_vision,
        fanuc_conveyor_vision,
        # motoman_base_static_transform,
        # fanuc_base_static_transform
    ]

    return nodes_to_start


def generate_launch_description():
    declared_arguments = []

    return LaunchDescription(declared_arguments + [OpaqueFunction(function=launch_setup)])
