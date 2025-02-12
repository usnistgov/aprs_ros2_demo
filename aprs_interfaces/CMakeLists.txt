cmake_minimum_required(VERSION 3.8)
project(aprs_interfaces)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()
# find dependencies
find_package(ament_cmake REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(plansys2_msgs REQUIRED)

set(msg_files
  "msg/Tray.msg"
  "msg/Trays.msg"
  "msg/SlotInfo.msg"
  "msg/RobotChangeover.msg"
)

set(srv_files
  "srv/MoveToNamedPose.srv"
  "srv/Pick.srv"
  "srv/Place.srv"
  "srv/GeneratePlan.srv"
  "srv/ClearCurrentState.srv"
  "srv/LocateTrays.srv"
  "srv/PneumaticGripperControl.srv"
)

set(action_files
  "action/ExecutePlan.action"
)

rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
  ${srv_files}
  ${action_files}

  DEPENDENCIES
  builtin_interfaces
  std_msgs
  geometry_msgs
  plansys2_msgs
  
  ADD_LINTER_TESTS
)
ament_export_dependencies(rosidl_default_runtime)
ament_package()
