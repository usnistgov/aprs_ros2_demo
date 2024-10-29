#include <robot_commander/motoman_robot_commander.hpp>

#include <rclcpp/rclcpp.hpp>

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);

  moveit::planning_interface::MoveGroupInterface::Options opt("motoman_arm");
  opt.move_group_namespace = "motoman";

  auto motoman_commander = std::make_shared<RobotCommander>("motoman_robot_commander", opt);

  sleep(3);
  
  std::cout << "After sleep" << std::endl;
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(motoman_commander);

  executor.spin();

  // std::thread([&executor]() { executor.spin(); }).detach();

  rclcpp::shutdown();
}