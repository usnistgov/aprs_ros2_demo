import math
import numpy as np

from geometry_msgs.msg import Quaternion

def euler_from_quaternion(quaternion: Quaternion) -> tuple[float, float, float]:
    """ Converts quaternion to euler roll, pitch, yaw

    Args:
        quaternion (Quaternion): input quaternion

    Returns:
        tuple[float, float, float]: roll, pitch, yaw
    """

    x = quaternion.x
    y = quaternion.y
    z = quaternion.z
    w = quaternion.w

    sinr_cosp = 2 * (w * x + y * z)
    cosr_cosp = 1 - 2 * (x * x + y * y)
    roll = np.arctan2(sinr_cosp, cosr_cosp)

    sinp = 2 * (w * y - z * x)
    pitch = np.arcsin(sinp)

    siny_cosp = 2 * (w * z + x * y)
    cosy_cosp = 1 - 2 * (y * y + z * z)
    yaw = np.arctan2(siny_cosp, cosy_cosp)

    return roll, pitch, yaw


def quaternion_from_euler(roll: float, pitch: float, yaw: float) -> Quaternion:
    """ Converts euler roll, pitch, yaw to quaternion

    Args:
        roll (float): roll
        pitch (float): pitch
        yaw (float): yaw

    Returns:
        Quaternion: output quaternion
    """
    cy = math.cos(yaw * 0.5)
    sy = math.sin(yaw * 0.5)
    cp = math.cos(pitch * 0.5)
    sp = math.sin(pitch * 0.5)
    cr = math.cos(roll * 0.5)
    sr = math.sin(roll * 0.5)

    q = Quaternion()

    q.x = cy * cp * sr - sy * sp * cr
    q.y = sy * cp * sr + cy * sp * cr
    q.z = sy * cp * cr - cy * sp * sr
    q.w = cy * cp * cr + sy * sp * sr

    return q