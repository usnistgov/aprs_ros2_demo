
#include <aprs_driver/pneumatic_gripper_controller.hpp>

namespace pneumatic_controller {

  controller_interface::InterfaceConfiguration PneumaticGripperController::command_interface_configuration() const 
  {
    controller_interface::InterfaceConfiguration config;
      
    config.type = controller_interface::interface_configuration_type::INDIVIDUAL;

    for (std::string name : joint_names_) {
      config.names.push_back(name + "/position");
    }

    return config;
  }

  controller_interface::InterfaceConfiguration PneumaticGripperController::state_interface_configuration() const
  {
    controller_interface::InterfaceConfiguration config;
      
    config.type = controller_interface::interface_configuration_type::INDIVIDUAL;

    for (std::string name : joint_names_) {
      config.names.push_back(name + "/position");
    }

    return config;
  }

  controller_interface::return_type PneumaticGripperController::update(const rclcpp::Time&, const rclcpp::Duration&) 
  {
    for (auto& cmd_interface : command_interfaces_) {
      cmd_interface.set_value(gripper_state_);
    }

    return controller_interface::return_type::OK;
  }

  CallbackReturn PneumaticGripperController::on_init() {
    try
    {
      auto_declare<std::vector<std::string>>("joints", {});
    }
    catch (const std::exception & e)
    {
      fprintf(stderr, "Exception thrown during init stage with message: %s \n", e.what());
      return CallbackReturn::ERROR;
    }

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn PneumaticGripperController::on_configure(const rclcpp_lifecycle::State&) 
  {
    joint_names_ = get_node()->get_parameter("joints").as_string_array();

    if (joint_names_.empty()) {
      RCLCPP_FATAL(get_node()->get_logger(), "joints parameter not set");
      return CallbackReturn::FAILURE;
    }

    change_gripper_state_ = get_node()->create_service<aprs_interfaces::srv::PneumaticGripperControl>(
      robot_ + "_actuate_gripper", 
      std::bind(&PneumaticGripperController::actuate_gripper_srv_cb_, this, std::placeholders::_1, std::placeholders::_2),
      rclcpp::ServicesQoS()
    );

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn PneumaticGripperController::on_activate(const rclcpp_lifecycle::State&)
  {
    if(robot_ == "motoman"){
      control_air(true);
    }
    actuate_gripper(false);

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn PneumaticGripperController::on_deactivate(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;

    RCLCPP_INFO(get_node()->get_logger(), "Inside on_deactivate");

    if(robot_ == "motoman"){
      control_air(false);
    }

    RCLCPP_INFO(get_node()->get_logger(), "Successfully deactivated!");

    return CallbackReturn::SUCCESS;
  }

  void PneumaticGripperController::actuate_gripper_srv_cb_(
    const std::shared_ptr<aprs_interfaces::srv::PneumaticGripperControl::Request> request,
    std::shared_ptr<aprs_interfaces::srv::PneumaticGripperControl::Response> response)
  {
    response->success = actuate_gripper(request->enable);
  }

  bool PneumaticGripperController::actuate_gripper(bool enable)
  {
    // Connect to gripper socket
    struct sockaddr_in gripper_socket_address;

    int gripper_socket = socket(AF_INET, SOCK_STREAM, 0);
    gripper_socket_address.sin_family = AF_INET;
    gripper_socket_address.sin_port = htons(robot_ports_[robot_]);
    inet_pton(AF_INET, robot_ips_[robot_], &gripper_socket_address.sin_addr);

    int connection_success = connect(gripper_socket, (struct sockaddr *)&gripper_socket_address, sizeof(gripper_socket_address));

    if (connection_success < 0){
      RCLCPP_INFO(get_node()->get_logger(), "Unable to connect to socket");
      return false;
    }

    sleep(1);

    std::vector<uint8_t> data;

    if(robot_ == "fanuc"){
      if (enable) {
        insert_byte(data, 1);
        gripper_state_ = 0.0;
      } else {
        insert_byte(data, 0);
        gripper_state_ = gripper_strokes_[robot_];
      }

      write_to_socket(gripper_socket, data);
    }
    else {
      if(enable){
        simple_message::WriteIORequest open_gripper_io(10010, 1);
        write_to_socket(gripper_socket, open_gripper_io.to_bytes());
        int length = get_packet_length(gripper_socket);
        read_from_socket(gripper_socket, length);

        simple_message::WriteIORequest close_gripper_io(10011, 0);
        write_to_socket(gripper_socket, close_gripper_io.to_bytes());
        length = get_packet_length(gripper_socket);
        read_from_socket(gripper_socket, length);

        gripper_state_ = gripper_strokes_[robot_];
      }
      else {
        simple_message::WriteIORequest open_gripper_io(10010, 0);
        write_to_socket(gripper_socket, open_gripper_io.to_bytes());
        int length = get_packet_length(gripper_socket);
        read_from_socket(gripper_socket, length);

        simple_message::WriteIORequest close_gripper_io(10011, 1);
        write_to_socket(gripper_socket, close_gripper_io.to_bytes());
        length = get_packet_length(gripper_socket);
        read_from_socket(gripper_socket, length);

        gripper_state_ = 0.0;
      }
    }
    
    close(gripper_socket);

    return true;
  }

  bool PneumaticGripperController::control_air(bool enable) {
    struct sockaddr_in gripper_socket_address;

    int gripper_socket = socket(AF_INET, SOCK_STREAM, 0);
    gripper_socket_address.sin_family = AF_INET;
    gripper_socket_address.sin_port = htons(robot_ports_[robot_]);
    inet_pton(AF_INET, robot_ips_[robot_], &gripper_socket_address.sin_addr);

    int connection_success = connect(gripper_socket, (struct sockaddr *)&gripper_socket_address, sizeof(gripper_socket_address));

    if (connection_success < 0){
      RCLCPP_INFO(get_node()->get_logger(), "Unable to connect to socket");
      return false;
    }

    sleep(1);

    std::vector<uint8_t> data;

    if(robot_ == "fanuc"){
      return false;
    }
    else {
      if(enable){
        simple_message::WriteIORequest air_io(10012, 1);
        write_to_socket(gripper_socket, air_io.to_bytes());
        int length = get_packet_length(gripper_socket);
        read_from_socket(gripper_socket, length);
      }
      else {
        simple_message::WriteIORequest air_io(10012, 0);
        write_to_socket(gripper_socket, air_io.to_bytes());
        int length = get_packet_length(gripper_socket);
        read_from_socket(gripper_socket, length);
      }
    }
    
    close(gripper_socket);

    return true;
  }

  PneumaticGripperController::~PneumaticGripperController(){
    on_deactivate(rclcpp_lifecycle::State());
  }

}  // namespace aprs_controllers

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(pneumatic_controller::PneumaticGripperController, controller_interface::ControllerInterface)