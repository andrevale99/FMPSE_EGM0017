#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_err.h"

#include "system.h"


void app_main(void)
{
    system_init();

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
