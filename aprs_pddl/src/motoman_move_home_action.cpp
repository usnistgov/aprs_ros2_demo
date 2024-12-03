#include <aprs_pddl/move_home_action.hpp>

MoveRobotAction::MoveRobotAction()
: plansys2::ActionExecutorClient("motoman_move_home", std::chrono::milliseconds(250)),
  waiting_for_response_{false},
  service_called_{false}
{
  move_home_client_ = this->create_client<aprs_interfaces::srv::MoveToNamedPose>("/motoman/move_to_named_pose");
}

void MoveRobotAction::do_work() {

  if(!service_called_) {

    auto request = std::make_shared<aprs_interfaces::srv::MoveToNamedPose::Request>();

    request->name = "home";

    move_home_client_->async_send_request(request,
    std::bind(&MoveRobotAction::move_response_cb_, this, std::placeholders::_1));

    waiting_for_response_=true;
    service_called_ = true;

  } else {
    if(!waiting_for_response_) {
      service_called_ = false;
      progress_ = 1.0;
      finish(true, progress_,"Picked Part in Slot Successfully");
    }
  }
  
  send_feedback(progress_);
}

void MoveRobotAction::move_response_cb_(rclcpp::Client<aprs_interfaces::srv::MoveToNamedPose>::SharedFuture future){
  auto result = future.get();
  if (!result->success) {
    finish(false, progress_, "Motoman Unable to Move Home");
  }
  waiting_for_response_ = false;
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<MoveRobotAction>();

  node->set_parameter(rclcpp::Parameter("action_name", "motoman_move_home"));
  node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);

  rclcpp::spin(node->get_node_base_interface());

  rclcpp::shutdown();

  return 0;
}