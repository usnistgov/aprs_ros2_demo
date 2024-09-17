// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from aprs_interfaces:srv/GenerateInitState.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__SRV__DETAIL__GENERATE_INIT_STATE__BUILDER_HPP_
#define APRS_INTERFACES__SRV__DETAIL__GENERATE_INIT_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "aprs_interfaces/srv/detail/generate_init_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace aprs_interfaces
{

namespace srv
{


}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::srv::GenerateInitState_Request>()
{
  return ::aprs_interfaces::srv::GenerateInitState_Request(rosidl_runtime_cpp::MessageInitialization::ZERO);
}

}  // namespace aprs_interfaces


namespace aprs_interfaces
{

namespace srv
{

namespace builder
{

class Init_GenerateInitState_Response_status
{
public:
  explicit Init_GenerateInitState_Response_status(::aprs_interfaces::srv::GenerateInitState_Response & msg)
  : msg_(msg)
  {}
  ::aprs_interfaces::srv::GenerateInitState_Response status(::aprs_interfaces::srv::GenerateInitState_Response::_status_type arg)
  {
    msg_.status = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aprs_interfaces::srv::GenerateInitState_Response msg_;
};

class Init_GenerateInitState_Response_success
{
public:
  Init_GenerateInitState_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_GenerateInitState_Response_status success(::aprs_interfaces::srv::GenerateInitState_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_GenerateInitState_Response_status(msg_);
  }

private:
  ::aprs_interfaces::srv::GenerateInitState_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::srv::GenerateInitState_Response>()
{
  return aprs_interfaces::srv::builder::Init_GenerateInitState_Response_success();
}

}  // namespace aprs_interfaces


namespace aprs_interfaces
{

namespace srv
{

namespace builder
{

class Init_GenerateInitState_Event_response
{
public:
  explicit Init_GenerateInitState_Event_response(::aprs_interfaces::srv::GenerateInitState_Event & msg)
  : msg_(msg)
  {}
  ::aprs_interfaces::srv::GenerateInitState_Event response(::aprs_interfaces::srv::GenerateInitState_Event::_response_type arg)
  {
    msg_.response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aprs_interfaces::srv::GenerateInitState_Event msg_;
};

class Init_GenerateInitState_Event_request
{
public:
  explicit Init_GenerateInitState_Event_request(::aprs_interfaces::srv::GenerateInitState_Event & msg)
  : msg_(msg)
  {}
  Init_GenerateInitState_Event_response request(::aprs_interfaces::srv::GenerateInitState_Event::_request_type arg)
  {
    msg_.request = std::move(arg);
    return Init_GenerateInitState_Event_response(msg_);
  }

private:
  ::aprs_interfaces::srv::GenerateInitState_Event msg_;
};

class Init_GenerateInitState_Event_info
{
public:
  Init_GenerateInitState_Event_info()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_GenerateInitState_Event_request info(::aprs_interfaces::srv::GenerateInitState_Event::_info_type arg)
  {
    msg_.info = std::move(arg);
    return Init_GenerateInitState_Event_request(msg_);
  }

private:
  ::aprs_interfaces::srv::GenerateInitState_Event msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::srv::GenerateInitState_Event>()
{
  return aprs_interfaces::srv::builder::Init_GenerateInitState_Event_info();
}

}  // namespace aprs_interfaces

#endif  // APRS_INTERFACES__SRV__DETAIL__GENERATE_INIT_STATE__BUILDER_HPP_
