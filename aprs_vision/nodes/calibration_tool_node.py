#!/usr/bin/env python3

import rclpy
from aprs_vision.calibration_tool import CalibrationTool, CameraException

def main(args=None):
    rclpy.init(args=args)

    try: 
        calibration_tool = CalibrationTool()
    except CameraException as e:
        print(e)
        return
    
    rotated_img = calibration_tool.set_rotation()
    
    if rotated_img is None:    
        print('Unable to calibrate')
    else:
        print('Set image rotation')

if __name__ == '__main__':
    main()