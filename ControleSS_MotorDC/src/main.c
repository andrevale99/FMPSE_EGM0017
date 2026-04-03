#include <stdio.h>

#include <stm32f411xe.h>

#include "systick.h"
#include "inits_perifericos.h"

#include "drv8833.h"
#include "motor_dc.h"
#include "lcd16x2.h"

volatile uint32_t cnt = 0;

void TIM3_IRQHandler(void)
{
    cnt++;
    TIM3->SR &= ~TIM_SR_UIF;
    // Estouro aproximadamente em 49ms
    // Limpar bit de evento
}

int main(void)
{
    setup_system();

    lcd16x2_handle lcd = {
        .d4.write = write_d4,
        .d5.write = write_d5,
        .d6.write = write_d6,
        .d7.write = write_d7,

        .en.write = write_en,
        .rs.write = write_rs,

        .delay_ms = delay_ms,
    };

    lcd16x2_init_4bits(&lcd);

    char buffer[8] = {0};
    while (1)
    {
        snprintf(buffer, 8, "%d\0", cnt);

        for (char *i = buffer; *i != '\0'; i++)
            lcd16x2_send_data(&lcd, *i);

        delay_ms(1000);
        lcd16x2_send_cmd(&lcd, RETURN_HOME);
        lcd16x2_send_cmd(&lcd, CLEAR_DISPLAY);

    }

    return 0;
}