#include "motor_dc.h"

static uint32_t maxDutyCycle = 0;

motor_dc_err motor_dc_on_off(motor_dc_handle_t *motor, uint8_t state)
{
    if (!(motor->m1.sleep.set_state) || !(motor->m2.sleep.set_state))
        return RET_INVALID_ARG;

    motor->m1.sleep.set_state(state);

    return RET_OK;
}

motor_dc_err motor_dc_pwm_channel(motor_dc_handle_t *motor, int8_t channel, uint32_t duty)
{
    if (channel < 0 || channel >= MOTOR_DC_CHANNELS)
        return RET_INVALID_ARG;

    if (!(motor->m1.sleep.set_state) || !(motor->m2.sleep.set_state))
        return RET_INVALID_ARG;

    motor->m1.sleep.set_state(1);

    if (channel == 0)
    {
        motor->m1.pwm.set_duty_cycle(duty);
        motor->m2.pwm.set_duty_cycle(0);
    }
    else
    {
        motor->m2.pwm.set_duty_cycle(duty);
        motor->m1.pwm.set_duty_cycle(0);
    }

    return RET_OK;
}