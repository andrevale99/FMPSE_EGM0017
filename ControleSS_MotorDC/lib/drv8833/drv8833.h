#ifndef DRV_8833_H
#define DRV_8833_H

#include <stdint.h>
#include <stddef.h>
#include <stm32f411xe.h>

#define RET_OK 0
#define RET_INVALID_ARG -1
#define RET_NO_GPIO -2

#define DRV8833_NUM_CHANNELS 4

typedef int8_t drv8833_err;

typedef struct
{
    void (*set_state)(uint8_t level);
} drv8833_gpio_config_t;

typedef struct
{
    void (*set_duty_cycle)(uint32_t duty);
    uint32_t maxDuty;
} drv8833_pwm_config_t;

typedef struct
{
    drv8833_gpio_config_t in[DRV8833_NUM_CHANNELS];
    drv8833_gpio_config_t sleep;
    drv8833_pwm_config_t pwm;
} drv8833_handle_t;


drv8833_err drv8833_set_in_level(drv8833_handle_t *drv8833, uint8_t channel, uint8_t state);

/**
 * @brief Define o estado do pino sleep do DRV8833.
 *
 * @param[in] sleepGPIO Ponteiro para a estrutura de controle do pino sleep.
 * @param[in] state Estado a ser aplicado (0 ou 1).
 *
 * @return
 *      - RET_OK: Estado definido com sucesso.
 *      - RET_INVALID_ARG: Ponteiro inválido.
 */
drv8833_err drv8833_set_sleep_state(drv8833_handle_t *drv8833, uint8_t state);

/**
 * @brief Define o duty cycle do sinal PWM do DRV8833.
 *
 * Ajusta o duty cycle respeitando o valor máximo configurado.
 *
 * @param[in,out] handle Ponteiro para a estrutura de controle do DRV8833.
 * @param[in] duty Valor do duty cycle desejado.
 *
 * @return
 *      - RET_OK: Duty cycle configurado com sucesso.
 *      - RET_INVALID_ARG: Configuração de PWM inválida.
 */
drv8833_err drv8833_set_duty_cycle(drv8833_handle_t *drv8833, uint32_t duty);

#endif