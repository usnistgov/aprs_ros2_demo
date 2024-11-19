(define (domain aprs_demo)
   (:requirements :strips :typing :durative-actions :universal-preconditions :equality )

    (:types 
        slot size robot location
    )
    
    ; (:constants
    ;     fanuc motoman - robot
    ;     small medium large - size
    ;     fanuc_table motoman_table fanuc_conveyor motoman_conveyor - location
    ; )
    
    (:predicates
        (slot_occupied ?slot_name - slot ?size - size)
        (slot_empty ?slot_name - slot ?size - size)
        (in_gripper ?size - size ?robot - robot)
        (robot_on_standby ?robot - robot)
        (gripper_empty ?robot - robot)
        (in_reach ?location - location ?robot - robot)
        (robot_faulty ?robot - robot)
        (robot_operational ?robot - robot)
        (slot_at ?slot_name - slot ?location - location)
        (conveyor_used_by ?robot - robot)
    )   

    (:durative-action pick
        :parameters (?slot_name - slot ?size - size ?robot - robot ?location - location)
        :duration (= ?duration 5)
        :condition (and 
            (at start (and
                (in_reach ?location ?robot)
                (slot_at ?slot_name ?location)
                (gripper_empty ?robot)
                (slot_occupied ?slot_name ?size)
                (robot_on_standby ?robot)
                (robot_operational ?robot)
            ))
        )
        :effect (and 
            (at start (not (robot_on_standby ?robot)))
            (at end (and
                (not (slot_occupied ?slot_name ?size))
                (slot_empty ?slot_name ?size)
                (in_gripper ?size ?robot)
                (robot_on_standby ?robot)
                (not (gripper_empty ?robot))
            ))
        )
    )

    (:durative-action place
        :parameters (?slot_name - slot ?size - size ?robot - robot ?location - location)
        :duration (= ?duration 5)
        :condition (and
            (at start (and
                (in_gripper ?size ?robot)
                (in_reach ?location ?robot)
                (slot_at ?slot_name ?location)
                (robot_on_standby ?robot)
                (slot_empty ?slot_name ?size)
                (robot_operational ?robot)
            ))
        )
        :effect (and 
            (at start (not (robot_on_standby ?robot)))
            (at end (and
                (slot_occupied ?slot_name ?size)
                (not (slot_empty ?slot_name ?size))
                (robot_on_standby ?robot)
                (gripper_empty ?robot)
                (not (in_gripper ?size ?robot))
            ))
        )
    )

    (:durative-action move_and_transfer_conveyor
        :parameters ()
        :duration (= ?duration 1)
        :condition (and 
            (at start (conveyor_used_by fanuc))
            (at start (robot_faulty fanuc))
        )
        :effect (and 
            (at start (not (conveyor_used_by fanuc)))
            (at end (conveyor_used_by motoman))
            (at end (not (in_reach fanuc_conveyor fanuc)))
            (at end (in_reach fanuc_conveyor motoman))
        )
    )

)