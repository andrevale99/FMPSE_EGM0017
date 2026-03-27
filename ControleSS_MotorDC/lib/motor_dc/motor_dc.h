#ifndef MOTOR_DC_H
#define MOTOR_DC_H

#include <stdint.h>
#include <stddef.h>
#include <stm32f411xe.h>

#define MOTOR_DC_OK 0
#define MOTOR_DC_INVALID_ARG -1
#define MOTOR_DC_INVALID_GPIOx -2

typedef int motor_dc_err;

typedef struct 
{
    GPIO_TypeDef *gpio;
    TIM_TypeDef *timerX;
    uint32_t prescale;
    uint32_t freq;
}motor_dc_config_t;

motor_dc_err motor_dc_init(motor_dc_config_t *config);

#endif