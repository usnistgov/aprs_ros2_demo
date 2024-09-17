# generated from rosidl_generator_py/resource/_idl.py.em
# with input from aprs_interfaces:msg/Object.idl
# generated code does not contain a copyright notice

# This is being done at the module level and not on the instance level to avoid looking
# for the same variable multiple times on each instance. This variable is not supposed to
# change during runtime so it makes sense to only look for it once.
from os import getenv

ros_python_check_fields = getenv('ROS_PYTHON_CHECK_FIELDS', default='')


# Import statements for member types

import builtins  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_Object(type):
    """Metaclass of message 'Object'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
        'KIT_TRAY': 1,
        'PART_TRAY': 2,
        'PART': 3,
        'SMALL_GEAR': 10,
        'MEDIUM_GEAR': 11,
        'LARGE_GEAR': 12,
        'SMALL_GEAR_TRAY': 13,
        'MEDIUM_GEAR_TRAY': 14,
        'LARGE_GEAR_TRAY': 15,
        'M2L1_KIT_TRAY': 16,
        'S2L2_KIT_TRAY': 17,
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('aprs_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'aprs_interfaces.msg.Object')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__object
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__object
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__object
            cls._TYPE_SUPPORT = module.type_support_msg__msg__object
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__object

            from geometry_msgs.msg import PoseStamped
            if PoseStamped.__class__._TYPE_SUPPORT is None:
                PoseStamped.__class__.__import_type_support__()

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
            'KIT_TRAY': cls.__constants['KIT_TRAY'],
            'PART_TRAY': cls.__constants['PART_TRAY'],
            'PART': cls.__constants['PART'],
            'SMALL_GEAR': cls.__constants['SMALL_GEAR'],
            'MEDIUM_GEAR': cls.__constants['MEDIUM_GEAR'],
            'LARGE_GEAR': cls.__constants['LARGE_GEAR'],
            'SMALL_GEAR_TRAY': cls.__constants['SMALL_GEAR_TRAY'],
            'MEDIUM_GEAR_TRAY': cls.__constants['MEDIUM_GEAR_TRAY'],
            'LARGE_GEAR_TRAY': cls.__constants['LARGE_GEAR_TRAY'],
            'M2L1_KIT_TRAY': cls.__constants['M2L1_KIT_TRAY'],
            'S2L2_KIT_TRAY': cls.__constants['S2L2_KIT_TRAY'],
        }

    @property
    def KIT_TRAY(self):
        """Message constant 'KIT_TRAY'."""
        return Metaclass_Object.__constants['KIT_TRAY']

    @property
    def PART_TRAY(self):
        """Message constant 'PART_TRAY'."""
        return Metaclass_Object.__constants['PART_TRAY']

    @property
    def PART(self):
        """Message constant 'PART'."""
        return Metaclass_Object.__constants['PART']

    @property
    def SMALL_GEAR(self):
        """Message constant 'SMALL_GEAR'."""
        return Metaclass_Object.__constants['SMALL_GEAR']

    @property
    def MEDIUM_GEAR(self):
        """Message constant 'MEDIUM_GEAR'."""
        return Metaclass_Object.__constants['MEDIUM_GEAR']

    @property
    def LARGE_GEAR(self):
        """Message constant 'LARGE_GEAR'."""
        return Metaclass_Object.__constants['LARGE_GEAR']

    @property
    def SMALL_GEAR_TRAY(self):
        """Message constant 'SMALL_GEAR_TRAY'."""
        return Metaclass_Object.__constants['SMALL_GEAR_TRAY']

    @property
    def MEDIUM_GEAR_TRAY(self):
        """Message constant 'MEDIUM_GEAR_TRAY'."""
        return Metaclass_Object.__constants['MEDIUM_GEAR_TRAY']

    @property
    def LARGE_GEAR_TRAY(self):
        """Message constant 'LARGE_GEAR_TRAY'."""
        return Metaclass_Object.__constants['LARGE_GEAR_TRAY']

    @property
    def M2L1_KIT_TRAY(self):
        """Message constant 'M2L1_KIT_TRAY'."""
        return Metaclass_Object.__constants['M2L1_KIT_TRAY']

    @property
    def S2L2_KIT_TRAY(self):
        """Message constant 'S2L2_KIT_TRAY'."""
        return Metaclass_Object.__constants['S2L2_KIT_TRAY']


class Object(metaclass=Metaclass_Object):
    """
    Message class 'Object'.

    Constants:
      KIT_TRAY
      PART_TRAY
      PART
      SMALL_GEAR
      MEDIUM_GEAR
      LARGE_GEAR
      SMALL_GEAR_TRAY
      MEDIUM_GEAR_TRAY
      LARGE_GEAR_TRAY
      M2L1_KIT_TRAY
      S2L2_KIT_TRAY
    """

    __slots__ = [
        '_name',
        '_object_type',
        '_object_identifier',
        '_pose_stamped',
        '_check_fields',
    ]

    _fields_and_field_types = {
        'name': 'string',
        'object_type': 'uint8',
        'object_identifier': 'uint8',
        'pose_stamped': 'geometry_msgs/PoseStamped',
    }

    # This attribute is used to store an rosidl_parser.definition variable
    # related to the data type of each of the components the message.
    SLOT_TYPES = (
        rosidl_parser.definition.UnboundedString(),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.NamespacedType(['geometry_msgs', 'msg'], 'PoseStamped'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        if 'check_fields' in kwargs:
            self._check_fields = kwargs['check_fields']
        else:
            self._check_fields = ros_python_check_fields == '1'
        if self._check_fields:
            assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
                'Invalid arguments passed to constructor: %s' % \
                ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.name = kwargs.get('name', str())
        self.object_type = kwargs.get('object_type', int())
        self.object_identifier = kwargs.get('object_identifier', int())
        from geometry_msgs.msg import PoseStamped
        self.pose_stamped = kwargs.get('pose_stamped', PoseStamped())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.get_fields_and_field_types().keys(), self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    if self._check_fields:
                        assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.name != other.name:
            return False
        if self.object_type != other.object_type:
            return False
        if self.object_identifier != other.object_identifier:
            return False
        if self.pose_stamped != other.pose_stamped:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def name(self):
        """Message field 'name'."""
        return self._name

    @name.setter
    def name(self, value):
        if self._check_fields:
            assert \
                isinstance(value, str), \
                "The 'name' field must be of type 'str'"
        self._name = value

    @builtins.property
    def object_type(self):
        """Message field 'object_type'."""
        return self._object_type

    @object_type.setter
    def object_type(self, value):
        if self._check_fields:
            assert \
                isinstance(value, int), \
                "The 'object_type' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'object_type' field must be an unsigned integer in [0, 255]"
        self._object_type = value

    @builtins.property
    def object_identifier(self):
        """Message field 'object_identifier'."""
        return self._object_identifier

    @object_identifier.setter
    def object_identifier(self, value):
        if self._check_fields:
            assert \
                isinstance(value, int), \
                "The 'object_identifier' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'object_identifier' field must be an unsigned integer in [0, 255]"
        self._object_identifier = value

    @builtins.property
    def pose_stamped(self):
        """Message field 'pose_stamped'."""
        return self._pose_stamped

    @pose_stamped.setter
    def pose_stamped(self, value):
        if self._check_fields:
            from geometry_msgs.msg import PoseStamped
            assert \
                isinstance(value, PoseStamped), \
                "The 'pose_stamped' field must be a sub message of type 'PoseStamped'"
        self._pose_stamped = value
