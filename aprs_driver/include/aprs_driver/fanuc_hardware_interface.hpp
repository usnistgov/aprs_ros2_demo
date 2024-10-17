#ifndef APRS_DRIVER__FANUC_HARDWARE_INTERFACE_
#define APRS_DRIVER__FANUC_HARDWARE_INTERFACE_

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/state.hpp>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <hardware_interface/visibility_control.h>
#include <hardware_interface/hardware_info.hpp>
#include <hardware_interface/system_interface.hpp>
#include "hardware_interface/types/hardware_interface_type_values.hpp"

#include <aprs_driver/simple_messages.hpp>
#include <aprs_driver/network_utilities.hpp>

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
    void update_from_socket();

    std::vector<double> hw_commands_;
    std::vector<double> hw_positions_;

    const char *robot_ip_ = "192.168.1.34";
    const int state_port_ = 11002;
    const int num_urdf_joints_ = 8;
    const int num_robot_joints_ = 6;
  
    struct sockaddr_in state_socket_address_;
    int state_socket_ = 0;

    bool activated_ = false;

    simple_message::Status current_status_;
    simple_message::JointPosition current_joint_position_;
  };

}

#endif  // APRS_DRIVER__FANUC_HARDWARE_INTERFACE_