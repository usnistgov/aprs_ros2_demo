from aprs_interfaces.msg import Tray

class SlotOffsets():
    offsets = {
        Tray.SMALL_GEAR_TRAY: {
            'slot_1': (-0.045, 0.045),
            'slot_2': (0.045, 0.045),
            'slot_3': (-0.045, -0.045),
            'slot_4': (0.045, -0.045),
        },
        Tray.MEDIUM_GEAR_TRAY: {
            'slot_1': (-0.050, 0.050),
            'slot_2': (0.050, 0.050),
            'slot_3': (-0.050, -0.050),
            'slot_4': (0.050, -0.050),
        },
        Tray.LARGE_GEAR_TRAY: {
            'slot_1': (-0.052, 0.06),
            'slot_2': (0.052, 0.06),
        },
        Tray.M2L1_KIT_TRAY: {
            'lg_1': (0.0, -0.075),
            'mg_1': (-0.065, 0.0),
            'mg_2': (0.065, 0.0),
        },
        Tray.S2L2_KIT_TRAY: {
            'lg_1': (-0.052, 0.060),
            'lg_2': (0.052, 0.060),
            'sg_1': (-0.045, -0.045),
            'sg_2': (0.045, -0.045),
        },
    }
