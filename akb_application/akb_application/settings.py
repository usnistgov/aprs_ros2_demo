# ROS

DETECTION_AREAS: list[str] = [
    'fanuc_table',
    'fanuc_conveyor',
    'motoman_table',
    'motoman_conveyor',
    'teach_table',
]

TRAYS_INFO_NAMES: dict[str, str] = {
    'fanuc_table': 'fanuc/table_trays_info',
    'fanuc_conveyor': 'fanuc/conveyor_trays_info',
    'motoman_table': 'motoman/table_trays_info',
    'motoman_conveyor': 'motoman/conveyor_trays_info',
    'teach_table': 'teach/table_trays_info',
}

REACHABLE_AREAS: dict[str, list[str]] = {
    'fanuc': ['fanuc_table', 'fanuc_conveyor'],
    'motoman': ['motoman_table', 'motoman_conveyor']
}

# REACHABLE_AREAS: dict[str, list[str]] = {
#     'fanuc': ['fanuc_table'],
#     'motoman': ['motoman_table']
# }

LOCATE_TRAYS_NAMES: dict[str, str] = {
    'fanuc_table': 'fanuc/locate_trays_on_table',
    'fanuc_conveyor': 'fanuc/locate_trays_on_conveyor',
    'motoman_table': 'motoman/locate_trays_on_table',
    'motoman_conveyor': 'motoman/locate_trays_on_conveyor',
    'teach_table': 'teach/locate_trays_on_table',
}

PLANNING_SCENE_NAMES = {
    "fanuc": "fanuc/initialize_planning_scene",
    "motoman": "motoman/initialize_planning_scene"
}

ACTUATE_GRIPPER_NAMES = {
    "fanuc": "fanuc/actuate_gripper",
    "motoman": "motoman/actuate_gripper"
}

MOVE_TO_POSE_NAMES = {
    "fanuc": "fanuc/move_to_named_pose",
    "motoman": "motoman/move_to_named_pose"
}

PICK_NAMES = {
    "fanuc": "fanuc/pick_from_slot",
    "motoman": "motoman/pick_from_slot"
}

PLACE_NAMES = {
    "fanuc": "fanuc/place_in_slot",
    "motoman": "motoman/place_in_slot"
}

PDDL_NAMES = {
    "plan": "generate_pddl_plan",
    "execute": "execute_pddl_plan",
    "clear": "clear_pddl_current_state",
}

CONTROLLER_STATUS_NAMES = {
    "fanuc": {
        "state": "fanuc/joint_state_broadcaster_status",
        "command": "fanuc/joint_trajectory_controller_status",
        "gripper": "fanuc/pneumatic_gripper_status"
    },
    "motoman": {
        "state": "motoman/joint_state_broadcaster_status",
        "command": "motoman/joint_trajectory_controller_status",
        "gripper": "motoman/pneumatic_gripper_status"
    },
}

NAMED_POSES = {
    "fanuc": [
        'home',
        'above_table',
        'above_conveyor',
        'conveyor_home'
    ],
    "motoman": [
        'home',
        'above_conveyor',
    ]
}
    

# FONTS

FONT_FAMILY = 'Arial'

TITLE_FONT_SIZE = 18
DROP_DOWN_FONT_SIZE = 14
BUTTON_FONT_SIZE = 16
TEXT_FONT_SIZE = 16
STATUS_FONT_SIZE = 14
SWITCH_FONT_SIZE = 18

TITLE_FONT_WEIGHT = 'bold'
DROP_DOWN_FONT_WEIGHT = 'normal'
BUTTON_FONT_WEIGHT = 'normal'
TEXT_FONT_WEIGHT = 'normal'
STATUS_FONT_WEIGHT = 'bold'
SWITCH_FONT_WEIGHT = 'bold'

# Colors

RED = '#E15554'
DARK_RED = '#B72A2A'
BLUE = '#4D9DE0'
DARK_BLUE = '#1C629C'
YELLOW = '#E1BC29'
DARK_YELLOW = '#A18517'
GREEN = '#3BB273'
DARK_GREEN = '#246B45'
PURPLE = '#7768AE'
DARK_PURPLE = '#3C325D'
DARK_GRAY = '#3A3A3A'
GRAY = '#616161'
LIGHT_GRAY = '#cfcfcf'

# PDDL Planning Table Label Widths
ROBOT_LABEL_WIDTH = 90
GEAR_SIZE_LABEL_WIDTH = 85
AREA_LABEL_WIDTH = 140
ACTION_LABEL_WIDTH = 75
SLOT_LABEL_WIDTH = 220


