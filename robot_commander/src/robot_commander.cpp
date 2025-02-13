#include <robot_commander/robot_commander.hpp>

RobotCommander::RobotCommander()
: Node("robot_commander")
{
  RCLCPP_INFO(get_logger(), "Starting robot commander node");

  // Declare parameters  
  declare_parameter("robot_name", "");
  declare_parameter("planning_group_name", "");
  declare_parameter("end_effector_link", "");

  declare_parameter("touch_links", std::vector<std::string>());

  declare_parameter("velocity_scaling_factor", -1.0);
  declare_parameter("acceleration_scaling_factor", -1.0);
  declare_parameter("motion_pause", 1.0);

  declare_parameter("gripper_rotation.roll", -1.0);
  declare_parameter("gripper_rotation.pitch", -1.0);

  declare_parameter("offsets.pick", -1.0);
  declare_parameter("offsets.place", -1.0);
  declare_parameter("offsets.above_slot", -1.0);

  // Get parameters
  planning_group_name_ = get_parameter("planning_group_name").as_string();
  robot_name_ = get_parameter("robot_name").as_string();
  end_effector_link_ = get_parameter("end_effector_link").as_string();

  touch_links_ = get_parameter("touch_links").as_string_array();

  vsf_ = get_parameter("velocity_scaling_factor").as_double();
  asf_ = get_parameter("acceleration_scaling_factor").as_double();
  motion_pause_ = get_parameter("motion_pause").as_double();

  gripper_roll_ = get_parameter("gripper_rotation.roll").as_double();
  gripper_pitch_ = get_parameter("gripper_rotation.pitch").as_double();

  pick_offset_ = get_parameter("offsets.pick").as_double();
  place_offset_ = get_parameter("offsets.place").as_double();
  above_slot_offset_ = get_parameter("offsets.above_slot").as_double();

  // Check that all params were set properly
  std::vector<std::string> string_params = {
    planning_group_name_,
    robot_name_,
    end_effector_link_};

  std::vector<double> double_params = {
    vsf_,
    asf_,
    gripper_roll_,
    gripper_pitch_,
    pick_offset_,
    place_offset_,
    above_slot_offset_
  };

  gripper_joint_name = robot_name_ + "_left_finger_joint";

  if ( std::any_of(string_params.begin(), string_params.end(), [](std::string s){return s == "";}) )
  {
    RCLCPP_ERROR(get_logger(), "Parameters not set properly");
    exit(0);
  }
  
  if ( std::any_of(double_params.begin(), double_params.end(), [](double d){return d == -1.0;}) )
  {
    RCLCPP_ERROR(get_logger(), "Parameters not set properly");
    exit(0);
  }

  moveit::planning_interface::MoveGroupInterface::Options opt(
    planning_group_name_,
    "robot_description",
    robot_name_
  );

  planning_interface_ = std::make_unique<moveit::planning_interface::MoveGroupInterface>(
    std::shared_ptr<rclcpp::Node>(std::move(this)), 
    opt, std::shared_ptr<tf2_ros::Buffer>(), 
    rclcpp::Duration::from_seconds(5));

  planning_scene_ = std::make_unique<moveit::planning_interface::PlanningSceneInterface>(robot_name_);

  client_cb_group_ = create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

  // Start up service servers
  pick_srv_ = create_service<aprs_interfaces::srv::Pick>(
    "/" + robot_name_ + "/pick_from_slot", 
    std::bind(&RobotCommander::pick_from_slot_cb, this, std::placeholders::_1, std::placeholders::_2),
    rclcpp::ServicesQoS(),
    client_cb_group_
  );

  place_srv_ = create_service<aprs_interfaces::srv::Place>(
    "/" + robot_name_ + "/place_in_slot", 
    std::bind(&RobotCommander::place_in_slot_cb, this, std::placeholders::_1, std::placeholders::_2),
    rclcpp::ServicesQoS(),
    client_cb_group_
  );

  move_to_named_pose_srv_ = create_service<aprs_interfaces::srv::MoveToNamedPose>(
    "/" + robot_name_ + "/move_to_named_pose", 
    std::bind(&RobotCommander::move_to_named_pose_cb, this, std::placeholders::_1, std::placeholders::_2),
    rclcpp::ServicesQoS(),
    client_cb_group_
  );

  initialize_planning_scene_srv_ = create_service<example_interfaces::srv::Trigger>(
    "/" + robot_name_ + "/initialize_planning_scene", 
    std::bind(&RobotCommander::initialize_planning_scene_cb, this, std::placeholders::_1, std::placeholders::_2),
    rclcpp::ServicesQoS(),
    client_cb_group_
  );

  // Create Subscriber
  trays_info_table_vision_sub_ = this->create_subscription<aprs_interfaces::msg::Trays>(
      "/" + robot_name_ + "/table_trays_info", rclcpp::SensorDataQoS(),
      std::bind(&RobotCommander::table_trays_info_cb, this, std::placeholders::_1));

  trays_info_conveyor_vision_sub_ = this->create_subscription<aprs_interfaces::msg::Trays>(
      "/" + robot_name_ + "/conveyor_trays_info", rclcpp::SensorDataQoS(),
      std::bind(&RobotCommander::conveyor_trays_info_cb, this, std::placeholders::_1));

  robot_changeover_sub_ = this->create_subscription<aprs_interfaces::msg::RobotChangeover>(
      "/robot_changeover_requested", rclcpp::SensorDataQoS(),
      std::bind(&RobotCommander::robot_changeover_cb, this, std::placeholders::_1));

  joint_state_sub_ = this->create_subscription<sensor_msgs::msg::JointState>(
      "/" + robot_name_ +"/joint_states", rclcpp::SensorDataQoS(),
      std::bind(&RobotCommander::joint_state_cb, this, std::placeholders::_1));

  // Create clients
  gripper_client_ = create_client<aprs_interfaces::srv::PneumaticGripperControl>("/" + robot_name_ + "/actuate_gripper");    
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
  std::vector<std::string> named_targets = planning_interface_->getNamedTargets();

  if (std::find(named_targets.begin(), named_targets.end(), pose_name) == named_targets.end()) {
    return std::make_pair(false, "Pose " + pose_name + " not found");
  }

  planning_interface_->setStartStateToCurrentState();

  // Set target
  planning_interface_->setNamedTarget(pose_name);
  
  // Plan to target
  std::pair<bool, moveit_msgs::msg::RobotTrajectory> plan = plan_to_target();

  //
  if (!plan.first) {
    return std::make_pair(false, "Unable to plan to " + pose_name);
  }

  // Send trajectory to controller
  planning_interface_->execute(plan.second);

  return std::make_pair(true, "Sent trajectory to move to " + pose_name);
}

