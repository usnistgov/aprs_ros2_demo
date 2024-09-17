// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from aprs_interfaces:msg/SlotInfo.idl
// generated code does not contain a copyright notice

#include "aprs_interfaces/msg/detail/slot_info__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_aprs_interfaces
const rosidl_type_hash_t *
aprs_interfaces__msg__SlotInfo__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x32, 0x52, 0x8f, 0xef, 0xec, 0x41, 0x9a, 0x8f,
      0xe3, 0xa1, 0xf5, 0xdb, 0x80, 0x30, 0x50, 0x50,
      0xe6, 0xa5, 0xb3, 0x2e, 0x90, 0x62, 0xcf, 0x24,
      0x5d, 0x1c, 0x11, 0xec, 0xcb, 0x79, 0xcd, 0xe2,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char aprs_interfaces__msg__SlotInfo__TYPE_NAME[] = "aprs_interfaces/msg/SlotInfo";

// Define type names, field names, and default values
static char aprs_interfaces__msg__SlotInfo__FIELD_NAME__occupied[] = "occupied";
static char aprs_interfaces__msg__SlotInfo__FIELD_NAME__size[] = "size";
static char aprs_interfaces__msg__SlotInfo__FIELD_NAME__name[] = "name";

static rosidl_runtime_c__type_description__Field aprs_interfaces__msg__SlotInfo__FIELDS[] = {
  {
    {aprs_interfaces__msg__SlotInfo__FIELD_NAME__occupied, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__msg__SlotInfo__FIELD_NAME__size, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {aprs_interfaces__msg__SlotInfo__FIELD_NAME__name, 4, 4},
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
aprs_interfaces__msg__SlotInfo__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {aprs_interfaces__msg__SlotInfo__TYPE_NAME, 28, 28},
      {aprs_interfaces__msg__SlotInfo__FIELDS, 3, 3},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "uint8 SMALL=1\n"
  "uint8 MEDIUM=2\n"
  "uint8 LARGE=3\n"
  "\n"
  "bool occupied\n"
  "uint8 size\n"
  "string name";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
aprs_interfaces__msg__SlotInfo__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {aprs_interfaces__msg__SlotInfo__TYPE_NAME, 28, 28},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 80, 80},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
aprs_interfaces__msg__SlotInfo__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *aprs_interfaces__msg__SlotInfo__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
