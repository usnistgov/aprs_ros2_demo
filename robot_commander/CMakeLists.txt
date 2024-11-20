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

# Find packages
find_package(ament_cmake REQUIRED)

foreach(Dependency IN ITEMS ${THIS_PACKAGE_INCLUDE_DEPENDS})
  find_package(${Dependency} REQUIRED)
endforeach()

# Build robot commander library
add_library(robot_commander_library SHARED src/robot_commander.cpp)

target_include_directories(robot_commander_library PUBLIC 
  include
)

ament_target_dependencies(robot_commander_library ${THIS_PACKAGE_INCLUDE_DEPENDS})

install(TARGETS robot_commander_library
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

# Build robot commander executable
add_executable(robot_commander src/robot_commander.cpp)
target_link_libraries(robot_commander
  robot_commander_library
)
ament_target_dependencies(robot_commander ${THIS_PACKAGE_INCLUDE_DEPENDS})

install(TARGETS
  robot_commander
  DESTINATION lib/${PROJECT_NAME}
)

# Install other directories to the share folder
install(
  DIRECTORY launch meshes config
  DESTINATION share/${PROJECT_NAME}
)

ament_package()