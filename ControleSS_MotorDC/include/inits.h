#ifndef INITS_H
#define INITS_H

#include <stm32f411xe.h>

void init_drv8833_channels(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Modo alternate, Fast speed, NO pull-up/down
    GPIOA->MODER |= (2 << 16) | (2 << 18);
    GPIOA->OSPEEDR |= (2 << 16) | (2 << 18);
    GPIOA->PUPDR = 0;

    // Alternate function 1 (TIM1_CHANNELS)
    GPIOA->AFR[1] |= (1 << 0) | (1 << 4);
}

#endif