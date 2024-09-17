// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from aprs_interfaces:srv/Place.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__SRV__DETAIL__PLACE__BUILDER_HPP_
#define APRS_INTERFACES__SRV__DETAIL__PLACE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "aprs_interfaces/srv/detail/place__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace aprs_interfaces
{

namespace srv
{

namespace builder
{

class Init_Place_Request_frame_name
{
public:
  Init_Place_Request_frame_name()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::aprs_interfaces::srv::Place_Request frame_name(::aprs_interfaces::srv::Place_Request::_frame_name_type arg)
  {
    msg_.frame_name = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aprs_interfaces::srv::Place_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::srv::Place_Request>()
{
  return aprs_interfaces::srv::builder::Init_Place_Request_frame_name();
}

}  // namespace aprs_interfaces


namespace aprs_interfaces
{

namespace srv
{

namespace builder
{

class Init_Place_Response_status
{
public:
  explicit Init_Place_Response_status(::aprs_interfaces::srv::Place_Response & msg)
  : msg_(msg)
  {}
  ::aprs_interfaces::srv::Place_Response status(::aprs_interfaces::srv::Place_Response::_status_type arg)
  {
    msg_.status = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aprs_interfaces::srv::Place_Response msg_;
};

class Init_Place_Response_success
{
public:
  Init_Place_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Place_Response_status success(::aprs_interfaces::srv::Place_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_Place_Response_status(msg_);
  }

private:
  ::aprs_interfaces::srv::Place_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::srv::Place_Response>()
{
  return aprs_interfaces::srv::builder::Init_Place_Response_success();
}

}  // namespace aprs_interfaces


namespace aprs_interfaces
{

namespace srv
{

namespace builder
{

class Init_Place_Event_response
{
public:
  explicit Init_Place_Event_response(::aprs_interfaces::srv::Place_Event & msg)
  : msg_(msg)
  {}
  ::aprs_interfaces::srv::Place_Event response(::aprs_interfaces::srv::Place_Event::_response_type arg)
  {
    msg_.response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aprs_interfaces::srv::Place_Event msg_;
};

class Init_Place_Event_request
{
public:
  explicit Init_Place_Event_request(::aprs_interfaces::srv::Place_Event & msg)
  : msg_(msg)
  {}
  Init_Place_Event_response request(::aprs_interfaces::srv::Place_Event::_request_type arg)
  {
    msg_.request = std::move(arg);
    return Init_Place_Event_response(msg_);
  }

private:
  ::aprs_interfaces::srv::Place_Event msg_;
};

class Init_Place_Event_info
{
public:
  Init_Place_Event_info()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Place_Event_request info(::aprs_interfaces::srv::Place_Event::_info_type arg)
  {
    msg_.info = std::move(arg);
    return Init_Place_Event_request(msg_);
  }

private:
  ::aprs_interfaces::srv::Place_Event msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::srv::Place_Event>()
{
  return aprs_interfaces::srv::builder::Init_Place_Event_info();
}

}  // namespace aprs_interfaces

#endif  // APRS_INTERFACES__SRV__DETAIL__PLACE__BUILDER_HPP_
