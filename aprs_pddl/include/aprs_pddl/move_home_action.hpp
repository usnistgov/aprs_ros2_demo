#ifndef ARIAC_PDDL_MOVE_HOME_HPP_
#define ARIAC_PDDL_MOVE_HOME_HPP_

#include "plansys2_executor/ActionExecutorClient.hpp"
#include "rclcpp/rclcpp.hpp"
#include <aprs_interfaces/srv/move_to_named_pose.hpp>

class MoveRobotAction : public plansys2::ActionExecutorClient
{
public:
  MoveRobotAction();

protected:
  void do_work() override;
  void reset_action();
private:

  rclcpp::Client<aprs_interfaces::srv::MoveToNamedPose>::SharedPtr move_home_client_;

  // Service response callbacks
  void move_response_cb_(rclcpp::Client<aprs_interfaces::srv::MoveToNamedPose>::SharedFuture future);

  // Flags
  bool waiting_for_response_;
  bool service_called_;  

  //progress
  double progress_;
};

#endif // ARIAC_PDDL_MOVE_HOME_HPP_