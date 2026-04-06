#include <stdio.h>

#include <stm32f411xe.h>

#include "systick.h"
#include "inits_perifericos.h"

#include "drv8833.h"
#include "motor_dc.h"
#include "lcd16x2.h"

volatile int32_t cnt = 0;
char buffer[18];

void write_interface(lcd16x2_handle *lcd);
void write_data(lcd16x2_handle *lcd);
void TIM4_IRQHandler(void);

int main(void)
{
    setup_system();

    motor_dc_handle_t motor = {
        .m1 = {
            .in[0].set_state = gpio_drv8833_set_state_in1,
            .pwm.set_duty_cycle = tim1_channel1_pwm_set_duty,
            .pwm.maxDuty = 999,
            .sleep = gpio_dr8833_set_state_sleep},

        .m2 = {
            .in[1].set_state = gpio_drv8833_set_state_in2,
            .pwm.set_duty_cycle = tim1_channel2_pwm_set_duty,
            .pwm.maxDuty = 999,
            .sleep = gpio_dr8833_set_state_sleep,
        },
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

    motor_dc_pwm_channel(&motor, 1, 999);

    write_interface(&lcd);

    while (1)
    {
        write_data(&lcd);
        delay_ms(1000);  
    }
    //204 pulsos em 50 ms *165 rpm
    return 0;
}

void write_interface(lcd16x2_handle *lcd)
{
    uint8_t size = 0;

    size = sprintf(buffer, "Pulsos:", cnt);
    lcd16x2_write_string(lcd, buffer, size);

    lcd16x2_send_cmd(lcd, SECOND_LINE);
    size = sprintf(buffer, "RPM:", cnt);
    lcd16x2_write_string(lcd, buffer, size);

    lcd16x2_send_cmd(lcd, RETURN_HOME);
}

void write_data(lcd16x2_handle *lcd)
{
    uint8_t size = 0;

    size = sprintf(buffer, "%li ", cnt);
    lcd16x2_send_cmd(lcd, SET_DDRAM | 0x08);
    lcd16x2_write_string(lcd, buffer, size);

    lcd16x2_send_cmd(lcd, SECOND_LINE | 0x5);
    size = sprintf(buffer, "%li ", cnt);
    lcd16x2_write_string(lcd, buffer, size);

    lcd16x2_send_cmd(lcd, RETURN_HOME);
}

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