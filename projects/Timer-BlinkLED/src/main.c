/**
 * STM32F103C8T6
 * Blue Pill Board
 *
 *
 * TIM1 is configured as an up-counter and triggers
 * an interrupt every 500ms to toggle an LED on PC13.
 *
 * References
 *  - http://homepage.cem.itesm.mx/carbajal/Microcontrollers/SLIDES/STM32F3%20Timers.pdf
 *  - http://www.keil.com/download/docs/363.asp
 *  - https://www.youtube.com/watch?v=mqA4ELmzCmM
 */

#include "stm32f1xx.h"

/* Modified from STM32_Init.c */
#define __TIM_USEC_TICK(PERIOD) ((SystemCoreClock / 1000000UL) * PERIOD)
#define __PSC(PERIOD)           (((__TIM_USEC_TICK(PERIOD) + 49999UL) / 50000UL) - 1)
#define __ARR(PERIOD)           ((__TIM_USEC_TICK(PERIOD) / (__PSC(PERIOD) + 1)) - 1)


void TIM1_Init(void) {
    // Enable TIM1 clock
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    // __HAL_RCC_TIM1_CLK_ENABLE();

    /**
     * Set timer to overflow every 500ms
     * (Auto-calculated)
     */
    uint32_t period = 500 * 1000;
    TIM1->PSC = __PSC(period);
    TIM1->ARR = __ARR(period);

    /**
     * Set timer to overflow every 500ms
     * (Self-calculation)
     *
     * Formula from datasheet
     *   timer freq = crystal freq / (PSC[15:0] + 1)
     *   PSC = crystal freq / timer freq - 1
     *   PSC = crystal freq * timer period - 1
     *
     * Timer period is for [ARR] counts.
     * Hence, overflow period = timer period * ARR
     *   PSC = crystal freq * (oveflow period / ARR) - 1
     *
     * If [ARR] = 1000 (arbitrary value):
     *   PSC = 8MHz * (500ms / 1000) - 1
     *   PSC = 3999
     *   ARR = 1000
     */
    TIM1->PSC = 3999; // Increment timer every [PSC] clock cycles
    TIM1->ARR = 1000; // Count from 0 to [ARR] value (overflow at [ARR])

    // Enable TIM1 interrupt
    TIM1->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM1_UP_IRQn);

    // Enable TIM1
    TIM1->CR1 |= TIM_CR1_CEN;
}

void TIM1_UP_IRQHandler(void) {
    /**
     * Bit 0 UIF: Update interrupt flag
     * This bit is set by hardware on an update event. It is cleared by software.
     */
    if (TIM1->SR & TIM_SR_UIF) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        TIM1->SR &= ~TIM_SR_UIF;
    }
}

int main(void) {
    SystemCoreClockUpdate();
    HAL_Init();

    // Init Timer
    TIM1_Init();

    // Configure PC13 as push-pull output
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // Do nothing
    while(1) {
        __NOP();
    }

    return 0;
}
