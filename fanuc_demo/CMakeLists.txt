cmake_minimum_required(VERSION 3.8)
project(fanuc_demo)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)

find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(rmw REQUIRED)

find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(example_interfaces)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(aprs_interfaces REQUIRED)



add_executable(fanuc_kitting src/fanuc_kitting.cpp)
target_include_directories(fanuc_kitting PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(fanuc_kitting PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

ament_target_dependencies(fanuc_kitting
  rclcpp
  rclcpp_components
  std_msgs
  geometry_msgs
  example_interfaces
  moveit_ros_planning_interface
  aprs_interfaces
)

install(TARGETS fanuc_kitting
RUNTIME DESTINATION lib/${PROJECT_NAME})

install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
