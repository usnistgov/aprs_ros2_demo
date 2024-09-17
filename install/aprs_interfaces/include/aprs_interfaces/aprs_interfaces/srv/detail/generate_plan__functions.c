// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from aprs_interfaces:srv/GeneratePlan.idl
// generated code does not contain a copyright notice
#include "aprs_interfaces/srv/detail/generate_plan__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

bool
aprs_interfaces__srv__GeneratePlan_Request__init(aprs_interfaces__srv__GeneratePlan_Request * msg)
{
  if (!msg) {
    return false;
  }
  // structure_needs_at_least_one_member
  return true;
}

void
aprs_interfaces__srv__GeneratePlan_Request__fini(aprs_interfaces__srv__GeneratePlan_Request * msg)
{
  if (!msg) {
    return;
  }
  // structure_needs_at_least_one_member
}

bool
aprs_interfaces__srv__GeneratePlan_Request__are_equal(const aprs_interfaces__srv__GeneratePlan_Request * lhs, const aprs_interfaces__srv__GeneratePlan_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // structure_needs_at_least_one_member
  if (lhs->structure_needs_at_least_one_member != rhs->structure_needs_at_least_one_member) {
    return false;
  }
  return true;
}

bool
aprs_interfaces__srv__GeneratePlan_Request__copy(
  const aprs_interfaces__srv__GeneratePlan_Request * input,
  aprs_interfaces__srv__GeneratePlan_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // structure_needs_at_least_one_member
  output->structure_needs_at_least_one_member = input->structure_needs_at_least_one_member;
  return true;
}

