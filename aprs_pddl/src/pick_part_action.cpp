#include <aprs_pddl/pick_part_action.hpp>

PickPartAction::PickPartAction()
: plansys2::ActionExecutorClient("pick_part", std::chrono::milliseconds(250)),
    waiting_for_response_{false},
    service_called_{false}
{}

void PickPartAction::do_work() {

  if(!service_called_) {

    rclcpp::Client<aprs_interfaces::srv::Pick>::SharedPtr pick_part_client;

    pick_part_client = this->create_client<aprs_interfaces::srv::Pick>("/pick_from_slot");

    auto request = std::make_shared<aprs_interfaces::srv::Pick::Request>();

    request->frame_name = current_arguments_[1];

    pick_part_client->async_send_request(request,
    std::bind(&PickPartAction::pick_response_cb, this, std::placeholders::_1));

    waiting_for_response_=true;
    service_called_ = true;

  } else {
    if(!waiting_for_response_) {
      service_called_ = false;
      progress_ = 1.0;
      finish(true, progress_,"Picked Part from Slot Successfully");
    }
  }
      
  send_feedback(progress_);
}

void PickPartAction::pick_response_cb(rclcpp::Client<aprs_interfaces::srv::Pick>::SharedFuture future){
  auto result = future.get();
  if (!result->success) {
    finish(false, progress_, "Unable to Pick Part");
  }
  waiting_for_response_ = false;
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<PickPartAction>();

  node->set_parameter(rclcpp::Parameter("action_name", "pick_part"));
  node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);

  rclcpp::spin(node->get_node_base_interface());

  rclcpp::shutdown();

  return 0;
}