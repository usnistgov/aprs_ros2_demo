#include <robot_commander/motoman_robot_commander.hpp>

RobotCommander::RobotCommander(std::string node_name, moveit::planning_interface::MoveGroupInterface::Options opt)
: Node(node_name),
  planning_interface_(std::shared_ptr<rclcpp::Node>(std::move(this)), opt, std::shared_ptr<tf2_ros::Buffer>()),
  planning_scene_("motoman"),
  group_name(opt.group_name),
  totg_(0.1, 1.0)
{
  RCLCPP_INFO(get_logger(), "Starting robot commander node");

  planning_interface_.setMaxAccelerationScalingFactor(0.1);
  planning_interface_.setMaxVelocityScalingFactor(0.1);
  planning_interface_.setPlanningTime(10.0);
  planning_interface_.setNumPlanningAttempts(5);
  planning_interface_.allowReplanning(true);
  planning_interface_.setReplanAttempts(5);

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

  initialize_planning_scene_srv_ = create_service<example_interfaces::srv::Trigger>(
    "/fanuc/initialize_planning_scene", 
    std::bind(&RobotCommander::initialize_planning_scene_cb, this, std::placeholders::_1, std::placeholders::_2),
    rclcpp::ServicesQoS(),
    client_cb_group_
  );

  // Create publishers
  // joint_command_publisher_ = create_publisher<std_msgs::msg::Float64MultiArray>("forward_position_controller/commands", 10);

  // Create clients
  gripper_client_ = create_client<aprs_interfaces::srv::PneumaticGripperControl>("/motoman/actuate_gripper");

  // trajectory_client_ = rclcpp_action::create_client<control_msgs::action::FollowJointTrajectory>(this, "/motoman/joint_trajectory_controller/follow_joint_trajectory");
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

  for (auto name : plan.second.joint_trajectory.joint_names) {
    RCLCPP_INFO_STREAM(get_logger(), name);
  }

  for (auto pos : plan.second.joint_trajectory.points.front().positions) {
    RCLCPP_INFO_STREAM(get_logger(), pos);
  }

  // RCLCPP_INFO_STREAM(get_logger(), "Trajectory has " << plan.second.joint_trajectory.points.size() << " points");

  // for(auto point: plan.second.joint_trajectory.points){
  //   rclcpp::Time time(point.time_from_start.sec, point.time_from_start.nanosec);
  //   RCLCPP_INFO_STREAM(get_logger(), "Time from start: " << time.seconds());
  // }

  // return std::make_pair(false, "Testing");

  if (!plan.first) {
    return std::make_pair(false, "Unable to plan to " + pose_name);
  }

  // for (auto name : planning_interface_.getJointNames()) {
  //   RCLCPP_INFO_STREAM(get_logger(), name);
  // } 

  planning_interface_.execute(plan.second);

  // Send trajectory to controller
  // send_trajectory(plan.second);

  return std::make_pair(true, "Sent trajectory to move to " + pose_name);
}

