/*
 * waveFormPolling.c
 *
 * Created: 09-04-2026 08:45:31
 * Author : heermehta
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>

/*#define OUT PD4

int main(void)
{
	uint8_t count = 0;

	DDRD = 0xFF;       // PORTD as output
	TCNT0 = 0x00;

	// Start Timer0 with prescaler 1024
	TCCR0 = (1<<CS00) | (1<<CS02);

	while (1)
	{
		// Wait for overflow
		while ((TIFR & (1<<TOV0)) == 0);

		TIFR = (1<<TOV0); // Clear flag
		TCNT0 = 0xE8;//0x00;

		count++;

		if (count >=10)   // control delay here
		{
			PORTD ^= (1<<OUT);  // Toggle output
			count = 0;
		}
	}
}*/
#define F_CPU 1000000UL
#include <avr/io.h>

#define OUT PD4

int main(void)
{
	DDRD = 0xFF;

	// Start Timer0 with prescaler 1024
	TCCR0 = (1<<CS00) | (1<<CS02);

	while (1)
	{
		TCNT0 = 0xE8;  // preload for 25 ms

		// Wait for overflow
		while ((TIFR & (1<<TOV0)) == 0);

		TIFR = (1<<TOV0); // clear flag

		PORTD ^= (1<<OUT); // toggle every 25 ms
	}
}