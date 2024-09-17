// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from aprs_interfaces:srv/MoveToNamedPose.idl
// generated code does not contain a copyright notice

#include "aprs_interfaces/srv/detail/move_to_named_pose__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_aprs_interfaces
const rosidl_type_hash_t *
aprs_interfaces__srv__MoveToNamedPose__get_type_hash(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x3c, 0x2d, 0x8d, 0xe7, 0x77, 0x6d, 0x06, 0x28,
      0xfe, 0x9f, 0x03, 0xab, 0x66, 0x95, 0xaa, 0x47,
      0x1d, 0x3a, 0xf5, 0xb4, 0x18, 0x2f, 0x28, 0x53,
      0xd4, 0x41, 0x19, 0x91, 0x58, 0x85, 0x11, 0x14,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_aprs_interfaces
const rosidl_type_hash_t *
aprs_interfaces__srv__MoveToNamedPose_Request__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x7f, 0x25, 0x33, 0x0c, 0x2d, 0x5e, 0x4a, 0xcc,
      0x4d, 0x3e, 0xf8, 0x17, 0xa0, 0x61, 0x91, 0x59,
      0x47, 0x21, 0x1b, 0x31, 0x57, 0x56, 0x00, 0x57,
      0xd3, 0x51, 0xa4, 0x5f, 0x60, 0xd3, 0x7c, 0x33,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_aprs_interfaces
const rosidl_type_hash_t *
aprs_interfaces__srv__MoveToNamedPose_Response__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x64, 0x56, 0x8b, 0xa1, 0x63, 0x69, 0xb4, 0xf5,
      0x30, 0xdf, 0xe0, 0x92, 0x48, 0x26, 0x5f, 0xbc,
      0x44, 0x14, 0x3f, 0xab, 0x38, 0xa7, 0xab, 0xa4,
      0xae, 0x64, 0xeb, 0x3f, 0x05, 0xdd, 0xd1, 0xf8,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_aprs_interfaces
const rosidl_type_hash_t *
aprs_interfaces__srv__MoveToNamedPose_Event__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x57, 0xba, 0x8d, 0xb4, 0x15, 0x3a, 0x1b, 0x69,
      0x74, 0xf1, 0xfc, 0x84, 0xfd, 0x11, 0x0e, 0xbb,
      0xf2, 0x2b, 0xff, 0xb4, 0x1e, 0xb5, 0x0f, 0x9f,
      0xa3, 0x2b, 0x15, 0x10, 0x7f, 0x4a, 0x55, 0x38,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "service_msgs/msg/detail/service_event_info__functions.h"
#include "builtin_interfaces/msg/detail/time__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
static const rosidl_type_hash_t service_msgs__msg__ServiceEventInfo__EXPECTED_HASH = {1, {
    0x41, 0xbc, 0xbb, 0xe0, 0x7a, 0x75, 0xc9, 0xb5,
    0x2b, 0xc9, 0x6b, 0xfd, 0x5c, 0x24, 0xd7, 0xf0,
    0xfc, 0x0a, 0x08, 0xc0, 0xcb, 0x79, 0x21, 0xb3,
    0x37, 0x3c, 0x57, 0x32, 0x34, 0x5a, 0x6f, 0x45,
  }};
#endif

static char aprs_interfaces__srv__MoveToNamedPose__TYPE_NAME[] = "aprs_interfaces/srv/MoveToNamedPose";
static char aprs_interfaces__srv__MoveToNamedPose_Event__TYPE_NAME[] = "aprs_interfaces/srv/MoveToNamedPose_Event";
static char aprs_interfaces__srv__MoveToNamedPose_Request__TYPE_NAME[] = "aprs_interfaces/srv/MoveToNamedPose_Request";
static char aprs_interfaces__srv__MoveToNamedPose_Response__TYPE_NAME[] = "aprs_interfaces/srv/MoveToNamedPose_Response";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char service_msgs__msg__ServiceEventInfo__TYPE_NAME[] = "service_msgs/msg/ServiceEventInfo";

// Define type names, field names, and default values
static char aprs_interfaces__srv__MoveToNamedPose__FIELD_NAME__request_message[] = "request_message";
static char aprs_interfaces__srv__MoveToNamedPose__FIELD_NAME__response_message[] = "response_message";
static char aprs_interfaces__srv__MoveToNamedPose__FIELD_NAME__event_message[] = "event_message";

static rosidl_runtime_c__type_description__Field aprs_interfaces__srv__MoveToNamedPose__FIELDS[] = {
  {
    {aprs_interfaces__srv__MoveToNamedPose__FIELD_NAME__request_message, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {aprs_interfaces__srv__MoveToNamedPose_Request__TYPE_NAME, 43, 43},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__MoveToNamedPose__FIELD_NAME__response_message, 16, 16},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {aprs_interfaces__srv__MoveToNamedPose_Response__TYPE_NAME, 44, 44},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__MoveToNamedPose__FIELD_NAME__event_message, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {aprs_interfaces__srv__MoveToNamedPose_Event__TYPE_NAME, 41, 41},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription aprs_interfaces__srv__MoveToNamedPose__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {aprs_interfaces__srv__MoveToNamedPose_Event__TYPE_NAME, 41, 41},
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__MoveToNamedPose_Request__TYPE_NAME, 43, 43},
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__MoveToNamedPose_Response__TYPE_NAME, 44, 44},
    {NULL, 0, 0},
  },
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
aprs_interfaces__srv__MoveToNamedPose__get_type_description(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {aprs_interfaces__srv__MoveToNamedPose__TYPE_NAME, 35, 35},
      {aprs_interfaces__srv__MoveToNamedPose__FIELDS, 3, 3},
    },
    {aprs_interfaces__srv__MoveToNamedPose__REFERENCED_TYPE_DESCRIPTIONS, 5, 5},
  };
  if (!constructed) {
    description.referenced_type_descriptions.data[0].fields = aprs_interfaces__srv__MoveToNamedPose_Event__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = aprs_interfaces__srv__MoveToNamedPose_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = aprs_interfaces__srv__MoveToNamedPose_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[4].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char aprs_interfaces__srv__MoveToNamedPose_Request__FIELD_NAME__name[] = "name";

static rosidl_runtime_c__type_description__Field aprs_interfaces__srv__MoveToNamedPose_Request__FIELDS[] = {
  {
    {aprs_interfaces__srv__MoveToNamedPose_Request__FIELD_NAME__name, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
aprs_interfaces__srv__MoveToNamedPose_Request__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {aprs_interfaces__srv__MoveToNamedPose_Request__TYPE_NAME, 43, 43},
      {aprs_interfaces__srv__MoveToNamedPose_Request__FIELDS, 1, 1},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char aprs_interfaces__srv__MoveToNamedPose_Response__FIELD_NAME__success[] = "success";
static char aprs_interfaces__srv__MoveToNamedPose_Response__FIELD_NAME__status[] = "status";

static rosidl_runtime_c__type_description__Field aprs_interfaces__srv__MoveToNamedPose_Response__FIELDS[] = {
  {
    {aprs_interfaces__srv__MoveToNamedPose_Response__FIELD_NAME__success, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__MoveToNamedPose_Response__FIELD_NAME__status, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
aprs_interfaces__srv__MoveToNamedPose_Response__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {aprs_interfaces__srv__MoveToNamedPose_Response__TYPE_NAME, 44, 44},
      {aprs_interfaces__srv__MoveToNamedPose_Response__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char aprs_interfaces__srv__MoveToNamedPose_Event__FIELD_NAME__info[] = "info";
static char aprs_interfaces__srv__MoveToNamedPose_Event__FIELD_NAME__request[] = "request";
static char aprs_interfaces__srv__MoveToNamedPose_Event__FIELD_NAME__response[] = "response";

static rosidl_runtime_c__type_description__Field aprs_interfaces__srv__MoveToNamedPose_Event__FIELDS[] = {
  {
    {aprs_interfaces__srv__MoveToNamedPose_Event__FIELD_NAME__info, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__MoveToNamedPose_Event__FIELD_NAME__request, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {aprs_interfaces__srv__MoveToNamedPose_Request__TYPE_NAME, 43, 43},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__MoveToNamedPose_Event__FIELD_NAME__response, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {aprs_interfaces__srv__MoveToNamedPose_Response__TYPE_NAME, 44, 44},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription aprs_interfaces__srv__MoveToNamedPose_Event__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {aprs_interfaces__srv__MoveToNamedPose_Request__TYPE_NAME, 43, 43},
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__MoveToNamedPose_Response__TYPE_NAME, 44, 44},
    {NULL, 0, 0},
  },
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
aprs_interfaces__srv__MoveToNamedPose_Event__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {aprs_interfaces__srv__MoveToNamedPose_Event__TYPE_NAME, 41, 41},
      {aprs_interfaces__srv__MoveToNamedPose_Event__FIELDS, 3, 3},
    },
    {aprs_interfaces__srv__MoveToNamedPose_Event__REFERENCED_TYPE_DESCRIPTIONS, 4, 4},
  };
  if (!constructed) {
    description.referenced_type_descriptions.data[0].fields = aprs_interfaces__srv__MoveToNamedPose_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = aprs_interfaces__srv__MoveToNamedPose_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[2].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "string name\n"
  "---\n"
  "bool success\n"
  "string status";

static char srv_encoding[] = "srv";
static char implicit_encoding[] = "implicit";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
aprs_interfaces__srv__MoveToNamedPose__get_individual_type_description_source(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {aprs_interfaces__srv__MoveToNamedPose__TYPE_NAME, 35, 35},
    {srv_encoding, 3, 3},
    {toplevel_type_raw_source, 42, 42},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
aprs_interfaces__srv__MoveToNamedPose_Request__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {aprs_interfaces__srv__MoveToNamedPose_Request__TYPE_NAME, 43, 43},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
aprs_interfaces__srv__MoveToNamedPose_Response__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {aprs_interfaces__srv__MoveToNamedPose_Response__TYPE_NAME, 44, 44},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
aprs_interfaces__srv__MoveToNamedPose_Event__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {aprs_interfaces__srv__MoveToNamedPose_Event__TYPE_NAME, 41, 41},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
aprs_interfaces__srv__MoveToNamedPose__get_type_description_sources(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[6];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 6, 6};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *aprs_interfaces__srv__MoveToNamedPose__get_individual_type_description_source(NULL),
    sources[1] = *aprs_interfaces__srv__MoveToNamedPose_Event__get_individual_type_description_source(NULL);
    sources[2] = *aprs_interfaces__srv__MoveToNamedPose_Request__get_individual_type_description_source(NULL);
    sources[3] = *aprs_interfaces__srv__MoveToNamedPose_Response__get_individual_type_description_source(NULL);
    sources[4] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[5] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
aprs_interfaces__srv__MoveToNamedPose_Request__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *aprs_interfaces__srv__MoveToNamedPose_Request__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
aprs_interfaces__srv__MoveToNamedPose_Response__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *aprs_interfaces__srv__MoveToNamedPose_Response__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
aprs_interfaces__srv__MoveToNamedPose_Event__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[5];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 5, 5};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *aprs_interfaces__srv__MoveToNamedPose_Event__get_individual_type_description_source(NULL),
    sources[1] = *aprs_interfaces__srv__MoveToNamedPose_Request__get_individual_type_description_source(NULL);
    sources[2] = *aprs_interfaces__srv__MoveToNamedPose_Response__get_individual_type_description_source(NULL);
    sources[3] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[4] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
