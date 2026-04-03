#include <memory.h>

#include <stm32f411xe.h>

#include "systick.h"
#include "inits_perifericos.h"

#include "drv8833.h"
#include "motor_dc.h"
#include "lcd16x2.h"

void TIM3_IRQHandler(void)
{
    // Estouro a cada 48ms
    // Limpar bit de evento
}

int main(void)
{
    rcc_enable_clocks();
    systick_init();
    gpio_lcd16x2_setup();

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
    lcd16x2_send_data(&lcd, 'A');

    char msg[7] = {"ANDRE\0"};

    while (1)
    {
        for (char *i = msg; *i != '\0'; i++)
        {
            lcd16x2_send_data(&lcd, *i);
            lcd.delay_ms(250);
        }
        lcd.delay_ms(1000);
        lcd16x2_send_cmd(&lcd, RETURN_HOME);
        lcd16x2_send_cmd(&lcd, CLEAR_DISPLAY);
        lcd.delay_ms(1000);
    }

    return 0;
}