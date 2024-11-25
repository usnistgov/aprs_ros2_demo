#!/usr/bin/env python3

import rclpy
from aprs_vision.detection_area import DetectionArea, VisionException

def main(args=None):
    rclpy.init(args=args)

    try: 
        detection_area = DetectionArea()

        try:
            rclpy.spin(detection_area)
        except KeyboardInterrupt:
            pass

    except VisionException as e:
        print(e)
        return


if __name__ == '__main__':
    main()