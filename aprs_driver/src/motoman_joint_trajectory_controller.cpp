#include <aprs_driver/motoman_joint_trajectory_controller.hpp>
#include <controller_interface/controller_interface.hpp>

namespace motoman_controller {

  controller_interface::InterfaceConfiguration MotomanJointTrajectoryController::command_interface_configuration() const
  {
    controller_interface::InterfaceConfiguration config;
    config.type = controller_interface::interface_configuration_type::INDIVIDUAL;

    for (std::string name : joint_names_) {
      config.names.push_back(name + "/position");
    }

    return config;
  }

  controller_interface::InterfaceConfiguration MotomanJointTrajectoryController::state_interface_configuration() const 
  {
    controller_interface::InterfaceConfiguration config;
    
    config.type = controller_interface::interface_configuration_type::INDIVIDUAL;

    for (std::string name : joint_names_) {
      config.names.push_back(name + "/position");
    }

    return config;
  }

  controller_interface::return_type MotomanJointTrajectoryController::update(
    const rclcpp::Time&,
    const rclcpp::Duration&) 
  {
    // The update loop is now just a status checker.
    // It triggers the thread and checks for final completion.
    
    if (received_goal_) {
      std::lock_guard<std::mutex> lock(action_mutex_);
      start_execution_ = true;  // Trigger the background thread
      received_goal_ = false;
      executing_ = true;
    }

    if (executing_ && !sending_trajectory_ && !start_execution_) {
      // Monitor final position reached (Non-blocking)
      std::vector<double> joint_errors;
      std::vector<float> goal_positions;

      // Extract goal positions from the last point of the trajectory
      auto trajectory_points = current_goal_->get_goal()->trajectory.points;
      if (trajectory_points.empty()) return controller_interface::return_type::OK;

      for (auto name : joint_names_){
        for (int i = 0; i < int(current_goal_->get_goal()->trajectory.joint_names.size()); i++){
          if (current_goal_->get_goal()->trajectory.joint_names[i] == name){
            goal_positions.push_back(trajectory_points.back().positions[i]);
            break;
          }
        }
      }

      for (int i = 0; i < int(state_interfaces_.size()); ++i) {
        auto value = state_interfaces_.at(i).get_optional();
        joint_errors.push_back(std::abs(goal_positions[i] - (value ? value.value() : 0.0)));
      }

      if (*std::max_element(std::begin(joint_errors), std::end(joint_errors)) < position_threshold_) {
        executing_ = false;
        RCLCPP_INFO(get_node()->get_logger(), "Finished execution");
        auto result = std::make_shared<FollowJointTrajectory::Result>();
        result->error_code = control_msgs::action::FollowJointTrajectory::Result::SUCCESSFUL;
        current_goal_->succeed(result);
      }
    }

    return controller_interface::return_type::OK;  
  }

