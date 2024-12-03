#ifndef ARIAC_PDDL_PICK_PART_HPP_
#define ARIAC_PDDL_PICK_PART_HPP_

#include "plansys2_executor/ActionExecutorClient.hpp"
#include "rclcpp/rclcpp.hpp"
#include <aprs_interfaces/srv/pick.hpp>

class PickPartAction : public plansys2::ActionExecutorClient
{
public:
  PickPartAction();

protected:
  void do_work() override;
  void reset_action();
private:

  rclcpp::Client<aprs_interfaces::srv::Pick>::SharedPtr pick_part_client_;

  // Service response callbacks
  void pick_response_cb_(rclcpp::Client<aprs_interfaces::srv::Pick>::SharedFuture future);

  // Flags
  bool waiting_for_response_;
  bool service_called_;  

  //progress
  double progress_;
};

#endif // ARIAC_PDDL_PICK_PART_HPP_