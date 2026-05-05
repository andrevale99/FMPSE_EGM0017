#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gptimer.h"

#include "esp_log.h"
#include "esp_err.h"

#include "encoder.h"
#include "motor_dc.h"

#include "defs.h"

const char *TAG = "MAIN";

volatile int Atualpulsos = 0;
volatile int Anteriorpulsos = 0;
volatile uint8_t flagPrint = false;

gptimer_handle_t handleTimer = NULL;

static bool IRAM_ATTR timer_alarm_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
{
    BaseType_t high_task_awoken = pdTRUE;

    encoder_config_t *encoder = (encoder_config_t *)user_data;

    pcnt_unit_get_count(encoder->handlePulseCounter, &Atualpulsos);

    flagPrint = true;

    pcnt_unit_clear_count(encoder->handlePulseCounter);

    return high_task_awoken;
}

void app_main(void)
{
    encoder_config_t EncoderConfig =
        {
            // .handlePulseCounter = pcnt_unit,

            .pcnt_chan_a = NULL,
            .pcnt_chan_b = NULL,

            .iChannelA = ENCODER_CHN_A_GPIO,
            .iChannelB = -1,

            .iMaxCount = ENCODER_MAX_LIMIT,
            .iMinCount = ENCODER_LOW_LIMIT,

            .PosEdgeActionA = PCNT_CHANNEL_EDGE_ACTION_HOLD,
            .NegEdgeActionA = PCNT_CHANNEL_EDGE_ACTION_INCREASE,

            .HighLevelActionA = PCNT_CHANNEL_LEVEL_ACTION_HOLD,
            .LowLevelActionA = PCNT_CHANNEL_LEVEL_ACTION_KEEP,

            .u32GlitchFilter = 1000,
        };

    encoder_init(&EncoderConfig);

    motor_dc_t motor = {
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
    };

    motor_dc_init(&motor);

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
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(handleTimer, &cbs, &EncoderConfig));

    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0,
        .alarm_count = 50000,
        .flags.auto_reload_on_alarm = true,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(handleTimer, &alarm_config));

    gptimer_enable(handleTimer);
    gptimer_start(handleTimer);

    while (1)
    {
        if (flagPrint)
        {
            ESP_LOGI(TAG, "Pulsos: %i", Atualpulsos);
            flagPrint = false;
        }
        
        motor_dc_set_duty(&motor, motor.M1GPIO, 256);
        vTaskDelay(pdMS_TO_TICKS(1000));
        motor_dc_set_duty(&motor, motor.M1GPIO, 512);
        vTaskDelay(pdMS_TO_TICKS(1000));
        motor_dc_set_duty(&motor, motor.M1GPIO, 1023);
        vTaskDelay(pdMS_TO_TICKS(1000));


        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
