#include "encoder.h"

static const char *TAG = "ENCODER";

esp_err_t encoder_init(encoder_config_t *config)
{

    /*Variavel de verificacao de erro*/
    esp_err_t xErrReturn = ESP_FAIL;

    if (!config)
    {
        xErrReturn = ESP_ERR_INVALID_ARG;
        ESP_LOGE(TAG, "*config nao inicializado: %s", esp_err_to_name(xErrReturn));
        return xErrReturn;
    }

    if (config->handlePulseCounter != NULL)
    {
        xErrReturn = ESP_ERR_INVALID_ARG;
        ESP_LOGE(TAG, "*Handle Ja inicializado: %s", esp_err_to_name(xErrReturn));
        return xErrReturn;
    }

    pcnt_unit_config_t unit_config = {
        .high_limit = config->iMaxCount,
        .low_limit = config->iMinCount,
    };

    xErrReturn = pcnt_new_unit(&unit_config, &config->handlePulseCounter);

    if (xErrReturn != ESP_OK)
    {
        ESP_LOGE(TAG, "Erro ao crair uma unidade pulse_counter: %s", esp_err_to_name(xErrReturn));
        return xErrReturn;
    }
    ESP_LOGI(TAG, "Unidade pulse_counter CRIADA");

    if (config->u32GlitchFilter > 0)
    {
        pcnt_glitch_filter_config_t filter_config = {
            .max_glitch_ns = config->u32GlitchFilter,
        };

        xErrReturn = pcnt_unit_set_glitch_filter(config->handlePulseCounter, &filter_config);

        if (xErrReturn != ESP_OK)
            ESP_LOGE(TAG, "Erro ao instalar o Glitch Filter: %s", esp_err_to_name(xErrReturn));
        
        ESP_LOGI(TAG, "Glitch Filter INICIALIZADO");
    }
    else
        ESP_LOGW(TAG, "Glitch Filter NAO inicializado");

    if (config->iChannelA >= 0)
    {
        ESP_LOGI(TAG, "Criando canal A...");

        pcnt_chan_config_t chan_a_config = {
            .edge_gpio_num = config->iChannelA,
            .level_gpio_num = config->iChannelB,
        };

        config->pcnt_chan_a = NULL;
        xErrReturn = pcnt_new_channel(config->handlePulseCounter, &chan_a_config, &config->pcnt_chan_a);
        if (xErrReturn != ESP_OK)
        {
            ESP_LOGE(TAG, "Erro ao configurar ao INICIAR o canal A: %s",
                     esp_err_to_name(xErrReturn));

            encoder_deinit(config);
            return xErrReturn;
        }
        ESP_LOGI(TAG, "Canal A INICIALIZADO");

        xErrReturn = pcnt_channel_set_edge_action(config->pcnt_chan_a,
                                                  config->PosEdgeActionA,
                                                  config->NegEdgeActionA);
        if (xErrReturn != ESP_OK)
        {
            ESP_LOGE(TAG, "Erro ao configurar Configurar os eventos de EDGE do canal A: %s",
                     esp_err_to_name(xErrReturn));

            encoder_deinit(config);
            return xErrReturn;
        }
        ESP_LOGI(TAG, "Configuracao dos eventos EDGE INICIALIZADAS");

        xErrReturn = pcnt_channel_set_level_action(config->pcnt_chan_a,
                                                   config->HighLevelActionA,
                                                   config->LowLevelActionA);

        if (xErrReturn != ESP_OK)
        {
            ESP_LOGE(TAG, "Erro ao configurar Configurar os eventos de LEVEL do canal A: %s",
                     esp_err_to_name(xErrReturn));

            encoder_deinit(config);
            return xErrReturn;
        }
        ESP_LOGI(TAG, "Configuracao dos eventos LEVEL INICIALIZADAS");

        ESP_LOGI(TAG, "Canal A Configurado");
    }
    else
        ESP_LOGW(TAG, "Canal A NAO configurado");

    if (config->iChannelB >= 0)
    {
        pcnt_chan_config_t chan_b_config = {
            .edge_gpio_num = config->iChannelB,
            .level_gpio_num = config->iChannelA,
        };

        xErrReturn = pcnt_new_channel(config->handlePulseCounter,
                                      &chan_b_config,
                                      &config->pcnt_chan_b);
        if (xErrReturn != ESP_OK)
        {
            ESP_LOGE(TAG, "Erro ao INICIAR o canal B: %s",
                     esp_err_to_name(xErrReturn));
            encoder_deinit(config);
            return xErrReturn;
        }

        xErrReturn = pcnt_channel_set_edge_action(config->pcnt_chan_b,
                                                  config->PosEdgeActionB,
                                                  config->NegEdgeActionB);
        if (xErrReturn != ESP_OK)
        {
            ESP_LOGE(TAG, "Erro ao configurar EDGE do canal B: %s",
                     esp_err_to_name(xErrReturn));
            encoder_deinit(config);
            return xErrReturn;
        }

        xErrReturn = pcnt_channel_set_level_action(config->pcnt_chan_b,
                                                   config->HighLevelActionB,
                                                   config->LowLevelActionB);
        if (xErrReturn != ESP_OK)
        {
            ESP_LOGE(TAG, "Erro ao configurar LEVEL do canal B: %s",
                     esp_err_to_name(xErrReturn));
            encoder_deinit(config);
            return xErrReturn;
        }

        ESP_LOGI(TAG, "Canal B Configurado");
    }
    else

        ESP_LOGW(TAG, "Canal B NAO configurado");

    if (config->pcnt_chan_a == NULL && config->pcnt_chan_b == NULL)
    {
        xErrReturn = ESP_ERR_INVALID_ARG;
        ESP_LOGE(TAG, "NENHUM canal foi configurado: %s", esp_err_to_name(xErrReturn));
        return xErrReturn;
    }

    ESP_LOGI(TAG, "Ativando o Pulse Counter");

    xErrReturn = pcnt_unit_enable(config->handlePulseCounter);
    if (xErrReturn != ESP_OK)
    {
        encoder_deinit(config);
        ESP_LOGE(TAG, "Erro ao habilitar a unidade: %s", esp_err_to_name(xErrReturn));
        return xErrReturn;
    }

    ESP_LOGI(TAG, "Limpando contador");

    xErrReturn = pcnt_unit_clear_count(config->handlePulseCounter);
    if (xErrReturn != ESP_OK)
    {
        encoder_deinit(config);
        ESP_LOGE(TAG, "Erro ao habilitar a unidade: %s", esp_err_to_name(xErrReturn));
        return xErrReturn;
    }

    ESP_LOGI(TAG, "INICIANDO contador");
    xErrReturn = pcnt_unit_start(config->handlePulseCounter);
    if (xErrReturn != ESP_OK)
    {
        encoder_deinit(config);
        ESP_LOGE(TAG, "Erro ao habilitar a unidade: %s", esp_err_to_name(xErrReturn));
        return xErrReturn;
    }

    ESP_LOGI(TAG, "Encoder INICIALIZADO");

    return ESP_OK;
}

esp_err_t encoder_deinit(encoder_config_t *config)
{
    if (!config || !config->handlePulseCounter)
        return ESP_ERR_INVALID_ARG;

    pcnt_unit_stop(config->handlePulseCounter);
    pcnt_unit_disable(config->handlePulseCounter);

    if (config->pcnt_chan_a)
    {
        pcnt_del_channel(config->pcnt_chan_a);
        config->pcnt_chan_a = NULL;
    }

    if (config->pcnt_chan_b)
    {
        pcnt_del_channel(config->pcnt_chan_b);
        config->pcnt_chan_b = NULL;
    }

    pcnt_del_unit(config->handlePulseCounter);
    config->handlePulseCounter = NULL;

    return ESP_OK;
}

uint8_t encoder_canais_ativos(encoder_config_t *encoder)
{
    uint8_t ui8CanaisAtivos = 0;

    if (encoder->pcnt_chan_a)
        ui8CanaisAtivos++;

    if (encoder->pcnt_chan_b)
        ui8CanaisAtivos++;

    return ui8CanaisAtivos;
}