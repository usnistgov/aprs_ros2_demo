// generated from rosidl_typesupport_cpp/resource/idl__type_support.cpp.em
// with input from aprs_interfaces:action/ExecutePlan.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "aprs_interfaces/action/detail/execute_plan__functions.h"
#include "aprs_interfaces/action/detail/execute_plan__struct.hpp"
#include "rosidl_typesupport_cpp/identifier.hpp"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
#include "rosidl_typesupport_cpp/visibility_control.h"
#include "rosidl_typesupport_interface/macros.h"

namespace aprs_interfaces
{

namespace action
{

namespace rosidl_typesupport_cpp
{

typedef struct _ExecutePlan_Goal_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ExecutePlan_Goal_type_support_ids_t;

static const _ExecutePlan_Goal_type_support_ids_t _ExecutePlan_Goal_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _ExecutePlan_Goal_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ExecutePlan_Goal_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ExecutePlan_Goal_type_support_symbol_names_t _ExecutePlan_Goal_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, aprs_interfaces, action, ExecutePlan_Goal)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, action, ExecutePlan_Goal)),
  }
};

typedef struct _ExecutePlan_Goal_type_support_data_t
{
  void * data[2];
} _ExecutePlan_Goal_type_support_data_t;

static _ExecutePlan_Goal_type_support_data_t _ExecutePlan_Goal_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ExecutePlan_Goal_message_typesupport_map = {
  2,
  "aprs_interfaces",
  &_ExecutePlan_Goal_message_typesupport_ids.typesupport_identifier[0],
  &_ExecutePlan_Goal_message_typesupport_symbol_names.symbol_name[0],
  &_ExecutePlan_Goal_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t ExecutePlan_Goal_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ExecutePlan_Goal_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &aprs_interfaces__action__ExecutePlan_Goal__get_type_hash,
  &aprs_interfaces__action__ExecutePlan_Goal__get_type_description,
  &aprs_interfaces__action__ExecutePlan_Goal__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace action

}  // namespace aprs_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_Goal>()
{
  return &::aprs_interfaces::action::rosidl_typesupport_cpp::ExecutePlan_Goal_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, aprs_interfaces, action, ExecutePlan_Goal)() {
  return get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_Goal>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__functions.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace aprs_interfaces
{

namespace action
{

namespace rosidl_typesupport_cpp
{

typedef struct _ExecutePlan_Result_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ExecutePlan_Result_type_support_ids_t;

static const _ExecutePlan_Result_type_support_ids_t _ExecutePlan_Result_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _ExecutePlan_Result_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ExecutePlan_Result_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ExecutePlan_Result_type_support_symbol_names_t _ExecutePlan_Result_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, aprs_interfaces, action, ExecutePlan_Result)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, action, ExecutePlan_Result)),
  }
};

typedef struct _ExecutePlan_Result_type_support_data_t
{
  void * data[2];
} _ExecutePlan_Result_type_support_data_t;

static _ExecutePlan_Result_type_support_data_t _ExecutePlan_Result_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ExecutePlan_Result_message_typesupport_map = {
  2,
  "aprs_interfaces",
  &_ExecutePlan_Result_message_typesupport_ids.typesupport_identifier[0],
  &_ExecutePlan_Result_message_typesupport_symbol_names.symbol_name[0],
  &_ExecutePlan_Result_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t ExecutePlan_Result_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ExecutePlan_Result_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &aprs_interfaces__action__ExecutePlan_Result__get_type_hash,
  &aprs_interfaces__action__ExecutePlan_Result__get_type_description,
  &aprs_interfaces__action__ExecutePlan_Result__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace action

}  // namespace aprs_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_Result>()
{
  return &::aprs_interfaces::action::rosidl_typesupport_cpp::ExecutePlan_Result_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, aprs_interfaces, action, ExecutePlan_Result)() {
  return get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_Result>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__functions.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace aprs_interfaces
{

namespace action
{

namespace rosidl_typesupport_cpp
{

typedef struct _ExecutePlan_Feedback_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ExecutePlan_Feedback_type_support_ids_t;

static const _ExecutePlan_Feedback_type_support_ids_t _ExecutePlan_Feedback_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _ExecutePlan_Feedback_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ExecutePlan_Feedback_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ExecutePlan_Feedback_type_support_symbol_names_t _ExecutePlan_Feedback_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, aprs_interfaces, action, ExecutePlan_Feedback)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, action, ExecutePlan_Feedback)),
  }
};

