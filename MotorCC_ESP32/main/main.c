#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gptimer.h"

#include "esp_log.h"
#include "esp_err.h"

#include "encoder.h"
#include "motor_dc.h"

#include "defs.h"
#include "timerISR.h"

const char *TAG = "MAIN";

int Atualpulsos = 0;
volatile uint8_t flagPrint = false;

pcnt_unit_handle_t pcnt_unit = NULL;

void app_main(void)
{
    encoder_t encoder =
        {
            .handlePulseCounter = pcnt_unit,

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

    encoder_init(&encoder);

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

        .pulsos_por_voltas = 11,
        .reducao = 33.8,
    };

    motor_dc_init(&motor);

    timer_isr_user_data_t dataArgs =
        {
            .encoder = encoder,
        };

    timer_isr_init(&dataArgs);

    while (1)
    {
        motor_dc_set_movement(&motor, MOTOR_DIR_FORWARD, 1023);
        motor_dc_set_movement(&motor, MOTOR_DIR_DECAY_FORWARD, 1023);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
