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

    hw_positions_.resize(info_.joints.size(), std::numeric_limits<double>::quiet_NaN());
    hw_velocities_.resize(info_.joints.size(), std::numeric_limits<double>::quiet_NaN());
    hw_accelerations_.resize(info_.joints.size(), std::numeric_limits<double>::quiet_NaN());
    hw_commands_.resize(info_.joints.size(), std::numeric_limits<double>::quiet_NaN());

    current_joint_feedback_.positions = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    current_joint_feedback_.velocities = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    current_joint_feedback_.accelerations = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    
    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::CallbackReturn MotomanHardwareInterface::on_configure(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;

    for (uint i = 0; i < hw_positions_.size(); i++)
    {
      hw_positions_[i] = 0;
      hw_velocities_[i] = 0;
      hw_accelerations_[i] = 0;
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

    MotoMotionCtrl start_traj_mode("START_TRAJ_MODE");

    std::vector<uint8_t> start_traj_mode_bytes = start_traj_mode.to_bytes();
    socket_write::write_socket(motion_socket_, start_traj_mode_bytes.data(), start_traj_mode_bytes.size());

    int length = get_packet_length(motion_socket_);

    if(length == 72){
      char *data = new char[length];
      socket_read::read_socket(motion_socket_, data, length);
      MotoMotionReply reply(data);
      if(reply.result != 0){
        RCLCPP_FATAL_STREAM(get_logger(), "Unable to start trajectory mode. Result code: "<<reply.result << " Subcode: " << reply.subcode);
        return hardware_interface::CallbackReturn::ERROR;
      }
    }
    else{
      RCLCPP_ERROR(get_logger(), "Message recieved from motion socket not a motion reply.");
    }
    

    read_joints();

    // Set commands equal to current state
    for (int i = 0; i < number_of_joints_; i++)
    {
      hw_positions_[i] = current_joint_feedback_.positions[i];
      hw_commands_[i] = current_joint_feedback_.positions[i];
    }

    prev_hw_commands_ = hw_commands_;
    RCLCPP_INFO(get_logger(), "Successfully activated!");

    activated = true;

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::CallbackReturn MotomanHardwareInterface::on_deactivate(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;

    RCLCPP_INFO(get_logger(), "Inside on_deactivate");

    MotoMotionCtrl stop_traj_mode("STOP_TRAJ_MODE");

    std::vector<uint8_t> stop_traj_mode_bytes = stop_traj_mode.to_bytes();
    socket_write::write_socket(motion_socket_, stop_traj_mode_bytes.data(), stop_traj_mode_bytes.size());

    int length = get_packet_length(motion_socket_);

    if(length == 72){
      char *data = new char[length];
      socket_read::read_socket(motion_socket_, data, length);
      MotoMotionReply reply(data);
      if(reply.result != 0){
        RCLCPP_FATAL_STREAM(get_logger(), "Unable to stop trajectory mode. Result code: "<<reply.result << " Subcode: " << reply.subcode);
        return hardware_interface::CallbackReturn::ERROR;
      }
    }
    else{
      RCLCPP_ERROR(get_logger(), "Message recieved from motion socket not a motion reply.");
    }

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

    for (int i = 0; i < number_of_joints_; i++)
    {
      hw_positions_[i] = current_joint_feedback_.positions[i];
      hw_velocities_[i] = current_joint_feedback_.velocities[i];
      hw_accelerations_[i] = current_joint_feedback_.accelerations[i];
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

    motion_requested = (prev_hw_commands_ != hw_commands_);

    if(motion_requested){
      bool ret = write_joints();
      if(!ret){
        return hardware_interface::return_type::OK;
      }
      prev_hw_commands_ = hw_commands_;
    }

    return hardware_interface::return_type::OK;
  }

  std::vector<hardware_interface::StateInterface> MotomanHardwareInterface::export_state_interfaces()
  {
    std::vector<hardware_interface::StateInterface> state_interfaces;
    
    for (uint i = 0; i < info_.joints.size(); i++)
    {
      state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &hw_positions_[i]));
      
      state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &hw_velocities_[i]));

      state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_EFFORT, &hw_accelerations_[i]));
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
    // close(motion_socket_);
    close(io_socket_);
  }

  int MotomanHardwareInterface::get_packet_length(int socket)
  {
    char *length_packet = new char[4];

    ssize_t ret = socket_read::read_socket(socket, length_packet, 4);

    if (ret < 0){
      RCLCPP_ERROR(get_logger(), "Unable to read from socket");
      return -1;
    }

    int length = ntohl(*(uint32_t*)length_packet);

    delete[] length_packet;

    return length;
  }

  void MotomanHardwareInterface::read_joints(){
    int length;

    for(int i = 0; i < 2; i++){
      length = get_packet_length(state_socket_);
      char *data = new char[length];
      socket_read::read_socket(state_socket_, data, length);
      switch(length){
        case STATUS:
          current_status_ = read_status_msg(data);
          break;
        case JOINT_FEEDBACK:
          current_joint_feedback_ = read_joint_feedback_msg(data);
          break;
        default:
          RCLCPP_ERROR(get_logger(), "Unknown message type received");
          return;
      }
    }
  }

  statusMsg MotomanHardwareInterface::read_status_msg(char *byte_stream){
    std::vector<int> status_vector;
    char temp[4];
    for(int i = 0; i < 10; i++){
      for(int j = 0; j < 4; j++){
        temp[j] = *(byte_stream+j);
      }
      byte_stream+=4;
      status_vector.push_back(ntohl(*(uint32_t*)temp));
    }
    statusMsg status;
    status.msg_type = status_vector[0];
    status.comm_type = status_vector[1];
    status.reply_code = status_vector[2];
    status.drives_powered = status_vector[3];
    status.e_stopped = status_vector[4];
    status.error_code = status_vector[5];
    status.in_error = status_vector[6];
    status.in_motion = status_vector[7];
    status.mode = status_vector[8];
    status.motion_possible = status_vector[9];

    return status;
  }

  jointFeedbackMsg MotomanHardwareInterface::read_joint_feedback_msg(char *byte_stream){
    std::vector<int> feedback_vector;
    char temp[4];
    for(int i = 0; i < 5; i++){
      for(int j = 0; j < 4; j++){
        temp[j] = *(byte_stream+j);
      }
      byte_stream+=4;
      feedback_vector.push_back(ntohl(*(uint32_t*)temp));
    }
    jointFeedbackMsg joint_feedback;
    joint_feedback.msg_type = feedback_vector[0];
    joint_feedback.comm_type = feedback_vector[1];
    joint_feedback.reply_code = feedback_vector[2];
    joint_feedback.robot_id = feedback_vector[3];
    joint_feedback.valid_fields = feedback_vector[4];

    for(int j = 0; j < 4; j++){
      temp[j] = *(byte_stream+j);
    }
    byte_stream+=4;

    joint_feedback.time = bin_to_float(temp);

    for(int i = 0; i < 10; i++){
      for(int j = 0; j < 4; j++){
        temp[j] = *(byte_stream+j);
      }
      byte_stream+=4;
      joint_feedback.positions.push_back(bin_to_float(temp));
    }
    for(int i = 0; i < 10; i++){
      for(int j = 0; j < 4; j++){
        temp[j] = *(byte_stream+j);
      }
      byte_stream+=4;
      joint_feedback.velocities.push_back(bin_to_float(temp));
    }
    // RCLCPP_INFO_STREAM(get_logger(), joint_feedback.velocities[0] << "\t" << joint_feedback.velocities[6]);
    for(int i = 0; i < 10; i++){
      for(int j = 0; j < 4; j++){
        temp[j] = *(byte_stream+j);
      }
      byte_stream+=4;
      joint_feedback.accelerations.push_back(bin_to_float(temp));
    }

    return joint_feedback;
  }

  bool MotomanHardwareInterface::write_joints(){
    // JointTrajPtFull initial_trajectory_point(1, current_joint_feedback_.positions, current_joint_feedback_);
    // MotoMotionReply reply = initial_trajectory_point.send_msg_and_get_feedback(motion_socket_);
    
    prev_hw_commands_ = hw_commands_;
    return true;
  }

  MotomanHardwareInterface::~MotomanHardwareInterface(){
    on_deactivate(rclcpp_lifecycle::State());
  }
}

