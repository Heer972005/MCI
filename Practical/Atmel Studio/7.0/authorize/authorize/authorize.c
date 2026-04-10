/*
 * authorize.c
 *
 * Created: 10-04-2026 22:00:52
 * Author : heermehta
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

// LCD
#define RS PD0
#define EN PD2

// Lock & Buzzer
#define LOCK PD6
#define BUZZ PD5

// Keypad
#define KEY_PRT PORTB
#define KEY_DDR DDRB
#define KEY_PIN PINB

// Authorized users
char valid_users[10][5] = {
	"1234","5678","1111","2222","3333",
	"4444","5555","6666","7777","8888"
};

char input[5];

//---------------- LCD ----------------//

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
	DDRD |= (1<<RS)|(1<<EN)|(1<<LOCK)|(1<<BUZZ);

	_delay_ms(20);

	LCD_Command(0x38);
	LCD_Command(0x0C);
	LCD_Command(0x06);
	LCD_Command(0x01);
}

//---------------- KEYPAD ----------------//

char keypad_getkey()
{
	char key[4][4] = {
		{'7','8','9','/'},
		{'4','5','6','*'},
		{'1','2','3','-'},
		{'C','0','=','+'}
	};

	KEY_DDR = 0xF0;
	KEY_PRT = 0x0F;

	while(1)
	{
		for(int i=0;i<4;i++)
		{
			KEY_PRT = ~(1 << (i+4));

			for(int j=0;j<4;j++)
			{
				if(!(KEY_PIN & (1<<j)))
				{
					_delay_ms(20);
					while(!(KEY_PIN & (1<<j)));
					return key[i][j];
				}
			}
		}
	}
}

//---------------- MAIN ----------------//

int main(void)
{
	LCD_Init();

	while(1)
	{
		// Flash message
		LCD_Clear();
		LCD_String("Enter GR No");
		_delay_ms(500);
		LCD_Clear();
		_delay_ms(500);

		LCD_String("Enter GR No");
		LCD_Command(0xC0);

		// Input 4 digits
		for(int i=0;i<4;i++)
		{
			input[i] = keypad_getkey();
			LCD_Char('*');
		}
		input[4] = '\0';

		_delay_ms(500);

		// Validation
		int valid = 0;

		for(int i=0;i<10;i++)
		{
			if(strcmp(input, valid_users[i]) == 0)
			{
				valid = 1;
				break;
			}
		}

		LCD_Clear();

		if(valid)
		{
			LCD_String("Access Granted");

			PORTD |= (1<<LOCK);   // ?? unlock
			_delay_ms(3000);

			PORTD &= ~(1<<LOCK);  // lock again
		}
		else
		{
			LCD_String("Access Denied");

			// ?? buzzer
			for(int i=0;i<200;i++)
			{
				PORTD ^= (1<<BUZZ);
				_delay_us(500);
			}
		}

		_delay_ms(2000);
	}
}