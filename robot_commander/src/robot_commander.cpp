#include <robot_commander/robot_commander.hpp>

RobotCommander::RobotCommander(std::string node_name)
 : Node(node_name)
{
  // RCLCPP_INFO(get_logger(), "Starting robot commander node");
}