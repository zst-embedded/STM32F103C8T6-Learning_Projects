#include "stm32f1xx.h"
#include <stdbool.h>

bool toggleLed = false;

void EXTI0_IRQHandler() {
    /**
     * EXTI_PR
     * This bit is set when the selected edge event
     * arrives on the external interrupt line. This
     * bit is cleared by writing a ‘1’ into the bit.
     */
    if (EXTI->PR & EXTI_PR_PR0) {
        EXTI->PR = EXTI_PR_PR0;
        toggleLed = true;
    }
}

int main(void) {
    SystemCoreClockUpdate();
    HAL_Init();

    /**
     * Hardware interrupt selection
     * To configure the 20 lines as interrupt sources, use the following procedure:
     *     • Configure the mask bits of the 20 Interrupt lines (EXTI_IMR)
     *     • Configure the Trigger Selection bits of the Interrupt lines (EXTI_RTSR and EXTI_FTSR)
     *     • Configure the enable and mask bits that control the NVIC IRQ channel mapped to the
     *       External Interrupt Controller (EXTI) so that an interrupt coming from one of the 20 lines
     *       can be correctly acknowledged.
     */
    EXTI->IMR |= EXTI_IMR_MR0; // Mask for EXTI0
    // EXTI->RTSR |= EXTI_RTSR_RT0; // Rising Edge Trigger
    EXTI->FTSR |= EXTI_FTSR_FT0; // Falling Edge Trigger
    AFIO->EXTICR[0] &= ~AFIO_EXTICR1_EXTI0_Msk;
    AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI0_PA; // Multiplex PA1 to EXTI0

    /* Setup NVIC (using CMSIS function) */
    NVIC_SetPriority(EXTI0_IRQn, 1);
    NVIC_EnableIRQ(EXTI0_IRQn);


    GPIO_InitTypeDef GPIO_InitStruct;

    // Configure PA0 as digital input
    __HAL_RCC_GPIOA_CLK_ENABLE();
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
        if (toggleLed) {
            toggleLed = false;
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        }
        HAL_Delay(500);
    }

    return 0;
}
