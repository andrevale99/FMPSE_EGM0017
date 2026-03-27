#ifndef MOTOR_DC_H
#define MOTOR_DC_H

#include <stdint.h>
#include <stddef.h>
#include <stm32f411xe.h>

#include "drv8833.h"

#define MOTOR_DC_OK 0
#define MOTOR_DC_INVALID_ARG -1
#define MOTOR_DC_INVALID_GPIOx -2

typedef int motor_dc_err;


typedef struct
{
    drv8833_gpio_config_t in1;
    drv8833_gpio_config_t in2;

    
}motor_dc_t;

motor_dc_err motor_dc_init(motor_dc_t *config);

#endif