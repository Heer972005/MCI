/*
 * PassCheck.c
 *
 * Created: 10-04-2026 19:33:04
 * Author : heermehta
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

/* LCD Pins */
#define RS PD0
#define EN PD1

/* Keypad */
#define KEY_PRT PORTB
#define KEY_DDR DDRB
#define KEY_PIN PINB

/* Function Prototypes */
void LCD_Init(void);
void LCD_Command(unsigned char cmnd);
void LCD_Char(unsigned char data);
void LCD_String(char *str);
void LCD_Clear(void);
char keypad_getkey(void);

/* Password */
char correct_pass[5] = "1234";
char entered_pass[5];


//---------------- LCD FUNCTIONS ----------------//

void LCD_Command(unsigned char cmnd)
{
	PORTC = cmnd;
	PORTD &= ~(1<<RS);
	PORTD &= ~(1<<EN);
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

void LCD_Init()
{
	DDRC = 0xFF;
	DDRD = 0xFF;
	_delay_ms(20);

	LCD_Command(0x38);
	LCD_Command(0x0C);
	LCD_Command(0x06);
	LCD_Command(0x01);
}

void LCD_String(char *str)
{
	while(*str) LCD_Char(*str++);
}

void LCD_Clear()
{
	LCD_Command(0x01);
}


//---------------- KEYPAD FUNCTION ----------------//
char keypad_getkey()
{
	char key[4][4] = {
		{'7','8','9','/'},
		{'4','5','6','*'},
		{'1','2','3','-'},
		{'C','0','=','+'}
	};

	KEY_DDR = 0xF0;   // Rows output, cols input
	KEY_PRT = 0xFF;   // Enable pull-ups

	while(1)
	{
		for(int i=0;i<4;i++)
		{
			KEY_PRT = ~(1 << (i+4));   // Activate one row

			for(int j=0;j<4;j++)
			{
				if(!(KEY_PIN & (1<<j)))   // Key pressed
				{
					_delay_ms(20);  // debounce

					while(!(KEY_PIN & (1<<j))); // ?? WAIT TILL RELEASE

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
		LCD_Clear();
		LCD_String("Enter Password");

		LCD_Command(0xC0); // second line

		// Take 4 digits
		for(int i=0;i<4;i++)
		{
			entered_pass[i] = keypad_getkey();
			LCD_Char('*');   // Mask input
		}
		entered_pass[4] = '\0';

		_delay_ms(500);

		LCD_Clear();

		// Compare passwords
		if(strcmp(entered_pass, correct_pass) == 0)
		{
			LCD_String("Correct Password");
		}
		else
		{
			LCD_String("Try Again");
		}

		_delay_ms(2000);
	}
}

