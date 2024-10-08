#ifndef MOTOMAN_HARDWARE__MOTOMAN_HARDWARE_INTERFACE_
#define MOTOMAN_HARDWARE__MOTOMAN_HARDWARE_INTERFACE_

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/state.hpp>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <hardware_interface/visibility_control.h>
#include <hardware_interface/hardware_info.hpp>
#include <hardware_interface/system_interface.hpp>

#include <cmath>

struct statusMsg{
  int msg_type;
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

struct jointFeedbackMsg{
  int msg_type;
  int comm_type;
  int reply_code;
  int robot_id;
  int valid_fields;
  float time;
  std::vector<float> positions;
  std::vector<float> velocities;
  std::vector<float> accelerations;
};

class MotoMotionReply{
  public:
    MotoMotionReply(char*);
    void output_data();
    int msg_type;
    int comm_type;
    int reply_code;
    int robot_id;
    int sequence;
    int command;
    int result;
    int subcode;
    std::vector<float> data;
  private:
    std::map<int, std::string> command_codes_to_str_ = {{-1, "MESSAG_REPLY_INCORRECT"},
                                                        {14, "CHECK_MOTION_READY"},
                                                        {200101, "CHECK_MOTION_READY"}, 
                                                        {200102, "CHECK_QUEUE_CNT"}, 
                                                        {200111, "STOP_MOTION"}, 
                                                        {200121, "START_TRAJ_MODE"},
                                                        {200122, "STOP_TRAJ_MODE"}};

    std::map<int, std::string> result_codes_to_str_ = {{-1, "NON_CONTROLLER_ERROR"},
                                                       {0, "SUCCESS/TRUE"},
                                                       {1, "BUSY"}, 
                                                       {2, "FAILURE/FALSE"}, 
                                                       {3, "INVALID"}, 
                                                       {4, "ALARM"},
                                                       {5, "NOT_READY"},
                                                       {6, "MP_FAILURE"}};

    std::map<int, std::string> subcode_codes_to_str_ = {{-1, "NON_CONTROLLER_ERROR"},
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
                                                        {11127, "IO_FEEDBACK_FAILURE"}};
                                                       
};

class WriteIOReply{
  public:
    WriteIOReply(char*);
    void output_data();
    int msg_type;
    int comm_type;
    int reply_code;
    int result;
};

class ReadIOReply{
  public:
    ReadIOReply(char*);
    void output_data();
    int msg_type;
    int comm_type;
    int reply_code;
    int value;
    int result;
};

namespace motoman_hardware {

  class MotomanHardwareInterface : public hardware_interface::SystemInterface {
  enum message_lengths {STATUS = 40, JOINT_FEEDBACK = 144};
  
  public:
    CallbackReturn on_init(const hardware_interface::HardwareInfo& info) override;

    CallbackReturn on_configure(const rclcpp_lifecycle::State & previous_state) override;

    CallbackReturn on_activate(const rclcpp_lifecycle::State& previous_state) override;

    CallbackReturn on_deactivate(const rclcpp_lifecycle::State& previous_state) override;

    hardware_interface::return_type read(const rclcpp::Time& time,
                                         const rclcpp::Duration& period) override;

    hardware_interface::return_type write(const rclcpp::Time& time,
                                          const rclcpp::Duration& period) override;

    std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
    std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

    ~MotomanHardwareInterface();

  private:
    rclcpp::Logger get_logger();

    bool setup_sockets();
    bool connect_sockets();
    void close_sockets();

    void read_joints();
    bool write_joints();

    bool open_gripper();
    bool close_gripper();

    int get_packet_length(int);

    statusMsg read_status_msg(char *);
    jointFeedbackMsg read_joint_feedback_msg(char *);

    MotoMotionReply send_motion_msg(std::vector<uint8_t>);

    statusMsg current_status_;
    jointFeedbackMsg current_joint_feedback_;

    int number_of_joints_ = 7;

    std::vector<double> hw_commands_;
    std::vector<double> prev_hw_commands_;

