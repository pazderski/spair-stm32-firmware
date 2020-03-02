import serial
import struct
import math
from collections import namedtuple
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import crc16

serialPort = serial.Serial(port = "COM8", baudrate=115200,
                           bytesize=8, timeout=0, stopbits=serial.STOPBITS_ONE)

valueData1 = 0  

fps = 30 

# Parameters
x_len = 200             # Number of points to display
y_range = [-150, 150]     # Range of possible Y values to display



# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = list(range(0, 200))
ys = [0] * x_len
ax.set_ylim(y_range)

# Create a blank line. We will update the line in animate
line, = ax.plot(xs, ys)

# Add labels
plt.title('Data over Time')
plt.xlabel('Samples')
plt.ylabel('Data')


readData = serialPort.flush()  
# This function is called periodically from FuncAnimation
def animate(i, ys):
    global valueData1
    
    time = i/fps
    readData = serialPort.read(128)  
    dataLen = len(readData)
    if dataLen == 10:
        dataBytesNumber = dataLen - 5
        data = readData[3:(3+dataBytesNumber)]
        crcComp = crc16.INITIAL_MODBUS
        crcComp = crc16.calcString(data, crcComp) 
        readFrame = struct.unpack('3sBbhH', readData)
        crcRead = readFrame[4]
        if crcRead == crcComp:
            valueData1 = readFrame[2]
        
                
    dataOut = struct.pack('Bbh', 1, int(20*math.sin(1.0*time)), 125)
    dataLen = len(dataOut)
    writeFrame = struct.pack("3sb%ss2s" %dataLen, b'\xAA\xAA\x00', dataLen, dataOut, b'\x00\x00');
    
    serialPort.write(writeFrame)
          
    # Add y to list
    ys.append(valueData1)

    # Limit y list to set number of items
    ys = ys[-x_len:]

    # Update line with new Y values
    line.set_ydata(ys)
    
    return line,

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig,
    animate,
    fargs=(ys,),
    interval=fps,
    blit=True)

plt.show()

   
