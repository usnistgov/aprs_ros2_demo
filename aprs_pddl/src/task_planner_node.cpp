#include <aprs_pddl/task_planner.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/executors/single_threaded_executor.hpp>

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);

  auto task_planner = std::make_shared<TaskPlanner>();

  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(task_planner);
  executor.spin();

  // while(rclcpp::ok()){
  //   // RCLCPP_INFO(task_planner->get_logger(), "Test");
  // };

  // rclcpp::shutdown();

  return 0;
}