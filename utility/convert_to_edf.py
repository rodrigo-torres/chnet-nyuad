import numpy as np
from PyMca5.PyMcaIO import EdfFile
import sys

def processLine(line, array):
    datum = int(line)
    array[(datum >> 14) & 0x3FFF] = datum & 0x3FFF

file = open(sys.argv[1],'r')
version = file.readline()

n_channels = 16384
image_xdim = 23
image_ydim = 46
n_rows = image_ydim
n_columns = image_xdim
line_counter = 0
line_data = []
map_data = []
#pix_data = np.zeros(n_channels, dtype=np.uint32)
pix_data = [0]*n_channels

header = file.readline()
line = file.readline()
prev_line = line

while line != '':
    while line[0] !='P':
        processLine(line, pix_data)
        line = file.readline()
        if line == '' :
            break
    
    line_data.append(pix_data)
    #pix_data = np.zeros(n_channels, dtype=np.uint32)
    pix_data = [0]*n_channels    

    if (int(header.split()[1]))/(image_xdim-1) == 1:
        map_data.append(line_data)
        line_data = []
        line_counter = line_counter + 1
        print("Progress loading: %2.1f %%" % ((line_counter*50) / image_ydim))
    
    header = line
    line = file.readline()

file.close()
#map_data = np.array(map_data)
counter2 = 0
data = np.ndarray((n_columns, n_channels), dtype=np.float32)
# OPTIONAL: a, b and c are the coefficients of your calibration (c expected to be 0.0)
#ddict = {"Mca a": a, "Mca b": b, "Mca c": c}
for i in range(n_rows):
    for j in range(n_columns):
       data[j, :] = map_data[i][j]
    edf = EdfFile.EdfFile("dmedf/%s_%05d.edf" % (sys.argv[1],i), access="wb")
    edf.WriteImage({},data)
#    edf.WriteImage(ddict, data)
    edf = None
    counter2 = counter2 + 1
    print("Progress converting: %2.1f %%" % (50 +  (counter2 * 50)/ n_rows))
