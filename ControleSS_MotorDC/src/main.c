#include <stm32f411xe.h>

#include "drv8833.h"
#include "motor_dc.h"

int main(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN;

    // Pinos A8 (TIM1_CH1) e A9 (TIM1_CH2)
    drv8833_config_t drv883 = {
        .IN[0].gpioReg = GPIOA,
        .IN[0].in = 8,
        .IN[0].AlternateFuncSel = GPIO_AFRH_AFSEL8_0,
        .IN[0].OutputMode = (0x2 << GPIO_MODER_MODE8_Pos),

        .IN[1].in = DRV8833_DISCONNECT_GPIO,
        .IN[2].in = DRV8833_DISCONNECT_GPIO,
        .IN[3].in = DRV8833_DISCONNECT_GPIO,

        .SLEEP.gpioReg = GPIOB,
        .SLEEP.in = 2,
    };

    motor_dc_config_t motor = {
        .gpio = GPIOA,
        .timerX = TIM1,
        .freq = 10,
        .prescale = 1024,
    };

    drv8833_init(&drv883);
    drv8833_set_sleep_state(&drv883, 1);
    motor_dc_init(&motor);

    while (1)
    {
    }

    return 0;
}