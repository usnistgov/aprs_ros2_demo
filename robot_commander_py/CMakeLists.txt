cmake_minimum_required(VERSION 3.8)
project(robot_commander_py)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclpy REQUIRED)

# Install Python modules
ament_python_install_package(${PROJECT_NAME} SCRIPTS_DESTINATION lib/${PROJECT_NAME})

# Install Python executables
install(PROGRAMS
  nodes/test_moveit.py
DESTINATION lib/${PROJECT_NAME}
)

# Install other directories to the share folder
install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()