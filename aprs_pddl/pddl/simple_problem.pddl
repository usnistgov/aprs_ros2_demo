(define (problem kit_building)
    (:domain aprs_demo)
    (:objects
        fanuc motoman - robot
        small medium large - size
        fanuc_table motoman_table fanuc_conveyor motoman_conveyor - location
        
        m2l1_kit_tray_1_lg_1 m2l1_kit_tray_1_mg_1 m2l1_kit_tray_1_mg_2 - slot
        m2l1_kit_tray_2_lg_1 m2l1_kit_tray_2_mg_1 m2l1_kit_tray_2_mg_2 - slot
        m2l1_kit_tray_3_lg_1 m2l1_kit_tray_3_mg_1 m2l1_kit_tray_3_mg_2 - slot
        large_gear_tray_1_slot_1 large_gear_tray_1_slot_2 - slot
        large_gear_tray_2_slot_1 large_gear_tray_2_slot_2 - slot
        large_gear_tray_3_slot_1 large_gear_tray_3_slot_2 - slot
        medium_gear_tray_1_slot_1 medium_gear_tray_1_slot_2 medium_gear_tray_1_slot_3 medium_gear_tray_1_slot_4 - slot
        medium_gear_tray_2_slot_1 medium_gear_tray_2_slot_2 medium_gear_tray_2_slot_3 medium_gear_tray_2_slot_4 - slot
    )

    (:init
        (robot_on_standby fanuc)
        (robot_on_standby motoman)
        (gripper_empty motoman)
        (gripper_empty fanuc)
        (conveyor_used_by fanuc)

        ; (robot_faulty motoman)
        ; (robot_faulty fanuc)
        (robot_operational motoman)
        (robot_operational fanuc)
        ; (in_reach motoman_table fanuc)

        (in_reach fanuc_table fanuc)
        (in_reach motoman_table motoman)
        (in_reach fanuc_conveyor fanuc)
        (in_reach motoman_conveyor motoman)

        (slot_occupied large_gear_tray_1_slot_1 large)
        (slot_occupied large_gear_tray_1_slot_2 large)

        (slot_occupied large_gear_tray_2_slot_1 large)
        (slot_occupied large_gear_tray_2_slot_2 large)

        (slot_occupied large_gear_tray_3_slot_1 large)
        (slot_occupied large_gear_tray_3_slot_2 large)

        (slot_occupied medium_gear_tray_1_slot_1 medium)
        (slot_occupied medium_gear_tray_1_slot_2 medium)
        (slot_occupied medium_gear_tray_1_slot_3 medium)
        (slot_empty medium_gear_tray_1_slot_4 medium)

        (slot_occupied medium_gear_tray_2_slot_1 medium)
        (slot_occupied medium_gear_tray_2_slot_2 medium)
        (slot_occupied medium_gear_tray_2_slot_3 medium)
        (slot_empty medium_gear_tray_2_slot_4 medium)

        (slot_empty m2l1_kit_tray_1_lg_1 large)
        (slot_empty m2l1_kit_tray_1_mg_1 medium)
        (slot_occupied m2l1_kit_tray_1_mg_2 medium)

        (slot_empty m2l1_kit_tray_2_lg_1 large)
        (slot_empty m2l1_kit_tray_2_mg_1 medium)
        (slot_occupied m2l1_kit_tray_2_mg_2 medium)

        (slot_empty m2l1_kit_tray_3_lg_1 large)
        (slot_empty m2l1_kit_tray_3_mg_1 medium)
        (slot_empty m2l1_kit_tray_3_mg_2 medium)

        (slot_at m2l1_kit_tray_1_lg_1 fanuc_table)
        (slot_at m2l1_kit_tray_1_mg_1 fanuc_table)
        (slot_at m2l1_kit_tray_1_mg_2 fanuc_table)
        (slot_at large_gear_tray_1_slot_1 fanuc_table)
        (slot_at large_gear_tray_1_slot_2 fanuc_table)
        (slot_at large_gear_tray_3_slot_1 fanuc_table)
        (slot_at large_gear_tray_3_slot_2 fanuc_table)
        (slot_at medium_gear_tray_1_slot_1 fanuc_table)
        (slot_at medium_gear_tray_1_slot_2 fanuc_table)
        (slot_at medium_gear_tray_1_slot_3 fanuc_table)
        (slot_at medium_gear_tray_1_slot_4 fanuc_table)

        (slot_at m2l1_kit_tray_2_lg_1 motoman_table)
        (slot_at m2l1_kit_tray_2_mg_1 motoman_table)
        (slot_at m2l1_kit_tray_2_mg_2 motoman_table)
        (slot_at large_gear_tray_2_slot_1 motoman_table)
        (slot_at large_gear_tray_2_slot_2 motoman_table)
        (slot_at medium_gear_tray_2_slot_1 motoman_table)
        (slot_at medium_gear_tray_2_slot_2 motoman_table)
        (slot_at medium_gear_tray_2_slot_3 motoman_table)
        (slot_at medium_gear_tray_2_slot_4 motoman_table)

        (slot_at m2l1_kit_tray_3_lg_1 fanuc_conveyor)
        (slot_at m2l1_kit_tray_3_mg_1 fanuc_conveyor)
        (slot_at m2l1_kit_tray_3_mg_2 fanuc_conveyor)
                
    )

    (:goal
        (and 
            (slot_occupied m2l1_kit_tray_1_lg_1 large)
            (slot_occupied m2l1_kit_tray_1_mg_1 medium)
            (slot_empty m2l1_kit_tray_1_mg_2 medium)
            (slot_occupied m2l1_kit_tray_2_lg_1 large)
            (slot_occupied m2l1_kit_tray_2_mg_1 medium)
            (slot_empty m2l1_kit_tray_2_mg_2 medium)
            (slot_occupied m2l1_kit_tray_3_lg_1 large)
            (slot_occupied m2l1_kit_tray_3_mg_1 medium)
            (slot_occupied m2l1_kit_tray_3_mg_2 medium)
            (gripper_empty motoman)
            (gripper_empty fanuc)
        )
    )

)