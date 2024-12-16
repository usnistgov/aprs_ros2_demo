
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

    auto controller_status = std_msgs::msg::Bool();
    controller_status.data = true;
    pneumatic_gripper_controller_status_pub_->publish(controller_status);

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

    pneumatic_gripper_controller_status_pub_ = get_node()->create_publisher<std_msgs::msg::Bool>(
      "pneumatic_gripper_status", rclcpp::QoS(1)
    );

    get_node()->declare_parameter("robot_name", "");

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
      "actuate_gripper", 
      std::bind(&PneumaticGripperController::actuate_gripper_srv_cb_, this, std::placeholders::_1, std::placeholders::_2),
      rclcpp::ServicesQoS()
    );

    robot_name_ = get_node()->get_parameter("robot_name").as_string();

    if(robot_name_ == ""){
      RCLCPP_ERROR(get_node()->get_logger(), "Robot name not specified");
      return CallbackReturn::FAILURE;
    }
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn PneumaticGripperController::on_activate(const rclcpp_lifecycle::State&)
  {
    // Connect to gripper socket
    struct sockaddr_in gripper_socket_address;

    gripper_socket = socket(AF_INET, SOCK_STREAM, 0);
    gripper_socket_address.sin_family = AF_INET;
    gripper_socket_address.sin_port = htons(robot_ports_[robot_name_]);
    inet_pton(AF_INET, robot_ips_[robot_name_], &gripper_socket_address.sin_addr);

    int connection_success = connect(gripper_socket, (struct sockaddr *)&gripper_socket_address, sizeof(gripper_socket_address));

    auto controller_status = std_msgs::msg::Bool();
    if (connection_success < 0){
      RCLCPP_INFO(get_node()->get_logger(), "Unable to connect to socket");
      controller_status.data = false;
      pneumatic_gripper_controller_status_pub_->publish(controller_status);
      return CallbackReturn::FAILURE;
    }

    if(robot_name_ == "fanuc"){
      std::vector<std::uint8_t> byte_array;

      insert_byte(byte_array, 3);
      write_to_socket(gripper_socket, byte_array);

      char* response = read_from_socket(gripper_socket, 4);

      int status = byte_to_int(response);

      if(status == 0){
        gripper_state_ = gripper_strokes_[robot_name_];
      } else if(status ==1) {
        gripper_state_ = 0.0;
      } else {
        RCLCPP_ERROR_STREAM(get_node()->get_logger(), "Unknown value recieved for fanuc gripper get state. Recieved: " << status);
        controller_status.data = false;
        pneumatic_gripper_controller_status_pub_->publish(controller_status);
        return CallbackReturn::FAILURE;
      }
    
    } else if (robot_name_ == "motoman"){
      control_air(true);
      
      simple_message::ReadIOReply reply;

      simple_message::ReadIORequest check_gripper_open(10010);
      write_to_socket(gripper_socket, check_gripper_open.to_bytes());
      int length = get_packet_length(gripper_socket);
      reply.init(read_from_socket(gripper_socket, length));

      if(reply.get_value() == 1){
        RCLCPP_INFO(get_node()->get_logger(), "Gripper starting open");
        gripper_state_ = gripper_strokes_[robot_name_];
      } else {
        RCLCPP_INFO(get_node()->get_logger(), "Gripper starting closed");
        gripper_state_ = 0.0;
      }
    } else {
      RCLCPP_ERROR(get_node()->get_logger(), "Robot name invalid");
      controller_status.data = false;
      pneumatic_gripper_controller_status_pub_->publish(controller_status);
      return CallbackReturn::FAILURE;
    }

    controller_status.data = true;
    pneumatic_gripper_controller_status_pub_->publish(controller_status);

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn PneumaticGripperController::on_deactivate(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;

    RCLCPP_INFO(get_node()->get_logger(), "Inside on_deactivate");

    if(robot_name_ == "motoman"){
      control_air(false);
    }
    if(robot_name_== "fanuc"){
      std::vector<uint8_t> data;
      insert_byte(data, 99);
      write_to_socket(gripper_socket, data);
    }

    close(gripper_socket);

    RCLCPP_INFO(get_node()->get_logger(), "Successfully deactivated!");

    auto controller_status = std_msgs::msg::Bool();
    controller_status.data = false;
    pneumatic_gripper_controller_status_pub_->publish(controller_status);

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

    std::vector<uint8_t> data;

    if(robot_name_ == "fanuc"){
      if (enable) {
        insert_byte(data, 1);
        gripper_state_ = 0.0;
      } else {
        insert_byte(data, 0);
        gripper_state_ = gripper_strokes_[robot_name_];
      }

      write_to_socket(gripper_socket, data);
      read_from_socket(gripper_socket, 4);
    } else {
      if(enable){
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
      else {
        simple_message::WriteIORequest open_gripper_io(10010, 1);
        write_to_socket(gripper_socket, open_gripper_io.to_bytes());
        int length = get_packet_length(gripper_socket);
        read_from_socket(gripper_socket, length);

        simple_message::WriteIORequest close_gripper_io(10011, 0);
        write_to_socket(gripper_socket, close_gripper_io.to_bytes());
        length = get_packet_length(gripper_socket);
        read_from_socket(gripper_socket, length);

        gripper_state_ = gripper_strokes_[robot_name_];
      }
    }
    
    return true;
  }

  bool PneumaticGripperController::control_air(bool enable) {
    std::vector<uint8_t> data;

    if(robot_name_ == "fanuc"){
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

    return true;
  }

  PneumaticGripperController::~PneumaticGripperController(){
    auto status = std_msgs::msg::Bool();
    status.data = false;
    pneumatic_gripper_controller_status_pub_->publish(status);
    on_deactivate(rclcpp_lifecycle::State());
  }

}  // namespace aprs_controllers

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(pneumatic_controller::PneumaticGripperController, controller_interface::ControllerInterface)