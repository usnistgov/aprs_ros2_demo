// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from aprs_interfaces:msg/Trays.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__TRAYS__STRUCT_HPP_
#define APRS_INTERFACES__MSG__DETAIL__TRAYS__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'kit_trays'
// Member 'part_trays'
#include "aprs_interfaces/msg/detail/tray__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__aprs_interfaces__msg__Trays __attribute__((deprecated))
#else
# define DEPRECATED__aprs_interfaces__msg__Trays __declspec(deprecated)
#endif

namespace aprs_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Trays_
{
  using Type = Trays_<ContainerAllocator>;

  explicit Trays_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
  }

  explicit Trays_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
    (void)_alloc;
  }

  // field types and members
  using _kit_trays_type =
    std::vector<aprs_interfaces::msg::Tray_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<aprs_interfaces::msg::Tray_<ContainerAllocator>>>;
  _kit_trays_type kit_trays;
  using _part_trays_type =
    std::vector<aprs_interfaces::msg::Tray_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<aprs_interfaces::msg::Tray_<ContainerAllocator>>>;
  _part_trays_type part_trays;

  // setters for named parameter idiom
  Type & set__kit_trays(
    const std::vector<aprs_interfaces::msg::Tray_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<aprs_interfaces::msg::Tray_<ContainerAllocator>>> & _arg)
  {
    this->kit_trays = _arg;
    return *this;
  }
  Type & set__part_trays(
    const std::vector<aprs_interfaces::msg::Tray_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<aprs_interfaces::msg::Tray_<ContainerAllocator>>> & _arg)
  {
    this->part_trays = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    aprs_interfaces::msg::Trays_<ContainerAllocator> *;
  using ConstRawPtr =
    const aprs_interfaces::msg::Trays_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<aprs_interfaces::msg::Trays_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<aprs_interfaces::msg::Trays_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      aprs_interfaces::msg::Trays_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<aprs_interfaces::msg::Trays_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      aprs_interfaces::msg::Trays_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<aprs_interfaces::msg::Trays_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<aprs_interfaces::msg::Trays_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<aprs_interfaces::msg::Trays_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__aprs_interfaces__msg__Trays
    std::shared_ptr<aprs_interfaces::msg::Trays_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__aprs_interfaces__msg__Trays
    std::shared_ptr<aprs_interfaces::msg::Trays_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Trays_ & other) const
  {
    if (this->kit_trays != other.kit_trays) {
      return false;
    }
    if (this->part_trays != other.part_trays) {
      return false;
    }
    return true;
  }
  bool operator!=(const Trays_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Trays_

// alias to use template instance with default allocator
using Trays =
  aprs_interfaces::msg::Trays_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace aprs_interfaces

#endif  // APRS_INTERFACES__MSG__DETAIL__TRAYS__STRUCT_HPP_
