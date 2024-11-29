#ifndef ARIAC_PDDL_MOVE_CONVEYOR_HPP_
#define ARIAC_PDDL_MOVE_CONVEYOR_HPP_

#include "plansys2_executor/ActionExecutorClient.hpp"
#include "rclcpp/rclcpp.hpp"
#include <conveyor_interfaces/srv/enable_conveyor.hpp>
#include <conveyor_interfaces/srv/set_conveyor_state.hpp>
#include <aprs_interfaces/srv/locate_trays.hpp>
#include <aprs_interfaces/srv/move_to_named_pose.hpp>


class MoveConveyorAction : public plansys2::ActionExecutorClient
{
public:
  MoveConveyorAction();

protected:
  void do_work() override;
  void reset_action();

private:

  rclcpp::Client<conveyor_interfaces::srv::EnableConveyor>::SharedPtr client_enable_conveyor_;
  rclcpp::Client<conveyor_interfaces::srv::SetConveyorState>::SharedPtr client_set_conveyor_state_;
  rclcpp::Client<aprs_interfaces::srv::LocateTrays>::SharedPtr client_locate_trays_;
  rclcpp::Client<aprs_interfaces::srv::MoveToNamedPose>::SharedPtr client_move_to_named_pose_;

  // Service response callbacks
  void enable_conveyor_response_callback(const rclcpp::Client<conveyor_interfaces::srv::EnableConveyor>::SharedFuture result);
  void set_conveyor_state_response_callback(const rclcpp::Client<conveyor_interfaces::srv::SetConveyorState>::SharedFuture result);
  void locate_trays_response_callback(const rclcpp::Client<aprs_interfaces::srv::LocateTrays>::SharedFuture result);
  void move_to_named_pose_response_callback(const rclcpp::Client<aprs_interfaces::srv::MoveToNamedPose>::SharedFuture result);

  // Flags
  bool waiting_for_enable_conveyor_response_;
  bool waiting_for_set_conveyor_state_response_;
  bool waiting_for_locate_trays_response_;
  bool waiting_for_move_to_named_pose_response_;
  bool service_called_enable_conveyor_;
  bool service_called_set_conveyor_state_;
  bool service_called_locate_trays_;
  bool service_called_move_to_named_pose_;

  // Conveyor Start Time
  rclcpp::Time conveyor_start_time_;

  //progress
  double progress_;
};

#endif // ARIAC_PDDL_MOVE_CONVEYOR_HPP_