#!/usr/bin/python
import serial
import datetime
import sys

log_file = sys.argv[1]

ser = serial.Serial(
    port='/dev/ttyUSB1', \
    baudrate=115200, \
    parity=serial.PARITY_NONE, \
    stopbits=serial.STOPBITS_ONE, \
    bytesize=serial.EIGHTBITS, \
    timeout=None)
# put timeout to None, so it blocks until readline reads a full line
# if you put it at 0, it sometimes read only half a line? Does not happen when putting it to None.

# print("connected to: " + ser.portstr)

#prevline = 'test\n'

with open(log_file, 'a', buffering=1) as f:
    f.write("****** Start logging experiment iteration at {0}. ******\n".format(datetime.datetime.now()))
    while True:
        line = ser.readline()
        #line = ser.read_until(b'\r')
        #if line:
        #       print(line),i
        #if line:
            #line = line.decode('ascii')
            #if '\n' in prevline:
        f.write("{0} - {1}\n".format(datetime.datetime.now(), line))
            #else:
            #f.write("{0}".format(line))
            #prevline = line
ser.close()