typedef struct _ExecutePlan_Feedback_type_support_data_t
{
  void * data[2];
} _ExecutePlan_Feedback_type_support_data_t;

static _ExecutePlan_Feedback_type_support_data_t _ExecutePlan_Feedback_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ExecutePlan_Feedback_message_typesupport_map = {
  2,
  "aprs_interfaces",
  &_ExecutePlan_Feedback_message_typesupport_ids.typesupport_identifier[0],
  &_ExecutePlan_Feedback_message_typesupport_symbol_names.symbol_name[0],
  &_ExecutePlan_Feedback_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t ExecutePlan_Feedback_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ExecutePlan_Feedback_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &aprs_interfaces__action__ExecutePlan_Feedback__get_type_hash,
  &aprs_interfaces__action__ExecutePlan_Feedback__get_type_description,
  &aprs_interfaces__action__ExecutePlan_Feedback__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace action

}  // namespace aprs_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_Feedback>()
{
  return &::aprs_interfaces::action::rosidl_typesupport_cpp::ExecutePlan_Feedback_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, aprs_interfaces, action, ExecutePlan_Feedback)() {
  return get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_Feedback>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__functions.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace aprs_interfaces
{

namespace action
{

namespace rosidl_typesupport_cpp
{

typedef struct _ExecutePlan_SendGoal_Request_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ExecutePlan_SendGoal_Request_type_support_ids_t;

static const _ExecutePlan_SendGoal_Request_type_support_ids_t _ExecutePlan_SendGoal_Request_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _ExecutePlan_SendGoal_Request_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ExecutePlan_SendGoal_Request_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ExecutePlan_SendGoal_Request_type_support_symbol_names_t _ExecutePlan_SendGoal_Request_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, aprs_interfaces, action, ExecutePlan_SendGoal_Request)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, action, ExecutePlan_SendGoal_Request)),
  }
};

typedef struct _ExecutePlan_SendGoal_Request_type_support_data_t
{
  void * data[2];
} _ExecutePlan_SendGoal_Request_type_support_data_t;

static _ExecutePlan_SendGoal_Request_type_support_data_t _ExecutePlan_SendGoal_Request_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ExecutePlan_SendGoal_Request_message_typesupport_map = {
  2,
  "aprs_interfaces",
  &_ExecutePlan_SendGoal_Request_message_typesupport_ids.typesupport_identifier[0],
  &_ExecutePlan_SendGoal_Request_message_typesupport_symbol_names.symbol_name[0],
  &_ExecutePlan_SendGoal_Request_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t ExecutePlan_SendGoal_Request_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ExecutePlan_SendGoal_Request_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &aprs_interfaces__action__ExecutePlan_SendGoal_Request__get_type_hash,
  &aprs_interfaces__action__ExecutePlan_SendGoal_Request__get_type_description,
  &aprs_interfaces__action__ExecutePlan_SendGoal_Request__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace action

}  // namespace aprs_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_SendGoal_Request>()
{
  return &::aprs_interfaces::action::rosidl_typesupport_cpp::ExecutePlan_SendGoal_Request_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, aprs_interfaces, action, ExecutePlan_SendGoal_Request)() {
  return get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_SendGoal_Request>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__functions.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace aprs_interfaces
{

namespace action
{

namespace rosidl_typesupport_cpp
{

typedef struct _ExecutePlan_SendGoal_Response_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ExecutePlan_SendGoal_Response_type_support_ids_t;

static const _ExecutePlan_SendGoal_Response_type_support_ids_t _ExecutePlan_SendGoal_Response_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _ExecutePlan_SendGoal_Response_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ExecutePlan_SendGoal_Response_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ExecutePlan_SendGoal_Response_type_support_symbol_names_t _ExecutePlan_SendGoal_Response_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, aprs_interfaces, action, ExecutePlan_SendGoal_Response)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, action, ExecutePlan_SendGoal_Response)),
  }
};

