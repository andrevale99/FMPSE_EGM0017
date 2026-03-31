#ifndef INITS_H
#define INITS_H

#include <stm32f411xe.h>

#define WRITE 1
#define CMD 0

#define CLEAR_DISPLAY 0x01
#define RETURN_HOME 0x02
#define SECOND_LINE 0xC0

#define ENTRY_MODE_SET 0x04
#define INCREMENT (ENTRY_MODE_SET | 0x02)     // I
#define DECREMENT (ENTRY_MODE_SET | 0x00)     // D
#define SHIFT_DISPLAY (ENTRY_MODE_SET | 0x01) // S

#define DISPLAY_CONTROL 0x08
#define DISPLAY_OFF (DISPLAY_CONTROL | 0x00)  // D
#define DISPLAY_ON (DISPLAY_CONTROL | 0x04)   // D
#define CURSOR_ON (DISPLAY_CONTROL | 0x02)    // C
#define BLINK_CURSOR (DISPLAY_CONTROL | 0x01) // B

#define CURSOR_SHIFT 0x10
#define DISPLAY_SHIFT (CURSOR_SHIFT | 0x08) // S
#define CURSOR_MOVE (CURSOR_SHIFT | 0x00)   // C
#define SHIFT_RIGHT (CURSOR_SHIFT | 0x04)   // R
#define SHIFT_LEFT (CURSOR_SHIFT | 0x00)    // L

#define FUNCTION_SET 0x20
#define BITS_8 (FUNCTION_SET | 0x00)    // DL
#define BITS_4 (FUNCTION_SET | 0x01)    // DL
#define LINES_1 (FUNCTION_SET | 0x00)   // N
#define LINES_2 (FUNCTION_SET | 0x08)   // N
#define FONT_5x8 (FUNCTION_SET | 0x00)  // F
#define FONT_5x10 (FUNCTION_SET | 0x04) // F

#define SET_DDRAM 0x80

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

void gpio_set_state_in2(uint8_t state)
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

void gpio_set_state_sleep(uint8_t state)
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

void TIMER3_Setup(void)
{
    // TIMER3 de 16 bits ativado
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Carrega o valor maximo da contagem
    TIM3->ARR = 779;

    // Ativa o prescale do contador
    TIM3->PSC = 1023;

    // Ativa o evento somente pelo Contador
    TIM3->EGR |= TIM_EGR_UG;

    // Ativa a interrupcao
    TIM3->DIER |= TIM_DIER_UIE;

    // Ativa a interrupcao somente pelo overflow/underflow ou DMA
    // e ativa o contador
    TIM3->CR1 |= TIM_CR1_URS | TIM_CR1_CEN;

    NVIC_EnableIRQ(TIM3_IRQn);
    NVIC_SetPriority(TIM3_IRQn, 0);
}

void lcd16x2_pulse_enable(void)
{
    delay_ms(1);
    GPIOB->BSRR |= (1 << 12); // EN = 1
    delay_ms(1);
    GPIOB->BSRR |= (1 << 28); // EN = 0
    delay_ms(1);
}

void lcd16x2_cmd(uint8_t cmd, uint8_t rs_state)
{
    GPIOA->BSRR = (0xF << 16);
    GPIOA->BSRR = ((cmd >> 4) & 0xF); /* sending upper nibble */

    if (rs_state == CMD)
        GPIOB->BSRR = (1 << 29); /* RS=0, command reg. */
    else
        GPIOB->BSRR = (1 << 13); /* RS=1, command reg. */

    lcd16x2_pulse_enable();

    GPIOA->BSRR = (0xF << 16);
    GPIOA->BSRR |= (cmd & 0xF); /* sending lower nibble */
    lcd16x2_pulse_enable();

    if (rs_state == WRITE)
        GPIOB->BSRR |= (1 << 29); /* RS=0, command reg. */

    delay_ms(50);
}

void lcd16x2_init4bits(void)
{
    // Pinos A0,A1,A2 e A3 como output
    GPIOA->MODER |= (0x55);
    GPIOA->OSPEEDR |= (0xAA);

    // Pinos B12 (EN) e B13 (RS)
    GPIOB->MODER |= (0x5 << 24);
    GPIOA->OSPEEDR |= (0xA << 24);

    delay_ms(100);

    lcd16x2_pulse_enable();
    delay_ms(10);
    lcd16x2_pulse_enable();
    delay_ms(10);

    lcd16x2_cmd(BITS_4 | LINES_2, CMD);                      // interface de 4 bits 2 linhas (aqui se habilita as 2 linhas)
                                                             // são enviados os 2 nibbles (0x2 e 0x8)
    lcd16x2_cmd(DISPLAY_ON | CURSOR_ON | BLINK_CURSOR, CMD); // mensagem aparente cursor inativo não piscando
    lcd16x2_cmd(SET_DDRAM | 0x00, CMD);                      // inicializa cursor na primeira posição a esquerda - 1a linha

    lcd16x2_cmd(CLEAR_DISPLAY, CMD);                         // limpa todo o display
    lcd16x2_cmd(RETURN_HOME, CMD);
}

#endif