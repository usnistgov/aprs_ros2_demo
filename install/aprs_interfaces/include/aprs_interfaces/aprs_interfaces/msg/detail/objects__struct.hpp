// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from aprs_interfaces:msg/Objects.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__OBJECTS__STRUCT_HPP_
#define APRS_INTERFACES__MSG__DETAIL__OBJECTS__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'objects'
#include "aprs_interfaces/msg/detail/object__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__aprs_interfaces__msg__Objects __attribute__((deprecated))
#else
# define DEPRECATED__aprs_interfaces__msg__Objects __declspec(deprecated)
#endif

namespace aprs_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Objects_
{
  using Type = Objects_<ContainerAllocator>;

  explicit Objects_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
  }

  explicit Objects_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
    (void)_alloc;
  }

  // field types and members
  using _objects_type =
    std::vector<aprs_interfaces::msg::Object_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<aprs_interfaces::msg::Object_<ContainerAllocator>>>;
  _objects_type objects;

  // setters for named parameter idiom
  Type & set__objects(
    const std::vector<aprs_interfaces::msg::Object_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<aprs_interfaces::msg::Object_<ContainerAllocator>>> & _arg)
  {
    this->objects = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    aprs_interfaces::msg::Objects_<ContainerAllocator> *;
  using ConstRawPtr =
    const aprs_interfaces::msg::Objects_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<aprs_interfaces::msg::Objects_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<aprs_interfaces::msg::Objects_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      aprs_interfaces::msg::Objects_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<aprs_interfaces::msg::Objects_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      aprs_interfaces::msg::Objects_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<aprs_interfaces::msg::Objects_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<aprs_interfaces::msg::Objects_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<aprs_interfaces::msg::Objects_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__aprs_interfaces__msg__Objects
    std::shared_ptr<aprs_interfaces::msg::Objects_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__aprs_interfaces__msg__Objects
    std::shared_ptr<aprs_interfaces::msg::Objects_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Objects_ & other) const
  {
    if (this->objects != other.objects) {
      return false;
    }
    return true;
  }
  bool operator!=(const Objects_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Objects_

// alias to use template instance with default allocator
using Objects =
  aprs_interfaces::msg::Objects_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace aprs_interfaces

#endif  // APRS_INTERFACES__MSG__DETAIL__OBJECTS__STRUCT_HPP_
