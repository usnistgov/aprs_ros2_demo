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

  // Create publishers
  joint_command_publisher_ = this->create_publisher<std_msgs::msg::Float64MultiArray>("/forward_postion_controller/commands", 10);

  // Create clients
  open_gripper_client_ = this->create_client<example_interfaces::srv::Trigger>("gripper_open");
  close_gripper_client_ = this->create_client<example_interfaces::srv::Trigger>("gripper_close");


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
  // check to see if requested name exists for planning group
  std::vector<std::string> named_targets = planning_interface_.getNamedTargets();

  if (std::find(named_targets.begin(), named_targets.end(), request->name) == named_targets.end()) {
    response->success = false;
    response->status = "Name " + request->name + " not found";
    return;
  }

  // Set target
  planning_interface_.setNamedTarget(request->name);
  
  // Plan to target
  std::pair<bool, moveit_msgs::msg::RobotTrajectory> plan = plan_to_target();

  //
  if (!plan.first) {
    response->success = false;
    response->status = "Unable to plan to " + request->name;
    return;
  }

  // Send trajectory to controller
  send_trajectory(plan.second);

  response->success = true;
  response->status = "Sent trajectory to move to" + request->name;
  
}

std::pair<bool, moveit_msgs::msg::RobotTrajectory> RobotCommander::plan_to_target()
{
  moveit::planning_interface::MoveGroupInterface::Plan plan;
  bool success = static_cast<bool>(planning_interface_.plan(plan));
  moveit_msgs::msg::RobotTrajectory trajectory = plan.trajectory;

  if (success)
  {
    return std::make_pair(true, trajectory);
  }
  else
  {
    RCLCPP_ERROR(get_logger(), "Unable to generate plan");
    return std::make_pair(false, trajectory);
  }
}

bool RobotCommander::send_trajectory(moveit_msgs::msg::RobotTrajectory trajectory)
{
  // Handle J23 translation
  handle_j23_transform(trajectory);
  
  std_msgs::msg::Float64MultiArray joint_positions;

  for (auto point : trajectory.joint_trajectory.points)
  {
    joint_positions.data = point.positions;
    
    joint_command_publisher_->publish(joint_positions);
    
    usleep(trajectory_spacing_);
  }
}

void RobotCommander::handle_j23_transform(moveit_msgs::msg::RobotTrajectory &trajectory)
{
  for (trajectory_msgs::msg::JointTrajectoryPoint &point : trajectory.joint_trajectory.points)
  {
    point.positions[2] -= point.positions[1];
  }
}