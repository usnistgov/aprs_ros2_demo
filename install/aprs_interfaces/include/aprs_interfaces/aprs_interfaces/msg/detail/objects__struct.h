// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from aprs_interfaces:msg/Objects.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__OBJECTS__STRUCT_H_
#define APRS_INTERFACES__MSG__DETAIL__OBJECTS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'objects'
#include "aprs_interfaces/msg/detail/object__struct.h"

/// Struct defined in msg/Objects in the package aprs_interfaces.
typedef struct aprs_interfaces__msg__Objects
{
  aprs_interfaces__msg__Object__Sequence objects;
} aprs_interfaces__msg__Objects;

// Struct for a sequence of aprs_interfaces__msg__Objects.
typedef struct aprs_interfaces__msg__Objects__Sequence
{
  aprs_interfaces__msg__Objects * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} aprs_interfaces__msg__Objects__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // APRS_INTERFACES__MSG__DETAIL__OBJECTS__STRUCT_H_
