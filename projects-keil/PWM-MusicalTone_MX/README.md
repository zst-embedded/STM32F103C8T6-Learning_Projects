## PWM-MusicalTone

	STM32F103C8T6
	Blue Pill Board

	A music tone will be produced for the buzzer on
	PA11 when push button PA0 is pressed. For each
	button press, a single note will be played in
	"Do, Re, Mi" sequence. It lasts for one second.

	(On boot, a test siren sound is played)

	PWM (TIM1 Channel 4) is used for tone generation
	the SysTick value is polled to stop the tone after
	1000 ticks has passed (ie. after 1000 ms or 1 sec)

	PA11 is connected to the positive terminal of
	the buzzer.

	PA0 is connected to a active-low push button.

	PC13 is the on-board LED. It is active-low and
	is turned on when the tone is playing.

	---

	IDE:
	- Keil uVision 5