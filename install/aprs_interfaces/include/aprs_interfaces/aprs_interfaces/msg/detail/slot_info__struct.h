// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from aprs_interfaces:msg/SlotInfo.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__SLOT_INFO__STRUCT_H_
#define APRS_INTERFACES__MSG__DETAIL__SLOT_INFO__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

/// Constant 'SMALL'.
enum
{
  aprs_interfaces__msg__SlotInfo__SMALL = 1
};

/// Constant 'MEDIUM'.
enum
{
  aprs_interfaces__msg__SlotInfo__MEDIUM = 2
};

/// Constant 'LARGE'.
enum
{
  aprs_interfaces__msg__SlotInfo__LARGE = 3
};

// Include directives for member types
// Member 'name'
#include "rosidl_runtime_c/string.h"

/// Struct defined in msg/SlotInfo in the package aprs_interfaces.
typedef struct aprs_interfaces__msg__SlotInfo
{
  bool occupied;
  uint8_t size;
  rosidl_runtime_c__String name;
} aprs_interfaces__msg__SlotInfo;

// Struct for a sequence of aprs_interfaces__msg__SlotInfo.
typedef struct aprs_interfaces__msg__SlotInfo__Sequence
{
  aprs_interfaces__msg__SlotInfo * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__msg__SlotInfo__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // APRS_INTERFACES__MSG__DETAIL__SLOT_INFO__STRUCT_H_