std::pair<bool, std::string> RobotCommander::pick_part(const std::string &slot_name)
{
  // Check if changeover was requested
  PICK_STATE state = PRE_PICK;
  if (changeover_requested) {
    handle_changeover(state);
    return std::make_pair(false, "Changeover requested");
  }

  pick_slot_name = slot_name;
  geometry_msgs::msg::Transform slot_transform;
  // Get slot transform from TF
  try {
    slot_transform = tf_buffer->lookupTransform("world", pick_slot_name, tf2::TimePointZero).transform;
  } catch (tf2::LookupException& e) {
    return std::make_pair(false, "Not a valid frame name");
  }

  // Set poses
  above_slot = build_robot_pose(
    slot_transform.translation.x,
    slot_transform.translation.y,
    slot_transform.translation.z + above_slot_offset_, 
    0.0
  );

  pick_pose = build_robot_pose(
    slot_transform.translation.x,
    slot_transform.translation.y,
    slot_transform.translation.z + pick_offset_, 
    0.0
  );

  // Execute pick states
  state = MOVE_TO_SLOT;
  while (state != FINISHED)
  {
    state = execute_pick(state);

    if (state == PLANNING_FAILURE){
      return std::make_pair(false, "Planning failed");
    } else if (state == EXECUTION_FAILURE){
      return std::make_pair(false, "Execution failed");
    } else if (changeover_requested) {
      handle_changeover(state);
      return std::make_pair(false, "Changeover requested");
    }

    sleep(motion_pause_);
  }

  return std::make_pair(true, "Successfully picked part");
}