JointTrajPtFull::JointTrajPtFull(int sequence, std::vector<float> goal_positions, jointFeedbackMsg current_position){
  seq = sequence;
  positions = goal_positions;

  while(positions.size() < 10){
    positions.push_back(0.0);
  }

  float max_distance = -1;
  float dist;
  for(int i = 0; i < goal_positions.size(); i++){
    dist = abs(goal_positions[0] - current_position.positions[0]);
    if(dist>max_distance){
      max_distance = dist;
    }
  }
  time = 7.0 * max_distance;
}

std::vector<uint8_t> JointTrajPtFull::to_bytes(){
  std::vector<uint8_t> byte_array;

  to_byte_and_insert(byte_array, length);
  to_byte_and_insert(byte_array, msg_type);
  to_byte_and_insert(byte_array, comm_type);
  to_byte_and_insert(byte_array, reply_code);
  to_byte_and_insert(byte_array, robot_id);
  to_byte_and_insert(byte_array, seq);
  to_byte_and_insert(byte_array, valid_fields);
  to_byte_and_insert(byte_array, float_to_ieee754(time));
  
  for(int i = 0; i < 10; i++){
    to_byte_and_insert(byte_array, float_to_ieee754(positions[i]));
  }

  for(int i = 0; i < 10; i++){
    to_byte_and_insert(byte_array, float_to_ieee754(positions[i]));
  }
  
  for(int i = 0; i < 10; i++){
    to_byte_and_insert(byte_array, float_to_ieee754(positions[i]));
  }
  return byte_array;
}

