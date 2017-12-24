# Timer-MusicalTone

	STM32F103C8T6
	Blue Pill Board

	A music tone will be produced for the buzzer on
	PB12 when push button PA0 is pressed. For each
	button press, a single note will be played in
	"Do, Re, Mi" sequence. It lasts for one second.

	TIM2 is used for tone generation and TIM3 is used
	to stop the tone after one-second has passed.

	PB12 is connected to the positive terminal of
	the buzzer via a 100-ohm series resistor.

	PA0 is connected to a active-low push button.

	PC13 is the on-board LED. It is active-low and
	is turned on when the tone is playing.

	---

	IDE:
	- SW4STM32

## References

- http://www.phy.mtu.edu/~suits/notefreqs.html
- http://www.keil.com/download/docs/363.asp
