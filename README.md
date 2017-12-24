# STM32F103C8T6 Learning Projects

This is a collection of projects which I have done for the generic STM32F103 Minimum System Development Board (commonly known as Blue Pill).

These projects use the STM32Cube HAL and STM32CubeMX. The IDE used is [SW4STM32](http://www.st.com/en/development-tools/sw4stm32.html) or [Keil uVision ARM MDK](http://www2.keil.com/mdk5).

Project                                            | Done on    | Category           | Extra Hardware
---------------------------------------------------|:----------:| ------------------ | -----------------
[BlinkLED]                                         | 2017-10-11 | Digital I/O        | 
[PushButtonPolling]                                | 2017-10-12 | Digital I/O        | Push Button
[PushButtonPolling_HAL]                            | 2017-10-12 | Digital I/O        | Push Button
[PushButtonInterrupt]                              | 2017-10-15 | External Interrupt | Push Button
[PushButtonInterrupt_HAL]                          | 2017-10-15 | External Interrupt | Push Button
[Timer-BlinkLED]                                   | 2017-10-29 | Timer              | 
[Timer-MusicalTone]                                | 2017-10-29 | Timer              | Buzzer, Push Button
[ADC-PWM_HAL]                                      | 2017-11-25 | ADC, PWM           | LEDs, Potentiometer
[PWM-MusicalTone_MX]                               | 2017-11-30 | PWM                | Buzzer, Push Button
[16x2-LCD-PWM_MX]                                  | 2017-12-02 | PWM, Interfacing   | HD44780 LCD display<br>(with RGB backlight)
[SPI-Write]                                        | 2017-12-09 | SPI                | MAX7219 + 8x8 LED Matrix
[SPI-Write_HAL]                                    | 2017-12-09 | SPI                | MAX7219 + 8x8 LED Matrix

#### Project name suffix
- ***Projects appended with `_HAL` are those using the Cube HAL library to setup the peripherals***
- ***Projects appended with `_MX` are those generated using STM32CubeMX.***
- ***Projects without anything appended are those using direct register defines to setup the peripherals***

[BlinkLED]: ./projects-sw4stm32/BlinkLED
[PushButtonPolling]: ./projects-sw4stm32/PushButtonPolling
[PushButtonPolling_HAL]: ./projects-sw4stm32/PushButtonPolling_HAL
[PushButtonInterrupt]: ./projects-sw4stm32/PushButtonInterrupt
[PushButtonInterrupt_HAL]: ./projects-sw4stm32/PushButtonInterrupt_HAL
[Timer-BlinkLED]: ./projects-sw4stm32/Timer-BlinkLED
[Timer-MusicalTone]: ./projects-sw4stm32/Timer-MusicalTone
[ADC-PWM_HAL]: ./projects-sw4stm32/ADC-PWM_HAL
[PWM-MusicalTone_MX]: ./projects-keil/PWM-MusicalTone_MX
[16x2-LCD-PWM_MX]: ./projects-keil/16x2-LCD-PWM_MX
[SPI-Write]: ./projects-sw4stm32/SPI-Write
[SPI-Write_HAL]: ./projects-sw4stm32/SPI-Write_HAL

## [Setup Guide + References](additional/setup.md)
