#ifndef VALVE_CONTROLLER_APP_H
#define VALVE_CONTROLLER_APP_H
#include <pico/stdlib.h>
#include <cstring>
#include <config.h>
#include <harp_message.h>
#include <harp_core.h>
#include <harp_c_app.h>
#include <valve_driver.h>
#ifdef DEBUG
    #include <stdio.h>
    #include <cstdio> // for printf
#endif

// Setup for Harp App
const size_t APP_REG_COUNT = 19;
const size_t VALVE_START_APP_ADDRESS = APP_REG_START_ADDRESS + 3;

extern RegSpecs app_reg_specs[APP_REG_COUNT];
extern RegFnPair reg_handler_fns[APP_REG_COUNT];
extern HarpCApp& app;

extern ValveDriver valve_drivers[NUM_VALVES];

// Valve configuration struct for configuring the Hit-and-hold driver
#pragma pack(push, 1)
struct ValveConfig
{
    float hit_output;
    float hold_output;
    uint16_t hit_duration_us;
};
#pragma pack(pop)

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
    ValveConfig ValveConfig0;
    ValveConfig ValveConfig1;
    ValveConfig ValveConfig2;
    ValveConfig ValveConfig3;
    ValveConfig ValveConfig4;
    ValveConfig ValveConfig5;
    ValveConfig ValveConfig6;
    ValveConfig ValveConfig7;
    ValveConfig ValveConfig8;
    ValveConfig ValveConfig9;
    ValveConfig ValveConfig10;
    ValveConfig ValveConfig11;
    ValveConfig ValveConfig12;
    ValveConfig ValveConfig13;
    ValveConfig ValveConfig14;
    ValveConfig ValveConfig15;
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
void read_any_valve_config(uint8_t reg_address);

void write_valves_clear(msg_t& msg);
void write_valves_set(msg_t& msg);
void write_valves_state(msg_t& msg);
void write_any_valve_config(msg_t& msg);

#endif // VALVE_CONTROLLER_APP_H
