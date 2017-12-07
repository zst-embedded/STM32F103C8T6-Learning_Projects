#include "tim.h"
#include "mylcd.h"

void LCD_sendCmd(LCD_HandleTypeDef *lcd, uint8_t data) {
    HAL_GPIO_WritePin(lcd->RS.Port, lcd->RS.Pin, GPIO_PIN_RESET);
    LCD_sendByte(lcd, data);
}

void LCD_sendChar(LCD_HandleTypeDef *lcd, uint8_t data) {
    HAL_GPIO_WritePin(lcd->RS.Port, lcd->RS.Pin, GPIO_PIN_SET);
    LCD_sendByte(lcd, data);
}

void LCD_sendNibble(LCD_HandleTypeDef *lcd, uint8_t data) {
    HAL_GPIO_WritePin(lcd->EN.Port, lcd->EN.Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(lcd->DB4.Port, lcd->DB4.Pin, !!(data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(lcd->DB5.Port, lcd->DB5.Pin, !!(data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(lcd->DB6.Port, lcd->DB6.Pin, !!(data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(lcd->DB7.Port, lcd->DB7.Pin, !!(data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(lcd->EN.Port, lcd->EN.Pin, GPIO_PIN_RESET);
}

void LCD_sendByte(LCD_HandleTypeDef *lcd, uint8_t data) {
    LCD_sendNibble(lcd, data >> 4); // High order bit
    LCD_sendNibble(lcd, data); // Low order bit
}

void LCD_puts(LCD_HandleTypeDef *lcd, char * data) {
    while (data[0] != '\0') {
        LCD_sendChar(lcd, data[0]);
        data++;
    }
}
