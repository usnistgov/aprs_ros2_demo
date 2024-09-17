// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from aprs_interfaces:msg/Tray.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__TRAY__STRUCT_HPP_
#define APRS_INTERFACES__MSG__DETAIL__TRAY__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'slots'
#include "aprs_interfaces/msg/detail/slot_info__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__aprs_interfaces__msg__Tray __attribute__((deprecated))
#else
# define DEPRECATED__aprs_interfaces__msg__Tray __declspec(deprecated)
#endif

namespace aprs_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Tray_
{
  using Type = Tray_<ContainerAllocator>;

  explicit Tray_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->identifier = 0;
      this->name = "";
    }
  }

  explicit Tray_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : name(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->identifier = 0;
      this->name = "";
    }
  }

  // field types and members
  using _identifier_type =
    uint8_t;
  _identifier_type identifier;
  using _name_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _name_type name;
  using _slots_type =
    std::vector<aprs_interfaces::msg::SlotInfo_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<aprs_interfaces::msg::SlotInfo_<ContainerAllocator>>>;
  _slots_type slots;

  // setters for named parameter idiom
  Type & set__identifier(
    const uint8_t & _arg)
  {
    this->identifier = _arg;
    return *this;
  }
  Type & set__name(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->name = _arg;
    return *this;
  }
  Type & set__slots(
    const std::vector<aprs_interfaces::msg::SlotInfo_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<aprs_interfaces::msg::SlotInfo_<ContainerAllocator>>> & _arg)
  {
    this->slots = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    aprs_interfaces::msg::Tray_<ContainerAllocator> *;
  using ConstRawPtr =
    const aprs_interfaces::msg::Tray_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<aprs_interfaces::msg::Tray_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<aprs_interfaces::msg::Tray_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      aprs_interfaces::msg::Tray_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<aprs_interfaces::msg::Tray_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      aprs_interfaces::msg::Tray_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<aprs_interfaces::msg::Tray_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<aprs_interfaces::msg::Tray_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<aprs_interfaces::msg::Tray_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__aprs_interfaces__msg__Tray
    std::shared_ptr<aprs_interfaces::msg::Tray_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__aprs_interfaces__msg__Tray
    std::shared_ptr<aprs_interfaces::msg::Tray_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Tray_ & other) const
  {
    if (this->identifier != other.identifier) {
      return false;
    }
    if (this->name != other.name) {
      return false;
    }
    if (this->slots != other.slots) {
      return false;
    }
    return true;
  }
  bool operator!=(const Tray_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Tray_

// alias to use template instance with default allocator
using Tray =
  aprs_interfaces::msg::Tray_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace aprs_interfaces

#endif  // APRS_INTERFACES__MSG__DETAIL__TRAY__STRUCT_HPP_
