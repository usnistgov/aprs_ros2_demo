This is a collection of ROS2 Packages for APRS LAB Demo

# Steps to Run the Demo

## Starting the Robot

1. Turn controller on
2. Switch key on controller to auto
3. Switch teach pendant to OFF
4. On teach pendant, press select --> find ROS_GRIP --> press enter
5. Clear any faults by pressing both shift and reset --> faults in the top left should turn green
6. Press green button on the controller

## Running the ROS nodes

1. Start bringup launch file (Starts ros controllers, RVIZ -> state should match)
    ```
    ros2 launch fanuc_description fanuc_bringup.launch.py 
    ```
2. Start robot commander -->  (Starts moveit, planning scene, and robot_commander_node -> move_group_interface)
    ```
    ros2 launch robot_commander fanuc_robot_commander.launch.py
    ```
3. Launch vision node --> (Starts fanuc_vision_node, teach_table_vision_node) 
    ```
    ros2 launch aprs_vision aprs_vision.launch.py
    ```
4. Move robot to home
    ```
    ros2 service call /move_to_named_pose aprs_interfaces/src/MoveToNamedPose "{name: home}" 
    ```  
5. Call vision system to locate trays 
    ```
    ros2 service call /fanuc_vision/locate_trays example_interfaces/srv/Trigger {}
    ros2 service call /teach_table_vision/locate_trays example_interfaces/srv/Trigger {}
    ```
6. Launch PDDL
    ```
    ros2 launch aprs_pddl aprs_pddl.launch.py (pick action, place action, plansys bringup)
    ```
7. Launch task_planner_node
    ```
    ros2 run aprs_pddl task_planner_node
    ```
8. Generate Initial State through PDDL
    ```
    ros2 service call /generate_pddl_init_state aprs_interfaces/srv/GenerateInitState
    ```
9. Open Plansys Terminal --> 
    ```
    ros2 run plansys2_terminal plansys2_terminal
    --> get plan
    --> run
    ```