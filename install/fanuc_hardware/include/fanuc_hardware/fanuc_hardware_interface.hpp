#ifndef FANUC_HARDWARE__FANUC_HARDWARE_INTERFACE_
#define FANUC_HARDWARE__FANUC_HARDWARE_INTERFACE_

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/state.hpp>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <hardware_interface/visibility_control.h>
#include <hardware_interface/hardware_info.hpp>
#include <hardware_interface/system_interface.hpp>


namespace fanuc_hardware {

  class FanucHardwareInterface : public hardware_interface::SystemInterface {

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

    bool flag = false;

    std::pair<bool, std::vector<float>> read_joints();
    std::pair<bool, const std::vector<uint8_t>> write_joints();

    int get_packet_length();

    int bin_to_int(char* data);
    
    float bin_to_float(char* data);

    uint32_t float_to_ieee754(float value);

    int number_of_joints_ = 6;

    union FloatUnion {
      float f;
      uint32_t i;
    };

    std::vector<double> hw_commands_;
    std::vector<double> hw_states_;
    std::vector<double> prev_hw_states_;
    std::vector<double> prev_hw_commands_;


    const char *robot_ip_ = "192.168.1.34";
    const int state_port_ = 11002;
    const int motion_port_ = 11000;
    const int state_buffer_length_ = 56;
    const int motion_buffer_length_ = 64;

    struct sockaddr_in state_socket_;
    struct sockaddr_in motion_socket_;

    int state_sock_ = 0;
    int motion_sock_ = 0;
    bool state_socket_created_{false};
    bool motion_socket_created_{false};
  };

}

#include <unistd.h>

namespace socket_read {
  ssize_t read_socket(int __fd, void *__buf, size_t __nbytes);
}
namespace socket_write {
  ssize_t write_socket(int __fd, void *__buf, size_t __nbytes);
}

#endif  // FANUC_HARDWARE__FANUC_HARDWARE_INTERFACE_