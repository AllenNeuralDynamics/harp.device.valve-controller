#!/usr/bin/env python3
from pyharp.device import Device, DeviceMode
import serial.tools.list_ports

#import logging
#logging.basicConfig()
#logger = logging.getLogger()
#logger.setLevel(logging.DEBUG)

# Open serial connection with the first Valve Controller.
com_port = None
ports = serial.tools.list_ports.comports()
for port, desc, hwid in sorted(ports):
    if desc.startswith("valve-controller"):
        print("{}: {} [{}]".format(port, desc, hwid))
        com_port = port
        break
device = Device(com_port)
device.info()                           # Display device's info on screen

reg_dump = device.dump_registers()
for reg_reply in reg_dump:
    print(reg_reply)
    print()

# Close connection
device.disconnect()
