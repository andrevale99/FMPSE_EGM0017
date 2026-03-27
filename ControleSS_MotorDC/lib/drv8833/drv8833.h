#ifndef DRV_8833_H
#define DRV_8833_H

#include <stdint.h>
#include <stddef.h>
#include <stm32f411xe.h>

#define DRV8833_NUM_CHANNELS 4

#define DRV8833_RET_OK 0
#define DRV8833_RET_INVALID_ARG -1
#define DRV8833_RET_NO_GPIO -2

#define DRV8833_DISCONNECT_GPIO -1

typedef int drv8833_err;

typedef struct
{
    GPIO_TypeDef *gpioReg;
    int32_t AlternateFuncSel;
    int32_t OutputMode;
    int32_t in;
} drv8833_gpio_config_t;

typedef struct
{
    drv8833_gpio_config_t IN[DRV8833_NUM_CHANNELS];
    drv8833_gpio_config_t SLEEP;
} drv8833_config_t;

drv8833_err drv8833_init(drv8833_config_t *config);

void drv8833_set_sleep_state(drv8833_config_t *config, uint8_t state);

#endif