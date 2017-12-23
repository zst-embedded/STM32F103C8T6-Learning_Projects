#pragma once

#include "stm32f1xx_hal.h"
#include <stdbool.h>

static bool PA0_Pressed = false;

void PA0_Handler_Init() {
    HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler() {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_0) {
        PA0_Pressed = true;
    }
}

