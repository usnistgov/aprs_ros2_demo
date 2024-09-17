// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from aprs_interfaces:srv/GenerateInitState.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "aprs_interfaces/srv/detail/generate_init_state__rosidl_typesupport_introspection_c.h"
#include "aprs_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "aprs_interfaces/srv/detail/generate_init_state__functions.h"
#include "aprs_interfaces/srv/detail/generate_init_state__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void aprs_interfaces__srv__GenerateInitState_Request__rosidl_typesupport_introspection_c__GenerateInitState_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  aprs_interfaces__srv__GenerateInitState_Request__init(message_memory);
}

void aprs_interfaces__srv__GenerateInitState_Request__rosidl_typesupport_introspection_c__GenerateInitState_Request_fini_function(void * message_memory)
{
  aprs_interfaces__srv__GenerateInitState_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember aprs_interfaces__srv__GenerateInitState_Request__rosidl_typesupport_introspection_c__GenerateInitState_Request_message_member_array[1] = {
  {
    "structure_needs_at_least_one_member",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aprs_interfaces__srv__GenerateInitState_Request, structure_needs_at_least_one_member),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers aprs_interfaces__srv__GenerateInitState_Request__rosidl_typesupport_introspection_c__GenerateInitState_Request_message_members = {
  "aprs_interfaces__srv",  // message namespace
  "GenerateInitState_Request",  // message name
  1,  // number of fields
  sizeof(aprs_interfaces__srv__GenerateInitState_Request),
  aprs_interfaces__srv__GenerateInitState_Request__rosidl_typesupport_introspection_c__GenerateInitState_Request_message_member_array,  // message members
  aprs_interfaces__srv__GenerateInitState_Request__rosidl_typesupport_introspection_c__GenerateInitState_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  aprs_interfaces__srv__GenerateInitState_Request__rosidl_typesupport_introspection_c__GenerateInitState_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t aprs_interfaces__srv__GenerateInitState_Request__rosidl_typesupport_introspection_c__GenerateInitState_Request_message_type_support_handle = {
  0,
  &aprs_interfaces__srv__GenerateInitState_Request__rosidl_typesupport_introspection_c__GenerateInitState_Request_message_members,
  get_message_typesupport_handle_function,
  &aprs_interfaces__srv__GenerateInitState_Request__get_type_hash,
  &aprs_interfaces__srv__GenerateInitState_Request__get_type_description,
  &aprs_interfaces__srv__GenerateInitState_Request__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_aprs_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, srv, GenerateInitState_Request)() {
  if (!aprs_interfaces__srv__GenerateInitState_Request__rosidl_typesupport_introspection_c__GenerateInitState_Request_message_type_support_handle.typesupport_identifier) {
    aprs_interfaces__srv__GenerateInitState_Request__rosidl_typesupport_introspection_c__GenerateInitState_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &aprs_interfaces__srv__GenerateInitState_Request__rosidl_typesupport_introspection_c__GenerateInitState_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "aprs_interfaces/srv/detail/generate_init_state__rosidl_typesupport_introspection_c.h"
// already included above
// #include "aprs_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "aprs_interfaces/srv/detail/generate_init_state__functions.h"
// already included above
// #include "aprs_interfaces/srv/detail/generate_init_state__struct.h"


// Include directives for member types
// Member `status`
#include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void aprs_interfaces__srv__GenerateInitState_Response__rosidl_typesupport_introspection_c__GenerateInitState_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  aprs_interfaces__srv__GenerateInitState_Response__init(message_memory);
}

void aprs_interfaces__srv__GenerateInitState_Response__rosidl_typesupport_introspection_c__GenerateInitState_Response_fini_function(void * message_memory)
{
  aprs_interfaces__srv__GenerateInitState_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember aprs_interfaces__srv__GenerateInitState_Response__rosidl_typesupport_introspection_c__GenerateInitState_Response_message_member_array[2] = {
  {
    "success",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aprs_interfaces__srv__GenerateInitState_Response, success),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "status",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aprs_interfaces__srv__GenerateInitState_Response, status),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers aprs_interfaces__srv__GenerateInitState_Response__rosidl_typesupport_introspection_c__GenerateInitState_Response_message_members = {
  "aprs_interfaces__srv",  // message namespace
  "GenerateInitState_Response",  // message name
  2,  // number of fields
  sizeof(aprs_interfaces__srv__GenerateInitState_Response),
  aprs_interfaces__srv__GenerateInitState_Response__rosidl_typesupport_introspection_c__GenerateInitState_Response_message_member_array,  // message members
  aprs_interfaces__srv__GenerateInitState_Response__rosidl_typesupport_introspection_c__GenerateInitState_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  aprs_interfaces__srv__GenerateInitState_Response__rosidl_typesupport_introspection_c__GenerateInitState_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t aprs_interfaces__srv__GenerateInitState_Response__rosidl_typesupport_introspection_c__GenerateInitState_Response_message_type_support_handle = {
  0,
  &aprs_interfaces__srv__GenerateInitState_Response__rosidl_typesupport_introspection_c__GenerateInitState_Response_message_members,
  get_message_typesupport_handle_function,
  &aprs_interfaces__srv__GenerateInitState_Response__get_type_hash,
  &aprs_interfaces__srv__GenerateInitState_Response__get_type_description,
  &aprs_interfaces__srv__GenerateInitState_Response__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_aprs_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, srv, GenerateInitState_Response)() {
  if (!aprs_interfaces__srv__GenerateInitState_Response__rosidl_typesupport_introspection_c__GenerateInitState_Response_message_type_support_handle.typesupport_identifier) {
    aprs_interfaces__srv__GenerateInitState_Response__rosidl_typesupport_introspection_c__GenerateInitState_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &aprs_interfaces__srv__GenerateInitState_Response__rosidl_typesupport_introspection_c__GenerateInitState_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "aprs_interfaces/srv/detail/generate_init_state__rosidl_typesupport_introspection_c.h"
// already included above
// #include "aprs_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "aprs_interfaces/srv/detail/generate_init_state__functions.h"
// already included above
// #include "aprs_interfaces/srv/detail/generate_init_state__struct.h"


// Include directives for member types
// Member `info`
#include "service_msgs/msg/service_event_info.h"
// Member `info`
#include "service_msgs/msg/detail/service_event_info__rosidl_typesupport_introspection_c.h"
// Member `request`
// Member `response`
#include "aprs_interfaces/srv/generate_init_state.h"
// Member `request`
// Member `response`
// already included above
// #include "aprs_interfaces/srv/detail/generate_init_state__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__GenerateInitState_Event_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  aprs_interfaces__srv__GenerateInitState_Event__init(message_memory);
}

void aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__GenerateInitState_Event_fini_function(void * message_memory)
{
  aprs_interfaces__srv__GenerateInitState_Event__fini(message_memory);
}

size_t aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__size_function__GenerateInitState_Event__request(
  const void * untyped_member)
{
  const aprs_interfaces__srv__GenerateInitState_Request__Sequence * member =
    (const aprs_interfaces__srv__GenerateInitState_Request__Sequence *)(untyped_member);
  return member->size;
}

const void * aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__get_const_function__GenerateInitState_Event__request(
  const void * untyped_member, size_t index)
{
  const aprs_interfaces__srv__GenerateInitState_Request__Sequence * member =
    (const aprs_interfaces__srv__GenerateInitState_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void * aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__get_function__GenerateInitState_Event__request(
  void * untyped_member, size_t index)
{
  aprs_interfaces__srv__GenerateInitState_Request__Sequence * member =
    (aprs_interfaces__srv__GenerateInitState_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__fetch_function__GenerateInitState_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const aprs_interfaces__srv__GenerateInitState_Request * item =
    ((const aprs_interfaces__srv__GenerateInitState_Request *)
    aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__get_const_function__GenerateInitState_Event__request(untyped_member, index));
  aprs_interfaces__srv__GenerateInitState_Request * value =
    (aprs_interfaces__srv__GenerateInitState_Request *)(untyped_value);
  *value = *item;
}

void aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__assign_function__GenerateInitState_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  aprs_interfaces__srv__GenerateInitState_Request * item =
    ((aprs_interfaces__srv__GenerateInitState_Request *)
    aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__get_function__GenerateInitState_Event__request(untyped_member, index));
  const aprs_interfaces__srv__GenerateInitState_Request * value =
    (const aprs_interfaces__srv__GenerateInitState_Request *)(untyped_value);
  *item = *value;
}

bool aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__resize_function__GenerateInitState_Event__request(
  void * untyped_member, size_t size)
{
  aprs_interfaces__srv__GenerateInitState_Request__Sequence * member =
    (aprs_interfaces__srv__GenerateInitState_Request__Sequence *)(untyped_member);
  aprs_interfaces__srv__GenerateInitState_Request__Sequence__fini(member);
  return aprs_interfaces__srv__GenerateInitState_Request__Sequence__init(member, size);
}

size_t aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__size_function__GenerateInitState_Event__response(
  const void * untyped_member)
{
  const aprs_interfaces__srv__GenerateInitState_Response__Sequence * member =
    (const aprs_interfaces__srv__GenerateInitState_Response__Sequence *)(untyped_member);
  return member->size;
}

const void * aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__get_const_function__GenerateInitState_Event__response(
  const void * untyped_member, size_t index)
{
  const aprs_interfaces__srv__GenerateInitState_Response__Sequence * member =
    (const aprs_interfaces__srv__GenerateInitState_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void * aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__get_function__GenerateInitState_Event__response(
  void * untyped_member, size_t index)
{
  aprs_interfaces__srv__GenerateInitState_Response__Sequence * member =
    (aprs_interfaces__srv__GenerateInitState_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__fetch_function__GenerateInitState_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const aprs_interfaces__srv__GenerateInitState_Response * item =
    ((const aprs_interfaces__srv__GenerateInitState_Response *)
    aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__get_const_function__GenerateInitState_Event__response(untyped_member, index));
  aprs_interfaces__srv__GenerateInitState_Response * value =
    (aprs_interfaces__srv__GenerateInitState_Response *)(untyped_value);
  *value = *item;
}

void aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__assign_function__GenerateInitState_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  aprs_interfaces__srv__GenerateInitState_Response * item =
    ((aprs_interfaces__srv__GenerateInitState_Response *)
    aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__get_function__GenerateInitState_Event__response(untyped_member, index));
  const aprs_interfaces__srv__GenerateInitState_Response * value =
    (const aprs_interfaces__srv__GenerateInitState_Response *)(untyped_value);
  *item = *value;
}

bool aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__resize_function__GenerateInitState_Event__response(
  void * untyped_member, size_t size)
{
  aprs_interfaces__srv__GenerateInitState_Response__Sequence * member =
    (aprs_interfaces__srv__GenerateInitState_Response__Sequence *)(untyped_member);
  aprs_interfaces__srv__GenerateInitState_Response__Sequence__fini(member);
  return aprs_interfaces__srv__GenerateInitState_Response__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__GenerateInitState_Event_message_member_array[3] = {
  {
    "info",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(aprs_interfaces__srv__GenerateInitState_Event, info),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "request",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(aprs_interfaces__srv__GenerateInitState_Event, request),  // bytes offset in struct
    NULL,  // default value
    aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__size_function__GenerateInitState_Event__request,  // size() function pointer
    aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__get_const_function__GenerateInitState_Event__request,  // get_const(index) function pointer
    aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__get_function__GenerateInitState_Event__request,  // get(index) function pointer
    aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__fetch_function__GenerateInitState_Event__request,  // fetch(index, &value) function pointer
    aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__assign_function__GenerateInitState_Event__request,  // assign(index, value) function pointer
    aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__resize_function__GenerateInitState_Event__request  // resize(index) function pointer
  },
  {
    "response",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(aprs_interfaces__srv__GenerateInitState_Event, response),  // bytes offset in struct
    NULL,  // default value
    aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__size_function__GenerateInitState_Event__response,  // size() function pointer
    aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__get_const_function__GenerateInitState_Event__response,  // get_const(index) function pointer
    aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__get_function__GenerateInitState_Event__response,  // get(index) function pointer
    aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__fetch_function__GenerateInitState_Event__response,  // fetch(index, &value) function pointer
    aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__assign_function__GenerateInitState_Event__response,  // assign(index, value) function pointer
    aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__resize_function__GenerateInitState_Event__response  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__GenerateInitState_Event_message_members = {
  "aprs_interfaces__srv",  // message namespace
  "GenerateInitState_Event",  // message name
  3,  // number of fields
  sizeof(aprs_interfaces__srv__GenerateInitState_Event),
  aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__GenerateInitState_Event_message_member_array,  // message members
  aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__GenerateInitState_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__GenerateInitState_Event_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__GenerateInitState_Event_message_type_support_handle = {
  0,
  &aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__GenerateInitState_Event_message_members,
  get_message_typesupport_handle_function,
  &aprs_interfaces__srv__GenerateInitState_Event__get_type_hash,
  &aprs_interfaces__srv__GenerateInitState_Event__get_type_description,
  &aprs_interfaces__srv__GenerateInitState_Event__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_aprs_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, srv, GenerateInitState_Event)() {
  aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__GenerateInitState_Event_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, service_msgs, msg, ServiceEventInfo)();
  aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__GenerateInitState_Event_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, srv, GenerateInitState_Request)();
  aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__GenerateInitState_Event_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, srv, GenerateInitState_Response)();
  if (!aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__GenerateInitState_Event_message_type_support_handle.typesupport_identifier) {
    aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__GenerateInitState_Event_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__GenerateInitState_Event_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "aprs_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "aprs_interfaces/srv/detail/generate_init_state__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers aprs_interfaces__srv__detail__generate_init_state__rosidl_typesupport_introspection_c__GenerateInitState_service_members = {
  "aprs_interfaces__srv",  // service namespace
  "GenerateInitState",  // service name
  // these two fields are initialized below on the first access
  NULL,  // request message
  // aprs_interfaces__srv__detail__generate_init_state__rosidl_typesupport_introspection_c__GenerateInitState_Request_message_type_support_handle,
  NULL,  // response message
  // aprs_interfaces__srv__detail__generate_init_state__rosidl_typesupport_introspection_c__GenerateInitState_Response_message_type_support_handle
  NULL  // event_message
  // aprs_interfaces__srv__detail__generate_init_state__rosidl_typesupport_introspection_c__GenerateInitState_Response_message_type_support_handle
};


static rosidl_service_type_support_t aprs_interfaces__srv__detail__generate_init_state__rosidl_typesupport_introspection_c__GenerateInitState_service_type_support_handle = {
  0,
  &aprs_interfaces__srv__detail__generate_init_state__rosidl_typesupport_introspection_c__GenerateInitState_service_members,
  get_service_typesupport_handle_function,
  &aprs_interfaces__srv__GenerateInitState_Request__rosidl_typesupport_introspection_c__GenerateInitState_Request_message_type_support_handle,
  &aprs_interfaces__srv__GenerateInitState_Response__rosidl_typesupport_introspection_c__GenerateInitState_Response_message_type_support_handle,
  &aprs_interfaces__srv__GenerateInitState_Event__rosidl_typesupport_introspection_c__GenerateInitState_Event_message_type_support_handle,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    aprs_interfaces,
    srv,
    GenerateInitState
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    aprs_interfaces,
    srv,
    GenerateInitState
  ),
  &aprs_interfaces__srv__GenerateInitState__get_type_hash,
  &aprs_interfaces__srv__GenerateInitState__get_type_description,
  &aprs_interfaces__srv__GenerateInitState__get_type_description_sources,
};

// Forward declaration of request/response type support functions
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, srv, GenerateInitState_Request)();

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, srv, GenerateInitState_Response)();

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, srv, GenerateInitState_Event)();

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_aprs_interfaces
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, srv, GenerateInitState)() {
  if (!aprs_interfaces__srv__detail__generate_init_state__rosidl_typesupport_introspection_c__GenerateInitState_service_type_support_handle.typesupport_identifier) {
    aprs_interfaces__srv__detail__generate_init_state__rosidl_typesupport_introspection_c__GenerateInitState_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)aprs_interfaces__srv__detail__generate_init_state__rosidl_typesupport_introspection_c__GenerateInitState_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, srv, GenerateInitState_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, srv, GenerateInitState_Response)()->data;
  }
  if (!service_members->event_members_) {
    service_members->event_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, aprs_interfaces, srv, GenerateInitState_Event)()->data;
  }

  return &aprs_interfaces__srv__detail__generate_init_state__rosidl_typesupport_introspection_c__GenerateInitState_service_type_support_handle;
}
