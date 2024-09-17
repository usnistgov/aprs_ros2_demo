// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from aprs_interfaces:srv/GeneratePlan.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__SRV__DETAIL__GENERATE_PLAN__STRUCT_H_
#define APRS_INTERFACES__SRV__DETAIL__GENERATE_PLAN__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/GeneratePlan in the package aprs_interfaces.
typedef struct aprs_interfaces__srv__GeneratePlan_Request
{
  uint8_t structure_needs_at_least_one_member;
} aprs_interfaces__srv__GeneratePlan_Request;

// Struct for a sequence of aprs_interfaces__srv__GeneratePlan_Request.
typedef struct aprs_interfaces__srv__GeneratePlan_Request__Sequence
{
  aprs_interfaces__srv__GeneratePlan_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__srv__GeneratePlan_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'plan'
// Member 'status'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/GeneratePlan in the package aprs_interfaces.
typedef struct aprs_interfaces__srv__GeneratePlan_Response
{
  bool success;
  rosidl_runtime_c__String plan;
  rosidl_runtime_c__String status;
} aprs_interfaces__srv__GeneratePlan_Response;

// Struct for a sequence of aprs_interfaces__srv__GeneratePlan_Response.
typedef struct aprs_interfaces__srv__GeneratePlan_Response__Sequence
{
  aprs_interfaces__srv__GeneratePlan_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__srv__GeneratePlan_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  aprs_interfaces__srv__GeneratePlan_Event__request__MAX_SIZE = 1
};
// response
enum
{
  aprs_interfaces__srv__GeneratePlan_Event__response__MAX_SIZE = 1
};

/// Struct defined in srv/GeneratePlan in the package aprs_interfaces.
typedef struct aprs_interfaces__srv__GeneratePlan_Event
{
  service_msgs__msg__ServiceEventInfo info;
  aprs_interfaces__srv__GeneratePlan_Request__Sequence request;
  aprs_interfaces__srv__GeneratePlan_Response__Sequence response;
} aprs_interfaces__srv__GeneratePlan_Event;

// Struct for a sequence of aprs_interfaces__srv__GeneratePlan_Event.
typedef struct aprs_interfaces__srv__GeneratePlan_Event__Sequence
{
  aprs_interfaces__srv__GeneratePlan_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__srv__GeneratePlan_Event__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // APRS_INTERFACES__SRV__DETAIL__GENERATE_PLAN__STRUCT_H_