typedef struct _ExecutePlan_SendGoal_Response_type_support_data_t
{
  void * data[2];
} _ExecutePlan_SendGoal_Response_type_support_data_t;

static _ExecutePlan_SendGoal_Response_type_support_data_t _ExecutePlan_SendGoal_Response_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ExecutePlan_SendGoal_Response_message_typesupport_map = {
  2,
  "aprs_interfaces",
  &_ExecutePlan_SendGoal_Response_message_typesupport_ids.typesupport_identifier[0],
  &_ExecutePlan_SendGoal_Response_message_typesupport_symbol_names.symbol_name[0],
  &_ExecutePlan_SendGoal_Response_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t ExecutePlan_SendGoal_Response_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ExecutePlan_SendGoal_Response_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &aprs_interfaces__action__ExecutePlan_SendGoal_Response__get_type_hash,
  &aprs_interfaces__action__ExecutePlan_SendGoal_Response__get_type_description,
  &aprs_interfaces__action__ExecutePlan_SendGoal_Response__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace action

}  // namespace aprs_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_SendGoal_Response>()
{
  return &::aprs_interfaces::action::rosidl_typesupport_cpp::ExecutePlan_SendGoal_Response_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, aprs_interfaces, action, ExecutePlan_SendGoal_Response)() {
  return get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_SendGoal_Response>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__functions.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace aprs_interfaces
{

namespace action
{

namespace rosidl_typesupport_cpp
{

typedef struct _ExecutePlan_SendGoal_Event_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ExecutePlan_SendGoal_Event_type_support_ids_t;

static const _ExecutePlan_SendGoal_Event_type_support_ids_t _ExecutePlan_SendGoal_Event_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _ExecutePlan_SendGoal_Event_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ExecutePlan_SendGoal_Event_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ExecutePlan_SendGoal_Event_type_support_symbol_names_t _ExecutePlan_SendGoal_Event_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, aprs_interfaces, action, ExecutePlan_SendGoal_Event)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, action, ExecutePlan_SendGoal_Event)),
  }
};

typedef struct _ExecutePlan_SendGoal_Event_type_support_data_t
{
  void * data[2];
} _ExecutePlan_SendGoal_Event_type_support_data_t;

