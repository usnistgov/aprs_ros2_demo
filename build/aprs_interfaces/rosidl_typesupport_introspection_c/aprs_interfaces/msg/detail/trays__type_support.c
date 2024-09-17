// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from aprs_interfaces:msg/Trays.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "aprs_interfaces/msg/detail/trays__rosidl_typesupport_introspection_c.h"
#include "aprs_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "aprs_interfaces/msg/detail/trays__functions.h"
#include "aprs_interfaces/msg/detail/trays__struct.h"


// Include directives for member types
// Member `kit_trays`
// Member `part_trays`
#include "aprs_interfaces/msg/tray.h"
// Member `kit_trays`
// Member `part_trays`
#include "aprs_interfaces/msg/detail/tray__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__Trays_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  aprs_interfaces__msg__Trays__init(message_memory);
}

void aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__Trays_fini_function(void * message_memory)
{
  aprs_interfaces__msg__Trays__fini(message_memory);
}

size_t aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__size_function__Trays__kit_trays(
  const void * untyped_member)
{
  const aprs_interfaces__msg__Tray__Sequence * member =
    (const aprs_interfaces__msg__Tray__Sequence *)(untyped_member);
  return member->size;
}

const void * aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__get_const_function__Trays__kit_trays(
  const void * untyped_member, size_t index)
{
  const aprs_interfaces__msg__Tray__Sequence * member =
    (const aprs_interfaces__msg__Tray__Sequence *)(untyped_member);
  return &member->data[index];
}

void * aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__get_function__Trays__kit_trays(
  void * untyped_member, size_t index)
{
  aprs_interfaces__msg__Tray__Sequence * member =
    (aprs_interfaces__msg__Tray__Sequence *)(untyped_member);
  return &member->data[index];
}

void aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__fetch_function__Trays__kit_trays(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const aprs_interfaces__msg__Tray * item =
    ((const aprs_interfaces__msg__Tray *)
    aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__get_const_function__Trays__kit_trays(untyped_member, index));
  aprs_interfaces__msg__Tray * value =
    (aprs_interfaces__msg__Tray *)(untyped_value);
  *value = *item;
}

void aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__assign_function__Trays__kit_trays(
  void * untyped_member, size_t index, const void * untyped_value)
{
  aprs_interfaces__msg__Tray * item =
    ((aprs_interfaces__msg__Tray *)
    aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__get_function__Trays__kit_trays(untyped_member, index));
  const aprs_interfaces__msg__Tray * value =
    (const aprs_interfaces__msg__Tray *)(untyped_value);
  *item = *value;
}

bool aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__resize_function__Trays__kit_trays(
  void * untyped_member, size_t size)
{
  aprs_interfaces__msg__Tray__Sequence * member =
    (aprs_interfaces__msg__Tray__Sequence *)(untyped_member);
  aprs_interfaces__msg__Tray__Sequence__fini(member);
  return aprs_interfaces__msg__Tray__Sequence__init(member, size);
}

size_t aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__size_function__Trays__part_trays(
  const void * untyped_member)
{
  const aprs_interfaces__msg__Tray__Sequence * member =
    (const aprs_interfaces__msg__Tray__Sequence *)(untyped_member);
  return member->size;
}

const void * aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__get_const_function__Trays__part_trays(
  const void * untyped_member, size_t index)
{
  const aprs_interfaces__msg__Tray__Sequence * member =
    (const aprs_interfaces__msg__Tray__Sequence *)(untyped_member);
  return &member->data[index];
}

void * aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__get_function__Trays__part_trays(
  void * untyped_member, size_t index)
{
  aprs_interfaces__msg__Tray__Sequence * member =
    (aprs_interfaces__msg__Tray__Sequence *)(untyped_member);
  return &member->data[index];
}

void aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__fetch_function__Trays__part_trays(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const aprs_interfaces__msg__Tray * item =
    ((const aprs_interfaces__msg__Tray *)
    aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__get_const_function__Trays__part_trays(untyped_member, index));
  aprs_interfaces__msg__Tray * value =
    (aprs_interfaces__msg__Tray *)(untyped_value);
  *value = *item;
}

void aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__assign_function__Trays__part_trays(
  void * untyped_member, size_t index, const void * untyped_value)
{
  aprs_interfaces__msg__Tray * item =
    ((aprs_interfaces__msg__Tray *)
    aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__get_function__Trays__part_trays(untyped_member, index));
  const aprs_interfaces__msg__Tray * value =
    (const aprs_interfaces__msg__Tray *)(untyped_value);
  *item = *value;
}

bool aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__resize_function__Trays__part_trays(
  void * untyped_member, size_t size)
{
  aprs_interfaces__msg__Tray__Sequence * member =
    (aprs_interfaces__msg__Tray__Sequence *)(untyped_member);
  aprs_interfaces__msg__Tray__Sequence__fini(member);
  return aprs_interfaces__msg__Tray__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__Trays_message_member_array[2] = {
  {
    "kit_trays",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aprs_interfaces__msg__Trays, kit_trays),  // bytes offset in struct
    NULL,  // default value
    aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__size_function__Trays__kit_trays,  // size() function pointer
    aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__get_const_function__Trays__kit_trays,  // get_const(index) function pointer
    aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__get_function__Trays__kit_trays,  // get(index) function pointer
    aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__fetch_function__Trays__kit_trays,  // fetch(index, &value) function pointer
    aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__assign_function__Trays__kit_trays,  // assign(index, value) function pointer
    aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__resize_function__Trays__kit_trays  // resize(index) function pointer
  },
  {
    "part_trays",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aprs_interfaces__msg__Trays, part_trays),  // bytes offset in struct
    NULL,  // default value
    aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__size_function__Trays__part_trays,  // size() function pointer
    aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__get_const_function__Trays__part_trays,  // get_const(index) function pointer
    aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__get_function__Trays__part_trays,  // get(index) function pointer
    aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__fetch_function__Trays__part_trays,  // fetch(index, &value) function pointer
    aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__assign_function__Trays__part_trays,  // assign(index, value) function pointer
    aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__resize_function__Trays__part_trays  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__Trays_message_members = {
  "aprs_interfaces__msg",  // message namespace
  "Trays",  // message name
  2,  // number of fields
  sizeof(aprs_interfaces__msg__Trays),
  aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__Trays_message_member_array,  // message members
  aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__Trays_init_function,  // function to initialize message memory (memory has to be allocated)
  aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__Trays_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__Trays_message_type_support_handle = {
  0,
  &aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__Trays_message_members,
  get_message_typesupport_handle_function,
  &aprs_interfaces__msg__Trays__get_type_hash,
  &aprs_interfaces__msg__Trays__get_type_description,
  &aprs_interfaces__msg__Trays__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_aprs_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, msg, Trays)() {
  aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__Trays_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, msg, Tray)();
  aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__Trays_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, msg, Tray)();
  if (!aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__Trays_message_type_support_handle.typesupport_identifier) {
    aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__Trays_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &aprs_interfaces__msg__Trays__rosidl_typesupport_introspection_c__Trays_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
