#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_err.h"

#include "encoder.h"

#include "env.h"
#include "system.h"
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

    timer_isr_user_data_t dataArgs =
        {
            .encoder = encoder,
        };

    timer_isr_init(&dataArgs);

    system_init();

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
