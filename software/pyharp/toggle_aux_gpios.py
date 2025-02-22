#!/usr/bin/env python3
from pyharp.device import Device, DeviceMode
from pyharp.messages import HarpMessage
from pyharp.messages import MessageType
from app_registers import AppRegs
from struct import pack, unpack
from time import sleep
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

print("Enabling AUX GPIOS: 0, 3, 4, 5, 6 as outputs")
gpio_dir = 0b01111001
reply = device.send(HarpMessage.WriteU8(AppRegs.AuxGPIODir, gpio_dir).frame)
print(f"reply: {reply.payload[0]:08b}")
print()
sleep(1.0)

GPIO_STATES = \
[
    (AppRegs.AuxGPIOSet, 0b00001001, "Setting GPIO0,3"),
    (AppRegs.AuxGPIOClear, 0b00001001, "Clearing GPIO0,3"),
    (AppRegs.AuxGPIOState, 0b00001001, "Setting GPIO0, 3 by writing to State"),
    (AppRegs.AuxGPIOState, 0b00000000, "Clearing GPIO0, 3 by writing to State"),
    (AppRegs.AuxGPIOState, 0b00010000, "Setting GPIO5 by writing to State"),
    (AppRegs.AuxGPIOState, 0b00100000, "Setting GPIO6 and Clearing 5 by writing to State"),
    (AppRegs.AuxGPIOState, 0b01000000, "Setting GPIO7 and Clearing 6 by writing to State"),
    (AppRegs.AuxGPIOState, 0b00000000, "Clearing all GPIOs"),
]

for reg, value , msg in GPIO_STATES:
    print(msg)
    reply = device.send(HarpMessage.WriteU8(reg, value).frame)
    print(f"reply: {reply.payload[0]:08b}")
    print()
    sleep(1.0)

# Close connection
device.disconnect()
