#include <aprs_driver/simple_messages.hpp>

/*
==============================================================================
STATUS MESSAGE
==============================================================================
*/

bool Status::init(char* input)
{
  std::vector<int> input_data;
  
  char temp[4];
  for(int i = 0; i < 10; i++){
    for(int j = 0; j < 4; j++){
      temp[j] = *(input + j);
    }

    int byte_value = ntohl(*(uint32_t*)temp);

    // Check that message type is correct
    if(i == 0 && byte_value!=msg_type){
      return false; 
    }
    
    input_data.push_back(byte_value);

    input += 4;
  }
  
  comm_type = input_data[1];
  reply_code = input_data[2];
  drives_powered = input_data[3];
  e_stopped = input_data[4];
  error_code = input_data[5];
  in_error = input_data[6];
  in_motion = input_data[7];
  mode = input_data[8];
  motion_possible = input_data[9];

  return true;
}

std::string Status::output()
{
  std::stringstream s;

  s << "STATUS MESSAGE\n";
  s << "\tdrives_powered: " << drives_powered << "\n";
  s << "\te_stopped: " << e_stopped << "\n";
  s << "\terror_code: " << error_code << "\n";
  s << "\tin_error: " << in_error << "\n";
  s << "\tin_motion: " << in_motion << "\n";
  s << "\tmode: " << mode << "\n";
  s << "\tmotion_possible: " << motion_possible << "\n";

  return s.str();
}

/*
==============================================================================
JOINT FEEDBACK MESSAGE
==============================================================================
*/

bool JointFeedback::init(char* input)
{
  std::vector<int> input_data;
  char temp[4];

  // Get integer input_data
  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 4; j++){
      temp[j] = *(input+j);
    }

    int byte_value = ntohl(*(uint32_t*)temp);

    // Check that message type is correct
    if(i == 0 && byte_value!=msg_type){
      return false; 
    }

    input_data.push_back(byte_value);

    input+=4;
  }

  comm_type = input_data[1];
  reply_code = input_data[2];
  robot_id = input_data[3];
  valid_fields = input_data[4];

  // Get time
  for(int j = 0; j < 4; j++){
    temp[j] = *(input + j);
  }
  input += 4;

  time = bytes_to_float(temp);

  // Get positions
  for(int i = 0; i < 10; i++){
    for(int j = 0; j < 4; j++){
      temp[j] = *(input+j);
    }
    input+=4;
    positions.push_back(bytes_to_float(temp));
  }

  // Get velocities
  for(int i = 0; i < 10; i++){
    for(int j = 0; j < 4; j++){
      temp[j] = *(input+j);
    }
    input+=4;
    velocities.push_back(bytes_to_float(temp));
  }

  // Get accelerations
  for(int i = 0; i < 10; i++){
    for(int j = 0; j < 4; j++){
      temp[j] = *(input+j);
    }
    input+=4;
    accelerations.push_back(bytes_to_float(temp));
  }

  return true;
}

std::string JointFeedback::output()
{
  std::stringstream s;

  s << "JOINT FEEDBACK MESSAGE\n";
  s << "\trobot id: " << robot_id << "\n";
  s << "\tvalid fields: " << valid_fields << "\n";
  s << "\ttime: " << time << "\n";

  int i=0;
  s << "\tpositions:\n";
  for (float p : positions){
    s << "\t\tjoint" << i << ": " << p << "\n";
    i++;
  }

  i=0;
  s << "\velocities:\n";
  for (float v : velocities){
    s << "\t\tjoint" << i << ": " << v << "\n";
    i++;
  }

  i=0;
  s << "\accelerations:\n";
  for (float a : accelerations){
    s << "\t\tjoint" << i << ": " << a << "\n";
    i++;
  }

  return s.str();
}

/*
==============================================================================
JOINT POSITION
==============================================================================
*/
bool JointPosition::init(char* input)
{
  std::vector<int> input_data;
  char temp[4];

  // Get integer input_data
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      temp[j] = *(input+j);
    }

    int byte_value = ntohl(*(uint32_t*)temp);

    // Check that message type is correct
    if(i == 0 && byte_value!=msg_type){
      return false; 
    }

    input_data.push_back(byte_value);

    input+=4;
  }

  comm_type = input_data[1];
  reply_code = input_data[2];
  sequence = input_data[3];

  // Get positions
  for(int i = 0; i < 10; i++){
    for(int j = 0; j < 4; j++){
      temp[j] = *(input+j);
    }
    input+=4;
    joint_data.push_back(bytes_to_float(temp));
  }

  return true;
}

