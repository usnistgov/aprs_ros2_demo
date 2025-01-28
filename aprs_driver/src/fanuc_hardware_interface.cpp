#include "aprs_driver/fanuc_hardware_interface.hpp"

namespace fanuc_hardware {
 
  hardware_interface::CallbackReturn FanucHardwareInterface::on_init(const hardware_interface::HardwareInfo& info)
  {
    // Ensure hardware info is structured correctly 
    if (hardware_interface::SystemInterface::on_init(info) != CallbackReturn::SUCCESS) {
      return hardware_interface::CallbackReturn::ERROR;
    }

    // Ensure hardware info has correct number of joints
    if (int(info_.joints.size()) != num_urdf_joints_) {
      RCLCPP_ERROR(get_logger(), "Got %d joints. Expected %d.", int(info_.joints.size()), num_urdf_joints_);
      return hardware_interface::CallbackReturn::ERROR;
    }

    hw_positions_.resize(num_urdf_joints_, std::numeric_limits<double>::quiet_NaN());
    hw_commands_.resize(num_urdf_joints_, std::numeric_limits<double>::quiet_NaN());

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::CallbackReturn FanucHardwareInterface::on_configure(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;

    // Initialize positions and commands to zero
    for (int i = 0; i < num_urdf_joints_; i++)
    {
      hw_positions_[i] = 0;
      hw_commands_[i] = 0;
    }

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::CallbackReturn FanucHardwareInterface::on_activate(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;

    // Connect to state socket
    state_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    state_socket_address_.sin_family = AF_INET;
    state_socket_address_.sin_port = htons(state_port_);
    inet_pton(AF_INET, robot_ip_, &state_socket_address_.sin_addr);

    int connection_success = connect(state_socket_, (struct sockaddr *)&state_socket_address_, sizeof(state_socket_address_));

    if (connection_success < 0){
      RCLCPP_INFO(get_logger(), "Unable to connect to socket");
      return hardware_interface::CallbackReturn::FAILURE;
    }

    update_from_robot_controller();

    RCLCPP_INFO_STREAM(get_logger(), current_status_.output());
    RCLCPP_INFO_STREAM(get_logger(), current_joint_position_.output());

    auto joint_data = current_joint_position_.get_joint_data();

    for (int i = 0; i < num_robot_joints_; i++)
    {
      hw_positions_[i] = joint_data[i];
      hw_commands_[i] = joint_data[i];
    }

    activated_ = true;

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::CallbackReturn FanucHardwareInterface::on_deactivate(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;

    close(state_socket_);

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::return_type FanucHardwareInterface::read(const rclcpp::Time& time, const rclcpp::Duration& period)
  {
    (void)time;
    (void)period;

    if (!activated_){
      return hardware_interface::return_type::OK;
    }

    update_from_robot_controller();

    if(current_status_.is_estopped()){
      return hardware_interface::return_type::DEACTIVATE;
    }

    auto joint_data = current_joint_position_.get_joint_data();

    joint_data[2] += joint_data[1];

    for (int i = 0; i < num_robot_joints_; i++)
    {
      hw_positions_[i] = joint_data[i];
    }

    return hardware_interface::return_type::OK;
  }

  hardware_interface::return_type FanucHardwareInterface::write(const rclcpp::Time& time, const rclcpp::Duration& period)
  {
    (void)time;
    (void)period;

    hw_positions_[6] = hw_commands_[6];
    hw_positions_[7] = hw_commands_[7];

    return hardware_interface::return_type::OK;
  }

  std::vector<hardware_interface::StateInterface> FanucHardwareInterface::export_state_interfaces()
  {
    std::vector<hardware_interface::StateInterface> state_interfaces;
    
    for (uint i = 0; i < info_.joints.size(); i++)
    {
      state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &hw_positions_[i]));
    }

    return state_interfaces;
  }

  std::vector<hardware_interface::CommandInterface> FanucHardwareInterface::export_command_interfaces() 
  {
    std::vector<hardware_interface::CommandInterface> command_interfaces;
    
    for (uint i = 0; i < info_.joints.size(); i++)
    {
      command_interfaces.emplace_back(hardware_interface::CommandInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &hw_commands_[i]));
    }

    return command_interfaces;
  }

  rclcpp::Logger FanucHardwareInterface::get_logger() {
    return rclcpp::get_logger("FanucHardwareInterface");
  }

  void FanucHardwareInterface::update_from_robot_controller() {
    bool recieved_status = false;
    bool recieved_joint_position = false;

    while (!recieved_status || !recieved_joint_position) {

      int length = get_packet_length(state_socket_);
      auto data = read_from_socket(state_socket_, length);

      switch (length) {
        case simple_message::Status::PACKET_LENGTH:
          if (current_status_.update(data))
            recieved_status = true;
          
          break;
        
        case simple_message::JointPosition::PACKET_LENGTH:
          if (current_joint_position_.update(data))
            recieved_joint_position = true;
          
          break;
      }
    }
  }

}

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(fanuc_hardware::FanucHardwareInterface, hardware_interface::SystemInterface)