#include "stm32f1xx.h"

int main(void) {
    // Set clock to 8MHz
    SystemCoreClockUpdate();

    // Initialize Cube HAL
    HAL_Init();

    GPIO_InitTypeDef GPIO_InitStruct;

    // Configure PA0 as digital input
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure PC13 as push-pull output
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    while(1) {
        // Press button on PA0 for shorter delay
        if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0) {
            HAL_Delay(50);
        } else {
            HAL_Delay(500);
        }
        // Toggle PC13
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    }

    return 0;
}
