#include <math.h>

#include <rclcpp/rclcpp.hpp>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit/trajectory_processing/time_optimal_trajectory_generation.h>
#include <moveit/robot_trajectory/robot_trajectory.h>

#include "tf2/exceptions.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"

#include <geometric_shapes/shapes.h>
#include <geometric_shapes/shape_operations.h>
#include <shape_msgs/msg/mesh.h>

#include <ament_index_cpp/get_package_share_directory.hpp>

#include <aprs_interfaces/msg/trays.hpp>
#include <aprs_interfaces/msg/tray.hpp>

#include <example_interfaces/srv/trigger.hpp>
#include <aprs_interfaces/srv/pick.hpp>
#include <aprs_interfaces/srv/place.hpp>
#include <aprs_interfaces/srv/move_to_named_pose.hpp>
#include <aprs_interfaces/srv/pneumatic_gripper_control.hpp>

class RobotCommander : public rclcpp::Node
{
  public:
    RobotCommander();

    bool actuate_gripper(bool enable);
    std::pair<bool, std::string> move_to_named_pose(const std::string &pose_name);
    std::pair<bool, std::string> pick_part(const std::string &slot_name);
    std::pair<bool, std::string> place_part(const std::string &slot_name);

  private:
    // Robot Move Functions
    std::pair<bool, moveit_msgs::msg::RobotTrajectory> plan_to_target();
    std::pair<bool, moveit_msgs::msg::RobotTrajectory> plan_cartesian(geometry_msgs::msg::Pose pose);
    void retime_trajectory(moveit_msgs::msg::RobotTrajectory& trajectory);

    // Utility Functions
    geometry_msgs::msg::Pose build_robot_pose(double x, double y, double z, double rotation);
    moveit_msgs::msg::CollisionObject create_collision_object(
      std::string name, 
      std::string parent_frame,
      std::string mesh_file,
      geometry_msgs::msg::Pose model_pose,
      int operation = moveit_msgs::msg::CollisionObject::ADD
    );
    std_msgs::msg::ColorRGBA get_object_color(int identifier);

    // MoveIt
    std::unique_ptr<moveit::planning_interface::MoveGroupInterface> planning_interface_;
    std::unique_ptr<moveit::planning_interface::PlanningSceneInterface> planning_scene_;
    trajectory_processing::TimeOptimalTrajectoryGeneration totg_;

    // TF
    std::unique_ptr<tf2_ros::Buffer> tf_buffer = std::make_unique<tf2_ros::Buffer>(get_clock());
    std::shared_ptr<tf2_ros::TransformListener> tf_listener = std::make_shared<tf2_ros::TransformListener>(*tf_buffer);

    // Clients
    rclcpp::Client<aprs_interfaces::srv::PneumaticGripperControl>::SharedPtr gripper_client_;

    // Subscriber
    rclcpp::Subscription<aprs_interfaces::msg::Trays>::SharedPtr trays_info_table_vision_sub_;
    rclcpp::Subscription<aprs_interfaces::msg::Trays>::SharedPtr trays_info_conveyor_vision_sub_;
    void table_trays_info_cb(const aprs_interfaces::msg::Trays::ConstSharedPtr msg);
    void conveyor_trays_info_cb(const aprs_interfaces::msg::Trays::ConstSharedPtr msg);

    // Services
    rclcpp::Service<aprs_interfaces::srv::Pick>::SharedPtr pick_srv_;
    rclcpp::Service<aprs_interfaces::srv::Place>::SharedPtr place_srv_;
    rclcpp::Service<aprs_interfaces::srv::MoveToNamedPose>::SharedPtr move_to_named_pose_srv_;
    rclcpp::Service<example_interfaces::srv::Trigger>::SharedPtr initialize_planning_scene_srv_;

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

    void initialize_planning_scene_cb(
      const std::shared_ptr<example_interfaces::srv::Trigger::Request> request,
      std::shared_ptr<example_interfaces::srv::Trigger::Response> response
    );

