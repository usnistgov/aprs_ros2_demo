// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from aprs_interfaces:msg/Object.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__OBJECT__TRAITS_HPP_
#define APRS_INTERFACES__MSG__DETAIL__OBJECT__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "aprs_interfaces/msg/detail/object__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'pose_stamped'
#include "geometry_msgs/msg/detail/pose_stamped__traits.hpp"

namespace aprs_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const Object & msg,
  std::ostream & out)
{
  out << "{";
  // member: name
  {
    out << "name: ";
    rosidl_generator_traits::value_to_yaml(msg.name, out);
    out << ", ";
  }

  // member: object_type
  {
    out << "object_type: ";
    rosidl_generator_traits::value_to_yaml(msg.object_type, out);
    out << ", ";
  }

  // member: object_identifier
  {
    out << "object_identifier: ";
    rosidl_generator_traits::value_to_yaml(msg.object_identifier, out);
    out << ", ";
  }

  // member: pose_stamped
  {
    out << "pose_stamped: ";
    to_flow_style_yaml(msg.pose_stamped, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Object & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: name
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "name: ";
    rosidl_generator_traits::value_to_yaml(msg.name, out);
    out << "\n";
  }

  // member: object_type
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "object_type: ";
    rosidl_generator_traits::value_to_yaml(msg.object_type, out);
    out << "\n";
  }

  // member: object_identifier
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "object_identifier: ";
    rosidl_generator_traits::value_to_yaml(msg.object_identifier, out);
    out << "\n";
  }

  // member: pose_stamped
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "pose_stamped:\n";
    to_block_style_yaml(msg.pose_stamped, out, indentation + 2);
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Object & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace aprs_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use aprs_interfaces::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const aprs_interfaces::msg::Object & msg,
  std::ostream & out, size_t indentation = 0)
{
  aprs_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use aprs_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const aprs_interfaces::msg::Object & msg)
{
  return aprs_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<aprs_interfaces::msg::Object>()
{
  return "aprs_interfaces::msg::Object";
}

template<>
inline const char * name<aprs_interfaces::msg::Object>()
{
  return "aprs_interfaces/msg/Object";
}

template<>
struct has_fixed_size<aprs_interfaces::msg::Object>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<aprs_interfaces::msg::Object>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<aprs_interfaces::msg::Object>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // APRS_INTERFACES__MSG__DETAIL__OBJECT__TRAITS_HPP_
