#ifndef MOTOR_TASK_H
#define MOTOR_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "env.h"

#include "motor_dc.h"

static const char *TAG = "MOTOR_TASK";

static TaskHandle_t handleTaskMotor = NULL;

static motor_dc_t motor = {
    .PwmTimer = {
        .clk_cfg = MOTOR_DC_CLOCK_SOURCE,
        .speed_mode = MOTOR_DC_SPEED,
        .timer_num = MOTOR_DC_PWM_TIMER,
        .freq_hz = MOTOR_DC_FREQ,
        .duty_resolution = MOTOR_DC_RESOLUTION_PWM,
    },

    .M1Channel = MOTOR_DC_M1_CHANNEL,
    .M1GPIO = MOTOR_DC_M1_GPIO,

    .M2Channel = MOTOR_DC_M2_CHANNEL,
    .M2GPIO = MOTOR_DC_M2_GPIO,

    .pulsos_por_voltas = 11,
    .reducao = 33.8,
};

static void vTaskMotor(void *pvArgs)
{
    esp_err_t check = motor_dc_init(&motor);
    if (check != ESP_OK)
    {
        ESP_LOGE(TAG, "Erro ao iniciar o componente od motor");
    }

    uint32_t valorRecebido;

    while (1)
    {
        xTaskNotifyWait(
            0x00,
            0xFFFFFFFF,
            &valorRecebido,
            portMAX_DELAY);
        
        ESP_LOGI(TAG, "Pulsos: %d", valorRecebido);

        // motor_dc_set_movement(&motor, MOTOR_DIR_ACCELERATE_FORWARD, 1023);
        // vTaskDelay(pdMS_TO_TICKS(1000));
        // motor_dc_set_movement(&motor, MOTOR_DIR_DECAY_FORWARD, 1023);
        // vTaskDelay(pdMS_TO_TICKS(1000));

        // motor_dc_set_movement(&motor, MOTOR_DIR_ACCELERATE_BACKWARD, 1023);
        // vTaskDelay(pdMS_TO_TICKS(1000));
        // motor_dc_set_movement(&motor, MOTOR_DIR_DECAY_BACKWARD, 1023);
        // vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void motor_task_init(void)
{
    xTaskCreatePinnedToCore(vTaskMotor, "taskMotor", STACK_SIZE_TASK_MOTOR, NULL,
                            1, &handleTaskMotor, CPU_0);
}

/* Getter para ISR */
TaskHandle_t motor_task_get_handle(void)
{
    return handleTaskMotor;
}

#endif