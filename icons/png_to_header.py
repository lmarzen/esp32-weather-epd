import getopt
import os.path
import sys
from PIL import Image

BITES_PER_LINE = 12
BITS_PER_BITE = 8

try:
    opts, args = getopt.getopt(sys.argv[1:],"hi:o:",["inputfile=","outputfile="])
except getopt.GetoptError:
    print('png_to_header.py -i <inputfile> -o <outputfile>')
    sys.exit(2)
for opt, arg in opts:
    if opt == '-h':
        print('png_to_header.py -i <inputfile> -o <outputfile>')
        sys.exit()
    elif opt in ("-i", "--inputfile"):
        inputfile = arg
    elif opt in ("-o", "--outputfile"):
        outputfile = arg

try: inputfile
except NameError:
    print("Error: inputfile is a required parameter. See usage -h.")
    exit()
try: outputfile
except NameError:
    print("Error: outputfile is a required parameter. See usage -h")
    exit()
   
src_image = Image.open(inputfile)
# Converts the image to black and white
src_bw = src_image.convert('1')
# Creates a list of the pixel values (0/1)
pixels = list(src_bw.getdata())

f = open(outputfile, "w")
var = os.path.basename(outputfile)
var = var.rsplit('.h',1)[0]

width, height = src_image.size
f.write("// " + str(width) + " x " + str(height) + "\n")
f.write("const unsigned char " + var + "[] PROGMEM = {\n ")


bit_cnt = 1
line_width = 1
tmp_bite = 0
n = len(pixels)

for i in range(n):
    if (pixels[i] != 0):
        tmp_bite |= 1
    if (bit_cnt < BITS_PER_BITE and i != n - 1):
        tmp_bite <<= 1
        bit_cnt += 1
    elif (i != n - 1):
        f.write(" " + "0x{:02x}".format(tmp_bite) + ",")
        if (line_width == BITES_PER_LINE):
            f.write("\n ")
            line_width = 1
        else:
            line_width += 1
        tmp_bite = 0
        bit_cnt = 1
        
f.write(" " + "0x{:02x}".format(tmp_bite) + "\n};")
f.close()