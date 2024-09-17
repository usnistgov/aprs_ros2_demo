// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from aprs_interfaces:msg/Object.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__MSG__DETAIL__OBJECT__BUILDER_HPP_
#define APRS_INTERFACES__MSG__DETAIL__OBJECT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "aprs_interfaces/msg/detail/object__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace aprs_interfaces
{

namespace msg
{

namespace builder
{

class Init_Object_pose_stamped
{
public:
  explicit Init_Object_pose_stamped(::aprs_interfaces::msg::Object & msg)
  : msg_(msg)
  {}
  ::aprs_interfaces::msg::Object pose_stamped(::aprs_interfaces::msg::Object::_pose_stamped_type arg)
  {
    msg_.pose_stamped = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aprs_interfaces::msg::Object msg_;
};

class Init_Object_object_identifier
{
public:
  explicit Init_Object_object_identifier(::aprs_interfaces::msg::Object & msg)
  : msg_(msg)
  {}
  Init_Object_pose_stamped object_identifier(::aprs_interfaces::msg::Object::_object_identifier_type arg)
  {
    msg_.object_identifier = std::move(arg);
    return Init_Object_pose_stamped(msg_);
  }

private:
  ::aprs_interfaces::msg::Object msg_;
};

class Init_Object_object_type
{
public:
  explicit Init_Object_object_type(::aprs_interfaces::msg::Object & msg)
  : msg_(msg)
  {}
  Init_Object_object_identifier object_type(::aprs_interfaces::msg::Object::_object_type_type arg)
  {
    msg_.object_type = std::move(arg);
    return Init_Object_object_identifier(msg_);
  }

private:
  ::aprs_interfaces::msg::Object msg_;
};

class Init_Object_name
{
public:
  Init_Object_name()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Object_object_type name(::aprs_interfaces::msg::Object::_name_type arg)
  {
    msg_.name = std::move(arg);
    return Init_Object_object_type(msg_);
  }

private:
  ::aprs_interfaces::msg::Object msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::msg::Object>()
{
  return aprs_interfaces::msg::builder::Init_Object_name();
}

}  // namespace aprs_interfaces

#endif  // APRS_INTERFACES__MSG__DETAIL__OBJECT__BUILDER_HPP_