aprs_interfaces__srv__GeneratePlan_Request *
aprs_interfaces__srv__GeneratePlan_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aprs_interfaces__srv__GeneratePlan_Request * msg = (aprs_interfaces__srv__GeneratePlan_Request *)allocator.allocate(sizeof(aprs_interfaces__srv__GeneratePlan_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(aprs_interfaces__srv__GeneratePlan_Request));
  bool success = aprs_interfaces__srv__GeneratePlan_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
aprs_interfaces__srv__GeneratePlan_Request__destroy(aprs_interfaces__srv__GeneratePlan_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    aprs_interfaces__srv__GeneratePlan_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
aprs_interfaces__srv__GeneratePlan_Request__Sequence__init(aprs_interfaces__srv__GeneratePlan_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aprs_interfaces__srv__GeneratePlan_Request * data = NULL;

  if (size) {
    data = (aprs_interfaces__srv__GeneratePlan_Request *)allocator.zero_allocate(size, sizeof(aprs_interfaces__srv__GeneratePlan_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = aprs_interfaces__srv__GeneratePlan_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        aprs_interfaces__srv__GeneratePlan_Request__fini(&data[i - 1]);
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
aprs_interfaces__srv__GeneratePlan_Request__Sequence__fini(aprs_interfaces__srv__GeneratePlan_Request__Sequence * array)
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
      aprs_interfaces__srv__GeneratePlan_Request__fini(&array->data[i]);
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

aprs_interfaces__srv__GeneratePlan_Request__Sequence *
aprs_interfaces__srv__GeneratePlan_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aprs_interfaces__srv__GeneratePlan_Request__Sequence * array = (aprs_interfaces__srv__GeneratePlan_Request__Sequence *)allocator.allocate(sizeof(aprs_interfaces__srv__GeneratePlan_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = aprs_interfaces__srv__GeneratePlan_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
aprs_interfaces__srv__GeneratePlan_Request__Sequence__destroy(aprs_interfaces__srv__GeneratePlan_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    aprs_interfaces__srv__GeneratePlan_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
aprs_interfaces__srv__GeneratePlan_Request__Sequence__are_equal(const aprs_interfaces__srv__GeneratePlan_Request__Sequence * lhs, const aprs_interfaces__srv__GeneratePlan_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!aprs_interfaces__srv__GeneratePlan_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
aprs_interfaces__srv__GeneratePlan_Request__Sequence__copy(
  const aprs_interfaces__srv__GeneratePlan_Request__Sequence * input,
  aprs_interfaces__srv__GeneratePlan_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(aprs_interfaces__srv__GeneratePlan_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    aprs_interfaces__srv__GeneratePlan_Request * data =
      (aprs_interfaces__srv__GeneratePlan_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!aprs_interfaces__srv__GeneratePlan_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          aprs_interfaces__srv__GeneratePlan_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!aprs_interfaces__srv__GeneratePlan_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `plan`
// Member `status`
#include "rosidl_runtime_c/string_functions.h"

bool
aprs_interfaces__srv__GeneratePlan_Response__init(aprs_interfaces__srv__GeneratePlan_Response * msg)
{
  if (!msg) {
    return false;
  }
  // success
  // plan
  if (!rosidl_runtime_c__String__init(&msg->plan)) {
    aprs_interfaces__srv__GeneratePlan_Response__fini(msg);
    return false;
  }
  // status
  if (!rosidl_runtime_c__String__init(&msg->status)) {
    aprs_interfaces__srv__GeneratePlan_Response__fini(msg);
    return false;
  }
  return true;
}

void
aprs_interfaces__srv__GeneratePlan_Response__fini(aprs_interfaces__srv__GeneratePlan_Response * msg)
{
  if (!msg) {
    return;
  }
  // success
  // plan
  rosidl_runtime_c__String__fini(&msg->plan);
  // status
  rosidl_runtime_c__String__fini(&msg->status);
}

bool
aprs_interfaces__srv__GeneratePlan_Response__are_equal(const aprs_interfaces__srv__GeneratePlan_Response * lhs, const aprs_interfaces__srv__GeneratePlan_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // success
  if (lhs->success != rhs->success) {
    return false;
  }
  // plan
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->plan), &(rhs->plan)))
  {
    return false;
  }
  // status
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->status), &(rhs->status)))
  {
    return false;
  }
  return true;
}

bool
aprs_interfaces__srv__GeneratePlan_Response__copy(
  const aprs_interfaces__srv__GeneratePlan_Response * input,
  aprs_interfaces__srv__GeneratePlan_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // success
  output->success = input->success;
  // plan
  if (!rosidl_runtime_c__String__copy(
      &(input->plan), &(output->plan)))
  {
    return false;
  }
  // status
  if (!rosidl_runtime_c__String__copy(
      &(input->status), &(output->status)))
  {
    return false;
  }
  return true;
}

aprs_interfaces__srv__GeneratePlan_Response *
aprs_interfaces__srv__GeneratePlan_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aprs_interfaces__srv__GeneratePlan_Response * msg = (aprs_interfaces__srv__GeneratePlan_Response *)allocator.allocate(sizeof(aprs_interfaces__srv__GeneratePlan_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(aprs_interfaces__srv__GeneratePlan_Response));
  bool success = aprs_interfaces__srv__GeneratePlan_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
aprs_interfaces__srv__GeneratePlan_Response__destroy(aprs_interfaces__srv__GeneratePlan_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    aprs_interfaces__srv__GeneratePlan_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
aprs_interfaces__srv__GeneratePlan_Response__Sequence__init(aprs_interfaces__srv__GeneratePlan_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aprs_interfaces__srv__GeneratePlan_Response * data = NULL;

  if (size) {
    data = (aprs_interfaces__srv__GeneratePlan_Response *)allocator.zero_allocate(size, sizeof(aprs_interfaces__srv__GeneratePlan_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = aprs_interfaces__srv__GeneratePlan_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        aprs_interfaces__srv__GeneratePlan_Response__fini(&data[i - 1]);
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
aprs_interfaces__srv__GeneratePlan_Response__Sequence__fini(aprs_interfaces__srv__GeneratePlan_Response__Sequence * array)
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
      aprs_interfaces__srv__GeneratePlan_Response__fini(&array->data[i]);
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

aprs_interfaces__srv__GeneratePlan_Response__Sequence *
aprs_interfaces__srv__GeneratePlan_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aprs_interfaces__srv__GeneratePlan_Response__Sequence * array = (aprs_interfaces__srv__GeneratePlan_Response__Sequence *)allocator.allocate(sizeof(aprs_interfaces__srv__GeneratePlan_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = aprs_interfaces__srv__GeneratePlan_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
aprs_interfaces__srv__GeneratePlan_Response__Sequence__destroy(aprs_interfaces__srv__GeneratePlan_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    aprs_interfaces__srv__GeneratePlan_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
aprs_interfaces__srv__GeneratePlan_Response__Sequence__are_equal(const aprs_interfaces__srv__GeneratePlan_Response__Sequence * lhs, const aprs_interfaces__srv__GeneratePlan_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!aprs_interfaces__srv__GeneratePlan_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
aprs_interfaces__srv__GeneratePlan_Response__Sequence__copy(
  const aprs_interfaces__srv__GeneratePlan_Response__Sequence * input,
  aprs_interfaces__srv__GeneratePlan_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(aprs_interfaces__srv__GeneratePlan_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    aprs_interfaces__srv__GeneratePlan_Response * data =
      (aprs_interfaces__srv__GeneratePlan_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!aprs_interfaces__srv__GeneratePlan_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          aprs_interfaces__srv__GeneratePlan_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!aprs_interfaces__srv__GeneratePlan_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `info`
#include "service_msgs/msg/detail/service_event_info__functions.h"
// Member `request`
// Member `response`
// already included above
// #include "aprs_interfaces/srv/detail/generate_plan__functions.h"

bool
aprs_interfaces__srv__GeneratePlan_Event__init(aprs_interfaces__srv__GeneratePlan_Event * msg)
{
  if (!msg) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__init(&msg->info)) {
    aprs_interfaces__srv__GeneratePlan_Event__fini(msg);
    return false;
  }
  // request
  if (!aprs_interfaces__srv__GeneratePlan_Request__Sequence__init(&msg->request, 0)) {
    aprs_interfaces__srv__GeneratePlan_Event__fini(msg);
    return false;
  }
  // response
  if (!aprs_interfaces__srv__GeneratePlan_Response__Sequence__init(&msg->response, 0)) {
    aprs_interfaces__srv__GeneratePlan_Event__fini(msg);
    return false;
  }
  return true;
}

void
aprs_interfaces__srv__GeneratePlan_Event__fini(aprs_interfaces__srv__GeneratePlan_Event * msg)
{
  if (!msg) {
    return;
  }
  // info
  service_msgs__msg__ServiceEventInfo__fini(&msg->info);
  // request
  aprs_interfaces__srv__GeneratePlan_Request__Sequence__fini(&msg->request);
  // response
  aprs_interfaces__srv__GeneratePlan_Response__Sequence__fini(&msg->response);
}

bool
aprs_interfaces__srv__GeneratePlan_Event__are_equal(const aprs_interfaces__srv__GeneratePlan_Event * lhs, const aprs_interfaces__srv__GeneratePlan_Event * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__are_equal(
      &(lhs->info), &(rhs->info)))
  {
    return false;
  }
  // request
  if (!aprs_interfaces__srv__GeneratePlan_Request__Sequence__are_equal(
      &(lhs->request), &(rhs->request)))
  {
    return false;
  }
  // response
  if (!aprs_interfaces__srv__GeneratePlan_Response__Sequence__are_equal(
      &(lhs->response), &(rhs->response)))
  {
    return false;
  }
  return true;
}

bool
aprs_interfaces__srv__GeneratePlan_Event__copy(
  const aprs_interfaces__srv__GeneratePlan_Event * input,
  aprs_interfaces__srv__GeneratePlan_Event * output)
{
  if (!input || !output) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__copy(
      &(input->info), &(output->info)))
  {
    return false;
  }
  // request
  if (!aprs_interfaces__srv__GeneratePlan_Request__Sequence__copy(
      &(input->request), &(output->request)))
  {
    return false;
  }
  // response
  if (!aprs_interfaces__srv__GeneratePlan_Response__Sequence__copy(
      &(input->response), &(output->response)))
  {
    return false;
  }
  return true;
}

aprs_interfaces__srv__GeneratePlan_Event *
aprs_interfaces__srv__GeneratePlan_Event__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aprs_interfaces__srv__GeneratePlan_Event * msg = (aprs_interfaces__srv__GeneratePlan_Event *)allocator.allocate(sizeof(aprs_interfaces__srv__GeneratePlan_Event), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(aprs_interfaces__srv__GeneratePlan_Event));
  bool success = aprs_interfaces__srv__GeneratePlan_Event__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
aprs_interfaces__srv__GeneratePlan_Event__destroy(aprs_interfaces__srv__GeneratePlan_Event * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    aprs_interfaces__srv__GeneratePlan_Event__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
aprs_interfaces__srv__GeneratePlan_Event__Sequence__init(aprs_interfaces__srv__GeneratePlan_Event__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aprs_interfaces__srv__GeneratePlan_Event * data = NULL;

  if (size) {
    data = (aprs_interfaces__srv__GeneratePlan_Event *)allocator.zero_allocate(size, sizeof(aprs_interfaces__srv__GeneratePlan_Event), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = aprs_interfaces__srv__GeneratePlan_Event__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        aprs_interfaces__srv__GeneratePlan_Event__fini(&data[i - 1]);
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
aprs_interfaces__srv__GeneratePlan_Event__Sequence__fini(aprs_interfaces__srv__GeneratePlan_Event__Sequence * array)
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
      aprs_interfaces__srv__GeneratePlan_Event__fini(&array->data[i]);
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

aprs_interfaces__srv__GeneratePlan_Event__Sequence *
aprs_interfaces__srv__GeneratePlan_Event__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  aprs_interfaces__srv__GeneratePlan_Event__Sequence * array = (aprs_interfaces__srv__GeneratePlan_Event__Sequence *)allocator.allocate(sizeof(aprs_interfaces__srv__GeneratePlan_Event__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = aprs_interfaces__srv__GeneratePlan_Event__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
aprs_interfaces__srv__GeneratePlan_Event__Sequence__destroy(aprs_interfaces__srv__GeneratePlan_Event__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    aprs_interfaces__srv__GeneratePlan_Event__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
aprs_interfaces__srv__GeneratePlan_Event__Sequence__are_equal(const aprs_interfaces__srv__GeneratePlan_Event__Sequence * lhs, const aprs_interfaces__srv__GeneratePlan_Event__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!aprs_interfaces__srv__GeneratePlan_Event__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
aprs_interfaces__srv__GeneratePlan_Event__Sequence__copy(
  const aprs_interfaces__srv__GeneratePlan_Event__Sequence * input,
  aprs_interfaces__srv__GeneratePlan_Event__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(aprs_interfaces__srv__GeneratePlan_Event);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    aprs_interfaces__srv__GeneratePlan_Event * data =
      (aprs_interfaces__srv__GeneratePlan_Event *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!aprs_interfaces__srv__GeneratePlan_Event__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          aprs_interfaces__srv__GeneratePlan_Event__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!aprs_interfaces__srv__GeneratePlan_Event__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
