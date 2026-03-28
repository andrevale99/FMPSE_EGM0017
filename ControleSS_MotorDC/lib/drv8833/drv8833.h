#ifndef DRV_8833_H
#define DRV_8833_H

#include <stdint.h>
#include <stddef.h>
#include <stm32f411xe.h>

#define RET_OK 0
#define RET_INVALID_ARG -1
#define RET_NO_GPIO -2

#define DRV8833_NUM_CHANNELS 4

typedef int drv8833_err;

typedef struct
{
    void (*config_gpio)(void);
    void (*set_state)(uint8_t level);
} drv8833_gpio_config_t;

typedef struct
{
    void (*config_pwm)(void);
}drv8833_pwm_config_t;


typedef struct
{
    drv8833_gpio_config_t in[DRV8833_NUM_CHANNELS];
    drv8833_gpio_config_t sleep;
    drv8833_pwm_config_t pwmConfig;
} drv8833_handle_t;

drv8833_err drv8833_init(drv8833_handle_t *config);

#endif