/**
 * STM32F103C8T6
 * Blue Pill Board
 *
 * SPI using MAX7219 to drive an 8x8 LED matrix
 * Each pixel is turned on one by one.
 *
 * This uses CubeHAL to initialise SPI
 *
 * Connections:
 *   CS  - PA4 | Software slave select
 *   CLK - PA5 | SPI clock
 *   DIN - PA7 | SPI MOSI
 *   NC  - PA6 | SPI MISO
 */

#include "stm32f1xx.h"

#define MAX7219_MODE_DECODE       0x0900
#define MAX7219_MODE_INTENSITY    0x0A00
#define MAX7219_MODE_SCAN_LIMIT   0x0B00
#define MAX7219_MODE_POWER        0x0C00
#define MAX7219_MODE_TEST         0x0F00
#define MAX7219_MODE_NOOP         0x0000

#define __SPI_CS_ENABLE__() do {    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);   } while(0)
#define __SPI_CS_DISABLE__() do {   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);     } while(0)

SPI_HandleTypeDef SpiHandle;

void SPI1_Init(SPI_HandleTypeDef *hspi) {
    __HAL_RCC_SPI1_CLK_ENABLE();

    hspi->Instance = SPI1;
    hspi->Init.DataSize = SPI_DATASIZE_16BIT; // 16-bit data frame format
    hspi->Init.Mode = SPI_MODE_MASTER; // Master device
    hspi->Init.CLKPolarity = SPI_POLARITY_HIGH; // Clock polarity: high when idle
    hspi->Init.CLKPhase = SPI_PHASE_2EDGE; // Capture data on 2nd edge (rising edge)
    hspi->Init.NSS = SPI_NSS_SOFT; // Use software slave select
    hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    // hspi.Init.Direction         = SPI_DIRECTION_2LINES;
    // hspi.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
    // hspi.Init.CRCPolynomial     = 7;
    // hspi.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    // hspi.Init.TIMode            = SPI_TIMODE_ENABLE;
    HAL_SPI_Init(hspi);
}

void SPI_Transmit(SPI_HandleTypeDef *hspi, uint16_t data) {
    // 2nd parameter - pData: pointer to data buffer
    HAL_SPI_Transmit(hspi, (uint8_t *) &data, 1, 1000);
}

void GPIO_LED_Init() {
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /* PC13 on-board LED */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void GPIO_SPI_Init() {
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    /* PA4 software slave select */
    /* General purpose output push-pull */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PA5 SPI clock */
    /* PA6 SPI MISO */
    /* PA7 SPI MOSI */
    /* Alternate function output Push-pull*/
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Remap AFIO */
    __HAL_AFIO_REMAP_SPI1_DISABLE(); // No remap (NSS/PA4, SCK/PA5, MISO/PA6, MOSI/PA7)
}

int main(void) {
    // Init system
    SystemCoreClockUpdate();
    HAL_Init();

    // Setup GPIOs
    GPIO_LED_Init();
    GPIO_SPI_Init();

    // Enable SPI1
    SPI1_Init(&SpiHandle);
    __HAL_SPI_ENABLE(&SpiHandle);

    // CS inactive
    __SPI_CS_DISABLE__();

    // Init MAX7219
    __SPI_CS_ENABLE__();
    SPI_Transmit(&SpiHandle, MAX7219_MODE_SCAN_LIMIT | 7);
    __SPI_CS_DISABLE__();

    __SPI_CS_ENABLE__();
    SPI_Transmit(&SpiHandle, MAX7219_MODE_INTENSITY | 0xF);
    __SPI_CS_DISABLE__();

    __SPI_CS_ENABLE__();
    SPI_Transmit(&SpiHandle, MAX7219_MODE_POWER | 0x1);
    __SPI_CS_DISABLE__();

    __SPI_CS_ENABLE__();
    SPI_Transmit(&SpiHandle, MAX7219_MODE_DECODE | 0);
    __SPI_CS_DISABLE__();

    while (1) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

        uint16_t col;
        int bit;

        for (col = 0x100; col <= 0x800; col += 0x100) {
            for (bit = 7; bit >= 0; bit--) {
                __SPI_CS_ENABLE__();
                SPI_Transmit(&SpiHandle, col | (1 << bit & 0xFF));
                __SPI_CS_DISABLE__();
                HAL_Delay(30);
            }
        }
    }
}
