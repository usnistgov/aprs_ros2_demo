#include <math.h>

#include <rclcpp/rclcpp.hpp>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit/trajectory_processing/time_optimal_trajectory_generation.h>
#include <moveit/robot_trajectory/robot_trajectory.h>

#include "tf2/exceptions.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"

#include <std_msgs/msg/float64_multi_array.hpp>
#include <example_interfaces/srv/trigger.hpp>
#include <aprs_interfaces/srv/pick.hpp>
#include <aprs_interfaces/srv/place.hpp>
#include <aprs_interfaces/srv/move_to_named_pose.hpp>
#include <aprs_interfaces/srv/pneumatic_gripper_control.hpp>

class RobotCommander : public rclcpp::Node
{
  public:
    RobotCommander(std::string node_name, moveit::planning_interface::MoveGroupInterface::Options opt);

    bool actuate_gripper(bool enable);
    std::pair<bool, std::string> move_to_named_pose(const std::string &pose_name);
    std::pair<bool, std::string> pick_part(const std::string &slot_name);
    std::pair<bool, std::string> place_part(const std::string &slot_name);

  private:
    // Robot Move Functions
    std::pair<bool, moveit_msgs::msg::RobotTrajectory> plan_to_target();
    std::pair<bool, moveit_msgs::msg::RobotTrajectory> plan_cartesian(geometry_msgs::msg::Pose pose);
    
    void send_trajectory(moveit_msgs::msg::RobotTrajectory trajectory);

    // Utility Functions
    void handle_j23_transform(moveit_msgs::msg::RobotTrajectory &trajectory);
    geometry_msgs::msg::Pose build_robot_pose(double x, double y, double z, double rotation);
    double largest_error(std::vector<double> v1, std::vector<double> v2);

    // MoveIt
    moveit::planning_interface::MoveGroupInterface planning_interface_;
    moveit::planning_interface::PlanningSceneInterface planning_scene_;
    trajectory_processing::TimeOptimalTrajectoryGeneration totg_;

    // TF
    std::unique_ptr<tf2_ros::Buffer> tf_buffer = std::make_unique<tf2_ros::Buffer>(get_clock());
    std::shared_ptr<tf2_ros::TransformListener> tf_listener = std::make_shared<tf2_ros::TransformListener>(*tf_buffer);
    
    // Publishers
    rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr joint_command_publisher_;

    // Clients
    rclcpp::Client<aprs_interfaces::srv::PneumaticGripperControl>::SharedPtr gripper_client_;


    // Services
    rclcpp::Service<aprs_interfaces::srv::Pick>::SharedPtr pick_srv_;
    rclcpp::Service<aprs_interfaces::srv::Place>::SharedPtr place_srv_;
    rclcpp::Service<aprs_interfaces::srv::MoveToNamedPose>::SharedPtr move_to_named_pose_srv_;

    // Callback groups
    rclcpp::CallbackGroup::SharedPtr client_cb_group_;

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

    // Response callbacks
    // void gripper_response_cb(rclcpp::Client<aprs_interfaces::srv::PneumaticGripperControl>::SharedFuture future);

    double vsf = 0.1;
    double asf = 0.5;
    double trajectory_spacing_ = 100000; // time between sending trajectory points in microseconds
    double pick_offset = 0.1;
    double place_offset = 0.1;
    double gripper_roll = 0;
    double gripper_pitch = M_PI_2;

    double goal_joint_tolerance = 0.01;
    
    std::string base_link = "fanuc_base_link";
    std::string group_name;

    bool holding_part = false;
};