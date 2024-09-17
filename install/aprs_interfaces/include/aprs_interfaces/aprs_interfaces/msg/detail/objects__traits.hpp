// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from aprs_interfaces:msg/Objects.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__OBJECTS__TRAITS_HPP_
#define APRS_INTERFACES__MSG__DETAIL__OBJECTS__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "aprs_interfaces/msg/detail/objects__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'objects'
#include "aprs_interfaces/msg/detail/object__traits.hpp"

namespace aprs_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const Objects & msg,
  std::ostream & out)
{
  out << "{";
  // member: objects
  {
    if (msg.objects.size() == 0) {
      out << "objects: []";
    } else {
      out << "objects: [";
      size_t pending_items = msg.objects.size();
      for (auto item : msg.objects) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Objects & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: objects
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.objects.size() == 0) {
      out << "objects: []\n";
    } else {
      out << "objects:\n";
      for (auto item : msg.objects) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Objects & msg, bool use_flow_style = false)
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
  const aprs_interfaces::msg::Objects & msg,
  std::ostream & out, size_t indentation = 0)
{
  aprs_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use aprs_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const aprs_interfaces::msg::Objects & msg)
{
  return aprs_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<aprs_interfaces::msg::Objects>()
{
  return "aprs_interfaces::msg::Objects";
}

template<>
inline const char * name<aprs_interfaces::msg::Objects>()
{
  return "aprs_interfaces/msg/Objects";
}

template<>
struct has_fixed_size<aprs_interfaces::msg::Objects>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<aprs_interfaces::msg::Objects>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<aprs_interfaces::msg::Objects>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // APRS_INTERFACES__MSG__DETAIL__OBJECTS__TRAITS_HPP_
