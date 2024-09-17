// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from aprs_interfaces:msg/Objects.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "aprs_interfaces/msg/detail/objects__rosidl_typesupport_introspection_c.h"
#include "aprs_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "aprs_interfaces/msg/detail/objects__functions.h"
#include "aprs_interfaces/msg/detail/objects__struct.h"


// Include directives for member types
// Member `objects`
#include "aprs_interfaces/msg/object.h"
// Member `objects`
#include "aprs_interfaces/msg/detail/object__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__Objects_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  aprs_interfaces__msg__Objects__init(message_memory);
}

void aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__Objects_fini_function(void * message_memory)
{
  aprs_interfaces__msg__Objects__fini(message_memory);
}

size_t aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__size_function__Objects__objects(
  const void * untyped_member)
{
  const aprs_interfaces__msg__Object__Sequence * member =
    (const aprs_interfaces__msg__Object__Sequence *)(untyped_member);
  return member->size;
}

const void * aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__get_const_function__Objects__objects(
  const void * untyped_member, size_t index)
{
  const aprs_interfaces__msg__Object__Sequence * member =
    (const aprs_interfaces__msg__Object__Sequence *)(untyped_member);
  return &member->data[index];
}

void * aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__get_function__Objects__objects(
  void * untyped_member, size_t index)
{
  aprs_interfaces__msg__Object__Sequence * member =
    (aprs_interfaces__msg__Object__Sequence *)(untyped_member);
  return &member->data[index];
}

void aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__fetch_function__Objects__objects(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const aprs_interfaces__msg__Object * item =
    ((const aprs_interfaces__msg__Object *)
    aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__get_const_function__Objects__objects(untyped_member, index));
  aprs_interfaces__msg__Object * value =
    (aprs_interfaces__msg__Object *)(untyped_value);
  *value = *item;
}

void aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__assign_function__Objects__objects(
  void * untyped_member, size_t index, const void * untyped_value)
{
  aprs_interfaces__msg__Object * item =
    ((aprs_interfaces__msg__Object *)
    aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__get_function__Objects__objects(untyped_member, index));
  const aprs_interfaces__msg__Object * value =
    (const aprs_interfaces__msg__Object *)(untyped_value);
  *item = *value;
}

bool aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__resize_function__Objects__objects(
  void * untyped_member, size_t size)
{
  aprs_interfaces__msg__Object__Sequence * member =
    (aprs_interfaces__msg__Object__Sequence *)(untyped_member);
  aprs_interfaces__msg__Object__Sequence__fini(member);
  return aprs_interfaces__msg__Object__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__Objects_message_member_array[1] = {
  {
    "objects",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aprs_interfaces__msg__Objects, objects),  // bytes offset in struct
    NULL,  // default value
    aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__size_function__Objects__objects,  // size() function pointer
    aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__get_const_function__Objects__objects,  // get_const(index) function pointer
    aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__get_function__Objects__objects,  // get(index) function pointer
    aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__fetch_function__Objects__objects,  // fetch(index, &value) function pointer
    aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__assign_function__Objects__objects,  // assign(index, value) function pointer
    aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__resize_function__Objects__objects  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__Objects_message_members = {
  "aprs_interfaces__msg",  // message namespace
  "Objects",  // message name
  1,  // number of fields
  sizeof(aprs_interfaces__msg__Objects),
  aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__Objects_message_member_array,  // message members
  aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__Objects_init_function,  // function to initialize message memory (memory has to be allocated)
  aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__Objects_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__Objects_message_type_support_handle = {
  0,
  &aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__Objects_message_members,
  get_message_typesupport_handle_function,
  &aprs_interfaces__msg__Objects__get_type_hash,
  &aprs_interfaces__msg__Objects__get_type_description,
  &aprs_interfaces__msg__Objects__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_aprs_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, msg, Objects)() {
  aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__Objects_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, msg, Object)();
  if (!aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__Objects_message_type_support_handle.typesupport_identifier) {
    aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__Objects_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &aprs_interfaces__msg__Objects__rosidl_typesupport_introspection_c__Objects_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
