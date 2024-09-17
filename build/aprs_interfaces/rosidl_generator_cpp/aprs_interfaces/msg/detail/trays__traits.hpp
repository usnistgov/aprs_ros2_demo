// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from aprs_interfaces:msg/Trays.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__TRAYS__TRAITS_HPP_
#define APRS_INTERFACES__MSG__DETAIL__TRAYS__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "aprs_interfaces/msg/detail/trays__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'kit_trays'
// Member 'part_trays'
#include "aprs_interfaces/msg/detail/tray__traits.hpp"

namespace aprs_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const Trays & msg,
  std::ostream & out)
{
  out << "{";
  // member: kit_trays
  {
    if (msg.kit_trays.size() == 0) {
      out << "kit_trays: []";
    } else {
      out << "kit_trays: [";
      size_t pending_items = msg.kit_trays.size();
      for (auto item : msg.kit_trays) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: part_trays
  {
    if (msg.part_trays.size() == 0) {
      out << "part_trays: []";
    } else {
      out << "part_trays: [";
      size_t pending_items = msg.part_trays.size();
      for (auto item : msg.part_trays) {
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
  const Trays & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: kit_trays
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.kit_trays.size() == 0) {
      out << "kit_trays: []\n";
    } else {
      out << "kit_trays:\n";
      for (auto item : msg.kit_trays) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: part_trays
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.part_trays.size() == 0) {
      out << "part_trays: []\n";
    } else {
      out << "part_trays:\n";
      for (auto item : msg.part_trays) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Trays & msg, bool use_flow_style = false)
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
  const aprs_interfaces::msg::Trays & msg,
  std::ostream & out, size_t indentation = 0)
{
  aprs_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use aprs_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const aprs_interfaces::msg::Trays & msg)
{
  return aprs_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<aprs_interfaces::msg::Trays>()
{
  return "aprs_interfaces::msg::Trays";
}

template<>
inline const char * name<aprs_interfaces::msg::Trays>()
{
  return "aprs_interfaces/msg/Trays";
}

template<>
struct has_fixed_size<aprs_interfaces::msg::Trays>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<aprs_interfaces::msg::Trays>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<aprs_interfaces::msg::Trays>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // APRS_INTERFACES__MSG__DETAIL__TRAYS__TRAITS_HPP_
