#include "system.h"
#include "env.h"
#include "encoder.h"

#include "timerISR.h"

#include "motor_task.h"

#include "driver/gptimer.h"
#include "driver/ledc.h"

int Atualpulsos = 0;
volatile uint8_t flagPrint = false;

pcnt_unit_handle_t pcnt_unit = NULL;

void system_init(void)
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

    motor_task_init();

    timer_isr_user_data_t dataArgs =
        {
            .encoder = &encoder,
        };

    dataArgs.handletaskMotor = motor_task_get_handle();

    timer_isr_init(&dataArgs);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}