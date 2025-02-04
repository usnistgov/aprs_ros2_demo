#!/usr/bin/env python3

import rclpy
from aprs_vision.detection_area import DetectionArea
from aprs_vision.stream_handler import StreamException

def main(args=None):
    rclpy.init(args=args)

    try:
        detection_area = DetectionArea()
        rclpy.spin(detection_area)

    except KeyboardInterrupt:
        return
    except StreamException as e:
        print(e)
        return

if __name__ == '__main__':
    main()
