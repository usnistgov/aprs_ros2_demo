import os
import math 
import numpy as np

from typing import Optional

import PyKDL
import pyassimp

from moveit_msgs.msg import CollisionObject, AttachedCollisionObject, ObjectColor

from geometry_msgs.msg import Pose, Point, Quaternion, PoseStamped, Transform, TransformStamped

from shape_msgs.msg import Mesh, MeshTriangle

from aprs_interfaces.msg import Tray, SlotInfo

from ament_index_python import get_package_share_directory


class PlanningSceneObjectInfo:
    TABLE = 20
    CONVEYOR = 21

    def __init__(self, name: str, identifier: int, pose: Pose):
        self.name_ = name
        self.pose_ = pose

        share_dir = get_package_share_directory('robot_commander')

        identifier_names = {
            SlotInfo.SMALL: 'small_gear',
            SlotInfo.MEDIUM: 'medium_gear',
            SlotInfo.LARGE: 'large_gear',
            Tray.SMALL_GEAR_TRAY: 'small_gear_part_tray',
            Tray.MEDIUM_GEAR_TRAY: 'medium_gear_part_tray',
            Tray.LARGE_GEAR_TRAY: 'large_gear_part_tray',
            Tray.S2L2_KIT_TRAY: 's2l2_kit_tray',
            Tray.M2L1_KIT_TRAY: 'm2l1_kit_tray',
            PlanningSceneObjectInfo.TABLE: 'optical_table',
            PlanningSceneObjectInfo.CONVEYOR: 'conveyor'
        }

        self.color_ = ObjectColor()
        self.color_.id = name
        self.color_.color.a = 1.0

        if 'tray' in identifier_names[identifier]:
            self.color_.color.r = 0.1
            self.color_.color.g = 0.1
            self.color_.color.b = 0.1
        elif 'gear' in identifier_names[identifier]:
            self.color_.color.r = 0.3
            self.color_.color.g = 0.7
            self.color_.color.b = 0.3
        else:
            self.color_.color.r = 0.8
            self.color_.color.g = 0.8
            self.color_.color.b = 0.8

        self.stl_path_ = os.path.join(share_dir, 'meshes', f'{identifier_names[identifier]}.stl')
    
    @property
    def name(self):
        return self.name_

    @property
    def pose(self):
        return self.pose_
    
    @property
    def stl_path(self):
        return self.stl_path_
    
    @property
    def color(self):
        return self.color_
    

def create_mesh_from_stl(stl_path: str) -> Optional[Mesh]:
    mesh = Mesh()

    with pyassimp.load(stl_path) as object:
        if not object.meshes: #type: ignore
            return None
        
        for face in object.meshes[0].faces: #type: ignore
            triangle = MeshTriangle()
            
            triangle.vertex_indices = [face[0], face[1], face[2]]
            
            mesh.triangles.append(triangle) #type: ignore
        
        for vertex in object.meshes[0].vertices: #type: ignore
            point = Point()
            point.x = float(vertex[0])
            point.y = float(vertex[1])
            point.z = float(vertex[2])
            
            mesh.vertices.append(point) #type: ignore

    return mesh


def build_collision_object(info: PlanningSceneObjectInfo) -> CollisionObject:
    obj = CollisionObject()
    
    obj.header.frame_id = 'world'
    obj.id = info.name
    obj.operation = CollisionObject.ADD
    obj.meshes = [create_mesh_from_stl(info.stl_path)]
    obj.mesh_poses = [info.pose]

    return obj


def multiply_pose(p1: Pose, p2: Pose) -> Pose:
    '''
    Use KDL to multiply two poses together.
    Args:
        p1 (Pose): Pose of the first frame
        p2 (Pose): Pose of the second frame
    Returns:
        Pose: Pose of the resulting frame
    '''

    o1 = p1.orientation
    frame1 = PyKDL.Frame(
        PyKDL.Rotation.Quaternion(o1.x, o1.y, o1.z, o1.w),
        PyKDL.Vector(p1.position.x, p1.position.y, p1.position.z))

    o2 = p2.orientation
    frame2 = PyKDL.Frame(
        PyKDL.Rotation.Quaternion(o2.x, o2.y, o2.z, o2.w),
        PyKDL.Vector(p2.position.x, p2.position.y, p2.position.z))

    frame3 = frame1 * frame2

    # return the resulting pose from frame3
    pose = Pose()
    pose.position.x = frame3.p.x()
    pose.position.y = frame3.p.y()
    pose.position.z = frame3.p.z()

    q = frame3.M.GetQuaternion()
    pose.orientation.x = q[0]
    pose.orientation.y = q[1]
    pose.orientation.z = q[2]
    pose.orientation.w = q[3]

    return pose

def quaternion_from_euler(roll: float, pitch: float, yaw: float) -> Quaternion:
    cy = math.cos(yaw * 0.5)
    sy = math.sin(yaw * 0.5)
    cp = math.cos(pitch * 0.5)
    sp = math.sin(pitch * 0.5)
    cr = math.cos(roll * 0.5)
    sr = math.sin(roll * 0.5)

    q = Quaternion()
    q.w = cy * cp * cr + sy * sp * sr
    q.x = cy * cp * sr - sy * sp * cr
    q.y = sy * cp * sr + cy * sp * cr
    q.z = sy * cp * cr - cy * sp * sr
    
    return q 

def euler_from_quaternion(quaternion: Quaternion) -> tuple[float, float, float]:
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

def convert_transform_to_pose(t: Transform) -> Pose:
    pose = Pose()
    pose.position.x = t.translation.x
    pose.position.y = t.translation.y
    pose.position.z = t.translation.z
    
    pose.orientation = t.rotation

    return pose

def convert_transform_stamped_to_pose_stamped(t: TransformStamped) -> PoseStamped:
    pose = PoseStamped()

    pose.header = t.header

    pose.pose.position.x = t.transform.translation.x
    pose.pose.position.y = t.transform.translation.y
    pose.pose.position.z = t.transform.translation.z
    
    pose.pose.orientation = t.transform.rotation

    return pose

def build_robot_pose(pose: Pose) -> Pose:

    robot_pose = Pose()

    robot_pose.position = pose.position
  
    roll = 0.0
    pitch = 3.14159

    # Create a rotation object from Euler angles specifying axes of rotation
    quat = quaternion_from_euler(roll, pitch, 0.0)

    robot_pose.orientation.x = quat.x
    robot_pose.orientation.y = quat.y
    robot_pose.orientation.z = quat.z
    robot_pose.orientation.w = quat.w

    return robot_pose
