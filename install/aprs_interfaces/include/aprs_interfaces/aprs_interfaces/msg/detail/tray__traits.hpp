// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from aprs_interfaces:msg/Tray.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__TRAY__TRAITS_HPP_
#define APRS_INTERFACES__MSG__DETAIL__TRAY__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "aprs_interfaces/msg/detail/tray__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'slots'
#include "aprs_interfaces/msg/detail/slot_info__traits.hpp"

namespace aprs_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const Tray & msg,
  std::ostream & out)
{
  out << "{";
  // member: identifier
  {
    out << "identifier: ";
    rosidl_generator_traits::value_to_yaml(msg.identifier, out);
    out << ", ";
  }

  // member: name
  {
    out << "name: ";
    rosidl_generator_traits::value_to_yaml(msg.name, out);
    out << ", ";
  }

  // member: slots
  {
    if (msg.slots.size() == 0) {
      out << "slots: []";
    } else {
      out << "slots: [";
      size_t pending_items = msg.slots.size();
      for (auto item : msg.slots) {
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
  const Tray & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: identifier
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "identifier: ";
    rosidl_generator_traits::value_to_yaml(msg.identifier, out);
    out << "\n";
  }

  // member: name
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "name: ";
    rosidl_generator_traits::value_to_yaml(msg.name, out);
    out << "\n";
  }

  // member: slots
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.slots.size() == 0) {
      out << "slots: []\n";
    } else {
      out << "slots:\n";
      for (auto item : msg.slots) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Tray & msg, bool use_flow_style = false)
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
  const aprs_interfaces::msg::Tray & msg,
  std::ostream & out, size_t indentation = 0)
{
  aprs_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use aprs_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const aprs_interfaces::msg::Tray & msg)
{
  return aprs_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<aprs_interfaces::msg::Tray>()
{
  return "aprs_interfaces::msg::Tray";
}

template<>
inline const char * name<aprs_interfaces::msg::Tray>()
{
  return "aprs_interfaces/msg/Tray";
}

template<>
struct has_fixed_size<aprs_interfaces::msg::Tray>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<aprs_interfaces::msg::Tray>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<aprs_interfaces::msg::Tray>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // APRS_INTERFACES__MSG__DETAIL__TRAY__TRAITS_HPP_
