#!/usr/bin/python
import serial
import datetime
import sys

log_file = sys.argv[1]

ser = serial.Serial(
    port='/dev/ttyUSB0', \
    baudrate=115200, \
    parity=serial.PARITY_NONE, \
    stopbits=serial.STOPBITS_ONE, \
    bytesize=serial.EIGHTBITS, \
    timeout=0)
# print("connected to: " + ser.portstr)

with open(log_file, 'a', buffering=1) as f:
    f.write("****** Start logging experiment iteration at {0}. ******\n".format(datetime.datetime.now()))
    while True:
        line = ser.readline();
        #if line:
        #       print(line),i
        if line:
            f.write("{0} - {1}\n".format(datetime.datetime.now(), str(line)))
ser.close()
