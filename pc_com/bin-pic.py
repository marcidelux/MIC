import argh

from PIL import Image

import argh

def convert(source_filename: str, destination_filename: str = "output.jpg"):
    """
    Convert an image to a binary file of RGB pixel values.
    source_filename: The source image filename.
    destination_filename: The destination filename. Defaults to output.bin.
    """

    with Image.open(source_filename) as img:
        width, height = img.size

        # Open the destination file in binary write mode
        with open(destination_filename, 'wb') as dest_file:
            for y in reversed(range(height)):
                if y % 2 == 0 :
                    for x in range(width):
                        pixel = img.getpixel((x, y))
                        pixel_bytes = bytes([pixel[0], pixel[1], pixel[2]])
                        dest_file.write(pixel_bytes)
                else:
                    for x in reversed(range(width)):
                        pixel = img.getpixel((x, y))
                        pixel_bytes = bytes([pixel[0], pixel[1], pixel[2]])
                        dest_file.write(pixel_bytes)
    print(
        f"Conversion completed. Image written to {destination_filename} as bytes.")

def printb(source_filename: str):
    with open(source_filename, mode='rb') as f:
        content = f.read()
        for i, b in enumerate(content):
            print("i:", i, "\tb:", b)

def dump(filename: str = "output.bin"):
    """
    Dump the contents of a binary file as integers.
    filename: The filename of the binary file to dump. Defaults to output.bin.
    """
    with open(filename, 'rb') as file:
        byte = file.read(1)
        while byte:
            # Convert the byte to an integer for printing
            byte_value = ord(byte)
            print(f"{byte_value} ", end="")
            byte = file.read(1)


command_parser = argh.ArghParser()

command_parser.add_commands([convert, dump, printb])

if __name__ == "__main__":
    command_parser.dispatch()