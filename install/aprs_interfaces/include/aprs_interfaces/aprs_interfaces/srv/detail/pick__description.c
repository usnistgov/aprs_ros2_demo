// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from aprs_interfaces:srv/Pick.idl
// generated code does not contain a copyright notice

#include "aprs_interfaces/srv/detail/pick__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_aprs_interfaces
const rosidl_type_hash_t *
aprs_interfaces__srv__Pick__get_type_hash(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xa5, 0x90, 0x70, 0x87, 0x85, 0x72, 0x49, 0x38,
      0xc7, 0x26, 0x96, 0x97, 0x4c, 0xae, 0x56, 0xf0,
      0x26, 0x8a, 0x22, 0x92, 0x9c, 0xd1, 0x32, 0x4e,
      0xc2, 0x9c, 0xbc, 0xf9, 0xa3, 0x45, 0xdf, 0x97,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_aprs_interfaces
const rosidl_type_hash_t *
aprs_interfaces__srv__Pick_Request__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xf3, 0xa1, 0x1b, 0xe1, 0x2e, 0x5d, 0x17, 0x0d,
      0xad, 0x59, 0x77, 0x4e, 0x45, 0xf7, 0x18, 0xbf,
      0x22, 0x6f, 0x20, 0x12, 0xb5, 0x23, 0x09, 0x21,
      0x52, 0x16, 0x37, 0xd0, 0xee, 0xf2, 0x9d, 0x34,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_aprs_interfaces
const rosidl_type_hash_t *
aprs_interfaces__srv__Pick_Response__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x5c, 0xd3, 0x43, 0xe9, 0xee, 0xb5, 0x37, 0x6f,
      0x94, 0x8e, 0xed, 0x56, 0x2d, 0x12, 0xe5, 0xbd,
      0xc9, 0xc6, 0x51, 0xd8, 0x32, 0x88, 0x67, 0x4b,
      0x2e, 0xed, 0xc3, 0x53, 0xf8, 0x67, 0xb0, 0x4c,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_aprs_interfaces
const rosidl_type_hash_t *
aprs_interfaces__srv__Pick_Event__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x6e, 0x35, 0x07, 0xa3, 0x53, 0x47, 0xd3, 0x39,
      0x3b, 0x35, 0x06, 0x52, 0x96, 0x0f, 0x57, 0x86,
      0xcd, 0x8d, 0xc5, 0x89, 0x18, 0x35, 0xb6, 0x0e,
      0x84, 0x77, 0x8d, 0x18, 0x3e, 0x7e, 0x66, 0xb9,
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

static char aprs_interfaces__srv__Pick__TYPE_NAME[] = "aprs_interfaces/srv/Pick";
static char aprs_interfaces__srv__Pick_Event__TYPE_NAME[] = "aprs_interfaces/srv/Pick_Event";
static char aprs_interfaces__srv__Pick_Request__TYPE_NAME[] = "aprs_interfaces/srv/Pick_Request";
static char aprs_interfaces__srv__Pick_Response__TYPE_NAME[] = "aprs_interfaces/srv/Pick_Response";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char service_msgs__msg__ServiceEventInfo__TYPE_NAME[] = "service_msgs/msg/ServiceEventInfo";

// Define type names, field names, and default values
static char aprs_interfaces__srv__Pick__FIELD_NAME__request_message[] = "request_message";
static char aprs_interfaces__srv__Pick__FIELD_NAME__response_message[] = "response_message";
static char aprs_interfaces__srv__Pick__FIELD_NAME__event_message[] = "event_message";

static rosidl_runtime_c__type_description__Field aprs_interfaces__srv__Pick__FIELDS[] = {
  {
    {aprs_interfaces__srv__Pick__FIELD_NAME__request_message, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {aprs_interfaces__srv__Pick_Request__TYPE_NAME, 32, 32},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__Pick__FIELD_NAME__response_message, 16, 16},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {aprs_interfaces__srv__Pick_Response__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__Pick__FIELD_NAME__event_message, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {aprs_interfaces__srv__Pick_Event__TYPE_NAME, 30, 30},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription aprs_interfaces__srv__Pick__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {aprs_interfaces__srv__Pick_Event__TYPE_NAME, 30, 30},
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__Pick_Request__TYPE_NAME, 32, 32},
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__Pick_Response__TYPE_NAME, 33, 33},
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
aprs_interfaces__srv__Pick__get_type_description(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {aprs_interfaces__srv__Pick__TYPE_NAME, 24, 24},
      {aprs_interfaces__srv__Pick__FIELDS, 3, 3},
    },
    {aprs_interfaces__srv__Pick__REFERENCED_TYPE_DESCRIPTIONS, 5, 5},
  };
  if (!constructed) {
    description.referenced_type_descriptions.data[0].fields = aprs_interfaces__srv__Pick_Event__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = aprs_interfaces__srv__Pick_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = aprs_interfaces__srv__Pick_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[4].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char aprs_interfaces__srv__Pick_Request__FIELD_NAME__frame_name[] = "frame_name";

static rosidl_runtime_c__type_description__Field aprs_interfaces__srv__Pick_Request__FIELDS[] = {
  {
    {aprs_interfaces__srv__Pick_Request__FIELD_NAME__frame_name, 10, 10},
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
aprs_interfaces__srv__Pick_Request__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {aprs_interfaces__srv__Pick_Request__TYPE_NAME, 32, 32},
      {aprs_interfaces__srv__Pick_Request__FIELDS, 1, 1},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char aprs_interfaces__srv__Pick_Response__FIELD_NAME__success[] = "success";
static char aprs_interfaces__srv__Pick_Response__FIELD_NAME__status[] = "status";

static rosidl_runtime_c__type_description__Field aprs_interfaces__srv__Pick_Response__FIELDS[] = {
  {
    {aprs_interfaces__srv__Pick_Response__FIELD_NAME__success, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__Pick_Response__FIELD_NAME__status, 6, 6},
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
aprs_interfaces__srv__Pick_Response__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {aprs_interfaces__srv__Pick_Response__TYPE_NAME, 33, 33},
      {aprs_interfaces__srv__Pick_Response__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char aprs_interfaces__srv__Pick_Event__FIELD_NAME__info[] = "info";
static char aprs_interfaces__srv__Pick_Event__FIELD_NAME__request[] = "request";
static char aprs_interfaces__srv__Pick_Event__FIELD_NAME__response[] = "response";

static rosidl_runtime_c__type_description__Field aprs_interfaces__srv__Pick_Event__FIELDS[] = {
  {
    {aprs_interfaces__srv__Pick_Event__FIELD_NAME__info, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__Pick_Event__FIELD_NAME__request, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {aprs_interfaces__srv__Pick_Request__TYPE_NAME, 32, 32},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__Pick_Event__FIELD_NAME__response, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {aprs_interfaces__srv__Pick_Response__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription aprs_interfaces__srv__Pick_Event__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {aprs_interfaces__srv__Pick_Request__TYPE_NAME, 32, 32},
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__srv__Pick_Response__TYPE_NAME, 33, 33},
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
aprs_interfaces__srv__Pick_Event__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {aprs_interfaces__srv__Pick_Event__TYPE_NAME, 30, 30},
      {aprs_interfaces__srv__Pick_Event__FIELDS, 3, 3},
    },
    {aprs_interfaces__srv__Pick_Event__REFERENCED_TYPE_DESCRIPTIONS, 4, 4},
  };
  if (!constructed) {
    description.referenced_type_descriptions.data[0].fields = aprs_interfaces__srv__Pick_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = aprs_interfaces__srv__Pick_Response__get_type_description(NULL)->type_description.fields;
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
aprs_interfaces__srv__Pick__get_individual_type_description_source(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {aprs_interfaces__srv__Pick__TYPE_NAME, 24, 24},
    {srv_encoding, 3, 3},
    {toplevel_type_raw_source, 48, 48},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
aprs_interfaces__srv__Pick_Request__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {aprs_interfaces__srv__Pick_Request__TYPE_NAME, 32, 32},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
aprs_interfaces__srv__Pick_Response__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {aprs_interfaces__srv__Pick_Response__TYPE_NAME, 33, 33},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
aprs_interfaces__srv__Pick_Event__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {aprs_interfaces__srv__Pick_Event__TYPE_NAME, 30, 30},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
aprs_interfaces__srv__Pick__get_type_description_sources(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[6];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 6, 6};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *aprs_interfaces__srv__Pick__get_individual_type_description_source(NULL),
    sources[1] = *aprs_interfaces__srv__Pick_Event__get_individual_type_description_source(NULL);
    sources[2] = *aprs_interfaces__srv__Pick_Request__get_individual_type_description_source(NULL);
    sources[3] = *aprs_interfaces__srv__Pick_Response__get_individual_type_description_source(NULL);
    sources[4] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[5] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
aprs_interfaces__srv__Pick_Request__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *aprs_interfaces__srv__Pick_Request__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
aprs_interfaces__srv__Pick_Response__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *aprs_interfaces__srv__Pick_Response__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
aprs_interfaces__srv__Pick_Event__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[5];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 5, 5};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *aprs_interfaces__srv__Pick_Event__get_individual_type_description_source(NULL),
    sources[1] = *aprs_interfaces__srv__Pick_Request__get_individual_type_description_source(NULL);
    sources[2] = *aprs_interfaces__srv__Pick_Response__get_individual_type_description_source(NULL);
    sources[3] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[4] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
