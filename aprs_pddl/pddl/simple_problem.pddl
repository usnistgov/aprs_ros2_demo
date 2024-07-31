(define (problem teach)
    (:domain aprs_demo)
    (:objects
        small - size
        medium - size
        large - size
        m2l1_kit_tray_1_lg_1 - slot
        m2l1_kit_tray_1_mg_1 - slot
        m2l1_kit_tray_1_mg_2 - slot
        m2l1_kit_tray_2_lg_1 - slot
        m2l1_kit_tray_2_mg_1 - slot
        m2l1_kit_tray_2_mg_2 - slot
        large_gear_tray_1_slot_1 - slot
        large_gear_tray_1_slot_2 - slot
        medium_gear_tray_1_slot_1 - slot
        medium_gear_tray_1_slot_2 - slot
        medium_gear_tray_1_slot_3 - slot
        medium_gear_tray_1_slot_4 - slot
        m2l1_kit_tray_3_lg_1 - slot
        m2l1_kit_tray_3_mg_1 - slot
        m2l1_kit_tray_3_mg_2 - slot
        m2l1_kit_tray_4_lg_1 - slot
        m2l1_kit_tray_4_mg_1 - slot
        m2l1_kit_tray_4_mg_2 - slot
        large_0 - part
        large_1 - part
        medium_0 - part
        medium_1 - part
        medium_2 - part
        medium_3 - part
    )

    (:init
        (robot_on_standby)
        (gripper_empty)

        (kit_slot_empty m2l1_kit_tray_1_lg_1 large)
        (kit_slot_empty m2l1_kit_tray_1_mg_1 medium)
        (kit_slot_empty m2l1_kit_tray_1_mg_2 medium)
        (kit_slot_empty m2l1_kit_tray_2_lg_1 large)
        (kit_slot_empty m2l1_kit_tray_2_mg_1 medium)
        (kit_slot_empty m2l1_kit_tray_2_mg_2 medium)

        (in_part_tray large_0 large_gear_tray_1_slot_1 large)
        (in_part_tray large_1 large_gear_tray_1_slot_2 large)

        (in_part_tray medium_0 medium_gear_tray_1_slot_1 medium)
        (in_part_tray medium_1 medium_gear_tray_1_slot_2 medium)
        (in_part_tray medium_2 medium_gear_tray_1_slot_3 medium)
        (in_part_tray medium_3 medium_gear_tray_1_slot_4 medium)
    )

    (:goal
        (and 
            (kit_slot_occupied m2l1_kit_tray_1_lg_1 large)
            (kit_slot_occupied m2l1_kit_tray_1_mg_1 medium)
            (kit_slot_empty m2l1_kit_tray_1_mg_2 medium)
            (kit_slot_occupied m2l1_kit_tray_2_lg_1 large)
            (kit_slot_occupied m2l1_kit_tray_2_mg_1 medium)
            (kit_slot_occupied m2l1_kit_tray_2_mg_2 medium)
        )
    )
)
