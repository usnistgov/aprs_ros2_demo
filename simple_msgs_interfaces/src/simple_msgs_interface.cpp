#include "simple_msgs_interfaces/simple_msgs_interface.hpp"

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

void MotoMotionReply::output_data(){
  // RCLCPP_INFO_STREAM(get_logger(), "Reply information:\n");
  // RCLCPP_INFO_STREAM(get_logger(), "\tRobot ID: " << robot_id);
  // RCLCPP_INFO_STREAM(get_logger(), "\tSequence: " << sequence);
  // RCLCPP_INFO_STREAM(get_logger(), "\tCommand: " << command_codes_to_str_[command]);
  // RCLCPP_INFO_STREAM(get_logger(), "\tResult: " << result_codes_to_str_[result]);
}

WriteIOBit::WriteIOBit(int target_address, std::string str_val){
  address = target_address;
  value = values_str_to_int_[str_val];
}

std::vector<uint8_t> WriteIOBit::to_bytes(){
  std::vector<uint8_t> byte_array;

  to_byte_and_insert(byte_array, length);
  to_byte_and_insert(byte_array, msg_type);
  to_byte_and_insert(byte_array, comm_type);
  to_byte_and_insert(byte_array, reply_code);
  to_byte_and_insert(byte_array, address);
  to_byte_and_insert(byte_array, value);

  return byte_array;
}

WriteIOReply::WriteIOReply(char* byte_stream){
  std::vector<int> socket_info_vector;
  char temp[4];
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      temp[j] = *(byte_stream+j);
    }
    byte_stream+=4;
    socket_info_vector.push_back(ntohl(*(uint32_t*)temp));
  }
  msg_type = socket_info_vector[0];
  comm_type = socket_info_vector[1];
  reply_code = socket_info_vector[2];
  result = socket_info_vector[3];
}

ReadIOBit::ReadIOBit(int target_address){
  address = target_address;
}

std::vector<uint8_t> ReadIOBit::to_bytes(){
  std::vector<uint8_t> byte_array;

  to_byte_and_insert(byte_array, length);
  to_byte_and_insert(byte_array, msg_type);
  to_byte_and_insert(byte_array, comm_type);
  to_byte_and_insert(byte_array, reply_code);
  to_byte_and_insert(byte_array, address);

  return byte_array;
}

ReadIOReply::ReadIOReply(char* byte_stream){
  std::vector<int> socket_info_vector;
  char temp[4];
  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 4; j++){
      temp[j] = *(byte_stream+j);
    }
    byte_stream+=4;
    socket_info_vector.push_back(ntohl(*(uint32_t*)temp));
  }
  msg_type = socket_info_vector[0];
  comm_type = socket_info_vector[1];
  reply_code = socket_info_vector[2];
  value = socket_info_vector[3];
  result = socket_info_vector[4];
}