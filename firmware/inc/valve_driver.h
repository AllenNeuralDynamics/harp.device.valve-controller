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
    float set_pwm_frequency(float freq_hz);

    void set_hit_duration_us(uint32_t hit_duration_us);

    void set_hold_duration_us(uint32_t hit_duration_us);

    /**
     * \brief enable the valve output
     * \note inline
     */
    inline void energize()
    }

    /**
     * \brief disable the valve output by setting duty cycle to 0.
     * \note inline.
     */
    inline void deenergize()
    {
    }


    void set_normalized_hit_output(float output);
    void set_normalized_hold_output(float output);

private:

    enum State
    {
        HIT,
        HOLD,
        DEENERGIZED
    };

    State state_;

    void reset_fsm();

    void update_fsm();

    float hit_output_;
    float hold_output_;

    uint32_t hit_duration_us_;

    uint32_t hit_start_time_us_;

    PWM pwm_;

    static const uint32_t DEFAULT_PWM_FREQUENCY_HZ = 25000;
};

#endif // VALVE_DRIVER_H
