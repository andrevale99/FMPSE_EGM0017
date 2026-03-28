#include "drv8833.h"

drv8833_err drv8833_init(drv8833_handle_t *config)
{
    if (!config)
        return RET_INVALID_ARG;

    for (uint8_t i = 0; i < DRV8833_NUM_CHANNELS; ++i)
    {
        if (!(config->in[i].config_gpio) ||
            !(config->in[i].set_state))
            continue;

        config->in[i].config_gpio();
        config->in[i].set_state(0);
    }

    if ((config->sleep.config_gpio) &&
        (config->sleep.set_state))
    {
        config->sleep.config_gpio();
        config->sleep.set_state(0);
    }

    if (config->pwm.config_pwm)
        config->pwm.config_pwm();

    return RET_OK;
}

drv8833_err drv8833_set_duty_cycle(drv8833_handle_t *config, uint32_t duty)
{
    if (!(config->pwm.config_pwm))
        return RET_INVALID_ARG;

    if (duty > config->pwm.maxDuty)
        config->pwm.set_duty_cycle(config->pwm.maxDuty);

    config->pwm.set_duty_cycle(duty);

    return RET_OK;
}