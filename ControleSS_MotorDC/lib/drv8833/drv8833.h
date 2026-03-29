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
    void (*config_gpio)(void);
    void (*set_state)(uint8_t level);
} drv8833_in_config_t;

typedef struct
{
    void (*config_gpio)(void);
    void (*set_state)(uint8_t level);
} drv8833_sleep_t;

typedef struct
{
    void (*config_pwm)(void);
    void (*set_duty_cycle)(uint32_t duty);
    uint32_t maxDuty;
} drv8833_pwm_config_t;

typedef struct
{
    drv8833_in_config_t in;
    drv8833_pwm_config_t pwm;
} drv8833_handle_t;

/**
 * @brief Inicializa os pinos de entrada e PWM do driver DRV8833.
 *
 * Configura os GPIOs de controle (INx), define o estado inicial como baixo
 * e inicializa o PWM caso esteja configurado.
 *
 * @param[in,out] handle Ponteiro para a estrutura de controle do DRV8833.
 *
 * @return
 *      - RET_OK: Inicialização realizada com sucesso.
 *      - RET_INVALID_ARG: Ponteiro inválido ou funções não configuradas.
 */
drv8833_err drv8833_in_init(drv8833_handle_t *handle);

/**
 * @brief Inicializa o pino de controle de sleep do DRV8833.
 *
 * Configura o GPIO responsável pelo controle de sleep e define
 * o estado inicial como desabilitado.
 *
 * @param[in,out] sleep_t Ponteiro para a estrutura de controle do pino sleep.
 *
 * @return
 *      - RET_OK: Inicialização realizada com sucesso.
 *      - RET_INVALID_ARG: Ponteiro inválido ou funções não configuradas.
 */
drv8833_err drv8833_sleep_init(drv8833_sleep_t *sleep_t);

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
drv8833_err drv8833_set_sleep_state(drv8833_sleep_t *sleepGPIO, uint8_t state);

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
drv8833_err drv8833_set_duty_cycle(drv8833_handle_t *handle, uint32_t duty);

#endif