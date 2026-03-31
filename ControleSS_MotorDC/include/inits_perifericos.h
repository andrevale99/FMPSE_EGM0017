#ifndef INITS_H
#define INITS_H

#include <stm32f411xe.h>

void rcc_enable_clocks(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
}

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

void gpio_set_state_in1(uint8_t state)
{
    if (state)
        GPIOA->BSRR |= (1<<8);
    else
        GPIOA->BSRR |= (1<<24);
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

void gpio_set_state_in2(uint8_t state)
{
    if (state)
        GPIOA->BSRR |= (1<<9);
    else
        GPIOA->BSRR |= (1<<25);
}

void gpio_drv8833_setup_sleep(void)
{
    // OUTPUT, medium speed, no pull-up/down
    GPIOB->MODER |= (1 << 4);
    GPIOB->OSPEEDR |= (1 << 4);
    GPIOB->PUPDR = 0;
}

void gpio_set_state_sleep(uint8_t state)
{
    if(state)
        GPIOB->BSRR |= (1<<2);
    else
        GPIOB->BSRR |= (1<<18);
}

void pwm_init(void)
{
    /* 3. Configura o Timer */
    TIM1->PSC = 15; // Prescaler
    TIM1->ARR = 999;  // Auto-reload

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

void TIMER3_Setup(void)
{
    //TIMER3 de 16 bits ativado
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    //Carrega o valor maximo da contagem
    TIM3->ARR = 779;

    //Ativa o prescale do contador
    TIM3->PSC = 1023;

    //Ativa o evento somente pelo Contador
    TIM3->EGR |= TIM_EGR_UG;

    //Ativa a interrupcao
    TIM3->DIER |= TIM_DIER_UIE;

    //Ativa a interrupcao somente pelo overflow/underflow ou DMA
    //e ativa o contador
    TIM3->CR1 |= TIM_CR1_URS | TIM_CR1_CEN;

    NVIC_EnableIRQ(TIM3_IRQn);
    NVIC_SetPriority(TIM3_IRQn, 0);
}

#endif