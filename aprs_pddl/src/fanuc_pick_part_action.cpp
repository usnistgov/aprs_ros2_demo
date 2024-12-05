#include <aprs_pddl/pick_part_action.hpp>

PickPartAction::PickPartAction()
: plansys2::ActionExecutorClient("fanuc_pick_part", std::chrono::milliseconds(250)),
  waiting_for_response_{false},
  service_called_{false}
{
  pick_part_client_ = this->create_client<aprs_interfaces::srv::Pick>("/fanuc/pick_from_slot");
}

void PickPartAction::do_work() {

  if(!service_called_) {

    auto request = std::make_shared<aprs_interfaces::srv::Pick::Request>();

    request->frame_name = current_arguments_[0];

    pick_part_client_->async_send_request(request,
    std::bind(&PickPartAction::pick_response_cb_, this, std::placeholders::_1));

    waiting_for_response_=true;
    service_called_ = true;

  } else {
    if(!waiting_for_response_) {
      service_called_ = false;
      finish(true, 1.0,"Picked Part in Slot Successfully");
    }
  }
  
  send_feedback(0.0);
}

void PickPartAction::pick_response_cb_(rclcpp::Client<aprs_interfaces::srv::Pick>::SharedFuture future){
  RCLCPP_INFO(get_logger(),"Inside  pick response callback");
  auto result = future.get();
  if (!result->success) {
    finish(false, 0.0, "Fanuc Unable to Pick Part");
  }
  waiting_for_response_ = false;
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<PickPartAction>();

  node->set_parameter(rclcpp::Parameter("action_name", "fanuc_pick"));
  node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);

  rclcpp::spin(node->get_node_base_interface());

  rclcpp::shutdown();

  return 0;
}