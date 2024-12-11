#ifndef APRS_DRIVER__FANUC_JOINT_TRAJECTORY_CONTROLLER_
#define APRS_DRIVER__FANUC_JOINT_TRAJECTORY_CONTROLLER_

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <rclcpp/node.hpp>
#include <rclcpp/publisher.hpp>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <controller_interface/controller_interface.hpp>

#include <control_msgs/action/follow_joint_trajectory.hpp>
#include <std_msgs/msg/bool.hpp>

#include <aprs_driver/simple_messages.hpp>
#include <aprs_driver/network_utilities.hpp>

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

namespace fanuc_controller {

class FanucJointTrajectoryController : public controller_interface::ControllerInterface {
 public:
  using FollowJointTrajectory = control_msgs::action::FollowJointTrajectory;
  using GoalHandleFollowJointTrajectory = rclcpp_action::ServerGoalHandle<FollowJointTrajectory>;

  controller_interface::InterfaceConfiguration command_interface_configuration() const override;
  controller_interface::InterfaceConfiguration state_interface_configuration() const override;
  controller_interface::return_type update(const rclcpp::Time& time,
                                           const rclcpp::Duration& period) override;
  CallbackReturn on_init() override;
  CallbackReturn on_configure(const rclcpp_lifecycle::State& previous_state) override;
  CallbackReturn on_activate(const rclcpp_lifecycle::State& previous_state) override;
  CallbackReturn on_deactivate(const rclcpp_lifecycle::State& previous_state) override;

  ~FanucJointTrajectoryController();

 private:
  rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr fanuc_joint_trajectory_controller_status_pub_;
  rclcpp_action::GoalResponse handle_goal(
    const rclcpp_action::GoalUUID & uuid,
    std::shared_ptr<const FollowJointTrajectory::Goal> goal);

  rclcpp_action::CancelResponse handle_cancel(
    const std::shared_ptr<GoalHandleFollowJointTrajectory> goal_handle);

  void handle_accepted(const std::shared_ptr<GoalHandleFollowJointTrajectory> goal_handle);

  std::vector<std::string> joint_names_;
  std::vector<double> joint_states_;

  bool executing_ = false;
  bool received_goal_ = false;
  bool cancel_requsted_ = false;
  rclcpp::Time trajectory_start_time_;
  trajectory_msgs::msg::JointTrajectoryPoint goal_point_;
  double position_threshold_ = 0.01;
  // control_msgs::action::FollowJointTrajectory::Result::SharedPtr result_;

  std::shared_ptr<GoalHandleFollowJointTrajectory> current_goal_;
  // trajectory_msgs::msg::JointTrajectory current_trajectory_;

  rclcpp_action::Server<FollowJointTrajectory>::SharedPtr action_server_;

  // Socket communication
  const char *robot_ip_ = "192.168.1.34";
  const int motion_port_ = 11000;

  struct sockaddr_in motion_socket_address_;
  int motion_socket_ = 0;

  int current_seq_ = 0;
  bool ready_for_next_point_ = false;
};
}  // namespace fanuc_controller

#endif  // APRS_DRIVER__FANUC_JOINT_TRAJECTORY_CONTROLLER_