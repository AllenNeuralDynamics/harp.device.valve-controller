#ifndef VALVE_DRIVER_H
#define VALVE_DRIVER_H
#include <stdint.h>
#include <pico/stdlib.h>
#include <pwm.h>
#ifdef DEBUG
    #include <cstdio> // for printf
#endif


/**
 * \brief ValveDriver class with hit-and-hold capability
 */
class ValveDriver
{
public:
/**
 * \brief constructor.
 * \param pwm_pin the pwm output pin to the controller enable pin.
 */
    ValveDriver(uint8_t pwm_pin);

/**
 * \brief destructor.
 */
    ~ValveDriver();

/**
 * \brief set the pwm frequency in Hz.
 * \returns the actual pwm frequency set on the device.
 */
    float set_pwm_frequency_hz(float freq_hz);

/**
 * \brief set the duration (in microseconds) that the valve operates at the
 *  "hit" power level before transitioning to the "hold" power level.
 */
    void set_hit_duration_us(uint32_t hit_duration_us);

/**
 * \brief disable hit and hold feature such that the valve always operates
 *  at the hit output power level.
 */
    void disable_hit_and_hold();

/**
 * \brief enable the valve output.
 */
    void energize();

/**
 * \brief disable the valve output.
 */
    void deenergize();

/**
 * \brief True if valve is currently turned enabled.
 */
    inline bool is_energized() const
    {return (state_ != DEENERGIZED);}

/**
 * \brief set the hit output power level.
 * \param output output power from 0.0 to 1.0. Note that while the input is a
 *  float, only 1 decimal place is used.
 */
    void set_normalized_hit_output(float output);

/**
 * \brief set the hold output power level.
 * \param output output power from 0.0 to 1.0. Note that while the input is a
 *  float, only 1 decimal place is used.
 */
    void set_normalized_hold_output(float output);

    inline float get_hit_output() const
    {return hit_output_;}

    inline float get_hold_output() const
    {return hold_output_;}

    inline float get_hit_duration_us() const
    {return hold_output_;}

private:

    enum State
    {
        HIT,
        HOLD,
        DEENERGIZED
    };

    State state_;

    void reset_fsm();

    bool energize_queued_;
    bool deenergize_queued_;
    void update_fsm();

    float hit_output_;
    float hold_output_;

    uint32_t hit_duration_us_;

    uint32_t hit_start_time_us_;

    PWM pwm_;

    static const uint32_t DEFAULT_PWM_FREQUENCY_HZ = 25000;
};

#endif // VALVE_DRIVER_H
