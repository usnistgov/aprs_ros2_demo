#ifndef FANUC_DEMO__FANUC_KITTING_HPP_
#define FANUC_DEMO__FANUC_KITTING_HPP_

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

#include <aprs_interfaces/msg/trays.hpp>
#include <aprs_interfaces/msg/slot_info.hpp>
#include <aprs_interfaces/msg/kit_tray.hpp>
#include <aprs_interfaces/msg/part_tray.hpp>
#include <aprs_interfaces/msg/object.hpp>

#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>

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
  bool FillKitTray();
  void FillKitSlots(std::vector<aprs_interfaces::msg::SlotInfo>& kit_tray_slots, const uint8_t part_type);
  bool EmptyKitTray();
  void EmptyKitSlots(std::vector<aprs_interfaces::msg::SlotInfo>& kit_tray_slots, const uint8_t part_type);
  bool PickPart(const std::string& slot_name, const std::string& tray_name);
  bool PlacePart(const std::string& slot_name, const std::string& tray_name);
  bool ReplicateTeachTable();

private:
  // Robot Move Functions
  std::pair<bool, moveit_msgs::msg::RobotTrajectory> FanucPlantoTarget();
  std::pair<bool, moveit_msgs::msg::RobotTrajectory> FanucPlanCartesian(std::vector<geometry_msgs::msg::Pose> waypoints, double vsf, double asf, bool avoid_collisions);
  bool FanucSendTrajectory(moveit_msgs::msg::RobotTrajectory trajectory);
  double calculateDistance(const geometry_msgs::msg::Pose& pose1, const geometry_msgs::msg::Pose& pose2);

  // MoveIt Interfaces
  moveit::planning_interface::MoveGroupInterface fanuc_arm_;
  moveit::planning_interface::PlanningSceneInterface planning_scene_;
  trajectory_processing::TimeOptimalTrajectoryGeneration totg_;

  // Clients
  rclcpp::Client<example_interfaces::srv::Trigger>::SharedPtr open_gripper_client_;
  rclcpp::Client<example_interfaces::srv::Trigger>::SharedPtr close_gripper_client_;
  rclcpp::Client<example_interfaces::srv::Trigger>::SharedPtr update_antvision_data_client_;

  // Publishers
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr fanuc_position_publisher_;

  // Subscribers
  rclcpp::Subscription<aprs_interfaces::msg::Trays>::SharedPtr trays_subscriber_;

  // TF
  std::unique_ptr<tf2_ros::Buffer> tf_buffer = std::make_unique<tf2_ros::Buffer>(get_clock());
  std::shared_ptr<tf2_ros::TransformListener> tf_listener = std::make_shared<tf2_ros::TransformListener>(*tf_buffer);

  // APRS Demo Objects
  std::vector<aprs_interfaces::msg::KitTray> kit_trays_;
  std::vector<aprs_interfaces::msg::PartTray> part_trays_;

  // Callbacks
  void TraysInfoCallback(const aprs_interfaces::msg::Trays::SharedPtr msg);
};

#endif  // FANUC_DEMO__FANUC_KITTING_HPP_