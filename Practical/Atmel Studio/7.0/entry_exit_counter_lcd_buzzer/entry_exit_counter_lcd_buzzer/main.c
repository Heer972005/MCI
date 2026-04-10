/*
 * entry_exit_counter_lcd_buzzer.c
 *
 * Created: 10-04-2026 21:24:41
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
#define BUZZ  PD5

int count = 0;
int MAX = 5;

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
	DDRD |= (1<<RS)|(1<<EN)|(1<<BUZZ);
	DDRD &= ~((1<<ENTRY)|(1<<EXIT));

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
		// ENTRY
		if(!(PIND & (1<<ENTRY)))
		{
			if(count < MAX)
			{
				count++;
			}
			_delay_ms(300);
		}

		// EXIT
		if(!(PIND & (1<<EXIT)))
		{
			if(count > 0)
			{
				count--;
			}
			_delay_ms(300);
		}

		// DISPLAY
		LCD_Command(0x80);
		LCD_String("Count:     ");

		LCD_Command(0xC0);
if(count == MAX)
{
	LCD_String("FULL ");
for(int i=0;i<200;i++)
{
	PORTD ^= (1<<BUZZ);  // toggle
	_delay_us(500);
}
}
else
{
	LCD_String("     ");   // ?? clear line
	LCD_Command(0xC0);

	itoa(count, buffer, 10);
	LCD_String(buffer);

	PORTD &= ~(1<<BUZZ);
}


		_delay_ms(200);
	}
}