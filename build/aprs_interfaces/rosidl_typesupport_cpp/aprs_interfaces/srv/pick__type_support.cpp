// generated from rosidl_typesupport_cpp/resource/idl__type_support.cpp.em
// with input from aprs_interfaces:srv/Pick.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "aprs_interfaces/srv/detail/pick__functions.h"
#include "aprs_interfaces/srv/detail/pick__struct.hpp"
#include "rosidl_typesupport_cpp/identifier.hpp"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
#include "rosidl_typesupport_cpp/visibility_control.h"
#include "rosidl_typesupport_interface/macros.h"

namespace aprs_interfaces
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _Pick_Request_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _Pick_Request_type_support_ids_t;

static const _Pick_Request_type_support_ids_t _Pick_Request_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _Pick_Request_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _Pick_Request_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _Pick_Request_type_support_symbol_names_t _Pick_Request_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, aprs_interfaces, srv, Pick_Request)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, srv, Pick_Request)),
  }
};

typedef struct _Pick_Request_type_support_data_t
{
  void * data[2];
} _Pick_Request_type_support_data_t;

static _Pick_Request_type_support_data_t _Pick_Request_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _Pick_Request_message_typesupport_map = {
  2,
  "aprs_interfaces",
  &_Pick_Request_message_typesupport_ids.typesupport_identifier[0],
  &_Pick_Request_message_typesupport_symbol_names.symbol_name[0],
  &_Pick_Request_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t Pick_Request_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_Pick_Request_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &aprs_interfaces__srv__Pick_Request__get_type_hash,
  &aprs_interfaces__srv__Pick_Request__get_type_description,
  &aprs_interfaces__srv__Pick_Request__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace aprs_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<aprs_interfaces::srv::Pick_Request>()
{
  return &::aprs_interfaces::srv::rosidl_typesupport_cpp::Pick_Request_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, aprs_interfaces, srv, Pick_Request)() {
  return get_message_type_support_handle<aprs_interfaces::srv::Pick_Request>();
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
// #include "aprs_interfaces/srv/detail/pick__functions.h"
// already included above
// #include "aprs_interfaces/srv/detail/pick__struct.hpp"
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

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _Pick_Response_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _Pick_Response_type_support_ids_t;

static const _Pick_Response_type_support_ids_t _Pick_Response_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _Pick_Response_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _Pick_Response_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _Pick_Response_type_support_symbol_names_t _Pick_Response_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, aprs_interfaces, srv, Pick_Response)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, srv, Pick_Response)),
  }
};

typedef struct _Pick_Response_type_support_data_t
{
  void * data[2];
} _Pick_Response_type_support_data_t;

static _Pick_Response_type_support_data_t _Pick_Response_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _Pick_Response_message_typesupport_map = {
  2,
  "aprs_interfaces",
  &_Pick_Response_message_typesupport_ids.typesupport_identifier[0],
  &_Pick_Response_message_typesupport_symbol_names.symbol_name[0],
  &_Pick_Response_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t Pick_Response_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_Pick_Response_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &aprs_interfaces__srv__Pick_Response__get_type_hash,
  &aprs_interfaces__srv__Pick_Response__get_type_description,
  &aprs_interfaces__srv__Pick_Response__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace aprs_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<aprs_interfaces::srv::Pick_Response>()
{
  return &::aprs_interfaces::srv::rosidl_typesupport_cpp::Pick_Response_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, aprs_interfaces, srv, Pick_Response)() {
  return get_message_type_support_handle<aprs_interfaces::srv::Pick_Response>();
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
// #include "aprs_interfaces/srv/detail/pick__functions.h"
// already included above
// #include "aprs_interfaces/srv/detail/pick__struct.hpp"
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

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _Pick_Event_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _Pick_Event_type_support_ids_t;

static const _Pick_Event_type_support_ids_t _Pick_Event_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _Pick_Event_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _Pick_Event_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _Pick_Event_type_support_symbol_names_t _Pick_Event_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, aprs_interfaces, srv, Pick_Event)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, srv, Pick_Event)),
  }
};

typedef struct _Pick_Event_type_support_data_t
{
  void * data[2];
} _Pick_Event_type_support_data_t;

static _Pick_Event_type_support_data_t _Pick_Event_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _Pick_Event_message_typesupport_map = {
  2,
  "aprs_interfaces",
  &_Pick_Event_message_typesupport_ids.typesupport_identifier[0],
  &_Pick_Event_message_typesupport_symbol_names.symbol_name[0],
  &_Pick_Event_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t Pick_Event_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_Pick_Event_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &aprs_interfaces__srv__Pick_Event__get_type_hash,
  &aprs_interfaces__srv__Pick_Event__get_type_description,
  &aprs_interfaces__srv__Pick_Event__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace aprs_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<aprs_interfaces::srv::Pick_Event>()
{
  return &::aprs_interfaces::srv::rosidl_typesupport_cpp::Pick_Event_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, aprs_interfaces, srv, Pick_Event)() {
  return get_message_type_support_handle<aprs_interfaces::srv::Pick_Event>();
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
// #include "aprs_interfaces/srv/detail/pick__struct.hpp"
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

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _Pick_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _Pick_type_support_ids_t;

static const _Pick_type_support_ids_t _Pick_service_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _Pick_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _Pick_type_support_symbol_names_t;
#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _Pick_type_support_symbol_names_t _Pick_service_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, aprs_interfaces, srv, Pick)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, aprs_interfaces, srv, Pick)),
  }
};

typedef struct _Pick_type_support_data_t
{
  void * data[2];
} _Pick_type_support_data_t;

static _Pick_type_support_data_t _Pick_service_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _Pick_service_typesupport_map = {
  2,
  "aprs_interfaces",
  &_Pick_service_typesupport_ids.typesupport_identifier[0],
  &_Pick_service_typesupport_symbol_names.symbol_name[0],
  &_Pick_service_typesupport_data.data[0],
};

static const rosidl_service_type_support_t Pick_service_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_Pick_service_typesupport_map),
  ::rosidl_typesupport_cpp::get_service_typesupport_handle_function,
  ::rosidl_typesupport_cpp::get_message_type_support_handle<aprs_interfaces::srv::Pick_Request>(),
  ::rosidl_typesupport_cpp::get_message_type_support_handle<aprs_interfaces::srv::Pick_Response>(),
  ::rosidl_typesupport_cpp::get_message_type_support_handle<aprs_interfaces::srv::Pick_Event>(),
  &::rosidl_typesupport_cpp::service_create_event_message<aprs_interfaces::srv::Pick>,
  &::rosidl_typesupport_cpp::service_destroy_event_message<aprs_interfaces::srv::Pick>,
  &aprs_interfaces__srv__Pick__get_type_hash,
  &aprs_interfaces__srv__Pick__get_type_description,
  &aprs_interfaces__srv__Pick__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace aprs_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_service_type_support_t *
get_service_type_support_handle<aprs_interfaces::srv::Pick>()
{
  return &::aprs_interfaces::srv::rosidl_typesupport_cpp::Pick_service_type_support_handle;
}

}  // namespace rosidl_typesupport_cpp
