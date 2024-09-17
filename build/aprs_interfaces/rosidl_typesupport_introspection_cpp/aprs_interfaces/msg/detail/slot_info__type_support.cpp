// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from aprs_interfaces:msg/SlotInfo.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "aprs_interfaces/msg/detail/slot_info__functions.h"
#include "aprs_interfaces/msg/detail/slot_info__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace aprs_interfaces
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void SlotInfo_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) aprs_interfaces::msg::SlotInfo(_init);
}

void SlotInfo_fini_function(void * message_memory)
{
  auto typed_message = static_cast<aprs_interfaces::msg::SlotInfo *>(message_memory);
  typed_message->~SlotInfo();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember SlotInfo_message_member_array[3] = {
  {
    "occupied",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aprs_interfaces::msg::SlotInfo, occupied),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "size",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aprs_interfaces::msg::SlotInfo, size),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "name",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aprs_interfaces::msg::SlotInfo, name),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers SlotInfo_message_members = {
  "aprs_interfaces::msg",  // message namespace
  "SlotInfo",  // message name
  3,  // number of fields
  sizeof(aprs_interfaces::msg::SlotInfo),
  SlotInfo_message_member_array,  // message members
  SlotInfo_init_function,  // function to initialize message memory (memory has to be allocated)
  SlotInfo_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t SlotInfo_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &SlotInfo_message_members,
  get_message_typesupport_handle_function,
  &aprs_interfaces__msg__SlotInfo__get_type_hash,
  &aprs_interfaces__msg__SlotInfo__get_type_description,
  &aprs_interfaces__msg__SlotInfo__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace aprs_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<aprs_interfaces::msg::SlotInfo>()
{
  return &::aprs_interfaces::msg::rosidl_typesupport_introspection_cpp::SlotInfo_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, msg, SlotInfo)() {
  return &::aprs_interfaces::msg::rosidl_typesupport_introspection_cpp::SlotInfo_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
