#include "aprs_driver/fanuc_hardware_interface.hpp"

#include "hardware_interface/types/hardware_interface_type_values.hpp"


namespace fanuc_hardware {
 
  hardware_interface::CallbackReturn FanucHardwareInterface::on_init(const hardware_interface::HardwareInfo& info)
  {
  
    if (hardware_interface::SystemInterface::on_init(info) != CallbackReturn::SUCCESS) {
      return hardware_interface::CallbackReturn::ERROR;
    }

    if (int(info_.joints.size()) != number_of_joints_) {
      RCLCPP_FATAL(get_logger(), "Got %d joints. Expected %d.", int(info_.joints.size()), number_of_joints_);
      return hardware_interface::CallbackReturn::ERROR;
    }

    // Create socket
    if((state_sock_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
      RCLCPP_ERROR(get_logger(), "State Socket creation error" );
      return hardware_interface::CallbackReturn::FAILURE;
    }

    state_socket_.sin_family = AF_INET;
    state_socket_.sin_port = htons(state_port_);

    // Convert IP addresses from text to binary form
    if(inet_pton(AF_INET, robot_ip_, &state_socket_.sin_addr) <= 0) 
    {
      RCLCPP_ERROR(get_logger(), "Invalid address / Address not supported");
      return hardware_interface::CallbackReturn::FAILURE;
    }

    // Create motion socket
    if((motion_sock_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
      RCLCPP_ERROR(get_logger(), "Motion Socket creation error" );
      return hardware_interface::CallbackReturn::FAILURE;
    }

    motion_socket_.sin_family = AF_INET;
    motion_socket_.sin_port = htons(motion_port_);

    // Convert IP addresses from text to binary form
    if(inet_pton(AF_INET, robot_ip_, &motion_socket_.sin_addr) <= 0) 
    {
      RCLCPP_ERROR(get_logger(), "Invalid address / Address not supported");
      return hardware_interface::CallbackReturn::FAILURE;
    }


    hw_states_.resize(info_.joints.size(), std::numeric_limits<double>::quiet_NaN());
    hw_commands_.resize(info_.joints.size(), std::numeric_limits<double>::quiet_NaN());

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::CallbackReturn FanucHardwareInterface::on_configure(const rclcpp_lifecycle::State& previous_state)
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

  hardware_interface::CallbackReturn FanucHardwareInterface::on_activate(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;

    // Connect to the State server
    if(connect(state_sock_, (struct sockaddr *)&state_socket_, sizeof(state_socket_)) < 0) 
    {
      RCLCPP_ERROR(get_logger(), "Connection failed");
      return hardware_interface::CallbackReturn::FAILURE;
    }

    state_socket_created_ = true;

    // Connect to the Motion server
    if(connect(motion_sock_, (struct sockaddr *)&motion_socket_, sizeof(motion_socket_)) < 0) 
    {
      RCLCPP_ERROR(get_logger(), "Connection failed");
      return hardware_interface::CallbackReturn::FAILURE;
    }

    motion_socket_created_ = true;

    auto ret = read_joints();

    while(!ret.first){
      ret = read_joints();
    }

    // if (!ret.first){
    //   return hardware_interface::CallbackReturn::FAILURE;
    // }

    std::vector<float> current_positions = ret.second;

    for (uint i = 0; i < hw_states_.size(); i++)
    {
      hw_states_[i] = current_positions[i];
      hw_commands_[i] = current_positions[i];
    }

    prev_hw_commands_ = hw_commands_;

    flag = true;

    RCLCPP_INFO(get_logger(), "Successfully activated!");

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::CallbackReturn FanucHardwareInterface::on_deactivate(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;

    if (state_socket_created_){
      close(state_sock_);
    }

    if (motion_socket_created_){
      close(motion_sock_);
    }

    RCLCPP_INFO(get_logger(), "Successfully deactivated!");

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::return_type FanucHardwareInterface::read(const rclcpp::Time& time, const rclcpp::Duration& period)
  {
    (void)time;
    (void)period;

    auto ret = read_joints();

    if (!ret.first){
      return hardware_interface::return_type::OK;
    }

    std::vector<float> current_positions = ret.second;

    for (uint i = 0; i < hw_states_.size(); i++)
    {
      hw_states_[i] = current_positions[i];
    }

    return hardware_interface::return_type::OK;
  }

  hardware_interface::return_type FanucHardwareInterface::write(const rclcpp::Time& time, const rclcpp::Duration& period)
  {
    (void)time;
    (void)period;

    if (prev_hw_commands_ != hw_commands_ && flag)
    {
      auto ret = write_joints();

      if (!ret.first){
        return hardware_interface::return_type::OK;
      }

      if (send(motion_sock_, ret.second.data(), ret.second.size(), 0) < 0)
      {
        RCLCPP_INFO(get_logger(), "Send failed");
        return hardware_interface::return_type::OK;
      }
      
      char buffer[1024] = {0};

      if (recv(motion_sock_, buffer, sizeof(buffer)-1,0) < 0)
      {
        RCLCPP_INFO(get_logger(),  "Receive failed");
        return hardware_interface::return_type::OK;
      }

      prev_hw_commands_ = hw_commands_;
    }

    return hardware_interface::return_type::OK;
  }

  std::vector<hardware_interface::StateInterface> FanucHardwareInterface::export_state_interfaces()
  {
    std::vector<hardware_interface::StateInterface> state_interfaces;
    
    for (uint i = 0; i < info_.joints.size(); i++)
    {
      state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &hw_states_[i]));
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

  std::pair<bool, std::vector<float>> FanucHardwareInterface::read_joints(){

    std::vector<float> joint_positions = {0, 0, 0, 0, 0, 0};

    // RCLCPP_INFO(get_logger(), "Reading joint states");
    
    // Read 4 bytes from socket to length of next packet 
    int length = get_packet_length();

    if (length < 0){
      RCLCPP_ERROR(get_logger(), "Issue with socket...reconnecting");

      close(state_sock_);
      state_sock_ = socket(AF_INET, SOCK_STREAM, 0);
      connect(state_sock_, (struct sockaddr *)&state_socket_, sizeof(state_socket_));
      length = get_packet_length();
    }

    if (length == 40){
      char *status_packet = new char[40];
      
      socket_read::read_socket(state_sock_, status_packet, 40);

      delete[] status_packet;

      length = get_packet_length();
    }

    if (length == state_buffer_length_){

      char *state_packet = new char[state_buffer_length_];
      
      socket_read::read_socket(state_sock_, state_packet, state_buffer_length_);

      // Check that the message type is correct
      char msg_type[4] = {state_packet[0], state_packet[1], state_packet[2], state_packet[3]};

      if (bin_to_int(msg_type) != 10){
        RCLCPP_INFO(get_logger(), "Message type is not Joint Position");
        return std::make_pair(false, joint_positions);
      }

      int start = 0;
      float joint_value = 0.0;
      joint_positions.clear();

      for (int i=0; i<6; i++){
        start = 16 + (4*i);
        
        char joint_bytes[4] = {state_packet[start], state_packet[start+1], state_packet[start+2], state_packet[start+3]};

        joint_value = bin_to_float(joint_bytes);

        joint_positions.push_back(joint_value);

        // RCLCPP_INFO_STREAM(get_logger(), "Joint " << std::to_string(i+1).c_str()  << ": " << std::to_string(joint_value));
      }

      // Correct for J2/J3 Interaction

      joint_positions[2] += joint_positions[1];

      delete[] state_packet;

      return std::make_pair(true, joint_positions);
    }

    RCLCPP_WARN_STREAM(get_logger(), "Packet length: " << std::to_string(length).c_str());
    return std::make_pair(false, joint_positions);
  }

  std::pair<bool, const std::vector<uint8_t>> FanucHardwareInterface::write_joints(){

    float velocity = 0.1;
    float duration = 10.0;
    std::vector<uint8_t> byte_array;

    uint32_t prefix_length = htonl(0x00000040);
    uint32_t msg_type = htonl(0x0000000b);
    uint32_t comm_type = htonl(0x00000002);
    uint32_t reply_code = 0x00;
    uint32_t sequence = htonl(0x00000001);

    std::vector<uint32_t> joint_data;
    for (float val : hw_commands_) {
        joint_data.push_back(float_to_ieee754(val));
    }
    while (joint_data.size() < 10) {
        joint_data.push_back(0x00000000);
    }

    uint32_t velocity_ieee754 = float_to_ieee754(velocity);
    uint32_t duration_ieee754 = float_to_ieee754(duration);

    byte_array.insert(byte_array.end(), reinterpret_cast<uint8_t*>(&prefix_length), reinterpret_cast<uint8_t*>(&prefix_length) + 4);
    byte_array.insert(byte_array.end(), reinterpret_cast<uint8_t*>(&msg_type), reinterpret_cast<uint8_t*>(&msg_type) + 4);
    byte_array.insert(byte_array.end(), reinterpret_cast<uint8_t*>(&comm_type), reinterpret_cast<uint8_t*>(&comm_type) + 4);
    byte_array.insert(byte_array.end(), reinterpret_cast<uint8_t*>(&reply_code), reinterpret_cast<uint8_t*>(&reply_code) + 4);
    byte_array.insert(byte_array.end(), reinterpret_cast<uint8_t*>(&sequence), reinterpret_cast<uint8_t*>(&sequence) + 4);

    for (uint32_t data : joint_data) {
        byte_array.insert(byte_array.end(), reinterpret_cast<uint8_t*>(&data), reinterpret_cast<uint8_t*>(&data) + 4);
    }

    byte_array.insert(byte_array.end(), reinterpret_cast<uint8_t*>(&velocity_ieee754), reinterpret_cast<uint8_t*>(&velocity_ieee754) + 4);
    byte_array.insert(byte_array.end(), reinterpret_cast<uint8_t*>(&duration_ieee754), reinterpret_cast<uint8_t*>(&duration_ieee754) + 4);

    return std::make_pair(true, byte_array);

  }

  int FanucHardwareInterface::get_packet_length()
  {
    char *length_packet = new char[4];

    ssize_t ret = socket_read::read_socket(state_sock_, length_packet, 4);

    if (ret < 0){
      RCLCPP_ERROR(get_logger(), "Unable to read from socket");
      return -1;
    }

    // Reverse packet 
    int length =  bin_to_int(length_packet);

    delete[] length_packet;

    return length;
  }

  int FanucHardwareInterface::bin_to_int(char* data)
  {
    int result;

    char reversed[4] = {data[3], data[2], data[1], data[0]};

    std::memcpy(&result, reversed, sizeof(int));

    return result;
  }

  float FanucHardwareInterface::bin_to_float(char* data)
  {
    float result;

    char reversed[4] = {data[3], data[2], data[1], data[0]};

    std::memcpy(&result, reversed, sizeof(float));

    return result;
  }

  uint32_t FanucHardwareInterface::float_to_ieee754(float value) {
    FloatUnion fu;
    fu.f = value;
    return htonl(fu.i);
  }

}


ssize_t socket_read::read_socket(int __fd, void *__buf, size_t __nbytes) {
  return read(__fd, __buf, __nbytes);
}

ssize_t socket_write::write_socket(int __fd, void *__buf, size_t __nbytes) {
  return write(__fd, __buf, __nbytes);
}


#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(fanuc_hardware::FanucHardwareInterface, hardware_interface::SystemInterface)