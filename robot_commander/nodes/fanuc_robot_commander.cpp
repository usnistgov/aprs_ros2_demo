#include <robot_commander/robot_commander.hpp>

#include <rclcpp/rclcpp.hpp>

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);

  auto fanuc_commander = std::make_shared<RobotCommander>("fanuc_robot_commander");

  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(fanuc_commander);

  executor.spin();

  // std::thread([&executor]() { executor.spin(); }).detach();

  rclcpp::shutdown();
}