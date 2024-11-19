#include <robot_commander/robot_commander.hpp>

#include <rclcpp/rclcpp.hpp>

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);

  // // Create MoveIt params
  // std::string planning_group_name = "fanuc_arm";
  // std::string description_param_name = "robot_description";
  // std::string move_group_ns = "fanuc";

  // moveit::planning_interface::MoveGroupInterface::Options opt(
  //   planning_group_name,
  //   description_param_name,
  //   move_group_ns
  // );

  std::string node_name = "fanuc_robot_commander";

  auto robot_commander = std::make_shared<RobotCommander>(node_name);

  // Add node to an executor
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(robot_commander);

  executor.spin();

  rclcpp::shutdown();
}