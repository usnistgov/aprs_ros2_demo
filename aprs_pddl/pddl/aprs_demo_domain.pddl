(define (domain aprs_demo)
    (:requirements :typing :durative-actions :negative-preconditions)
    (:types 
        part slot size
    )
    
    (:predicates
        (in_part_tray ?obj - part ?slot_name - slot ?size - size)
        (kit_slot_occupied ?slot_name - slot ?size - size)
        (kit_slot_empty ?slot_name - slot ?size - size)
        (in_gripper ?obj - part ?size - size)
        (robot_on_standby)
        (gripper_empty) 

    )

    (:durative-action pick
        :parameters (?obj - part ?slot_name - slot ?size - size)
         :duration
            (= ?duration 5)
        :condition (and 
            (at start (gripper_empty))
            (at start (in_part_tray ?obj ?slot_name ?size))
            (at start (robot_on_standby))
        )
        :effect (and 
            (at end (not (in_part_tray ?obj ?slot_name ?size)))
            (at end (in_gripper ?obj ?size))
            (at start (not (robot_on_standby)))
            (at end (robot_on_standby))
            (at end (not (gripper_empty)))
        )
    )

    (:durative-action place
        :parameters (?obj - part ?slot_name - slot ?size - size)
         :duration
            (= ?duration 5)
        :condition (and
            (at start (in_gripper ?obj ?size))
            (at start (robot_on_standby))
            (at start (kit_slot_empty ?slot_name ?size))
        )
        :effect (and 
            (at end (not (in_gripper ?obj ?size)))
            (at end (kit_slot_occupied ?slot_name ?size))
            (at end (not (kit_slot_empty ?slot_name ?size)))
            (at start (not (robot_on_standby)))
            (at end (robot_on_standby))
            (at end (gripper_empty))
        )
    )

    (:durative-action remove
        :parameters (?slot_name - slot ?size - size)
         :duration
            (= ?duration 5)
        :condition (and 
            (at start (gripper_empty))
            (at start (kit_slot_occupied ?slot_name ?size))
            (at start (robot_on_standby))
        )
        :effect (and 
            (at end (not (kit_slot_occupied ?slot_name ?size)))
            (at end (kit_slot_empty ?slot_name ?size))
            (at start (not (robot_on_standby)))
            (at end (robot_on_standby))
            (at end (gripper_empty))
        )
    )
)