/**
 * STM32F103C8T6
 * Blue Pill Board
 *
 * SPI using MAX7219 to drive an 8x8 LED matrix
 * Each pixel is turned on one by one.
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

#define __SPI_CS_ENABLE__() do {    GPIOA->BSRR |= GPIO_BSRR_BR4;   } while(0)
#define __SPI_CS_DISABLE__() do {   GPIOA->BSRR |= GPIO_BSRR_BS4;   } while(0)

void SPI_Init(SPI_TypeDef *SPIx) {
    // (!!) Need to enable if using software slave select pin
    // https://stackoverflow.com/a/46992761
    SPIx->CR2 |= SPI_CR2_SSOE; // SS output enable

    SPIx->CR1 = 0; // Clear unwanted values from register
    SPIx->CR1 |= SPI_CR1_DFF;  // 16-bit data frame format
    SPIx->CR1 |= SPI_CR1_MSTR; // Master device
    SPIx->CR1 |= SPI_CR1_CPOL; // Clock polarity: high when idle
    SPIx->CR1 |= SPI_CR1_CPHA; // Capture data on 2nd edge (rising edge)

    // Bits 5:3 BR[2:0]: Baud rate control
    // 000: fPCLK/2
    // 001: fPCLK/4
    // 010: fPCLK/8
    // 011: fPCLK/16
    // 100: fPCLK/32
    // 101: fPCLK/64
    // 110: fPCLK/128
    // 111: fPCLK/256
    SPIx->CR1 |= 0 << SPI_CR1_BR_Pos;

    SPIx->CR1 |= SPI_CR1_SPE; // Peripheral enabled
}

void SPI_Transmit(SPI_TypeDef *SPIx, uint16_t data) {
    while ((SPIx->SR & SPI_SR_BSY)); // BSY=1: SPI is busy in communication
    while ((SPIx->SR & SPI_SR_TXE) == 0); // TXE=0: Tx buffer not empty

    SPIx->DR = data;

    while ((SPIx->SR & SPI_SR_TXE) == 0);
    while ((SPIx->SR & SPI_SR_BSY));
}

void GPIO_LED_Init() {
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    /* PC13 on-board LED */
    GPIOC->CRH &= ~GPIO_CRH_CNF13; // 00: General purpose output push-pull
    GPIOC->CRH |= GPIO_CRH_MODE13; // 11: Output mode, max speed 50 MHz.
}

void GPIO_SPI_Init() {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* PA4 software slave select */
    GPIOA->CRL &= ~GPIO_CRL_CNF4; // 00: General purpose output push-pull
    GPIOA->CRL |= GPIO_CRL_MODE4; // 11: Output mode, max speed 50 MHz.

    /* PA5 SPI clock */
    GPIOA->CRL &= ~GPIO_CRL_CNF5;
    GPIOA->CRL |= GPIO_CRL_CNF5_1; // 10: Alternate function output Push-pull
    GPIOA->CRL |= GPIO_CRL_MODE5; // 11: Output mode, max speed 50 MHz.

    /* PA6 SPI MISO */
    GPIOA->CRL &= ~GPIO_CRL_CNF6;
    GPIOA->CRL |= GPIO_CRL_CNF6_1; // 10: Alternate function output Push-pull
    GPIOA->CRL |= GPIO_CRL_MODE6; // 11: Output mode, max speed 50 MHz.

    /* PA7 SPI MOSI */
    GPIOA->CRL &= ~GPIO_CRL_CNF7;
    GPIOA->CRL |= GPIO_CRL_CNF7_1; // 10: Alternate function output Push-pull
    GPIOA->CRL |= GPIO_CRL_MODE7; // 11: Output mode, max speed 50 MHz.

    /* Remap AFIO */
    AFIO->MAPR &= ~AFIO_MAPR_SPI1_REMAP; // 0: No remap (NSS/PA4, SCK/PA5, MISO/PA6, MOSI/PA7)
}

int main(void) {
    // Init system
    SystemCoreClockUpdate();
    HAL_Init();

    // Setup GPIOs
    GPIO_LED_Init();
    GPIO_SPI_Init();

    // Enable SPI1
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    SPI_Init(SPI1);

    // CS inactive
    __SPI_CS_DISABLE__();

    // Init MAX7219
    __SPI_CS_ENABLE__();
    SPI_Transmit(SPI1, MAX7219_MODE_SCAN_LIMIT | 7);
    __SPI_CS_DISABLE__();

    __SPI_CS_ENABLE__();
    SPI_Transmit(SPI1, MAX7219_MODE_INTENSITY | 0xF);
    __SPI_CS_DISABLE__();

    __SPI_CS_ENABLE__();
    SPI_Transmit(SPI1, MAX7219_MODE_POWER | 0x1);
    __SPI_CS_DISABLE__();

    __SPI_CS_ENABLE__();
    SPI_Transmit(SPI1, MAX7219_MODE_DECODE | 0);
    __SPI_CS_DISABLE__();

    while (1) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

        uint16_t col;
        int bit;

        for (col = 0x100; col <= 0x800; col += 0x100) {
            for (bit = 0; bit < 8; bit++) {
                __SPI_CS_ENABLE__();
                SPI_Transmit(SPI1, col | (1 << bit & 0xFF));
                __SPI_CS_DISABLE__();
                HAL_Delay(30);
            }
        }
    }
}
