#!/usr/bin/env python3
from pyharp.device import Device, DeviceMode
from pyharp.messages import HarpMessage
from pyharp.messages import MessageType
from app_registers import AppRegs
from struct import pack, unpack
import os
import serial.tools.list_ports

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

print("Enabling Valve0")
valves = 0x0001 # Turn on valve0
reply = device.send(HarpMessage.WriteU16(AppRegs.ValvesSet, valves).frame)
print("reply:")
print(reply)

# Close connection
device.disconnect()
