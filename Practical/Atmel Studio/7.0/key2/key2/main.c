/*
 * key2.c
 *
 * Created: 10-04-2026 15:52:03
 * Author : heermehta
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include<string.h>
/* LCD Pins */
#define RS PD0
#define EN PD1
void LCD_Init(void);
void LCD_Command(unsigned char cmnd);
void LCD_Char(unsigned char data);
void LCD_String(char *str);
void LCD_SetCursor(unsigned char row, unsigned char col);
char keyfind(void);
/* Keypad */
#define KEY_PRT PORTB
#define KEY_DDR DDRB
#define KEY_PIN PINB

unsigned char keypad[4][4] = {
	{'7','8','9','/'},
	{'4','5','6','*'},
	{'1','2','3','-'},
	{'C','0','=','+'}
};

 unsigned char colloc, rowloc;

char keyfind()
{
	while(1)
	{
		// Step 1: Set rows output, cols input
		KEY_DDR = 0xF0;
		KEY_PRT = 0x0F;   // enable pull-ups + rows HIGH

		// Wait for key press
		while((KEY_PIN & 0x0F) == 0x0F);

		_delay_ms(20); // debounce

		// Scan rows one by one
		for(rowloc = 0; rowloc < 4; rowloc++)
		{
			KEY_PRT = (0xFF & ~(1 << (rowloc + 4))); // one row LOW

			colloc = KEY_PIN & 0x0F;

			if(colloc != 0x0F)
			{
				if(colloc == 0x0E) return keypad[rowloc][0];
				if(colloc == 0x0D) return keypad[rowloc][1];
				if(colloc == 0x0B) return keypad[rowloc][2];
				if(colloc == 0x07) return keypad[rowloc][3];
			}
		}
	}
}

// /* LCD FUNCTIONS */
//
void LCD_Command(unsigned char cmnd)
{
	PORTC = cmnd;
	PORTD &= ~(1<<RS);

	PORTD |= (1<<EN);
	_delay_ms(1);
	PORTD &= ~(1<<EN);
	_delay_ms(3);
}
//
void LCD_Char(unsigned char data)
{
	PORTC = data;
	PORTD |= (1<<RS);

	PORTD |= (1<<EN);
	_delay_ms(1);
	PORTD &= ~(1<<EN);
	_delay_ms(1);
}

void LCD_String(char *str)
{
	while(*str)
	{
		LCD_Char(*str++);
	}
}


void LCD_Init(void)
{
	DDRC = 0xFF;
// 	DDRD |= (1<<RS) | (1<<EN);
	_delay_ms(20);
	LCD_Command(0x38);
	LCD_Command(0x0C);
	LCD_Command(0x06);
	LCD_Command(0x01);
	_delay_ms(2);
}
void LCD_SetCursor(unsigned char row, unsigned char col)
{
	if (row == 0)
	LCD_Command(0x80 + col);
	else
	LCD_Command(0xC0 + col);
}

// /* MAIN */
//
int main(void)
{
	DDRC = 0xFF;  // LCD Data lines (D0–D7)
	DDRD |= (1<<RS) | (1<<EN);  // Control pins
	LCD_Init();
	LCD_SetCursor(0,0);
	LCD_String("Press Key:");

	while(1)
	{
		char key = keyfind();

		LCD_SetCursor(1,0);
		LCD_Char(key);
		LCD_String("   ");
	}
}

