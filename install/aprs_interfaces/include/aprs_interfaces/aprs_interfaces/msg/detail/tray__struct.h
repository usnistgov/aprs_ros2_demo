// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from aprs_interfaces:msg/Tray.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__TRAY__STRUCT_H_
#define APRS_INTERFACES__MSG__DETAIL__TRAY__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'name'
#include "rosidl_runtime_c/string.h"
// Member 'slots'
#include "aprs_interfaces/msg/detail/slot_info__struct.h"

/// Struct defined in msg/Tray in the package aprs_interfaces.
typedef struct aprs_interfaces__msg__Tray
{
  /// 13: SMALL_GEAR_TRAY, 14: MEDIUM_GEAR_TRAY, 15: LARGE_GEAR_TRAY 16: M2L1_KIT_TRAY, 17: S2L2_KIT_TRAY
  uint8_t identifier;
  rosidl_runtime_c__String name;
  aprs_interfaces__msg__SlotInfo__Sequence slots;
} aprs_interfaces__msg__Tray;

// Struct for a sequence of aprs_interfaces__msg__Tray.
typedef struct aprs_interfaces__msg__Tray__Sequence
{
  aprs_interfaces__msg__Tray * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__msg__Tray__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // APRS_INTERFACES__MSG__DETAIL__TRAY__STRUCT_H_
