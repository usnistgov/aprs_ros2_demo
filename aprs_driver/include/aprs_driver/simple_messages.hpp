#include <rclcpp/rclcpp.hpp>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <cmath>

/*
==============================================================================
STATUS
==============================================================================
*/
class Status{
  bool init(char* input);
  std::string output();

  int packet_length = 40;
  int msg_type = 13;

  int comm_type;
  int reply_code;
  int drives_powered;
  int e_stopped;
  int error_code;
  int in_error;
  int in_motion;
  int mode;
  int motion_possible;
};

/*
==============================================================================
JointFeedback
==============================================================================
*/
class JointFeedback{
  bool init(char* input);
  std::string output();

  int packet_length = 144;
  int msg_type = 15;

  int comm_type;
  int reply_code;
  int robot_id;
  int valid_fields;
  float time;
  std::vector<float> positions;
  std::vector<float> velocities;
  std::vector<float> accelerations;
};

/*
==============================================================================
JointPosition
==============================================================================
*/
class JointPosition{
  bool init(char* input);
  std::string output();

  int packet_length = 56;
  int msg_type = 10;

  int comm_type;
  int reply_code;
  int sequence;
  std::vector<float> joint_data;
};

/*
==============================================================================
WriteIOReply
==============================================================================
*/
class WriteIOReply{
  bool init(char* input);
  std::string output();

  int packet_length = 16;

  int msg_type = 2006;
  int comm_type;
  int reply_code;
  int result;
};

/*
==============================================================================
ReadIOReply
==============================================================================
*/
class ReadIOReply{
  bool init(char* input);
  std::string output();

  int packet_length = 20;
  int msg_type = 2004;

  int comm_type;
  int reply_code;
  int value;
  int result;
};

/*
==============================================================================
MotoMotionReply
==============================================================================
*/
class MotoMotionReply{
  bool init(char* input);
  std::string output();

  int packet_length = 72;
  int msg_type = 2002;

  int comm_type;
  int reply_code;
  int robot_id;
  int sequence;
  int command;
  int result;
  int subcode;
  std::vector<float> data;

  std::map<int, std::string> command_codes = {
    {14, "CHECK_MOTION_READY"},
    {200101, "CHECK_MOTION_READY"}, 
    {200102, "CHECK_QUEUE_CNT"}, 
    {200111, "STOP_MOTION"}, 
    {200121, "START_TRAJ_MODE"},
    {200122, "STOP_TRAJ_MODE"}
  };

  std::map<int, std::string> result_codes = {
    {0, "SUCCESS/TRUE"},
    {1, "BUSY"}, 
    {2, "FAILURE/FALSE"}, 
    {3, "INVALID"}, 
    {4, "ALARM"},
    {5, "NOT_READY"},
    {6, "MP_FAILURE"}
  };

  std::map<int, std::string> subcodes = {
    {0, "ROS_RESULT_SUCCESS/ROS_RESULT_TRUE"},
    {1, "ROS_RESULT_BUSY"},
    {2, "ROS_RESULT_FAILURE/ROS_RESULT_FALSE"},
    {3, "ROS_RESULT_INVALID"},
    {4, "ROS_RESULT_ALARM"},
    {5, "ROS_RESULT_NOT_READY"},
    {6, "ROS_RESULT_MP_FAILURE"},
    {3000, "ROS_RESULT_INVALID_UNSPECIFIED"},
    {3001, "ROS_RESULT_INVALID_MSGSIZE"},
    {3002, "ROS_RESULT_INVALID_MSGHEADER"},
    {3003, "ROS_RESULT_INVALID_MSGTYPE"},
    {3004, "ROS_RESULT_INVALID_GROUPNO"},
    {3005, "ROS_RESULT_INVALID_SEQUENCE"},
    {3006, "ROS_RESULT_INVALID_COMMAND"},
    {3010, "ROS_RESULT_INVALID_DATA"},
    {3011, "ROS_RESULT_INVALID_DATA_START_POS"},
    {3012, "ROS_RESULT_INVALID_DATA_POSITION"},
    {3013, "ROS_RESULT_INVALID_DATA_SPEED"},
    {3014, "ROS_RESULT_INVALID_DATA_ACCEL"},
    {3015, "ROS_RESULT_INVALID_DATA_INSUFFICIENT"},
    {5000, "ROS_RESULT_NOT_READY_UNSPECIFIED"},
    {5001, "ROS_RESULT_NOT_READY_ALARM"},
    {5002, "ROS_RESULT_NOT_READY_ERROR"},
    {5003, "ROS_RESULT_NOT_READY_ESTOP"},
    {5004, "ROS_RESULT_NOT_READY_NOT_PLAY"},
    {5005, "ROS_RESULT_NOT_READY_NOT_REMOTE"},
    {5006, "ROS_RESULT_NOT_READY_SERVO_OFF"},
    {5007, "ROS_RESULT_NOT_READY_HOLD"},
    {5008, "ROS_RESULT_NOT_READY_NOT_STARTED"},
    {5009, "ROS_RESULT_NOT_READY_WAITING_ROS"},
    {11120, "IO_FEEDBACK_WAITING_MP_INCMOVE"},
    {11121, "IO_FEEDBACK_MP_INCMOVE_DONE"}, 
    {11122, "IO_FEEDBACK_MP_INITIALIZATION_DONE"}, 
    {11123, "IO_FEEDBACK_CONNECTSERVERRUNNING"}, 
    {11124, "IO_FEEDBACK_MOTIONSERVERCONNECTED"},
    {11125, "IO_FEEDBACK_STATESERVERCONNECTED"},
    {11127, "IO_FEEDBACK_FAILURE"},
    {11127, "IO_FEEDBACK_FAILURE"}
  };                                             
};




