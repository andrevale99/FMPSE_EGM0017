#include "system.h"
#include "env.h"
#include "encoder.h"

#include "timerISR.h"

#include "motor_task.h"

#include "driver/gptimer.h"
#include "driver/ledc.h"

void system_init(void)
{
    motor_task_init();

    timer_isr_user_data_t dataArgs;

    dataArgs.encoder = motor_task_get_encoder_t(),
    dataArgs.handletaskMotor = motor_task_get_handle(),
    
    timer_isr_init(&dataArgs);
}