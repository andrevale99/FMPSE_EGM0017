#include <memory.h>

#include <stm32f411xe.h>

#include "inits.h"

#include "drv8833.h"

int main(void)
{
    drv8833_handle_t drv8833;
    memset(&drv8833, 0, sizeof(drv8833));
    
    drv8833.in[0].config_gpio = init_drv8833_channels;

    while (1)
    {
    }

    return 0;
}