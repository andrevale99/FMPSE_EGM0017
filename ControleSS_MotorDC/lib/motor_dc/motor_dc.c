#include "motor_dc.h"

motor_dc_err motor_dc_init(motor_dc_handle_t *handle)
{
    if (!handle)
        return RET_INVALID_ARG;

    return RET_OK;
}

motor_dc_err motor_dc_pwm_channel(motor_dc_handle_t *motor, int8_t channel, uint32_t duty)
{
    if(channel < 0 || channel >= MOTOR_DC_CHANNELS)
        return RET_INVALID_ARG;

    if(channel == 0)
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