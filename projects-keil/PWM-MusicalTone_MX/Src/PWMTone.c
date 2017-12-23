#include "PWMTone.h"

const float ToneFreq[] = {
    1046.50, //C6   
    1174.66, //D6
    1318.51, //E6
    1396.91, //F6
    1567.98, //G6
    1760.00, //A6
    1975.53, //B6
    2093.00, //C7
    -1 // End of array (if -1, then restart array index)
};

float Tone_getNextNote(void) {
    float note = ToneFreq[Tone_index++];
    if (note < 0) {
        Tone_index = 0;
        return Tone_getNextNote();
    }
    return note;
}

void Tone_start(TIM_HandleTypeDef *htim, int channel, float frequency) {
    htim->Init.Prescaler = (SystemCoreClock / frequency / 100) - 1;
    htim->Init.Period = 100-1;
    HAL_TIM_PWM_Init(htim);
    HAL_TIM_PWM_Start(htim, channel);
}

void Tone_setFrequency(TIM_HandleTypeDef *htim, float frequency) {
    htim->Instance->PSC = (SystemCoreClock / frequency / 100) - 1;;
}

void Tone_stop(TIM_HandleTypeDef *htim, int channel) {
    HAL_TIM_PWM_Stop(htim, channel);
}

void Tone_setDutyCycle(TIM_HandleTypeDef *htim, int channel, int percentage) {
    __HAL_TIM_SET_COMPARE(htim, channel, percentage);
}
