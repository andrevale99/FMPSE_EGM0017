#ifndef LCD16X2_H
#define LCD16X2_H

#include <stdint.h>

#include <stm32f411xe.h>

#include "systick.h"

typedef struct 
{
    GPIO_TypeDef GPIOx_DATA;
    uint8_t d4;
    uint8_t d5;
    uint8_t d6;
    uint8_t d7;

    GPIO_TypeDef GPIOx_RS;
    uint8_t rs;
    
    GPIO_TypeDef GPIOx_EN;
    uint8_t en;

}lcd16x2_gpio_t;

typedef struct
{

}lcd16x2_config_t;


#endif