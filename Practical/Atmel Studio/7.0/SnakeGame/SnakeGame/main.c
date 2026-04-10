/*
 * SnakeGame.c
 *
 * Created: 10-04-2026 23:06:38
 * Author : heermehta
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

// LCD
#define RS PD0
#define EN PD2

// Keypad
#define KEY_PRT PORTB
#define KEY_DDR DDRB
#define KEY_PIN PINB

int snake_x = 0;
int snake_y = 0;

int food_x = 10;
int food_y = 1;

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

void LCD_Clear()
{
	LCD_Command(0x01);
}

void LCD_Init()
{
	DDRC = 0xFF;
	DDRD |= (1<<RS)|(1<<EN);

	_delay_ms(20);

	LCD_Command(0x38);
	LCD_Command(0x0C);
	LCD_Command(0x06);
	LCD_Command(0x01);
}

//------------- KEYPAD ------------//

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

	for(int i=0;i<4;i++)
	{
		KEY_PRT = ~(1 << (i+4));

		for(int j=0;j<4;j++)
		{
			if(!(KEY_PIN & (1<<j)))
			{
				_delay_ms(50);
				return key[i][j];
			}
		}
	}
	return 0;
}

//------------- DRAW ------------//

void draw()
{
	LCD_Clear();

	// Draw snake
	LCD_Command(0x80 + snake_x + (snake_y * 0x40));
	LCD_Char('O');

	// Draw food
	LCD_Command(0x80 + food_x + (food_y * 0x40));
	LCD_Char('*');
}

//------------- MAIN ------------//

int main(void)
{
	char key;

	LCD_Init();

	while(1)
	{
		key = keypad_getkey();

		// Movement
		if(key == '6') snake_x++;   // RIGHT
		if(key == '4') snake_x--;   // LEFT
		if(key == '8') snake_y--;   // UP
		if(key == '2') snake_y++;   // DOWN

		// Boundaries
		if(snake_x > 15) snake_x = 15;
		if(snake_x < 0)  snake_x = 0;
		if(snake_y > 1)  snake_y = 1;
		if(snake_y < 0)  snake_y = 0;

		// Eat food
		if(snake_x == food_x && snake_y == food_y)
		{
			food_x = rand() % 16;
			food_y = rand() % 2;
		}

		draw();

		_delay_ms(300);
	}
}