    // ROS Parameters
    std::string planning_group_name_;
    
    std::string robot_name_;
    std::string end_effector_link_;
    double vsf_;
    double asf_;
    double pick_offset_;
    double place_offset_;
    double above_slot_offset_;
    double gripper_roll_;
    double gripper_pitch_;

    // Flags
    bool received_table_tray_info = false;
    bool received_conveyor_tray_info = false;
    bool holding_part = false;

    // Variables
    aprs_interfaces::msg::Trays table_trays_info;
    aprs_interfaces::msg::Trays conveyor_trays_info;
    std::string attached_part_name;
    int attached_part_type;

    // Maps
    std::map<int, int> gear_counter = {
      {aprs_interfaces::msg::SlotInfo::SMALL, 0},
      {aprs_interfaces::msg::SlotInfo::MEDIUM, 0},
      {aprs_interfaces::msg::SlotInfo::LARGE, 0}
    };

    std::map<int, std::string> gear_names = {
      {aprs_interfaces::msg::SlotInfo::SMALL, "small_gear"},
      {aprs_interfaces::msg::SlotInfo::MEDIUM, "medium_gear"},
      {aprs_interfaces::msg::SlotInfo::LARGE, "large_gear"},
    };

    std::map<std::string, std::string> slot_objects;
    std::map<std::string, int> slot_types;

    std::map<int, std::string> tray_stl_names = {
      {aprs_interfaces::msg::Tray::S2L2_KIT_TRAY, "s2l2_kit_tray.stl"},
      {aprs_interfaces::msg::Tray::M2L1_KIT_TRAY, "m2l1_kit_tray.stl"},
      {aprs_interfaces::msg::Tray::SMALL_GEAR_TRAY, "small_gear_part_tray.stl"},
      {aprs_interfaces::msg::Tray::MEDIUM_GEAR_TRAY, "medium_gear_part_tray.stl"},
      {aprs_interfaces::msg::Tray::LARGE_GEAR_TRAY, "large_gear_part_tray.stl"}
    };

    std::map<int, std::string> gear_stl_names = {
      {aprs_interfaces::msg::SlotInfo::SMALL, "small_gear.stl"},
      {aprs_interfaces::msg::SlotInfo::MEDIUM, "medium_gear.stl"},
      {aprs_interfaces::msg::SlotInfo::LARGE, "large_gear.stl"},
    };

    std::map<int, std::string> object_colors = {
      {aprs_interfaces::msg::Tray::S2L2_KIT_TRAY, "red"},
      {aprs_interfaces::msg::Tray::M2L1_KIT_TRAY, "brown"},
      {aprs_interfaces::msg::Tray::SMALL_GEAR_TRAY, "pink"},
      {aprs_interfaces::msg::Tray::MEDIUM_GEAR_TRAY, "blue"},
      {aprs_interfaces::msg::Tray::LARGE_GEAR_TRAY, "purple"},
      {aprs_interfaces::msg::SlotInfo::SMALL, "yellow"},
      {aprs_interfaces::msg::SlotInfo::MEDIUM, "orange"},
      {aprs_interfaces::msg::SlotInfo::LARGE, "green"},
      {-1, "black"}
    };

    std::map<std::string, std::vector<double>> color_values = {
      {"red", std::vector<double>{.96, .26, .21, 1}},
      {"green", std::vector<double>{.3, .69, .31, 1}},
      {"blue", std::vector<double>{.13, .59, .95, 1}},
      {"orange", std::vector<double>{1, .6, 0, 1}},
      {"yellow", std::vector<double>{1, .76, .03, 1}},
      {"purple", std::vector<double>{.4, .23, .72, 1}},
      {"pink", std::vector<double>{.91, .12, .39, 1}},
      {"brown", std::vector<double>{.47, .33, .28, 1}},
      {"black", std::vector<double>{0, 0, 0, 0.8}}
    };
};