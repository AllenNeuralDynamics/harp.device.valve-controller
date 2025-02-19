#include <valve_driver.h>


ValveDriver::ValveDriver(uint8_t pwm_pin)
:pwm_{pwm_pin}, hit_output_{1.0}, hold_output_{1.0}, hit_duration_us_{0}
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

float ValveDriver::set_normalized_hit_output(float output)
{

}

float ValveDriver::set_normalized_hold_output(float output)
{
    hit_output_ = output;
    // Apply the frequency change if we are in the right state.
}


float ValveDriver::set_pwm_frequency_hz(float hz)
{
    pwm_.set_frequency(hz);
}


void ValveDriver::reset_fsm()
{
    hold_start_time_us_ = 0;
    hit_start_time_us_ = 0;
    state_ = OFF;
}

void ValveDriver::update_fsm()
{
    uint32_t curr_time_us = time_us_32();
    // Handle state transition logic.
    State next_state{state_};
    switch (state_)
    {
        case HIT:
            if (curr_time_us - hit_start_time_us_) >= hit_duration_us_
                next_state_ = HOLD;
            break;
        case HOLD:
            break;
        case DEENERGIZED:
            break;
        default:
            break;
    }
    // Handle state-change or state-dependent output logic.
    if (state_ != HIT) && (next_state == HIT)
    {
        hit_start_time_us_ = curr_time_us;
        pwm_.set_duty_cycle(hit_output_);
    }
    else if (state == HIT) && (next_state == HOLD)
        pwm_.set_duty_cycle(hold_output_);
    else if (next_state == DEENERGIZED)
        pwm_.set_duty_cycle(0);
    // Apply state transition.
    state_ = next_state;
}