static _ExecutePlan_SendGoal_Event_type_support_data_t _ExecutePlan_SendGoal_Event_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ExecutePlan_SendGoal_Event_message_typesupport_map = {
  2,
  "aprs_interfaces",
  &_ExecutePlan_SendGoal_Event_message_typesupport_ids.typesupport_identifier[0],
  &_ExecutePlan_SendGoal_Event_message_typesupport_symbol_names.symbol_name[0],
  &_ExecutePlan_SendGoal_Event_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t ExecutePlan_SendGoal_Event_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ExecutePlan_SendGoal_Event_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &aprs_interfaces__action__ExecutePlan_SendGoal_Event__get_type_hash,
  &aprs_interfaces__action__ExecutePlan_SendGoal_Event__get_type_description,
  &aprs_interfaces__action__ExecutePlan_SendGoal_Event__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace action

}  // namespace aprs_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_SendGoal_Event>()
{
  return &::aprs_interfaces::action::rosidl_typesupport_cpp::ExecutePlan_SendGoal_Event_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, aprs_interfaces, action, ExecutePlan_SendGoal_Event)() {
  return get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_SendGoal_Event>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
#include "rosidl_runtime_c/service_type_support_struct.h"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_cpp/service_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace aprs_interfaces
{

namespace action
{

namespace rosidl_typesupport_cpp
{

typedef struct _ExecutePlan_SendGoal_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ExecutePlan_SendGoal_type_support_ids_t;

static const _ExecutePlan_SendGoal_type_support_ids_t _ExecutePlan_SendGoal_service_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _ExecutePlan_SendGoal_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ExecutePlan_SendGoal_type_support_symbol_names_t;
#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ExecutePlan_SendGoal_type_support_symbol_names_t _ExecutePlan_SendGoal_service_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, aprs_interfaces, action, ExecutePlan_SendGoal)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, action, ExecutePlan_SendGoal)),
  }
};

typedef struct _ExecutePlan_SendGoal_type_support_data_t
{
  void * data[2];
} _ExecutePlan_SendGoal_type_support_data_t;

static _ExecutePlan_SendGoal_type_support_data_t _ExecutePlan_SendGoal_service_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ExecutePlan_SendGoal_service_typesupport_map = {
  2,
  "aprs_interfaces",
  &_ExecutePlan_SendGoal_service_typesupport_ids.typesupport_identifier[0],
  &_ExecutePlan_SendGoal_service_typesupport_symbol_names.symbol_name[0],
  &_ExecutePlan_SendGoal_service_typesupport_data.data[0],
};

static const rosidl_service_type_support_t ExecutePlan_SendGoal_service_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ExecutePlan_SendGoal_service_typesupport_map),
  ::rosidl_typesupport_cpp::get_service_typesupport_handle_function,
  ::rosidl_typesupport_cpp::get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_SendGoal_Request>(),
  ::rosidl_typesupport_cpp::get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_SendGoal_Response>(),
  ::rosidl_typesupport_cpp::get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_SendGoal_Event>(),
  &::rosidl_typesupport_cpp::service_create_event_message<aprs_interfaces::action::ExecutePlan_SendGoal>,
  &::rosidl_typesupport_cpp::service_destroy_event_message<aprs_interfaces::action::ExecutePlan_SendGoal>,
  &aprs_interfaces__action__ExecutePlan_SendGoal__get_type_hash,
  &aprs_interfaces__action__ExecutePlan_SendGoal__get_type_description,
  &aprs_interfaces__action__ExecutePlan_SendGoal__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace action

}  // namespace aprs_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_service_type_support_t *
get_service_type_support_handle<aprs_interfaces::action::ExecutePlan_SendGoal>()
{
  return &::aprs_interfaces::action::rosidl_typesupport_cpp::ExecutePlan_SendGoal_service_type_support_handle;
}

}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__functions.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace aprs_interfaces
{

namespace action
{

namespace rosidl_typesupport_cpp
{

typedef struct _ExecutePlan_GetResult_Request_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ExecutePlan_GetResult_Request_type_support_ids_t;

static const _ExecutePlan_GetResult_Request_type_support_ids_t _ExecutePlan_GetResult_Request_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _ExecutePlan_GetResult_Request_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ExecutePlan_GetResult_Request_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ExecutePlan_GetResult_Request_type_support_symbol_names_t _ExecutePlan_GetResult_Request_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, aprs_interfaces, action, ExecutePlan_GetResult_Request)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, action, ExecutePlan_GetResult_Request)),
  }
};

typedef struct _ExecutePlan_GetResult_Request_type_support_data_t
{
  void * data[2];
} _ExecutePlan_GetResult_Request_type_support_data_t;

