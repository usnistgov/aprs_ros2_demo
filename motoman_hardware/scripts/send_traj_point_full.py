import socket
import struct
import math
import time
from time import sleep, time as time_func

class MotoMotionReply:
    def __init__(self, vals: list):
        self.msg_type: int     = vals[0]
        self.comm_type: int    = vals[1]
        self.reply_type: int   = vals[2]
        self.robot_id: int     = vals[3]
        self.sequence: int     = vals[4]
        self.command: int      = vals[5]
        self.result: int       = vals[6]
        self.subcode: int      = vals[7]
        self.data: list[float] = vals[8:]

class ReadSocketReply:
    def __init__(self, vals: list):
        self.msg_type: int     = vals[0]
        self.comm_type: int    = vals[1]
        self.reply_type: int   = vals[2]
        self.value: int        = vals[3]
        self.reply_code        = vals[4]

class MotoMotionCtrl:
    _command_types = {
        "CHECK_MOTION_READY": 200101,
        "CHECK_QUEUE_CNT": 200102,
        "STOP_MOTION": 200111,
        "START_TRAJ_MODE": 200121,
        "STOP_TRAJ_MODE": 200122
    }

    def __init__(self, command: str):
        self.length = 64
        self.msg_type = 2001
        self.comm_type = 2
        self.reply_code = 0
        self.robot_id = 0
        self.seq = 0
        self.command = self._command_types[command.upper()]
        self.data = [0.0] * 10
    
    def to_bytes(self) -> bytes:
        return struct.pack('>I 3I 3I 10f',
            self.length,
            self.msg_type,
            self.comm_type,
            self.reply_code,
            self.robot_id,
            self.seq,
            self.command,
            *self.data
        )
    
    def send_msg_and_get_feedback(self, s: socket.socket, sleep_before_reply: int = 0) -> MotoMotionReply:
        s.send(self.to_bytes())

        sleep(sleep_before_reply)

        length = int(s.recv(4).hex(),16)
        
        response = struct.unpack('>3I 5I 10f', s.recv(length))
        
        return MotoMotionReply(list(response))
        
class JointTrajPtFull:
    def __init__(self, 
        seq: int,
        time: float,
        positions: list[float],
        velocities: list[float],
        accelerations: list[float]
    ):
        self.length = 136
        self.msg_type = 14
        self.comm_type = 2
        self.reply_code = 0
        self.robot_id = 0
        self.seq = seq
        self.valid_fields = 7
            
        self.time = time
        
        self.positions = positions + [0.0] * (10-len(positions))        
        self.velocities = velocities + [0.0] * (10-len(velocities))
        self.accelerations = accelerations + [0.0] * (10-len(accelerations))
    
    def to_bytes(self) -> bytes:
        return struct.pack('>I 3I 3I 1f 10f 10f 10f',
            self.length,
            self.msg_type,
            self.comm_type,
            self.reply_code,
            self.robot_id,
            self.seq,
            self.valid_fields,
            self.time,
            *self.positions,
            *self.velocities,
            *self.accelerations
        )

    def send_msg_and_get_feedback(self, s: socket.socket, sleep_before_reply: int = 0) -> MotoMotionReply:
        s.send(self.to_bytes())

        sleep(sleep_before_reply)

        length = int(s.recv(4).hex(),16)
        
        response = struct.unpack('>3I 5I 10f', s.recv(length))
        
        return MotoMotionReply(list(response))
    
class WriteIOBIT:
    def __init__(self, 
        io_address: int,
        io_value: int
    ):
        self.length = 20
        self.msg_type = 2005
        self.comm_type = 2
        self.reply_code = 0
        self.io_address = io_address
        self.io_value = io_value
    
    def to_bytes(self) -> bytes:
        return struct.pack('>I 3I 2I',
            self.length,
            self.msg_type,
            self.comm_type,
            self.reply_code,
            self.io_address,
            self.io_value
        )
    
    def send_msg_and_get_feedback(self, s: socket.socket, sleep_before_reply: int = 0) -> MotoMotionReply:
        s.send(self.to_bytes())

        sleep(sleep_before_reply)

        length = int(s.recv(4).hex(),16)
        
        print(length)
        response = struct.unpack('>3I 1I', s.recv(length))
        
        return MotoMotionReply(list(response))

