#include "drv8833.h"

drv8833_err drv8833_set_in_level(drv8833_handle_t *drv8833, uint8_t channel, uint8_t state)
{
    if (!(drv8833->in[channel].set_state))
        return RET_INVALID_ARG;

    drv8833->in[channel].set_state(state);

    return RET_OK;
}

drv8833_err drv8833_set_sleep_state(drv8833_handle_t *drv8833, uint8_t state)
{
    if (!(drv8833->sleep.set_state))
        return RET_INVALID_ARG;

    drv8833->sleep.set_state(state);

    return RET_OK;
}

drv8833_err drv8833_set_duty_cycle(drv8833_handle_t *drv8833, uint32_t duty)
{
    if (!(drv8833->pwm.set_duty_cycle))
        return RET_INVALID_ARG;

    if (duty > drv8833->pwm.maxDuty)
        drv8833->pwm.set_duty_cycle(drv8833->pwm.maxDuty);

    drv8833->pwm.set_duty_cycle(duty);

    return RET_OK;
}