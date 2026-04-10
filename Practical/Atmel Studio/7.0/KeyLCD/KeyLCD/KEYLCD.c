/*
 * KeyLCD.c
 *
 * Created: 03-04-2026 10:47:39
 * Author : heermehta
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

/* LCD Control Pins */
#define RS PD0
#define EN PD1

/* Keypad Definitions */
#define KEY_PRT PORTC
#define KEY_DDR DDRC
#define KEY_PIN PINC

/* Function Prototypes */
void LCD_Init(void);
void LCD_Command(unsigned char cmnd);
void LCD_Char(unsigned char data);
void LCD_String(char *str);
void LCD_SetCursor(unsigned char row, unsigned char col);
char keyfind(void);

/* Keypad Layout */
unsigned char keypad[4][4] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};

unsigned char colloc, rowloc;

/* Scrolling Text */
char text[] = "  MCI";
char text2[] = "  OS";

/* ?? IMPROVED NON-BLOCKING KEYPAD FUNCTION */
char keyfind()
{
	KEY_DDR = 0xF0;   // Rows output, columns input
	KEY_PRT = 0xFF;   // Enable pull-ups

	// Check if any key pressed
	if ((KEY_PIN & 0x0F) != 0x0F)
	{
		_delay_ms(20); // debounce

		// Confirm press again
		if ((KEY_PIN & 0x0F) != 0x0F)
		{
			// Scan rows
			for (int r = 0; r < 4; r++)
			{
				KEY_PRT = ~(1 << (r + 4)); // activate one row LOW
				_delay_ms(1);

				colloc = KEY_PIN & 0x0F;

				if (colloc != 0x0F)
				{
					rowloc = r;

					// Detect column
					if (colloc == 0x0E) return keypad[rowloc][0];
					else if (colloc == 0x0D) return keypad[rowloc][1];
					else if (colloc == 0x0B) return keypad[rowloc][2];
					else if (colloc == 0x07) return keypad[rowloc][3];
				}
			}
		}
	}

	return 0; // no key pressed
}

/* LCD Functions */

void LCD_Init(void)
{
	DDRB = 0xFF;
	DDRD |= (1<<RS) | (1<<EN);

	_delay_ms(20);
	LCD_Command(0x38);
	LCD_Command(0x0C);
	LCD_Command(0x06);
	LCD_Command(0x01);
	_delay_ms(2);
}

void LCD_Command(unsigned char cmnd)
{
	PORTB = cmnd;
	PORTD &= ~(1<<RS);

	PORTD |= (1<<EN);
	_delay_ms(1);
	PORTD &= ~(1<<EN);
	_delay_ms(2);
}

void LCD_Char(unsigned char data)
{
	PORTB = data;
	PORTD |= (1<<RS);

	PORTD |= (1<<EN);
	_delay_ms(1);
	PORTD &= ~(1<<EN);
	_delay_ms(2);
}

void LCD_String(char *str)
{
	while (*str)
	{
		LCD_Char(*str++);
	}
}

void LCD_SetCursor(unsigned char row, unsigned char col)
{
	if (row == 0)
	LCD_Command(0x80 + col);
	else
	LCD_Command(0xC0 + col);
}

/* MAIN FUNCTION */
int main(void)
{
	LCD_Init();

	int len = strlen(text);
	int len1 = strlen(text2);

	int scroll_enable = 1;
	int i = 0;

	char last_key = 0;

	int scroll_counter = 0;   // ? NEW

	while (1)
	{
		char key = keyfind();

		// ?? Detect NEW key press only
		if (key != 0 && key != last_key)
		{
			if (key == 'A')
			scroll_enable = 0;
			else if (key == 'B')
			scroll_enable = 1;
		}

		last_key = key;

		// ?? Display line 1
		LCD_Command(0x80);
		for (int j = 0; j < 16; j++)
		{
			LCD_Char(text[(i + j) % len]);
		}

		// ?? Display line 2
		LCD_Command(0xC0);
		for (int j = 0; j < 16; j++)
		{
			LCD_Char(text2[(len1 + j - i) % len1]);
		}

		// ?? CONTROL SCROLL SPEED (instead of big delay)
		if (scroll_enable)
		{
			scroll_counter++;

			if (scroll_counter >= 10)   // adjust speed here
			{
				i++;
				if (i >= len) i = 0;
				scroll_counter = 0;
			}
		}

		_delay_ms(10);  // ?? SMALL delay (important!)
	}
}


