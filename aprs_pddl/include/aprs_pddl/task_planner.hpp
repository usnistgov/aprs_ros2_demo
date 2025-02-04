#include <rclcpp/qos.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>

#include <plansys2_pddl_parser/Utils.h>

#include "plansys2_msgs/msg/action_execution_info.hpp"
#include "plansys2_msgs/msg/plan.hpp"

#include "plansys2_domain_expert/DomainExpertClient.hpp"
#include "plansys2_executor/ExecutorClient.hpp"
#include "plansys2_planner/PlannerClient.hpp"
#include "plansys2_problem_expert/ProblemExpertClient.hpp"

#include <ament_index_cpp/get_package_share_directory.hpp>

#include "aprs_interfaces/srv/generate_plan.hpp"
#include "aprs_interfaces/srv/clear_current_state.hpp"
#include "aprs_interfaces/srv/move_to_named_pose.hpp"
#include "aprs_interfaces/action/execute_plan.hpp"
#include "aprs_interfaces/msg/trays.hpp"
#include "aprs_interfaces/msg/slot_info.hpp"
#include "aprs_interfaces/msg/tray.hpp"
#include "aprs_interfaces/msg/robot_changeover.hpp"

class TaskPlanner : public rclcpp::Node
{
public:
  /// Constructor
  TaskPlanner();

  ~TaskPlanner();

  void init_world_state();
  void init_goal_state();
  void update_world_state(std::string robot);
  void process_trays(const std::vector<aprs_interfaces::msg::Tray>& trays, const std::string& location);

 private:

  // Plansys2
  std::shared_ptr<plansys2::DomainExpertClient> domain_expert_;
  std::shared_ptr<plansys2::PlannerClient> planner_client_;
  std::shared_ptr<plansys2::ProblemExpertClient> problem_expert_;
  std::shared_ptr<plansys2::ExecutorClient> executor_client_; 

  rclcpp::Service<aprs_interfaces::srv::ClearCurrentState>::SharedPtr clear_current_state_server_;
  rclcpp::Service<aprs_interfaces::srv::GeneratePlan>::SharedPtr generate_plan_server_;
  rclcpp_action::Server<aprs_interfaces::action::ExecutePlan>::SharedPtr execute_plan_action_;

  rclcpp::Subscription<aprs_interfaces::msg::Trays>::SharedPtr fanuc_table_subscriber_;
  rclcpp::Subscription<aprs_interfaces::msg::Trays>::SharedPtr fanuc_conveyor_subscriber_;
  rclcpp::Subscription<aprs_interfaces::msg::Trays>::SharedPtr motoman_table_subscriber_;
  rclcpp::Subscription<aprs_interfaces::msg::Trays>::SharedPtr motoman_conveyor_subscriber_;
  rclcpp::Subscription<aprs_interfaces::msg::Trays>::SharedPtr teach_table_subscriber_;
  rclcpp::Subscription<aprs_interfaces::msg::RobotChangeover>::SharedPtr robot_changeover_subscriber_;

  rclcpp::Client<aprs_interfaces::srv::MoveToNamedPose>::SharedPtr fanuc_move_to_named_pose_client_;
  rclcpp::Client<aprs_interfaces::srv::MoveToNamedPose>::SharedPtr motoman_move_to_named_pose_client_;

  rclcpp::CallbackGroup::SharedPtr cb_group_;
  rclcpp::CallbackGroup::SharedPtr topic_cb_group_;
  
  // APRS Demo Objects
  std::vector<aprs_interfaces::msg::Tray> fanuc_table_kit_trays_;
  std::vector<aprs_interfaces::msg::Tray> fanuc_table_part_trays_;
  std::vector<aprs_interfaces::msg::Tray> fanuc_conveyor_kit_trays_;
  std::vector<aprs_interfaces::msg::Tray> fanuc_conveyor_part_trays_;

  std::vector<aprs_interfaces::msg::Tray> motoman_table_kit_trays_;
  std::vector<aprs_interfaces::msg::Tray> motoman_table_part_trays_;
  std::vector<aprs_interfaces::msg::Tray> motoman_conveyor_kit_trays_;
  std::vector<aprs_interfaces::msg::Tray> motoman_conveyor_part_trays_;

  std::vector<aprs_interfaces::msg::Tray> teach_table_kit_trays_;
  std::vector<aprs_interfaces::msg::Tray> teach_table_part_trays_;

  // Flags
  bool recieved_fanuc_table_info = false;
  bool recieved_fanuc_conveyor_info = false;
  bool recieved_motoman_table_info = false;
  bool recieved_motoman_conveyor_info = false;
  bool recieved_teach_table_info = false;

  // Robot Status
  bool fanuc_changeover=false;
  bool motoman_changeover=false;

  // PDDL
  std::string goal_str_;

  // Callbacks
  void FanucTableTraysInfoCallback(const aprs_interfaces::msg::Trays::SharedPtr msg);
  void FanucConveyorTraysInfoCallback(const aprs_interfaces::msg::Trays::SharedPtr msg);
  void MotomanTableTraysInfoCallback(const aprs_interfaces::msg::Trays::SharedPtr msg);
  void MotomanConveyorTraysInfoCallback(const aprs_interfaces::msg::Trays::SharedPtr msg);
  void TeachTableTraysInfoCallback(const aprs_interfaces::msg::Trays::SharedPtr msg);
  void RobotChangeoverCallback(const aprs_interfaces::msg::RobotChangeover::SharedPtr msg);

  void GeneratePlanCallback(
    const std::shared_ptr<aprs_interfaces::srv::GeneratePlan::Request> request,
    const std::shared_ptr<aprs_interfaces::srv::GeneratePlan::Response> response
  );
  void ClearCurrentStateCallback(
    const std::shared_ptr<aprs_interfaces::srv::ClearCurrentState::Request> request,
    const std::shared_ptr<aprs_interfaces::srv::ClearCurrentState::Response> response
  );

  rclcpp_action::GoalResponse ExecutePlanHandelGoal(const rclcpp_action::GoalUUID & uuid, std::shared_ptr<const aprs_interfaces::action::ExecutePlan::Goal> goal);
  rclcpp_action::CancelResponse ExecutePlanHandelCancel(const std::shared_ptr<rclcpp_action::ServerGoalHandle<aprs_interfaces::action::ExecutePlan>> goal_handle);
  void ExecutePlanHandelAccept(const std::shared_ptr<rclcpp_action::ServerGoalHandle<aprs_interfaces::action::ExecutePlan>> goal_handle);
  void ExecutePlanExecute(const std::shared_ptr<rclcpp_action::ServerGoalHandle<aprs_interfaces::action::ExecutePlan>> goal_handle);
  bool MoveRobotsHome();

  std::map<uint8_t, std::string> part_size_{
    {aprs_interfaces::msg::SlotInfo::SMALL, "small"},
    {aprs_interfaces::msg::SlotInfo::MEDIUM, "medium"},
    {aprs_interfaces::msg::SlotInfo::LARGE, "large"},
  };
};