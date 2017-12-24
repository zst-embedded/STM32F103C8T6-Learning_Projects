#include "stm32f1xx.h"
#include <stdbool.h>
#include <math.h>


// Taken from http://www.phy.mtu.edu/~suits/notefreqs.html
const float ToneFreq[] = {
    1046.50, // C6
    1174.66, // D6
    1318.51, // E6
    1396.91, // F6
    1567.98, // G6
    1760.00, // A6
    1975.53, // B6
    2093.00, // C7
    -1 // End of array (if -1, then restart array index)
};

/* Modified from STM32_Init.c */
#define __TIM_USEC_TICK(PERIOD) ((SystemCoreClock / 1000000UL) * PERIOD)
#define __PSC(PERIOD)           (((__TIM_USEC_TICK(PERIOD) + 49999UL) / 50000UL) - 1)
#define __ARR(PERIOD)           ((__TIM_USEC_TICK(PERIOD) / (__PSC(PERIOD) + 1)) - 1)


/**************************************
 * Function prototypes & variables
 **************************************/
bool ButtonPressed = false;

void TIM2_Init();
void TIM2_StartFrequency(float);
void TIM2_IRQHandler(void);

void TIM3_Init(void);
void TIM3_StartTiming(void);
void TIM3_IRQHandler(void);

void EXTI0_Init(void);
void EXTI0_IRQHandler(void);

void startTone(float);
void stopTone(void);
int main(void);


/**************************************
 * Timer 2 for tone generation
 * Square wave (50% duty cycle)
 **************************************/

void TIM2_Init() {
    __HAL_RCC_TIM2_CLK_ENABLE();

    TIM2->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_StartFrequency(float freq) {
    TIM2->SR &= ~TIM_SR_UIF; // Clear flag
    TIM2->CNT = 0; // Clear count

    float usec = 1e6 / freq;
    TIM2->PSC = round(__PSC(usec));
    TIM2->ARR = round(__ARR(usec));

    TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
        TIM2->SR &= ~TIM_SR_UIF;
    }
}


/**************************************
 * Timer 3 for one-second timer
 **************************************/

void TIM3_Init() {
    __HAL_RCC_TIM3_CLK_ENABLE();

    TIM3->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM3_IRQn);

    long one_sec = 1 * 1000 * 1000;
    TIM3->PSC = __PSC(one_sec);
    TIM3->ARR = __ARR(one_sec);
}

void TIM3_StartTiming() {
    TIM3->SR &= ~TIM_SR_UIF; // Clear flag
    TIM3->CNT = 0; // Clear count
    TIM3->CR1 |= TIM_CR1_CEN;
}

void TIM3_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) {
        TIM3->SR &= ~TIM_SR_UIF;
        stopTone();
    }
}


/**************************************
 * External Interrupt & Push Button
 **************************************/

void EXTI0_Init() {
    EXTI->IMR |= EXTI_IMR_MR0; // Mask for EXTI0
    EXTI->FTSR |= EXTI_FTSR_FT0; // Enable Falling Edge Trigger
    EXTI->RTSR &= ~EXTI_RTSR_RT0; // Disable Rising Edge Trigger
    AFIO->EXTICR[0] &= ~AFIO_EXTICR1_EXTI0_Msk;
    AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI0_PA; // Multiplex PA0 to EXTI0

    NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler() {
    if (EXTI->PR & EXTI_PR_PR0) {
        ButtonPressed = true;
        EXTI->PR = EXTI_PR_PR0;
    }
}


/**************************************
 * Main Functions
 **************************************/

void startTone(float freq) {
    // 1 cycle == 2 toggles
    TIM2_StartFrequency(freq / 2.0);
    TIM3_StartTiming();
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

void stopTone() {
    // Stop Tone Gen
    TIM2->CR1 &= ~TIM_CR1_CEN;
    // Stop one-second timer
    TIM3->CR1 &= ~TIM_CR1_CEN;
    // Turn off buzzer
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

int main(void) {
    SystemCoreClockUpdate();
    HAL_Init();

    GPIO_InitTypeDef GPIO_InitStruct;

    // Configure PA0 as digital input (internal pullup)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure PB1 as push-pull output (buzzer)
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Configure PC13 as push-pull output (on-board LED)
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // Init peripherals
    TIM2_Init();
    TIM3_Init();
    EXTI0_Init();

    // Fixes bug of "Do" not playing on first boot.
    stopTone();
    startTone(0);

    // Main loop
    int index = 0;
    float freq = 0;
    while(1) {
        if (ButtonPressed) {
            freq = ToneFreq[index++];
            if (freq == -1) {
                index = 0;
                continue;
            }

            // Create tone
            stopTone();
            startTone(freq);

            ButtonPressed = false;
        }
        // Delay for debouncing
        HAL_Delay(300);
    }

    return 0;
}
