(define (domain aprs_demo)
   (:requirements :strips :typing :durative-actions :universal-preconditions :equality )

    (:types 
        slot size robot location
    )
    
    (:constants
        fanuc motoman - robot
        small medium large - size
        fanuc_table motoman_table fanuc_conveyor motoman_conveyor - location
    )
    
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

    (:durative-action fanuc_pick
        :parameters (?slot_name - slot ?size - size ?robot - robot ?location - location)
        :duration (= ?duration 5)
        :condition (and 
            (at start (and
                (in_reach ?location fanuc)
                (slot_at ?slot_name ?location)
                (gripper_empty fanuc)
                (slot_occupied ?slot_name ?size)
                (robot_on_standby fanuc)
                (robot_operational fanuc)
            ))
        )
        :effect (and 
            (at start (not (robot_on_standby fanuc)))
            (at end (and
                (not (slot_occupied ?slot_name ?size))
                (slot_empty ?slot_name ?size)
                (in_gripper ?size fanuc)
                (robot_on_standby fanuc)
                (not (gripper_empty fanuc))
            ))
        )
    )

    (:durative-action fanuc_place
        :parameters (?slot_name - slot ?size - size ?robot - robot ?location - location)
        :duration (= ?duration 5)
        :condition (and
            (at start (and
                (in_gripper ?size fanuc)
                (in_reach ?location fanuc)
                (slot_at ?slot_name ?location)
                (robot_on_standby fanuc)
                (slot_empty ?slot_name ?size)
                (robot_operational fanuc)
            ))
        )
        :effect (and 
            (at start (not (robot_on_standby fanuc)))
            (at end (and
                (slot_occupied ?slot_name ?size)
                (not (slot_empty ?slot_name ?size))
                (robot_on_standby fanuc)
                (gripper_empty fanuc)
                (not (in_gripper ?size fanuc))
            ))
        )
    )

        (:durative-action motoman_pick
        :parameters (?slot_name - slot ?size - size ?robot - robot ?location - location)
        :duration (= ?duration 5)
        :condition (and 
            (at start (and
                (in_reach ?location motoman)
                (slot_at ?slot_name ?location)
                (gripper_empty motoman)
                (slot_occupied ?slot_name ?size)
                (robot_on_standby motoman)
                (robot_operational motoman)
            ))
        )
        :effect (and 
            (at start (not (robot_on_standby motoman)))
            (at end (and
                (not (slot_occupied ?slot_name ?size))
                (slot_empty ?slot_name ?size)
                (in_gripper ?size motoman)
                (robot_on_standby motoman)
                (not (gripper_empty motoman))
            ))
        )
    )

    (:durative-action motoman_place
        :parameters (?slot_name - slot ?size - size ?robot - robot ?location - location)
        :duration (= ?duration 5)
        :condition (and
            (at start (and
                (in_gripper ?size motoman)
                (in_reach ?location motoman)
                (slot_at ?slot_name ?location)
                (robot_on_standby motoman)
                (slot_empty ?slot_name ?size)
                (robot_operational motoman)
            ))
        )
        :effect (and 
            (at start (not (robot_on_standby motoman)))
            (at end (and
                (slot_occupied ?slot_name ?size)
                (not (slot_empty ?slot_name ?size))
                (robot_on_standby motoman)
                (gripper_empty motoman)
                (not (in_gripper ?size motoman))
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