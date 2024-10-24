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
    const rclcpp::Time& time,
    const rclcpp::Duration& period) 
  {
    (void) period;
    if (received_goal_) {
      
      RCLCPP_INFO(get_node()->get_logger(), "Starting execution");
      trajectory_start_time_ = time;
      received_goal_ = false;
      executing_ = true;
      current_seq_ = 0;
      ready_for_next_point_ = true;
    }

    if (executing_) {
      if (cancel_requsted_) {
        simple_message::MotoMotionCtrl stop_motion_msg("STOP_MOTION");
        write_to_socket(motion_socket_, stop_motion_msg.to_bytes());
        int length = get_packet_length(motion_socket_);
        read_from_socket(motion_socket_, length);
        
        auto result = std::make_shared<FollowJointTrajectory::Result>();
        result->error_code = control_msgs::action::FollowJointTrajectory::Result::SUCCESSFUL;
        result->error_string = "Goal canceled";
        current_goal_->abort(result);
        executing_ = false;
      }

      if (ready_for_next_point_){
        std::vector<float> current_positions;
        for (int i = 0; i < int(state_interfaces_.size()); ++i) {
          const auto& position_interface = state_interfaces_.at(i);
          current_positions.push_back(position_interface.get_value());
        }
        
        std::vector<float> velocities(10,0);
        std::vector<float> accelerations(10,0);
        int length;
        simple_message::MotoMotionReply reply;

        if(current_seq_ == 0){
          simple_message::JointTrajPtFull initial_point(0, 0.0, current_positions, velocities, accelerations);
          write_to_socket(motion_socket_, initial_point.to_bytes());
          length = get_packet_length(motion_socket_);
          reply.init(read_from_socket(motion_socket_, length));

          RCLCPP_INFO_STREAM(get_node()->get_logger(), "Result: " << reply.get_result() << "\nSubcode: " << reply.get_subcode());
          current_seq_++;
        }

        
        goal_point_ = current_goal_->get_goal()->trajectory.points[current_seq_ - 1];
        
        float time_from_start = 0;
        rclcpp::Time current_point_time(goal_point_.time_from_start.sec, goal_point_.time_from_start.nanosec);

        time_from_start = current_point_time.seconds();

        std::vector<float> positions;

        for (auto p: goal_point_.positions) {
          positions.push_back(float(p));
        }
        
        simple_message::JointTrajPtFull goal_point(current_seq_, time_from_start, positions, velocities, accelerations);
        write_to_socket(motion_socket_, goal_point.to_bytes());
        length = get_packet_length(motion_socket_);
        reply.init(read_from_socket(motion_socket_, length));

        RCLCPP_INFO_STREAM(get_node()->get_logger(), "Result: " << reply.get_result() << "\nSubcode: " << reply.get_subcode());

        ready_for_next_point_ = false;
      }

      // Check if goal is reached
      std::vector<double> joint_errors;
      for (int i = 0; i < int(state_interfaces_.size()); ++i) {
        const auto& position_interface = state_interfaces_.at(i);

        joint_errors.push_back(abs(goal_point_.positions[i] - position_interface.get_value()));
      }

      if (*std::max_element(std::begin(joint_errors), std::end(joint_errors)) < position_threshold_) {
        ready_for_next_point_ = true;
        RCLCPP_INFO_STREAM(get_node()->get_logger(), "Finished execution for point " << current_seq_);
        
        // Check if all points reached
        if (current_goal_->get_goal()->trajectory.points.size() == current_seq_ ) {
          executing_ = false;
          RCLCPP_INFO(get_node()->get_logger(), "Finished execution");
          auto result = std::make_shared<FollowJointTrajectory::Result>();
          result->error_code = control_msgs::action::FollowJointTrajectory::Result::SUCCESSFUL;
          current_goal_->succeed(result);
        } else {
          current_seq_++;
        }
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
      "follow_joint_trajectory",
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

    if(!reply.is_successful()){
      RCLCPP_ERROR(get_node()->get_logger(), "\n\nUnable to start trajectory controller");
      return CallbackReturn::FAILURE;
    }

    joint_states_.clear();

    for (int i = 0; i < int(state_interfaces_.size()); ++i) {
      const auto& position_interface = state_interfaces_.at(i);

      joint_states_.push_back(position_interface.get_value());
    }
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

    return CallbackReturn::SUCCESS;
  }

  MotomanJointTrajectoryController::~MotomanJointTrajectoryController(){
    on_deactivate(rclcpp_lifecycle::State());
  }

}  // namespace motoman_controller

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(motoman_controller::MotomanJointTrajectoryController, controller_interface::ControllerInterface)