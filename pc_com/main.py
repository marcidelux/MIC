import sys
from PyQt6.QtGui import QGuiApplication
from PyQt6.QtQml import QQmlApplicationEngine
from PyQt6.QtCore import QObject, pyqtSlot
from urllib.parse import urlparse
from PIL import Image

import serial
import time

def load_binary_data(filepath):
    with open(filepath, 'rb') as file:
        return bytearray(file.read())

port = "/dev/ttyUSB0"
baudrate = 9600

def get_file_path(url):
    parsed = urlparse(url)
    return parsed.path

def convert(source_filename: str, destination_filename: str = "output.jpg"):
    with Image.open(source_filename) as img:
        width, height = img.size

        # Open the destination file in binary write mode
        with open(destination_filename, 'wb') as dest_file:
            for y in reversed(range(height)):
                if y % 2 == 0 :
                    for x in range(width):
                        pixel = img.getpixel((x, y))
                        pixel_bytes = bytes([pixel[1], pixel[0], pixel[2]])
                        dest_file.write(pixel_bytes)
                else:
                    for x in reversed(range(width)):
                        pixel = img.getpixel((x, y))
                        pixel_bytes = bytes([pixel[1], pixel[0], pixel[2]])
                        dest_file.write(pixel_bytes)
    print(
        f"Conversion completed. Image written to {destination_filename} as bytes.")

class Backend(QObject):
    def __init__(self):
        super().__init__()

    @pyqtSlot(str, str)
    def makeImage(self, source, destination):
        srcPath = get_file_path(source)
        convert(srcPath, destination)

    @pyqtSlot(str, str)
    def sendCommand(self, command, destination):
        print("command: ", command)
        print("destination: ", destination)
        if command == "WRITE":
            command_type = 0x01
            param_str = '/' + destination + '\0'
            parameter = bytearray(param_str, 'utf-8')
            file_path = get_file_path(destination)
            data = load_binary_data(file_path)
        elif command == "DIR":
            command_type = 0x04
            param_str = "/"
            parameter = bytearray()
            data = bytearray()
        elif command == "SHOW":
            command_type = 0x02
            param_str = '/' + destination + '\0'
            parameter = bytearray(param_str, 'utf-8')
            data = bytearray()

        param_len = len(parameter)
        data_len = len(data)
        ser = serial.Serial(port, baudrate, timeout=1)
        command = bytearray([command_type, param_len, data_len & 0xff, (data_len >> 8) & 0xff])
        command += parameter
        command += data
        print("paramstr: ", param_str)
        print(command)
        ser.write(command)
        ser.close()

def main():
    # Create an instance of the application
    app = QGuiApplication(sys.argv)

    # Create a QML engine
    engine = QQmlApplicationEngine()

    # Create an instance of the Backend class
    backend = Backend()

    # Expose the Backend object to QML
    engine.rootContext().setContextProperty("backend", backend)

    # Load the QML file
    engine.load('main.qml')

    # Check if the QML file has loaded correctly
    if not engine.rootObjects():
        return -1

    # Execute the application
    return app.exec()

if __name__ == '__main__':
    sys.exit(main())