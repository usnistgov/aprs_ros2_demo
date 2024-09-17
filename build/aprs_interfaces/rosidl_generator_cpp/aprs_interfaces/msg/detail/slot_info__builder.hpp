// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from aprs_interfaces:msg/SlotInfo.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__SLOT_INFO__BUILDER_HPP_
#define APRS_INTERFACES__MSG__DETAIL__SLOT_INFO__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "aprs_interfaces/msg/detail/slot_info__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace aprs_interfaces
{

namespace msg
{

namespace builder
{

class Init_SlotInfo_name
{
public:
  explicit Init_SlotInfo_name(::aprs_interfaces::msg::SlotInfo & msg)
  : msg_(msg)
  {}
  ::aprs_interfaces::msg::SlotInfo name(::aprs_interfaces::msg::SlotInfo::_name_type arg)
  {
    msg_.name = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aprs_interfaces::msg::SlotInfo msg_;
};

class Init_SlotInfo_size
{
public:
  explicit Init_SlotInfo_size(::aprs_interfaces::msg::SlotInfo & msg)
  : msg_(msg)
  {}
  Init_SlotInfo_name size(::aprs_interfaces::msg::SlotInfo::_size_type arg)
  {
    msg_.size = std::move(arg);
    return Init_SlotInfo_name(msg_);
  }

private:
  ::aprs_interfaces::msg::SlotInfo msg_;
};

class Init_SlotInfo_occupied
{
public:
  Init_SlotInfo_occupied()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SlotInfo_size occupied(::aprs_interfaces::msg::SlotInfo::_occupied_type arg)
  {
    msg_.occupied = std::move(arg);
    return Init_SlotInfo_size(msg_);
  }

private:
  ::aprs_interfaces::msg::SlotInfo msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::msg::SlotInfo>()
{
  return aprs_interfaces::msg::builder::Init_SlotInfo_occupied();
}

}  // namespace aprs_interfaces

#endif  // APRS_INTERFACES__MSG__DETAIL__SLOT_INFO__BUILDER_HPP_
