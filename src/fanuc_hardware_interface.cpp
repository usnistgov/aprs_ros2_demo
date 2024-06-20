#include "fanuc_hardware/fanuc_hardware_interface.hpp"

#include <hardware_interface/system_interface.hpp>

namespace fanuc_hardware {

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;
  
  FanucHardwareInterface::FanucHardwareInterface()
  {

  }

  CallbackReturn FanucHardwareInterface::on_init(const hardware_interface::HardwareInfo& info)
  {
    (void)info;

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn FanucHardwareInterface::on_activate(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn FanucHardwareInterface::on_deactivate(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;
    return CallbackReturn::SUCCESS;
  }

  hardware_interface::return_type FanucHardwareInterface::read(const rclcpp::Time& time, const rclcpp::Duration& period)
  {
    (void)time;
    (void)period;
    return hardware_interface::return_type::OK;
  }

  hardware_interface::return_type FanucHardwareInterface::write(const rclcpp::Time& time, const rclcpp::Duration& period)
  {
    (void)time;
    (void)period;
    return hardware_interface::return_type::OK;
  }

  std::vector<hardware_interface::StateInterface> FanucHardwareInterface::export_state_interfaces()
  {
    std::vector<hardware_interface::StateInterface> state_interfaces;

    return state_interfaces;
  }

  std::vector<hardware_interface::CommandInterface> export_command_interfaces() 
  {
    std::vector<hardware_interface::CommandInterface> command_interfaces;

    return command_interfaces;
  }

}