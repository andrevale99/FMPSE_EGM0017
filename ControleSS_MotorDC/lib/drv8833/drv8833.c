#include "drv8833.h"

static int active_rcc(GPIO_TypeDef *gpio)
{
    if (gpio == GPIOA)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        return DRV8833_RET_OK;
    }
    else if (gpio == GPIOB)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        return DRV8833_RET_OK;
    }
    else if (gpio == GPIOC)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
        return DRV8833_RET_OK;
    }
    else if (gpio == GPIOD)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
        return DRV8833_RET_OK;
    }
    else if (gpio == GPIOE)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
        return DRV8833_RET_OK;
    }
    else if (gpio == GPIOH)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
        return DRV8833_RET_OK;
    }

    return DRV8833_RET_INVALID_ARG;
}

drv8833_err drv8833_init(drv8833_config_t *config)
{
    if (!config)
        return DRV8833_RET_INVALID_ARG;

    if (config->SLEEP.in != DRV8833_DISCONNECT_GPIO)
    {
        if ((active_rcc(config->SLEEP.gpioReg) < 0))
            return DRV8833_RET_NO_GPIO;

        config->SLEEP.gpioReg->MODER = (1 << (config->SLEEP.in * 2));
        config->SLEEP.gpioReg->OSPEEDR = (1 < (config->SLEEP.in * 2));
        config->SLEEP.gpioReg->ODR = (0 << config->SLEEP.in);
    }

    for (uint8_t i = 0; i < DRV8833_NUM_CHANNELS; ++i)
    {
        if (config->IN[i].in != DRV8833_DISCONNECT_GPIO)
        {
            active_rcc(config->IN[i].gpioReg);

            config->IN[i].gpioReg->MODER = config->IN[i].OutputMode;

            if (config->IN[i].in < 8)
                config->IN[i].gpioReg->AFR[0] = config->IN[i].AlternateFuncSel;
            else
                config->IN[i].gpioReg->AFR[1] = config->IN[i].AlternateFuncSel;
        }
    }

    return DRV8833_RET_OK;
}

void drv8833_set_sleep_state(drv8833_config_t *config, uint8_t state)
{
    if (state)
        config->SLEEP.gpioReg->ODR |= (state << config->SLEEP.in);
    else
        config->SLEEP.gpioReg->ODR &= ~(state << config->SLEEP.in);
}