#include <aprs_pddl/pick_part_action.hpp>

PickPartAction::PickPartAction()
: plansys2::ActionExecutorClient("pick_part", std::chrono::milliseconds(250)),
  fanuc_waiting_for_response_{false},
  fanuc_service_called_{false}, 
  motoman_waiting_for_response_{false},
  motoman_service_called_{false}
{
  pick_part_client_fanuc_ = this->create_client<aprs_interfaces::srv::Pick>("/fanuc/pick_from_slot");
  pick_part_client_motoman_ = this->create_client<aprs_interfaces::srv::Pick>("/motoman/pick_from_slot");
}

void PickPartAction::do_work() {

  if(!fanuc_service_called_ && current_arguments_[2] == "fanuc") {

    auto request = std::make_shared<aprs_interfaces::srv::Pick::Request>();

    request->frame_name = current_arguments_[0];

    pick_part_client_fanuc_->async_send_request(request,
    std::bind(&PickPartAction::pick_response_cb_fanuc, this, std::placeholders::_1));

    fanuc_waiting_for_response_=true;
    fanuc_service_called_ = true;

  } else {
    if(!fanuc_waiting_for_response_) {
      fanuc_service_called_ = false;
      progress_ = 1.0;
      finish(true, progress_,"Pickd Part in Slot Successfully");
    }
  }

  if(!motoman_service_called_ && current_arguments_[2] == "motoman") {

    auto request = std::make_shared<aprs_interfaces::srv::Pick::Request>();

    request->frame_name = current_arguments_[0];

    pick_part_client_motoman_->async_send_request(request,
    std::bind(&PickPartAction::pick_response_cb_motoman, this, std::placeholders::_1));
    
    motoman_waiting_for_response_=true;
    motoman_service_called_ = true;

  } else {
    if(!motoman_waiting_for_response_) {
      motoman_service_called_ = false;
      progress_ = 1.0;
      finish(true, progress_,"Pickd Part in Slot Successfully");
    }
  }
      
  send_feedback(progress_);
}

void PickPartAction::pick_response_cb_fanuc(rclcpp::Client<aprs_interfaces::srv::Pick>::SharedFuture future){
  RCLCPP_INFO(get_logger(),"Inside fanuc pick response callback");
  auto result = future.get();
  if (!result->success) {
    finish(false, progress_, "Fanuc Unable to Pick Part");
  }
  fanuc_waiting_for_response_ = false;
}

void PickPartAction::pick_response_cb_motoman(rclcpp::Client<aprs_interfaces::srv::Pick>::SharedFuture future){
  RCLCPP_INFO(get_logger(),"Inside motoman pick response callback");
  auto result = future.get();
  if (!result->success) {
    finish(false, progress_, "Motoman Unable to Pick Part");
  }
  motoman_waiting_for_response_ = false;
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<PickPartAction>();

  node->set_parameter(rclcpp::Parameter("action_name", "pick"));
  node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);

  rclcpp::spin(node->get_node_base_interface());

  rclcpp::shutdown();

  return 0;
}