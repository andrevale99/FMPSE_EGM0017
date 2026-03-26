#include <stm32f411xe.h>

#include "drv8833.h"

int main(void)
{
    // Pinos A8 (TIM1_CH1) e A9 (TIM1_CH2)
    drv8833_config_t motorA = {
        .IN[0].gpioReg = GPIOA,
        .IN[0].in = 8,
        .IN[0].AlternateFuncSel = GPIO_AFRH_AFSEL8_0,
        .IN[0].OutputMode = (0x2 << GPIO_MODER_MODE8_Pos),
    
        .IN[1].in = DRV8833_DISCONNECT_GPIO,
        .IN[2].in = DRV8833_DISCONNECT_GPIO,
        .IN[3].in = DRV8833_DISCONNECT_GPIO,
    };

    drv8833_init(&motorA);

    return 0;
}