#include <aprs_pddl/task_planner.hpp>

TaskPlanner::TaskPlanner() : Node("task_planner")
{
  fanuc_table_subscriber_ = this->create_subscription<aprs_interfaces::msg::Trays>("/fanuc/table_trays_info", 10, bind(&TaskPlanner::FanucTableTraysInfoCallback, this, std::placeholders::_1));
  fanuc_conveyor_subscriber_ = this->create_subscription<aprs_interfaces::msg::Trays>("/fanuc/conveyor_trays_info", 10, bind(&TaskPlanner::FanucConveyorTraysInfoCallback, this, std::placeholders::_1));
  motoman_table_subscriber_ = this->create_subscription<aprs_interfaces::msg::Trays>("/motoman/table_trays_info", 10, bind(&TaskPlanner::MotomanTableTraysInfoCallback, this, std::placeholders::_1));
  motoman_conveyor_subscriber_ = this->create_subscription<aprs_interfaces::msg::Trays>("/motoman/conveyor_trays_info", 10, bind(&TaskPlanner::MotomanConveyorTraysInfoCallback, this, std::placeholders::_1));
  teach_table_subscriber_ = this->create_subscription<aprs_interfaces::msg::Trays>("/teach/table_trays_info", 10, bind(&TaskPlanner::TeachTableTraysInfoCallback, this, std::placeholders::_1));
  robot_status_subscriber_ = this->create_subscription<aprs_interfaces::msg::RobotStatus>("/robot_status", 10, bind(&TaskPlanner::RobotStatusCallback, this, std::placeholders::_1));

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

void TaskPlanner::FanucTableTraysInfoCallback(const aprs_interfaces::msg::Trays::SharedPtr msg)
{
  if (fanuc_table_kit_trays_.empty() && fanuc_table_part_trays_.empty()){
    fanuc_table_kit_trays_ = msg->kit_trays;
    fanuc_table_part_trays_ = msg->part_trays;
  }
  recieved_fanuc_table_info = true;
}

void TaskPlanner::FanucConveyorTraysInfoCallback(const aprs_interfaces::msg::Trays::SharedPtr msg)
{
  if (fanuc_conveyor_kit_trays_.empty() && fanuc_conveyor_part_trays_.empty()){
    fanuc_conveyor_kit_trays_ = msg->kit_trays;
    fanuc_conveyor_part_trays_ = msg->part_trays;
  }
  recieved_fanuc_conveyor_info = true;
}

void TaskPlanner::MotomanTableTraysInfoCallback(const aprs_interfaces::msg::Trays::SharedPtr msg)
{
  if (motoman_table_kit_trays_.empty() && motoman_table_part_trays_.empty()){
    motoman_table_kit_trays_ = msg->kit_trays;
    motoman_table_part_trays_ = msg->part_trays;
  }
  recieved_motoman_table_info = true;
}

void TaskPlanner::MotomanConveyorTraysInfoCallback(const aprs_interfaces::msg::Trays::SharedPtr msg)
{
  if (motoman_conveyor_kit_trays_.empty() && motoman_conveyor_part_trays_.empty()){
    motoman_conveyor_kit_trays_ = msg->kit_trays;
    motoman_conveyor_part_trays_ = msg->part_trays;
  }
  recieved_motoman_conveyor_info = true;
}

void TaskPlanner::TeachTableTraysInfoCallback(const aprs_interfaces::msg::Trays::SharedPtr msg)
{
  if (teach_table_kit_trays_.empty() && teach_table_part_trays_.empty()){
    teach_table_kit_trays_ = msg->kit_trays;
    teach_table_part_trays_ = msg->part_trays;
  }
  recieved_teach_table_info = true;
}

void TaskPlanner::RobotStatusCallback(const aprs_interfaces::msg::RobotStatus::SharedPtr msg)
{
  fanuc_operational = msg->fanuc;
  motoman_operational = msg->motoman;
}

void TaskPlanner::ClearCurrentStateCallback(const std::shared_ptr<aprs_interfaces::srv::ClearCurrentState::Request>,
                                            const std::shared_ptr<aprs_interfaces::srv::ClearCurrentState::Response> response)
{
  problem_expert_->clearGoal();
  problem_expert_->clearKnowledge();
  goal_str_.clear();
  response->success = true;
  response->status = "Successfully Cleared Knowledge and Goal";
}

void TaskPlanner::GeneratePlanCallback(const std::shared_ptr<aprs_interfaces::srv::GeneratePlan::Request>,
                                        const std::shared_ptr<aprs_interfaces::srv::GeneratePlan::Response> response)
{
  if (!recieved_fanuc_table_info || !recieved_fanuc_conveyor_info || !recieved_motoman_table_info || !recieved_motoman_conveyor_info || !recieved_teach_table_info){
    RCLCPP_ERROR(this->get_logger(), "Could not generate plan, missing tray information");
    response->success = false;
    response->status = "Could not generate plan, missing tray information";
    return;
  }

  if (goal_str_.empty()){
    init_world_state();
    init_goal_state();
  }
  else if (!fanuc_operational){
    update_world_state("fanuc");
  }
  else if (!motoman_operational){
    update_world_state("motoman");
  }

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

void TaskPlanner::process_trays(const std::vector<aprs_interfaces::msg::Tray>& trays, const std::string& location) {
  for (const auto& tray : trays) {
    for (const auto& slot : tray.slots) {
      problem_expert_->addInstance(plansys2::Instance{slot.name, "slot"});
      problem_expert_->addPredicate(plansys2::Predicate("(slot_at " + slot.name + " " + location + ")"));
      if (!slot.occupied) {
        problem_expert_->addPredicate(plansys2::Predicate("(slot_empty " + slot.name + " " + part_size_[slot.size] + ")"));
      } else {
        problem_expert_->addPredicate(plansys2::Predicate("(slot_occupied " + slot.name + " " + part_size_[slot.size] + ")"));
      }
    }
  }
}

void TaskPlanner::init_world_state(){

  problem_expert_->addInstance(plansys2::Instance{"small", "size"});
  problem_expert_->addInstance(plansys2::Instance{"medium", "size"});
  problem_expert_->addInstance(plansys2::Instance{"large", "size"});

  problem_expert_->addInstance(plansys2::Instance{"fanuc", "robot"});
  problem_expert_->addInstance(plansys2::Instance{"motoman", "robot"});

  problem_expert_->addInstance(plansys2::Instance{"fanuc_table", "location"});
  problem_expert_->addInstance(plansys2::Instance{"fanuc_conveyor", "location"});
  problem_expert_->addInstance(plansys2::Instance{"motoman_table", "location"});
  problem_expert_->addInstance(plansys2::Instance{"motoman_conveyor", "location"});

  problem_expert_->addPredicate(plansys2::Predicate("(robot_on_standby fanuc)"));
  problem_expert_->addPredicate(plansys2::Predicate("(robot_on_standby motoman)"));

  problem_expert_->addPredicate(plansys2::Predicate("(gripper_empty fanuc)"));
  problem_expert_->addPredicate(plansys2::Predicate("(gripper_empty motoman)"));

  problem_expert_->addPredicate(plansys2::Predicate("(robot_operational fanuc)"));
  problem_expert_->addPredicate(plansys2::Predicate("(robot_operational motoman)"));

  problem_expert_->addPredicate(plansys2::Predicate("(in_reach fanuc_table fanuc)"));
  problem_expert_->addPredicate(plansys2::Predicate("(in_reach motoman_table motoman)"));
  problem_expert_->addPredicate(plansys2::Predicate("(in_reach fanuc_conveyor fanuc)"));
  problem_expert_->addPredicate(plansys2::Predicate("(in_reach motoman_conveyor motoman)"));

  // Process Fanuc Table
  process_trays(fanuc_table_kit_trays_, "fanuc_table");
  process_trays(fanuc_table_part_trays_, "fanuc_table");

  // Process Fanuc Conveyor
  process_trays(fanuc_conveyor_kit_trays_, "fanuc_conveyor");
  process_trays(fanuc_conveyor_part_trays_, "fanuc_conveyor");

  // Process Motoman Table
  process_trays(motoman_table_kit_trays_, "motoman_table");
  process_trays(motoman_table_part_trays_, "motoman_table");

  // Process Motoman Conveyor
  process_trays(motoman_conveyor_kit_trays_, "motoman_conveyor");
  process_trays(motoman_conveyor_part_trays_, "motoman_conveyor");
}

void TaskPlanner::init_goal_state(){

  goal_str_ = "(and";

  for (auto& kit_tray : teach_table_kit_trays_){
    for (auto& kit_tray_slot : kit_tray.slots){
      if (!kit_tray_slot.occupied) {
        goal_str_ += ("(slot_empty " + kit_tray_slot.name + " " + part_size_[kit_tray_slot.size] + ")");
      }
      else{
        goal_str_ += ("(slot_occupied " + kit_tray_slot.name + " " + part_size_[kit_tray_slot.size] + ")");
      }
    }
  }

  goal_str_ += "(at_home fanuc)(at_home motoman))";

  RCLCPP_INFO_STREAM(get_logger(), "Goal is: " << goal_str_);
  problem_expert_->setGoal(plansys2::Goal(goal_str_));
}

void TaskPlanner::update_world_state(std::string robot){
  if (robot == "fanuc"){
    problem_expert_->removePredicate(plansys2::Predicate("(robot_operational fanuc)"));
    problem_expert_->removePredicate(plansys2::Predicate("(robot_on_standby fanuc)"));
    problem_expert_->removePredicate(plansys2::Predicate("(in_reach fanuc_table fanuc)"));
    problem_expert_->removePredicate(plansys2::Predicate("(in_reach fanuc_conveyor fanuc)"));

    problem_expert_->addPredicate(plansys2::Predicate("(robot_faulty fanuc)"));
    problem_expert_->addPredicate(plansys2::Predicate("(in_reach fanuc_conveyor motoman)"));
  }
  else if (robot == "motoman"){
    problem_expert_->removePredicate(plansys2::Predicate("(robot_operational motoman)"));
    problem_expert_->removePredicate(plansys2::Predicate("(robot_on_standby motoman)"));
    problem_expert_->removePredicate(plansys2::Predicate("(in_reach motoman_table motoman)"));
    problem_expert_->removePredicate(plansys2::Predicate("(in_reach motoman_conveyor motoman)"));

    problem_expert_->addPredicate(plansys2::Predicate("(robot_faulty motoman)"));
    problem_expert_->addPredicate(plansys2::Predicate("(in_reach motoman_table fanuc)"));
  }
}