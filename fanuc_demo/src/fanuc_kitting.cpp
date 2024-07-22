#include <fanuc_demo/fanuc_kitting.hpp>

FanucDemo::FanucDemo()
    : Node("fanuc_demo"),
      fanuc_arm_(std::shared_ptr<rclcpp::Node>(std::move(this)), "fanuc_arm"),
      planning_scene_()
{
  // Use upper joint velocity and acceleration limits
  fanuc_arm_.setMaxAccelerationScalingFactor(1.0);
  fanuc_arm_.setMaxVelocityScalingFactor(1.0);
  fanuc_arm_.setPlanningTime(30.0);
  fanuc_arm_.setNumPlanningAttempts(5);

  open_gripper_client_ = this->create_client<example_interfaces::srv::Trigger>("gripper_open");
  close_gripper_client_ = this->create_client<example_interfaces::srv::Trigger>("gripper_close");
  update_antvision_data_client_ = this->create_client<example_interfaces::srv::Trigger>("update_vision_data");

  fanuc_position_publisher_ = this->create_publisher<std_msgs::msg::Float64MultiArray>("/forward_position_controller/commands", 10);

  trays_subscriber_ = this->create_subscription<aprs_interfaces::msg::Trays>("/fanuc_trays_info", 10, bind(&FanucDemo::TraysInfoCallback, this, std::placeholders::_1));

  RCLCPP_INFO(this->get_logger(), "Initialization successful.");
}

FanucDemo::~FanucDemo()
{
  fanuc_arm_.~MoveGroupInterface();
}

void FanucDemo::TraysInfoCallback(const aprs_interfaces::msg::Trays::SharedPtr msg)
{
  if (kit_trays_.empty() && part_trays_.empty()){
    kit_trays_ = msg->kit_trays;
    part_trays_ = msg->part_trays;
  }
}

void FanucDemo::FanucSendHome()
{
  fanuc_arm_.setNamedTarget("home");
  auto plan = FanucPlantoTarget();
  if (plan.first)
  {
    FanucSendTrajectory(plan.second);
  }

}

