#include <aprs_driver/fanuc_joint_trajectory_controller.hpp>
#include <controller_interface/controller_interface.hpp>

namespace fanuc_controller {

  controller_interface::InterfaceConfiguration FanucJointTrajectoryController::command_interface_configuration() const
  {
    controller_interface::InterfaceConfiguration config;
    config.type = controller_interface::interface_configuration_type::INDIVIDUAL;

    for (std::string name : joint_names_) {
      config.names.push_back(name + "/position");
    }

    return config;
  }

  controller_interface::InterfaceConfiguration FanucJointTrajectoryController::state_interface_configuration() const 
  {
    controller_interface::InterfaceConfiguration config;
    
    config.type = controller_interface::interface_configuration_type::INDIVIDUAL;

    for (std::string name : joint_names_) {
      config.names.push_back(name + "/position");
    }

    return config;
  }

  controller_interface::return_type FanucJointTrajectoryController::update(
    const rclcpp::Time& time,
    const rclcpp::Duration& period) 
  {
    // RCLCPP_INFO(get_node()->get_logger(), "In update");

    // for (int i = 0; i < int(command_interfaces_.size()); ++i) {
    //   command_interfaces_[i].set_value(joint_states_[i]);
    // }

    if (received_goal_) {
      // Send trajectory point to motion port
      RCLCPP_INFO(get_node()->get_logger(), "Starting execution");
      trajectory_start_time_ = time;
      received_goal_ = false;
      executing_ = true;
    }

    if (executing_) {
      if ((time - trajectory_start_time_).seconds() > 10) {
        executing_ = false;
        RCLCPP_INFO(get_node()->get_logger(), "Finished execution");
        auto result = std::make_shared<FollowJointTrajectory::Result>();
        result->error_code = control_msgs::action::FollowJointTrajectory::Result::SUCCESSFUL;
        current_goal_->succeed(result);
        // (void) current_goal_;
      }
    }

    return controller_interface::return_type::OK;
  }

  CallbackReturn FanucJointTrajectoryController::on_init() {
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

  CallbackReturn FanucJointTrajectoryController::on_configure(
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
      "/follow_joint_trajectory",
      std::bind(&FanucJointTrajectoryController::handle_goal, this, _1, _2),
      std::bind(&FanucJointTrajectoryController::handle_cancel, this, _1),
      std::bind(&FanucJointTrajectoryController::handle_accepted, this, _1));

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn FanucJointTrajectoryController::on_activate(
      const rclcpp_lifecycle::State&)
  {
    joint_states_.clear();

    for (int i = 0; i < int(state_interfaces_.size()); ++i) {
      const auto& position_interface = state_interfaces_.at(i);

      joint_states_.push_back(position_interface.get_value());
    }
    
    return CallbackReturn::SUCCESS;
  }

  rclcpp_action::GoalResponse FanucJointTrajectoryController::handle_goal(
    const rclcpp_action::GoalUUID & uuid, 
    std::shared_ptr<const FollowJointTrajectory::Goal> goal)
  {
    // Check if a goal is being executed
    if (executing_) {
      RCLCPP_WARN(get_node()->get_logger(), "Goal already being executed");
      return rclcpp_action::GoalResponse::REJECT;
    }

    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  rclcpp_action::CancelResponse FanucJointTrajectoryController::handle_cancel(
    const std::shared_ptr<GoalHandleFollowJointTrajectory> goal_handle)
  {

  }

  void FanucJointTrajectoryController::handle_accepted(
    const std::shared_ptr<GoalHandleFollowJointTrajectory> goal_handle)
  {
    int num_points = goal_handle->get_goal()->trajectory.points.size();

    RCLCPP_INFO_STREAM(get_node()->get_logger(), "Executing goal with " << num_points << " points.");

    current_goal_ = goal_handle;

    received_goal_ = true;
  }

}  // namespace fanuc_controller

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(fanuc_controller::FanucJointTrajectoryController, controller_interface::ControllerInterface)