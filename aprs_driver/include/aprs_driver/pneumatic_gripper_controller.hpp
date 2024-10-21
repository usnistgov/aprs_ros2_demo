#ifndef APRS_DRIVER__PNEUMATIC_GRIPPER_CONTROLLER_
#define APRS_DRIVER__PNEUMATIC_GRIPPER_CONTROLLER_

#include <rclcpp/rclcpp.hpp>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <controller_interface/controller_interface.hpp>
#include <aprs_interfaces/srv/pneumatic_gripper_control.hpp>
#include <aprs_driver/network_utilities.hpp>
#include <aprs_driver/simple_messages.hpp>

#include <exception>
#include <string>

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

namespace pneumatic_controller {
  class PneumaticGripperController : public controller_interface::ControllerInterface {
  public:
    controller_interface::InterfaceConfiguration command_interface_configuration() const override;
    controller_interface::InterfaceConfiguration state_interface_configuration() const override;
    controller_interface::return_type update(const rclcpp::Time& time,
                                            const rclcpp::Duration& period) override;
    CallbackReturn on_init() override;
    CallbackReturn on_configure(const rclcpp_lifecycle::State& previous_state) override;
    CallbackReturn on_activate(const rclcpp_lifecycle::State& previous_state) override;
    CallbackReturn on_deactivate(const rclcpp_lifecycle::State& previous_state) override;

    ~PneumaticGripperController();
  private:
    bool actuate_gripper(bool enable);
    bool control_air(bool enable);
    void actuate_gripper_srv_cb_(const std::shared_ptr<aprs_interfaces::srv::PneumaticGripperControl::Request> request,
                                    std::shared_ptr<aprs_interfaces::srv::PneumaticGripperControl::Response> response);

    rclcpp::Service<aprs_interfaces::srv::PneumaticGripperControl>::SharedPtr change_gripper_state_;
  
    std::vector<std::string> joint_names_;
    double gripper_stroke_ = 0.0075;
    double gripper_state_ = 0;

    // Socket communication
    const char *robot_ip_ = "192.168.1.34";
    const int gripper_port_ = 12123;

    std::map<std::string, char*> robot_ips_ = {{"motoman", "192.168.1.33"}, {"fanuc", "192.168.1.34"}};
    std::map<std::string, int> robot_ports_ = {{"motoman", 50242}, {"fanuc", 12123}};
    std::map<std::string, double> gripper_strokes_ = {{"motoman", 0.02}, {"fanuc", 0.0075}};

    std::string robot_ = "fanuc";
  };
}  // namespace aprs_controllers

#endif  // APRS_DRIVER__PNEUMATIC_GRIPPER_CONTROLLER_