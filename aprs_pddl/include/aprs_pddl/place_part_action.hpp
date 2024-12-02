#ifndef ARIAC_PDDL_PLACE_PART_HPP_
#define ARIAC_PDDL_PLACE_PART_HPP_

#include "plansys2_executor/ActionExecutorClient.hpp"
#include "rclcpp/rclcpp.hpp"
#include <aprs_interfaces/srv/place.hpp>

class PlacePartAction : public plansys2::ActionExecutorClient
{
public:
  PlacePartAction();

protected:
  void do_work() override;
  void reset_action(std::string robot);

private:

  rclcpp::Client<aprs_interfaces::srv::Place>::SharedPtr place_part_client_;
  
  // Service response callbacks
  void place_response_cb_(rclcpp::Client<aprs_interfaces::srv::Place>::SharedFuture future);

  // Flags
  bool waiting_for_response_;
  bool service_called_;  


  //progress
  double progress_;
};

#endif // ARIAC_PDDL_PLACE_PART_HPP_