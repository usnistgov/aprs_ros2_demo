import cv2
import os
import json

ARUCO_DICT = cv2.aruco.DICT_4X4_250  # Dictionary ID
SQUARES_VERTICALLY = 5               # Number of squares vertically
SQUARES_HORIZONTALLY = 7             # Number of squares horizontally
SQUARE_LENGTH = .0338                   # Square side length (in meters)
MARKER_LENGTH = .024                   # ArUco marker side length (in meters)

def get_calibration_parameters(image_file):
    # Define the aruco dictionary, charuco board and detector
    dictionary = cv2.aruco.getPredefinedDictionary(ARUCO_DICT)
    board = cv2.aruco.CharucoBoard((SQUARES_VERTICALLY, SQUARES_HORIZONTALLY), SQUARE_LENGTH, MARKER_LENGTH, dictionary)
    params = cv2.aruco.DetectorParameters()
    detector = cv2.aruco.ArucoDetector(dictionary, params)
    
    # Load images from directory
    # image_files = [os.path.join(img_dir, f) for f in os.listdir(img_dir) if f.endswith(".mp4")]
    all_charuco_ids = []
    all_charuco_corners = []

    # Loop over images and extraction of corners
    # for image_file in image_files:

    image = cv2.imread(image_file)
    if image is None:
        print('Bad Read')
        exit()
    image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    imgSize = image.shape
    image_copy = image.copy()
    marker_corners, marker_ids, rejectedCandidates = detector.detectMarkers(image)
    
    if len(marker_ids) > 0: # If at least one marker is detected
        # cv2.aruco.drawDetectedMarkers(image_copy, marker_corners, marker_ids)
        ret, charucoCorners, charucoIds = cv2.aruco.interpolateCornersCharuco(marker_corners, marker_ids, image, board)

        if charucoIds is not None and len(charucoCorners) > 3:
            all_charuco_corners.append(charucoCorners)
            all_charuco_ids.append(charucoIds)
    
    # Calibrate camera with extracted information
    result, mtx, dist, rvecs, tvecs = cv2.aruco.calibrateCameraCharuco(all_charuco_corners, all_charuco_ids, board, imgSize, None, None)
    return mtx, dist

OUTPUT_JSON = 'fanuc_conveyor_calibration.json'

mtx, dist = get_calibration_parameters('fanuc_conveyor_calibration.mp4')
data = {"mtx": mtx.tolist(), "dist": dist.tolist()}

with open(OUTPUT_JSON, 'w') as json_file:
    json.dump(data, json_file, indent=4)

print(f'Data has been saved to {OUTPUT_JSON}')