  void MotomanJointTrajectoryController::run_trajectory_execution() {
    while (thread_running_) {
      if (start_execution_) {
        sending_trajectory_ = true;
        int seq = 0;

        // 1. CONSTRUCT AND SEND INITIAL POINT
        std::vector<float> current_positions;
        for (int i = 0; i < num_robot_joints_; ++i) {
            auto value = state_interfaces_.at(i).get_optional();
            current_positions.push_back(value ? value.value() : 0.0f);
        }
        
        std::vector<float> zeros(10, 0.0f);
        simple_message::JointTrajPtFull initial_point(seq, 0.0f, current_positions, zeros, zeros);
        
        write_to_socket(motion_socket_, initial_point.to_bytes());
        simple_message::MotoMotionReply init_reply;
        init_reply.init(read_from_socket(motion_socket_, get_packet_length(motion_socket_)));

        if (!init_reply.is_successful()) {
            RCLCPP_ERROR(get_node()->get_logger(), "Initial point rejected");
            start_execution_ = false;
            sending_trajectory_ = false;
            continue;
        }
        seq++;

        // 2. SEND REMAINING POINTS
        auto points = current_goal_->get_goal()->trajectory.points;
        for (size_t p_idx = 0; p_idx < points.size(); ++p_idx) {
          if (!thread_running_) break;

          std::vector<float> pos, vel, acc;
          for (auto& name : joint_names_) {
            for (size_t i = 0; i < current_goal_->get_goal()->trajectory.joint_names.size(); ++i) {
              if (current_goal_->get_goal()->trajectory.joint_names[i] == name) {
                pos.push_back(points[p_idx].positions[i]);
                vel.push_back(points[p_idx].velocities[i]);
                acc.push_back(points[p_idx].accelerations[i]);
                break;
              }
            }
          }

          float time_s = rclcpp::Duration(points[p_idx].time_from_start).seconds();
          simple_message::JointTrajPtFull pt(seq, time_s, pos, vel, acc);

          // This blocks the background thread, which is fine!
          write_to_socket(motion_socket_, pt.to_bytes());
          simple_message::MotoMotionReply reply;
          reply.init(read_from_socket(motion_socket_, get_packet_length(motion_socket_)));

          if (reply.is_busy()) {
            p_idx--; // Retry this point
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
          } else if (reply.is_successful()) {
            seq++;
          } else {
            RCLCPP_ERROR(get_node()->get_logger(), "Point %ld rejected", p_idx);
            break;
          }
        }

        sending_trajectory_ = false;
        start_execution_ = false;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }

  CallbackReturn MotomanJointTrajectoryController::on_activate(const rclcpp_lifecycle::State& /*state*/) {
    motion_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    motion_socket_address_.sin_family = AF_INET;
    motion_socket_address_.sin_port = htons(motion_port_);
    inet_pton(AF_INET, robot_ip_, &motion_socket_address_.sin_addr);

    int connection_success = connect(motion_socket_, (struct sockaddr *)&motion_socket_address_, sizeof(motion_socket_address_));

    if (connection_success < 0){
      RCLCPP_INFO(get_node()->get_logger(), "Unable to connect to socket");
      return CallbackReturn::FAILURE;
    }

    simple_message::MotoMotionReply reply;

    simple_message::MotoMotionCtrl start_traj_mode_msg("START_TRAJ_MODE");
    write_to_socket(motion_socket_, start_traj_mode_msg.to_bytes());
    int length = get_packet_length(motion_socket_);
    reply.init(read_from_socket(motion_socket_, length));

    if(!reply.is_successful()){
      RCLCPP_ERROR(get_node()->get_logger(), "\n\nUnable to start trajectory controller");
      return CallbackReturn::FAILURE;
    }

    thread_running_ = true;
    worker_thread_ = std::thread(&MotomanJointTrajectoryController::run_trajectory_execution, this);
    
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn MotomanJointTrajectoryController::on_deactivate(const rclcpp_lifecycle::State&) {
    thread_running_ = false;
    if (worker_thread_.joinable()) {
      worker_thread_.join();
    }

    simple_message::MotoMotionCtrl stop_traj_mode_msg("STOP_TRAJ_MODE");
    write_to_socket(motion_socket_, stop_traj_mode_msg.to_bytes());
    int length = get_packet_length(motion_socket_);
    read_from_socket(motion_socket_, length); 

    close(motion_socket_);

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn MotomanJointTrajectoryController::on_init() {
    try
    {
      auto_declare<std::vector<std::string>>("joints", {});
    }
    catch (const std::exception & e)
    {
      fprintf(stderr, "Exception thrown during init stage with message: %s \n", e.what());
      return CallbackReturn::ERROR;
    }

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn MotomanJointTrajectoryController::on_configure(
    const rclcpp_lifecycle::State&) 
  {
    joint_names_ = get_node()->get_parameter("joints").as_string_array();

    if (joint_names_.empty()) {
      RCLCPP_FATAL(get_node()->get_logger(), "joints parameter not set");
      return CallbackReturn::FAILURE;
    }

    // Initialize action server
    using namespace std::placeholders;
    action_server_ = rclcpp_action::create_server<control_msgs::action::FollowJointTrajectory>(
      get_node()->get_node_base_interface(), get_node()->get_node_clock_interface(),
      get_node()->get_node_logging_interface(), get_node()->get_node_waitables_interface(),
      std::string(get_node()->get_name()) + "/follow_joint_trajectory",
      std::bind(&MotomanJointTrajectoryController::handle_goal, this, _1, _2),
      std::bind(&MotomanJointTrajectoryController::handle_cancel, this, _1),
      std::bind(&MotomanJointTrajectoryController::handle_accepted, this, _1));

    return CallbackReturn::SUCCESS;
  }

  rclcpp_action::GoalResponse MotomanJointTrajectoryController::handle_goal(
    const rclcpp_action::GoalUUID & uuid, 
    std::shared_ptr<const FollowJointTrajectory::Goal> goal)
  {
    (void) uuid;
    (void) goal;
    // Check if a goal is being executed
    if (executing_) {
      RCLCPP_WARN(get_node()->get_logger(), "Goal already being executed");
      return rclcpp_action::GoalResponse::REJECT;
    }

    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  rclcpp_action::CancelResponse MotomanJointTrajectoryController::handle_cancel(
    const std::shared_ptr<GoalHandleFollowJointTrajectory> goal_handle)
  {
    (void) goal_handle;
    cancel_requsted_ = true;

    return rclcpp_action::CancelResponse::ACCEPT;
  }

  void MotomanJointTrajectoryController::handle_accepted(
    const std::shared_ptr<GoalHandleFollowJointTrajectory> goal_handle)
  {
    int num_points = goal_handle->get_goal()->trajectory.points.size();

    RCLCPP_INFO_STREAM(get_node()->get_logger(), "Executing goal with " << num_points << " points.");

    current_goal_ = goal_handle;

    received_goal_ = true;
  }

  MotomanJointTrajectoryController::~MotomanJointTrajectoryController(){
    on_deactivate(rclcpp_lifecycle::State());
  }

}