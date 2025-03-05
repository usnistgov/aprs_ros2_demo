import os

from launch import LaunchDescription
from launch.actions import OpaqueFunction
from launch.substitutions import PathJoinSubstitution

from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

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
    
    # Move group node
    move_group_node = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        namespace='fanuc',
        output="screen",
        parameters=[
            moveit_config.to_dict(),
        ],
    ) 

    rviz_config_file = PathJoinSubstitution(
        [FindPackageShare("fanuc_moveit_config"), "config", "fanuc.rviz"]
    )

    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        output="log",
        parameters=[
            moveit_config.to_dict(),
        ],
        arguments=["-d", rviz_config_file],
    )

    nodes_to_start = [
        move_group_node,
        rviz_node
    ]

    return nodes_to_start


def generate_launch_description():
    declared_arguments = []

    return LaunchDescription(declared_arguments + [OpaqueFunction(function=launch_setup)])