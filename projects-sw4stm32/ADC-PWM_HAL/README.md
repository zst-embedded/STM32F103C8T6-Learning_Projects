# ADC-PWM_HAL

	STM32F103C8T6
	Blue Pill Board

	This project is to make use of ADC and PWM.

	A potentiometer is attached to PA0 (ADC1, Channel 0)

	One LED is connected on PA8 (TIM1, Channel 1)
	Its brightness is controlled by the pot.

	Another LED is connected on PA10 (TIM1, Channel 3)
	It fades from 0% to 100% repeatedly (triangle wave)

	---

	IDE:
	- SW4STM32


## References

+ ADC
	- http://microtechnics.ru/en/stm32cube-stm-and-adc-analog-to-digital-converter/
	- https://visualgdb.com/tutorials/arm/stm32/adc/
	- https://electronics.stackexchange.com/questions/202938/stm32-adc-conversion-using-hal

+ PWM
	- https://electronics.stackexchange.com/a/239783
	- https://stackoverflow.com/questions/43483762/using-stm32-hal-timer-and-adjusting-the-duty-cycle-of-a-pwm-signal
	- https://www.waveshare.com/wiki/STM32CubeMX_Tutorial_Series:_PWM
	- https://www.youtube.com/watch?v=Ql6klWiROik