class ReadIOBit:
    def __init__(self, 
        io_address: int,
    ):
        self.length = 16
        self.msg_type = 2003
        self.comm_type = 2
        self.reply_code = 0
        self.io_address = io_address
    
    def to_bytes(self) -> bytes:
        return struct.pack('>I 3I 1I',
            self.length,
            self.msg_type,
            self.comm_type,
            self.reply_code,
            self.io_address
        )
    
    def send_msg_and_get_feedback(self, s: socket.socket, sleep_before_reply: int = 0) -> ReadSocketReply:
        s.send(self.to_bytes())

        sleep(sleep_before_reply)

        length = int(s.recv(4).hex(),16)
        
        print(length)
        response = struct.unpack('>3I 2I', s.recv(length))
        
        return ReadSocketReply(list(response))

def open_gripper(s: socket.socket):
    print("Openning gripper")
    open_gripper_io = WriteIOBIT(10010, 1)
    print(f"Response: {open_gripper_io.send_msg_and_get_feedback(s)}")
    
    close_gripper_io = WriteIOBIT(10011, 0)
    print(f"Response: {close_gripper_io.send_msg_and_get_feedback(s)}")
    
    air_io = WriteIOBIT(10012, 1)
    print(f"Response: {air_io.send_msg_and_get_feedback(s)}")

def close_gripper(s: socket.socket):
    print("Closing gripper")
    open_gripper_io = WriteIOBIT(10010, 0)
    print(f"Response: {open_gripper_io.send_msg_and_get_feedback(s)}")
    
    close_gripper_io = WriteIOBIT(10011, 1)
    print(f"Response: {close_gripper_io.send_msg_and_get_feedback(s)}")
    
    air_io = WriteIOBIT(10012, 0)
    print(f"Response: {air_io.send_msg_and_get_feedback(s)}")

