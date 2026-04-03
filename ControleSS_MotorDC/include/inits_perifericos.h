#ifndef INITS_H
#define INITS_H

#include <stm32f411xe.h>
#include "lcd16x2.h"

void rcc_enable_clocks(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
}

// =================================
// SETUP PARA O DRV8833
// =================================

void gpio_drv8833_setup_in1(void)
{
    // Modo alternate, Fast speed, NO pull-up/down
    GPIOA->MODER |= (2 << 16);
    GPIOA->OSPEEDR |= (2 << 16);
    GPIOA->PUPDR = 0;

    // Alternate function 1 (TIM1_CHANNELS)
    // para os pinos PA8
    GPIOA->AFR[1] |= (1 << 0);
}

void gpio_drv8833_set_state_in1(uint8_t state)
{
    if (state)
        GPIOA->BSRR |= (1 << 8);
    else
        GPIOA->BSRR |= (1 << 24);
}

void gpio_drv8833_setup_in2(void)
{
    // Modo alternate, Fast speed, NO pull-up/down
    GPIOA->MODER |= (2 << 18);
    GPIOA->OSPEEDR |= (2 << 18);
    GPIOA->PUPDR = 0;

    // Alternate function 1 (TIM1_CHANNELS)
    // para os pinos PA9
    GPIOA->AFR[1] |= (1 << 4);
}

void gpio_drv8833_set_state_in2(uint8_t state)
{
    if (state)
        GPIOA->BSRR |= (1 << 9);
    else
        GPIOA->BSRR |= (1 << 25);
}

void gpio_drv8833_setup_sleep(void)
{
    // OUTPUT, medium speed, no pull-up/down
    GPIOB->MODER |= (1 << 4);
    GPIOB->OSPEEDR |= (1 << 4);
    GPIOB->PUPDR = 0;
}

void gpio_dr8833_set_state_sleep(uint8_t state)
{
    if (state)
        GPIOB->BSRR |= (1 << 2);
    else
        GPIOB->BSRR |= (1 << 18);
}

void pwm_init(void)
{
    /* 3. Configura o Timer */
    TIM1->PSC = 15;  // Prescaler
    TIM1->ARR = 999; // Auto-reload

    /* Duty cycle inicial (50%) */
    TIM1->CCR1 = 0;

    /* 4. PWM mode 1 no canal 1 e 2*/
    TIM1->CCMR1 &= ~(TIM_CCMR1_OC1M);
    TIM1->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos) | (6 << TIM_CCMR1_OC2M_Pos); // PWM mode 1

    TIM1->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE; // preload enable

    /* 5. Habilita saída do canal */
    TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

    /* 6. Habilita saída principal (timer avançado) */
    TIM1->BDTR |= TIM_BDTR_MOE;

    /* 7. Habilita auto-reload preload */
    TIM1->CR1 |= TIM_CR1_ARPE;

    /* 8. Atualiza registros */
    TIM1->EGR |= TIM_EGR_UG;

    /* 9. Inicia o timer */
    TIM1->CR1 |= TIM_CR1_CEN;
}

void tim1_channel1_pwm_set_duty(uint32_t duty)
{
    TIM1->CCR1 = duty;
}

void tim1_channel2_pwm_set_duty(uint32_t duty)
{
    TIM1->CCR2 = duty;
}

// ================================
//  TIMER 3 SETUP (AMOSTRAGEM)
// ================================

void TIMER4_Setup(void)
{
    // TIMER3 de 16 bits ativado
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    // Carrega o valor maximo da contagem
    TIM4->ARR = 780;

    // Ativa o prescale do contador
    TIM4->PSC = 1023;

    // Carregamento imediato de ARR
    TIM4->EGR |= TIM_EGR_UG;

    // Ativa a interrupcao
    TIM4->DIER |= TIM_DIER_UIE;

    TIM4->SR = 0;

    NVIC_EnableIRQ(TIM4_IRQn);
    NVIC_SetPriority(TIM4_IRQn, 0);

    // Ativa a interrupcao somente pelo overflow/underflow ou DMA
    // e ativa o contador
    TIM4->CR1 |= TIM_CR1_URS | TIM_CR1_CEN;
}

// =================================
// SETUP PARA O LCD16x2
// =================================

void gpio_lcd16x2_setup(void)
{
    // Pinos A0,A1,A2 e A3 como output
    GPIOA->MODER |= (0x55);
    GPIOA->OSPEEDR |= (0xAA);

    // Pinos B12 (EN) e B13 (RS)
    GPIOB->MODER |= (0x5 << 24);
    GPIOB->OSPEEDR |= (0xA << 24);
}

void write_d4(uint8_t state)
{
    if (state)
        GPIOA->BSRR |= (1 << 0);
    else
        GPIOA->BSRR |= (1 << (0 + 16));
}

void write_d5(uint8_t state)
{
    if (state)
        GPIOA->BSRR |= (1 << 1);
    else
        GPIOA->BSRR |= (1 << (1 + 16));
}

void write_d6(uint8_t state)
{
    if (state)
        GPIOA->BSRR |= (1 << 2);
    else
        GPIOA->BSRR |= (1 << (2 + 16));
}

void write_d7(uint8_t state)
{
    if (state)
        GPIOA->BSRR |= (1 << 3);
    else
        GPIOA->BSRR |= (1 << (3 + 16));
}

void write_en(uint8_t state)
{
    if (state)
        GPIOB->BSRR |= (1 << 12); // EN = 1
    else
        GPIOB->BSRR |= (1 << 28); // EN = 0
}

void write_rs(uint8_t state)
{
    if (state)
        GPIOB->BSRR |= (1 << 13);
    else
        GPIOB->BSRR |= (1 << 29);
}

// ===============================
// 
// ===============================

// ===============================
// SETUP SYSTEM
// ===============================

void setup_system(void)
{
    rcc_enable_clocks();
    systick_init();
    gpio_lcd16x2_setup();
    gpio_drv8833_setup_in1();
    gpio_drv8833_setup_in2();
    gpio_drv8833_setup_sleep();

    pwm_init();
    TIMER4_Setup();
}

#endif