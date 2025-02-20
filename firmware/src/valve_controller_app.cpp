#include <valve_controller_app.h>

app_regs_t app_regs;


/// Create Hit-and-Hold Valve Drivers.
/// The underlying PWM peripheral, aka: a PWM Slice, controls two adjacent PWM
/// pins and must be configured with the same settings. This is OK since we are
/// enforcing the same underlying peripheral settings (i.e: frequency) across
/// all Slices.
ValveDriver valve_drivers[NUM_VALVES]
{{VALVE_PIN_BASE},
 {VALVE_PIN_BASE + 1},
 {VALVE_PIN_BASE + 2},
 {VALVE_PIN_BASE + 3},
 {VALVE_PIN_BASE + 4},
 {VALVE_PIN_BASE + 5},
 {VALVE_PIN_BASE + 6},
 {VALVE_PIN_BASE + 7},
 {VALVE_PIN_BASE + 8},
 {VALVE_PIN_BASE + 9},
 {VALVE_PIN_BASE + 10},
 {VALVE_PIN_BASE + 11},
 {VALVE_PIN_BASE + 12},
 {VALVE_PIN_BASE + 13},
 {VALVE_PIN_BASE + 14},
 {VALVE_PIN_BASE + 15}};

// Define "specs" per-register
RegSpecs app_reg_specs[APP_REG_COUNT]
{
    {(uint8_t*)&app_regs.ValvesState, sizeof(app_regs.ValvesState), U16},
    {(uint8_t*)&app_regs.ValvesSet, sizeof(app_regs.ValvesSet), U16},
    {(uint8_t*)&app_regs.ValvesClear, sizeof(app_regs.ValvesClear), U16},
    {(uint8_t*)&app_regs.ValveConfigs[0], sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfigs[1], sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfigs[2], sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfigs[3], sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfigs[4], sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfigs[5], sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfigs[6], sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfigs[7], sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfigs[8], sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfigs[9], sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfigs[10], sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfigs[11], sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfigs[12], sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfigs[13], sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfigs[14], sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfigs[15], sizeof(ValveConfig), U8},
    // More specs here if we add additional registers.
};

RegFnPair reg_handler_fns[APP_REG_COUNT]
{
    {read_valves_state, write_valves_state},
    {read_valves_set, write_valves_set},
    {read_valves_clear, write_valves_clear},
    {read_any_valve_config, write_any_valve_config}, // valve 0
    {read_any_valve_config, write_any_valve_config}, // valve 1
    {read_any_valve_config, write_any_valve_config}, // ...
    {read_any_valve_config, write_any_valve_config},
    {read_any_valve_config, write_any_valve_config},
    {read_any_valve_config, write_any_valve_config},
    {read_any_valve_config, write_any_valve_config},
    {read_any_valve_config, write_any_valve_config},
    {read_any_valve_config, write_any_valve_config},
    {read_any_valve_config, write_any_valve_config},
    {read_any_valve_config, write_any_valve_config},
    {read_any_valve_config, write_any_valve_config},
    {read_any_valve_config, write_any_valve_config},
    {read_any_valve_config, write_any_valve_config},
    {read_any_valve_config, write_any_valve_config},
    {read_any_valve_config, write_any_valve_config}, // valve 15
};


void read_valves_state(uint8_t reg_address)
{
    for (size_t valve_index = 0; valve_index < NUM_VALVES; ++valve_index)
    {
        app_regs.ValvesState = 0;
        if (valve_drivers[valve_index].is_energized())
            app_regs.ValvesState |= (typeof(app_regs.ValvesState))(1) << valve_index;
    }
    if (!HarpCore::is_muted())
        HarpCore::send_harp_reply(READ, reg_address);
}

void write_valves_state(msg_t& msg)
{
    HarpCore::copy_msg_payload_to_register(msg);
    for (size_t valve_index = 0; valve_index < NUM_VALVES; ++valve_index)
    {
        if ((app_regs.ValvesState >> valve_index) & (typeof(app_regs.ValvesState))(1))
            valve_drivers[valve_index].energize();
        else
            valve_drivers[valve_index].deenergize();
    }
    // Reply with the actual value that we wrote.
    if (!HarpCore::is_muted())
        HarpCore::send_harp_reply(WRITE, msg.header.address);
}

