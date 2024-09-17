// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from aprs_interfaces:msg/SlotInfo.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__SLOT_INFO__TRAITS_HPP_
#define APRS_INTERFACES__MSG__DETAIL__SLOT_INFO__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "aprs_interfaces/msg/detail/slot_info__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace aprs_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const SlotInfo & msg,
  std::ostream & out)
{
  out << "{";
  // member: occupied
  {
    out << "occupied: ";
    rosidl_generator_traits::value_to_yaml(msg.occupied, out);
    out << ", ";
  }

  // member: size
  {
    out << "size: ";
    rosidl_generator_traits::value_to_yaml(msg.size, out);
    out << ", ";
  }

  // member: name
  {
    out << "name: ";
    rosidl_generator_traits::value_to_yaml(msg.name, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SlotInfo & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: occupied
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "occupied: ";
    rosidl_generator_traits::value_to_yaml(msg.occupied, out);
    out << "\n";
  }

  // member: size
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "size: ";
    rosidl_generator_traits::value_to_yaml(msg.size, out);
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
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SlotInfo & msg, bool use_flow_style = false)
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
  const aprs_interfaces::msg::SlotInfo & msg,
  std::ostream & out, size_t indentation = 0)
{
  aprs_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use aprs_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const aprs_interfaces::msg::SlotInfo & msg)
{
  return aprs_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<aprs_interfaces::msg::SlotInfo>()
{
  return "aprs_interfaces::msg::SlotInfo";
}

template<>
inline const char * name<aprs_interfaces::msg::SlotInfo>()
{
  return "aprs_interfaces/msg/SlotInfo";
}

template<>
struct has_fixed_size<aprs_interfaces::msg::SlotInfo>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<aprs_interfaces::msg::SlotInfo>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<aprs_interfaces::msg::SlotInfo>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // APRS_INTERFACES__MSG__DETAIL__SLOT_INFO__TRAITS_HPP_
