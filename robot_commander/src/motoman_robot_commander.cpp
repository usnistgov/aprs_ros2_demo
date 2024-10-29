#include <robot_commander/motoman_robot_commander.hpp>

RobotCommander::RobotCommander(std::string node_name, moveit::planning_interface::MoveGroupInterface::Options opt)
: Node(node_name),
  planning_interface_(std::shared_ptr<rclcpp::Node>(std::move(this)), opt, std::shared_ptr<tf2_ros::Buffer>()),
  planning_scene_("motoman"),
  group_name(opt.group_name)
{
  RCLCPP_INFO(get_logger(), "Starting robot commander node");

  client_cb_group_ = create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

  // Start up service servers
  pick_srv_ = create_service<aprs_interfaces::srv::Pick>(
    "/motoman/pick_from_slot", 
    std::bind(&RobotCommander::pick_from_slot_cb, this, std::placeholders::_1, std::placeholders::_2),
    rclcpp::ServicesQoS(),
    client_cb_group_
  );

  place_srv_ = create_service<aprs_interfaces::srv::Place>(
    "/motoman/place_in_slot", 
    std::bind(&RobotCommander::place_in_slot_cb, this, std::placeholders::_1, std::placeholders::_2),
    rclcpp::ServicesQoS(),
    client_cb_group_
  );

  move_to_named_pose_srv_ = create_service<aprs_interfaces::srv::MoveToNamedPose>(
    "/motoman/move_to_named_pose", 
    std::bind(&RobotCommander::move_to_named_pose_cb, this, std::placeholders::_1, std::placeholders::_2),
    rclcpp::ServicesQoS(),
    client_cb_group_
  );

  // Create publishers
  joint_command_publisher_ = create_publisher<std_msgs::msg::Float64MultiArray>("forward_position_controller/commands", 10);

  // Create clients
  gripper_client_ = create_client<aprs_interfaces::srv::PneumaticGripperControl>("/motoman/actuate_gripper");
}

bool RobotCommander::actuate_gripper(bool enable)
{
  auto req = std::make_shared<aprs_interfaces::srv::PneumaticGripperControl::Request>();
  req->enable = enable;

  auto future = gripper_client_->async_send_request(req);

  future.wait();

  return future.get()->success;
}

std::pair<bool, std::string> RobotCommander::move_to_named_pose(const std::string &pose_name)
{
  // check to see if requested name exists for planning group
  std::vector<std::string> named_targets = planning_interface_.getNamedTargets();

  if (std::find(named_targets.begin(), named_targets.end(), pose_name) == named_targets.end()) {
    return std::make_pair(false, "Pose " + pose_name + " not found");
  }
  // planning_interface_.getCurrentState();

  planning_interface_.setStartStateToCurrentState();

  // Set target
  planning_interface_.setNamedTarget(pose_name);
  
  // Plan to target
  std::pair<bool, moveit_msgs::msg::RobotTrajectory> plan = plan_to_target();

  //
  if (!plan.first) {
    return std::make_pair(false, "Unable to plan to " + pose_name);
  }

  // Send trajectory to controller
  planning_interface_.execute(plan.second);

  return std::make_pair(true, "Sent trajectory to move to" + pose_name);
}

std::pair<bool, std::string> RobotCommander::pick_part(const std::string &slot_name)
{
  std::pair<bool, moveit_msgs::msg::RobotTrajectory> plan;

  // Get slot transform from TF
  geometry_msgs::msg::Transform slot_t;
  try {
    slot_t = tf_buffer->lookupTransform(base_link, slot_name, tf2::TimePointZero).transform;
  } catch (tf2::LookupException& e) {
    return std::make_pair(false, "Not a valid frame name");
  }

  // Move to pose above slot
  geometry_msgs::msg::Pose above_slot;
  above_slot = build_robot_pose(slot_t.translation.x, slot_t.translation.y, slot_t.translation.z + pick_offset, 0.0);

  plan = plan_cartesian(above_slot);
  
  if (!plan.first)
    return std::make_pair(false, "Unable to plan to above slot");

  planning_interface_.execute(plan.second);

  actuate_gripper(false);

  sleep(0.5);

  // Move to pick pose
  geometry_msgs::msg::Pose pick_pose;
  pick_pose = build_robot_pose(slot_t.translation.x, slot_t.translation.y, slot_t.translation.z, 0.0);

  plan = plan_cartesian(pick_pose);
  
  if (!plan.first)
    return std::make_pair(false, "Unable to plan to pick pose");

  planning_interface_.execute(plan.second);

  sleep(0.5);

  actuate_gripper(true);

  sleep(0.5);

  holding_part = true;

  // Move back up
  plan = plan_cartesian(above_slot);
  
  if (!plan.first)
    return std::make_pair(false, "Unable to plan to above slot");

  planning_interface_.execute(plan.second);

  return std::make_pair(true, "Successfully picked part");
}

