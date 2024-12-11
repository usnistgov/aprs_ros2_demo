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
    if (received_goal_) {
      RCLCPP_INFO(get_node()->get_logger(), "\n\nRECEIVED GOAL");
      current_seq_ = 0;
      received_goal_ = false;

      for (auto name : current_goal_->get_goal()->trajectory.joint_names) {
        RCLCPP_INFO_STREAM(get_node()->get_logger(), name);
      }

      // Construct initial point
      std::vector<float> current_positions;

      for (int i = 0; i < num_robot_joints_; ++i) {
        current_positions.push_back(state_interfaces_.at(i).get_value());
      }
      
      std::vector<float> velocities(10,0);
      std::vector<float> accelerations(10,0);
      float time_from_start = 0.0;
      
      simple_message::JointTrajPtFull initial_point(current_seq_, time_from_start, current_positions, velocities, accelerations);
      
      // Send intial point
      write_to_socket(motion_socket_, initial_point.to_bytes());
  
      // Get reply    
      simple_message::MotoMotionReply reply;
      reply.init(read_from_socket(motion_socket_, get_packet_length(motion_socket_)));

      RCLCPP_INFO_STREAM(get_node()->get_logger(), reply.output());

      if (!reply.is_successful()) {
        auto result = std::make_shared<FollowJointTrajectory::Result>();
        result->error_code = control_msgs::action::FollowJointTrajectory::Result::INVALID_GOAL;
        result->error_string = "Initial point rejected by controller, check that initial positions are correct";
        current_goal_->succeed(result);
      } else {
        sending_trajectory_ = true;
        current_seq_++;
      } 

    } else if (sending_trajectory_) {
      // Construct joint trajectory point
      std::vector<float> positions;
      std::vector<float> velocities;
      std::vector<float> accelerations;

      for (auto name : joint_names_){
        for (int i = 0; i < int(current_goal_->get_goal()->trajectory.points[current_seq_ - 1].positions.size()); i++){
          if (current_goal_->get_goal()->trajectory.joint_names[i] == name){
            positions.push_back(current_goal_->get_goal()->trajectory.points[current_seq_ - 1].positions[i]);
            velocities.push_back(current_goal_->get_goal()->trajectory.points[current_seq_ - 1].velocities[i]);
            accelerations.push_back(current_goal_->get_goal()->trajectory.points[current_seq_ - 1].accelerations[i]);
            break;
          }
        }
      }
      
      // std::vector<float> velocities(10,0);
      // std::vector<float> accelerations(10,0);

      rclcpp::Duration time_from_start(current_goal_->get_goal()->trajectory.points[current_seq_ - 1].time_from_start);
      
      simple_message::JointTrajPtFull traj_point(current_seq_, time_from_start.seconds(), positions, velocities, accelerations);

      // Try to send next point in sequence
      write_to_socket(motion_socket_, traj_point.to_bytes());

      // Get reply
      simple_message::MotoMotionReply reply;
      reply.init(read_from_socket(motion_socket_, get_packet_length(motion_socket_)));

      RCLCPP_INFO_STREAM(get_node()->get_logger(), reply.output());

      // Verify that point was recieved by controller properly
      if (reply.is_successful()) {

        // Check if last point to be sent
        if (current_seq_ == int(current_goal_->get_goal()->trajectory.points.size())) {
          executing_ = true;
          sending_trajectory_ = false;
        } else {
          current_seq_++;
        }

      } else if (reply.is_busy()) {
        // Send point again

      } else {
        auto result = std::make_shared<FollowJointTrajectory::Result>();
        result->error_code = control_msgs::action::FollowJointTrajectory::Result::INVALID_GOAL;
        result->error_string = "Point rejected by controller";
        current_goal_->succeed(result);
        sending_trajectory_ = false;
      }
    } else if (executing_) {
      // Check if final position has been reached
      std::vector<double> joint_errors;
      std::vector<float> goal_positions;

      for (auto name : joint_names_){
        for (int i = 0; i < int(current_goal_->get_goal()->trajectory.points[current_seq_ - 1].positions.size()); i++){
          if (current_goal_->get_goal()->trajectory.joint_names[i] == name){
            goal_positions.push_back(current_goal_->get_goal()->trajectory.points.back().positions[i]);
            break;
          }
        }
      }

      for (int i = 0; i < int(state_interfaces_.size()); ++i) {
        joint_errors.push_back(abs(goal_positions[i] - state_interfaces_.at(i).get_value()));
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

    motoman_joint_trajectory_controller_status_pub_ = get_node()->create_publisher<std_msgs::msg::Bool>(
      "joint_trajectory_controller_status", rclcpp::QoS(1)
    );

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

  CallbackReturn MotomanJointTrajectoryController::on_activate(
      const rclcpp_lifecycle::State&)
  {
     // Connect to motion socket
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

    auto status = std_msgs::msg::Bool();
    if(!reply.is_successful()){
      RCLCPP_ERROR(get_node()->get_logger(), "\n\nUnable to start trajectory controller");
      status.data = false;
      motoman_joint_trajectory_controller_status_pub_->publish(status);
      return CallbackReturn::FAILURE;
    }

    joint_states_.clear();

    for (int i = 0; i < int(state_interfaces_.size()); ++i) {
      const auto& position_interface = state_interfaces_.at(i);

      joint_states_.push_back(position_interface.get_value());
    }
    status.data = true;
    motoman_joint_trajectory_controller_status_pub_->publish(status);
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

  CallbackReturn MotomanJointTrajectoryController::on_deactivate(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;

    simple_message::MotoMotionCtrl stop_traj_mode_msg("STOP_TRAJ_MODE");
    write_to_socket(motion_socket_, stop_traj_mode_msg.to_bytes());
    int length = get_packet_length(motion_socket_);
    read_from_socket(motion_socket_, length); 

    close(motion_socket_);

    auto status = std_msgs::msg::Bool();
    status.data = false;
    motoman_joint_trajectory_controller_status_pub_->publish(status);

    return CallbackReturn::SUCCESS;
  }

  MotomanJointTrajectoryController::~MotomanJointTrajectoryController(){
    on_deactivate(rclcpp_lifecycle::State());
  }

}  // namespace motoman_controller

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(motoman_controller::MotomanJointTrajectoryController, controller_interface::ControllerInterface)