#include <robot_commander/motoman_robot_commander.hpp>

#include <rclcpp/rclcpp.hpp>

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);

  auto motoman_commander = std::make_shared<RobotCommander>("motoman_robot_commander", "motoman_arm");

  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(motoman_commander);

  executor.spin();

  // std::thread([&executor]() { executor.spin(); }).detach();

  rclcpp::shutdown();
}