#ifndef MOTOR_DC_H
#define MOTOR_DC_H

#include <stm32f411xe.h>

#include <stdint.h>

#include "drv8833.h"

#define RET_OK 0
#define RET_INVALID_ARG -1
#define RET_NO_GPIO -2

#define MOTOR_DC_CHANNELS 2

typedef int8_t motor_dc_err;

typedef struct
{
    drv8833_handle_t m1;
    drv8833_handle_t m2;
} motor_dc_handle_t;

motor_dc_err motor_dc_on_off(motor_dc_handle_t *motor, uint8_t state);

/**
 * @brief Controla o motor DC selecionando o canal e aplicando duty cycle.
 *
 * Define qual canal do driver será ativado e aplica o duty cycle
 * correspondente, garantindo que o outro canal seja DESLIGADO.
 *
 * @param[in,out] motor Ponteiro para a estrutura de controle do motor DC.
 * @param[in] channel Canal a ser ativado (0 ou 1).
 * @param[in] duty Valor do duty cycle a ser aplicado.
 *
 * @return
 *      - RET_OK: Operação realizada com sucesso.
 *      - RET_INVALID_ARG: Canal inválido.
 */
motor_dc_err motor_dc_pwm_channel(motor_dc_handle_t *motor, int8_t channel, uint32_t duty);

#endif