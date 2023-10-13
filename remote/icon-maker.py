from math import ceil
from PIL import Image, ImageOps
import bitarray
import os
import glob

# Read image
def readImage(path):
    image = Image.open(path)
    image = ImageOps.pad(image, (ceil(image.width/ 8) * 8, image.height))
    return image.convert("L")

# Convert image to bit list
def toBitList(image):
    lst = list(image.getdata())
    return [int(bool(i)) for i in lst]

def toBytes(blist):
    str = b''
    for i in range(0, len(blist), 8):
        str += bitarray.bitarray(blist[i:i+8]).tobytes()
    return str

def toSnakeCase(name):
    return '_'.join(name.split('-'))

def imageTobByteString(imagePath):
    img = readImage(imagePath)
    blist = toBitList(img)
    bytes = toBytes(blist)
    str = ''.join(['\\x{:02x}'.format(b) for b in bytes])
    str = str.replace('\\x00', '\\x80')
    name = os.path.basename(imagePath).split('.')[0]
    name = toSnakeCase(name)

    width = '\\x{:02x}'.format(img.width)
    height = '\\x{:02x}'.format(img.height)

    return '\tconst PROGMEM uint8_t {0}[] = "{1}{2}{3}";\n'.format(name, width, height, str).encode()

# Main
def main():
    iconsHeader = open('./icons.h', 'wb')
    icons = glob.glob('./resources/*.bmp')

    iconsHeader.write(b'#pragma once\nnamespace cardian::remote::icons {\n')
    for icon in icons:
        iconsHeader.write(imageTobByteString(icon))
    iconsHeader.write(b'}')

if __name__ == "__main__":
    main()