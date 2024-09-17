// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from aprs_interfaces:msg/Object.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__OBJECT__STRUCT_HPP_
#define APRS_INTERFACES__MSG__DETAIL__OBJECT__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'pose_stamped'
#include "geometry_msgs/msg/detail/pose_stamped__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__aprs_interfaces__msg__Object __attribute__((deprecated))
#else
# define DEPRECATED__aprs_interfaces__msg__Object __declspec(deprecated)
#endif

namespace aprs_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Object_
{
  using Type = Object_<ContainerAllocator>;

  explicit Object_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : pose_stamped(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->name = "";
      this->object_type = 0;
      this->object_identifier = 0;
    }
  }

  explicit Object_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : name(_alloc),
    pose_stamped(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->name = "";
      this->object_type = 0;
      this->object_identifier = 0;
    }
  }

  // field types and members
  using _name_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _name_type name;
  using _object_type_type =
    uint8_t;
  _object_type_type object_type;
  using _object_identifier_type =
    uint8_t;
  _object_identifier_type object_identifier;
  using _pose_stamped_type =
    geometry_msgs::msg::PoseStamped_<ContainerAllocator>;
  _pose_stamped_type pose_stamped;

  // setters for named parameter idiom
  Type & set__name(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->name = _arg;
    return *this;
  }
  Type & set__object_type(
    const uint8_t & _arg)
  {
    this->object_type = _arg;
    return *this;
  }
  Type & set__object_identifier(
    const uint8_t & _arg)
  {
    this->object_identifier = _arg;
    return *this;
  }
  Type & set__pose_stamped(
    const geometry_msgs::msg::PoseStamped_<ContainerAllocator> & _arg)
  {
    this->pose_stamped = _arg;
    return *this;
  }

  // constant declarations
  static constexpr uint8_t KIT_TRAY =
    1u;
  static constexpr uint8_t PART_TRAY =
    2u;
  static constexpr uint8_t PART =
    3u;
  static constexpr uint8_t SMALL_GEAR =
    10u;
  static constexpr uint8_t MEDIUM_GEAR =
    11u;
  static constexpr uint8_t LARGE_GEAR =
    12u;
  static constexpr uint8_t SMALL_GEAR_TRAY =
    13u;
  static constexpr uint8_t MEDIUM_GEAR_TRAY =
    14u;
  static constexpr uint8_t LARGE_GEAR_TRAY =
    15u;
  static constexpr uint8_t M2L1_KIT_TRAY =
    16u;
  static constexpr uint8_t S2L2_KIT_TRAY =
    17u;

  // pointer types
  using RawPtr =
    aprs_interfaces::msg::Object_<ContainerAllocator> *;
  using ConstRawPtr =
    const aprs_interfaces::msg::Object_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<aprs_interfaces::msg::Object_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<aprs_interfaces::msg::Object_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      aprs_interfaces::msg::Object_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<aprs_interfaces::msg::Object_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      aprs_interfaces::msg::Object_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<aprs_interfaces::msg::Object_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<aprs_interfaces::msg::Object_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<aprs_interfaces::msg::Object_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__aprs_interfaces__msg__Object
    std::shared_ptr<aprs_interfaces::msg::Object_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__aprs_interfaces__msg__Object
    std::shared_ptr<aprs_interfaces::msg::Object_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Object_ & other) const
  {
    if (this->name != other.name) {
      return false;
    }
    if (this->object_type != other.object_type) {
      return false;
    }
    if (this->object_identifier != other.object_identifier) {
      return false;
    }
    if (this->pose_stamped != other.pose_stamped) {
      return false;
    }
    return true;
  }
  bool operator!=(const Object_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Object_

// alias to use template instance with default allocator
using Object =
  aprs_interfaces::msg::Object_<std::allocator<void>>;

// constant definitions
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t Object_<ContainerAllocator>::KIT_TRAY;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t Object_<ContainerAllocator>::PART_TRAY;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t Object_<ContainerAllocator>::PART;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t Object_<ContainerAllocator>::SMALL_GEAR;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t Object_<ContainerAllocator>::MEDIUM_GEAR;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t Object_<ContainerAllocator>::LARGE_GEAR;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t Object_<ContainerAllocator>::SMALL_GEAR_TRAY;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t Object_<ContainerAllocator>::MEDIUM_GEAR_TRAY;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t Object_<ContainerAllocator>::LARGE_GEAR_TRAY;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t Object_<ContainerAllocator>::M2L1_KIT_TRAY;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr uint8_t Object_<ContainerAllocator>::S2L2_KIT_TRAY;
#endif  // __cplusplus < 201703L

}  // namespace msg

}  // namespace aprs_interfaces

#endif  // APRS_INTERFACES__MSG__DETAIL__OBJECT__STRUCT_HPP_
