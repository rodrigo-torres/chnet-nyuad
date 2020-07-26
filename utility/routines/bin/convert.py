import numpy as np
from PyMca5.PyMcaIO import EdfFile
import sys
import os

#Some variable declarations
n_channels = 16384
line_counter = 0
line_data = []
map_data = []
pix_data = [0]*n_channels

def processLine(line, array):
    datum = int(line)
    array[(datum >> 14) & 0x3FFF] = datum & 0x3FFF

if len(sys.argv) < 2:
    print("[!] You didn't provide a filename. Exiting")
    exit()

file = open(sys.argv[1],'r')
#Check appropriate header
line = file.readline()
if line[0] != '{':
    print("[!] Faulty header. Exiting")
    exit()

#Parse through the header info
while line != "}\n":
    line = file.readline()
    if 'Row_length' in line:
        tokens = line.split(" = ")
        image_xdim = int(tokens[1])
    if 'Rows_total' in line:
        tokens = line.split(" = ")
        image_ydim = int(tokens[1])

header = file.readline()
line = file.readline()
prev_line = line

# Load the file and construct the arrays
while line != '':
    while line[0] !='P':
        processLine(line, pix_data)
        line = file.readline()
        if line == '' :
            break
    
    line_data.append(pix_data)
    pix_data = [0]*n_channels    

    if (int(header.split()[1]))/(image_xdim-1) == 1:
        map_data.append(line_data)
        line_data = []
        line_counter = line_counter + 1
        print("Progress loading: %2.1f %%" % ((line_counter*50) / image_ydim))
    
    header = line
    line = file.readline()
file.close()

os.mkdir("dmedf")

counter2 = 0
data = np.ndarray((image_xdim, n_channels), dtype=np.float32)
# OPTIONAL: a, b and c are the coefficients of your calibration (c expected to be 0.0)
#ddict = {"Mca a": a, "Mca b": b, "Mca c": c}
for i in range(image_ydim):
    for j in range(image_xdim):
       print("%d %d"  % (i, j))
       data[j, :] = map_data[i][j]
    edf = EdfFile.EdfFile("dmedf/%s_%05d.edf" % (sys.argv[1],i), access="wb")
    edf.WriteImage({},data)
#    edf.WriteImage(ddict, data)
    edf = None
    counter2 = counter2 + 1
    print("Progress converting: %2.1f %%" % (50 +  (counter2 * 50)/ image_ydim))
