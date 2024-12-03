#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from random import random, randint
import PyKDL

from aprs_interfaces.msg import Trays, Tray, SlotInfo


class MinimalPublisher(Node):

    def __init__(self):
        super().__init__('minimal_publisher')
        self.publisher_teach_ = self.create_publisher(Trays, '/teach/table_trays_info', 10)
        self.publisher_fanuc_ = self.create_publisher(Trays, '/fanuc/conveyor_trays_info', 10)
        self.publisher_motoman_ = self.create_publisher(Trays, '/motoman/conveyor_trays_info', 10)
        timer_period = 1.0  # seconds
        self.timer = self.create_timer(timer_period, self.publish_trays)
        self.i = 0

    def publish_trays(self):
        msg = Trays()

        m2l1_tray = Tray()
        m2l1_tray.identifier = Tray.M2L1_KIT_TRAY
        m2l1_tray.name = "m2l1_kit_tray_02"

        slot_lg_1 = SlotInfo()
        slot_lg_1.occupied = True
        slot_lg_1.size = SlotInfo.LARGE
        slot_lg_1.name = "m2l1_kit_tray_02_lg_1"
        m2l1_tray.slots.append(slot_lg_1)

        slot_mg_1 = SlotInfo()
        slot_mg_1.occupied = True
        slot_mg_1.size = SlotInfo.MEDIUM
        slot_mg_1.name = "m2l1_kit_tray_02_mg_1"
        m2l1_tray.slots.append(slot_mg_1)

        slot_mg_2 = SlotInfo()
        slot_mg_2.occupied = False
        slot_mg_2.size = SlotInfo.MEDIUM
        slot_mg_2.name = "m2l1_kit_tray_02_mg_2"
        m2l1_tray.slots.append(slot_mg_2)

        s2l2_tray = Tray()
        s2l2_tray.identifier = Tray.S2L2_KIT_TRAY
        s2l2_tray.name = "s2l2_kit_tray_05"

        slot_lg_1 = SlotInfo()
        slot_lg_1.occupied = True
        slot_lg_1.size = SlotInfo.LARGE
        slot_lg_1.name = "s2l2_kit_tray_05_lg_1"
        s2l2_tray.slots.append(slot_lg_1)

        slot_lg_2 = SlotInfo()
        slot_lg_2.occupied = False
        slot_lg_2.size = SlotInfo.LARGE
        slot_lg_2.name = "s2l2_kit_tray_05_lg_2"
        s2l2_tray.slots.append(slot_lg_2)

        slot_sg_1 = SlotInfo()
        slot_sg_1.occupied = True
        slot_sg_1.size = SlotInfo.SMALL
        slot_sg_1.name = "s2l2_kit_tray_05_sg_1"
        s2l2_tray.slots.append(slot_sg_1)

        slot_sg_2 = SlotInfo()
        slot_sg_2.occupied = False
        slot_sg_2.size = SlotInfo.SMALL
        slot_sg_2.name = "s2l2_kit_tray_05_sg_2"
        s2l2_tray.slots.append(slot_sg_2)

        msg.kit_trays.append(m2l1_tray)
        msg.kit_trays.append(s2l2_tray)
        # self.publisher_teach_.publish(msg)

        empty_trays = Trays()
        self.publisher_fanuc_.publish(empty_trays)
        self.publisher_motoman_.publish(empty_trays)


def main(args=None):
    rclpy.init(args=args)

    minimal_publisher = MinimalPublisher()

    rclpy.spin(minimal_publisher)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_publisher.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()