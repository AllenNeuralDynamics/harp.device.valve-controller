#include <valve_controller_app.h>

app_regs_t app_regs;


// Create Hit-and-Hold Valve Drivers.
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
    {(uint8_t*)&app_regs.ValveConfig0, sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfig1, sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfig2, sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfig3, sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfig4, sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfig5, sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfig6, sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfig7, sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfig8, sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfig9, sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfig10, sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfig11, sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfig12, sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfig13, sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfig14, sizeof(ValveConfig), U8},
    {(uint8_t*)&app_regs.ValveConfig15, sizeof(ValveConfig), U8},
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
    // TODO: in the future, we will read them from the Hit-and-Hold FSMs.
    app_regs.ValvesState = uint16_t(gpio_get_all() >> VALVE_PIN_BASE);
    if (!HarpCore::is_muted())
        HarpCore::send_harp_reply(READ, reg_address);
}

void write_valves_state(msg_t& msg)
{
    HarpCore::copy_msg_payload_to_register(msg);
    // write to output pins (not including pins controlled by PWM Tasks).
    // TODO: in the future, we will apply them to the Hit-and-Hold FSMs.
    gpio_put_masked(uint32_t(app_regs.ValvesState) << VALVE_PIN_BASE,
                    uint32_t(app_regs.ValvesState) << VALVE_PIN_BASE);
    // Read back what we just wrote to the peripheral since it's fast.
    // Add delay for the change to take effect. (May be related to slew rate).
    asm volatile("nop \n nop \n nop");
    app_regs.ValvesState = uint16_t(gpio_get_all() >> VALVE_PIN_BASE);
#if defined(DEBUG)
    printf("Wrote to GPIOs. New GPIO state: 0x%08lx\r\n", gpio_get_all());
#endif
    // Reply with the actual value that we wrote.
    if (!HarpCore::is_muted())
        HarpCore::send_harp_reply(WRITE, msg.header.address);
}

void read_valves_set(uint8_t reg_address)
{
    if (!HarpCore::is_muted())
        HarpCore::send_harp_reply(READ, reg_address);
}

void write_valves_set(msg_t& msg)
{
    // Reply with the actual value that we wrote.
    if (!HarpCore::is_muted())
        HarpCore::send_harp_reply(WRITE, msg.header.address);
}

void read_valves_clear(uint8_t reg_address)
{
    if (!HarpCore::is_muted())
        HarpCore::send_harp_reply(READ, reg_address);
}

void write_valves_clear(msg_t& msg)
{
    // Reply with the actual value that we wrote.
    if (!HarpCore::is_muted())
        HarpCore::send_harp_reply(WRITE, msg.header.address);
}


void read_any_valve_config(uint8_t reg_address)
{
    // FIXME implement this.
    if (!HarpCore::is_muted())
        HarpCore::send_harp_reply(READ, reg_address);
}


void write_any_valve_config(msg_t& msg)
{
    HarpCore::copy_msg_payload_to_register(msg);
    uint8_t valve_index = msg.header.address - VALVE_START_APP_ADDRESS;
    // Register struct is packed, so we can access the valve configuration
    // w/ pointer arithmetic.
    ValveConfig* valve_cfg_ptr = (&(app_regs.ValveConfig0))
                                    + sizeof(ValveConfig) * valve_index;
    // Apply the configuration.
    valve_drivers[valve_index].set_hit_duration_us(valve_cfg_ptr->hit_duration_us);
    valve_drivers[valve_index].set_normalized_hit_output(valve_cfg_ptr->hit_output);
    valve_drivers[valve_index].set_normalized_hold_output(valve_cfg_ptr->hit_output);
    if (!HarpCore::is_muted())
        HarpCore::send_harp_reply(WRITE, msg.header.address);
}

void update_app_state()
{

}

void reset_app()
{
    // init all GPIO pins we are using.
    gpio_init_mask((VALVES_MASK << VALVE_PIN_BASE) | (GPIOS_MASK << GPIO_PIN_BASE));
    // Reset exposed GPIO pins to inputs.
    gpio_set_dir_masked(GPIOS_MASK << GPIO_PIN_BASE, 0);
    // Reset Buffer ctrl pins to all outputs and drive an input setting.
    gpio_set_dir_masked(VALVES_MASK << VALVE_PIN_BASE, 0xFFFFFFFF);
    gpio_put_masked(VALVES_MASK << VALVE_PIN_BASE, 0);

    // Reset Harp register struct elements.
    app_regs.ValvesState = 0;
    app_regs.ValvesSet = 0;
    app_regs.ValvesClear = 0;
}

