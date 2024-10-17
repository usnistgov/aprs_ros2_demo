cmake_minimum_required(VERSION 3.5)
project(aprs_driver)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(THIS_PACKAGE_INCLUDE_DEPENDS
  rclcpp
  hardware_interface
  controller_interface
  pluginlib
  rclcpp_components
  control_msgs
  rclcpp_action
)

find_package(ament_cmake REQUIRED)

foreach(Dependency IN ITEMS ${THIS_PACKAGE_INCLUDE_DEPENDS})
  find_package(${Dependency} REQUIRED)
endforeach()

# Build simple messages library
add_library(simple_messages SHARED
  src/simple_messages.cpp
)

target_include_directories(simple_messages PUBLIC
  include
)

# Build fanuc hardware interface library
add_library(fanuc_hardware SHARED
  src/fanuc_hardware_interface.cpp
)

target_include_directories(fanuc_hardware PRIVATE
  include
)

target_link_libraries(fanuc_hardware simple_messages)
ament_target_dependencies(fanuc_hardware ${THIS_PACKAGE_INCLUDE_DEPENDS})

pluginlib_export_plugin_description_file(hardware_interface fanuc_hardware.xml)

# Build fanuc joint trajectory controller
add_library(fanuc_controller SHARED
  src/fanuc_joint_trajectory_controller.cpp
)

target_include_directories(fanuc_controller PRIVATE
  include
)

target_link_libraries(fanuc_controller simple_messages)
ament_target_dependencies(fanuc_controller ${THIS_PACKAGE_INCLUDE_DEPENDS})

pluginlib_export_plugin_description_file(controller_interface fanuc_controller.xml)

install(
  TARGETS simple_messages fanuc_hardware fanuc_controller
  DESTINATION lib
)

ament_package()