PICK_STATE RobotCommander::execute_pick(PICK_STATE state)
{
  std::pair<bool, moveit_msgs::msg::RobotTrajectory> plan;

  switch (state)
  {
  case MOVE_TO_SLOT:
    // Plan to above slot
    plan = plan_cartesian(above_slot);
    
    if (!plan.first)
      return PLANNING_FAILURE;

    // Execute to above slot
    if (planning_interface_->execute(plan.second) != moveit_msgs::msg::MoveItErrorCodes::SUCCESS) {
      return EXECUTION_FAILURE;
    }

    actuate_gripper(false);

    return PRE_GRASP;

  case PRE_GRASP:
    // Move down to gear
    plan = plan_cartesian(pick_pose);
    
    if (!plan.first)
      return PLANNING_FAILURE;

    if (planning_interface_->execute(plan.second) != moveit_msgs::msg::MoveItErrorCodes::SUCCESS) {
      return EXECUTION_FAILURE;
    }

    return GRASP;
  
  case GRASP:
    // Close gripper and add part to planning scene
    actuate_gripper(true);

    attached_part_name = slot_objects[pick_slot_name];
    attached_part_type = slot_types[pick_slot_name];

    planning_interface_->attachObject(slot_objects[pick_slot_name], end_effector_link_, touch_links_);
    slot_objects[pick_slot_name] = "";
    slot_types[pick_slot_name] = -1;

    holding_part = true;

    return POST_GRASP;

  case POST_GRASP:
    // Move back to above slot
    plan = plan_cartesian(above_slot);
  
    if (!plan.first)
      return PLANNING_FAILURE;

    if (planning_interface_->execute(plan.second) != moveit_msgs::msg::MoveItErrorCodes::SUCCESS) {
      return EXECUTION_FAILURE;
    }
    
    return FINISHED;
  
  default:
    return EXECUTION_FAILURE;
  }
}

void RobotCommander::handle_changeover(PICK_STATE state)
{
  std::pair<bool, moveit_msgs::msg::RobotTrajectory> plan;

  switch (state)
  {
  
  case PRE_PICK:
    // Do nothing
    break;

  case MOVE_TO_SLOT:
    // Do nothing
    break;
  
  case PRE_GRASP:
    // Move up
    plan = plan_cartesian(above_slot);
  
    if (!plan.first)
      return;

    if (planning_interface_->execute(plan.second) != moveit_msgs::msg::MoveItErrorCodes::SUCCESS) {
      return;
    }

    break;
  
  case GRASP:
    // Open gripper
    actuate_gripper(false);
    
    // Detach object in planning scene
    planning_interface_->detachObject();
    holding_part = false;

    // Move up
    plan = plan_cartesian(above_slot);
  
    if (!plan.first)
      return;

    if (planning_interface_->execute(plan.second) != moveit_msgs::msg::MoveItErrorCodes::SUCCESS) {
      return;
    }

    break;
  
  case POST_GRASP:
    // Move down
    plan = plan_cartesian(pick_pose);
  
    if (!plan.first)
      return;

    if (planning_interface_->execute(plan.second) != moveit_msgs::msg::MoveItErrorCodes::SUCCESS) {
      return;
    }

    // Open gripper
    actuate_gripper(false);
    
    // Detach object in planning scene
    planning_interface_->detachObject();
    holding_part = false;

    // Move up
    plan = plan_cartesian(above_slot);
  
    if (!plan.first)
      return;

    if (planning_interface_->execute(plan.second) != moveit_msgs::msg::MoveItErrorCodes::SUCCESS) {
      return;
    }

    break;
  
  default:
    break;
  }
}

