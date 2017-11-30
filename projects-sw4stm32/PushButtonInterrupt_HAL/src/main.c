/**
 * STM32F103C8T6
 * Blue Pill Board
 *
 * Toggle LED on PC13 with
 * push button on PA0.
 *
 * PA0 uses external interrupt
 * set up using Cube HAL.
 *
 * References
 *  - http://www.hertaville.com/external-interrupts-on-the-stm32f0.html
 *  - https://www.simonmartin.ch/resources/stm32/dl/STM32%20Tutorial%2007%20-%20GPIO%20Interrupts%20(EXTI)%20using%20HAL%20(and%20FreeRTOS).pdf
 *
 */

#include "stm32f1xx.h"
#include <stdbool.h>

bool toggleLed = false;

void EXTI0_IRQHandler() {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_0) {
        toggleLed = true;
    }
}

/*
// Alternatively:
void EXTI0_IRQHandler() {
    if(__HAL_GPIO_EXTI_GET_FLAG(GPIO_PIN_0)) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
        toggleLed = true;
    }
}
*/

int main(void) {
    SystemCoreClockUpdate();
    HAL_Init();

    /* Setup NVIC (using Cube HAL) */
    HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    GPIO_InitTypeDef GPIO_InitStruct;

    // Configure PA0 as external interrupt on falling edge
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure PC13 as push-pull output
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    while(1) {
        if (toggleLed) {
            toggleLed = false;
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        }
        HAL_Delay(500);
    }

    return 0;
}