MotoMotionReply JointTrajPtFull::send_msg_and_get_feedback(int motion_socket){
  std::vector<uint8_t> msg_bytes = to_bytes();
  send(motion_socket, msg_bytes.data(), msg_bytes.size(), 0);
  
  int reply_length = get_reply_packet_length(motion_socket);

  char *data = new char[reply_length];

  socket_read::read_socket(motion_socket, data, reply_length);
  
  return MotoMotionReply(data);
}

MotoMotionCtrl::MotoMotionCtrl(std::string command_type){
  command = command_types_[command_type];
}

std::vector<uint8_t> MotoMotionCtrl::to_bytes(){
  std::vector<uint8_t> byte_array;

  to_byte_and_insert(byte_array, length);
  to_byte_and_insert(byte_array, msg_type);
  to_byte_and_insert(byte_array, comm_type);
  to_byte_and_insert(byte_array, reply_code);
  to_byte_and_insert(byte_array, robot_id);
  to_byte_and_insert(byte_array, seq);
  to_byte_and_insert(byte_array, command);
  
  for(int i = 0; i < 10; i++){
    to_byte_and_insert(byte_array, float_to_ieee754(0.0));
  }

  return byte_array;
}

MotoMotionReply::MotoMotionReply(char* byte_stream){
  std::vector<int> feedback_vector;
  char temp[4];
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 4; j++){
      temp[j] = *(byte_stream+j);
    }
    byte_stream+=4;
    feedback_vector.push_back(ntohl(*(uint32_t*)temp));
  }
  msg_type = feedback_vector[0];
  comm_type = feedback_vector[1];
  reply_code = feedback_vector[2];
  robot_id = feedback_vector[3];
  sequence = feedback_vector[4];
  command = feedback_vector[5];
  result = feedback_vector[6];
  subcode = feedback_vector[7];

  std::vector<float> data;
  for(int i = 0; i < 10; i++){
    for(int j = 0; j < 4; j++){
      temp[j] = *(byte_stream+j);
    }
    byte_stream+=4;
    data.push_back(bin_to_float(temp));
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