std::pair<bool, std::string> RobotCommander::place_part(const std::string &slot_name)
{
  std::pair<bool, moveit_msgs::msg::RobotTrajectory> plan;

  // Get slot transform from TF
  geometry_msgs::msg::Transform slot_t;

  try {
    slot_t = tf_buffer->lookupTransform("world", slot_name, tf2::TimePointZero).transform;
  } catch (tf2::LookupException& e) {
    return std::make_pair(false, "Not a valid frame name");
  }

  // Move to pose above slot
  geometry_msgs::msg::Pose above_slot;
  above_slot = build_robot_pose(slot_t.translation.x, slot_t.translation.y, slot_t.translation.z + above_slot_offset_, 0.0);

  plan = plan_cartesian(above_slot);
  
  if (!plan.first)
    return std::make_pair(false, "Unable to plan to above slot");

  if (planning_interface_->execute(plan.second) != moveit_msgs::msg::MoveItErrorCodes::SUCCESS) {
    return std::make_pair(false, "Unable to move to above slot");
  }

  sleep(motion_pause_);

  // Move to place pose
  geometry_msgs::msg::Pose place_pose;
  place_pose = build_robot_pose(slot_t.translation.x, slot_t.translation.y, slot_t.translation.z + place_offset_, 0.0);

  plan = plan_cartesian(place_pose);
  
  if (!plan.first)
    return std::make_pair(false, "Unable to plan to place pose");

  if (planning_interface_->execute(plan.second) != moveit_msgs::msg::MoveItErrorCodes::SUCCESS) {
    return std::make_pair(false, "Unable to move to place pose");
  }

  actuate_gripper(false);

  planning_interface_->detachObject(attached_part_name);

  sleep(motion_pause_);

  moveit_msgs::msg::CollisionObject gear;
  gear.header.frame_id = "world";
  gear.header.stamp = now();
  gear.id = attached_part_name;
  gear.pose = planning_scene_->getObjectPoses(std::vector<std::string>{attached_part_name})[attached_part_name];
  gear.pose.position.z -= (place_offset_-pick_offset_);
  gear.operation = moveit_msgs::msg::CollisionObject::MOVE;
  planning_scene_->applyCollisionObject(gear, get_object_color(attached_part_type));
  slot_objects[slot_name] = attached_part_name;
  slot_types[slot_name] = attached_part_type;
  attached_part_name = "";
  attached_part_type = -1;

  holding_part = false;

  // Move back up
  plan = plan_cartesian(above_slot);
  
  if (!plan.first)
    return std::make_pair(false, "Unable to plan to above slot");

  if (planning_interface_->execute(plan.second) != moveit_msgs::msg::MoveItErrorCodes::SUCCESS) {
    return std::make_pair(false, "Unable to move to above slot");
  }

  sleep(motion_pause_);

  if (changeover_requested){
    return std::make_pair(false, "Changeover requested");
  }

  return std::make_pair(true, "Successfully placed part");
}

void RobotCommander::pick_from_slot_cb(
  const std::shared_ptr<aprs_interfaces::srv::Pick::Request> request,
  std::shared_ptr<aprs_interfaces::srv::Pick::Response> response)
{
  if (holding_part && !gripper_open) {
    response->status = "Unable to pick, already holding a part";
    response->success = false;
    return;
  }

  holding_part = false;

  auto result = pick_part(request->frame_name);

  response->success = result.first;
  response->status = result.second;
}

