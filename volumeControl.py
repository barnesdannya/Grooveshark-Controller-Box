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
        distance = ser.readline()
        print(distance)
        ser.write('2'.encode('ascii'))
    else:
        if(messageSignal == B'P'):
            print("change play/pause state")
ser.close()