// 
// #define F_CPU 1000000UL
// #include <avr/io.h>
// #include <util/delay.h>
// #include <string.h>
// 
// /* LCD Control Pins */
// #define RS PD0
// #define EN PD1
// 
// /* Keypad Definitions */
// #define KEY_PRT PORTC
// #define KEY_DDR DDRC
// #define KEY_PIN PINC
// 
// /* Function Prototypes */
// void LCD_Init(void);
// void LCD_Command(unsigned char cmnd);
// void LCD_Char(unsigned char data);
// void LCD_String(char *str);
// void LCD_SetCursor(unsigned char row, unsigned char col);
// char keyfind(void);
// 
// /* Keypad Layout */
// unsigned char keypad[4][4] = {
// 	{'1','2','3','A'},
// 	{'4','5','6','B'},
// 	{'7','8','9','C'},
// 	{'*','0','#','D'}
// };
// 
// unsigned char colloc, rowloc;
// 
// /* Scrolling Text */
// char text[] = "  MCI";
// char text2[] = "  OS";
// 
// /* ?? IMPROVED NON-BLOCKING KEYPAD FUNCTION */
// char keyfind()
// {
// 	KEY_DDR = 0x0F;   // PC0–PC3 = rows (output), PC4–PC7 = columns (input)
// 	KEY_PRT = 0xF0;   // enable pull-ups on columns
// 
// 	// Check if any key pressed (check columns now!)
// 	if ((KEY_PIN & 0xF0) != 0xF0)
// 	{
// 		_delay_ms(20); // debounce
// 
// 		if ((KEY_PIN & 0xF0) != 0xF0)
// 		{
// 			for (int r = 0; r < 4; r++)
// 			{
// 				// Keep columns HIGH, make one row LOW
// 				KEY_PRT = (0xF0) | (~(1 << r) & 0x0F);
// 
// 				_delay_ms(1);
// 
// 				colloc = KEY_PIN & 0xF0;
// 
// 				if (colloc != 0xF0)
// 				{
// 					rowloc = r;
// 
// 					if (colloc == 0xE0) return keypad[rowloc][0];
// 					else if (colloc == 0xD0) return keypad[rowloc][1];
// 					else if (colloc == 0xB0) return keypad[rowloc][2];
// 					else if (colloc == 0x70) return keypad[rowloc][3];
// 				}
// 			}
// 		}
// 	}
// 
// 	return 0;
// }
// 
// /* LCD Functions */
// 
// void LCD_Init(void)
// {
// 	DDRA = 0xFF;
// 	DDRD |= (1<<RS) | (1<<EN);
// 
// 	_delay_ms(20);
// 	LCD_Command(0x38);
// 	LCD_Command(0x0C);
// 	LCD_Command(0x06);
// 	LCD_Command(0x01);
// 	_delay_ms(2);
// }
// 
// void LCD_Command(unsigned char cmnd)
// {
// 	PORTA = cmnd;
// 	PORTD &= ~(1<<RS);
// 
// 	PORTD |= (1<<EN);
// 	_delay_ms(1);
// 	PORTD &= ~(1<<EN);
// 	_delay_ms(2);
// }
// 
// void LCD_Char(unsigned char data)
// {
// 	PORTA = data;
// 	PORTD |= (1<<RS);
// 
// 	PORTD |= (1<<EN);
// 	_delay_ms(1);
// 	PORTD &= ~(1<<EN);
// 	_delay_ms(2);
// }
// 
// void LCD_String(char *str)
// {
// 	while (*str)
// 	{
// 		LCD_Char(*str++);
// 	}
// }
// 
// void LCD_SetCursor(unsigned char row, unsigned char col)
// {
// 	if (row == 0)
// 	LCD_Command(0x80 + col);
// 	else
// 	LCD_Command(0xC0 + col);
// }
// 
// /* MAIN FUNCTION */
// int main(void)
// {
// 	LCD_Init();
// 
// 	int len = strlen(text);
// 	int len1 = strlen(text2);
// 
// 	int scroll_enable = 1;
// 	int i = 0;
// 
// 	char last_key = 0;
// 
// 	int scroll_counter = 0;   // ? NEW
// 
// 	while (1)
// 	{
// 		char key = keyfind();
// 
// 		// ?? Detect NEW key press only
// 		if (key != 0 && key != last_key)
// 		{
// 			if (key == 'A')
// 			scroll_enable = 0;
// 			else if (key == 'B')
// 			scroll_enable = 1;
// 		}
// 
// 		last_key = key;
// 
// 		// ?? Display line 1
// 		LCD_Command(0x80);
// 		for (int j = 0; j < 16; j++)
// 		{
// 			LCD_Char(text[(i + j) % len]);
// 		}
// 
// 		// ?? Display line 2
// 		LCD_Command(0xC0);
// 		for (int j = 0; j < 16; j++)
// 		{
// 			LCD_Char(text2[(len1 + j - i) % len1]);
// 		}
// 
// 		// ?? CONTROL SCROLL SPEED (instead of big delay)
// 		if (scroll_enable)
// 		{
// 			scroll_counter++;
// 
// 			if (scroll_counter >= 10)   // adjust speed here
// 			{
// 				i++;
// 				if (i >= len) i = 0;
// 				scroll_counter = 0;
// 			}
// 		}
// 
// 		_delay_ms(10);  // ?? SMALL delay (important!)
// 	}
// }




