import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch_ros.actions import Node

from launch.actions import OpaqueFunction

def launch_setup(_):

    parameters = os.path.join(get_package_share_directory('aprs_vision'), 'config', 'parameters.yaml')

    fanuc_table_detection_area = Node(
        package='aprs_vision',
        executable='detection_area_node.py',
        name='table_detection_area',
        namespace='fanuc',
        parameters=[parameters],
        output='screen',
    )

    motoman_table_detection_area = Node(
        package='aprs_vision',
        executable='detection_area_node.py',
        name='table_detection_area',
        namespace='motoman',
        parameters=[parameters],
        output='screen',
    )

    fanuc_conveyor_detection_area = Node(
        package='aprs_vision',
        executable='detection_area_node.py',
        name='conveyor_detection_area',
        namespace='fanuc',
        parameters=[parameters],
        output='screen',
    )

    # motoman_conveyor_detection_area = Node(
    #     package='aprs_vision',
    #     executable='detection_area_node.py',
    #     name='conveyor_detection_area',
    #     namespace='motoman',
    #     parameters=[parameters],
    #     output='screen',
    # )

    teach_table_detection_area = Node(
        package='aprs_vision',
        executable='detection_area_node.py',
        name='teach_table_detection_area',
        namespace='teach',
        parameters=[parameters],
        output='screen',
    )

    nodes_to_start = [
        fanuc_table_detection_area,
        motoman_table_detection_area,
        fanuc_conveyor_detection_area,
        # motoman_conveyor_detection_area,
        teach_table_detection_area
    ]

    return nodes_to_start


def generate_launch_description():
    declared_arguments = []

    return LaunchDescription(declared_arguments + [OpaqueFunction(function=launch_setup)])
