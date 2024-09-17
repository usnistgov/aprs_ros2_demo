// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from aprs_interfaces:action/ExecutePlan.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__ACTION__DETAIL__EXECUTE_PLAN__BUILDER_HPP_
#define APRS_INTERFACES__ACTION__DETAIL__EXECUTE_PLAN__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "aprs_interfaces/action/detail/execute_plan__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace aprs_interfaces
{

namespace action
{


}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::action::ExecutePlan_Goal>()
{
  return ::aprs_interfaces::action::ExecutePlan_Goal(rosidl_runtime_cpp::MessageInitialization::ZERO);
}

}  // namespace aprs_interfaces


namespace aprs_interfaces
{

namespace action
{


}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::action::ExecutePlan_Result>()
{
  return ::aprs_interfaces::action::ExecutePlan_Result(rosidl_runtime_cpp::MessageInitialization::ZERO);
}

}  // namespace aprs_interfaces


namespace aprs_interfaces
{

namespace action
{


}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::action::ExecutePlan_Feedback>()
{
  return ::aprs_interfaces::action::ExecutePlan_Feedback(rosidl_runtime_cpp::MessageInitialization::ZERO);
}

}  // namespace aprs_interfaces


namespace aprs_interfaces
{

namespace action
{

namespace builder
{

class Init_ExecutePlan_SendGoal_Request_goal
{
public:
  explicit Init_ExecutePlan_SendGoal_Request_goal(::aprs_interfaces::action::ExecutePlan_SendGoal_Request & msg)
  : msg_(msg)
  {}
  ::aprs_interfaces::action::ExecutePlan_SendGoal_Request goal(::aprs_interfaces::action::ExecutePlan_SendGoal_Request::_goal_type arg)
  {
    msg_.goal = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aprs_interfaces::action::ExecutePlan_SendGoal_Request msg_;
};

class Init_ExecutePlan_SendGoal_Request_goal_id
{
public:
  Init_ExecutePlan_SendGoal_Request_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ExecutePlan_SendGoal_Request_goal goal_id(::aprs_interfaces::action::ExecutePlan_SendGoal_Request::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return Init_ExecutePlan_SendGoal_Request_goal(msg_);
  }

private:
  ::aprs_interfaces::action::ExecutePlan_SendGoal_Request msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::action::ExecutePlan_SendGoal_Request>()
{
  return aprs_interfaces::action::builder::Init_ExecutePlan_SendGoal_Request_goal_id();
}

}  // namespace aprs_interfaces


namespace aprs_interfaces
{

namespace action
{

namespace builder
{

class Init_ExecutePlan_SendGoal_Response_stamp
{
public:
  explicit Init_ExecutePlan_SendGoal_Response_stamp(::aprs_interfaces::action::ExecutePlan_SendGoal_Response & msg)
  : msg_(msg)
  {}
  ::aprs_interfaces::action::ExecutePlan_SendGoal_Response stamp(::aprs_interfaces::action::ExecutePlan_SendGoal_Response::_stamp_type arg)
  {
    msg_.stamp = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aprs_interfaces::action::ExecutePlan_SendGoal_Response msg_;
};

class Init_ExecutePlan_SendGoal_Response_accepted
{
public:
  Init_ExecutePlan_SendGoal_Response_accepted()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ExecutePlan_SendGoal_Response_stamp accepted(::aprs_interfaces::action::ExecutePlan_SendGoal_Response::_accepted_type arg)
  {
    msg_.accepted = std::move(arg);
    return Init_ExecutePlan_SendGoal_Response_stamp(msg_);
  }

private:
  ::aprs_interfaces::action::ExecutePlan_SendGoal_Response msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::action::ExecutePlan_SendGoal_Response>()
{
  return aprs_interfaces::action::builder::Init_ExecutePlan_SendGoal_Response_accepted();
}

}  // namespace aprs_interfaces


namespace aprs_interfaces
{

namespace action
{

namespace builder
{

class Init_ExecutePlan_SendGoal_Event_response
{
public:
  explicit Init_ExecutePlan_SendGoal_Event_response(::aprs_interfaces::action::ExecutePlan_SendGoal_Event & msg)
  : msg_(msg)
  {}
  ::aprs_interfaces::action::ExecutePlan_SendGoal_Event response(::aprs_interfaces::action::ExecutePlan_SendGoal_Event::_response_type arg)
  {
    msg_.response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aprs_interfaces::action::ExecutePlan_SendGoal_Event msg_;
};

class Init_ExecutePlan_SendGoal_Event_request
{
public:
  explicit Init_ExecutePlan_SendGoal_Event_request(::aprs_interfaces::action::ExecutePlan_SendGoal_Event & msg)
  : msg_(msg)
  {}
  Init_ExecutePlan_SendGoal_Event_response request(::aprs_interfaces::action::ExecutePlan_SendGoal_Event::_request_type arg)
  {
    msg_.request = std::move(arg);
    return Init_ExecutePlan_SendGoal_Event_response(msg_);
  }

private:
  ::aprs_interfaces::action::ExecutePlan_SendGoal_Event msg_;
};

class Init_ExecutePlan_SendGoal_Event_info
{
public:
  Init_ExecutePlan_SendGoal_Event_info()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ExecutePlan_SendGoal_Event_request info(::aprs_interfaces::action::ExecutePlan_SendGoal_Event::_info_type arg)
  {
    msg_.info = std::move(arg);
    return Init_ExecutePlan_SendGoal_Event_request(msg_);
  }

private:
  ::aprs_interfaces::action::ExecutePlan_SendGoal_Event msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::action::ExecutePlan_SendGoal_Event>()
{
  return aprs_interfaces::action::builder::Init_ExecutePlan_SendGoal_Event_info();
}

}  // namespace aprs_interfaces


namespace aprs_interfaces
{

namespace action
{

namespace builder
{

class Init_ExecutePlan_GetResult_Request_goal_id
{
public:
  Init_ExecutePlan_GetResult_Request_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::aprs_interfaces::action::ExecutePlan_GetResult_Request goal_id(::aprs_interfaces::action::ExecutePlan_GetResult_Request::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aprs_interfaces::action::ExecutePlan_GetResult_Request msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::action::ExecutePlan_GetResult_Request>()
{
  return aprs_interfaces::action::builder::Init_ExecutePlan_GetResult_Request_goal_id();
}

}  // namespace aprs_interfaces


namespace aprs_interfaces
{

namespace action
{

namespace builder
{

class Init_ExecutePlan_GetResult_Response_result
{
public:
  explicit Init_ExecutePlan_GetResult_Response_result(::aprs_interfaces::action::ExecutePlan_GetResult_Response & msg)
  : msg_(msg)
  {}
  ::aprs_interfaces::action::ExecutePlan_GetResult_Response result(::aprs_interfaces::action::ExecutePlan_GetResult_Response::_result_type arg)
  {
    msg_.result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aprs_interfaces::action::ExecutePlan_GetResult_Response msg_;
};

class Init_ExecutePlan_GetResult_Response_status
{
public:
  Init_ExecutePlan_GetResult_Response_status()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ExecutePlan_GetResult_Response_result status(::aprs_interfaces::action::ExecutePlan_GetResult_Response::_status_type arg)
  {
    msg_.status = std::move(arg);
    return Init_ExecutePlan_GetResult_Response_result(msg_);
  }

private:
  ::aprs_interfaces::action::ExecutePlan_GetResult_Response msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::action::ExecutePlan_GetResult_Response>()
{
  return aprs_interfaces::action::builder::Init_ExecutePlan_GetResult_Response_status();
}

}  // namespace aprs_interfaces


namespace aprs_interfaces
{

namespace action
{

namespace builder
{

class Init_ExecutePlan_GetResult_Event_response
{
public:
  explicit Init_ExecutePlan_GetResult_Event_response(::aprs_interfaces::action::ExecutePlan_GetResult_Event & msg)
  : msg_(msg)
  {}
  ::aprs_interfaces::action::ExecutePlan_GetResult_Event response(::aprs_interfaces::action::ExecutePlan_GetResult_Event::_response_type arg)
  {
    msg_.response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aprs_interfaces::action::ExecutePlan_GetResult_Event msg_;
};

class Init_ExecutePlan_GetResult_Event_request
{
public:
  explicit Init_ExecutePlan_GetResult_Event_request(::aprs_interfaces::action::ExecutePlan_GetResult_Event & msg)
  : msg_(msg)
  {}
  Init_ExecutePlan_GetResult_Event_response request(::aprs_interfaces::action::ExecutePlan_GetResult_Event::_request_type arg)
  {
    msg_.request = std::move(arg);
    return Init_ExecutePlan_GetResult_Event_response(msg_);
  }

private:
  ::aprs_interfaces::action::ExecutePlan_GetResult_Event msg_;
};

class Init_ExecutePlan_GetResult_Event_info
{
public:
  Init_ExecutePlan_GetResult_Event_info()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ExecutePlan_GetResult_Event_request info(::aprs_interfaces::action::ExecutePlan_GetResult_Event::_info_type arg)
  {
    msg_.info = std::move(arg);
    return Init_ExecutePlan_GetResult_Event_request(msg_);
  }

private:
  ::aprs_interfaces::action::ExecutePlan_GetResult_Event msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::action::ExecutePlan_GetResult_Event>()
{
  return aprs_interfaces::action::builder::Init_ExecutePlan_GetResult_Event_info();
}

}  // namespace aprs_interfaces


namespace aprs_interfaces
{

namespace action
{

namespace builder
{

class Init_ExecutePlan_FeedbackMessage_feedback
{
public:
  explicit Init_ExecutePlan_FeedbackMessage_feedback(::aprs_interfaces::action::ExecutePlan_FeedbackMessage & msg)
  : msg_(msg)
  {}
  ::aprs_interfaces::action::ExecutePlan_FeedbackMessage feedback(::aprs_interfaces::action::ExecutePlan_FeedbackMessage::_feedback_type arg)
  {
    msg_.feedback = std::move(arg);
    return std::move(msg_);
  }

private:
  ::aprs_interfaces::action::ExecutePlan_FeedbackMessage msg_;
};

class Init_ExecutePlan_FeedbackMessage_goal_id
{
public:
  Init_ExecutePlan_FeedbackMessage_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ExecutePlan_FeedbackMessage_feedback goal_id(::aprs_interfaces::action::ExecutePlan_FeedbackMessage::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return Init_ExecutePlan_FeedbackMessage_feedback(msg_);
  }

private:
  ::aprs_interfaces::action::ExecutePlan_FeedbackMessage msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::aprs_interfaces::action::ExecutePlan_FeedbackMessage>()
{
  return aprs_interfaces::action::builder::Init_ExecutePlan_FeedbackMessage_goal_id();
}

}  // namespace aprs_interfaces

#endif  // APRS_INTERFACES__ACTION__DETAIL__EXECUTE_PLAN__BUILDER_HPP_
