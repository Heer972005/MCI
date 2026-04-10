/*
 * calc.c
 *
 * Created: 10-04-2026 19:46:21
 * Author : heermehta
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

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

/* Variables */
long num1 = 0, num2 = 0, result = 0;
char op;

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
	DDRD = 0xFF;
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
	KEY_PRT = 0xFF;

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
	char key;
	char buffer[16];

	LCD_Init();

	while(1)
	{
		LCD_Clear();
		num1 = num2 = 0;

		// -------- FIRST NUMBER --------
		while(1)
		{
			key = keypad_getkey();
					if(key == 'C')   // ?? CLEAR
					{
						LCD_Clear();
						num1 = 0;
						continue;
					}
			if(key >= '0' && key <= '9')
			{
				num1 = num1 * 10 + (key - '0');
				LCD_Char(key);
			}
			else
			{
				op = key;
				LCD_Char(op);
				break;
			}
		}

		// -------- SECOND NUMBER --------
		while(1)
		{
			key = keypad_getkey();
		if(key == 'C')   // ?? CLEAR
		{
			LCD_Clear();
			num1 = num2 = 0;
			goto restart;
		}
			if(key >= '0' && key <= '9')
			{
				num2 = num2 * 10 + (key - '0');
				LCD_Char(key);
			}
			else if(key == '=')
			{
				break;
			}
		}

		// -------- CALCULATION --------
		switch(op)
		{
			case '+': result = num1 + num2; break;
			case '-': result = num1 - num2; break;
			case '*': result = num1 * num2; break;
			case '/': result = num2 ? num1 / num2 : 0; break;
		}

		// -------- DISPLAY RESULT --------
		LCD_Command(0xC0);
		itoa(result, buffer, 10);
		LCD_String(buffer);
		// -------- WAIT FOR C TO CLEAR --------
		while(1)
		{
			key = keypad_getkey();
			if(key == 'C')
			{
				LCD_Clear();
				break;
			}
		}

		restart: ;
	}
}