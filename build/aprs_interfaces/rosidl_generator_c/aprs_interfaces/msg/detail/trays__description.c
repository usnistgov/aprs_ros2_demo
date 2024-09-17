// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from aprs_interfaces:msg/Trays.idl
// generated code does not contain a copyright notice

#include "aprs_interfaces/msg/detail/trays__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_aprs_interfaces
const rosidl_type_hash_t *
aprs_interfaces__msg__Trays__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xb1, 0x07, 0xbb, 0xce, 0x75, 0x35, 0xb2, 0xdd,
      0x48, 0x76, 0x95, 0xd3, 0x56, 0x31, 0xaf, 0x99,
      0x4f, 0xf5, 0x68, 0x9a, 0x2d, 0x13, 0xfb, 0x0b,
      0x6c, 0x3c, 0x65, 0x64, 0x8b, 0x44, 0xc1, 0x6f,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "aprs_interfaces/msg/detail/tray__functions.h"
#include "aprs_interfaces/msg/detail/slot_info__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t aprs_interfaces__msg__SlotInfo__EXPECTED_HASH = {1, {
    0x32, 0x52, 0x8f, 0xef, 0xec, 0x41, 0x9a, 0x8f,
    0xe3, 0xa1, 0xf5, 0xdb, 0x80, 0x30, 0x50, 0x50,
    0xe6, 0xa5, 0xb3, 0x2e, 0x90, 0x62, 0xcf, 0x24,
    0x5d, 0x1c, 0x11, 0xec, 0xcb, 0x79, 0xcd, 0xe2,
  }};
static const rosidl_type_hash_t aprs_interfaces__msg__Tray__EXPECTED_HASH = {1, {
    0x10, 0xe6, 0x38, 0x7d, 0xb3, 0xbd, 0x0f, 0xa4,
    0x9b, 0x9f, 0xc5, 0x45, 0xae, 0xb5, 0xa5, 0x95,
    0x1b, 0xc6, 0xa5, 0xed, 0xd1, 0xe6, 0xed, 0xc3,
    0x07, 0x87, 0xa2, 0x29, 0x15, 0x2a, 0xfc, 0xf3,
  }};
#endif

static char aprs_interfaces__msg__Trays__TYPE_NAME[] = "aprs_interfaces/msg/Trays";
static char aprs_interfaces__msg__SlotInfo__TYPE_NAME[] = "aprs_interfaces/msg/SlotInfo";
static char aprs_interfaces__msg__Tray__TYPE_NAME[] = "aprs_interfaces/msg/Tray";

// Define type names, field names, and default values
static char aprs_interfaces__msg__Trays__FIELD_NAME__kit_trays[] = "kit_trays";
static char aprs_interfaces__msg__Trays__FIELD_NAME__part_trays[] = "part_trays";

static rosidl_runtime_c__type_description__Field aprs_interfaces__msg__Trays__FIELDS[] = {
  {
    {aprs_interfaces__msg__Trays__FIELD_NAME__kit_trays, 9, 9},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {aprs_interfaces__msg__Tray__TYPE_NAME, 24, 24},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__msg__Trays__FIELD_NAME__part_trays, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {aprs_interfaces__msg__Tray__TYPE_NAME, 24, 24},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription aprs_interfaces__msg__Trays__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {aprs_interfaces__msg__SlotInfo__TYPE_NAME, 28, 28},
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__msg__Tray__TYPE_NAME, 24, 24},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
aprs_interfaces__msg__Trays__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {aprs_interfaces__msg__Trays__TYPE_NAME, 25, 25},
      {aprs_interfaces__msg__Trays__FIELDS, 2, 2},
    },
    {aprs_interfaces__msg__Trays__REFERENCED_TYPE_DESCRIPTIONS, 2, 2},
  };
  if (!constructed) {
    assert(0 == memcmp(&aprs_interfaces__msg__SlotInfo__EXPECTED_HASH, aprs_interfaces__msg__SlotInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = aprs_interfaces__msg__SlotInfo__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&aprs_interfaces__msg__Tray__EXPECTED_HASH, aprs_interfaces__msg__Tray__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = aprs_interfaces__msg__Tray__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "aprs_interfaces/Tray[] kit_trays\n"
  "aprs_interfaces/Tray[] part_trays";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
aprs_interfaces__msg__Trays__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {aprs_interfaces__msg__Trays__TYPE_NAME, 25, 25},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 66, 66},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
aprs_interfaces__msg__Trays__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[3];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 3, 3};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *aprs_interfaces__msg__Trays__get_individual_type_description_source(NULL),
    sources[1] = *aprs_interfaces__msg__SlotInfo__get_individual_type_description_source(NULL);
    sources[2] = *aprs_interfaces__msg__Tray__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
