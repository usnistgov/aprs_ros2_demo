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

#include "simple_msgs_interfaces/simple_msgs_interface.hpp"

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

    bool is_gripper_opened();

    int get_packet_length(int);

    statusMsg read_status_msg(char *);
    jointFeedbackMsg read_joint_feedback_msg(char *);

    MotoMotionReply send_motion_msg(std::vector<uint8_t>);

    statusMsg current_status_;
    jointFeedbackMsg current_joint_feedback_;

    int num_urdf_joints_ = 9;
    int num_robot_joints_ = 7;

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

    int sequence = 0;

    bool activated = false;
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