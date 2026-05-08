#ifndef TIMER_ISR_H
#define TIMER_ISR_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gptimer.h"

#include "env.h"

#include "motor_dc.h"
#include "encoder.h"

typedef struct
{
    encoder_t *encoder;
    TaskHandle_t handletaskMotor;
} timer_isr_user_data_t;

static gptimer_handle_t handleTimer = NULL;

static timer_isr_user_data_t StaticData;

static bool IRAM_ATTR timer_alarm_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
{
    BaseType_t high_task_awoken = pdTRUE;

    timer_isr_user_data_t *data = (timer_isr_user_data_t *)user_data;

    // No sistema esta quadratura X4, logo, conta o dobro de pulsos
    int pulsos = (encoder_get_pulses(data->encoder)>>1);

    xTaskNotifyFromISR(
        data->handletaskMotor,
        pulsos,
        eSetValueWithOverwrite,
        &high_task_awoken);

    //calculo do controlador

    return high_task_awoken;
}

void timer_isr_init(timer_isr_user_data_t *userdata)
{
    memcpy(&StaticData, userdata, sizeof(timer_isr_user_data_t));
    
    gptimer_config_t config =
        {
            .clk_src = GPTIMER_CLK_SRC_DEFAULT,
            .direction = GPTIMER_COUNT_UP,
            .resolution_hz = TIMER_RESOLUTION_HZ, // 1 MHz = 1 us
        };

    ESP_ERROR_CHECK(gptimer_new_timer(&config, &handleTimer));

    gptimer_event_callbacks_t cbs = {
        .on_alarm = timer_alarm_callback,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(handleTimer, &cbs, &StaticData));

    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0,
        .alarm_count = FREQUENCIA_HZ_AMOSTRAGEM_ENCODER,
        .flags.auto_reload_on_alarm = true,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(handleTimer, &alarm_config));

    gptimer_enable(handleTimer);
    gptimer_start(handleTimer);
}

#endif