# 16x2-LCD-PWM

### Description

This is a Hello World project using Adafruit 16x2 HD44780 character LCD display (with RGB Backlight).

An LCD library (`mylcd.h/mylcd.c`) is created from scratch.

To control the RGB backlight, inverted PWM is generated on PA[1:3]
(active-low backlight)

### Pin Connections

***Take note of the separate 5V supply from the MCU's 3V3 supply***

	/* ----------------------------------
	 * PIN CONNECTIONS FOR LCD
	 * ----------------------------------
	 * (01) Vss - GND
	 * (02) Vdd - 5V
	 * (03) Vee - Pot Contrast
	 * (04) RS  - PA8
	 * (05) R/W - GND
	 * (06) En  - PA9
	 * (07) DB0
	 * (08) DB1
	 * (09) DB2
	 * (10) DB3
	 * (11) DB4 - PB12
	 * (12) DB5 - PB13
	 * (13) DB6 - PB14
	 * (14) DB7 - PB15
	 * (15) LED (+) - 3V3
	 * (16) LED (R-) - PA1 (PWM2/2)
	 * (17) LED (G-) - PA2 (PWM2/3)
	 * (18) LED (B-) - PA3 (PWM2/4)
	 * ----------------------------------
	 */