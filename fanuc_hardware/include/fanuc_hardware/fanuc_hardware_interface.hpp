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
    ~FanucHardwareInterface();

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

    std::pair<bool, std::vector<float>> read_joints();

    int number_of_joints_ = 6;

    std::vector<double> hw_commands_;
    std::vector<double> hw_states_;

    const char *robot_ip_ = "192.168.1.34";
    const int state_port_ = 11002;
    const int state_buffer_length_ = 56;

    struct sockaddr_in state_socket_;

    int sock_ = 0;
    bool socket_created_{false};
  };

}

#include <unistd.h>

namespace socket_read {
  ssize_t read_socket(int __fd, void *__buf, size_t __nbytes);
}

#endif  // FANUC_HARDWARE__FANUC_HARDWARE_INTERFACE_