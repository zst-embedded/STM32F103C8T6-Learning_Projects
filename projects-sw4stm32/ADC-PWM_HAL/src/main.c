#include "stm32f1xx.h"
#include <stdlib.h>

// Peripherals
TIM_HandleTypeDef htim1;
ADC_HandleTypeDef hadc1;

// Function prototypes
void PWM_Init();
void PWM1_ChangeDutyCycle(int);
void PWM3_ChangeDutyCycle(int);
void ADC_Init();
int ADC_Poll();

// Main
int main(void) {
    SystemCoreClockUpdate();
    HAL_Init();

    // Init PWM
    PWM_Init();
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_ALL);

    // Enable ADC for PA0
    ADC_Init();

    while(1) {
        for (int x = -100; x < 100; x++) {
            PWM3_ChangeDutyCycle(abs(x));
            HAL_Delay(3);

            // Non-blocking for interrupt
            HAL_ADC_Start_IT(&hadc1);
        }
    }

    return 0;
}

void PWM_Init() {
    /* Enable clock for Timer 1 */
    __HAL_RCC_TIM1_CLK_ENABLE();

    /* Enable Timer 1 for PWM generation */
    htim1.Instance = TIM1;
    htim1.Init.Prescaler = (SystemCoreClock / 100000) - 1; // 100 kHz tick;
    htim1.Init.Period = 100 - 1;
    //htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    //htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    //htim1.Init.RepetitionCounter = 0;
    //htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_PWM_Init(&htim1);

    /* Setup PWM channels */
    TIM_OC_InitTypeDef sConfigOC;
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 50;
    //sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    //sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    //sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    //sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    //sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3); // PA10
    HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1); // PA8

    /* Configure PA8 & PA10 as alternate function push-pull */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void PWM1_ChangeDutyCycle(int duty_percentage) {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, duty_percentage);
}
void PWM3_ChangeDutyCycle(int duty_percentage) {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, duty_percentage);
}

void ADC_Init() {
    /* Enable clock for ADC1 */
    __HAL_RCC_ADC1_CLK_ENABLE();

    /* Enable ADC1 */
    hadc1.Instance = ADC1;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    HAL_ADC_Init(&hadc1);

    /* Setup ADC channel 8 */
    ADC_ChannelConfTypeDef sConfig;
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /* Enable ADC interrupts */
    HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);

    /* Setup analog input on PA0 */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* Blocking ADC (polling) */
int ADC_Poll() {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 100);
    int result = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return result;
}

void ADC1_2_IRQHandler(void) {
    HAL_ADC_IRQHandler(&hadc1);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    int result = HAL_ADC_GetValue(&hadc1);
    PWM1_ChangeDutyCycle(result/ 4096.0 * 100);
}

