// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from aprs_interfaces:msg/SlotInfo.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__SLOT_INFO__STRUCT_HPP_
#define APRS_INTERFACES__MSG__DETAIL__SLOT_INFO__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__aprs_interfaces__msg__SlotInfo __attribute__((deprecated))
#else
# define DEPRECATED__aprs_interfaces__msg__SlotInfo __declspec(deprecated)
#endif

namespace aprs_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct SlotInfo_
{
  using Type = SlotInfo_<ContainerAllocator>;

  explicit SlotInfo_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->occupied = false;
      this->size = 0;
      this->name = "";
    }
  }

  explicit SlotInfo_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : name(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->occupied = false;
      this->size = 0;
      this->name = "";
    }
  }

  // field types and members
  using _occupied_type =
    bool;
  _occupied_type occupied;
  using _size_type =
    uint8_t;
  _size_type size;
  using _name_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _name_type name;

  // setters for named parameter idiom
  Type & set__occupied(
    const bool & _arg)
  {
    this->occupied = _arg;
    return *this;
  }
  Type & set__size(
    const uint8_t & _arg)
  {
    this->size = _arg;
    return *this;
  }
  Type & set__name(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->name = _arg;
    return *this;
  }

  // constant declarations
  static constexpr uint8_t SMALL =
    1u;
  static constexpr uint8_t MEDIUM =
    2u;
  static constexpr uint8_t LARGE =
    3u;

  // pointer types
  using RawPtr =
    aprs_interfaces::msg::SlotInfo_<ContainerAllocator> *;
  using ConstRawPtr =
    const aprs_interfaces::msg::SlotInfo_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<aprs_interfaces::msg::SlotInfo_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<aprs_interfaces::msg::SlotInfo_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      aprs_interfaces::msg::SlotInfo_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<aprs_interfaces::msg::SlotInfo_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      aprs_interfaces::msg::SlotInfo_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<aprs_interfaces::msg::SlotInfo_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<aprs_interfaces::msg::SlotInfo_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<aprs_interfaces::msg::SlotInfo_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__aprs_interfaces__msg__SlotInfo
    std::shared_ptr<aprs_interfaces::msg::SlotInfo_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__aprs_interfaces__msg__SlotInfo
    std::shared_ptr<aprs_interfaces::msg::SlotInfo_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SlotInfo_ & other) const
  {
    if (this->occupied != other.occupied) {
      return false;
    }
    if (this->size != other.size) {
      return false;
    }
    if (this->name != other.name) {
      return false;
    }
    return true;
  }
  bool operator!=(const SlotInfo_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SlotInfo_

// alias to use template instance with default allocator
using SlotInfo =
  aprs_interfaces::msg::SlotInfo_<std::allocator<void>>;

// constant definitions
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t SlotInfo_<ContainerAllocator>::SMALL;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t SlotInfo_<ContainerAllocator>::MEDIUM;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t SlotInfo_<ContainerAllocator>::LARGE;
#endif  // __cplusplus < 201703L

}  // namespace msg

}  // namespace aprs_interfaces

#endif  // APRS_INTERFACES__MSG__DETAIL__SLOT_INFO__STRUCT_HPP_
