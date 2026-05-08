#ifndef MOTOR_TASK_H
#define MOTOR_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "env.h"

#include "motor_dc.h"

static const char *TAG = "MOTOR_TASK";

static TaskHandle_t handleTaskMotor = NULL;
static pcnt_unit_handle_t pcnt_unit = NULL;

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

    .pulsos_por_voltas = MOTOR_DC_PULSOS_POR_VOLTA,
    .reducao = MOTOR_DC_REDUCAO,
};

static encoder_t encoder =
    {
        .handlePulseCounter = NULL,

        .pcnt_chan_a = NULL,
        .pcnt_chan_b = NULL,

        .iChannelA = ENCODER_CHN_A_GPIO,
        .iChannelB = ENCODER_CHN_B_GPIO,

        .iMaxCount = ENCODER_MAX_LIMIT,
        .iMinCount = ENCODER_LOW_LIMIT,

        .PosEdgeActionA = PCNT_CHANNEL_EDGE_ACTION_INCREASE,
        .NegEdgeActionA = PCNT_CHANNEL_EDGE_ACTION_HOLD,

        .HighLevelActionA = PCNT_CHANNEL_LEVEL_ACTION_KEEP,
        .LowLevelActionA = PCNT_CHANNEL_LEVEL_ACTION_INVERSE,

        .PosEdgeActionB = PCNT_CHANNEL_EDGE_ACTION_HOLD,
        .NegEdgeActionB = PCNT_CHANNEL_EDGE_ACTION_DECREASE,

        .HighLevelActionB = PCNT_CHANNEL_LEVEL_ACTION_HOLD,
        .LowLevelActionB = PCNT_CHANNEL_LEVEL_ACTION_KEEP,

        .u32GlitchFilter = 1000,
};

static void vTaskMotor(void *pvArgs)
{
    motor_dc_t *motor = (motor_dc_t *)pvArgs;

    esp_err_t check = motor_dc_init(motor);
    if (check != ESP_OK)
    {
        ESP_LOGE(TAG, "Erro ao iniciar o componente od motor");
    }

    uint32_t pulsosISR = 0;
    int32_t pulsos = 0;
    float RPM = 0.0;

    while (1)
    {
        xTaskNotifyWait(
            0x00,
            0xFFFFFFFF,
            &pulsosISR,
            portMAX_DELAY);

        motor_dc_set_movement(motor, MOTOR_DIR_FORWARD, 1023);

        pulsos = pulsosISR;

        RPM = MOTOR_DC_PULSOS_PARA_RPM(pulsos);
        // uint32_t RPM = motor_dc_get_rpm(&motor, pulsos, FREQUENCIA_MS_AMOSTRAGEM_ENCODER);
        ESP_LOGI(TAG, "Pulsos: %i, RPM: %.2f", (int32_t)pulsos, RPM);

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
    encoder.handlePulseCounter = pcnt_unit;

    ESP_ERROR_CHECK(encoder_init(&encoder));

    xTaskCreatePinnedToCore(vTaskMotor, "taskMotor", STACK_SIZE_TASK_MOTOR, (void *)&motor,
                            1, &handleTaskMotor, CPU_0);
}

/* Handle para timerISR */
TaskHandle_t motor_task_get_handle(void)
{
    return handleTaskMotor;
}

/* encoder_t para timerISR*/
encoder_t *motor_task_get_encoder_t(void)
{
    return &encoder;
}

#endif