#ifndef MOTOR_DC
#define MOTOR_DC

#include "driver/ledc.h"

#include "esp_err.h"
#include "esp_log.h"

typedef enum {
    MOTOR_DIR_FORWARD,
    MOTOR_DIR_BACKWARD,
    MOTOR_DIR_BRAKE,
    MOTOR_DIR_COAST
} motor_dir_t;

typedef struct
{
    ledc_timer_config_t PwmTimer;

    ledc_channel_t M1Channel;
    ledc_channel_t M2Channel;

    int M1GPIO;
    int M2GPIO;

    int pulsos_por_voltas;
    float reducao;
} motor_dc_t;

esp_err_t motor_dc_init(motor_dc_t *motor);

esp_err_t motor_dc_set_duty(motor_dc_t *motor, int gpio, uint32_t duty);

int motor_dc_get_duty(int gpio, motor_dc_t *motor);

#endif