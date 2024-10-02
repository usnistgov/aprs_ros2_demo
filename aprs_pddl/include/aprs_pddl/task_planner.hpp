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

#include "aprs_interfaces/srv/generate_init_state.hpp"
#include "aprs_interfaces/srv/generate_plan.hpp"
#include "aprs_interfaces/srv/clear_current_state.hpp"
#include "aprs_interfaces/action/execute_plan.hpp"
#include "aprs_interfaces/msg/trays.hpp"
#include "aprs_interfaces/msg/slot_info.hpp"
#include "aprs_interfaces/msg/tray.hpp"

class TaskPlanner : public rclcpp::Node
{
public:
  /// Constructor
  TaskPlanner();

  ~TaskPlanner();

  void init_world_state();
  void init_goal_state();

 private:

  // Plansys2
  std::shared_ptr<plansys2::DomainExpertClient> domain_expert_;
  std::shared_ptr<plansys2::PlannerClient> planner_client_;
  std::shared_ptr<plansys2::ProblemExpertClient> problem_expert_;
  std::shared_ptr<plansys2::ExecutorClient> executor_client_; 

  rclcpp::Service<aprs_interfaces::srv::GenerateInitState>::SharedPtr generate_init_state_server_;
  rclcpp::Service<aprs_interfaces::srv::ClearCurrentState>::SharedPtr clear_current_state_server_;
  rclcpp::Service<aprs_interfaces::srv::GeneratePlan>::SharedPtr generate_plan_server_;
  rclcpp_action::Server<aprs_interfaces::action::ExecutePlan>::SharedPtr execute_plan_action_;

  rclcpp::Subscription<aprs_interfaces::msg::Trays>::SharedPtr fanuc_trays_subscriber_;
  rclcpp::Subscription<aprs_interfaces::msg::Trays>::SharedPtr teach_trays_subscriber_;

  rclcpp::CallbackGroup::SharedPtr cb_group_;
  rclcpp::CallbackGroup::SharedPtr topic_cb_group_;
  
  // APRS Demo Objects
  std::vector<aprs_interfaces::msg::Tray> fanuc_kit_trays_;
  std::vector<aprs_interfaces::msg::Tray> fanuc_part_trays_;
  std::vector<aprs_interfaces::msg::Tray> teach_kit_trays_;
  std::vector<aprs_interfaces::msg::Tray> teach_part_trays_;

  //bool
  bool recieved_fanuc_tray_info = false;
  bool recieved_teach_tray_info = false;

  std::string goal_str_ = "(and";

  // Callbacks
  void FanucTraysInfoCallback(const aprs_interfaces::msg::Trays::SharedPtr msg);
  void TeachTraysInfoCallback(const aprs_interfaces::msg::Trays::SharedPtr msg);

  void GenerateInitStateCallback(
    const std::shared_ptr<aprs_interfaces::srv::GenerateInitState::Request> request,
    const std::shared_ptr<aprs_interfaces::srv::GenerateInitState::Response> response
  ); 
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

  std::string GeneratePartName(const uint8_t part_size);

  std::map<uint8_t, int> part_size_count_{
    {aprs_interfaces::msg::SlotInfo::SMALL, 0},
    {aprs_interfaces::msg::SlotInfo::MEDIUM, 0},
    {aprs_interfaces::msg::SlotInfo::LARGE, 0},
  };

  std::map<uint8_t, std::string> part_size_{
    {aprs_interfaces::msg::SlotInfo::SMALL, "small"},
    {aprs_interfaces::msg::SlotInfo::MEDIUM, "medium"},
    {aprs_interfaces::msg::SlotInfo::LARGE, "large"},
  };
};