#ifndef ENCODER_H
#define ENCODER_H

#include "driver/pulse_cnt.h"
#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_err.h"

typedef struct
{
    pcnt_unit_handle_t handlePulseCounter;

    pcnt_channel_handle_t pcnt_chan_a;
    pcnt_channel_handle_t pcnt_chan_b;

    int iMaxCount;
    int iMinCount;

    int iChannelA;
    int iChannelB;

    uint32_t u32GlitchFilter;

    pcnt_channel_edge_action_t PosEdgeActionA;
    pcnt_channel_edge_action_t NegEdgeActionA;
    pcnt_channel_level_action_t HighLevelActionA;
    pcnt_channel_level_action_t LowLevelActionA;

    pcnt_channel_edge_action_t PosEdgeActionB;
    pcnt_channel_edge_action_t NegEdgeActionB;
    pcnt_channel_level_action_t HighLevelActionB;
    pcnt_channel_level_action_t LowLevelActionB;
} encoder_t;

esp_err_t encoder_init(encoder_t *config);

esp_err_t encoder_deinit(encoder_t *config);

uint8_t encoder_canais_ativos(encoder_t *encoder);

int encoder_get_pulses(encoder_t *encoder);

#endif