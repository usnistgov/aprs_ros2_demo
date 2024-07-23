from aprs_interfaces.msg import Object


class SlotOffsets():
    offsets = {
        Object.S2L2_KIT_TRAY: {
            'lg_1': (-0.028, -0.055),
            'lg_2': (-0.028, 0.055),
            'sg_1': (0.055, -0.04),
            'sg_2': (0.055, 0.04),
        },
        Object.LARGE_GEAR_TRAY: {
            'slot_1': (0.0, 0.055),
            'slot_2': (0.0, -0.055),
        },
        Object.MEDIUM_GEAR_TRAY: {
            'slot_1': (0.038, -0.038),
            'slot_2': (0.038, 0.038),
            'slot_3': (-0.038, -0.038),
            'slot_4': (-0.038, 0.038),
        },
        Object.M2L1_KIT_TRAY: {
            'lg_1': (0.0, 0.04),
            'mg_1': (0.04, -0.055),
            'mg_2': (-0.04, -0.055),
        },
        Object.SMALL_GEAR_TRAY: {
            'slot_1': (0.028, -0.028),
            'slot_2': (0.028, 0.028),
            'slot_3': (-0.028, -0.028),
            'slot_4': (-0.028, 0.028),
        },
    }
