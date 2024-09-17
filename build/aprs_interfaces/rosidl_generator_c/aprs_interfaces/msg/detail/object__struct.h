// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from aprs_interfaces:msg/Object.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__OBJECT__STRUCT_H_
#define APRS_INTERFACES__MSG__DETAIL__OBJECT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

/// Constant 'KIT_TRAY'.
enum
{
  aprs_interfaces__msg__Object__KIT_TRAY = 1
};

/// Constant 'PART_TRAY'.
enum
{
  aprs_interfaces__msg__Object__PART_TRAY = 2
};

/// Constant 'PART'.
enum
{
  aprs_interfaces__msg__Object__PART = 3
};

/// Constant 'SMALL_GEAR'.
/**
  * Object Identifier
 */
enum
{
  aprs_interfaces__msg__Object__SMALL_GEAR = 10
};

/// Constant 'MEDIUM_GEAR'.
enum
{
  aprs_interfaces__msg__Object__MEDIUM_GEAR = 11
};

/// Constant 'LARGE_GEAR'.
enum
{
  aprs_interfaces__msg__Object__LARGE_GEAR = 12
};

/// Constant 'SMALL_GEAR_TRAY'.
enum
{
  aprs_interfaces__msg__Object__SMALL_GEAR_TRAY = 13
};

/// Constant 'MEDIUM_GEAR_TRAY'.
enum
{
  aprs_interfaces__msg__Object__MEDIUM_GEAR_TRAY = 14
};

/// Constant 'LARGE_GEAR_TRAY'.
enum
{
  aprs_interfaces__msg__Object__LARGE_GEAR_TRAY = 15
};

/// Constant 'M2L1_KIT_TRAY'.
enum
{
  aprs_interfaces__msg__Object__M2L1_KIT_TRAY = 16
};

/// Constant 'S2L2_KIT_TRAY'.
enum
{
  aprs_interfaces__msg__Object__S2L2_KIT_TRAY = 17
};

// Include directives for member types
// Member 'name'
#include "rosidl_runtime_c/string.h"
// Member 'pose_stamped'
#include "geometry_msgs/msg/detail/pose_stamped__struct.h"

/// Struct defined in msg/Object in the package aprs_interfaces.
/**
  * Object Type
 */
typedef struct aprs_interfaces__msg__Object
{
  rosidl_runtime_c__String name;
  /// 1: KitTray, 2: PartTray, 3: Part
  uint8_t object_type;
  /// 10: SmallGear, 11: MediumGear, 12: LargeGear, 13: SmallGearTray, 14: MediumGearTray, 15: LargeGearTray, 16: M2L1KitTray, 17: S2L2KitTray
  uint8_t object_identifier;
  geometry_msgs__msg__PoseStamped pose_stamped;
} aprs_interfaces__msg__Object;

// Struct for a sequence of aprs_interfaces__msg__Object.
typedef struct aprs_interfaces__msg__Object__Sequence
{
  aprs_interfaces__msg__Object * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__msg__Object__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // APRS_INTERFACES__MSG__DETAIL__OBJECT__STRUCT_H_