void read_valves_set(uint8_t reg_address)
{
    // Return the most recently set value.
    if (!HarpCore::is_muted())
        HarpCore::send_harp_reply(READ, reg_address);
}

void write_valves_set(msg_t& msg)
{
    HarpCore::copy_msg_payload_to_register(msg);
    for (size_t valve_index = 0; valve_index < NUM_VALVES; ++valve_index)
    {
        if ((app_regs.ValvesSet >> valve_index) & (typeof(app_regs.ValvesSet))(1))
            valve_drivers[valve_index].energize();
    }
    if (!HarpCore::is_muted())
        HarpCore::send_harp_reply(WRITE, msg.header.address);
}

void read_valves_clear(uint8_t reg_address)
{
    // Return the most recently cleared value.
    if (!HarpCore::is_muted())
        HarpCore::send_harp_reply(READ, reg_address);
}

void write_valves_clear(msg_t& msg)
{
    HarpCore::copy_msg_payload_to_register(msg);
    for (size_t valve_index = 0; valve_index < NUM_VALVES; ++valve_index)
    {
        if ((app_regs.ValvesClear >> valve_index) & (typeof(app_regs.ValvesClear))(1))
            valve_drivers[valve_index].deenergize();
    }
    // Reply with the actual value that we wrote.
    if (!HarpCore::is_muted())
        HarpCore::send_harp_reply(WRITE, msg.header.address);
}


void read_any_valve_config(uint8_t reg_address)
{
    uint8_t valve_index = reg_address - VALVE_START_APP_ADDRESS;
    ValveConfig& valve_cfg = app_regs.ValveConfigs[valve_index];
    const ValveDriver& valve_driver = valve_drivers[valve_index];
    // Update Harp App registers with ValveDriver class contents.
    valve_cfg.hit_output = valve_driver.get_hit_output();
    valve_cfg.hold_output = valve_driver.get_hold_output();
    valve_cfg.hit_duration_us = valve_driver.get_hit_duration_us();
    if (!HarpCore::is_muted())
        HarpCore::send_harp_reply(READ, reg_address);
}


void write_any_valve_config(msg_t& msg)
{
    HarpCore::copy_msg_payload_to_register(msg);
    uint8_t valve_index = msg.header.address - VALVE_START_APP_ADDRESS;
    const ValveConfig& valve_cfg = app_regs.ValveConfigs[valve_index];
    ValveDriver& valve_driver = valve_drivers[valve_index];
    // Apply the configuration.
    valve_driver.set_hit_duration_us(valve_cfg.hit_duration_us);
    valve_driver.set_normalized_hit_output(valve_cfg.hit_output);
    valve_driver.set_normalized_hold_output(valve_cfg.hold_output);
    if (!HarpCore::is_muted())
        HarpCore::send_harp_reply(WRITE, msg.header.address);
}

void update_app_state()
{
    for (auto& valve_driver: valve_drivers)
        valve_driver.update_fsm();
}

void reset_app()
{
    // init all GPIO pins we are using.
    // FIXME: Valve drivers should handle underlying GPIO pins
    gpio_init_mask((VALVES_MASK << VALVE_PIN_BASE) | (GPIOS_MASK << GPIO_PIN_BASE));
    gpio_set_dir_masked(VALVES_MASK << VALVE_PIN_BASE, 0xFFFFFFFF);
    gpio_put_masked(VALVES_MASK << VALVE_PIN_BASE, 0);
    // Reset exposed GPIO pins to inputs.
    gpio_set_dir_masked(GPIOS_MASK << GPIO_PIN_BASE, 0);

    // Reset Harp register struct elements.
    app_regs.ValvesState = 0;
    app_regs.ValvesSet = 0;
    app_regs.ValvesClear = 0;
    // Turn off all outputs.
    for (auto& valve_driver: valve_drivers)
        valve_driver.deenergize();
}

