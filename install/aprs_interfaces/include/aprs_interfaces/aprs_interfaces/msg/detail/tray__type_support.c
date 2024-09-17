// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from aprs_interfaces:msg/Tray.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "aprs_interfaces/msg/detail/tray__rosidl_typesupport_introspection_c.h"
#include "aprs_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "aprs_interfaces/msg/detail/tray__functions.h"
#include "aprs_interfaces/msg/detail/tray__struct.h"


// Include directives for member types
// Member `name`
#include "rosidl_runtime_c/string_functions.h"
// Member `slots`
#include "aprs_interfaces/msg/slot_info.h"
// Member `slots`
#include "aprs_interfaces/msg/detail/slot_info__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__Tray_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  aprs_interfaces__msg__Tray__init(message_memory);
}

void aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__Tray_fini_function(void * message_memory)
{
  aprs_interfaces__msg__Tray__fini(message_memory);
}

size_t aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__size_function__Tray__slots(
  const void * untyped_member)
{
  const aprs_interfaces__msg__SlotInfo__Sequence * member =
    (const aprs_interfaces__msg__SlotInfo__Sequence *)(untyped_member);
  return member->size;
}

const void * aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__get_const_function__Tray__slots(
  const void * untyped_member, size_t index)
{
  const aprs_interfaces__msg__SlotInfo__Sequence * member =
    (const aprs_interfaces__msg__SlotInfo__Sequence *)(untyped_member);
  return &member->data[index];
}

void * aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__get_function__Tray__slots(
  void * untyped_member, size_t index)
{
  aprs_interfaces__msg__SlotInfo__Sequence * member =
    (aprs_interfaces__msg__SlotInfo__Sequence *)(untyped_member);
  return &member->data[index];
}

void aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__fetch_function__Tray__slots(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const aprs_interfaces__msg__SlotInfo * item =
    ((const aprs_interfaces__msg__SlotInfo *)
    aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__get_const_function__Tray__slots(untyped_member, index));
  aprs_interfaces__msg__SlotInfo * value =
    (aprs_interfaces__msg__SlotInfo *)(untyped_value);
  *value = *item;
}

void aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__assign_function__Tray__slots(
  void * untyped_member, size_t index, const void * untyped_value)
{
  aprs_interfaces__msg__SlotInfo * item =
    ((aprs_interfaces__msg__SlotInfo *)
    aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__get_function__Tray__slots(untyped_member, index));
  const aprs_interfaces__msg__SlotInfo * value =
    (const aprs_interfaces__msg__SlotInfo *)(untyped_value);
  *item = *value;
}

bool aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__resize_function__Tray__slots(
  void * untyped_member, size_t size)
{
  aprs_interfaces__msg__SlotInfo__Sequence * member =
    (aprs_interfaces__msg__SlotInfo__Sequence *)(untyped_member);
  aprs_interfaces__msg__SlotInfo__Sequence__fini(member);
  return aprs_interfaces__msg__SlotInfo__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__Tray_message_member_array[3] = {
  {
    "identifier",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aprs_interfaces__msg__Tray, identifier),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "name",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aprs_interfaces__msg__Tray, name),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "slots",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aprs_interfaces__msg__Tray, slots),  // bytes offset in struct
    NULL,  // default value
    aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__size_function__Tray__slots,  // size() function pointer
    aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__get_const_function__Tray__slots,  // get_const(index) function pointer
    aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__get_function__Tray__slots,  // get(index) function pointer
    aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__fetch_function__Tray__slots,  // fetch(index, &value) function pointer
    aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__assign_function__Tray__slots,  // assign(index, value) function pointer
    aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__resize_function__Tray__slots  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__Tray_message_members = {
  "aprs_interfaces__msg",  // message namespace
  "Tray",  // message name
  3,  // number of fields
  sizeof(aprs_interfaces__msg__Tray),
  aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__Tray_message_member_array,  // message members
  aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__Tray_init_function,  // function to initialize message memory (memory has to be allocated)
  aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__Tray_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__Tray_message_type_support_handle = {
  0,
  &aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__Tray_message_members,
  get_message_typesupport_handle_function,
  &aprs_interfaces__msg__Tray__get_type_hash,
  &aprs_interfaces__msg__Tray__get_type_description,
  &aprs_interfaces__msg__Tray__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_aprs_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, msg, Tray)() {
  aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__Tray_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, msg, SlotInfo)();
  if (!aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__Tray_message_type_support_handle.typesupport_identifier) {
    aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__Tray_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &aprs_interfaces__msg__Tray__rosidl_typesupport_introspection_c__Tray_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