std::pair<bool, std::string> RobotCommander::place_part(const std::string &slot_name)
{
  std::pair<bool, moveit_msgs::msg::RobotTrajectory> plan;

  // Get slot transform from TF
  geometry_msgs::msg::Transform slot_t;

  try {
    slot_t = tf_buffer->lookupTransform(base_link, slot_name, tf2::TimePointZero).transform;
  } catch (tf2::LookupException& e) {
    return std::make_pair(false, "Not a valid frame name");
  }

  // Move to pose above slot
  geometry_msgs::msg::Pose above_slot;
  above_slot = build_robot_pose(slot_t.translation.x, slot_t.translation.y, slot_t.translation.z + place_offset, 0.0);

  plan = plan_cartesian(above_slot);
  
  if (!plan.first)
    return std::make_pair(false, "Unable to plan to above slot");

  planning_interface_.execute(plan.second);

  // Move to place pose
  geometry_msgs::msg::Pose place_pose;
  place_pose = build_robot_pose(slot_t.translation.x, slot_t.translation.y, slot_t.translation.z, 0.0);

  plan = plan_cartesian(place_pose);
  
  if (!plan.first)
    return std::make_pair(false, "Unable to plan to place pose");

  planning_interface_.execute(plan.second);

  actuate_gripper(false);

  holding_part = false;

  // Move back up
  plan = plan_cartesian(above_slot);
  
  if (!plan.first)
    return std::make_pair(false, "Unable to plan to above slot");

  planning_interface_.execute(plan.second);

  return std::make_pair(true, "Successfully placed part");
}

void RobotCommander::pick_from_slot_cb(
  const std::shared_ptr<aprs_interfaces::srv::Pick::Request> request,
  std::shared_ptr<aprs_interfaces::srv::Pick::Response> response)
{
  if (holding_part) {
    response->status = "Unable to pick, already holding a part";
    response->success = false;
    return;
  }

  auto result = pick_part(request->frame_name);

  response->success = result.first;
  response->status = result.second;
}

void RobotCommander::place_in_slot_cb(
  const std::shared_ptr<aprs_interfaces::srv::Place::Request> request,
  std::shared_ptr<aprs_interfaces::srv::Place::Response> response)
{
  if (!holding_part) {
    response->status = "Unable to place, not holding a part";
    response->success = false;
    return;
  }

  auto result = place_part(request->frame_name);

  response->success = result.first;
  response->status = result.second;
}

void RobotCommander::move_to_named_pose_cb(
  const std::shared_ptr<aprs_interfaces::srv::MoveToNamedPose::Request> request,
  std::shared_ptr<aprs_interfaces::srv::MoveToNamedPose::Response> response)
{
  auto result = move_to_named_pose(request->name);

  response->success = result.first;
  response->status = result.second;
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

std::pair<bool, moveit_msgs::msg::RobotTrajectory> RobotCommander::plan_cartesian(geometry_msgs::msg::Pose pose)
{
  moveit_msgs::msg::RobotTrajectory trajectory;

  std::vector<geometry_msgs::msg::Pose> waypoints = {pose};

  double path_fraction = planning_interface_.computeCartesianPath(waypoints, 0.01, 0.0, trajectory, false);

  if (path_fraction < 1.0)
  {
    RCLCPP_ERROR(get_logger(), "Unable to generate trajectory through waypoints");
    return std::make_pair(false, trajectory);
  }

  // Retime trajectory
  robot_trajectory::RobotTrajectory rt(planning_interface_.getCurrentState()->getRobotModel(), group_name);
  rt.setRobotTrajectoryMsg(*planning_interface_.getCurrentState(), trajectory);
  totg_.computeTimeStamps(rt, vsf, asf);
  
  rt.getRobotTrajectoryMsg(trajectory);

  return std::make_pair(true, trajectory);
}

double RobotCommander::largest_error(std::vector<double> v1, std::vector<double> v2)
{
  std::vector<double> d;

  for (size_t i=0; i<v1.size(); i++) {
    d.push_back(std::abs(v1[i] - v2[i]));
  }

  return *std::max_element(std::begin(d), std::end(d));
} 

geometry_msgs::msg::Pose RobotCommander::build_robot_pose(double x, double y, double z, double rotation)
{
  geometry_msgs::msg::Pose p;
  p.position.x = x;
  p.position.y = y;
  p.position.z = z;

  tf2::Quaternion q;
  q.setRPY(gripper_roll, gripper_pitch, rotation);

  p.orientation.x = q.getX();
  p.orientation.y = q.getY();
  p.orientation.z = q.getZ();
  p.orientation.w = q.getW();

  return p;
}