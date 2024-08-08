#!/usr/bin/env python3

import socket
import re

CONFIG_FANUC = {
    "port":5001,
    # "host_ip":"viz.campus.nist.gov",
    "host_ip": "192.168.1.21"
}

CONFIG_TEACH = {
    "port":6001,
    # "host_ip":"viz.campus.nist.gov",
    "host_ip": "192.168.1.21"
}

CONFIG_MOTOMAN = {
    "port":5002,
    # "host_ip":"viz.campus.nist.gov",
    "host_ip": "192.168.1.21"
}

class VisionUtility():
    def __init__(self, config):
        if config == "CONFIG_FANUC":
            self.config = CONFIG_FANUC
        elif config == "CONFIG_TEACH":
            self.config = CONFIG_TEACH
        elif config == "CONFIG_MOTOMAN":
            self.config = CONFIG_MOTOMAN
        self.buffer = b''
    
    def get_object_locations(self) -> list[tuple[str, float, float, float, str]]:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((self.config['host_ip'], self.config['port']))

        while b'\n\r' not in self.buffer:
            data = s.recv(1024)
            self.buffer += data
        line,sep,self.buffer = self.buffer.partition(b'\n\r')
        line.decode()
        raw_string = line.decode('ascii')

        regexp = re.compile('([\\w.-]*,){6}')

        tmp_string = raw_string
        part_locations = []
        while result := regexp.match(tmp_string):
            entry = result.group().split(',')[0:6]
            if all(entry):
                name = entry[0]
                rotation = float(entry[1])
                x = float(entry[2]) * 0.001
                y = float(entry[3]) * 0.001
                type = entry[-1]
                part_locations.append((name, rotation, x, y, type))
            tmp_string = tmp_string.replace(result.group(), '')
            
        s.close()

        return part_locations