static _ExecutePlan_GetResult_Request_type_support_data_t _ExecutePlan_GetResult_Request_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ExecutePlan_GetResult_Request_message_typesupport_map = {
  2,
  "aprs_interfaces",
  &_ExecutePlan_GetResult_Request_message_typesupport_ids.typesupport_identifier[0],
  &_ExecutePlan_GetResult_Request_message_typesupport_symbol_names.symbol_name[0],
  &_ExecutePlan_GetResult_Request_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t ExecutePlan_GetResult_Request_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ExecutePlan_GetResult_Request_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &aprs_interfaces__action__ExecutePlan_GetResult_Request__get_type_hash,
  &aprs_interfaces__action__ExecutePlan_GetResult_Request__get_type_description,
  &aprs_interfaces__action__ExecutePlan_GetResult_Request__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace action

}  // namespace aprs_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_GetResult_Request>()
{
  return &::aprs_interfaces::action::rosidl_typesupport_cpp::ExecutePlan_GetResult_Request_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, aprs_interfaces, action, ExecutePlan_GetResult_Request)() {
  return get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_GetResult_Request>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__functions.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace aprs_interfaces
{

namespace action
{

namespace rosidl_typesupport_cpp
{

typedef struct _ExecutePlan_GetResult_Response_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ExecutePlan_GetResult_Response_type_support_ids_t;

static const _ExecutePlan_GetResult_Response_type_support_ids_t _ExecutePlan_GetResult_Response_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _ExecutePlan_GetResult_Response_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ExecutePlan_GetResult_Response_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ExecutePlan_GetResult_Response_type_support_symbol_names_t _ExecutePlan_GetResult_Response_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, aprs_interfaces, action, ExecutePlan_GetResult_Response)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, action, ExecutePlan_GetResult_Response)),
  }
};

typedef struct _ExecutePlan_GetResult_Response_type_support_data_t
{
  void * data[2];
} _ExecutePlan_GetResult_Response_type_support_data_t;

static _ExecutePlan_GetResult_Response_type_support_data_t _ExecutePlan_GetResult_Response_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ExecutePlan_GetResult_Response_message_typesupport_map = {
  2,
  "aprs_interfaces",
  &_ExecutePlan_GetResult_Response_message_typesupport_ids.typesupport_identifier[0],
  &_ExecutePlan_GetResult_Response_message_typesupport_symbol_names.symbol_name[0],
  &_ExecutePlan_GetResult_Response_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t ExecutePlan_GetResult_Response_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ExecutePlan_GetResult_Response_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &aprs_interfaces__action__ExecutePlan_GetResult_Response__get_type_hash,
  &aprs_interfaces__action__ExecutePlan_GetResult_Response__get_type_description,
  &aprs_interfaces__action__ExecutePlan_GetResult_Response__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace action

}  // namespace aprs_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_GetResult_Response>()
{
  return &::aprs_interfaces::action::rosidl_typesupport_cpp::ExecutePlan_GetResult_Response_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, aprs_interfaces, action, ExecutePlan_GetResult_Response)() {
  return get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_GetResult_Response>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__functions.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace aprs_interfaces
{

namespace action
{

namespace rosidl_typesupport_cpp
{

typedef struct _ExecutePlan_GetResult_Event_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ExecutePlan_GetResult_Event_type_support_ids_t;

static const _ExecutePlan_GetResult_Event_type_support_ids_t _ExecutePlan_GetResult_Event_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _ExecutePlan_GetResult_Event_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ExecutePlan_GetResult_Event_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ExecutePlan_GetResult_Event_type_support_symbol_names_t _ExecutePlan_GetResult_Event_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, aprs_interfaces, action, ExecutePlan_GetResult_Event)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, action, ExecutePlan_GetResult_Event)),
  }
};

typedef struct _ExecutePlan_GetResult_Event_type_support_data_t
{
  void * data[2];
} _ExecutePlan_GetResult_Event_type_support_data_t;