// class JointTrajPtFull{
//   public:
//     JointTrajPtFull(int, std::vector<float>, jointFeedbackMsg);
//     std::vector<uint8_t> to_bytes();
//     int length = 136;
//     int msg_type = 14;
//     int comm_type = 2;
//     int reply_code = 0;
//     int robot_id = 0;
//     int seq;
//     int valid_fields = 7;
//     float time;
//     std::vector<float> positions = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
//     std::vector<float> velocities = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
//     std::vector<float> accelerations = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
// };

// class MotoMotionCtrl{
//   public:
//     MotoMotionCtrl(std::string);
//     std::vector<uint8_t> to_bytes();
//   private:
//     std::map<std::string, int> command_types_ = { {"CHECK_MOTION_READY", 200101}, 
//                                                   {"CHECK_QUEUE_CNT", 200102}, 
//                                                   {"STOP_MOTION", 200111}, 
//                                                   {"START_TRAJ_MODE", 200121},
//                                                   {"STOP_TRAJ_MODE", 200122} };
//     int length = 64;
//     int msg_type = 2001;
//     int comm_type = 2;
//     int reply_code = 0;
//     int robot_id = 0;
//     int seq = 0;
//     int command;
//     std::vector<float> data = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
// };

// class WriteIOBit{
//   public:
//     WriteIOBit(int, std::string);
//     std::vector<uint8_t> to_bytes();
//   private:
//     int length = 25;
//     int msg_type = 2005;
//     int comm_type = 2;
//     int reply_code = 0;
//     int address = 0;
//     int value;
//     std::map<std::string, int> values_str_to_int_ = { {"on", 1}, 
//                                                       {"off", 0} };
// };

// class ReadIOBit{
//   public:
//     ReadIOBit(int);
//     std::vector<uint8_t> to_bytes();
//   private:
//     int length = 16;
//     int msg_type = 2003;
//     int comm_type = 2;
//     int reply_code = 0;
//     int address;
// };

// template <typename T,
//           typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
// std::vector<uint8_t> splitValueToBytes(T const& value)
// {
//     std::vector<uint8_t> bytes;

//     for (size_t i = 0; i < sizeof(value); i++)
//     {
//         uint8_t byte = value >> (i * 8);
//         bytes.insert(bytes.begin(), byte);
//     }

//     return bytes;
// }

// // template <typename T>
// void to_byte_and_insert(std::vector<uint8_t>& byte_array, u_int32_t data){
//   // std::vector<uint8_t> temp;
//   // for (size_t i = 0; i < sizeof(data); i++)
//   //   {
//   //     uint8_t byte = data >> (i * 8);
//   //     temp.insert(bytes.begin(), byte);
//   //   }

//   byte_array.insert(byte_array.end(), reinterpret_cast<uint8_t*>(&data), reinterpret_cast<uint8_t*>(&data) + 4);
// }

// union FloatUnion {
//   float f;
//   uint32_t i;
// };

// uint32_t float_to_ieee754(float value) {
//   FloatUnion fu;
//   fu.f = value;
//   return htonl(fu.i);
// }

// float bin_to_float(char* data)
// {
//   float result; 

//   char reversed[4] = {data[3], data[2], data[1], data[0]};

//   std::memcpy(&result, reversed, sizeof(float));

//   return result;
// }

// std::vector<double> slicing(std::vector<double>& arr,
//                     int X, int Y)
// {
 
//     // Starting and Ending iterators
//     auto start = arr.begin() + X;
//     auto end = arr.begin() + Y + 1;
 
//     // To store the sliced vector
//     std::vector<double> result(Y - X + 1);
 
//     // Copy vector using copy function()
//     copy(start, end, result.begin());
 
//     // Return the final sliced vector
//     return result;
// }