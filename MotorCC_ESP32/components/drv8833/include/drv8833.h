#ifndef DRV8833_H
#define DRV8833_H

#include "driver/gpio.h"
#include "driver/ledc.h"

#include "esp_err.h"
#include "esp_log.h"

#define DRV8833_NUM_CHANNELS 4


typedef struct
{
    gpio_config_t in[DRV8833_NUM_CHANNELS];
    gpio_config_t sleep;
    gpio_config_t fault;
    ledc_timer_config_t PwmTimer;

}drv8833_t;


esp_err_t drv8833_init(drv8833_t *drv);

esp_err_t drv8833_set_in_level(drv8833_t *drv, uint8_t level);

esp_err_t drv8833_set_in_pwm(drv8833_t *drv, uint32_t duty);

esp_err_t drv8833_set_sleep_level(drv8833_t *drv, uint8_t level);

int drv8833_get_fault_situation(drv8833_t *drv);


#endif