std::pair<bool, moveit_msgs::msg::RobotTrajectory>  FanucDemo::FanucPlantoTarget()
{
  moveit::planning_interface::MoveGroupInterface::Plan plan;
  bool success = static_cast<bool>(fanuc_arm_.plan(plan));
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

std::pair<bool,moveit_msgs::msg::RobotTrajectory> FanucDemo::FanucPlanCartesian(
    std::vector<geometry_msgs::msg::Pose> waypoints, double vsf, double asf, bool avoid_collisions)
{
  moveit_msgs::msg::RobotTrajectory trajectory;

  double path_fraction = fanuc_arm_.computeCartesianPath(waypoints, 0.01, 0.0, trajectory, avoid_collisions);

  if (path_fraction < 0.9)
  {
    RCLCPP_ERROR(get_logger(), "Unable to generate trajectory through waypoints");
    return std::make_pair(false, trajectory);
  }

  // Retime trajectory
  robot_trajectory::RobotTrajectory rt(fanuc_arm_.getCurrentState()->getRobotModel(), "floor_robot");
  rt.setRobotTrajectoryMsg(*fanuc_arm_.getCurrentState(), trajectory);
  totg_.computeTimeStamps(rt, vsf, asf);
  rt.getRobotTrajectoryMsg(trajectory);

  return std::make_pair(true, trajectory);
}

bool FanucDemo::FanucSendTrajectory(moveit_msgs::msg::RobotTrajectory trajectory)
{
  // Send Entire Trajectory
  
  for (auto point : trajectory.joint_trajectory.points)
  {
    point.positions[2] -= point.positions[1];
    std_msgs::msg::Float64MultiArray joint_positions;
    joint_positions.data = point.positions;
    fanuc_position_publisher_->publish(joint_positions);
    usleep(100000);
    // std::stringstream joint_position_ss;
    // for (auto position : point.positions){
    //   joint_position_ss << position << " ";
    // }
    // RCLCPP_INFO(get_logger(),"Joint Positions: %s",joint_position_ss.str().c_str());
  }
  

  // Send Last Point of Trajectory

  // trajectory.joint_trajectory.points.back().positions[2] -= trajectory.joint_trajectory.points.back().positions[1];

  // std_msgs::msg::Float64MultiArray joint_positions;
  // trajectory.joint_trajectory.points.back().positions;
  // joint_positions.data = trajectory.joint_trajectory.points.back().positions;
  // fanuc_position_publisher_->publish(joint_positions);
  // usleep(100000);
  // std::stringstream joint_position_ss;
  // for (auto position : trajectory.joint_trajectory.points.back().positions){
  //   joint_position_ss << position << " ";
  // }
  // RCLCPP_INFO(get_logger(),"Joint Positions: %s",joint_position_ss.str().c_str());

  return true;
}

bool FanucDemo::MoveToJoints(std::vector<double> joint_values)
{
  std_msgs::msg::Float64MultiArray joint_positions;
  joint_positions.data = joint_values;
  fanuc_position_publisher_->publish(joint_positions);
 
  return true;
}

bool FanucDemo::FanucOpenGripper()
{
  auto request = std::make_shared<example_interfaces::srv::Trigger::Request>();
  auto result = open_gripper_client_->async_send_request(request);
  return result.get()->success;
}

bool FanucDemo::FanucCloseGripper()
{
  auto request = std::make_shared<example_interfaces::srv::Trigger::Request>();
  auto result = close_gripper_client_->async_send_request(request);
  return result.get()->success;
}

double FanucDemo::calculateDistance(const geometry_msgs::msg::Pose& pose1, const geometry_msgs::msg::Pose& pose2) {
  double dx = pose1.position.x - pose2.position.x;
  double dy = pose1.position.y - pose2.position.y;
  double dz = pose1.position.z - pose2.position.z;
  return std::sqrt(dx * dx + dy * dy + dz * dz);
}

bool FanucDemo::BuildTarget(){
  int joint_num = 1;
  for (auto joint_value: fanuc_arm_.getCurrentJointValues()) {
     RCLCPP_INFO_STREAM(get_logger(), "Joint " << joint_num << " : " << joint_value);
     joint_num++;
  }

  // Perform Joint Space Planning

  geometry_msgs::msg::Pose robot_pose = fanuc_arm_.getCurrentPose().pose;

  RCLCPP_INFO_STREAM(get_logger(),"The current Robot Pose is X: " << robot_pose.position.x << " Y: " << robot_pose.position.y << " Z: " << robot_pose.position.z);

  robot_pose.position.z = 0.06;
  robot_pose.position.x = -0.052;
  robot_pose.position.y = 0.298;

  RCLCPP_INFO_STREAM(get_logger(), "The New Target Robot Pose is X: "
                                           << robot_pose.position.x
                                           << " Y: " << robot_pose.position.y
                                           << " Z: " << robot_pose.position.z);

  fanuc_arm_.setPoseTarget(robot_pose);
  auto plan = FanucPlantoTarget();
  if (plan.first)
  {
    FanucSendTrajectory(plan.second);
  }

  // Perform Cartesian Path Planning

  /*
  geometry_msgs::msg::Pose robot_pose = fanuc_arm_.getCurrentPose().pose;

  RCLCPP_INFO_STREAM(get_logger(),"The current Robot Pose is X: " << robot_pose.position.x << " Y: " << robot_pose.position.y << " Z: " << robot_pose.position.z);

  std::vector<geometry_msgs::msg::Pose> waypoints;
  robot_pose.position.x += 0.1;
  waypoints.push_back(robot_pose);

  auto plan = FanucPlanCartesian(waypoints, 0.1, 0.1, true);
  if (plan.first)
  {
    FanucSendTrajectory(plan.second);
  } 
  */
 
  return true;
}

bool FanucDemo::PickPart(const std::string& slot_name, const std::string& tray_name)
{
  // geometry_msgs::msg::TransformStamped tray_transform = tf_buffer->lookupTransform("fanuc_base_link", tray_name, tf2::TimePointZero);
  geometry_msgs::msg::TransformStamped pick_transform = tf_buffer->lookupTransform("fanuc_base_link", slot_name, tf2::TimePointZero);

  RCLCPP_INFO_STREAM(get_logger(), "Picking part " << slot_name << " at X: " << pick_transform.transform.translation.x
                                                      << " Y: " << pick_transform.transform.translation.y
                                                      << " Z: " << pick_transform.transform.translation.z);

  geometry_msgs::msg::Pose robot_pose = fanuc_arm_.getCurrentPose().pose;

  std::vector<geometry_msgs::msg::Pose> waypoints;

  geometry_msgs::msg::Pose pick_pose;
  pick_pose.position.x = pick_transform.transform.translation.x;
  pick_pose.position.y = pick_transform.transform.translation.y;
  pick_pose.position.z = pick_transform.transform.translation.z + 0.07;
  pick_pose.orientation = robot_pose.orientation;

  waypoints.push_back(pick_pose);

  pick_pose.position.z -= 0.07;

  waypoints.push_back(pick_pose);

  auto plan = FanucPlanCartesian(waypoints, 0.1, 0.1, false);
  if (plan.first)
  {
    FanucSendTrajectory(plan.second);
  } 
  else
  {
    FanucOpenGripper();
    return false;
  }

  robot_pose = fanuc_arm_.getCurrentPose().pose;
  while (calculateDistance(robot_pose,pick_pose) > 0.001) {
    robot_pose = fanuc_arm_.getCurrentPose().pose;
    RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 1000, "Moving to pick pose");
  }

  FanucCloseGripper();

  waypoints.clear();
  
  pick_pose.position.z += 0.07;

  waypoints.push_back(pick_pose);

  plan = FanucPlanCartesian(waypoints, 0.1, 0.1, false);
  if (plan.first)
  {
    FanucSendTrajectory(plan.second);
  } 
  else
  {
    FanucOpenGripper();
    return false;
  }

  return true;
}

