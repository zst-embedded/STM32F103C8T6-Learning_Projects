/**
 * STM32F103C8T6
 * Blue Pill Board
 *
 * Blinking LED on PC13
 */

#include "stm32f1xx.h"

int main(void) {
    // Set clock to 8MHz
    SystemCoreClock = 8e6;

    // Initialize Cube HAL
    HAL_Init();

    // Enable I/O port C clock
    RCC->APB2ENR = RCC_APB2ENR_IOPCEN;

    // Set PC13 to output (high speed)
    GPIOC->CRH |= GPIO_CRH_MODE13; // 11: Output mode, max speed 50 MHz

    // Set PC13 to output (push-pull)
    GPIOC->CRH &= ~GPIO_CRH_CNF13; // 00: General purpose output push-pull

    /* Alternatively, using atomic bit set-reset registers
    while(1) {
        // Set PC13
        GPIOC->BSRR = GPIO_BSRR_BS13;
        HAL_Delay(500);

        // Reset PC13
        GPIOC->BSRR = GPIO_BSRR_BR13;
        HAL_Delay(500);
    }
     */

    while(1) {
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        HAL_Delay(500);
    }

    return 0;
}
