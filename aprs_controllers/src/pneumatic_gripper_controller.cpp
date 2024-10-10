/*
This software was developed by employees of the National Institute of Standards and Technology (NIST), an agency of the Federal Government. Pursuant to title 17 United States Code Section 105, works of NIST employees are not subject to copyright protection in the United States and are considered to be in the public domain. Permission to freely use, copy, modify, and distribute this software and its documentation without fee is hereby granted, provided that this notice and disclaimer of warranty appears in all copies.

The software is provided 'as is' without any warranty of any kind, either expressed, implied, or statutory, including, but not limited to, any warranty that the software will conform to specifications, any implied warranties of merchantability, fitness for a particular purpose, and freedom from infringement, and any warranty that the documentation will conform to the software, or any warranty that the software will be error free. In no event shall NIST be liable for any damages, including, but not limited to, direct, indirect, special or consequential damages, arising out of, resulting from, or in any way connected with this software, whether or not based upon warranty, contract, tort, or otherwise, whether or not injury was sustained by persons or property or otherwise, and whether or not loss was sustained from, or arose out of the results of, or use of, the software or services provided hereunder.

Distributions of NIST software should also include copyright and licensing statements of any third-party software that are legally bundled with the code in compliance with the conditions of those licenses.
*/

#include <aprs_controllers/pneumatic_gripper_controller.hpp>

namespace aprs_controllers {

controller_interface::InterfaceConfiguration
PneumaticGripperController::command_interface_configuration() const {
  controller_interface::InterfaceConfiguration config;
  config.type = controller_interface::interface_configuration_type::INDIVIDUAL;

  config.names.push_back(gripper_joint_name + "/position");

  return config;
}

controller_interface::InterfaceConfiguration
PneumaticGripperController::state_interface_configuration() const {
  controller_interface::InterfaceConfiguration config;
  config.type = controller_interface::interface_configuration_type::INDIVIDUAL;

  config.names.push_back(gripper_joint_name + "/position");

  return config;
}

controller_interface::return_type PneumaticGripperController::update(
  const rclcpp::Time& /*time*/,
  const rclcpp::Duration& /*period*/) 
{

  for (auto& cmd_interface : command_interfaces_) {
    cmd_interface.set_value(gripper_command);
  }

  return controller_interface::return_type::OK;
}

CallbackReturn PneumaticGripperController::on_init() {
  try
  {
    auto_declare<std::vector<std::string>>("joints", {});
    change_gripper_state_ = get_node()->create_service<aprs_interfaces::srv::PneumaticGripperControl>(
    "/change_gripper_state", 
    std::bind(&PneumaticGripperController::update_gripper_command, this, std::placeholders::_1, std::placeholders::_2),
    rclcpp::ServicesQoS()
    );
  }
  catch (const std::exception & e)
  {
    fprintf(stderr, "Exception thrown during init stage with message: %s \n", e.what());
    return CallbackReturn::ERROR;
  }

  return CallbackReturn::SUCCESS;
}

CallbackReturn PneumaticGripperController::on_configure(
  const rclcpp_lifecycle::State&) 
{
  gripper_joint_name = get_node()->get_parameter("joints").as_string_array().back();

  return CallbackReturn::SUCCESS;
}

CallbackReturn PneumaticGripperController::on_activate(
    const rclcpp_lifecycle::State&)
{
  gripper_command = 0;
  
  return CallbackReturn::SUCCESS;
}

void PneumaticGripperController::update_gripper_command(
  const std::shared_ptr<aprs_interfaces::srv::PneumaticGripperControl::Request> request,
  std::shared_ptr<aprs_interfaces::srv::PneumaticGripperControl::Response> response)
{

  if (request->enable == false && state_interfaces_.at(0).get_value() != 0){
    gripper_command = gripper_stroke_;
    response->status = "Opening gripper";
    response->success = true;
  }
  else if (request->enable == true && state_interfaces_.at(0).get_value() != gripper_stroke_){
    gripper_command = 0;
    response->status = "Closing gripper";
    response->success = true;
  }
  else{
    response->status = "Gripper is already in requested state";
    response->success = false;
  }
}

}  // namespace aprs_controllers
#include "pluginlib/class_list_macros.hpp"
// NOLINTNEXTLINE
PLUGINLIB_EXPORT_CLASS(aprs_controllers::PneumaticGripperController,
                       controller_interface::ControllerInterface)