bool FanucDemo::PlacePart(const std::string& slot_name, const std::string& tray_name)
{
  RCLCPP_INFO_STREAM(get_logger(), tray_name);

  // geometry_msgs::msg::TransformStamped tray_transform = tf_buffer->lookupTransform("fanuc_base_link", tray_name, tf2::TimePointZero);
  geometry_msgs::msg::TransformStamped place_transform = tf_buffer->lookupTransform("fanuc_base_link", slot_name, tf2::TimePointZero);

  RCLCPP_INFO_STREAM(get_logger(), "Placing part in slot " << slot_name << " at X: " << place_transform.transform.translation.x
                                                            << " Y: " << place_transform.transform.translation.y
                                                            << " Z: " << place_transform.transform.translation.z);

  geometry_msgs::msg::Pose robot_pose = fanuc_arm_.getCurrentPose().pose;

  std::vector<geometry_msgs::msg::Pose> waypoints;
  
  geometry_msgs::msg::Pose place_pose;
  place_pose.position.x = place_transform.transform.translation.x;
  place_pose.position.y = place_transform.transform.translation.y;
  place_pose.position.z = place_transform.transform.translation.z + 0.07;
  place_pose.orientation = robot_pose.orientation;

  waypoints.push_back(place_pose);

  place_pose.position.z -= 0.06;

  waypoints.push_back(place_pose);

  auto plan = FanucPlanCartesian(waypoints, 0.1, 0.1, false);
  if (plan.first)
  {
    FanucSendTrajectory(plan.second);
  } 
  else
  {
    FanucOpenGripper();
    return false;
  }

  robot_pose = fanuc_arm_.getCurrentPose().pose;
  while (calculateDistance(robot_pose,place_pose) > 0.001) {
    robot_pose = fanuc_arm_.getCurrentPose().pose;
    RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 1000, "Moving to place pose");
  }

  FanucOpenGripper();

  waypoints.clear();
  
  place_pose.position.z += 0.06;

  waypoints.push_back(place_pose);

  plan = FanucPlanCartesian(waypoints, 0.1, 0.1, false);
  if (plan.first)
  {
    FanucSendTrajectory(plan.second);
  } 
  else
  {
    FanucOpenGripper();
    return false;
  }
  
  return true;
}

