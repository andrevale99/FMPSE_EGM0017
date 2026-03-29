#include "drv8833.h"

drv8833_err drv8833_in_init(drv8833_handle_t *handle)
{
    if (!handle)
        return RET_INVALID_ARG;

    if (!(handle->in.config_gpio) ||
        !(handle->in.set_state))
        return RET_INVALID_ARG;

    handle->in.config_gpio();
    handle->in.set_state(0);

    if (handle->pwm.config_pwm)
        handle->pwm.config_pwm();

    return RET_OK;
}

drv8833_err drv8833_sleep_init(drv8833_sleep_t *sleep_t)
{
    if(!sleep_t)
        return RET_INVALID_ARG;
    
    if(!(sleep_t->config_gpio) || !(sleep_t->set_state))
        return RET_INVALID_ARG;

    sleep_t->config_gpio();
    sleep_t->set_state(0);

    return RET_OK;
}

drv8833_err drv8833_set_sleep_state(drv8833_sleep_t *sleepGPIO, uint8_t state)
{
    if (!(sleepGPIO))
        return RET_INVALID_ARG;

    sleepGPIO->set_state(state);

    return RET_OK;
}

drv8833_err drv8833_set_duty_cycle(drv8833_handle_t *handle, uint32_t duty)
{
    if (!(handle->pwm.config_pwm))
        return RET_INVALID_ARG;

    if (duty > handle->pwm.maxDuty)
        handle->pwm.set_duty_cycle(handle->pwm.maxDuty);

    handle->pwm.set_duty_cycle(duty);

    return RET_OK;
}