/*
 * entry_exit_counter_lcd.c
 *
 * Created: 10-04-2026 21:18:30
 * Author : heermehta
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define RS PD0
#define EN PD2

#define ENTRY PD3
#define EXIT  PD4

int count = 0;

//------------- LCD ------------//

void LCD_Command(unsigned char cmnd)
{
	PORTC = cmnd;
	PORTD &= ~(1<<RS);
	PORTD |= (1<<EN);
	_delay_us(1);
	PORTD &= ~(1<<EN);
	_delay_ms(2);
}

void LCD_Char(unsigned char data)
{
	PORTC = data;
	PORTD |= (1<<RS);
	PORTD |= (1<<EN);
	_delay_us(1);
	PORTD &= ~(1<<EN);
	_delay_ms(2);
}

void LCD_String(char *str)
{
	while(*str) LCD_Char(*str++);
}

void LCD_Clear()
{
	LCD_Command(0x01);
}

void LCD_Init()
{
	DDRC = 0xFF;
	DDRD |= (1<<RS)|(1<<EN);   // only control pins output
	DDRD &= ~((1<<ENTRY)|(1<<EXIT)); // sensors as input

	PORTD |= (1<<ENTRY)|(1<<EXIT); // pull-ups

	_delay_ms(20);

	LCD_Command(0x38);
	LCD_Command(0x0C);
	LCD_Command(0x06);
	LCD_Command(0x01);
}

//------------- MAIN ------------//

int main(void)
{
	char buffer[10];

	LCD_Init();

	while(1)
	{
		// ENTRY SENSOR
		if(!(PIND & (1<<ENTRY)))
		{
			count++;
			_delay_ms(300); // debounce
		}

		// EXIT SENSOR
		if(!(PIND & (1<<EXIT)))
		{
			if(count > 0) count--;
			_delay_ms(300);
		}

		// DISPLAY
		itoa(count, buffer, 10);

		LCD_Command(0x80);
		LCD_String("Count:     ");

		LCD_Command(0xC0);
		LCD_String(buffer);

		_delay_ms(200);
	}
}