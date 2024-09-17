// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from aprs_interfaces:msg/Trays.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__TRAYS__STRUCT_H_
#define APRS_INTERFACES__MSG__DETAIL__TRAYS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'kit_trays'
// Member 'part_trays'
#include "aprs_interfaces/msg/detail/tray__struct.h"

/// Struct defined in msg/Trays in the package aprs_interfaces.
typedef struct aprs_interfaces__msg__Trays
{
  aprs_interfaces__msg__Tray__Sequence kit_trays;
  aprs_interfaces__msg__Tray__Sequence part_trays;
} aprs_interfaces__msg__Trays;

// Struct for a sequence of aprs_interfaces__msg__Trays.
typedef struct aprs_interfaces__msg__Trays__Sequence
{
  aprs_interfaces__msg__Trays * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__msg__Trays__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // APRS_INTERFACES__MSG__DETAIL__TRAYS__STRUCT_H_