static _ExecutePlan_GetResult_Event_type_support_data_t _ExecutePlan_GetResult_Event_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ExecutePlan_GetResult_Event_message_typesupport_map = {
  2,
  "aprs_interfaces",
  &_ExecutePlan_GetResult_Event_message_typesupport_ids.typesupport_identifier[0],
  &_ExecutePlan_GetResult_Event_message_typesupport_symbol_names.symbol_name[0],
  &_ExecutePlan_GetResult_Event_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t ExecutePlan_GetResult_Event_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ExecutePlan_GetResult_Event_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &aprs_interfaces__action__ExecutePlan_GetResult_Event__get_type_hash,
  &aprs_interfaces__action__ExecutePlan_GetResult_Event__get_type_description,
  &aprs_interfaces__action__ExecutePlan_GetResult_Event__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace action

}  // namespace aprs_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_GetResult_Event>()
{
  return &::aprs_interfaces::action::rosidl_typesupport_cpp::ExecutePlan_GetResult_Event_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, aprs_interfaces, action, ExecutePlan_GetResult_Event)() {
  return get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_GetResult_Event>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/service_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace aprs_interfaces
{

namespace action
{

namespace rosidl_typesupport_cpp
{

typedef struct _ExecutePlan_GetResult_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ExecutePlan_GetResult_type_support_ids_t;

static const _ExecutePlan_GetResult_type_support_ids_t _ExecutePlan_GetResult_service_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _ExecutePlan_GetResult_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ExecutePlan_GetResult_type_support_symbol_names_t;
#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ExecutePlan_GetResult_type_support_symbol_names_t _ExecutePlan_GetResult_service_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, aprs_interfaces, action, ExecutePlan_GetResult)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, action, ExecutePlan_GetResult)),
  }
};

typedef struct _ExecutePlan_GetResult_type_support_data_t
{
  void * data[2];
} _ExecutePlan_GetResult_type_support_data_t;

static _ExecutePlan_GetResult_type_support_data_t _ExecutePlan_GetResult_service_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ExecutePlan_GetResult_service_typesupport_map = {
  2,
  "aprs_interfaces",
  &_ExecutePlan_GetResult_service_typesupport_ids.typesupport_identifier[0],
  &_ExecutePlan_GetResult_service_typesupport_symbol_names.symbol_name[0],
  &_ExecutePlan_GetResult_service_typesupport_data.data[0],
};

static const rosidl_service_type_support_t ExecutePlan_GetResult_service_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ExecutePlan_GetResult_service_typesupport_map),
  ::rosidl_typesupport_cpp::get_service_typesupport_handle_function,
  ::rosidl_typesupport_cpp::get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_GetResult_Request>(),
  ::rosidl_typesupport_cpp::get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_GetResult_Response>(),
  ::rosidl_typesupport_cpp::get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_GetResult_Event>(),
  &::rosidl_typesupport_cpp::service_create_event_message<aprs_interfaces::action::ExecutePlan_GetResult>,
  &::rosidl_typesupport_cpp::service_destroy_event_message<aprs_interfaces::action::ExecutePlan_GetResult>,
  &aprs_interfaces__action__ExecutePlan_GetResult__get_type_hash,
  &aprs_interfaces__action__ExecutePlan_GetResult__get_type_description,
  &aprs_interfaces__action__ExecutePlan_GetResult__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace action

}  // namespace aprs_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_service_type_support_t *
get_service_type_support_handle<aprs_interfaces::action::ExecutePlan_GetResult>()
{
  return &::aprs_interfaces::action::rosidl_typesupport_cpp::ExecutePlan_GetResult_service_type_support_handle;
}

}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__functions.h"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace aprs_interfaces
{

namespace action
{

namespace rosidl_typesupport_cpp
{

typedef struct _ExecutePlan_FeedbackMessage_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ExecutePlan_FeedbackMessage_type_support_ids_t;

static const _ExecutePlan_FeedbackMessage_type_support_ids_t _ExecutePlan_FeedbackMessage_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _ExecutePlan_FeedbackMessage_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ExecutePlan_FeedbackMessage_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ExecutePlan_FeedbackMessage_type_support_symbol_names_t _ExecutePlan_FeedbackMessage_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, aprs_interfaces, action, ExecutePlan_FeedbackMessage)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, action, ExecutePlan_FeedbackMessage)),
  }
};

