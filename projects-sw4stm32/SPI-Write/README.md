# SPI-Write

	STM32F103C8T6
	Blue Pill Board

	SPI using MAX7219 to drive an 8x8 LED matrix
	Each pixel is turned on one by one.

	---

	IDE:
	- SW4STM32

## Connections

Label | Pin | Function
--- | --- | ---
CS  | PA4 | Software slave select
CLK | PA5 | SPI clock
DIN | PA7 | SPI MOSI
NC  | PA6 | SPI MISO

## References

- https://www.simonmartin.ch/resources/stm32/dl/STM32%20Tutorial%2001%20-%20GPIO%20Operations%20using%20HAL%20(and%20FreeRTOS).pdf
