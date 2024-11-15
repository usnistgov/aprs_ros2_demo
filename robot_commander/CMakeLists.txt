cmake_minimum_required(VERSION 3.5)
project(robot_commander)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(THIS_PACKAGE_INCLUDE_DEPENDS
  rclcpp
  control_msgs
  aprs_interfaces
  example_interfaces
  moveit_ros_planning_interface
  shape_msgs
)

find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)

foreach(Dependency IN ITEMS ${THIS_PACKAGE_INCLUDE_DEPENDS})
  find_package(${Dependency} REQUIRED)
endforeach()

# Add robot commander library
add_library(robot_commander SHARED
  src/robot_commander.cpp
)

# Add motoman robot commander library
add_library(motoman_robot_commander_lib SHARED
  src/motoman_robot_commander.cpp
)

target_include_directories(robot_commander PUBLIC
  include
)

target_include_directories(motoman_robot_commander_lib PUBLIC
  include
)

ament_target_dependencies(${PROJECT_NAME} ${THIS_PACKAGE_INCLUDE_DEPENDS})
ament_target_dependencies(motoman_robot_commander_lib ${THIS_PACKAGE_INCLUDE_DEPENDS})

install(TARGETS robot_commander
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

install(TARGETS motoman_robot_commander_lib
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

# Add fanuc commander executable
add_executable(fanuc_robot_commander nodes/fanuc_robot_commander.cpp)
target_include_directories(fanuc_robot_commander PRIVATE include)
target_link_libraries(fanuc_robot_commander
    robot_commander
)
ament_target_dependencies(fanuc_robot_commander ${THIS_PACKAGE_INCLUDE_DEPENDS})

install(TARGETS fanuc_robot_commander
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

# Add motoman commander executable
add_executable(motoman_robot_commander nodes/motoman_robot_commander.cpp)
target_include_directories(motoman_robot_commander PRIVATE include)
target_link_libraries(motoman_robot_commander
    motoman_robot_commander_lib
)
ament_target_dependencies(motoman_robot_commander ${THIS_PACKAGE_INCLUDE_DEPENDS})

install(TARGETS motoman_robot_commander
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(PROGRAMS
  nodes/test_follow_joint_trajectory.py
  DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  DIRECTORY launch meshes
  DESTINATION share/${PROJECT_NAME}
)

ament_export_libraries(
  ${PROJECT_NAME}
)

ament_export_include_directories(
  include
)

ament_export_dependencies(
  ${THIS_PACKAGE_INCLUDE_DEPENDS}
)

ament_package()