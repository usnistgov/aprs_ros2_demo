#include <robot_commander/robot_commander.hpp>

#include <rclcpp/rclcpp.hpp>

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);

  moveit::planning_interface::MoveGroupInterface::Options opt("fanuc_arm");
  opt.move_group_namespace = "fanuc";

  auto fanuc_commander = std::make_shared<RobotCommander>("fanuc_robot_commander", opt);

  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(fanuc_commander);

  executor.spin();

  // std::thread([&executor]() { executor.spin(); }).detach();

  rclcpp::shutdown();
}