// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from aprs_interfaces:srv/Place.idl
// generated code does not contain a copyright notice

#include "aprs_interfaces/srv/detail/place__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_aprs_interfaces
const rosidl_type_hash_t *
aprs_interfaces__srv__Place__get_type_hash(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x2e, 0xc6, 0x21, 0x6f, 0xb0, 0xe6, 0x67, 0x8b,
      0xab, 0x1f, 0xb5, 0xc5, 0xbe, 0x58, 0x9f, 0x11,
      0x3d, 0xb1, 0x22, 0x68, 0x4f, 0xb9, 0xc4, 0xb9,
      0xcc, 0xf6, 0x7b, 0x2a, 0x32, 0xef, 0x7b, 0xb6,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_aprs_interfaces
const rosidl_type_hash_t *
aprs_interfaces__srv__Place_Request__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xec, 0x96, 0x21, 0xb9, 0xb3, 0xcd, 0x48, 0x1a,
      0x4c, 0x35, 0xda, 0x7e, 0x15, 0xa3, 0x25, 0x98,
      0x10, 0xde, 0xad, 0xfd, 0xf3, 0x41, 0x98, 0x53,
      0x3b, 0xd6, 0x21, 0x35, 0x38, 0xa0, 0xd3, 0x7e,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_aprs_interfaces
const rosidl_type_hash_t *
aprs_interfaces__srv__Place_Response__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x30, 0x61, 0x04, 0xdb, 0xfb, 0x6f, 0x42, 0xe7,
      0x3d, 0xdf, 0x99, 0x3a, 0x22, 0xed, 0x7f, 0x6c,
      0xa5, 0xfb, 0x2a, 0x49, 0xdd, 0x27, 0x0b, 0xa9,
      0xf5, 0x9d, 0x8d, 0x91, 0x42, 0xd9, 0x22, 0xb7,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_aprs_interfaces
const rosidl_type_hash_t *
aprs_interfaces__srv__Place_Event__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x6c, 0xbb, 0x5e, 0x67, 0xb3, 0xc7, 0xc8, 0x8d,
      0x9b, 0xe8, 0x4d, 0x31, 0x14, 0x94, 0x51, 0x31,
      0x19, 0x8b, 0xd3, 0xde, 0x4e, 0xe8, 0x8c, 0x51,
      0x78, 0xf2, 0x86, 0x1c, 0xfa, 0x51, 0x02, 0x7d,
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

static char aprs_interfaces__srv__Place__TYPE_NAME[] = "aprs_interfaces/srv/Place";
static char aprs_interfaces__srv__Place_Event__TYPE_NAME[] = "aprs_interfaces/srv/Place_Event";
static char aprs_interfaces__srv__Place_Request__TYPE_NAME[] = "aprs_interfaces/srv/Place_Request";
static char aprs_interfaces__srv__Place_Response__TYPE_NAME[] = "aprs_interfaces/srv/Place_Response";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char service_msgs__msg__ServiceEventInfo__TYPE_NAME[] = "service_msgs/msg/ServiceEventInfo";

// Define type names, field names, and default values
static char aprs_interfaces__srv__Place__FIELD_NAME__request_message[] = "request_message";
static char aprs_interfaces__srv__Place__FIELD_NAME__response_message[] = "response_message";
static char aprs_interfaces__srv__Place__FIELD_NAME__event_message[] = "event_message";

static rosidl_runtime_c__type_description__Field aprs_interfaces__srv__Place__FIELDS[] = {
  {
    {aprs_interfaces__srv__Place__FIELD_NAME__request_message, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {aprs_interfaces__srv__Place_Request__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__Place__FIELD_NAME__response_message, 16, 16},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {aprs_interfaces__srv__Place_Response__TYPE_NAME, 34, 34},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__Place__FIELD_NAME__event_message, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {aprs_interfaces__srv__Place_Event__TYPE_NAME, 31, 31},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription aprs_interfaces__srv__Place__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {aprs_interfaces__srv__Place_Event__TYPE_NAME, 31, 31},
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__Place_Request__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__Place_Response__TYPE_NAME, 34, 34},
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
aprs_interfaces__srv__Place__get_type_description(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {aprs_interfaces__srv__Place__TYPE_NAME, 25, 25},
      {aprs_interfaces__srv__Place__FIELDS, 3, 3},
    },
    {aprs_interfaces__srv__Place__REFERENCED_TYPE_DESCRIPTIONS, 5, 5},
  };
  if (!constructed) {
    description.referenced_type_descriptions.data[0].fields = aprs_interfaces__srv__Place_Event__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = aprs_interfaces__srv__Place_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = aprs_interfaces__srv__Place_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[4].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char aprs_interfaces__srv__Place_Request__FIELD_NAME__frame_name[] = "frame_name";

static rosidl_runtime_c__type_description__Field aprs_interfaces__srv__Place_Request__FIELDS[] = {
  {
    {aprs_interfaces__srv__Place_Request__FIELD_NAME__frame_name, 10, 10},
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
aprs_interfaces__srv__Place_Request__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {aprs_interfaces__srv__Place_Request__TYPE_NAME, 33, 33},
      {aprs_interfaces__srv__Place_Request__FIELDS, 1, 1},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char aprs_interfaces__srv__Place_Response__FIELD_NAME__success[] = "success";
static char aprs_interfaces__srv__Place_Response__FIELD_NAME__status[] = "status";

static rosidl_runtime_c__type_description__Field aprs_interfaces__srv__Place_Response__FIELDS[] = {
  {
    {aprs_interfaces__srv__Place_Response__FIELD_NAME__success, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__Place_Response__FIELD_NAME__status, 6, 6},
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
aprs_interfaces__srv__Place_Response__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {aprs_interfaces__srv__Place_Response__TYPE_NAME, 34, 34},
      {aprs_interfaces__srv__Place_Response__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char aprs_interfaces__srv__Place_Event__FIELD_NAME__info[] = "info";
static char aprs_interfaces__srv__Place_Event__FIELD_NAME__request[] = "request";
static char aprs_interfaces__srv__Place_Event__FIELD_NAME__response[] = "response";

static rosidl_runtime_c__type_description__Field aprs_interfaces__srv__Place_Event__FIELDS[] = {
  {
    {aprs_interfaces__srv__Place_Event__FIELD_NAME__info, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__Place_Event__FIELD_NAME__request, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {aprs_interfaces__srv__Place_Request__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__Place_Event__FIELD_NAME__response, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {aprs_interfaces__srv__Place_Response__TYPE_NAME, 34, 34},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription aprs_interfaces__srv__Place_Event__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {aprs_interfaces__srv__Place_Request__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__Place_Response__TYPE_NAME, 34, 34},
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
aprs_interfaces__srv__Place_Event__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {aprs_interfaces__srv__Place_Event__TYPE_NAME, 31, 31},
      {aprs_interfaces__srv__Place_Event__FIELDS, 3, 3},
    },
    {aprs_interfaces__srv__Place_Event__REFERENCED_TYPE_DESCRIPTIONS, 4, 4},
  };
  if (!constructed) {
    description.referenced_type_descriptions.data[0].fields = aprs_interfaces__srv__Place_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = aprs_interfaces__srv__Place_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[2].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "string frame_name\n"
  "---\n"
  "bool success\n"
  "string status";

static char srv_encoding[] = "srv";
static char implicit_encoding[] = "implicit";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
aprs_interfaces__srv__Place__get_individual_type_description_source(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {aprs_interfaces__srv__Place__TYPE_NAME, 25, 25},
    {srv_encoding, 3, 3},
    {toplevel_type_raw_source, 48, 48},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
aprs_interfaces__srv__Place_Request__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {aprs_interfaces__srv__Place_Request__TYPE_NAME, 33, 33},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
aprs_interfaces__srv__Place_Response__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {aprs_interfaces__srv__Place_Response__TYPE_NAME, 34, 34},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
aprs_interfaces__srv__Place_Event__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {aprs_interfaces__srv__Place_Event__TYPE_NAME, 31, 31},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
aprs_interfaces__srv__Place__get_type_description_sources(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[6];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 6, 6};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *aprs_interfaces__srv__Place__get_individual_type_description_source(NULL),
    sources[1] = *aprs_interfaces__srv__Place_Event__get_individual_type_description_source(NULL);
    sources[2] = *aprs_interfaces__srv__Place_Request__get_individual_type_description_source(NULL);
    sources[3] = *aprs_interfaces__srv__Place_Response__get_individual_type_description_source(NULL);
    sources[4] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[5] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
aprs_interfaces__srv__Place_Request__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *aprs_interfaces__srv__Place_Request__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
aprs_interfaces__srv__Place_Response__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *aprs_interfaces__srv__Place_Response__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
aprs_interfaces__srv__Place_Event__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[5];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 5, 5};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *aprs_interfaces__srv__Place_Event__get_individual_type_description_source(NULL),
    sources[1] = *aprs_interfaces__srv__Place_Request__get_individual_type_description_source(NULL);
    sources[2] = *aprs_interfaces__srv__Place_Response__get_individual_type_description_source(NULL);
    sources[3] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[4] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
