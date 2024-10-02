#ifndef MOTOMAN_HARDWARE__MOTOMAN_HARDWARE_INTERFACE_
#define MOTOMAN_HARDWARE__MOTOMAN_HARDWARE_INTERFACE_

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/state.hpp>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <hardware_interface/visibility_control.h>
#include <hardware_interface/hardware_info.hpp>
#include <hardware_interface/system_interface.hpp>

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
    void write_joints();

    int get_packet_length();

    float bin_to_float(char*);

    statusMsg read_status_msg(char *);
    jointFeedbackMsg read_joint_feedback_msg(char *);

    statusMsg current_status_;
    jointFeedbackMsg current_joint_feedback_;

    int number_of_joints_ = 7;

    std::vector<double> hw_commands_;
    std::vector<double> hw_states_;

    std::vector<double> joint_positions_;

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

#include <unistd.h>

namespace socket_read {
  ssize_t read_socket(int __fd, void *__buf, size_t __nbytes);
}
namespace socket_write {
  ssize_t write_socket(int __fd, void *__buf, size_t __nbytes);
}

#endif  // MOTOMAN_HARDWARE__MOTOMAN_HARDWARE_INTERFACE_