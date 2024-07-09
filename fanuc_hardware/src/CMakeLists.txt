cmake_minimum_required(VERSION 3.5)
project(fanuc_hardware)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(rclpy)
find_package(example_interfaces)
find_package(ament_cmake_python REQUIRED)

add_library(fanuc_hardware
  SHARED
  src/fanuc_hardware_interface.cpp
)

target_include_directories(
  fanuc_hardware
  PRIVATE
  include
)

ament_target_dependencies(
  fanuc_hardware
  hardware_interface
  pluginlib
  rclcpp
)

pluginlib_export_plugin_description_file(hardware_interface fanuc_hardware.xml)

install(
  TARGETS fanuc_hardware
  DESTINATION lib
)

install(
  DIRECTORY include/
  DESTINATION include
)


ament_export_include_directories(
  include
)

ament_export_libraries(
  fanuc_hardware
)

ament_export_dependencies(
  hardware_interface
  pluginlib
  rclcpp
)

install(PROGRAMS
  nodes/command_publisher.py
  nodes/fanuc_gripper_control.py
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()