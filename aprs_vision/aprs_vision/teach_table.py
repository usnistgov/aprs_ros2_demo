import cv2
import numpy as np
from aprs_vision.vision_table import VisionTable
from aprs_vision.gear_detection import GearDetection
from aprs_interfaces.msg import SlotInfo
from cv2.typing import MatLike
from typing import Optional

class TeachTable(VisionTable):
    video_stream = "http://192.168.1.105/mjpg/video.mjpg"
    map_x_image = 'teach_table_map_x.npy'
    map_y_image = 'teach_table_map_y.npy'
    background_image = 'teach_table_background.jpg'
    publish_frames = False

    conversion_factor = 0.8466 # 30 pixels is 25.4 mm so 1 pixel is .8466 mm

    gear_detection_values = {
        SlotInfo.LARGE: GearDetection((40, 90, 123), (83, 186, 255), 60),
        SlotInfo.MEDIUM: GearDetection((0, 100, 230), (20, 175, 255), 44),
        SlotInfo.SMALL: GearDetection((10, 10, 205), (45, 145, 255), 30)
    }

    def __init__(self):
        super().__init__("teach_table", "teach_table_vision")

    def remove_background(self, frame: MatLike) -> MatLike:
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # cv2.imwrite('hsv.jpg', hsv)

        no_background = cv2.inRange(hsv, (0, 0, 135), (255, 255, 255)) # type: ignore      
        
        canvas = np.zeros(no_background.shape, dtype=np.uint8)

        contours,_ = cv2.findContours(no_background, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        filtered_contours = []
        for c in contours:
            if cv2.contourArea(c) > 100:
                filtered_contours.append(c)

        cv2.drawContours(canvas, filtered_contours, -1, color=255, thickness=cv2.FILLED) # type: ignore

        return cv2.bitwise_and(frame, frame, mask=canvas)
    
    # def generate_grid_maps(self, frame: MatLike) -> Optional[MatLike]:
    #     offset = 15

    #     # Corners are manually deduced from location of screw heads in table
    #     top_left = (145 + offset, 140 + offset)
    #     top_right = (700 - offset, 140 + offset)
    #     bottom_right = (700 - offset, 500 - offset)
    #     bottom_left = (145 + offset, 500 - offset)

    #     # Black out everything from image that is not the active region
    #     teach_table_table_corners = np.array([top_right, bottom_right, bottom_left, top_left])

    #     maskImage = np.zeros(frame.shape, dtype=np.uint8)
    #     cv2.drawContours(maskImage, [teach_table_table_corners], 0, (255, 255, 255), -1)

    #     active_region = cv2.bitwise_and(frame, maskImage)

    #     # Detect optical table holes 
    #     blur = cv2.GaussianBlur(active_region,(5,5),0)

    #     hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)

    #     threshold = cv2.inRange(hsv, (0, 0, 130), (255, 50, 255)) # type: ignore
        
    #     contours, _ = cv2.findContours(threshold, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

    #     rows = 20
    #     columns = 32

    #     if not len(contours) == rows * columns:
    #         self.get_logger().error("Not able to detect all holes")
    #         return None

    #     center_points = []
        
    #     canvas = np.zeros(frame.shape, dtype=np.uint8)

    #     for contour in contours:
    #         # Calculate moments for each contour
    #         M = cv2.moments(contour)

    #         # Calculate center of contour
    #         if M["m00"] != 0:
    #             cX = int(M["m10"] / M["m00"])
    #             cY = int(M["m01"] / M["m00"])

    #             center_points.append((cX, cY))

    #         cv2.circle(canvas, (cX, cY), 3, (255, 255, 255), -1)

    #     center_y = 167
    #     sorted_points = []
    #     working_points = []

    #     for i in range(rows):
    #         for point in center_points:
    #             if center_y - 8 <= point[1] <= center_y + 8:
    #                 working_points.append(point)
                
    #         sorted_points += sorted(working_points, key=lambda k: [k[0]])

    #         working_points.clear()

    #         cv2.line(canvas, (0, center_y), (1000, center_y), (0, 255, 0), 1)

    #         center_y += 16.6
    #         center_y = int(center_y)
            
    #     if not len(sorted_points) == len(center_points):
    #         self.get_logger().error("Not able to properly sort holes")
    #         return
        
    #     actual_points = []

    #     for i in range(rows):
    #         x = (i * 30)
    #         for j in range(columns):
    #             y = (j * 30)
    #             actual_points.append([x, y])

    #     grid_x, grid_y = np.mgrid[0:rows*30, 0:columns*30]

    #     destination = np.array(actual_points)
    #     source = np.array(sorted_points)

    #     grid_z = griddata(destination, source, (grid_x, grid_y), method='cubic')
    #     map_x = np.append([], [ar[:,0] for ar in grid_z]).reshape(rows*30,columns*30)
    #     map_y = np.append([], [ar[:,1] for ar in grid_z]).reshape(rows*30,columns*30)
    #     map_x_32 = map_x.astype('float32')
    #     map_y_32 = map_y.astype('float32')

    #     np.save("map_x.npy", map_x_32)
    #     np.save("map_y.npy", map_y_32)
        
    #     return canvas