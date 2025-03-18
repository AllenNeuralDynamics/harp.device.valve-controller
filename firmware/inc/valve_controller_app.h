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
inline constexpr size_t APP_REG_COUNT = 27;
// Numeric addresses for Harp Registers (clunky).
inline constexpr size_t VALVE_START_APP_ADDRESS = APP_REG_START_ADDRESS + 3;
inline constexpr size_t LAST_VALVE_APP_ADDRESS = VALVE_START_APP_ADDRESS + NUM_VALVES - 1;
inline constexpr size_t AUX_GPIO_INPUT_RISE_EVENT_ADDRESS = LAST_VALVE_APP_ADDRESS + 5;
inline constexpr size_t AUX_GPIO_RISING_INPUTS_ADDRESS = AUX_GPIO_INPUT_RISE_EVENT_ADDRESS + 2;
inline constexpr size_t AUX_GPIO_FALLING_INPUTS_ADDRESS = AUX_GPIO_INPUT_RISE_EVENT_ADDRESS + 3;


// Debounce parameters for Aux GPIOS specified as inputs
inline constexpr uint32_t DEBOUNCE_INTERVAL_US = 1000;
inline constexpr uint32_t MIN_DEBOUNCE_SCORE = 0;
inline constexpr uint32_t MAX_DEBOUNCE_SCORE = 10;
inline constexpr uint32_t DEBOUNCE_ON_THRESHOLD = 2 * MAX_DEBOUNCE_SCORE / 3;
inline constexpr uint32_t DEBOUNCE_OFF_THRESHOLD = MAX_DEBOUNCE_SCORE / 3;

extern RegSpecs app_reg_specs[APP_REG_COUNT];
extern RegFnPair reg_handler_fns[APP_REG_COUNT];
extern HarpCApp& app;

extern ValveDriver valve_drivers[NUM_VALVES];

extern uint8_t old_aux_gpio_inputs;

// Valve configuration struct for configuring the Hit-and-hold driver
#pragma pack(push, 1)
struct ValveConfig
{
    float hit_output;
    float hold_output;
    uint32_t hit_duration_us;
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
    /// @ref ValveConfigs are represented as 16 individual registers instead
    /// of a register with an array of 16 ValveConfigs.
    ValveConfig ValveConfigs[NUM_VALVES]; // Represents app regs: 35, 36, ... 50
                                          // 16 Heterogeneous registers each
                                          // representing a ValveConfig packed
                                          // struct.
    uint8_t AuxGPIODir;
    uint8_t AuxGPIOState;
    uint8_t AuxGPIOSet;
    uint8_t AuxGPIOClear;

    uint8_t AuxGPIOInputRiseEvent;
    uint8_t AuxGPIOInputFallEvent;
    uint8_t AuxGPIORisingInputs; // Raw state of which inputs rose (could be multiple)
    uint8_t AuxGPIOFallingInputs; // Raw state of which inputs fell (could be multiple)
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

inline uint8_t read_aux_gpios()
{return uint8_t((gpio_get_all() >> GPIO_PIN_BASE) & GPIOS_MASK);}

void read_valves_state(uint8_t reg_address);
void read_valves_set(uint8_t reg_address);
void read_valves_clear(uint8_t reg_address);
void read_any_valve_config(uint8_t reg_address);
void read_aux_gpio_state(uint8_t reg_address);

void write_valves_state(msg_t& msg);
void write_valves_set(msg_t& msg);
void write_valves_clear(msg_t& msg);
void write_any_valve_config(msg_t& msg);
void write_aux_gpio_dir(msg_t& msg);
void write_aux_gpio_state(msg_t& msg);
void write_aux_gpio_set(msg_t& msg);
void write_aux_gpio_clear(msg_t& msg);

#endif // VALVE_CONTROLLER_APP_H
