#!/usr/bin/env python3

import rclpy
import cv2
from aprs_vision.calibration_tool import CalibrationTool, CameraException, CalibrationException

def main(args=None):
    rclpy.init(args=args)

    try: 
        calibration_tool = CalibrationTool()
    except CameraException as e:
        print(e)
        return
    try:
        rotated_img = calibration_tool.set_rotation(calibration_tool.get_frame())

        crop_start, crop_end = calibration_tool.select_region(rotated_img, 'cropped region')

        cropped_img = rotated_img[
            crop_start[1] - calibration_tool.crop_offset:crop_end[1] + calibration_tool.crop_offset,
            crop_start[0] - calibration_tool.crop_offset:crop_end[0] + calibration_tool.crop_offset
        ]

        threshold_img = calibration_tool.set_hsv_threshold(cropped_img)

        remove_excess_img = calibration_tool.remove_excess_detected_points(threshold_img)

        column_start, column_end = calibration_tool.select_region(remove_excess_img, 'column')

        row_start, row_end = calibration_tool.select_region(remove_excess_img, 'row')

        print(f'Column start: {column_start}  Column end: {column_end}  Row start: {row_start}  Row end: {row_end}')

    except CalibrationException as e:
        print(e)
        return


if __name__ == '__main__':
    main()