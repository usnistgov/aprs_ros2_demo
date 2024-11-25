#!/usr/bin/env python3

import rclpy
import cv2
import numpy as np
import tkinter as tk
from tkinter import filedialog
from aprs_vision.calibration_tool import CalibrationTool, CameraException, CalibrationException

def main(args=None):
    rclpy.init(args=args)
    root = tk.Tk()
    root.withdraw()

    try: 
        calibration_tool = CalibrationTool()
    except CameraException as e:
        print(e)
        return
    try:
        rotated_img, angle = calibration_tool.set_rotation(calibration_tool.get_frame())

        crop_start, crop_end = calibration_tool.select_region(rotated_img, 'cropped region')

        cropped_img = rotated_img[
            crop_start[1] - calibration_tool.crop_offset:crop_end[1] + calibration_tool.crop_offset,
            crop_start[0] - calibration_tool.crop_offset:crop_end[0] + calibration_tool.crop_offset
        ]

        threshold_img = calibration_tool.set_hsv_threshold(cropped_img)

        remove_excess_img = calibration_tool.remove_excess_detected_points(threshold_img)

        column_start, column_end = calibration_tool.select_region(remove_excess_img, 'column')

        row_start, row_end = calibration_tool.select_region(remove_excess_img, 'row')

        all_holes, contours_in_selected_column, contours_in_selected_row = calibration_tool.detect_contours(
            remove_excess_img, 
            column_start, 
            column_end, 
            row_start, 
            row_end
        )

        sorted_holes = calibration_tool.sort_all_holes(remove_excess_img, column_start[1], all_holes, contours_in_selected_column)

        map_x, map_y = calibration_tool.create_mappings(sorted_holes, len(contours_in_selected_column), len(contours_in_selected_row))

        filepath = filedialog.asksaveasfilename(defaultextension='.npz')
        
        np.savez(filepath, map_x=map_x, map_y=map_y, angle=angle, crop_start=crop_start, crop_end=crop_end)

        print('Mapping Created')
    except CalibrationException as e:
        print(e)
        return


if __name__ == '__main__':
    main()