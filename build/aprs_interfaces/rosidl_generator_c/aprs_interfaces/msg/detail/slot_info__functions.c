// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from aprs_interfaces:msg/SlotInfo.idl
// generated code does not contain a copyright notice
#include "aprs_interfaces/msg/detail/slot_info__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `name`
#include "rosidl_runtime_c/string_functions.h"

bool
aprs_interfaces__msg__SlotInfo__init(aprs_interfaces__msg__SlotInfo * msg)
{
  if (!msg) {
    return false;
  }
  // occupied
  // size
  // name
  if (!rosidl_runtime_c__String__init(&msg->name)) {
    aprs_interfaces__msg__SlotInfo__fini(msg);
    return false;
  }
  return true;
}

void
aprs_interfaces__msg__SlotInfo__fini(aprs_interfaces__msg__SlotInfo * msg)
{
  if (!msg) {
    return;
  }
  // occupied
  // size
  // name
  rosidl_runtime_c__String__fini(&msg->name);
}

bool
aprs_interfaces__msg__SlotInfo__are_equal(const aprs_interfaces__msg__SlotInfo * lhs, const aprs_interfaces__msg__SlotInfo * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // occupied
  if (lhs->occupied != rhs->occupied) {
    return false;
  }
  // size
  if (lhs->size != rhs->size) {
    return false;
  }
  // name
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->name), &(rhs->name)))
  {
    return false;
  }
  return true;
}

bool
aprs_interfaces__msg__SlotInfo__copy(
  const aprs_interfaces__msg__SlotInfo * input,
  aprs_interfaces__msg__SlotInfo * output)
{
  if (!input || !output) {
    return false;
  }
  // occupied
  output->occupied = input->occupied;
  // size
  output->size = input->size;
  // name
  if (!rosidl_runtime_c__String__copy(
      &(input->name), &(output->name)))
  {
    return false;
  }
  return true;
}

aprs_interfaces__msg__SlotInfo *
aprs_interfaces__msg__SlotInfo__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aprs_interfaces__msg__SlotInfo * msg = (aprs_interfaces__msg__SlotInfo *)allocator.allocate(sizeof(aprs_interfaces__msg__SlotInfo), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(aprs_interfaces__msg__SlotInfo));
  bool success = aprs_interfaces__msg__SlotInfo__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
aprs_interfaces__msg__SlotInfo__destroy(aprs_interfaces__msg__SlotInfo * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    aprs_interfaces__msg__SlotInfo__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
aprs_interfaces__msg__SlotInfo__Sequence__init(aprs_interfaces__msg__SlotInfo__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aprs_interfaces__msg__SlotInfo * data = NULL;

  if (size) {
    data = (aprs_interfaces__msg__SlotInfo *)allocator.zero_allocate(size, sizeof(aprs_interfaces__msg__SlotInfo), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = aprs_interfaces__msg__SlotInfo__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        aprs_interfaces__msg__SlotInfo__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
aprs_interfaces__msg__SlotInfo__Sequence__fini(aprs_interfaces__msg__SlotInfo__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      aprs_interfaces__msg__SlotInfo__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

aprs_interfaces__msg__SlotInfo__Sequence *
aprs_interfaces__msg__SlotInfo__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aprs_interfaces__msg__SlotInfo__Sequence * array = (aprs_interfaces__msg__SlotInfo__Sequence *)allocator.allocate(sizeof(aprs_interfaces__msg__SlotInfo__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = aprs_interfaces__msg__SlotInfo__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
aprs_interfaces__msg__SlotInfo__Sequence__destroy(aprs_interfaces__msg__SlotInfo__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    aprs_interfaces__msg__SlotInfo__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
aprs_interfaces__msg__SlotInfo__Sequence__are_equal(const aprs_interfaces__msg__SlotInfo__Sequence * lhs, const aprs_interfaces__msg__SlotInfo__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!aprs_interfaces__msg__SlotInfo__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
aprs_interfaces__msg__SlotInfo__Sequence__copy(
  const aprs_interfaces__msg__SlotInfo__Sequence * input,
  aprs_interfaces__msg__SlotInfo__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(aprs_interfaces__msg__SlotInfo);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    aprs_interfaces__msg__SlotInfo * data =
      (aprs_interfaces__msg__SlotInfo *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!aprs_interfaces__msg__SlotInfo__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          aprs_interfaces__msg__SlotInfo__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!aprs_interfaces__msg__SlotInfo__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
