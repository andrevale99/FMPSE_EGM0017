#include "motor_dc.h"

static int active_rcc(GPIO_TypeDef *gpio)
{
    if (gpio == GPIOA)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        return MOTOR_DC_OK;
    }
    else if (gpio == GPIOB)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        return MOTOR_DC_OK;
    }
    else if (gpio == GPIOC)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
        return MOTOR_DC_OK;
    }
    else if (gpio == GPIOD)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
        return MOTOR_DC_OK;
    }
    else if (gpio == GPIOE)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
        return MOTOR_DC_OK;
    }
    else if (gpio == GPIOH)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
        return MOTOR_DC_OK;
    }

    return MOTOR_DC_INVALID_ARG;
}

motor_dc_err motor_dc_init(motor_dc_config_t *config)
{
    if (!config)
        return MOTOR_DC_INVALID_ARG;

    if ((active_rcc(config->gpio) < 0))
        return MOTOR_DC_INVALID_GPIOx;

    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

        /* 3. Configura o Timer */
    config->timerX->PSC = 8399; // Prescaler
    config->timerX->ARR = 800;  // Auto-reload

    /* Duty cycle inicial (50%) */
    config->timerX->CCR1 = 500;

    /* 4. PWM mode 1 no canal 1 */
    config->timerX->CCMR1 &= ~(TIM_CCMR1_OC1M);
    config->timerX->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos); // PWM mode 1

    config->timerX->CCMR1 |= TIM_CCMR1_OC1PE; // preload enable

    /* 5. Habilita saída do canal */
    config->timerX->CCER |= TIM_CCER_CC1E;

    /* 6. Habilita saída principal (timer avançado) */
    config->timerX->BDTR |= TIM_BDTR_MOE;

    /* 7. Habilita auto-reload preload */
    config->timerX->CR1 |= TIM_CR1_ARPE;

    /* 8. Atualiza registros */
    config->timerX->EGR |= TIM_EGR_UG;

    /* 9. Inicia o timer */
    config->timerX->CR1 |= TIM_CR1_CEN;

    return MOTOR_DC_OK;
}