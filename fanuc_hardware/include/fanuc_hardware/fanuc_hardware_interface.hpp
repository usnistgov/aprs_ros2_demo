#ifndef FANUC_HARDWARE__FANUC_HARDWARE_INTERFACE_
#define FANUC_HARDWARE__FANUC_HARDWARE_INTERFACE_

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/state.hpp>

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

    int number_of_joints_ = 6;

    std::vector<double> hw_commands_;
    std::vector<double> hw_states_;
  };

}

#endif  // FANUC_HARDWARE__FANUC_HARDWARE_INTERFACE_