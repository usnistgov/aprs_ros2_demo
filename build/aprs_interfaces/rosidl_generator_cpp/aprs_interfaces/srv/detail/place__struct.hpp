// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from aprs_interfaces:srv/Place.idl
// generated code does not contain a copyright notice

#ifndef APRS_INTERFACES__SRV__DETAIL__PLACE__STRUCT_HPP_
#define APRS_INTERFACES__SRV__DETAIL__PLACE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__aprs_interfaces__srv__Place_Request __attribute__((deprecated))
#else
# define DEPRECATED__aprs_interfaces__srv__Place_Request __declspec(deprecated)
#endif

namespace aprs_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Place_Request_
{
  using Type = Place_Request_<ContainerAllocator>;

  explicit Place_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->frame_name = "";
    }
  }

  explicit Place_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : frame_name(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->frame_name = "";
    }
  }

  // field types and members
  using _frame_name_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _frame_name_type frame_name;

  // setters for named parameter idiom
  Type & set__frame_name(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->frame_name = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    aprs_interfaces::srv::Place_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const aprs_interfaces::srv::Place_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<aprs_interfaces::srv::Place_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<aprs_interfaces::srv::Place_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      aprs_interfaces::srv::Place_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<aprs_interfaces::srv::Place_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      aprs_interfaces::srv::Place_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<aprs_interfaces::srv::Place_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<aprs_interfaces::srv::Place_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<aprs_interfaces::srv::Place_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__aprs_interfaces__srv__Place_Request
    std::shared_ptr<aprs_interfaces::srv::Place_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__aprs_interfaces__srv__Place_Request
    std::shared_ptr<aprs_interfaces::srv::Place_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Place_Request_ & other) const
  {
    if (this->frame_name != other.frame_name) {
      return false;
    }
    return true;
  }
  bool operator!=(const Place_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Place_Request_

// alias to use template instance with default allocator
using Place_Request =
  aprs_interfaces::srv::Place_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace aprs_interfaces


#ifndef _WIN32
# define DEPRECATED__aprs_interfaces__srv__Place_Response __attribute__((deprecated))
#else
# define DEPRECATED__aprs_interfaces__srv__Place_Response __declspec(deprecated)
#endif

namespace aprs_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Place_Response_
{
  using Type = Place_Response_<ContainerAllocator>;

  explicit Place_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->status = "";
    }
  }

  explicit Place_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : status(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->status = "";
    }
  }

  // field types and members
  using _success_type =
    bool;
  _success_type success;
  using _status_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _status_type status;

  // setters for named parameter idiom
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }
  Type & set__status(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->status = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    aprs_interfaces::srv::Place_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const aprs_interfaces::srv::Place_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<aprs_interfaces::srv::Place_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<aprs_interfaces::srv::Place_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      aprs_interfaces::srv::Place_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<aprs_interfaces::srv::Place_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      aprs_interfaces::srv::Place_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<aprs_interfaces::srv::Place_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<aprs_interfaces::srv::Place_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<aprs_interfaces::srv::Place_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__aprs_interfaces__srv__Place_Response
    std::shared_ptr<aprs_interfaces::srv::Place_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__aprs_interfaces__srv__Place_Response
    std::shared_ptr<aprs_interfaces::srv::Place_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Place_Response_ & other) const
  {
    if (this->success != other.success) {
      return false;
    }
    if (this->status != other.status) {
      return false;
    }
    return true;
  }
  bool operator!=(const Place_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Place_Response_

// alias to use template instance with default allocator
using Place_Response =
  aprs_interfaces::srv::Place_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace aprs_interfaces


// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__aprs_interfaces__srv__Place_Event __attribute__((deprecated))
#else
# define DEPRECATED__aprs_interfaces__srv__Place_Event __declspec(deprecated)
#endif

namespace aprs_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Place_Event_
{
  using Type = Place_Event_<ContainerAllocator>;

  explicit Place_Event_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : info(_init)
  {
    (void)_init;
  }

  explicit Place_Event_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : info(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _info_type =
    service_msgs::msg::ServiceEventInfo_<ContainerAllocator>;
  _info_type info;
  using _request_type =
    rosidl_runtime_cpp::BoundedVector<aprs_interfaces::srv::Place_Request_<ContainerAllocator>, 1, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<aprs_interfaces::srv::Place_Request_<ContainerAllocator>>>;
  _request_type request;
  using _response_type =
    rosidl_runtime_cpp::BoundedVector<aprs_interfaces::srv::Place_Response_<ContainerAllocator>, 1, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<aprs_interfaces::srv::Place_Response_<ContainerAllocator>>>;
  _response_type response;

  // setters for named parameter idiom
  Type & set__info(
    const service_msgs::msg::ServiceEventInfo_<ContainerAllocator> & _arg)
  {
    this->info = _arg;
    return *this;
  }
  Type & set__request(
    const rosidl_runtime_cpp::BoundedVector<aprs_interfaces::srv::Place_Request_<ContainerAllocator>, 1, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<aprs_interfaces::srv::Place_Request_<ContainerAllocator>>> & _arg)
  {
    this->request = _arg;
    return *this;
  }
  Type & set__response(
    const rosidl_runtime_cpp::BoundedVector<aprs_interfaces::srv::Place_Response_<ContainerAllocator>, 1, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<aprs_interfaces::srv::Place_Response_<ContainerAllocator>>> & _arg)
  {
    this->response = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    aprs_interfaces::srv::Place_Event_<ContainerAllocator> *;
  using ConstRawPtr =
    const aprs_interfaces::srv::Place_Event_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<aprs_interfaces::srv::Place_Event_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<aprs_interfaces::srv::Place_Event_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      aprs_interfaces::srv::Place_Event_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<aprs_interfaces::srv::Place_Event_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      aprs_interfaces::srv::Place_Event_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<aprs_interfaces::srv::Place_Event_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<aprs_interfaces::srv::Place_Event_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<aprs_interfaces::srv::Place_Event_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__aprs_interfaces__srv__Place_Event
    std::shared_ptr<aprs_interfaces::srv::Place_Event_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__aprs_interfaces__srv__Place_Event
    std::shared_ptr<aprs_interfaces::srv::Place_Event_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Place_Event_ & other) const
  {
    if (this->info != other.info) {
      return false;
    }
    if (this->request != other.request) {
      return false;
    }
    if (this->response != other.response) {
      return false;
    }
    return true;
  }
  bool operator!=(const Place_Event_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Place_Event_

// alias to use template instance with default allocator
using Place_Event =
  aprs_interfaces::srv::Place_Event_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace aprs_interfaces

namespace aprs_interfaces
{

namespace srv
{

struct Place
{
  using Request = aprs_interfaces::srv::Place_Request;
  using Response = aprs_interfaces::srv::Place_Response;
  using Event = aprs_interfaces::srv::Place_Event;
};

}  // namespace srv

}  // namespace aprs_interfaces

#endif  // APRS_INTERFACES__SRV__DETAIL__PLACE__STRUCT_HPP_
