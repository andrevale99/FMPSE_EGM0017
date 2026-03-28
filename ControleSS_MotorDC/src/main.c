#include <memory.h>

#include <stm32f411xe.h>

#include "inits.h"

#include "drv8833.h"

int main(void)
{
    drv8833_handle_t drv8833;
    memset(&drv8833, 0, sizeof(drv8833));
    
    rcc_enable_clocks();

    drv8833.in[0].config_gpio = gpio_drv8833_setup_in4;
    drv8833.in[0].set_state = gpio_set_state_in4;

    drv8833.in[1].config_gpio = gpio_drv8833_setup_in3;
    drv8833.in[1].set_state = gpio_set_state_in3;

    drv8833.sleep.config_gpio = gpio_drv8833_setup_sleep;
    drv8833.sleep.set_state = gpio_set_state_sleep;

    drv8833.pwm.config_pwm = pwm_init;
    drv8833.pwm.set_duty_cycle = pwm_set_duty;
    drv8833.pwm.maxDuty = 999;

    drv8833_init(&drv8833);

    drv8833_set_duty_cycle(&drv8833,10);

    drv8833.sleep.set_state(1);
    
    while (1)
    {
    }

    return 0;
}