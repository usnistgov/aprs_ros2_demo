#include <aprs_pddl/move_conveyor_action.hpp>

MoveConveyorAction::MoveConveyorAction()
: plansys2::ActionExecutorClient("pick_part", std::chrono::milliseconds(250)),
    waiting_for_enable_conveyor_response_(false),
    waiting_for_set_conveyor_state_response_(false),
    waiting_for_locate_trays_response_(false),
    waiting_for_move_to_named_pose_response_(false),
    service_called_enable_conveyor_(false),
    service_called_set_conveyor_state_(false),
    service_called_locate_trays_(false),
    service_called_move_to_named_pose_(false),
    progress_(0.0)
{
    client_enable_conveyor_ = create_client<conveyor_interfaces::srv::EnableConveyor>("/conveyor/enable");
    client_set_conveyor_state_ = create_client<conveyor_interfaces::srv::SetConveyorState>("/conveyor/set_state");
    client_locate_trays_ = create_client<aprs_interfaces::srv::LocateTrays>("/motoman/conveyor_vision/locate_trays");
    client_move_to_named_pose_ = create_client<aprs_interfaces::srv::MoveToNamedPose>("/motoman/move_to_named_pose");
}

void MoveConveyorAction::do_work() {
    RCLCPP_INFO(get_logger(),"Inside Do work");

    if(!service_called_enable_conveyor_){
        // Enable Conveyor
        auto request_enable = std::make_shared<conveyor_interfaces::srv::EnableConveyor::Request>();
        request_enable->enable = true;
        client_enable_conveyor_->async_send_request(request_enable, std::bind(&MoveConveyorAction::enable_conveyor_response_callback, this, std::placeholders::_1));
        waiting_for_enable_conveyor_response_ = true;
        service_called_enable_conveyor_ = true;
    }
    if(!service_called_move_to_named_pose_){
        // Move to Motoman Home
        auto request = std::make_shared<aprs_interfaces::srv::MoveToNamedPose::Request>();
        request->name = "home";
        client_move_to_named_pose_->async_send_request(request, std::bind(&MoveConveyorAction::move_to_named_pose_response_callback, this, std::placeholders::_1));
        waiting_for_move_to_named_pose_response_ = true;
        service_called_move_to_named_pose_ = true;
    }
    else if(!waiting_for_enable_conveyor_response_ && !service_called_set_conveyor_state_){
        // Set Conveyor State
        auto request_set_state = std::make_shared<conveyor_interfaces::srv::SetConveyorState::Request>();
        request_set_state->speed = 50;
        request_set_state->direction = 1;
        client_set_conveyor_state_->async_send_request(request_set_state, std::bind(&MoveConveyorAction::set_conveyor_state_response_callback, this, std::placeholders::_1));
        waiting_for_set_conveyor_state_response_ = true;
        service_called_set_conveyor_state_ = true;
        conveyor_start_time_ = now();
    }
    else if(!waiting_for_enable_conveyor_response_ && service_called_set_conveyor_state_ && (now() - conveyor_start_time_).seconds() > 5){
        // Disable Conveyor
        auto request_enable = std::make_shared<conveyor_interfaces::srv::EnableConveyor::Request>();
        request_enable->enable = false;
        client_enable_conveyor_->async_send_request(request_enable, std::bind(&MoveConveyorAction::enable_conveyor_response_callback, this, std::placeholders::_1));
        waiting_for_enable_conveyor_response_ = true;
    }
    else if(!waiting_for_enable_conveyor_response_ && service_called_set_conveyor_state_ && !service_called_locate_trays_){
        // Locate Trays
        auto request_locate_trays = std::make_shared<aprs_interfaces::srv::LocateTrays::Request>();
        client_locate_trays_->async_send_request(request_locate_trays, std::bind(&MoveConveyorAction::locate_trays_response_callback, this, std::placeholders::_1));
        waiting_for_locate_trays_response_ = true;
        service_called_locate_trays_ = true;
    }
    else if(!waiting_for_locate_trays_response_){
        progress_ = 1.0;
        reset_action();
        finish(true, progress_, "Conveyor Moved Successfully");
    }
        
    send_feedback(progress_);
}

void MoveConveyorAction::enable_conveyor_response_callback(rclcpp::Client<conveyor_interfaces::srv::EnableConveyor>::SharedFuture future){
    RCLCPP_INFO(get_logger(),"Inside enable conveyor response callback");
    auto result = future.get();
    if (!result->success) {
        finish(false, progress_, "Unable to Enable Conveyor");
    }
    waiting_for_enable_conveyor_response_ = false;
    progress_ += 0.125;
}

void MoveConveyorAction::set_conveyor_state_response_callback(rclcpp::Client<conveyor_interfaces::srv::SetConveyorState>::SharedFuture future){
    RCLCPP_INFO(get_logger(),"Inside set conveyor state response callback");
    auto result = future.get();
    if (!result->success) {
        finish(false, progress_, "Unable to Set Conveyor State");
    }
    waiting_for_set_conveyor_state_response_ = false;
    progress_ += 0.25;
}

void MoveConveyorAction::locate_trays_response_callback(rclcpp::Client<aprs_interfaces::srv::LocateTrays>::SharedFuture future){
    RCLCPP_INFO(get_logger(),"Inside locate trays response callback");
    auto result = future.get();
    if (!result->success) {
        finish(false, progress_, "Unable to Locate Trays");
    }
    waiting_for_locate_trays_response_ = false;
    progress_ += 0.25;
}

void MoveConveyorAction::move_to_named_pose_response_callback(rclcpp::Client<aprs_interfaces::srv::MoveToNamedPose>::SharedFuture future){
    RCLCPP_INFO(get_logger(),"Inside move to named pose response callback");
    auto result = future.get();
    if (!result->success) {
        finish(false, progress_, "Unable to Move to Named Pose");
    }
    waiting_for_move_to_named_pose_response_ = false;
    progress_ += 0.25;
}

void MoveConveyorAction::reset_action() {
    waiting_for_enable_conveyor_response_ = false;
    waiting_for_set_conveyor_state_response_ = false;
    waiting_for_locate_trays_response_ = false;
    waiting_for_move_to_named_pose_response_ = false;
    service_called_enable_conveyor_ = false;
    service_called_set_conveyor_state_ = false;
    service_called_locate_trays_ = false;
    service_called_move_to_named_pose_ = false;
    progress_ = 0.0;
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<MoveConveyorAction>();

  node->set_parameter(rclcpp::Parameter("action_name", "move_and_transfer_conveyor"));
  node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);

  rclcpp::spin(node->get_node_base_interface());

  rclcpp::shutdown();

  return 0;
}