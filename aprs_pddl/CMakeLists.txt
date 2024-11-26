cmake_minimum_required(VERSION 3.8)
project(aprs_pddl)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(THIS_PACKAGE_INCLUDE_DEPENDS
  rclcpp
  rclcpp_action
  aprs_interfaces
  plansys2_msgs
  plansys2_executor
  plansys2_domain_expert
  plansys2_planner
  plansys2_problem_expert
  plansys2_pddl_parser
  conveyor_interfaces
)

find_package(ament_cmake REQUIRED)

foreach(Dependency IN ITEMS ${THIS_PACKAGE_INCLUDE_DEPENDS})
  find_package(${Dependency} REQUIRED)
endforeach()

# World State Library
add_library(task_planner_library SHARED
  src/task_planner.cpp
)
target_include_directories(task_planner_library PUBLIC 
  include
)

ament_target_dependencies(task_planner_library ${THIS_PACKAGE_INCLUDE_DEPENDS})

install(TARGETS task_planner_library
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

# World state node executable
add_executable(task_planner_node src/task_planner_node.cpp)
target_include_directories(task_planner_node PRIVATE include)
target_link_libraries(task_planner_node
  task_planner_library
)
ament_target_dependencies(task_planner_node ${THIS_PACKAGE_INCLUDE_DEPENDS})

# Action Executables

# pick_part_action
add_executable(pick_part_action_node src/pick_part_action.cpp)
target_include_directories(pick_part_action_node PRIVATE include)
ament_target_dependencies(pick_part_action_node ${THIS_PACKAGE_INCLUDE_DEPENDS})

# place_part_action
add_executable(place_part_action_node src/place_part_action.cpp)
target_include_directories(place_part_action_node PRIVATE include)
ament_target_dependencies(place_part_action_node ${THIS_PACKAGE_INCLUDE_DEPENDS})

# move_conveyor_action
add_executable(move_conveyor_action_node src/move_conveyor_action.cpp)
target_include_directories(move_conveyor_action_node PRIVATE include)
ament_target_dependencies(move_conveyor_action_node ${THIS_PACKAGE_INCLUDE_DEPENDS})

install(TARGETS
    task_planner_node
    pick_part_action_node
    place_part_action_node
    move_conveyor_action_node
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include
)

install(DIRECTORY 
  launch
  pddl
  DESTINATION share/${PROJECT_NAME}
)

ament_export_libraries(
  task_planner_library
)

ament_export_include_directories(
  include
)

ament_export_dependencies(
  ${THIS_PACKAGE_INCLUDE_DEPENDS}
)

ament_package()