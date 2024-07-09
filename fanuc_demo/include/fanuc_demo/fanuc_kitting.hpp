#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>

#include <unistd.h>
#include <sstream>
#include <cmath>

#include <ament_index_cpp/get_package_share_directory.hpp>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit/trajectory_processing/time_optimal_trajectory_generation.h>
#include <moveit_msgs/msg/collision_object.hpp>

#include <example_interfaces/srv/trigger.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>

class FanucDemo : public rclcpp::Node
{
public:
  /// Constructor
  FanucDemo();

  ~FanucDemo();

  void FanucSendHome();
  bool FanucOpenGripper();
  bool FanucCloseGripper();
  bool BuildTarget();
  bool MoveToJoints(std::vector<double> joint_values);

private:
  // Robot Move Functions
  std::pair<bool, moveit_msgs::msg::RobotTrajectory> FanucPlantoTarget();
  std::pair<bool, moveit_msgs::msg::RobotTrajectory> FanucPlanCartesian(std::vector<geometry_msgs::msg::Pose> waypoints, double vsf, double asf, bool avoid_collisions);
  bool FanucSendTrajectory(moveit_msgs::msg::RobotTrajectory trajectory);

  // MoveIt Interfaces
  moveit::planning_interface::MoveGroupInterface fanuc_arm_;
  moveit::planning_interface::PlanningSceneInterface planning_scene_;
  trajectory_processing::TimeOptimalTrajectoryGeneration totg_;

  // Services
  rclcpp::Client<example_interfaces::srv::Trigger>::SharedPtr open_gripper_client_;
  rclcpp::Client<example_interfaces::srv::Trigger>::SharedPtr close_gripper_client_;

  // Publishers
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr fanuc_position_publisher_;
};
