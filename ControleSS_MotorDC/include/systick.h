#ifndef SYSTICK_H
#define SYSTICK_H

#include <stm32f411xe.h>
#include <stdint.h>

static volatile uint32_t msTicks = 0; 

/* Interrupção do SysTick */
void SysTick_Handler(void)
{
    msTicks++;
}

/* Inicializa SysTick para 1 ms */
void systick_init(void)
{
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);
}

/* Delay em ms */
void delay_ms(uint32_t ms)
{
    uint32_t start = msTicks;
    while ((msTicks - start) < ms);
}

#endif