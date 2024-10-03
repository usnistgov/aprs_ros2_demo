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
    int msg_type;
    int comm_type;
    int reply_code;
    int robot_id;
    int sequence;
    int command;
    int result;
    int subcode;
    std::vector<float> data;
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

  private:
    rclcpp::Logger get_logger();

    bool setup_sockets();
    bool connect_sockets();
    void close_sockets();

    void read_joints();
    bool write_joints();

    int get_packet_length(int);

    statusMsg read_status_msg(char *);
    jointFeedbackMsg read_joint_feedback_msg(char *);

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