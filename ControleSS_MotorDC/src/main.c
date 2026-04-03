#include <stdio.h>

#include <stm32f411xe.h>

#include "systick.h"
#include "inits_perifericos.h"

#include "drv8833.h"
#include "motor_dc.h"
#include "lcd16x2.h"

volatile int32_t cnt = 0;

void TIM4_IRQHandler(void)
{
    if (TIM4->SR & TIM_SR_UIF)
    {
        cnt = (int16_t)TIM3->CNT;
        TIM3->CNT = 0;
        TIM4->SR &= ~TIM_SR_UIF;
        // Estouro aproximadamente em 49ms
        // Limpar bit de evento
    }
}

int main(void)
{
    setup_system();

    motor_dc_handle_t motor = {
        .m1.in[0].set_state = gpio_drv8833_set_state_in1,
        .m1.pwm.set_duty_cycle = tim1_channel1_pwm_set_duty,
        .m1.pwm.maxDuty = 999,
        .m1.sleep = gpio_dr8833_set_state_sleep,

        .m2.in[1].set_state = gpio_drv8833_set_state_in2,
        .m2.pwm.set_duty_cycle = tim1_channel2_pwm_set_duty,
        .m2.pwm.maxDuty = 999,
        .m2.sleep = gpio_dr8833_set_state_sleep,
    };

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

    motor_dc_pwm_channel(&motor, 0, 999);

    char buffer[8] = {0};
    while (1)
    {
        snprintf(buffer, 8, "%li", cnt);

        for (char *i = buffer; *i != '\0'; i++)
            lcd16x2_send_data(&lcd, *i);

        delay_ms(1000);
        lcd16x2_send_cmd(&lcd, RETURN_HOME);
        lcd16x2_send_cmd(&lcd, CLEAR_DISPLAY);
    }

    return 0;
}