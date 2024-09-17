// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from aprs_interfaces:msg/Trays.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__TRAYS__BUILDER_HPP_
#define APRS_INTERFACES__MSG__DETAIL__TRAYS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "aprs_interfaces/msg/detail/trays__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace aprs_interfaces
{

namespace msg
{

namespace builder
{

class Init_Trays_part_trays
{
public:
  explicit Init_Trays_part_trays(::aprs_interfaces::msg::Trays & msg)
  : msg_(msg)
  {}
  ::aprs_interfaces::msg::Trays part_trays(::aprs_interfaces::msg::Trays::_part_trays_type arg)
  {
    msg_.part_trays = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aprs_interfaces::msg::Trays msg_;
};

class Init_Trays_kit_trays
{
public:
  Init_Trays_kit_trays()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Trays_part_trays kit_trays(::aprs_interfaces::msg::Trays::_kit_trays_type arg)
  {
    msg_.kit_trays = std::move(arg);
    return Init_Trays_part_trays(msg_);
  }

private:
  ::aprs_interfaces::msg::Trays msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::msg::Trays>()
{
  return aprs_interfaces::msg::builder::Init_Trays_kit_trays();
}

}  // namespace aprs_interfaces

#endif  // APRS_INTERFACES__MSG__DETAIL__TRAYS__BUILDER_HPP_
