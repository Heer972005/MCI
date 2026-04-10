/*
 * waveFormInterrupt.c
 *
 * Created: 09-04-2026 09:11:13
 * Author : heermehta
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#define OUT PD4

// ISR for Timer0 overflow
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 0xE8;            // preload for 25 ms
	PORTD ^= (1<<OUT);       // toggle every interrupt
}

int main(void)
{
	DDRD = 0xFF;             // PORTD as output

	TCNT0 = 0xE8;            // initial preload

	TIMSK = (1<<TOIE0);      // enable Timer0 overflow interrupt

	TCCR0 = (1<<CS00) | (1<<CS02);  // prescaler 1024

	sei();                   // enable global interrupts

	while (1)
	{
		// CPU free ??
	}
}

