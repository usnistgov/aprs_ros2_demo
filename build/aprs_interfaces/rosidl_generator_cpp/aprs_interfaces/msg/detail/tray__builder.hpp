// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from aprs_interfaces:msg/Tray.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__TRAY__BUILDER_HPP_
#define APRS_INTERFACES__MSG__DETAIL__TRAY__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "aprs_interfaces/msg/detail/tray__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace aprs_interfaces
{

namespace msg
{

namespace builder
{

class Init_Tray_slots
{
public:
  explicit Init_Tray_slots(::aprs_interfaces::msg::Tray & msg)
  : msg_(msg)
  {}
  ::aprs_interfaces::msg::Tray slots(::aprs_interfaces::msg::Tray::_slots_type arg)
  {
    msg_.slots = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aprs_interfaces::msg::Tray msg_;
};

class Init_Tray_name
{
public:
  explicit Init_Tray_name(::aprs_interfaces::msg::Tray & msg)
  : msg_(msg)
  {}
  Init_Tray_slots name(::aprs_interfaces::msg::Tray::_name_type arg)
  {
    msg_.name = std::move(arg);
    return Init_Tray_slots(msg_);
  }

private:
  ::aprs_interfaces::msg::Tray msg_;
};

class Init_Tray_identifier
{
public:
  Init_Tray_identifier()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Tray_name identifier(::aprs_interfaces::msg::Tray::_identifier_type arg)
  {
    msg_.identifier = std::move(arg);
    return Init_Tray_name(msg_);
  }

private:
  ::aprs_interfaces::msg::Tray msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::msg::Tray>()
{
  return aprs_interfaces::msg::builder::Init_Tray_identifier();
}

}  // namespace aprs_interfaces

#endif  // APRS_INTERFACES__MSG__DETAIL__TRAY__BUILDER_HPP_
