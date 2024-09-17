// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from aprs_interfaces:msg/Trays.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "aprs_interfaces/msg/detail/trays__functions.h"
#include "aprs_interfaces/msg/detail/trays__struct.hpp"
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

void Trays_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) aprs_interfaces::msg::Trays(_init);
}

void Trays_fini_function(void * message_memory)
{
  auto typed_message = static_cast<aprs_interfaces::msg::Trays *>(message_memory);
  typed_message->~Trays();
}

size_t size_function__Trays__kit_trays(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<aprs_interfaces::msg::Tray> *>(untyped_member);
  return member->size();
}

const void * get_const_function__Trays__kit_trays(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<aprs_interfaces::msg::Tray> *>(untyped_member);
  return &member[index];
}

void * get_function__Trays__kit_trays(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<aprs_interfaces::msg::Tray> *>(untyped_member);
  return &member[index];
}

void fetch_function__Trays__kit_trays(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const aprs_interfaces::msg::Tray *>(
    get_const_function__Trays__kit_trays(untyped_member, index));
  auto & value = *reinterpret_cast<aprs_interfaces::msg::Tray *>(untyped_value);
  value = item;
}

void assign_function__Trays__kit_trays(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<aprs_interfaces::msg::Tray *>(
    get_function__Trays__kit_trays(untyped_member, index));
  const auto & value = *reinterpret_cast<const aprs_interfaces::msg::Tray *>(untyped_value);
  item = value;
}

void resize_function__Trays__kit_trays(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<aprs_interfaces::msg::Tray> *>(untyped_member);
  member->resize(size);
}

size_t size_function__Trays__part_trays(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<aprs_interfaces::msg::Tray> *>(untyped_member);
  return member->size();
}

const void * get_const_function__Trays__part_trays(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<aprs_interfaces::msg::Tray> *>(untyped_member);
  return &member[index];
}

void * get_function__Trays__part_trays(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<aprs_interfaces::msg::Tray> *>(untyped_member);
  return &member[index];
}

void fetch_function__Trays__part_trays(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const aprs_interfaces::msg::Tray *>(
    get_const_function__Trays__part_trays(untyped_member, index));
  auto & value = *reinterpret_cast<aprs_interfaces::msg::Tray *>(untyped_value);
  value = item;
}

void assign_function__Trays__part_trays(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<aprs_interfaces::msg::Tray *>(
    get_function__Trays__part_trays(untyped_member, index));
  const auto & value = *reinterpret_cast<const aprs_interfaces::msg::Tray *>(untyped_value);
  item = value;
}

void resize_function__Trays__part_trays(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<aprs_interfaces::msg::Tray> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember Trays_message_member_array[2] = {
  {
    "kit_trays",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<aprs_interfaces::msg::Tray>(),  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aprs_interfaces::msg::Trays, kit_trays),  // bytes offset in struct
    nullptr,  // default value
    size_function__Trays__kit_trays,  // size() function pointer
    get_const_function__Trays__kit_trays,  // get_const(index) function pointer
    get_function__Trays__kit_trays,  // get(index) function pointer
    fetch_function__Trays__kit_trays,  // fetch(index, &value) function pointer
    assign_function__Trays__kit_trays,  // assign(index, value) function pointer
    resize_function__Trays__kit_trays  // resize(index) function pointer
  },
  {
    "part_trays",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<aprs_interfaces::msg::Tray>(),  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aprs_interfaces::msg::Trays, part_trays),  // bytes offset in struct
    nullptr,  // default value
    size_function__Trays__part_trays,  // size() function pointer
    get_const_function__Trays__part_trays,  // get_const(index) function pointer
    get_function__Trays__part_trays,  // get(index) function pointer
    fetch_function__Trays__part_trays,  // fetch(index, &value) function pointer
    assign_function__Trays__part_trays,  // assign(index, value) function pointer
    resize_function__Trays__part_trays  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers Trays_message_members = {
  "aprs_interfaces::msg",  // message namespace
  "Trays",  // message name
  2,  // number of fields
  sizeof(aprs_interfaces::msg::Trays),
  Trays_message_member_array,  // message members
  Trays_init_function,  // function to initialize message memory (memory has to be allocated)
  Trays_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t Trays_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &Trays_message_members,
  get_message_typesupport_handle_function,
  &aprs_interfaces__msg__Trays__get_type_hash,
  &aprs_interfaces__msg__Trays__get_type_description,
  &aprs_interfaces__msg__Trays__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace aprs_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<aprs_interfaces::msg::Trays>()
{
  return &::aprs_interfaces::msg::rosidl_typesupport_introspection_cpp::Trays_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, msg, Trays)() {
  return &::aprs_interfaces::msg::rosidl_typesupport_introspection_cpp::Trays_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
