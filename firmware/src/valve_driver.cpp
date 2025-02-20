#include <valve_driver.h>


ValveDriver::ValveDriver(uint8_t pwm_pin)
:pwm_{pwm_pin}, hit_output_{1.0}, hold_output_{1.0}, hit_duration_us_{0},
 energize_queued_{false}, deenergize_queued_{false}
{
    pwm_.set_duty_cycle(0);
    pwm_.enable_output();
    reset_fsm();
    set_pwm_frequency_hz(DEFAULT_PWM_FREQUENCY_HZ);
}

ValveDriver::~ValveDriver()
{
    pwm_.disable_output();
}

void ValveDriver::set_normalized_hit_output(float output)
{
    hit_output_ = output;
    // Reapply the frequency change if we are in the right state.
    if (state_ == HIT)
        pwm_.set_duty_cycle(hit_output_);
}

void ValveDriver::set_normalized_hold_output(float output)
{
    hold_output_ = output;
    // Reapply the frequency change if we are in the right state.
    if (state_ == HOLD)
        pwm_.set_duty_cycle(hold_output_);
}

void ValveDriver::set_hit_duration_us(uint32_t hit_duration_us)
{
    hit_duration_us_ = hit_duration_us_;
}

float ValveDriver::set_pwm_frequency_hz(float hz)
{
    return pwm_.set_frequency(hz);
}

void ValveDriver::reset_fsm()
{
    hit_start_time_us_ = 0;
    // Default to "hold" settings that are identical to "hit" settings.
    hit_output_ = 1.0f;
    hold_output_ = 1.0f;
    hit_duration_us_ = 0;
    state_ = DEENERGIZED;
}

void ValveDriver::energize()
{
    energize_queued_ = true;
    update_fsm();
}

void ValveDriver::deenergize()
{
    deenergize_queued_ = true;
    update_fsm();
}

void ValveDriver::update_fsm()
{
    uint32_t curr_time_us = time_us_32();
    // Handle next-state logic (based on inputs and current state).
    State next_state{state_};
    // Start with state transition logic that applies to all states.
    if (energize_queued_)
        next_state = HIT;
    else if (deenergize_queued_)
        next_state = DEENERGIZED;
    else // Handle state-based state transition logic.
    {
        switch (state_)
        {
            case HIT:
                if ((curr_time_us - hit_start_time_us_) >= hit_duration_us_)
                    next_state = HOLD;
                break;
            case HOLD:
                break;
            case DEENERGIZED:
                break;
            default:
                break;
        }
    }
    // Handle output logic based on inputs current state, and next state.
    if (energize_queued_ && (next_state == HIT))
    {
        hit_start_time_us_ = curr_time_us;
        pwm_.set_duty_cycle(hit_output_);
    }
    else if ((state_ != HOLD) && (next_state == HOLD))
        pwm_.set_duty_cycle(hold_output_);
    else if (next_state == DEENERGIZED)
        pwm_.set_duty_cycle(0);

    // Clear flags
    energize_queued_ = false;
    deenergize_queued_ = false;

    // Apply state transition.
    state_ = next_state;
}