std::pair<bool, std::string> RobotCommander::pick_part(const std::string &slot_name)
{
  std::pair<bool, moveit_msgs::msg::RobotTrajectory> plan;

  geometry_msgs::msg::Pose current_pose = planning_interface_.getCurrentPose().pose;
  current_pose.position.z += 0.1;
  plan = plan_cartesian(current_pose);
  
  if (!plan.first)
    return std::make_pair(false, "Unable to plan up cartesian");

  planning_interface_.execute(plan.second);

  // // Get slot transform from TF
  // geometry_msgs::msg::Transform slot_t;
  // try {
  //   slot_t = tf_buffer->lookupTransform("world", slot_name, tf2::TimePointZero).transform;
  // } catch (tf2::LookupException& e) {
  //   return std::make_pair(false, "Not a valid frame name");
  // }

  // double joint_1_pos = planning_interface_.getCurrentJointValues()[0];
  // RCLCPP_INFO_STREAM(get_logger(),joint_1_pos);

  // if (slot_name.find("conveyor") != std::string::npos && joint_1_pos > 0){
  //   RCLCPP_INFO(get_logger(),"Moving to Above Conveyor");
  //   auto result = move_to_named_pose("above_conveyor");
  //   if (!result.first){
  //     return result;
  //   }
  // } else if (slot_name.find("table") != std::string::npos && joint_1_pos < 0)
  // {
  //   RCLCPP_INFO(get_logger(),"Moving to Above Table");
  //   auto result = move_to_named_pose("above_table");
  //   if (!result.first){
  //     return result;
  //   }
  // }  

  // // Move to pose above slot
  // geometry_msgs::msg::Pose above_slot;
  // above_slot = build_robot_pose(slot_t.translation.x, slot_t.translation.y, slot_t.translation.z + above_slot_offset, 0.0);

  // plan = plan_cartesian(above_slot);
  
  // if (!plan.first)
  //   return std::make_pair(false, "Unable to plan to above slot");

  // planning_interface_.execute(plan.second);

  // actuate_gripper(false);

  // sleep(0.5);

  // // Move to pick pose
  // geometry_msgs::msg::Pose pick_pose;
  // pick_pose = build_robot_pose(slot_t.translation.x, slot_t.translation.y, slot_t.translation.z + pick_offset, 0.0);

  // plan = plan_cartesian(pick_pose);
  
  // if (!plan.first)
  //   return std::make_pair(false, "Unable to plan to pick pose");

  // planning_interface_.execute(plan.second);

  // sleep(0.5);

  // actuate_gripper(true);

  // attached_part_name = slot_objects[slot_name];
  // attached_part_type = slot_types[slot_name];

  // // moveit_msgs::msg::CollisionObject gear;
  // // gear.header.frame_id = "world";
  // // gear.header.stamp = now();
  // // gear.id = attached_part_name;
  // // gear.pose = planning_scene_.getObjectPoses(std::vector<std::string>{attached_part_name})[attached_part_name];
  // // gear.pose.position.z += pick_offset;
  // // gear.operation = moveit_msgs::msg::CollisionObject::MOVE;
  // // planning_scene_.applyCollisionObject(gear);

  // planning_interface_.attachObject(slot_objects[slot_name],"fanuc_tool0");
  // slot_objects[slot_name] = "";
  // slot_types[slot_name] = -1;

  // sleep(0.5);

  // holding_part = true;

  // // Move back up
  // plan = plan_cartesian(above_slot);
  
  // if (!plan.first)
  //   return std::make_pair(false, "Unable to plan to above slot");

  // planning_interface_.execute(plan.second);

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

  send_trajectory(plan.second);

  // Move to place pose
  geometry_msgs::msg::Pose place_pose;
  place_pose = build_robot_pose(slot_t.translation.x, slot_t.translation.y, slot_t.translation.z, 0.0);

  plan = plan_cartesian(place_pose);
  
  if (!plan.first)
    return std::make_pair(false, "Unable to plan to place pose");

  send_trajectory(plan.second);

  actuate_gripper(false);

  planning_interface_.detachObject(attached_part_name);
  sleep(0.5);
  moveit_msgs::msg::CollisionObject gear;
  gear.header.frame_id = "world";
  gear.header.stamp = now();
  gear.id = attached_part_name;
  gear.pose = planning_scene_.getObjectPoses(std::vector<std::string>{attached_part_name})[attached_part_name];
  gear.pose.position.z -= (place_offset-pick_offset);
  gear.operation = moveit_msgs::msg::CollisionObject::MOVE;
  planning_scene_.applyCollisionObject(gear);
  slot_objects[slot_name] = attached_part_name;
  attached_part_name = "";

  holding_part = false;

  // Move back up
  plan = plan_cartesian(above_slot);
  
  if (!plan.first)
    return std::make_pair(false, "Unable to plan to above slot");

  send_trajectory(plan.second);

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
    // Retime trajectory
    robot_trajectory::RobotTrajectory rt(planning_interface_.getCurrentState()->getRobotModel(), group_name);
    rt.setRobotTrajectoryMsg(*planning_interface_.getCurrentState(), trajectory);

    // trajectory_processing::totgComputeTimeStamps(10, rt);

    totg_.computeTimeStamps(rt, vsf, asf);
    rt.getRobotTrajectoryMsg(trajectory);

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

bool RobotCommander::send_trajectory(moveit_msgs::msg::RobotTrajectory trajectory)
{
  // robot_trajectory::RobotTrajectory rt(planning_interface_.getCurrentState()->getRobotModel(), "motoman_arm");
  // rt.setRobotTrajectoryMsg(*planning_interface_.getCurrentState(), trajectory);
  // totg_.computeTimeStamps(rt, vsf, asf);
  // rt.getRobotTrajectoryMsg(trajectory);

  // auto goal = control_msgs::action::FollowJointTrajectory::Goal();
  // goal.trajectory = trajectory.joint_trajectory;

  // for(std::string name :goal.trajectory.joint_names){
  //   control_msgs::msg::JointTolerance path_tolerance;
  //   path_tolerance.name = name;
  //   path_tolerance.position = 0.0;
  //   path_tolerance.velocity = 0.0;
  //   path_tolerance.acceleration = 0.0;

  //   control_msgs::msg::JointTolerance goal_tolerance;
  //   goal_tolerance.name = name;
  //   goal_tolerance.position = 0.05;
  //   goal_tolerance.velocity = 0.05;
  //   goal_tolerance.acceleration = 0.05;

  //   goal.path_tolerance.push_back(path_tolerance);
  //   goal.goal_tolerance.push_back(goal_tolerance);
  // }

  // goal.goal_time_tolerance.sec = 30;

  
  // trajectory_client_->wait_for_action_server();


  // RCLCPP_INFO(get_logger(), "After wait for action server");

  // auto future = trajectory_client_->async_send_goal(goal);

  // auto goal_handle = future.get();

  // RCLCPP_INFO(get_logger(), "Waiting until motion complete");
  // while(goal_handle->get_status() != action_msgs::msg::GoalStatus::STATUS_SUCCEEDED){
  //   sleep(0.1);
  // }

  return true;
}

moveit_msgs::msg::CollisionObject RobotCommander::create_collision_object(
  std::string name, std::string parent_frame, std::string mesh_file, geometry_msgs::msg::Pose model_pose, int operation)
{
  moveit_msgs::msg::CollisionObject collision;

  collision.id = name;
  collision.header.frame_id = parent_frame;

  shape_msgs::msg::Mesh mesh;
  shapes::ShapeMsg mesh_msg;

  std::string package_share_directory = ament_index_cpp::get_package_share_directory("robot_commander");
  std::stringstream path;
  path << "file://" << package_share_directory << "/meshes/" << mesh_file;
  std::string model_path = path.str();

  shapes::Mesh *m = shapes::createMeshFromResource(model_path);
  shapes::constructMsgFromShape(m, mesh_msg);

  mesh = boost::get<shape_msgs::msg::Mesh>(mesh_msg);

  collision.meshes.push_back(mesh);
  collision.mesh_poses.push_back(model_pose);

  collision.operation = operation;

  return collision;
}

void RobotCommander::trays_info_cb(
  const aprs_interfaces::msg::Trays::ConstSharedPtr msg)
{
  received_tray_info = true;
  table_trays_info = *msg;
}

void RobotCommander::initialize_planning_scene_cb(
  const std::shared_ptr<example_interfaces::srv::Trigger::Request>,
  std::shared_ptr<example_interfaces::srv::Trigger::Response> response)
{
  if (!received_tray_info){
    response->success = false;
    response->message = "Tray info not yet received";
    return;
  }

  // Clear Planning Scene
  std::vector<std::string> object_ids;
  for(const auto& [object_id,_] : planning_scene_.getObjects()){
    object_ids.push_back(object_id);
  }
  if (!object_ids.empty()){
    planning_scene_.removeCollisionObjects(object_ids);
  }

  geometry_msgs::msg::Pose optical_table_pose;
  planning_scene_.applyCollisionObject(create_collision_object("optical_table","world","optical_table.stl", optical_table_pose),get_object_color(-1));
  
  // Add kit trays
  for (auto kit_tray : table_trays_info.kit_trays){
    planning_scene_.applyCollisionObject(
      create_collision_object(
        kit_tray.name, 
        kit_tray.tray_pose.header.frame_id, 
        tray_stl_names[kit_tray.identifier], 
        kit_tray.tray_pose.pose,
        moveit_msgs::msg::CollisionObject::APPEND
      ),
      get_object_color(kit_tray.identifier)
    );
  }

  // Add part trays
  for (auto part_tray : table_trays_info.part_trays){
    planning_scene_.applyCollisionObject(
      create_collision_object(
        part_tray.name, 
        part_tray.tray_pose.header.frame_id, 
        tray_stl_names[part_tray.identifier], 
        part_tray.tray_pose.pose,
        moveit_msgs::msg::CollisionObject::APPEND
      ), 
      get_object_color(part_tray.identifier)
    );
    
    // Add gears
    for (auto slot : part_tray.slots){
      if (slot.occupied){
        gear_counter[slot.size]++;
        std::string gear_name = gear_names[slot.size] + "_" + std::to_string(gear_counter[slot.size]);
        slot_objects.insert_or_assign(slot.name, gear_name);
        slot_types.insert_or_assign(slot.name, slot.size);

        planning_scene_.applyCollisionObject(
          create_collision_object(
            gear_name, 
            part_tray.name, 
            gear_stl_names[slot.size], 
            slot.slot_pose.pose,
            moveit_msgs::msg::CollisionObject::APPEND
          ),
          get_object_color(slot.size)
        );
      } else{
        slot_objects.insert_or_assign(slot.name, "");
      }
    }     
  }

  response->success = true;
  response->message = "Planning scene objects set!";
}

std_msgs::msg::ColorRGBA RobotCommander::get_object_color(int identifier){
  std_msgs::msg::ColorRGBA color;
  auto values = color_values[object_colors[identifier]];
  color.r = values[0];
  color.g = values[1];
  color.b = values[2];
  color.a = values[3];

  return color;    
}

std::vector<std::string> RobotCommander::split_string(std::string s, std::string delimiter){
  std::vector<std::string> v;
  if(s.empty()){
    return v;
  }
  
  int start = 0;
  do {
    int idx = s.find(delimiter, start);
    if (idx == std::string::npos){
      break;
    }
    int length = idx - start;
    v.push_back(s.substr(start,length));
    start+= (length + delimiter.size());
  } while (true);
  v.push_back(s.substr(start));

  return v;
}