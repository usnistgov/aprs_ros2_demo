#!/usr/bin/env python3

import socket
import re

from time import sleep

CONFIG = {
        "port":5001,
        "host_ip":"viz.campus.nist.gov",
        }

class AntVisionUtility():
    def __init__(self, config=CONFIG):
        self.config = config
        self.buffer = b''
    
    def get_object_locations(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((self.config['host_ip'], self.config['port']))

        while b'\n\r' not in self.buffer:
            data = s.recv(1024)
            self.buffer += data
        line,sep,self.buffer = self.buffer.partition(b'\n\r')
        line.decode()
        raw_string = line.decode('ascii')

        regexp = re.compile('([\w.-]*,){6}')

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


if __name__ == '__main__':
    avu = AntVisionUtility()
    parts = avu.get_object_locations()
    for p in parts:
        print(p)