def main():
    ## Get current joint positions
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:

        s.connect(("192.168.1.33", 50241))
        
        received_joint_values = False
        received_status = False
        while not received_joint_values or not received_status:

            length = int(s.recv(4).hex(),16)
            
            byte_stream = s.recv(length)
            
            if length == 40: 
                received_status = True   
                status = struct.unpack('>3I 7I', byte_stream)
                
                print(f'Motion possible: {status[9]}')
            
            if length == 144:
                feedback = struct.unpack('>3I 2I f 10f 10f 10f', byte_stream)
                
                time = feedback[5]
                joint_positions = list(feedback[6:13])
                
                print(f'Time: {time}')
                
                # Correct sign
                # for i in range(len(joint_positions)):
                #     joint_positions *= -1
                # joint_positions[1] *= -1
                # joint_positions[4] *= -1
                # joint_positions[6] *= -1
                
                joint_names = ['S', 'L', 'E', 'U', 'R', 'B', 'T']
                
                for name, pos in zip(joint_names, joint_positions):
                    print(f'Joint {name}: {math.degrees(pos)}')
                
                received_joint_values = True
        
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:

        s.connect(("192.168.1.33", 50240))
        
        ## Send start trajectory msg
        start_traj_msg = MotoMotionCtrl("START_TRAJ_MODE")
        reply = start_traj_msg.send_msg_and_get_feedback(s)
        print(f"Message type: {reply.command}")
        print(f"Result: {reply.result}\nSubcode: {reply.subcode}\n")
        
        # ## Send start trajectory msg
        # queue_cnt_msg = MotoMotionCtrl("CHECK_QUEUE_CNT")
        # reply = queue_cnt_msg.send_msg_and_get_feedback(s)
        # print(f"Message type: {reply.command}")
        # print(f"Sequence: {reply.sequence}\nResult: {reply.result}\nSubcode: {reply.subcode}\n")
        
        # queue_cnt_msg = MotoMotionCtrl("CHECK_MOTION_READY")
        # reply = queue_cnt_msg.send_msg_and_get_feedback(s)
        # print(f"Message type: {reply.command}")
        # print(f"Result: {reply.sequence}\nSubcode: {reply.subcode}\n")
                
        sleep(1)
    
    # with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    #     sock.connect(("192.168.1.33", 50242))
    #     open_gripper(sock)
    #     check_if_gripper_closed = ReadIOBit(10010)
    #     response = check_if_gripper_closed.send_msg_and_get_feedback(sock)
    #     print(response[-2])
    #     check_if_gripper_closed = ReadIOBit(10011)
    #     response = check_if_gripper_closed.send_msg_and_get_feedback(sock)
    #     print(response[-2])
    #     sleep(10)
    #     close_gripper(sock)
    #     check_if_gripper_closed = ReadIOBit(10010)
    #     response = check_if_gripper_closed.send_msg_and_get_feedback(sock)
    #     print(response[-2])
    #     check_if_gripper_closed = ReadIOBit(10011)
    #     response = check_if_gripper_closed.send_msg_and_get_feedback(sock)
    #     print(response[-2])
        velocities = [0.0] * 7
        accelerations = [0.0] * 7

        print("\n".join([str(val) for val in joint_positions]))
        init_trajectory = JointTrajPtFull(0, 0.0, joint_positions, velocities, accelerations)
        reply = init_trajectory.send_msg_and_get_feedback(s)
        print(f"Message type: {reply.command}")
        print(f"Initial Trajectory pt sent\nResult: {reply.result}\nSubcode: {reply.subcode}\n")
        
        sleep(1)
        # joint_positions[0] = -1.5707
        # joint_positions[1] = 0.0
        # joint_positions[2] = 0.0
        # joint_positions[3] = 1.5707
        # joint_positions[4] = 0.0
        # joint_positions[5] = 1.5707
        # joint_positions[6] = 3.1415
        # joint_positions[6] -= 0.5
        # # velocities = [0.01] * 7
        # # accelerations = [0.01] * 7
        # motion_time = 7.0
                
        # pt_1 = JointTrajPtFull(1, motion_time, joint_positions, velocities, accelerations)
        # reply = pt_1.send_msg_and_get_feedback(s, 1)
        # print(f"Message type: {reply.command}")
        # print(f"Point 1 sent\nResult: {reply.result}\nSubcode: {reply.subcode}\n")
        
        # sleep(motion_time+1)

        while True:
            try:
                sleep(0.1)
            except KeyboardInterrupt:
                break
        
        # stop_motion = MotoMotionCtrl("STOP_MOTION")
        # reply = stop_motion.send_msg_and_get_feedback(s)
        # print(f"Message type: {reply.command}")
        # print(f"Result: {reply.result}\nSubcode: {reply.subcode}\n")
        
        # queue_cnt_msg = MotoMotionCtrl("CHECK_QUEUE_CNT")
        # reply = queue_cnt_msg.send_msg_and_get_feedback(s)
        # print(f"Message type: {reply.command}")
        # print(f"Sequence: {reply.sequence}\nResult: {reply.result}\nSubcode: {reply.subcode}\n")
    
    # with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        # s.connect(("192.168.1.33", 50240))
        stop_traj_msg = MotoMotionCtrl("STOP_TRAJ_MODE")
        reply = stop_traj_msg.send_msg_and_get_feedback(s)
        print(f"Message type: {reply.command}")
        print(f"Result: {reply.result}\nSubcode: {reply.subcode}\n")
        
        sleep(1)
    
    
if __name__ == "__main__":
    main()