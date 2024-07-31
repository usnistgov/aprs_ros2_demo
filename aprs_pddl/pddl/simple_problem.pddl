(define (problem teach)
    (:domain aprs_demo)
    (:objects
        gear_1 gear_2 gear_3 gear_4 - part
        pt_1 kt_1 pt_2 kt_2 pt_3 kt_3 pt_4 kt_4 - slot
        small medium large - size
    )

    (:init
        (in_part_tray gear_1 pt_1 small)
        (in_part_tray gear_4 pt_4 small)
        (in_part_tray gear_2 pt_2 large)
        ; (in_part_tray gear_3 pt_3 medium)

        (kit_slot_occupied kt_2 medium)
        (kit_slot_empty kt_1 small)
        (kit_slot_empty kt_3 large)
        (kit_slot_empty kt_4 small)

        (robot_on_standby)
        (gripper_empty)
    )

    (:goal
        (and
            (kit_slot_occupied kt_1 small)
            (kit_slot_occupied kt_4 small)
            (kit_slot_empty kt_2 medium)
            (kit_slot_occupied kt_3 large)
        )
    )
)
