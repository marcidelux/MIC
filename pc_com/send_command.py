import serial
import time

def load_binary_data(filepath):
    with open(filepath, 'rb') as file:
        return bytearray(file.read())

port = "/dev/ttyUSB0"
baudrate = 9600


"""
command_type = 0x01
param_str = "/test.bin\0"
parameter = bytearray(param_str, 'utf-8')
file_path = "/home/napoleopard/Projects/Embedded/LedCube/pc_com/test.bin"  # Replace with your file path
data = load_binary_data(file_path)
"""

"""
"""
command_type = 0x02
param_str = "/test.bin\0"
parameter = bytearray(param_str, 'utf-8')
data = bytearray()


param_len = len(parameter)
data_len = len(data)

print("datalen: ", data_len)
print("paramlen: ", param_len)

ser = serial.Serial(port, baudrate, timeout=1)
command = bytearray([command_type, param_len, data_len & 0xff, (data_len >> 8) & 0xff])
command += parameter
command += data

print(command)

ser.write(command)

ser.close()
