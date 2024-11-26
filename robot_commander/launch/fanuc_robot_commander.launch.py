import os

from ament_index_python.packages import get_package_share_directory

from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch.actions import OpaqueFunction

from moveit_configs_utils import MoveItConfigsBuilder

def launch_setup(context, *args, **kwargs):

    urdf = os.path.join(get_package_share_directory("fanuc_description"), "urdf/fanuc.urdf.xacro")

    moveit_config = (
        MoveItConfigsBuilder("fanuc", package_name="fanuc_moveit_config")
        .robot_description(urdf)
        .robot_description_semantic(file_path="config/fanuc.srdf")
        .trajectory_execution(file_path="config/controllers.yaml")
        .planning_pipelines(pipelines=["ompl"])
        .to_moveit_configs()
    )

    robot_commander_config = os.path.join(get_package_share_directory('robot_commander'), 'config', 'parameters.yaml')

    # Move group node
    move_group_node = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        namespace="fanuc",
        output="screen",
        parameters=[
            moveit_config.to_dict(),
        ],
    )   
        
    # Fanuc Robot Commander
    fanuc_robot_commander = Node(
        package="robot_commander",
        executable="robot_commander",
        name="fanuc_robot_commander",
        output="screen",
        parameters=[
            moveit_config.to_dict(),
            robot_commander_config
        ],
        remappings=[
            ("joint_states", "/fanuc/joint_states"),
        ],
    )

    # RVIZ 
    rviz_config_file = PathJoinSubstitution(
        [FindPackageShare("robot_commander"), "config", "fanuc.rviz"]
    )

    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        output="log",
        namespace="fanuc",
        arguments=["-d", rviz_config_file],
        parameters=[
            moveit_config.to_dict(),
        ],
    )

    nodes_to_start = [
        move_group_node,
        fanuc_robot_commander,
        rviz_node
    ]

    return nodes_to_start

def generate_launch_description():
    declared_arguments = []

    return LaunchDescription(declared_arguments + [OpaqueFunction(function=launch_setup)])