#include "motor_dc.h"

#define DELAY_DECAY_ACCELERATE 100
#define STEP_DELAY_DECAY 10

static const char *TAG = "MOTOR_DC";

esp_err_t motor_dc_init(motor_dc_t *motor)
{
    esp_err_t xErrCheck = ESP_FAIL;

    ESP_LOGI(TAG, "Iniciando motor_dc...");

    if (!motor)
    {
        ESP_LOGE(TAG, "Estrutura Vazia motor_t");
        xErrCheck = ESP_ERR_INVALID_ARG;
        return xErrCheck;
    }

    ESP_LOGI(TAG, "Iniciando PWMTimer no timer: %d", motor->PwmTimer);
    xErrCheck = ledc_timer_config(&motor->PwmTimer);
    if (xErrCheck != ESP_OK)
    {
        ESP_LOGE(TAG, "Erro ao configurar o TIMER %d, verificar estutura PwmTimer",
                 motor->PwmTimer);
        motor_dc_deinit(motor);
        return xErrCheck;
    }

    if (motor->M1Channel == motor->M2Channel)
    {
        ESP_LOGE(TAG, "Mesmo canal PWM para M1 (chn: %d) e M2 (chn: %d)",
                 motor->M1Channel, motor->M2Channel);

        motor_dc_deinit(motor);

        return ESP_ERR_INVALID_ARG;
    }

    ledc_channel_config_t configM1 = {
        .channel = motor->M1Channel,
        .timer_sel = motor->PwmTimer.timer_num,
        .speed_mode = motor->PwmTimer.speed_mode,
        .gpio_num = motor->M1GPIO,
        .duty = 0,
        .hpoint = 0,
    };

    ESP_LOGI(TAG, "Configurando o Canal M1, GPIO: %d", motor->M1GPIO);
    xErrCheck = ledc_channel_config(&configM1);
    if (xErrCheck != ESP_OK)
    {
        ESP_LOGE(TAG, "Erro ao instalar o canal do PWM no GPIo %d",
                 motor->M1GPIO);

        motor_dc_deinit(motor);
        return xErrCheck;
    }
    ESP_LOGI(TAG, "Canal M1 configurado");

    ledc_channel_config_t configM2 = {
        .channel = motor->M2Channel,
        .timer_sel = motor->PwmTimer.timer_num,
        .speed_mode = motor->PwmTimer.speed_mode,
        .gpio_num = motor->M2GPIO,
        .duty = 0,
        .hpoint = 0,
    };

    ESP_LOGI(TAG, "Configurando o Canal M2, GPIO: %d", motor->M2GPIO);
    xErrCheck = ledc_channel_config(&configM2);
    if (xErrCheck != ESP_OK)
    {
        ESP_LOGE(TAG, "Erro ao instalar o canal do PWM no GPIo %d",
                 motor->M2GPIO);

        motor_dc_deinit(motor);
        return xErrCheck;
    }
    ESP_LOGI(TAG, "Canal M2 configurado");

    ESP_LOGI(TAG, "motor_dc_t configurado");

    return ESP_OK;
}

