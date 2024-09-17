// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from aprs_interfaces:msg/Tray.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__TRAY__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define APRS_INTERFACES__MSG__DETAIL__TRAY__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "aprs_interfaces/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "aprs_interfaces/msg/detail/tray__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace aprs_interfaces
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_aprs_interfaces
cdr_serialize(
  const aprs_interfaces::msg::Tray & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_aprs_interfaces
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  aprs_interfaces::msg::Tray & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_aprs_interfaces
get_serialized_size(
  const aprs_interfaces::msg::Tray & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_aprs_interfaces
max_serialized_size_Tray(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace aprs_interfaces

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_aprs_interfaces
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, aprs_interfaces, msg, Tray)();

#ifdef __cplusplus
}
#endif

#endif  // APRS_INTERFACES__MSG__DETAIL__TRAY__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
