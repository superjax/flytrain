import serial

ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=921600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)
print ser.portstr       # check which port was really used

print "sending data"
ser.write("o")      # write a string

print "done"
ser.close()             # close port
