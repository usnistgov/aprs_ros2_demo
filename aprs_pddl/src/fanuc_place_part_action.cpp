#include <aprs_pddl/place_part_action.hpp>

PlacePartAction::PlacePartAction()
: plansys2::ActionExecutorClient("fanuc_place_part", std::chrono::milliseconds(250)),
  waiting_for_response_{false},
  service_called_{false}
{
  place_part_client_ = this->create_client<aprs_interfaces::srv::Place>("/fanuc/place_in_slot");
}

void PlacePartAction::do_work() {

  if(!service_called_) {

    auto request = std::make_shared<aprs_interfaces::srv::Place::Request>();

    request->frame_name = current_arguments_[0];

    place_part_client_->async_send_request(request,
    std::bind(&PlacePartAction::place_response_cb_, this, std::placeholders::_1));

    waiting_for_response_=true;
    service_called_ = true;

  } else {
    if(!waiting_for_response_) {
      service_called_ = false;
      progress_ = 1.0;
      finish(true, progress_,"Placed Part in Slot Successfully");
    }
  }
      
  send_feedback(progress_);
}

void PlacePartAction::place_response_cb_(rclcpp::Client<aprs_interfaces::srv::Place>::SharedFuture future){
  auto result = future.get();
  if (!result->success) {
    finish(false, progress_, "Fanuc Unable to Place Part");
  }
  waiting_for_response_ = false;
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<PlacePartAction>();

  node->set_parameter(rclcpp::Parameter("action_name", "fanuc_place"));
  node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);

  rclcpp::spin(node->get_node_base_interface());

  rclcpp::shutdown();

  return 0;
}