void FanucDemo::FillKitSlots(std::vector<aprs_interfaces::msg::SlotInfo>& kit_tray_slots, const uint8_t part_type)
{
  for (auto& kit_tray_slot : kit_tray_slots)
  {
    if (!kit_tray_slot.occupied)
    {
      bool slot_filled = false;
      for (auto& part_tray : part_trays_)
      {
        if (part_tray.identifier == part_type)
        {
          for (auto& part_tray_slot : part_tray.slots)
          {
            if (part_tray_slot.occupied)
            {
              PickPart(part_tray_slot.name, part_tray.name);
              PlacePart(kit_tray_slot.name,kit_tray_slot.name.substr(0, kit_tray_slot.name.size()-5));
              kit_tray_slot.occupied = true;
              part_tray_slot.occupied = false;
              slot_filled = true;
              break;
            }
          }
        }
        if (slot_filled)
        {
          break;
        }
      }
    }
  }
}

void FanucDemo::EmptyKitSlots(std::vector<aprs_interfaces::msg::SlotInfo>& kit_tray_slots, const uint8_t part_type)
{
  for (auto& kit_tray_slot : kit_tray_slots)
  {
    if (kit_tray_slot.occupied)
    {
      bool slot_empty = false;
      for (auto& part_tray : part_trays_)
      {
        if (part_tray.identifier == part_type)
        {
          for (auto& part_tray_slot : part_tray.slots)
          {
            if (!part_tray_slot.occupied)
            {
              PickPart(kit_tray_slot.name,kit_tray_slot.name.substr(0, kit_tray_slot.name.size()-5));
              PlacePart(part_tray_slot.name, part_tray.name);
              kit_tray_slot.occupied = false;
              part_tray_slot.occupied = true;
              slot_empty = true;
              break;
            }
          }
        }
        if (slot_empty)
        {
          break;
        }
      }
    }
  }
}

bool FanucDemo::FillKitTray()
{
  for (auto& kit_tray : kit_trays_)
  {
    FillKitSlots(kit_tray.large_gear_slots, aprs_interfaces::msg::Object::LARGE_GEAR_TRAY);
    FillKitSlots(kit_tray.medium_gear_slots, aprs_interfaces::msg::Object::MEDIUM_GEAR_TRAY);
    FillKitSlots(kit_tray.small_gear_slots, aprs_interfaces::msg::Object::SMALL_GEAR_TRAY);
  }
  return true;
}

bool FanucDemo::EmptyKitTray()
{
  for (auto& kit_tray : kit_trays_)
  {
    EmptyKitSlots(kit_tray.large_gear_slots, aprs_interfaces::msg::Object::LARGE_GEAR_TRAY);
    EmptyKitSlots(kit_tray.medium_gear_slots, aprs_interfaces::msg::Object::MEDIUM_GEAR_TRAY);
    EmptyKitSlots(kit_tray.small_gear_slots, aprs_interfaces::msg::Object::SMALL_GEAR_TRAY);
  }
  return true;
}

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);

  auto fanuc_demo = std::make_shared<FanucDemo>();

  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(fanuc_demo);
  std::thread([&executor]() { executor.spin(); }).detach();

  sleep(3);

  // std::vector<double> joint_values{2.79,0.0,0.0,0.0,-1.54,0.0};
  
  // fanuc_demo->MoveToJoints(joint_values);
  fanuc_demo->FanucSendHome();
  fanuc_demo->FillKitTray();
  fanuc_demo->EmptyKitTray();
  fanuc_demo->FanucSendHome();
  // fanuc_demo->BuildTarget();

  rclcpp::shutdown();
}
