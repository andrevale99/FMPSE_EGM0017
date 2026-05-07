#ifndef SYS_QUEUES_H
#define SYS_QUEUES_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

QueueHandle_t queue_isr_motor_task = NULL;

void system_queue_init(void)
{
    
}


#endif