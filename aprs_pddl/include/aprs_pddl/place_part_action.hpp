#ifndef ARIAC_PDDL_PLACE_PART_HPP_
#define ARIAC_PDDL_PLACE_PART_HPP_

#include "plansys2_executor/ActionExecutorClient.hpp"
#include "rclcpp/rclcpp.hpp"
#include <aprs_interfaces/srv/place.hpp>
#include <aprs_pddl/place_part_action.hpp>

class PlacePartAction : public plansys2::ActionExecutorClient
{
public:
  PlacePartAction();

protected:
  void do_work() override;
  void reset_action();

private:

  // Service response callbacks
  void place_response_cb(rclcpp::Client<aprs_interfaces::srv::Place>::SharedFuture future);

  // Flags
  bool waiting_for_response_;
  bool service_called_;

  //progress
  double progress_;
};

#endif // ARIAC_PDDL_PLACE_PART_HPP_