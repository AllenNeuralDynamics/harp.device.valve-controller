"""ValveController app registers. Later these will be extracted from the device.yaml"""
from enum import IntEnum



class AppRegs(IntEnum):
    ValvesState = 32
    ValvesSet = 33
    ValvesClear = 34
    ValveConfigs0 = 35
    ValveConfigs1 = 36
    ValveConfigs2 = 37
    ValveConfigs3 = 38
    ValveConfigs4 = 39
    ValveConfigs5 = 40
    ValveConfigs6 = 41
    ValveConfigs7 = 42
    ValveConfigs8 = 43
    ValveConfigs9 = 44
    ValveConfigs10 = 45
    ValveConfigs11 = 46
    ValveConfigs12 = 47
    ValveConfigs13 = 48
    ValveConfigs14 = 49
    ValveConfigs15 = 50
