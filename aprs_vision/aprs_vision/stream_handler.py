#!/usr/bin/env python3

import os
from typing import Optional

import cv2
from cv2.typing import MatLike

import numpy as np
from numpy.lib.npyio import NpzFile

class StreamException(Exception):
    pass

class StreamHandler:
    def __init__(self, video_stream: str, calibration_file: str):
        self.capture = cv2.VideoCapture(video_stream)

        ret, frame = self.capture.read()

        if not ret:
            raise StreamException("Unable to connect to camera stream")

        if not os.path.exists(calibration_file):
            raise StreamException(f"Calibration file not found at [{calibration_file}]")

        calibration: NpzFile = np.load(calibration_file)

        try:
            self.map_x = calibration['map_x']
            self.map_y = calibration['map_y']

            self.rotation_matrix: Optional[MatLike]
            angle = int(calibration['angle'])

            if angle != 0:
                self.rotation_matrix = cv2.getRotationMatrix2D((frame.shape[1] / 2, frame.shape[0] / 2), angle, 1)
            else:
                self.rotation_matrix = None

            crop_start = calibration['crop_start']
            self.crop_start_x = int(crop_start[0])
            self.crop_start_y = int(crop_start[1])

            crop_end = calibration['crop_end']
            self.crop_end_x = int(crop_end[0])
            self.crop_end_y = int(crop_end[1])

        except KeyError as e:
            raise StreamException(e) from e

    def read_frame(self) -> MatLike:
        """ Reads a frame from the video stream and returns the rectified image based on the calibration parameters

        Raises:
            StreamException: Exception on lost connection

        Returns:
            MatLike: Rotated, cropped, and remapped frame
        """
        ret, frame = self.capture.read()

        if not ret:
            raise StreamException("Lost connection to camera")

        return self.rectify_frame(frame)

    def rectify_frame(self, frame: MatLike) -> MatLike:
        """Takes in image from camera and rectifies it based on the calibration data and returns the recitified image. This rectification
        process includes rotating the image, cropping the image, and applying the mapping so that 30 pixels is one inch
        
        Args:
            frame (MatLike): Input image from camera
        
        Returns:
            MatLike: Rectified image
        """

        if self.rotation_matrix is not None:
            frame = cv2.warpAffine(frame, self.rotation_matrix, (frame.shape[1], frame.shape[0]))

        frame = frame[self.crop_start_y:self.crop_end_y, self.crop_start_x:self.crop_end_x]

        frame = cv2.remap(frame, self.map_x, self.map_y, cv2.INTER_CUBIC)

        return frame
