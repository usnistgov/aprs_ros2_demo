cmake_minimum_required(VERSION 3.5)
project(simple_msgs_interfaces)

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
find_package(rclcpp_components REQUIRED)
find_package(example_interfaces)
find_package(ament_cmake_python REQUIRED)

add_library(simple_msgs_interface
  SHARED
  src/simple_msgs_interface.cpp
)

target_include_directories(
  simple_msgs_interface
  PRIVATE
  include
)

ament_target_dependencies(
  simple_msgs_interface
  rclcpp
)

install(
  TARGETS simple_msgs_interface
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
  simple_msgs_interface
)

ament_export_dependencies(
  rclcpp
)

ament_package()