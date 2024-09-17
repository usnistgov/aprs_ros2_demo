// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from aprs_interfaces:srv/GeneratePlan.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__SRV__DETAIL__GENERATE_PLAN__TRAITS_HPP_
#define APRS_INTERFACES__SRV__DETAIL__GENERATE_PLAN__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "aprs_interfaces/srv/detail/generate_plan__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace aprs_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const GeneratePlan_Request & msg,
  std::ostream & out)
{
  (void)msg;
  out << "null";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GeneratePlan_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  (void)msg;
  (void)indentation;
  out << "null\n";
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GeneratePlan_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace aprs_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use aprs_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const aprs_interfaces::srv::GeneratePlan_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  aprs_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use aprs_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const aprs_interfaces::srv::GeneratePlan_Request & msg)
{
  return aprs_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<aprs_interfaces::srv::GeneratePlan_Request>()
{
  return "aprs_interfaces::srv::GeneratePlan_Request";
}

template<>
inline const char * name<aprs_interfaces::srv::GeneratePlan_Request>()
{
  return "aprs_interfaces/srv/GeneratePlan_Request";
}

template<>
struct has_fixed_size<aprs_interfaces::srv::GeneratePlan_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<aprs_interfaces::srv::GeneratePlan_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<aprs_interfaces::srv::GeneratePlan_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace aprs_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const GeneratePlan_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: success
  {
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << ", ";
  }

  // member: plan
  {
    out << "plan: ";
    rosidl_generator_traits::value_to_yaml(msg.plan, out);
    out << ", ";
  }

  // member: status
  {
    out << "status: ";
    rosidl_generator_traits::value_to_yaml(msg.status, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GeneratePlan_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: success
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << "\n";
  }

  // member: plan
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "plan: ";
    rosidl_generator_traits::value_to_yaml(msg.plan, out);
    out << "\n";
  }

  // member: status
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "status: ";
    rosidl_generator_traits::value_to_yaml(msg.status, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GeneratePlan_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace aprs_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use aprs_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const aprs_interfaces::srv::GeneratePlan_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  aprs_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use aprs_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const aprs_interfaces::srv::GeneratePlan_Response & msg)
{
  return aprs_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<aprs_interfaces::srv::GeneratePlan_Response>()
{
  return "aprs_interfaces::srv::GeneratePlan_Response";
}

template<>
inline const char * name<aprs_interfaces::srv::GeneratePlan_Response>()
{
  return "aprs_interfaces/srv/GeneratePlan_Response";
}

template<>
struct has_fixed_size<aprs_interfaces::srv::GeneratePlan_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<aprs_interfaces::srv::GeneratePlan_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<aprs_interfaces::srv::GeneratePlan_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__traits.hpp"

namespace aprs_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const GeneratePlan_Event & msg,
  std::ostream & out)
{
  out << "{";
  // member: info
  {
    out << "info: ";
    to_flow_style_yaml(msg.info, out);
    out << ", ";
  }

  // member: request
  {
    if (msg.request.size() == 0) {
      out << "request: []";
    } else {
      out << "request: [";
      size_t pending_items = msg.request.size();
      for (auto item : msg.request) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: response
  {
    if (msg.response.size() == 0) {
      out << "response: []";
    } else {
      out << "response: [";
      size_t pending_items = msg.response.size();
      for (auto item : msg.response) {
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
  const GeneratePlan_Event & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: info
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "info:\n";
    to_block_style_yaml(msg.info, out, indentation + 2);
  }

  // member: request
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.request.size() == 0) {
      out << "request: []\n";
    } else {
      out << "request:\n";
      for (auto item : msg.request) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: response
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.response.size() == 0) {
      out << "response: []\n";
    } else {
      out << "response:\n";
      for (auto item : msg.response) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GeneratePlan_Event & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace aprs_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use aprs_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const aprs_interfaces::srv::GeneratePlan_Event & msg,
  std::ostream & out, size_t indentation = 0)
{
  aprs_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use aprs_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const aprs_interfaces::srv::GeneratePlan_Event & msg)
{
  return aprs_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<aprs_interfaces::srv::GeneratePlan_Event>()
{
  return "aprs_interfaces::srv::GeneratePlan_Event";
}

template<>
inline const char * name<aprs_interfaces::srv::GeneratePlan_Event>()
{
  return "aprs_interfaces/srv/GeneratePlan_Event";
}

template<>
struct has_fixed_size<aprs_interfaces::srv::GeneratePlan_Event>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<aprs_interfaces::srv::GeneratePlan_Event>
  : std::integral_constant<bool, has_bounded_size<aprs_interfaces::srv::GeneratePlan_Request>::value && has_bounded_size<aprs_interfaces::srv::GeneratePlan_Response>::value && has_bounded_size<service_msgs::msg::ServiceEventInfo>::value> {};

template<>
struct is_message<aprs_interfaces::srv::GeneratePlan_Event>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<aprs_interfaces::srv::GeneratePlan>()
{
  return "aprs_interfaces::srv::GeneratePlan";
}

template<>
inline const char * name<aprs_interfaces::srv::GeneratePlan>()
{
  return "aprs_interfaces/srv/GeneratePlan";
}

template<>
struct has_fixed_size<aprs_interfaces::srv::GeneratePlan>
  : std::integral_constant<
    bool,
    has_fixed_size<aprs_interfaces::srv::GeneratePlan_Request>::value &&
    has_fixed_size<aprs_interfaces::srv::GeneratePlan_Response>::value
  >
{
};

template<>
struct has_bounded_size<aprs_interfaces::srv::GeneratePlan>
  : std::integral_constant<
    bool,
    has_bounded_size<aprs_interfaces::srv::GeneratePlan_Request>::value &&
    has_bounded_size<aprs_interfaces::srv::GeneratePlan_Response>::value
  >
{
};

template<>
struct is_service<aprs_interfaces::srv::GeneratePlan>
  : std::true_type
{
};

template<>
struct is_service_request<aprs_interfaces::srv::GeneratePlan_Request>
  : std::true_type
{
};

template<>
struct is_service_response<aprs_interfaces::srv::GeneratePlan_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // APRS_INTERFACES__SRV__DETAIL__GENERATE_PLAN__TRAITS_HPP_
