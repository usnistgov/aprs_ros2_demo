// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from aprs_interfaces:msg/Tray.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "aprs_interfaces/msg/detail/tray__functions.h"
#include "aprs_interfaces/msg/detail/tray__struct.hpp"
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

void Tray_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) aprs_interfaces::msg::Tray(_init);
}

void Tray_fini_function(void * message_memory)
{
  auto typed_message = static_cast<aprs_interfaces::msg::Tray *>(message_memory);
  typed_message->~Tray();
}

size_t size_function__Tray__slots(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<aprs_interfaces::msg::SlotInfo> *>(untyped_member);
  return member->size();
}

const void * get_const_function__Tray__slots(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<aprs_interfaces::msg::SlotInfo> *>(untyped_member);
  return &member[index];
}

void * get_function__Tray__slots(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<aprs_interfaces::msg::SlotInfo> *>(untyped_member);
  return &member[index];
}

void fetch_function__Tray__slots(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const aprs_interfaces::msg::SlotInfo *>(
    get_const_function__Tray__slots(untyped_member, index));
  auto & value = *reinterpret_cast<aprs_interfaces::msg::SlotInfo *>(untyped_value);
  value = item;
}

void assign_function__Tray__slots(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<aprs_interfaces::msg::SlotInfo *>(
    get_function__Tray__slots(untyped_member, index));
  const auto & value = *reinterpret_cast<const aprs_interfaces::msg::SlotInfo *>(untyped_value);
  item = value;
}

void resize_function__Tray__slots(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<aprs_interfaces::msg::SlotInfo> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember Tray_message_member_array[3] = {
  {
    "identifier",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aprs_interfaces::msg::Tray, identifier),  // bytes offset in struct
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
    offsetof(aprs_interfaces::msg::Tray, name),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "slots",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<aprs_interfaces::msg::SlotInfo>(),  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aprs_interfaces::msg::Tray, slots),  // bytes offset in struct
    nullptr,  // default value
    size_function__Tray__slots,  // size() function pointer
    get_const_function__Tray__slots,  // get_const(index) function pointer
    get_function__Tray__slots,  // get(index) function pointer
    fetch_function__Tray__slots,  // fetch(index, &value) function pointer
    assign_function__Tray__slots,  // assign(index, value) function pointer
    resize_function__Tray__slots  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers Tray_message_members = {
  "aprs_interfaces::msg",  // message namespace
  "Tray",  // message name
  3,  // number of fields
  sizeof(aprs_interfaces::msg::Tray),
  Tray_message_member_array,  // message members
  Tray_init_function,  // function to initialize message memory (memory has to be allocated)
  Tray_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t Tray_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &Tray_message_members,
  get_message_typesupport_handle_function,
  &aprs_interfaces__msg__Tray__get_type_hash,
  &aprs_interfaces__msg__Tray__get_type_description,
  &aprs_interfaces__msg__Tray__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace aprs_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<aprs_interfaces::msg::Tray>()
{
  return &::aprs_interfaces::msg::rosidl_typesupport_introspection_cpp::Tray_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, msg, Tray)() {
  return &::aprs_interfaces::msg::rosidl_typesupport_introspection_cpp::Tray_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
