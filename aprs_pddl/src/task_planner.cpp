#include <aprs_pddl/task_planner.hpp>

TaskPlanner::TaskPlanner() : Node("task_planner")
{
  teach_trays_subscriber_ = this->create_subscription<aprs_interfaces::msg::Trays>("/teach_table_vision/trays_info", 10, bind(&TaskPlanner::TeachTraysInfoCallback, this, std::placeholders::_1));
  fanuc_trays_subscriber_ = this->create_subscription<aprs_interfaces::msg::Trays>("/fanuc_vision/trays_info", 10, bind(&TaskPlanner::FanucTraysInfoCallback, this, std::placeholders::_1));

  generate_init_state_server_ = this->create_service<aprs_interfaces::srv::GenerateInitState>(
    "/generate_pddl_init_state",
    std::bind(&TaskPlanner::GenerateInitStateCallback, this, std::placeholders::_1, std::placeholders::_2));

  generate_plan_server_ = this->create_service<aprs_interfaces::srv::GeneratePlan>(
    "/generate_pddl_plan",
    std::bind(&TaskPlanner::GeneratePlanCallback, this, std::placeholders::_1, std::placeholders::_2));

  execute_plan_action_ = rclcpp_action::create_server<aprs_interfaces::action::ExecutePlan>(
    this,
    "/execute_pddl_plan",
    std::bind(&TaskPlanner::ExecutePlanHandelGoal, this, std::placeholders::_1, std::placeholders::_2),
    std::bind(&TaskPlanner::ExecutePlanHandelCancel, this, std::placeholders::_1),
    std::bind(&TaskPlanner::ExecutePlanHandelAccept, this, std::placeholders::_1));

  clear_current_state_server_ = this->create_service<aprs_interfaces::srv::ClearCurrentState>(
    "/clear_pddl_current_state",
    std::bind(&TaskPlanner::ClearCurrentStateCallback, this, std::placeholders::_1, std::placeholders::_2));

  domain_expert_ = std::make_shared<plansys2::DomainExpertClient>();
  planner_client_ = std::make_shared<plansys2::PlannerClient>();
  problem_expert_ = std::make_shared<plansys2::ProblemExpertClient>();
  executor_client_ = std::make_shared<plansys2::ExecutorClient>();

  RCLCPP_INFO(get_logger(), "Initialization successful.");
}

TaskPlanner::~TaskPlanner()
{
}

void TaskPlanner::FanucTraysInfoCallback(const aprs_interfaces::msg::Trays::SharedPtr msg)
{
  if (fanuc_kit_trays_.empty() && fanuc_part_trays_.empty()){
    fanuc_kit_trays_ = msg->kit_trays;
    fanuc_part_trays_ = msg->part_trays;
  }
  recieved_fanuc_tray_info = true;
}

void TaskPlanner::TeachTraysInfoCallback(const aprs_interfaces::msg::Trays::SharedPtr msg)
{
  if (teach_kit_trays_.empty() && teach_part_trays_.empty()){
    teach_kit_trays_ = msg->kit_trays;
    teach_part_trays_ = msg->part_trays;
  }
  recieved_teach_tray_info = true;
}

void TaskPlanner::GenerateInitStateCallback(const std::shared_ptr<aprs_interfaces::srv::GenerateInitState::Request> request,
                                            const std::shared_ptr<aprs_interfaces::srv::GenerateInitState::Response> response)
{
  (void)request;
  init_world_state();
  init_goal_state();

  response->success = true;
  response->status = "Successfully Created Plansys2 Problem File";
}

void TaskPlanner::ClearCurrentStateCallback(const std::shared_ptr<aprs_interfaces::srv::ClearCurrentState::Request> request,
                                            const std::shared_ptr<aprs_interfaces::srv::ClearCurrentState::Response> response)
{
  (void)request;
  problem_expert_->clearGoal();
  problem_expert_->clearKnowledge();
  goal_str_ = "(and";
  response->success = true;
  response->status = "Successfully Cleared Current State";
}

void TaskPlanner::GeneratePlanCallback(const std::shared_ptr<aprs_interfaces::srv::GeneratePlan::Request> request,
                                        const std::shared_ptr<aprs_interfaces::srv::GeneratePlan::Response> response){
  (void)request;
  auto domain = domain_expert_->getDomain();
  auto problem = problem_expert_->getProblem();
  auto plan = planner_client_->getPlan(domain, problem);

  if (!plan.has_value()){
    RCLCPP_ERROR(this->get_logger(), "Could not generate plan");
    response->success = false;
    response->status = "Could not generate plan";
    return;
  }

  response->success = true;
  response->plan = plan.value();
  response->status = "Successfully Generated Plan";
}

