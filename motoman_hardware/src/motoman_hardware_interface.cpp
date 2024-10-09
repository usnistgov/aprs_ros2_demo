#include "motoman_hardware/motoman_hardware_interface.hpp"

#include "hardware_interface/types/hardware_interface_type_values.hpp"



namespace motoman_hardware {
  hardware_interface::CallbackReturn MotomanHardwareInterface::on_init(const hardware_interface::HardwareInfo& info)
  {
    if (hardware_interface::SystemInterface::on_init(info) != CallbackReturn::SUCCESS) {
      return hardware_interface::CallbackReturn::ERROR;
    }

    if (int(info_.joints.size()) != num_urdf_joints_) {
      RCLCPP_ERROR(get_logger(), "Got %d joints. Expected %d.", int(info_.joints.size()), num_urdf_joints_);
      return hardware_interface::CallbackReturn::ERROR;
    }

    if(!setup_sockets()){
      RCLCPP_ERROR(get_logger(), "Unable to setup sockets");
      return hardware_interface::CallbackReturn::ERROR;
    }

    hw_positions_.resize(num_urdf_joints_, std::numeric_limits<double>::quiet_NaN());
    hw_velocities_.resize(num_urdf_joints_, std::numeric_limits<double>::quiet_NaN());
    hw_accelerations_.resize(num_urdf_joints_, std::numeric_limits<double>::quiet_NaN());
    hw_commands_.resize(num_urdf_joints_, std::numeric_limits<double>::quiet_NaN());

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
      RCLCPP_ERROR(get_logger(), "Unable to connect to sockets");
      return hardware_interface::CallbackReturn::ERROR;
    }

    MotoMotionCtrl start_traj_mode("START_TRAJ_MODE");

    std::vector<uint8_t> start_traj_mode_bytes = start_traj_mode.to_bytes();
    
    MotoMotionReply reply = send_motion_msg(start_traj_mode_bytes);

    sleep(1);

    read_joints();

    if(is_gripper_opened()){
      hw_positions_[7] = 0.02;
      hw_positions_[8] = 0.02;
    } else {
      hw_positions_[7] = 0.0;
      hw_positions_[8] = 0.0;
    }

    hw_commands_[7] = hw_positions_[7];
    hw_commands_[8] = hw_positions_[8];

    // Set commands equal to current state
    for (int i = 0; i < num_robot_joints_; i++)
    {
      hw_positions_[i] = current_joint_feedback_.positions[i];
      hw_commands_[i] = current_joint_feedback_.positions[i];
    }

    prev_hw_commands_ = hw_commands_;

    RCLCPP_INFO_STREAM(get_logger(), reply.output_data());

    sequence++;

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
    MotoMotionReply reply = send_motion_msg(stop_traj_mode_bytes);

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

    for (int i = 0; i < num_robot_joints_; i++)
    {
      // RCLCPP_INFO_STREAM(get_logger(), "Joint " << i << ": " << current_joint_feedback_.positions[i]);
      hw_positions_[i] = current_joint_feedback_.positions[i];
      hw_velocities_[i] = current_joint_feedback_.velocities[i];
      hw_accelerations_[i] = current_joint_feedback_.accelerations[i];
    }

    if(is_gripper_opened()){
      hw_positions_[7] = 0.02;
      hw_positions_[8] = 0.02;
    } else {
      hw_positions_[7] = 0.0;
      hw_positions_[8] = 0.0;
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

    bool motion_requested = false;
    for(int i = 0; i < num_robot_joints_; i++){
      if(prev_hw_commands_[i] != hw_commands_[i]){
        motion_requested = true;
        break;
      }
    }

    bool gripper_requested = (prev_hw_commands_[7] != hw_commands_[7]) && (hw_commands_[7] != hw_positions_[7]);

    if(gripper_requested){
      if(hw_commands_[7] == 0.0){
        close_gripper();
        } 
      else{
        open_gripper();
      }
    }

    if(motion_requested){
      RCLCPP_INFO(get_logger(), "Motion requested true");
    }

    if(motion_requested){
      write_joints();
    }

    prev_hw_commands_ = hw_commands_;
    return hardware_interface::return_type::OK;
  }

