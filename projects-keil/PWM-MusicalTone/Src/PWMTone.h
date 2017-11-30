#pragma once

#include "stm32f1xx_hal.h"

static int Tone_index = 0;

float Tone_getNextNote(void);
void Tone_stop(TIM_HandleTypeDef *htim, int channel);
void Tone_start(TIM_HandleTypeDef *htim, int channel, float frequency);
void Tone_setFrequency(TIM_HandleTypeDef *htim, float frequency);
void Tone_setDutyCycle(TIM_HandleTypeDef *htim, int channel, int percentage);