rclcpp_action::GoalResponse TaskPlanner::ExecutePlanHandelGoal(const rclcpp_action::GoalUUID & uuid, std::shared_ptr<const aprs_interfaces::action::ExecutePlan::Goal> goal){
  RCLCPP_INFO(this->get_logger(), "Received goal request with actions:");
  for (auto& action : goal->plan.items){
    RCLCPP_INFO(this->get_logger(), "%s", action.action.c_str());
  }
  (void)uuid;
  return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
}
rclcpp_action::CancelResponse TaskPlanner::ExecutePlanHandelCancel(const std::shared_ptr<rclcpp_action::ServerGoalHandle<aprs_interfaces::action::ExecutePlan>> goal_handle){
  RCLCPP_INFO(this->get_logger(), "Received request to cancel goal" );
  (void)goal_handle;
  return rclcpp_action::CancelResponse::ACCEPT;
}
void TaskPlanner::ExecutePlanHandelAccept(const std::shared_ptr<rclcpp_action::ServerGoalHandle<aprs_interfaces::action::ExecutePlan>> goal_handle){
   using namespace std::placeholders;
    // this needs to return quickly to avoid blocking the executor, so spin up a new thread
    std::thread{std::bind(&TaskPlanner::ExecutePlanExecute, this, _1), goal_handle}.detach();
}
void TaskPlanner::ExecutePlanExecute(const std::shared_ptr<rclcpp_action::ServerGoalHandle<aprs_interfaces::action::ExecutePlan>> goal_handle){
  rclcpp::Rate loop_rate(1);
  const auto goal = goal_handle->get_goal();
  plansys2_msgs::action::ExecutePlan::Feedback plansys_feedback;
  auto feedback = std::make_shared<aprs_interfaces::action::ExecutePlan::Feedback>();
  auto result = std::make_shared<aprs_interfaces::action::ExecutePlan::Result>();

  if (goal->plan.items.empty()){
    RCLCPP_ERROR(this->get_logger(), "Received plan is null");
    result->success = false;
    result->status = "Plan is null";
    goal_handle->abort(result);
    return;
  }

  if (!executor_client_->start_plan_execution(goal->plan)){
    RCLCPP_ERROR(this->get_logger(), "Error starting plan execution");
    result->success = false;    
    result->status = "Error starting plan execution";
    goal_handle->abort(result);
    return;
  }

  while (executor_client_->execute_and_check_plan()){

    if (goal_handle->is_canceling()){
      RCLCPP_INFO(this->get_logger(), "Goal canceled");
      executor_client_->cancel_plan_execution();
      result->success = false;
      result->status = "Goal canceled";
      goal_handle->canceled(result);
      return;
    }

    plansys_feedback = executor_client_->getFeedBack();

    for (auto& action_feedback : plansys_feedback.action_execution_status){
      if (action_feedback.status == plansys2_msgs::msg::ActionExecutionInfo::EXECUTING){
        feedback->current_action = action_feedback.action;
      }
    }
  }

  auto plansys_result = executor_client_->getResult();

  if (plansys_result->success){
    RCLCPP_INFO(this->get_logger(), "Plan executed successfully");
    result->success = true;
    result->status = "Plan executed successfully";
    goal_handle->succeed(result);
    return;
  } else {
    RCLCPP_ERROR(this->get_logger(), "Error executing plan");
    auto action_execution_status = plansys_result->action_execution_status;
    for (auto& action_status : action_execution_status){
      if (action_status.status == plansys2_msgs::msg::ActionExecutionInfo::FAILED){
        RCLCPP_ERROR(this->get_logger(), ("Action " + action_status.action + " failed").c_str());
        result->success = false;
        result->status = "Action " + action_status.action + " failed";
        goal_handle->abort(result);
        return;
      }
    }
  }
}

void TaskPlanner::init_world_state(){

  problem_expert_->addInstance(plansys2::Instance{"small", "size"});
  problem_expert_->addInstance(plansys2::Instance{"medium", "size"});
  problem_expert_->addInstance(plansys2::Instance{"large", "size"});

  problem_expert_->addPredicate(plansys2::Predicate("(robot_on_standby"));
  problem_expert_->addPredicate(plansys2::Predicate("(gripper_empty"));

  for (auto& kit_tray : fanuc_kit_trays_){
    for (auto& kit_tray_slot : kit_tray.slots){
      problem_expert_->addInstance(plansys2::Instance{kit_tray_slot.name, "slot"});
      if (!kit_tray_slot.occupied) {
        problem_expert_->addPredicate(plansys2::Predicate("(kit_slot_empty " + kit_tray_slot.name + " " + part_size_[kit_tray_slot.size] + ")"));
      }
      else{
        problem_expert_->addPredicate(plansys2::Predicate("(kit_slot_occupied " + kit_tray_slot.name + " " + part_size_[kit_tray_slot.size] + ")"));
      }
    }
  }

  for (auto& part_tray : fanuc_part_trays_){
    for (auto& part_tray_slot : part_tray.slots){
      problem_expert_->addInstance(plansys2::Instance{part_tray_slot.name, "slot"});
      if (!part_tray_slot.occupied) {
        continue;
      }
      else{
        problem_expert_->addPredicate(plansys2::Predicate("(in_part_tray " + GeneratePartName(part_tray_slot.size) + " " + part_tray_slot.name + " " + part_size_[part_tray_slot.size] + ")"));
      }
    }
  }
}

std::string TaskPlanner::GeneratePartName(const uint8_t part_size){

  std::string part_name = part_size_[part_size] + "_" + std::to_string(part_size_count_[part_size]);
  problem_expert_->addInstance(plansys2::Instance{part_name, "part"});
  part_size_count_[part_size] += 1;

  return part_name;
}

void TaskPlanner::init_goal_state(){

  for (auto& kit_tray : teach_kit_trays_){
    for (auto& kit_tray_slot : kit_tray.slots){
      problem_expert_->addInstance(plansys2::Instance{kit_tray_slot.name, "slot"});
      if (!kit_tray_slot.occupied) {
        goal_str_ += ("(kit_slot_empty " + kit_tray_slot.name + " " + part_size_[kit_tray_slot.size] + ")");
      }
      else{
        goal_str_ += ("(kit_slot_occupied " + kit_tray_slot.name + " " + part_size_[kit_tray_slot.size] + ")");
      }
    }
  }

  goal_str_ += ")";

  RCLCPP_INFO_STREAM(get_logger(), "Goal is: " << goal_str_);
  problem_expert_->setGoal(plansys2::Goal(goal_str_));
}