esp_err_t motor_dc_deinit(motor_dc_t *motor)
{
    if (!motor)
    {
        ESP_LOGE(TAG, "Estrutura motor_dc_t nula");
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGI(TAG, "Finalizando motor_dc...");

    ledc_set_duty(motor->PwmTimer.speed_mode, motor->M1Channel, 0);
    ledc_update_duty(motor->PwmTimer.speed_mode, motor->M1Channel);

    ledc_set_duty(motor->PwmTimer.speed_mode, motor->M2Channel, 0);
    ledc_update_duty(motor->PwmTimer.speed_mode, motor->M2Channel);

    ledc_stop(motor->PwmTimer.speed_mode, motor->M1Channel, 0);
    ledc_stop(motor->PwmTimer.speed_mode, motor->M2Channel, 0);

    ESP_LOGI(TAG, "motor_dc finalizado com sucesso");

    return ESP_OK;
}

esp_err_t motor_dc_set_duty(motor_dc_t *motor, int gpio, uint32_t duty)
{
    if (duty > (1 << motor->PwmTimer.duty_resolution))
    {
        duty = (1 << motor->PwmTimer.duty_resolution) - 1;
        ESP_LOGW(TAG, "Estouro no Duty Cycle, colocando para o maximo: %d", duty);
    }

    if (gpio == motor->M1GPIO)
    {
        ledc_set_duty(motor->PwmTimer.speed_mode, motor->M1Channel, duty);
        ledc_update_duty(motor->PwmTimer.speed_mode, motor->M1Channel);

        ledc_set_duty(motor->PwmTimer.speed_mode, motor->M2Channel, 0);
        ledc_update_duty(motor->PwmTimer.speed_mode, motor->M2Channel);
    }
    else if (gpio == motor->M2GPIO)
    {
        ledc_set_duty(motor->PwmTimer.speed_mode, motor->M1Channel, 0);
        ledc_update_duty(motor->PwmTimer.speed_mode, motor->M1Channel);

        ledc_set_duty(motor->PwmTimer.speed_mode, motor->M2Channel, duty);
        ledc_update_duty(motor->PwmTimer.speed_mode, motor->M2Channel);
    }
    else
    {
        ESP_LOGE(TAG, "Nenhuma GPIO vinculado, verificar estrutura motor_dc_t");
        return ESP_ERR_INVALID_ARG;
    }

    return ESP_OK;
}

int motor_dc_get_duty(int gpio, motor_dc_t *motor)
{
    if (gpio == motor->M1GPIO)
        return ledc_get_duty(motor->PwmTimer.speed_mode, motor->M1Channel);
    else if (gpio == motor->M2GPIO)
        return ledc_get_duty(motor->PwmTimer.speed_mode, motor->M2Channel);
    else
    {
        ESP_LOGE(TAG, "Nenhum Canal/GPIO foi configurado");
        return -1;
    }
}

esp_err_t motor_dc_set_movement(motor_dc_t *motor, motor_dir_t move, uint32_t duty)
{
    esp_err_t xErrCheck = ESP_OK;

    int step = 0;
    uint32_t current_duty = 0;
    uint32_t delta = 0;
    uint32_t newDuty = 0;

    switch (move)
    {
    case MOTOR_DIR_FORWARD:
        motor_dc_set_duty(motor, motor->M1GPIO, duty);
        break;

    case MOTOR_DIR_BACKWARD:
        motor_dc_set_duty(motor, motor->M2GPIO, duty);
        break;

    case MOTOR_DIR_BRAKE:
        motor_dc_set_duty(motor, motor->M1GPIO, 0);
        break;

    case MOTOR_DIR_DECAY_FORWARD:
        step = motor_dc_get_duty(motor->M1GPIO, motor) / STEP_DELAY_DECAY;
        for (int i = STEP_DELAY_DECAY; i > 0; i--)
        {
            duty = step * (i - 1);
            motor_dc_set_duty(motor, motor->M1GPIO, duty);
            vTaskDelay(pdMS_TO_TICKS(DELAY_DECAY_ACCELERATE));
        }
        break;

    case MOTOR_DIR_DECAY_BACKWARD:
        step = motor_dc_get_duty(motor->M2GPIO, motor) / STEP_DELAY_DECAY;
        for (int i = STEP_DELAY_DECAY; i > 0; i--)
        {
            duty = step * (i - 1);
            motor_dc_set_duty(motor, motor->M2GPIO, duty);
            vTaskDelay(pdMS_TO_TICKS(DELAY_DECAY_ACCELERATE));
        }
        break;

    case MOTOR_DIR_ACCELERATE_FORWARD:
        current_duty = motor_dc_get_duty(motor->M1GPIO, motor);

        if (duty > current_duty)
        {
            delta = duty - current_duty;

            for (int i = 1; i <= STEP_DELAY_DECAY; i++)
            {
                newDuty = current_duty +
                          (delta * i * i) / (STEP_DELAY_DECAY * STEP_DELAY_DECAY); // rampa suave (quadrática)

                motor_dc_set_duty(motor, motor->M1GPIO, newDuty);
                vTaskDelay(pdMS_TO_TICKS(DELAY_DECAY_ACCELERATE));
            }
        }
        else
        {
            ESP_LOGW(TAG, "Duty inferior ao atual, utilizar MOTOR_DIR_DECAY_x");
            xErrCheck = ESP_ERR_INVALID_ARG;
        }
        break;

    case MOTOR_DIR_ACCELERATE_BACKWARD:
        current_duty = motor_dc_get_duty(motor->M2GPIO, motor);

        if (duty > current_duty)
        {
            delta = duty - current_duty;

            for (int i = 1; i <= STEP_DELAY_DECAY; i++)
            {
                newDuty = current_duty +
                          (delta * i * i) / (STEP_DELAY_DECAY * STEP_DELAY_DECAY); // rampa suave (quadrática)

                motor_dc_set_duty(motor, motor->M2GPIO, newDuty);
                vTaskDelay(pdMS_TO_TICKS(DELAY_DECAY_ACCELERATE));
            }
        }
        else
        {
            ESP_LOGW(TAG, "Duty inferior ao atual, utilizar MOTOR_DIR_DECAY_x");
            xErrCheck = ESP_ERR_INVALID_ARG;
        }
        break;

    default:
        ESP_LOGE(TAG, "Movimento acao nao reconhecido");
        xErrCheck = ESP_ERR_INVALID_ARG;
        break;
    }

    return xErrCheck;
}