typedef struct _ExecutePlan_FeedbackMessage_type_support_data_t
{
  void * data[2];
} _ExecutePlan_FeedbackMessage_type_support_data_t;

static _ExecutePlan_FeedbackMessage_type_support_data_t _ExecutePlan_FeedbackMessage_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ExecutePlan_FeedbackMessage_message_typesupport_map = {
  2,
  "aprs_interfaces",
  &_ExecutePlan_FeedbackMessage_message_typesupport_ids.typesupport_identifier[0],
  &_ExecutePlan_FeedbackMessage_message_typesupport_symbol_names.symbol_name[0],
  &_ExecutePlan_FeedbackMessage_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t ExecutePlan_FeedbackMessage_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ExecutePlan_FeedbackMessage_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &aprs_interfaces__action__ExecutePlan_FeedbackMessage__get_type_hash,
  &aprs_interfaces__action__ExecutePlan_FeedbackMessage__get_type_description,
  &aprs_interfaces__action__ExecutePlan_FeedbackMessage__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace action

}  // namespace aprs_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_FeedbackMessage>()
{
  return &::aprs_interfaces::action::rosidl_typesupport_cpp::ExecutePlan_FeedbackMessage_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, aprs_interfaces, action, ExecutePlan_FeedbackMessage)() {
  return get_message_type_support_handle<aprs_interfaces::action::ExecutePlan_FeedbackMessage>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

#include "action_msgs/msg/goal_status_array.hpp"
#include "action_msgs/srv/cancel_goal.hpp"
// already included above
// #include "aprs_interfaces/action/detail/execute_plan__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
#include "rosidl_runtime_c/action_type_support_struct.h"
#include "rosidl_typesupport_cpp/action_type_support.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_cpp/service_type_support.hpp"

namespace aprs_interfaces
{

namespace action
{

namespace rosidl_typesupport_cpp
{

static rosidl_action_type_support_t ExecutePlan_action_type_support_handle = {
  NULL, NULL, NULL, NULL, NULL,
  &aprs_interfaces__action__ExecutePlan__get_type_hash,
  &aprs_interfaces__action__ExecutePlan__get_type_description,
  &aprs_interfaces__action__ExecutePlan__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace action

}  // namespace aprs_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_action_type_support_t *
get_action_type_support_handle<aprs_interfaces::action::ExecutePlan>()
{
  using ::aprs_interfaces::action::rosidl_typesupport_cpp::ExecutePlan_action_type_support_handle;
  // Thread-safe by always writing the same values to the static struct
  ExecutePlan_action_type_support_handle.goal_service_type_support = get_service_type_support_handle<::aprs_interfaces::action::ExecutePlan::Impl::SendGoalService>();
  ExecutePlan_action_type_support_handle.result_service_type_support = get_service_type_support_handle<::aprs_interfaces::action::ExecutePlan::Impl::GetResultService>();
  ExecutePlan_action_type_support_handle.cancel_service_type_support = get_service_type_support_handle<::aprs_interfaces::action::ExecutePlan::Impl::CancelGoalService>();
  ExecutePlan_action_type_support_handle.feedback_message_type_support = get_message_type_support_handle<::aprs_interfaces::action::ExecutePlan::Impl::FeedbackMessage>();
  ExecutePlan_action_type_support_handle.status_message_type_support = get_message_type_support_handle<::aprs_interfaces::action::ExecutePlan::Impl::GoalStatusMessage>();
  return &ExecutePlan_action_type_support_handle;
}

}  // namespace rosidl_typesupport_cpp
