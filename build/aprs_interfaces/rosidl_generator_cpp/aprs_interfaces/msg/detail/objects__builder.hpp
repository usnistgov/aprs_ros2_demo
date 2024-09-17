// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from aprs_interfaces:msg/Objects.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__OBJECTS__BUILDER_HPP_
#define APRS_INTERFACES__MSG__DETAIL__OBJECTS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "aprs_interfaces/msg/detail/objects__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace aprs_interfaces
{

namespace msg
{

namespace builder
{

class Init_Objects_objects
{
public:
  Init_Objects_objects()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::aprs_interfaces::msg::Objects objects(::aprs_interfaces::msg::Objects::_objects_type arg)
  {
    msg_.objects = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aprs_interfaces::msg::Objects msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::msg::Objects>()
{
  return aprs_interfaces::msg::builder::Init_Objects_objects();
}

}  // namespace aprs_interfaces

#endif  // APRS_INTERFACES__MSG__DETAIL__OBJECTS__BUILDER_HPP_
