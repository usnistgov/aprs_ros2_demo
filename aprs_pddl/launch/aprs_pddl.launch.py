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
    fanuc_place_part_action = Node(
        package='aprs_pddl',
        executable='fanuc_place_part_action_node',
        name='fanuc_place',
        output='screen',
        parameters=[])
    
    fanuc_pick_part_action = Node(
        package='aprs_pddl',
        executable='fanuc_pick_part_action_node',
        name='fanuc_pick',
        output='screen',
        parameters=[])
    
    motoman_place_part_action = Node(
        package='aprs_pddl',
        executable='motoman_place_part_action_node',
        name='motoman_place',
        output='screen',
        parameters=[])
    
    motoman_pick_part_action = Node(
        package='aprs_pddl',
        executable='motoman_pick_part_action_node',
        name='motoman_pick',
        output='screen',
        parameters=[])

    nodes_to_start = [
        plansys2_cmd,
        fanuc_place_part_action,
        fanuc_pick_part_action,
        motoman_place_part_action,
        motoman_pick_part_action
    ]

    return nodes_to_start

def generate_launch_description():
    declared_arguments = []

    return LaunchDescription(declared_arguments + [OpaqueFunction(function=launch_setup)])