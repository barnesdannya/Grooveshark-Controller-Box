import serial
import time
ser = serial.Serial()
ser.baudrate = 9600
ser.port = 2
ser.open()
while (True):
    messageSignal = ser.read()
    print(messageSignal)
    if(messageSignal == B'D'):
        distance = int(ser.readline())
        volumeLevel = distance/1.8
        #code to make volume system call
        if(volumeLevel == 0):
            ledCount = str(0)
        else:
            ledCount = str(int(volumeLevel/25)+1)
        ser.write(ledCount.encode('ascii'))
    else:
        if(messageSignal == B'P'):
            print("change play/pause state")
ser.close()
