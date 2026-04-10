/*
 * keypad.c
 *
 * Created: 03-04-2026 10:10:50
 * Author : heermehta
 */ 

/*
 * Keypad.c
 *
 * Created: 03-04-2026 08:14:51
 * Author : Rishi's Hp omen
 */ 
/*
   4x4 Keypad Interfacing with ATmega16/32
   // Mhttp://www.electronicwings.com
 */ 
 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

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

/* Keypad Function */
char keyfind()
{
	while (1)
	{
		KEY_DDR = 0xF0;   // Rows output, columns input
		KEY_PRT = 0x0F;   // Enable pull-ups
		// Wait for key release
		do {
			KEY_PRT &= 0x0F;
			colloc = (KEY_PIN & 0x0F);
		} while (colloc != 0x0F);

		// Wait for key press
		do {
			_delay_ms(20);
			colloc = (KEY_PIN & 0x0F);
		} while (colloc == 0x0F);

		_delay_ms(20);

		// Detect row
		// Detect row properly
		KEY_DDR = 0xF0;   // rows output, columns input

		// Row 1
		KEY_PRT = 0xEF;
		_delay_ms(1);
		colloc = (KEY_PIN & 0x0F);
		if (colloc != 0x0F) { rowloc = 0; break; }

		// Row 2
		KEY_PRT = 0xDF;
		_delay_ms(1);
		colloc = (KEY_PIN & 0x0F);
		if (colloc != 0x0F) { rowloc = 1; break; }

		// Row 3
		KEY_PRT = 0xBF;
		_delay_ms(1);
		colloc = (KEY_PIN & 0x0F);
		if (colloc != 0x0F) { rowloc = 2; break; }

		// Row 4
		KEY_PRT = 0x7F;
		_delay_ms(1);
		colloc = (KEY_PIN & 0x0F);
		if (colloc != 0x0F) { rowloc = 3; break; }
	}

	if (colloc == 0x0E) return keypad[rowloc][0];
	else if (colloc == 0x0D) return keypad[rowloc][1];
	else if (colloc == 0x0B) return keypad[rowloc][2];
	else return keypad[rowloc][3];
}

/* LCD Functions */

void LCD_Init(void)
{
	DDRA = 0xFF;
	DDRD |= (1<<RS) | (1<<EN);

	_delay_ms(20);
	LCD_Command(0x38); // 8-bit, 2-line
	LCD_Command(0x0C); // Display ON
	LCD_Command(0x06); // Cursor increment
	LCD_Command(0x01); // Clear display
	_delay_ms(2);
}

void LCD_Command(unsigned char cmnd)
{
	PORTA = cmnd;
	PORTD &= ~(1<<RS);

	PORTD |= (1<<EN);
	_delay_ms(1);
	PORTD &= ~(1<<EN);
	_delay_ms(2);
}

void LCD_Char(unsigned char data)
{
	PORTA = data;
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

	LCD_SetCursor(0,0);
	LCD_String("Press Key:");

	while (1)
	{
		char key = keyfind();

		LCD_SetCursor(1,0);
		LCD_String("Key: ");
		LCD_Char(key);
		LCD_String("   "); // clear extra chars
	}
}