  std::vector<hardware_interface::StateInterface> MotomanHardwareInterface::export_state_interfaces()
  {
    std::vector<hardware_interface::StateInterface> state_interfaces;
    
    for (uint i = 0; i < num_urdf_joints_; i++)
    {
      state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &hw_positions_[i]));
      
      if (i < num_robot_joints_) {
        state_interfaces.emplace_back(hardware_interface::StateInterface(
          info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &hw_velocities_[i]));

        state_interfaces.emplace_back(hardware_interface::StateInterface(
          info_.joints[i].name, hardware_interface::HW_IF_EFFORT, &hw_accelerations_[i]));
      }  
    }

    return state_interfaces;
  }

  std::vector<hardware_interface::CommandInterface> MotomanHardwareInterface::export_command_interfaces() 
  {
    std::vector<hardware_interface::CommandInterface> command_interfaces;
    
    for (uint i = 0; i < num_urdf_joints_; i++)
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
    JointTrajPtFull initial_trajectory_point(0, current_joint_feedback_.positions, current_joint_feedback_);

    for(auto position : initial_trajectory_point.positions){
      RCLCPP_INFO_STREAM(get_logger(), position);
    }

    initial_trajectory_point.time = 0.0;
    MotoMotionReply reply = send_motion_msg(initial_trajectory_point.to_bytes());

    std::vector<float> new_positions(10, 0);
    for(int i = 0; i < num_robot_joints_; i++){
      new_positions[i] = (hw_commands_[i]);
    }

    for(auto position: new_positions){
      RCLCPP_INFO_STREAM(get_logger(), position);
    }

    JointTrajPtFull target_trajectory_point(1, new_positions, current_joint_feedback_);
    reply = send_motion_msg(target_trajectory_point.to_bytes());

    sequence++;

    return true;
  }

  MotoMotionReply MotomanHardwareInterface::send_motion_msg(std::vector<uint8_t> byte_stream){
    socket_write::write_socket(motion_socket_, byte_stream.data(), byte_stream.size());
    
    int length = get_packet_length(motion_socket_);

    RCLCPP_INFO_STREAM(get_logger(), "Length: " << length);

    if(length == 72){
      char *data = new char[length];
      socket_read::read_socket(motion_socket_, data, length);
      MotoMotionReply reply(data);
      RCLCPP_INFO_STREAM(get_logger(), "Reply result: " << reply.result);
      if(reply.result != 0){
        RCLCPP_ERROR_STREAM(get_logger(), "Unable to send motion control message with command type: " << reply.command << ". Result code: "<<reply.result << " Subcode: " << reply.subcode);
      }
      return reply;
    }
    else{
      char* dummy_str;
      MotoMotionReply dummy_reply(dummy_str);
      RCLCPP_ERROR(get_logger(), "Message recieved from motion socket not a motion reply.");
      return dummy_reply;
    }
  }

  bool MotomanHardwareInterface::open_gripper(){
    WriteIOBit open_gripper_io(10010, "on");
    std::vector<uint8_t> byte_stream = open_gripper_io.to_bytes();
    socket_write::write_socket(io_socket_, byte_stream.data(), byte_stream.size());
    int length = get_packet_length(io_socket_);
    
    if (length == 16){
      char *data = new char[length];
      socket_read::read_socket(io_socket_, data, length);
      WriteIOReply reply(data);
      if(reply.result != 0){
        RCLCPP_ERROR(get_logger(), "Unable to write open_gripper to true to socket");
        return false;
      }
    } else {
      RCLCPP_ERROR_STREAM(get_logger(), "Invalid response recieved from write_io msg. Recieved message with length: " << length);
      return false;
    }
    
    WriteIOBit close_gripper_io(10011, "off");
    byte_stream = close_gripper_io.to_bytes();
    socket_write::write_socket(io_socket_, byte_stream.data(), byte_stream.size());

    length = get_packet_length(io_socket_);
    
    if (length == 16){
      char *data = new char[length];
      socket_read::read_socket(io_socket_, data, length);
      WriteIOReply reply(data);
      if(reply.result != 0){
        RCLCPP_ERROR(get_logger(), "Unable to write close_gripper to false to socket");
        return false;
      }
    } else {
      RCLCPP_ERROR_STREAM(get_logger(), "Invalid response recieved from write_io msg. Recieved message with length: " << length);
      return false;
    }
    
    WriteIOBit air_io(10012, "on");
    byte_stream = air_io.to_bytes();
    socket_write::write_socket(io_socket_, byte_stream.data(), byte_stream.size());
    length = get_packet_length(io_socket_);
    
    if (length == 16){
      char *data = new char[length];
      socket_read::read_socket(io_socket_, data, length);
      WriteIOReply reply(data);
      if(reply.result != 0){
        RCLCPP_ERROR(get_logger(), "Unable to write air to true to socket");
        return false;
      }
    } else {
      RCLCPP_ERROR_STREAM(get_logger(), "Invalid response recieved from write_io msg. Recieved message with length: " << length);
      return false;
    }

    return true;
  }

  bool MotomanHardwareInterface::close_gripper(){
    WriteIOBit open_gripper_io(10010, "off");
    std::vector<uint8_t> byte_stream = open_gripper_io.to_bytes();
    socket_write::write_socket(io_socket_, byte_stream.data(), byte_stream.size());

    int length = get_packet_length(io_socket_);
    
    if (length == 16){
      char *data = new char[length];
      socket_read::read_socket(io_socket_, data, length);
      WriteIOReply reply(data);
      if(reply.result != 0){
        RCLCPP_ERROR(get_logger(), "Unable to write open_gripper to false to socket");
        return false;
      }
    } else {
      RCLCPP_ERROR_STREAM(get_logger(), "Invalid response recieved from write_io msg. Recieved message with length: " << length);
      return false;
    }
    
    WriteIOBit close_gripper_io(10011, "on");
    byte_stream = close_gripper_io.to_bytes();
    socket_write::write_socket(io_socket_, byte_stream.data(), byte_stream.size());

    length = get_packet_length(io_socket_);
    
    if (length == 16){
      char *data = new char[length];
      socket_read::read_socket(io_socket_, data, length);
      WriteIOReply reply(data);
      if(reply.result != 0){
        RCLCPP_ERROR(get_logger(), "Unable to write close_gripper to true to socket");
        return false;
      }
    } else {
      RCLCPP_ERROR_STREAM(get_logger(), "Invalid response recieved from write_io msg. Recieved message with length: " << length);
      return false;
    }
    
    WriteIOBit air_io(10012, "on");
    byte_stream = air_io.to_bytes();
    socket_write::write_socket(io_socket_, byte_stream.data(), byte_stream.size());

    length = get_packet_length(io_socket_);
    
    if (length == 16){
      char *data = new char[length];
      socket_read::read_socket(io_socket_, data, length);
      WriteIOReply reply(data);
      if(reply.result != 0){
        RCLCPP_ERROR(get_logger(), "Unable to write air to false to socket");
        return false;
      }
    } else {
      RCLCPP_ERROR_STREAM(get_logger(), "Invalid response recieved from write_io msg. Recieved message with length: " << length);
      return false;
    }
    
    return true;
  }

  bool MotomanHardwareInterface::is_gripper_opened(){
    ReadIOBit check_gripper_open(10010);
    std::vector<uint8_t> gripper_byte_stream = check_gripper_open.to_bytes();
    socket_write::write_socket(io_socket_, gripper_byte_stream.data(), gripper_byte_stream.size());
    int length = get_packet_length(io_socket_);
    bool gripper_opened;
    if (length == 20){
      char *data = new char[length];
      socket_read::read_socket(io_socket_, data, length);
      ReadIOReply reply(data);
      if(reply.result != 0){
        RCLCPP_ERROR(get_logger(), "Unable to read open_gripper address");
      } else {
        gripper_opened = (reply.value == 1);
      }
    } else {
      RCLCPP_ERROR_STREAM(get_logger(), "Invalid response recieved from read_io msg. Recieved message with length: " << length);
    }
    return gripper_opened;
  }

  MotomanHardwareInterface::~MotomanHardwareInterface(){
    on_deactivate(rclcpp_lifecycle::State());
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