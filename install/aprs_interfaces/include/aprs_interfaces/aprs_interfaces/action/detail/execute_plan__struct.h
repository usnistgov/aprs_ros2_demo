// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from aprs_interfaces:action/ExecutePlan.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__ACTION__DETAIL__EXECUTE_PLAN__STRUCT_H_
#define APRS_INTERFACES__ACTION__DETAIL__EXECUTE_PLAN__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in action/ExecutePlan in the package aprs_interfaces.
typedef struct aprs_interfaces__action__ExecutePlan_Goal
{
  uint8_t structure_needs_at_least_one_member;
} aprs_interfaces__action__ExecutePlan_Goal;

// Struct for a sequence of aprs_interfaces__action__ExecutePlan_Goal.
typedef struct aprs_interfaces__action__ExecutePlan_Goal__Sequence
{
  aprs_interfaces__action__ExecutePlan_Goal * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__action__ExecutePlan_Goal__Sequence;

// Constants defined in the message

/// Struct defined in action/ExecutePlan in the package aprs_interfaces.
typedef struct aprs_interfaces__action__ExecutePlan_Result
{
  uint8_t structure_needs_at_least_one_member;
} aprs_interfaces__action__ExecutePlan_Result;

// Struct for a sequence of aprs_interfaces__action__ExecutePlan_Result.
typedef struct aprs_interfaces__action__ExecutePlan_Result__Sequence
{
  aprs_interfaces__action__ExecutePlan_Result * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__action__ExecutePlan_Result__Sequence;

// Constants defined in the message

/// Struct defined in action/ExecutePlan in the package aprs_interfaces.
typedef struct aprs_interfaces__action__ExecutePlan_Feedback
{
  uint8_t structure_needs_at_least_one_member;
} aprs_interfaces__action__ExecutePlan_Feedback;

// Struct for a sequence of aprs_interfaces__action__ExecutePlan_Feedback.
typedef struct aprs_interfaces__action__ExecutePlan_Feedback__Sequence
{
  aprs_interfaces__action__ExecutePlan_Feedback * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__action__ExecutePlan_Feedback__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
#include "unique_identifier_msgs/msg/detail/uuid__struct.h"
// Member 'goal'
#include "aprs_interfaces/action/detail/execute_plan__struct.h"

/// Struct defined in action/ExecutePlan in the package aprs_interfaces.
typedef struct aprs_interfaces__action__ExecutePlan_SendGoal_Request
{
  unique_identifier_msgs__msg__UUID goal_id;
  aprs_interfaces__action__ExecutePlan_Goal goal;
} aprs_interfaces__action__ExecutePlan_SendGoal_Request;

// Struct for a sequence of aprs_interfaces__action__ExecutePlan_SendGoal_Request.
typedef struct aprs_interfaces__action__ExecutePlan_SendGoal_Request__Sequence
{
  aprs_interfaces__action__ExecutePlan_SendGoal_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__action__ExecutePlan_SendGoal_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__struct.h"

/// Struct defined in action/ExecutePlan in the package aprs_interfaces.
typedef struct aprs_interfaces__action__ExecutePlan_SendGoal_Response
{
  bool accepted;
  builtin_interfaces__msg__Time stamp;
} aprs_interfaces__action__ExecutePlan_SendGoal_Response;

// Struct for a sequence of aprs_interfaces__action__ExecutePlan_SendGoal_Response.
typedef struct aprs_interfaces__action__ExecutePlan_SendGoal_Response__Sequence
{
  aprs_interfaces__action__ExecutePlan_SendGoal_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__action__ExecutePlan_SendGoal_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  aprs_interfaces__action__ExecutePlan_SendGoal_Event__request__MAX_SIZE = 1
};
// response
enum
{
  aprs_interfaces__action__ExecutePlan_SendGoal_Event__response__MAX_SIZE = 1
};

/// Struct defined in action/ExecutePlan in the package aprs_interfaces.
typedef struct aprs_interfaces__action__ExecutePlan_SendGoal_Event
{
  service_msgs__msg__ServiceEventInfo info;
  aprs_interfaces__action__ExecutePlan_SendGoal_Request__Sequence request;
  aprs_interfaces__action__ExecutePlan_SendGoal_Response__Sequence response;
} aprs_interfaces__action__ExecutePlan_SendGoal_Event;

// Struct for a sequence of aprs_interfaces__action__ExecutePlan_SendGoal_Event.
typedef struct aprs_interfaces__action__ExecutePlan_SendGoal_Event__Sequence
{
  aprs_interfaces__action__ExecutePlan_SendGoal_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__action__ExecutePlan_SendGoal_Event__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__struct.h"

/// Struct defined in action/ExecutePlan in the package aprs_interfaces.
typedef struct aprs_interfaces__action__ExecutePlan_GetResult_Request
{
  unique_identifier_msgs__msg__UUID goal_id;
} aprs_interfaces__action__ExecutePlan_GetResult_Request;

// Struct for a sequence of aprs_interfaces__action__ExecutePlan_GetResult_Request.
typedef struct aprs_interfaces__action__ExecutePlan_GetResult_Request__Sequence
{
  aprs_interfaces__action__ExecutePlan_GetResult_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__action__ExecutePlan_GetResult_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'result'
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__struct.h"

/// Struct defined in action/ExecutePlan in the package aprs_interfaces.
typedef struct aprs_interfaces__action__ExecutePlan_GetResult_Response
{
  int8_t status;
  aprs_interfaces__action__ExecutePlan_Result result;
} aprs_interfaces__action__ExecutePlan_GetResult_Response;

// Struct for a sequence of aprs_interfaces__action__ExecutePlan_GetResult_Response.
typedef struct aprs_interfaces__action__ExecutePlan_GetResult_Response__Sequence
{
  aprs_interfaces__action__ExecutePlan_GetResult_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__action__ExecutePlan_GetResult_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
// already included above
// #include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  aprs_interfaces__action__ExecutePlan_GetResult_Event__request__MAX_SIZE = 1
};
// response
enum
{
  aprs_interfaces__action__ExecutePlan_GetResult_Event__response__MAX_SIZE = 1
};

/// Struct defined in action/ExecutePlan in the package aprs_interfaces.
typedef struct aprs_interfaces__action__ExecutePlan_GetResult_Event
{
  service_msgs__msg__ServiceEventInfo info;
  aprs_interfaces__action__ExecutePlan_GetResult_Request__Sequence request;
  aprs_interfaces__action__ExecutePlan_GetResult_Response__Sequence response;
} aprs_interfaces__action__ExecutePlan_GetResult_Event;

// Struct for a sequence of aprs_interfaces__action__ExecutePlan_GetResult_Event.
typedef struct aprs_interfaces__action__ExecutePlan_GetResult_Event__Sequence
{
  aprs_interfaces__action__ExecutePlan_GetResult_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__action__ExecutePlan_GetResult_Event__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__struct.h"
// Member 'feedback'
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__struct.h"

/// Struct defined in action/ExecutePlan in the package aprs_interfaces.
typedef struct aprs_interfaces__action__ExecutePlan_FeedbackMessage
{
  unique_identifier_msgs__msg__UUID goal_id;
  aprs_interfaces__action__ExecutePlan_Feedback feedback;
} aprs_interfaces__action__ExecutePlan_FeedbackMessage;

// Struct for a sequence of aprs_interfaces__action__ExecutePlan_FeedbackMessage.
typedef struct aprs_interfaces__action__ExecutePlan_FeedbackMessage__Sequence
{
  aprs_interfaces__action__ExecutePlan_FeedbackMessage * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__action__ExecutePlan_FeedbackMessage__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // APRS_INTERFACES__ACTION__DETAIL__EXECUTE_PLAN__STRUCT_H_
