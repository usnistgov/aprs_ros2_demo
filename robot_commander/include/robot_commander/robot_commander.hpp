#include <rclcpp/rclcpp.hpp>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <aprs_interfaces/srv/pick.hpp>
#include <aprs_interfaces/srv/place.hpp>
#include <aprs_interfaces/srv/move_to_named_pose.hpp>

class RobotCommander : public rclcpp::Node
{
  public:
    RobotCommander(std::string node_name, std::string group_name);

  private:
    // MoveIt

    moveit::planning_interface::MoveGroupInterface planning_interface_;
    moveit::planning_interface::PlanningSceneInterface planning_scene_;

    // Services
    rclcpp::Service<aprs_interfaces::srv::Pick>::SharedPtr pick_srv_;
    rclcpp::Service<aprs_interfaces::srv::Place>::SharedPtr place_srv_;
    rclcpp::Service<aprs_interfaces::srv::MoveToNamedPose>::SharedPtr move_to_named_pose_srv_;

    // Service Callbacks
    void pick_from_slot_cb(
      const std::shared_ptr<aprs_interfaces::srv::Pick::Request> request,
      std::shared_ptr<aprs_interfaces::srv::Pick::Response> response
    );

    void place_in_slot_cb(
      const std::shared_ptr<aprs_interfaces::srv::Place::Request> request,
      std::shared_ptr<aprs_interfaces::srv::Place::Response> response
    );

    void move_to_named_pose_cb(
      const std::shared_ptr<aprs_interfaces::srv::MoveToNamedPose::Request> request,
      std::shared_ptr<aprs_interfaces::srv::MoveToNamedPose::Response> response
    );

};