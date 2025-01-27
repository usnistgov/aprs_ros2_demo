from aprs_interfaces.srv import LocateTrays
import rclpy
from rclpy.node import Node
from rclpy.executors import MultiThreadedExecutor
from aprs_interfaces.srv import GeneratePlan
from rclpy.action import ActionServer
from rclpy.action.server import ServerGoalHandle
from aprs_interfaces.action import ExecutePlan
from time import sleep
from plansys2_msgs.msg import PlanItem


class MinimalService(Node):

    def __init__(self):
        super().__init__('minimal_service')
        self.srv = self.create_service(GeneratePlan, '/generate_pddl_plan', self.add_two_ints_callback)

    def add_two_ints_callback(self, request, response: GeneratePlan.Response):
        response.success = True

        plan_item = PlanItem()
        plan_item.action = "Action 1"

        response.plan.items.append(plan_item)

        plan_item2 = PlanItem()
        plan_item2.action = "Action 2"

        response.plan.items.append(plan_item2)

        return response

class FibonacciActionServer(Node):

    def __init__(self):
        super().__init__('pddl_server')
        self._action_server = ActionServer(
            self,
            ExecutePlan,
            '/execute_pddl_plan',
            self.execute_callback)
    
    def execute_callback(self, goal_handle: ServerGoalHandle):
        self.get_logger().info('Executing goal...')

        feedback_msg = ExecutePlan.Feedback()

        for i in range(1, 11):
            feedback_msg.current_action = f"Feedback loop {i}"
            goal_handle.publish_feedback(feedback_msg)
            sleep(1)

        goal_handle.succeed()

        result = ExecutePlan.Result()
        result.success = True
        result.status = "Finished execution"
        return result

def main(args=None):
    rclpy.init(args=args)
    executor = MultiThreadedExecutor(2)

    minimal_service = MinimalService()
    action_server = FibonacciActionServer()

    executor.add_node(minimal_service)
    executor.add_node(action_server)

    executor.spin()

    rclpy.shutdown()


if __name__ == '__main__':
    main()