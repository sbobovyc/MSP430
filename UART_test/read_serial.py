import serial
import struct
import binascii

ser = serial.Serial(port="COM3", baudrate=9600, bytesize=serial.EIGHTBITS,
                    parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE)

try:
    while 1:
        while ser.inWaiting() < 4:
            pass
        data = ser.read(4)
        value, = struct.unpack("f", data)
        print binascii.hexlify(data)
        print value
        
except KeyboardInterrupt:
    ser.close()
    print "Save data here"
except:    
    ser.close()
    print "Another error"
    
    
