#include "stm32f1xx.h"

int main(void) {
    // Set clock to 8MHz
    SystemCoreClock = 8e6;

    // Initialize Cube HAL
    HAL_Init();

    // Enable I/O port A clock
    RCC->APB2ENR = RCC_APB2ENR_IOPAEN;
    // Set PA0 to input
    GPIOA->CRL &= ~GPIO_CRL_MODE0; // 00: Input mode (reset state)
    GPIOA->CRL &= ~GPIO_CRL_CNF0;
    GPIOA->CRL |= 0b10 << GPIO_CRL_CNF0_Pos; // 10: Pull-up/down
    GPIOA->ODR |= GPIO_ODR_ODR0; // 1: Input pull-up

    // Enable I/O port C clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    // Set PC13 to output
    GPIOC->CRH |= GPIO_CRH_MODE13; // 11: Output mode, max speed 50 MHz
    GPIOC->CRH &= ~GPIO_CRH_CNF13; // 00: General purpose output push-pull

    while(1) {
        // Press button on PA0 for shorter delay
        if ((GPIOA->IDR & GPIO_IDR_IDR0) == 0) {
            HAL_Delay(50);
        } else {
            HAL_Delay(500);
        }
        // Toggle PC13
        GPIOC->ODR ^= GPIO_ODR_ODR13;
    }

    return 0;
}
