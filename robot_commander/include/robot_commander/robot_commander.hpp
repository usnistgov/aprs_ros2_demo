#include <rclcpp/rclcpp.hpp>

class RobotCommander : public rclcpp::Node
{
  public:
    RobotCommander(std::string node_name);
};