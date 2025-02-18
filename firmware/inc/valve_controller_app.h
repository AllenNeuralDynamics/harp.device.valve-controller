#ifndef VALVE_CONTROLLER_APP_H
#define VALVE_CONTROLLER_APP_H
#include <pico/stdlib.h>
#include <cstring>
#include <config.h>
#include <harp_message.h>
#include <harp_core.h>
#include <harp_c_app.h>
#ifdef DEBUG
    #include <stdio.h>
    #include <cstdio> // for printf
#endif

// Setup for Harp App
const size_t reg_count = 3;

extern RegSpecs app_reg_specs[reg_count];
extern RegFnPair reg_handler_fns[reg_count];
extern HarpCApp& app;

// Registers
#pragma pack(push, 1)
struct app_regs_t
{
    uint16_t ValvesState; // Raw (energized/deenergized) state of all valves.
                          // Bitmask: one bit per valve.
                          // Write: 0 = deenergize. 1 = energize
                          // Read: 0 = deenergized. 1 = energized
    uint16_t ValvesSet; // Energize the valves specified in the bitmask.
                        // Bitmask: one bit per valve. (1 = energize)
                        // Read values are identical to ValveStates.
    uint16_t ValvesClear; // Deenergize the valve specified in the bitmask.
                          // Bitmask: one bit per valve. (1 = de-energize)
                          // Read values are the bitwise inverse of ValvesState
    // More app "registers" here.
};
#pragma pack(pop)

extern app_regs_t app_regs;

/**
 * \brief update the app state. Called in a loop.
 */
void update_app_state();

/**
 * \brief reset the app.
 */
void reset_app();

void read_valves_clear(uint8_t reg_address);
void read_valves_set(uint8_t reg_address);
void read_valves_state(uint8_t reg_address);

void write_valves_clear(msg_t& msg);
void write_valves_set(msg_t& msg);
void write_valves_state(msg_t& msg);

#endif // VALVE_CONTROLLER_APP_H
