#include "lcd16x2.h"

lcd16x2_err lcd16x2_init_4bits(lcd16x2_handle *handle)
{
    // Delay para estabilizar VSS
    handle->delay_ms(50);

    lcd16x2_pulse_enable();
    handle->delay_ms(50);
    lcd16x2_pulse_enable();
    handle->delay_ms(50);

    lcd16x2_send_cmd(handle, BITS_4 | LINES_2);                      // interface de 4 bits 2 linhas (aqui se habilita as 2 linhas)
                                                                     // são enviados os 2 nibbles (0x2 e 0x8)
    lcd16x2_send_cmd(handle, DISPLAY_ON | CURSOR_ON | BLINK_CURSOR); // mensagem aparente cursor inativo não piscando
    lcd16x2_send_cmd(handle, SET_DDRAM | 0x00);                      // inicializa cursor na primeira posição a esquerda - 1a linha

    lcd16x2_send_cmd(handle, CLEAR_DISPLAY); // limpa todo o display
    lcd16x2_send_cmd(handle, RETURN_HOME);

    return 0;
}

lcd16x2_err lcd16x2_send_cmd(lcd16x2_handle *handle, uint8_t cmd)
{
    GPIOA->BSRR = (0xF << 16);
    GPIOA->BSRR = ((cmd >> 4) & 0xF); /* sending upper nibble */

    GPIOB->BSRR = (1 << 29); /* RS=0, command reg. */

    lcd16x2_pulse_enable();

    GPIOA->BSRR = (0xF << 16);
    GPIOA->BSRR |= (cmd & 0xF); /* sending lower nibble */
    lcd16x2_pulse_enable();

    handle->delay_ms(50);

    return 0;
}

lcd16x2_err lcd16x2_send_data(lcd16x2_handle *handle, uint8_t data)
{
    GPIOA->BSRR = (0xF << 16);
    GPIOA->BSRR = ((data >> 4) & 0xF); /* sending upper nibble */

    // Coloca o pino em 1 (WRITE)
    GPIOB->BSRR = (1 << 13); /* RS=1, command reg. */

    lcd16x2_pulse_enable();

    GPIOA->BSRR = (0xF << 16);
    GPIOA->BSRR |= (data & 0xF); /* sending lower nibble */
    lcd16x2_pulse_enable();

    // Desliga o RS
    GPIOB->BSRR |= (1 << 29); /* RS=0, command reg. */

    handle->delay_ms(50);
    
    return 0;
}