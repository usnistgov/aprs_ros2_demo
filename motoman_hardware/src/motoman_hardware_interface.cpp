#include "motoman_hardware/motoman_hardware_interface.hpp"

#include "hardware_interface/types/hardware_interface_type_values.hpp"



namespace motoman_hardware {
  hardware_interface::CallbackReturn MotomanHardwareInterface::on_init(const hardware_interface::HardwareInfo& info)
  {
  
    if (hardware_interface::SystemInterface::on_init(info) != CallbackReturn::SUCCESS) {
      return hardware_interface::CallbackReturn::ERROR;
    }

    if (int(info_.joints.size()) != number_of_joints_) {
      RCLCPP_FATAL(get_logger(), "Got %d joints. Expected %d.", int(info_.joints.size()), number_of_joints_);
      return hardware_interface::CallbackReturn::ERROR;
    }

    if(!setup_sockets()){
      RCLCPP_FATAL(get_logger(), "Unable to setup sockets");
      return hardware_interface::CallbackReturn::ERROR;
    }

    hw_states_.resize(info_.joints.size(), std::numeric_limits<double>::quiet_NaN());
    hw_commands_.resize(info_.joints.size(), std::numeric_limits<double>::quiet_NaN());
    joint_positions_.resize(info_.joints.size(), std::numeric_limits<double>::quiet_NaN());

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::CallbackReturn MotomanHardwareInterface::on_configure(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;

    for (uint i = 0; i < hw_states_.size(); i++)
    {
      hw_states_[i] = 0;
      hw_commands_[i] = 0;
    }

    RCLCPP_INFO(get_logger(), "Successfully configured!");

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::CallbackReturn MotomanHardwareInterface::on_activate(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;

    // Connect to sockets
    if(!connect_sockets()){
      RCLCPP_FATAL(get_logger(), "Unable to connect to sockets");
      return hardware_interface::CallbackReturn::ERROR;
    }

    read_joints();

    // Set commands equal to current state
    for (uint i = 0; i < hw_states_.size(); i++)
    {
      hw_states_[i] = joint_positions_[i];
      hw_commands_[i] = joint_positions_[i];
    }

    RCLCPP_INFO(get_logger(), "Successfully activated!");

    activated = true;

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::CallbackReturn MotomanHardwareInterface::on_deactivate(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;
      
    close_sockets();

    RCLCPP_INFO(get_logger(), "Successfully deactivated!");

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::return_type MotomanHardwareInterface::read(const rclcpp::Time& time, const rclcpp::Duration& period)
  {
    (void)time;
    (void)period;
    
    if(!activated){
      return hardware_interface::return_type::OK;
    }
    
    read_joints();


    for (uint i = 0; i < hw_states_.size(); i++)
    {
      hw_states_[i] = joint_positions_[i];
    }

    return hardware_interface::return_type::OK;
  }

  hardware_interface::return_type MotomanHardwareInterface::write(const rclcpp::Time& time, const rclcpp::Duration& period)
  {
    (void)time;
    (void)period;

    if(!activated){
      return hardware_interface::return_type::OK;
    }

    if(motion_requested){
      write_joints();
    }

    return hardware_interface::return_type::OK;
  }

  std::vector<hardware_interface::StateInterface> MotomanHardwareInterface::export_state_interfaces()
  {
    std::vector<hardware_interface::StateInterface> state_interfaces;
    
    for (uint i = 0; i < info_.joints.size(); i++)
    {
      state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &hw_states_[i]));
    }

    return state_interfaces;
  }

  std::vector<hardware_interface::CommandInterface> MotomanHardwareInterface::export_command_interfaces() 
  {
    std::vector<hardware_interface::CommandInterface> command_interfaces;
    
    for (uint i = 0; i < info_.joints.size(); i++)
    {
      command_interfaces.emplace_back(hardware_interface::CommandInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &hw_commands_[i]));
    }

    return command_interfaces;
  }

  rclcpp::Logger MotomanHardwareInterface::get_logger() {
    return rclcpp::get_logger("MotomanHardwareInterface");
  }

  bool MotomanHardwareInterface::setup_sockets(){
    // Create state socket
    if((state_socket_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
      return false;
    }

    state_socket_address_.sin_family = AF_INET;
    state_socket_address_.sin_port = htons(state_port_);

    // Convert IP addresses from text to binary form
    if(inet_pton(AF_INET, robot_ip_, &state_socket_address_.sin_addr) <= 0) 
    {
      return false;
    }

    // Create motion socket
    if((motion_socket_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
      return false;
    }

    motion_socket_address_.sin_family = AF_INET;
    motion_socket_address_.sin_port = htons(motion_port_);

    // Convert IP addresses from text to binary form
    if(inet_pton(AF_INET, robot_ip_, &motion_socket_address_.sin_addr) <= 0) 
    {
      return false;
    }

    // Create io socket
    if((io_socket_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
      return false;
    }

    io_socket_address_.sin_family = AF_INET;
    io_socket_address_.sin_port = htons(io_port_);

    // Convert IP addresses from text to binary form
    if(inet_pton(AF_INET, robot_ip_, &io_socket_address_.sin_addr) <= 0) 
    {
      return false;
    }
    return true;
  }

  bool MotomanHardwareInterface::connect_sockets(){
    // Connect to the state server
    if(connect(state_socket_, (struct sockaddr *)&state_socket_address_, sizeof(state_socket_address_)) < 0) 
    {
      return false;
    }

    // Connect to the motion server
    if(connect(motion_socket_, (struct sockaddr *)&motion_socket_address_, sizeof(motion_socket_address_)) < 0) 
    {
      return false;
    }

    // Connect to the io server
    if(connect(io_socket_, (struct sockaddr *)&io_socket_address_, sizeof(io_socket_address_)) < 0) 
    {
      return false;
    }
    return true;
  }

  void MotomanHardwareInterface::close_sockets(){
    close(state_socket_);
    close(motion_socket_);
    close(io_socket_);
  }

  int MotomanHardwareInterface::bin_to_int(char* data)
  {
    int result;

    char reversed[4] = {data[3], data[2], data[1], data[0]};

    std::memcpy(&result, reversed, sizeof(int));

    return result;
  }

  void MotomanHardwareInterface::read_joints(){

    // int length = get_packet_length();

    // if (length < 0){
    //   RCLCPP_ERROR(get_logger(), "Issue with socket...reconnecting");

    //   close(state_sock_);
    //   state_sock_ = socket(AF_INET, SOCK_STREAM, 0);
    //   connect(state_sock_, (struct sockaddr *)&state_socket_, sizeof(state_socket_));

    //   return std::make_pair(false, joint_positions);
    // }

    // if (length != state_buffer_length_) {
    //   if(length == 40){
    //     char *status_packet = new char[40];

    //     socket_read::read_socket(state_sock_, status_packet, 40);

    //     std::vector<std::string> status_labels = {"msg_type", "comm_type", "reply_code", "drives_powered", "estopped", "error_code", "in_error", "in_motion", "mode", "motion_possible"};
    //     int start = 0;
    //     int val;
    //     RCLCPP_INFO_STREAM(get_logger(), "Packet length: " << length);
    //     for (auto label : status_labels){
    //       char bytes[4] = {status_packet[start], status_packet[start+1], status_packet[start+2], status_packet[start+3]};
    //       val = bin_to_int(bytes);
    //       RCLCPP_INFO_STREAM(get_logger(), label << ": " << val);
    //       start+=4;
    //     }
    //     RCLCPP_INFO(get_logger(), "\n");
    //   }
    //   else{
    //     RCLCPP_INFO_STREAM(get_logger(), "Reading garbage data of length " << length);

    //     char *throwaway = new char[length];
        
    //     socket_read::read_socket(state_socket_, throwaway, length);

    //     delete[] throwaway;
    //   }
    // }
  }

  void MotomanHardwareInterface::write_joints(){

  }
}



ssize_t socket_read::read_socket(int __fd, void *__buf, size_t __nbytes) {
  return read(__fd, __buf, __nbytes);
}

ssize_t socket_write::write_socket(int __fd, void *__buf, size_t __nbytes) {
  return write(__fd, __buf, __nbytes);
}


#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(motoman_hardware::MotomanHardwareInterface, hardware_interface::SystemInterface)