void RobotCommander::place_in_slot_cb(
  const std::shared_ptr<aprs_interfaces::srv::Place::Request> request,
  std::shared_ptr<aprs_interfaces::srv::Place::Response> response)
{
  if (!holding_part && gripper_open) {
    response->status = "Unable to place, not holding a part";
    response->success = false;
    return;
  }

  holding_part = true;

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
  bool success = static_cast<bool>(planning_interface_->plan(plan));
  moveit_msgs::msg::RobotTrajectory trajectory = plan.trajectory;

  if (success)
  {
    retime_trajectory(trajectory);

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

  double path_fraction = planning_interface_->computeCartesianPath(waypoints, 0.01, 0.0, trajectory);

  if (path_fraction < 1.0)
  {
    RCLCPP_ERROR(get_logger(), "Unable to generate trajectory through waypoints");
    return std::make_pair(false, trajectory);
  }

  retime_trajectory(trajectory);

  return std::make_pair(true, trajectory);
}

void RobotCommander::retime_trajectory(moveit_msgs::msg::RobotTrajectory& trajectory)
{
  // Retime trajectory
  robot_trajectory::RobotTrajectory rt(planning_interface_->getCurrentState()->getRobotModel(), planning_group_name_);
  rt.setRobotTrajectoryMsg(*planning_interface_->getCurrentState(), trajectory);
  totg_.computeTimeStamps(rt, vsf_, asf_);
  
  rt.getRobotTrajectoryMsg(trajectory);
}

geometry_msgs::msg::Pose RobotCommander::build_robot_pose(double x, double y, double z, double rotation)
{
  geometry_msgs::msg::Pose p;
  p.position.x = x;
  p.position.y = y;
  p.position.z = z;

  tf2::Quaternion q;
  q.setRPY(gripper_roll_, gripper_pitch_, rotation);

  p.orientation.x = q.getX();
  p.orientation.y = q.getY();
  p.orientation.z = q.getZ();
  p.orientation.w = q.getW();

  return p;
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

void RobotCommander::table_trays_info_cb(
  const aprs_interfaces::msg::Trays::ConstSharedPtr msg)
{
  received_table_tray_info = true;
  table_trays_info = *msg;
}

void RobotCommander::conveyor_trays_info_cb(
  const aprs_interfaces::msg::Trays::ConstSharedPtr msg)
{
  received_conveyor_tray_info = true;
  conveyor_trays_info = *msg;
}

void RobotCommander::robot_changeover_cb(
  const aprs_interfaces::msg::RobotChangeover::ConstSharedPtr msg)
{
  if (msg->fanuc || msg->motoman) {
    changeover_requested = true; 
  }

  if (robot_name_ == "fanuc") {
    disabled = msg->fanuc;
  } 

  if (robot_name_ == "motoman") {
    disabled = msg->motoman;
  }
}

void RobotCommander::joint_state_cb(
  const sensor_msgs::msg::JointState::ConstSharedPtr msg)
{
  for(size_t i=0; i<msg->name.size(); i++){
    if(msg->name[i]==gripper_joint_name){
      gripper_open = msg->position[i]!=0;
    }
  }
  if(gripper_open && attached_part_name!=""){
    planning_interface_->detachObject(attached_part_name);
    attached_part_name = "";
  }
}

void RobotCommander::initialize_planning_scene_cb(
  const std::shared_ptr<example_interfaces::srv::Trigger::Request>,
  std::shared_ptr<example_interfaces::srv::Trigger::Response> response)
{
  // Clear Planning Scene
  std::vector<std::string> object_ids;
  for(const auto& [object_id,_] : planning_scene_->getObjects()){
    object_ids.push_back(object_id);
  }
  if (!object_ids.empty()){
    planning_scene_->removeCollisionObjects(object_ids);
  }

  geometry_msgs::msg::Pose optical_table_pose;
  geometry_msgs::msg::Pose conveyor_belt_pose;
  conveyor_belt_pose.position.x = -0.3937;
  conveyor_belt_pose.position.y = -0.0762;
  conveyor_belt_pose.position.z = 0.0625;
  planning_scene_->applyCollisionObject(create_collision_object("optical_table","world","optical_table.stl", optical_table_pose),get_object_color(-1));
  planning_scene_->applyCollisionObject(create_collision_object("conveyor_belt", "world", "conveyor.stl", conveyor_belt_pose), get_object_color(-1));
  
  if (!received_table_tray_info && !received_conveyor_tray_info){
    response->success = true;
    response->message = "Tray info not yet received";
    return;
  }

  // Create vector of all trays and gears
  std::vector<aprs_interfaces::msg::Tray> all_trays;
  if (received_table_tray_info){
    all_trays.insert(all_trays.end(),table_trays_info.kit_trays.begin(),table_trays_info.kit_trays.end());
    all_trays.insert(all_trays.end(),table_trays_info.part_trays.begin(),table_trays_info.part_trays.end());
  }
  if (received_conveyor_tray_info){
    // all_trays.insert(all_trays.end(),conveyor_trays_info.kit_trays.begin(),conveyor_trays_info.kit_trays.end());
    // all_trays.insert(all_trays.end(),conveyor_trays_info.part_trays.begin(),conveyor_trays_info.part_trays.end());
  }

  // Add trays to planning scene
  for (auto tray : all_trays){
    planning_scene_->applyCollisionObject(
      create_collision_object(
        tray.name, 
        tray.tray_pose.header.frame_id, 
        tray_stl_names[tray.identifier], 
        tray.tray_pose.pose,
        moveit_msgs::msg::CollisionObject::APPEND
      ),
      get_object_color(tray.identifier)
    );

    // Add gears to planning scene
    for (auto slot : tray.slots){
      if (slot.occupied){
        gear_counter[slot.size]++;
        std::string gear_name = gear_names[slot.size] + "_" + std::to_string(gear_counter[slot.size]);
        slot_objects.insert_or_assign(slot.name, gear_name);
        slot_types.insert_or_assign(slot.name, slot.size);

        planning_scene_->applyCollisionObject(
          create_collision_object(
            gear_name, 
            tray.name, 
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

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);

  auto robot_commander = std::make_shared<RobotCommander>();

  // Add node to an executor
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(robot_commander);

  executor.spin();

  rclcpp::shutdown();
}