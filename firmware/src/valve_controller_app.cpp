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
RegSpecs app_reg_specs[reg_count]
{
    {(uint8_t*)&app_regs.ValvesState, sizeof(app_regs.ValvesState), U16},
    {(uint8_t*)&app_regs.ValvesSet, sizeof(app_regs.ValvesSet), U16},
    {(uint8_t*)&app_regs.ValvesClear, sizeof(app_regs.ValvesClear), U16},
    // More specs here if we add additional registers.
};

RegFnPair reg_handler_fns[reg_count]
{
    {read_valves_state, write_valves_state},
    {read_valves_set, write_valves_set},
    {read_valves_clear, write_valves_clear},
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

