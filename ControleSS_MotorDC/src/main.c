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
}

int main(void)
{
    drv8833_handle_t drv8833;
    memset(&drv8833, 0, sizeof(drv8833));

    drv8833_sleep_t sleepGPIO = {
        .config_gpio = gpio_drv8833_setup_sleep,
        .set_state = gpio_set_state_sleep,
    };

    rcc_enable_clocks();

    motor_dc_handle_t motor = {
        .m1 =
            {
                .in.config_gpio = gpio_drv8833_setup_in1,
                .in.set_state = gpio_set_state_in1,

                .pwm.config_pwm = pwm_init,
                .pwm.set_duty_cycle = tim1_channel1_pwm_set_duty,
                .pwm.maxDuty = 999,
            },

        .m2 = {
            .in.config_gpio = gpio_drv8833_setup_in2,
            .in.set_state = gpio_set_state_in2,

            .pwm.config_pwm = pwm_init,
            .pwm.set_duty_cycle = tim1_channel2_pwm_set_duty,
            .pwm.maxDuty = 999,
        },
    };

    motor_dc_init(&motor);
    drv8833_sleep_init(&sleepGPIO);

    drv8833_set_sleep_state(&sleepGPIO, 1);

    while (1)
    {
    }

    return 0;
}