    std::vector<double> hw_positions_;
    std::vector<double> hw_velocities_;
    std::vector<double> hw_accelerations_;

    const char *robot_ip_ = "192.168.1.33";
    const int state_port_ = 50241;
    const int motion_port_ = 50240;
    const int io_port_ = 50242;

    struct sockaddr_in state_socket_address_;
    struct sockaddr_in motion_socket_address_;
    struct sockaddr_in io_socket_address_;

    int state_socket_ = 0;
    int motion_socket_ = 0;
    int io_socket_ = 0;

    bool activated = false;
    bool motion_requested = false;
  };

}

class JointTrajPtFull{
  public:
    JointTrajPtFull(int, std::vector<float>, jointFeedbackMsg);
    MotoMotionReply send_msg_and_get_feedback(int);
  private:
    std::vector<uint8_t> to_bytes();

    int length = 136;
    int msg_type = 14;
    int comm_type = 2;
    int reply_code = 0;
    int robot_id = 0;
    int seq;
    int valid_fields = 7;
    float time;
    std::vector<float> positions;
    std::vector<float> velocities = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    std::vector<float> accelerations = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
};

class MotoMotionCtrl{
  public:
    MotoMotionCtrl(std::string);
    std::vector<uint8_t> to_bytes();
  private:
    std::map<std::string, int> command_types_ = { {"CHECK_MOTION_READY", 200101}, 
                                                  {"CHECK_QUEUE_CNT", 200102}, 
                                                  {"STOP_MOTION", 200111}, 
                                                  {"START_TRAJ_MODE", 200121},
                                                  {"STOP_TRAJ_MODE", 200122} };
    int length = 64;
    int msg_type = 2001;
    int comm_type = 2;
    int reply_code = 0;
    int robot_id = 0;
    int seq = 0;
    int command;
    std::vector<float> data = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
};

class WriteIOBit{
  public:
    WriteIOBit(int, std::string);
    std::vector<uint8_t> to_bytes();
  private:
    int length = 25;
    int msg_type = 2005;
    int comm_type = 2;
    int reply_code = 0;
    int address = 0;
    int value;
    std::map<std::string, int> values_str_to_int_ = { {"on", 1}, 
                                                      {"off", 0} };
};

class ReadIOBit{
  public:
    ReadIOBit(int);
    std::vector<uint8_t> to_bytes();
  private:
    int length = 16;
    int msg_type = 2003;
    int comm_type = 2;
    int reply_code = 0;
    int address;
};

template <typename T,
          typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
std::vector<uint8_t> splitValueToBytes(T const& value)
{
    std::vector<uint8_t> bytes;

    for (size_t i = 0; i < sizeof(value); i++)
    {
        uint8_t byte = value >> (i * 8);
        bytes.insert(bytes.begin(), byte);
    }

    return bytes;
}

template <typename T>
void to_byte_and_insert(std::vector<uint8_t>& byte_array, T value){
  std::vector<uint8_t> temp = splitValueToBytes(value);
  byte_array.insert(byte_array.end(), temp.begin(), temp.end());
}

union FloatUnion {
  float f;
  uint32_t i;
};

uint32_t float_to_ieee754(float value) {
  FloatUnion fu;
  fu.f = value;
  return htonl(fu.i);
}

float bin_to_float(char* data)
{
  float result; 

  char reversed[4] = {data[3], data[2], data[1], data[0]};

  std::memcpy(&result, reversed, sizeof(float));

  return result;
}

#include <unistd.h>

namespace socket_read {
  ssize_t read_socket(int __fd, void *__buf, size_t __nbytes);
}
namespace socket_write {
  ssize_t write_socket(int __fd, void *__buf, size_t __nbytes);
}

int get_reply_packet_length(int socket)
{
  char *length_packet = new char[4];

  ssize_t ret = socket_read::read_socket(socket, length_packet, 4);

  if (ret < 0){
    return -1;
  }

  int length = ntohl(*(uint32_t*)length_packet);

  delete[] length_packet;

  return length;
}

#endif  // MOTOMAN_HARDWARE__MOTOMAN_HARDWARE_INTERFACE_