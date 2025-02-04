import os

from ament_index_python.packages import get_package_share_directory

from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution, LaunchConfiguration
from launch.actions import OpaqueFunction, DeclareLaunchArgument

from moveit_configs_utils import MoveItConfigsBuilder

def launch_setup(context, *args, **kwargs):

    robots = LaunchConfiguration('robots')

    if robots.perform(context) == 'all':
        robots_to_start = ['fanuc', 'motoman']
    elif robots.perform(context) == 'fanuc':
        robots_to_start = ['fanuc']
    elif robots.perform(context) == 'motoman':
        robots_to_start = ['motoman']
    else:
        print("robots argument is not valid. Please use [\'all\', \'fanuc\', or \'motoman\']")
        return

    robot_commander_parameters = os.path.join(get_package_share_directory('robot_commander'), 'config', 'parameters.yaml')
    nodes_to_start = []
    
    for robot in robots_to_start:

        urdf = os.path.join(get_package_share_directory(f"{robot}_description"), f"urdf/{robot}.urdf.xacro")

        moveit_config = (
            MoveItConfigsBuilder(robot, package_name=f"{robot}_moveit_config")
            .robot_description(urdf)
            .robot_description_semantic(file_path=f"config/{robot}.srdf")
            .trajectory_execution(file_path="config/controllers.yaml")
            .planning_pipelines(pipelines=["ompl"])
            .to_moveit_configs()
        )

        # Move group node
        move_group_node = Node(
            package="moveit_ros_move_group",
            executable="move_group",
            namespace=robot,
            output="screen",
            parameters=[
                moveit_config.to_dict(),
            ],
        )   
            
        # Robot Commander Node
        robot_commander = Node(
            package="robot_commander",
            executable="robot_commander",
            name=f"{robot}_robot_commander",
            output="screen",
            parameters=[
                moveit_config.to_dict(),
                robot_commander_parameters
            ],
            remappings=[
                ("joint_states", f"/{robot}/joint_states"),
            ],
        )

         # RVIZ 
        rviz_config_file = PathJoinSubstitution(
            [FindPackageShare("robot_commander"), "config", f"{robot}.rviz"]
        )

        rviz_node = Node(
            package="rviz2",
            executable="rviz2",
            output="log",
            arguments=["-d", rviz_config_file],
            parameters=[
                moveit_config.to_dict(),
            ],
        )

        nodes_to_start.append(move_group_node)
        nodes_to_start.append(robot_commander)
        nodes_to_start.append(rviz_node)
   
    return nodes_to_start

def generate_launch_description():
    declared_arguments = [
        DeclareLaunchArgument(
            'robots',
            default_value='all'
        )
    ]

    return LaunchDescription(declared_arguments + [OpaqueFunction(function=launch_setup)])