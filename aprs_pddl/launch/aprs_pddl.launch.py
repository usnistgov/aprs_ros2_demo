import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription , OpaqueFunction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node

def launch_setup(context, *args, **kwargs):

    pkg_share = get_package_share_directory('aprs_pddl')

    plansys2_cmd = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(
            get_package_share_directory('plansys2_bringup'),
            'launch',
            'plansys2_bringup_launch_monolithic.py')),
        launch_arguments={
          'model_file': pkg_share + '/pddl/aprs_demo_domain.pddl',
          }.items())

    # Specify the actions
    place_part_action = Node(
        package='aprs_pddl',
        executable='place_part_action_node',
        name='place',
        output='screen',
        parameters=[])
    
    pick_part_action = Node(
        package='aprs_pddl',
        executable='pick_part_action_node',
        name='pick',
        output='screen',
        parameters=[])

    nodes_to_start = [
        plansys2_cmd,
        place_part_action,
        pick_part_action,
    ]

    return nodes_to_start

def generate_launch_description():
    declared_arguments = []

    return LaunchDescription(declared_arguments + [OpaqueFunction(function=launch_setup)])