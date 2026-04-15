#ifndef INITS_H
#define INITS_H

#include <stm32f411xe.h>
#include "lcd16x2.h"

#define DRV8833_GPIO_SLEEP 2
#define DRV8833_GPIO_IN_1 8
#define DRV8833_GPIO_IN_2 9

#define LCD_GPIO_D4 0
#define LCD_GPIO_D5 1
#define LCD_GPIO_D6 2
#define LCD_GPIO_D7 3
#define LCD_GPIO_EN 12
#define LCD_GPIO_RS 13

#define ENCODER_CHN_A 7
#define ENCODER_CHN_B 6

void rcc_enable_clocks(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
}

// =================================
// SETUP PARA O DRV8833
// =================================

void gpio_drv8833_setup_in1(void)
{
    // Modo alternate, Fast speed, NO pull-up/down
    GPIOA->MODER |= (2 << (DRV8833_GPIO_IN_1 * 2));
    GPIOA->OSPEEDR |= (2 << (DRV8833_GPIO_IN_1 * 2));
    GPIOA->PUPDR = 0;

    // Alternate function 1 (TIM1_CHANNELS)
    // para os pinos PA8
    GPIOA->AFR[1] |= (1 << 0);
}

void gpio_drv8833_set_state_in1(uint8_t state)
{
    if (state)
        GPIOA->BSRR |= (1 << (DRV8833_GPIO_IN_1 * 2));
    else
        GPIOA->BSRR |= (1 << (DRV8833_GPIO_IN_1 + 16));
}

void gpio_drv8833_setup_in2(void)
{
    // Modo alternate, Fast speed, NO pull-up/down
    GPIOA->MODER |= (2 << (DRV8833_GPIO_IN_2 * 2));
    GPIOA->OSPEEDR |= (2 << (DRV8833_GPIO_IN_2 * 2));
    GPIOA->PUPDR = 0;

    // Alternate function 1 (TIM1_CHANNELS)
    // para os pinos PA9
    GPIOA->AFR[1] |= (1 << 4);
}

void gpio_drv8833_set_state_in2(uint8_t state)
{
    if (state)
        GPIOA->BSRR |= (1 << (DRV8833_GPIO_IN_2));
    else
        GPIOA->BSRR |= (1 << ((DRV8833_GPIO_IN_2 + 16)));
}

void gpio_drv8833_setup_sleep(void)
{
    // OUTPUT, medium speed, no pull-up/down
    GPIOB->MODER |= (1 << (DRV8833_GPIO_SLEEP * 2));
    GPIOB->OSPEEDR |= (1 << (DRV8833_GPIO_SLEEP * 2));
    GPIOB->PUPDR = 0;
}

void gpio_dr8833_set_state_sleep(uint8_t state)
{
    if (state)
        GPIOB->BSRR |= (1 << DRV8833_GPIO_SLEEP);
    else
        GPIOB->BSRR |= (1 << (DRV8833_GPIO_SLEEP + 2));
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
        GPIOA->BSRR |= (1 << LCD_GPIO_D4);
    else
        GPIOA->BSRR |= (1 << (LCD_GPIO_D4 + 16));
}

void write_d5(uint8_t state)
{
    if (state)
        GPIOA->BSRR |= (1 << LCD_GPIO_D5);
    else
        GPIOA->BSRR |= (1 << (LCD_GPIO_D5 + 16));
}

void write_d6(uint8_t state)
{
    if (state)
        GPIOA->BSRR |= (1 << LCD_GPIO_D6);
    else
        GPIOA->BSRR |= (1 << (LCD_GPIO_D6 + 16));
}

void write_d7(uint8_t state)
{
    if (state)
        GPIOA->BSRR |= (1 << LCD_GPIO_D7);
    else
        GPIOA->BSRR |= (1 << (LCD_GPIO_D7 + 16));
}

void write_en(uint8_t state)
{
    if (state)
        GPIOB->BSRR |= (1 << LCD_GPIO_EN); // EN = 1
    else
        GPIOB->BSRR |= (1 << (LCD_GPIO_EN + 16)); // EN = 0
}

void write_rs(uint8_t state)
{
    if (state)
        GPIOB->BSRR |= (1 << LCD_GPIO_RS);
    else
        GPIOB->BSRR |= (1 << (LCD_GPIO_EN + 16));
}

// ===================================
// CONFIGURAR ENCODER INTERFACE (TIM3)
// ===================================

void TIMER3_setup(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* PA6, PA7 → AF2 */
    GPIOA->MODER &= ~((3 << (ENCODER_CHN_B * 2)) | (3 << (ENCODER_CHN_A * 2)));
    GPIOA->MODER |= ((2 << (ENCODER_CHN_B * 2)) | (2 << (ENCODER_CHN_A * 2)));

    GPIOA->AFR[0] |= (2 << (ENCODER_CHN_B * 4)) | (2 << (ENCODER_CHN_A * 4));

    /* Pull-up (recomendado) */
    GPIOA->PUPDR |= (1 << (ENCODER_CHN_B * 2)) | (1 << (ENCODER_CHN_A * 2));

    /* Encoder mode */
    TIM3->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;

    /* CH1 e CH2 como entrada */
    TIM3->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;

    /* Contador */
    TIM3->ARR = 0xFFFF;
    TIM3->CNT = 0;

    TIM3->CR1 |= TIM_CR1_CEN;
}

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
    TIMER3_setup();
}

#endif