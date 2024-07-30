#include <robot_commander/robot_commander.hpp>

RobotCommander::RobotCommander(std::string node_name, std::string group_name)
: Node(node_name),
  planning_interface_(std::shared_ptr<rclcpp::Node>(std::move(this)), group_name),
  planning_scene_()
{
  RCLCPP_INFO(get_logger(), "Starting robot commander node");

  // Start up service servers
  pick_srv_ = this->create_service<aprs_interfaces::srv::Pick>(
    "/pick_from_slot", 
    std::bind(&RobotCommander::pick_from_slot_cb, this, std::placeholders::_1, std::placeholders::_2)
  );

  place_srv_ = this->create_service<aprs_interfaces::srv::Place>(
    "/place_in_slot", 
    std::bind(&RobotCommander::place_in_slot_cb, this, std::placeholders::_1, std::placeholders::_2)
  );

  move_to_named_pose_srv_ = this->create_service<aprs_interfaces::srv::MoveToNamedPose>(
    "/move_to_named_pose", 
    std::bind(&RobotCommander::move_to_named_pose_cb, this, std::placeholders::_1, std::placeholders::_2)
  );

}

void RobotCommander::pick_from_slot_cb(
  const std::shared_ptr<aprs_interfaces::srv::Pick::Request> request,
  std::shared_ptr<aprs_interfaces::srv::Pick::Response> response)
{

}

void RobotCommander::place_in_slot_cb(
  const std::shared_ptr<aprs_interfaces::srv::Place::Request> request,
  std::shared_ptr<aprs_interfaces::srv::Place::Response> response)
{
  
} 

void RobotCommander::move_to_named_pose_cb(
  const std::shared_ptr<aprs_interfaces::srv::MoveToNamedPose::Request> request,
  std::shared_ptr<aprs_interfaces::srv::MoveToNamedPose::Response> response)
{
  
}