// 
// #define F_CPU 1000000UL
// #include <avr/io.h>
// #include <util/delay.h>
// #include <string.h>
// 
// /* LCD Control Pins */
// #define RS PD0
// #define EN PD1
// 
// /* Keypad Definitions */
// #define KEY_PRT PORTB
// #define KEY_DDR DDRB
// #define KEY_PIN PINB
// 
// /* Function Prototypes */
// void LCD_Init(void);
// void LCD_Command(unsigned char cmnd);
// void LCD_Char(unsigned char data);
// void LCD_String(char *str);
// void LCD_SetCursor(unsigned char row, unsigned char col);
// char keyfind(void);
// 
// /* Keypad Layout */
// unsigned char keypad[4][4] = {
// 	{'1','2','3','A'},
// 	{'4','5','6','B'},
// 	{'7','8','9','C'},
// 	{'*','0','#','D'}
// };
// 
// unsigned char colloc, rowloc;
// 
// /* Scrolling Text */
// char text[] = "  MCI";
// char text2[] = "  OS";
// 
// /* ?? IMPROVED NON-BLOCKING KEYPAD FUNCTION */
// char keyfind()
// {
// 	KEY_DDR = 0x0F;   // PC0–PC3 = rows (output), PC4–PC7 = columns (input)
// 	KEY_PRT = 0xF0;   // enable pull-ups on columns
// 
// 	// Check if any key pressed (check columns now!)
// 	if ((KEY_PIN & 0xF0) != 0xF0)
// 	{
// 		_delay_ms(20); // debounce
// 
// 		if ((KEY_PIN & 0xF0) != 0xF0)
// 		{
// 			for (int r = 0; r < 4; r++)
// 			{
// 				// Keep columns HIGH, make one row LOW
// 				KEY_PRT = (0xF0) | (~(1 << r) & 0x0F);
// 
// 				_delay_ms(1);
// 
// 				colloc = KEY_PIN & 0xF0;
// 
// 				if (colloc != 0xF0)
// 				{
// 					rowloc = r;
// 
// 					if (colloc == 0xE0) return keypad[rowloc][0];
// 					else if (colloc == 0xD0) return keypad[rowloc][1];
// 					else if (colloc == 0xB0) return keypad[rowloc][2];
// 					else if (colloc == 0x70) return keypad[rowloc][3];
// 				}
// 			}
// 		}
// 	}
// 
// 	return 0;
// }
// 
// /* LCD Functions */
// 
// void LCD_Init(void)
// {
// 	DDRC = 0xFF;
// 	DDRD |= (1<<RS) | (1<<EN);
// 
// 	_delay_ms(20);
// 	LCD_Command(0x38);
// 	LCD_Command(0x0C);
// 	LCD_Command(0x06);
// 	LCD_Command(0x01);
// 	_delay_ms(2);
// }
// 
// void LCD_Command(unsigned char cmnd)
// {
// 	PORTC = cmnd;
// 	PORTD &= ~(1<<RS);
// 
// 	PORTD |= (1<<EN);
// 	_delay_ms(1);
// 	PORTD &= ~(1<<EN);
// 	_delay_ms(2);
// }
// 
// void LCD_Char(unsigned char data)
// {
// 	PORTC = data;
// 	PORTD |= (1<<RS);
// 
// 	PORTD |= (1<<EN);
// 	_delay_ms(1);
// 	PORTD &= ~(1<<EN);
// 	_delay_ms(2);
// }
// 
// void LCD_String(char *str)
// {
// 	while (*str)
// 	{
// 		LCD_Char(*str++);
// 	}
// }
// 
// void LCD_SetCursor(unsigned char row, unsigned char col)
// {
// 	if (row == 0)
// 	LCD_Command(0x80 + col);
// 	else
// 	LCD_Command(0xC0 + col);
// }
// 
// /* MAIN FUNCTION */
// int main(void)
// {
// 	LCD_Init();
// 
// 	int len = strlen(text);
// 	int len1 = strlen(text2);
// 
// 	int scroll_enable = 1;
// 	int i = 0;
// 
// 	char last_key = 0;
// 
// 	int scroll_counter = 0;   // ? NEW
// 
// 	while (1)
// 	{
// 		char key = keyfind();
// 
// 		// ?? Detect NEW key press only
// 		if (key != 0 && key != last_key)
// 		{
// 			if (key == 'A')
// 			scroll_enable = 0;
// 			else if (key == 'B')
// 			scroll_enable = 1;
// 		}
// 
// 		last_key = key;
// 
// 		// ?? Display line 1
// 		LCD_Command(0x80);
// 		for (int j = 0; j < 16; j++)
// 		{
// 			LCD_Char(text[(i + j) % len]);
// 		}
// 
// 		// ?? Display line 2
// 		LCD_Command(0xC0);
// 		for (int j = 0; j < 16; j++)
// 		{
// 			LCD_Char(text2[(len1 + j - i) % len1]);
// 		}
// 
// 		// ?? CONTROL SCROLL SPEED (instead of big delay)
// 		if (scroll_enable)
// 		{
// 			scroll_counter++;
// 
// 			if (scroll_counter >= 10)   // adjust speed here
// 			{
// 				i++;
// 				if (i >= len) i = 0;
// 				scroll_counter = 0;
// 			}
// 		}
// 
// 		_delay_ms(10);  // ?? SMALL delay (important!)
// 	}
// }