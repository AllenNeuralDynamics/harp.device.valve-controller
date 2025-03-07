#!/usr/bin/env python3
from pyharp.device import Device, DeviceMode
from pyharp.messages import HarpMessage
from pyharp.messages import MessageType
from app_registers import AppRegs
from struct import pack, unpack
from time import sleep
import os
import serial.tools.list_ports

import logging
logger = logging.getLogger()
logger.addHandler(logging.StreamHandler())

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

print()
print("Enabling all aux gpios as inputs.")
gpio_dir = 0b00000000
reply = device.send(HarpMessage.WriteU8(AppRegs.AuxGPIODir, gpio_dir).frame)
print(f"reply: {reply.payload[0]:08b}")
print()
gpio_rise_pins = 0b00000001
gpio_fall_pins = 0b00000001
print(f"Enabling aux gpio: {gpio_rise_pins:08b} to send EVENTS on a rising edge.")
print(f"Enabling aux gpio: {gpio_fall_pins:08b} to send EVENTS on a falling edge.")
reply = device.send(HarpMessage.WriteU8(AppRegs.AuxGPIOInputRiseEvent, gpio_rise_pins).frame)
reply = device.send(HarpMessage.WriteU8(AppRegs.AuxGPIOInputFallEvent, gpio_fall_pins).frame)

try:
    while True:
        for msg in device.get_events():
            print(msg)
            print()
except KeyboardInterrupt:
    print("Disabling output event messages on input changes.")
    reply = device.send(HarpMessage.WriteU8(AppRegs.AuxGPIOInputRiseEvent, 0).frame)
    reply = device.send(HarpMessage.WriteU8(AppRegs.AuxGPIOInputFallEvent, 0).frame)
    device.disconnect()