std::string JointPosition::output()
{
  std::stringstream s;

  s << "JOINT POSITION MESSAGE\n";
  s << "\tsequence: " << sequence << "\n";

  int i=0;
  s << "\tpositions:\n";
  for (float p : joint_data){
    s << "\t\tjoint" << i << ": " << p << "\n";
    i++;
  }

  return s.str();
}

/* 
WRITE IO REPLY
*/
bool WriteIOReply::init(char* input){
  std::vector<int> input_data;
  char temp[4];

  // Get integer input_data
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      temp[j] = *(input+j);
    }

    int byte_value = ntohl(*(uint32_t*)temp);

    // Check that message type is correct
    if(i == 0 && byte_value!=msg_type){
      return false; 
    }

    input_data.push_back(byte_value);

    input+=4;
  }

  comm_type = input_data[1];
  reply_code = input_data[2];
  result = input_data[3];
}

std::string WriteIOReply::output()
{
  std::stringstream s;

  s << "WRITE IO REPLY\n";
  s << "\tresult: " << result << "\n";

  return s.str();
}

/*
============================================================================== 
READ IO REPLY
==============================================================================
*/

bool ReadIOReply::init(char* input){
  std::vector<int> input_data;
  char temp[4];

  // Get integer input_data
  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 4; j++){
      temp[j] = *(input+j);
    }

    int byte_value = ntohl(*(uint32_t*)temp);

    // Check that message type is correct
    if(i == 0 && byte_value!=msg_type){
      return false; 
    }

    input_data.push_back(byte_value);

    input+=4;
  }

  comm_type = input_data[1];
  reply_code = input_data[2];
  value = input_data[3];
  result = input_data[4];
}

std::string ReadIOReply::output()
{
  std::stringstream s;

  s << "READ IO REPLY\n";
  s << "\tvalue: " << value << "\n";
  s << "\tresult: " << result << "\n";

  return s.str();
}

/*
==============================================================================
MOTO MOTION REPLY
==============================================================================
*/
bool MotoMotionReply::init(char* input){
  std::vector<int> input_data;
  char temp[4];

  // Get integer input_data
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 4; j++){
      temp[j] = *(input+j);
    }

    int byte_value = ntohl(*(uint32_t*)temp);

    // Check that message type is correct
    if(i == 0 && byte_value!=msg_type){
      return false; 
    }

    input_data.push_back(byte_value);

    input+=4;
  }

  comm_type = input_data[1];
  reply_code = input_data[2];
  robot_id = input_data[3];
  sequence = input_data[4];
  command = input_data[5];
  result = input_data[6];
  subcode = input_data[7];

  for(int i = 0; i < 10; i++){
    for(int j = 0; j < 4; j++){
      temp[j] = *(input+j);
    }
    input+=4;
    data.push_back(bytes_to_float(temp));
  }
}

std::string MotoMotionReply::output()
{
  std::stringstream s;

  s << "JOINT FEEDBACK MESSAGE\n";
  s << "\trobot id: " << robot_id << "\n";
  s << "\tvalid fields: " << sequence << "\n";

  if (command_codes.find(command) != command_codes.end())
    s << "\tcommand: " << command_codes[command] << "\n";
  else
    s << "\tcommand: " << "NOT_FOUND (" << command << ")\n";

  if (result_codes.find(result) != result_codes.end())
    s << "\rresult: " << result_codes[result] << "\n";
  else
    s << "\tresult: " << "NOT_FOUND (" << result << ")\n";

  if (subcodes.find(subcode) != subcodes.end())
    s << "\tsubcode: " << subcodes[subcode] << "\n";
  else
    s << "\tsubcode: " << "NOT_FOUND (" << subcode << ")\n";

  s << "\tdata:\n";
  for (float d : data){
    s << "\t\t" << d << "\n";
  }

  return s.str();
}

/*
==============================================================================
UTILITIES
==============================================================================
*/

float bytes_to_float(char* bytes)
{
  float result;

  uint32_t temp = ntohl(*(uint32_t*)bytes);
  std::memcpy(&result, &temp, sizeof(float));

  return result;
}