#ifndef APRS_DRIVER__MOTOMAN_HARDWARE_INTERFACE_
#define APRS_DRIVER__MOTOMAN_HARDWARE_INTERFACE_

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

namespace motoman_hardware {

  class MotomanHardwareInterface : public hardware_interface::SystemInterface {

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

    ~MotomanHardwareInterface() {on_deactivate(rclcpp_lifecycle::State());};

  private:
    rclcpp::Logger get_logger();
    void update_from_robot_controller();

    std::vector<double> hw_commands_;
    std::vector<double> hw_positions_;
    std::vector<double> hw_velocities_;
    std::vector<double> hw_accelerations_;

    const char *robot_ip_ = "192.168.1.33";
    const int state_port_ = 50241;
    const int num_urdf_joints_ = 9;
    const int num_robot_joints_ = 7;
  
    struct sockaddr_in state_socket_address_;
    int state_socket_ = 0;

    bool activated_ = false;

    simple_message::Status current_status_;
    simple_message::JointFeedback current_joint_feedback_;
  };

}

#endif  // APRS_DRIVER__MOTOMAN_HARDWARE_INTERFACE_