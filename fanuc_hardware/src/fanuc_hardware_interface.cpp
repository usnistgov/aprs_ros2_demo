#include "fanuc_hardware/fanuc_hardware_interface.hpp"

#include "hardware_interface/types/hardware_interface_type_values.hpp"

namespace fanuc_hardware {

  hardware_interface::CallbackReturn FanucHardwareInterface::on_init(const hardware_interface::HardwareInfo& info)
  {
  
    if (hardware_interface::SystemInterface::on_init(info) != CallbackReturn::SUCCESS) {
      return hardware_interface::CallbackReturn::ERROR;
    }

    if (int(info_.joints.size()) != number_of_joints_) {
      RCLCPP_FATAL(get_logger(), "Got %d joints. Expected %d.", int(info_.joints.size()), number_of_joints_);
      return hardware_interface::CallbackReturn::ERROR;
    }

    hw_states_.resize(info_.joints.size(), std::numeric_limits<double>::quiet_NaN());
    hw_commands_.resize(info_.joints.size(), std::numeric_limits<double>::quiet_NaN());

    // TODO: Connect to socket 

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::CallbackReturn FanucHardwareInterface::on_configure(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;

    for (uint i = 0; i < hw_states_.size(); i++)
    {
      hw_states_[i] = 0;
      hw_commands_[i] = 0;
    }

    RCLCPP_INFO(rclcpp::get_logger("RRBotSystemPositionOnlyHardware"), "Successfully configured!");

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::CallbackReturn FanucHardwareInterface::on_activate(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;

    for (uint i = 0; i < hw_states_.size(); i++)
    {
      hw_commands_[i] = hw_states_[i];
    }

    RCLCPP_INFO(get_logger(), "Successfully activated!");

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::CallbackReturn FanucHardwareInterface::on_deactivate(const rclcpp_lifecycle::State& previous_state)
  {
    (void)previous_state;

    RCLCPP_INFO(get_logger(), "Successfully deactivated!");

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::return_type FanucHardwareInterface::read(const rclcpp::Time& time, const rclcpp::Duration& period)
  {
    (void)time;
    (void)period;

    // TODO: Read data from state socket

    return hardware_interface::return_type::OK;
  }

  hardware_interface::return_type FanucHardwareInterface::write(const rclcpp::Time& time, const rclcpp::Duration& period)
  {
    (void)time;
    (void)period;

    // TODO: Write joint traj to move socket

    return hardware_interface::return_type::OK;
  }

  std::vector<hardware_interface::StateInterface> FanucHardwareInterface::export_state_interfaces()
  {
    std::vector<hardware_interface::StateInterface> state_interfaces;
    
    for (uint i = 0; i < info_.joints.size(); i++)
    {
      state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &hw_states_[i]));
    }

    return state_interfaces;
  }

  std::vector<hardware_interface::CommandInterface> FanucHardwareInterface::export_command_interfaces() 
  {
    std::vector<hardware_interface::CommandInterface> command_interfaces;
    
    for (uint i = 0; i < info_.joints.size(); i++)
    {
      command_interfaces.emplace_back(hardware_interface::CommandInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &hw_commands_[i]));
    }

    return command_interfaces;
  }

  rclcpp::Logger FanucHardwareInterface::get_logger() {
    return rclcpp::get_logger("FanucHardwareInterface");
  }

}

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(fanuc_hardware::FanucHardwareInterface, hardware_interface::SystemInterface)