#include "lcd16x2.h"

lcd16x2_err lcd16x2_init_4bits(const lcd16x2_handle *handle)
{
    if (!(handle->delay_ms))
        return -1;

    handle->delay_ms(100);

    handle->rs.write(0);

    /* 0x3 */
    handle->d4.write(1);
    handle->d5.write(1);
    handle->d6.write(0);
    handle->d7.write(0);
    pulse_enable(handle);
    handle->delay_ms(5);

    /* 0x3 */
    pulse_enable(handle);
    handle->delay_ms(5);

    /* 0x3 */
    pulse_enable(handle);
    handle->delay_ms(1);

    /* 0x2 → 4 bits */
    handle->d4.write(0);
    handle->d5.write(1);
    handle->d6.write(0);
    handle->d7.write(0);
    pulse_enable(handle);
    handle->delay_ms(1);

    lcd16x2_send_cmd(handle, BITS_4 | LINES_2); // interface de 4 bits 2 linhas (aqui se habilita as 2 linhas)
    // são enviados os 2 nibbles (0x2 e 0x8)
    lcd16x2_send_cmd(handle, DISPLAY_ON | BLINK_CURSOR); // mensagem aparente cursor inativo não piscando

    lcd16x2_send_cmd(handle, RETURN_HOME);
    lcd16x2_send_cmd(handle, CLEAR_DISPLAY); // limpa todo o display

    return 0;
}

lcd16x2_err lcd16x2_send_cmd(const lcd16x2_handle *handle, uint8_t cmd)
{
    /* --- MSB --- */
    handle->d4.write(0);
    handle->d5.write(0);
    handle->d6.write(0);
    handle->d7.write(0);

    handle->d4.write((cmd >> 4) & 0x1);
    handle->d5.write((cmd >> 5) & 0x1);
    handle->d6.write((cmd >> 6) & 0x1);
    handle->d7.write((cmd >> 7) & 0x1);

    /* RS = 0 para comando */
    handle->rs.write(0);

    pulse_enable(handle);

    /* --- LSB --- */
    handle->d4.write(0);
    handle->d5.write(0);
    handle->d6.write(0);
    handle->d7.write(0);

    handle->d4.write((cmd >> 0) & 0x1);
    handle->d5.write((cmd >> 1) & 0x1);
    handle->d6.write((cmd >> 2) & 0x1);
    handle->d7.write((cmd >> 3) & 0x1);

    /* RS = 0 para comando */
    handle->rs.write(0);

    pulse_enable(handle);

    handle->delay_ms(2);

    return 0;
}

lcd16x2_err lcd16x2_send_data(const lcd16x2_handle *handle, uint8_t data)
{

    /* --- MSB --- */
    handle->d4.write(0);
    handle->d5.write(0);
    handle->d6.write(0);
    handle->d7.write(0);

    handle->d4.write((data >> 4) & 0x1);
    handle->d5.write((data >> 5) & 0x1);
    handle->d6.write((data >> 6) & 0x1);
    handle->d7.write((data >> 7) & 0x1);

    /* RS = 1 para dado */
    handle->rs.write(1);

    pulse_enable(handle);

    /* --- LSB --- */
    handle->d4.write(0);
    handle->d5.write(0);
    handle->d6.write(0);
    handle->d7.write(0);

    handle->d4.write((data >> 0) & 0x1);
    handle->d5.write((data >> 1) & 0x1);
    handle->d6.write((data >> 2) & 0x1);
    handle->d7.write((data >> 3) & 0x1);

    /* RS = 1 para dado */
    handle->rs.write(1);

    pulse_enable(handle);

    handle->delay_ms(1); // pode reduzir depois

    return 0;
}

void lcd16x2_write_string(const lcd16x2_handle *handle, const char *str, uint8_t size)
{

    for(uint8_t idx=0; idx<size; ++idx)
        lcd16x2_send_data(handle, str[idx]);
}

void pulse_enable(const lcd16x2_handle *handle)
{
    handle->delay_ms(1);

    handle->en.write(1);
    handle->delay_ms(2);
    handle->en.write(0);

    handle->delay_ms(1);
}