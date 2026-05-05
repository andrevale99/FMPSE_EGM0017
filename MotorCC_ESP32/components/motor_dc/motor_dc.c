#include "motor_dc.h"

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
        return xErrCheck;
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
        return xErrCheck;
    }
    ESP_LOGI(TAG, "Canal M2 configurado");

    ESP_LOGI(TAG, "motor_dc_t configurado");

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