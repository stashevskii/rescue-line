import sensor
import time
import math
import pyb
import struct

i2c = pyb.I2C(4, pyb.I2C.SLAVE, addr=0x12)
i2c.deinit()
i2c = pyb.I2C(4, pyb.I2C.SLAVE, addr=0x12)
print("Waiting for Arduino...")

while True:
    text = "6"
    data = struct.pack("<%ds" % len(text), text)

    try:
        i2c.send(struct.pack("<h", len(data)), timeout=10000)
        i2c.send(
            struct.pack("<h", len(data)), timeout=10000
        )
        try:
            i2c.send(data, timeout=10000)
            print("Sent Data!")
        except OSError:
            pass
    except OSError:
        pass
