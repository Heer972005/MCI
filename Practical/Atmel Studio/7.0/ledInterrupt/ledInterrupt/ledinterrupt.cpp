/*
 * ledInterrupt.cpp
 *
 * Created: 09-04-2026 08:33:09
 * Author : heermehta
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#define LED PD4

volatile uint8_t overflowCount = 0;

// ISR for Timer0 Overflow
ISR(TIMER0_OVF_vect)
{
	overflowCount++;

	if (overflowCount >= 38)
	{
		PORTD ^= (1<<LED);  // Toggle LED
		overflowCount = 0;
	}
}

int main(void)
{
	DDRD = 0xFF;  // PORTD as output

	TCNT0 = 0x00; // Initial value

	// Enable Timer0 overflow interrupt
	TIMSK = (1<<TOIE0);

	// Start timer with prescaler 1024
	TCCR0 = (1<<CS00) | (1<<CS02);

	sei(); // Enable global interrupts

	while (1)
	{
		// Main loop does nothing
		// CPU is free for other tasks
	}
}