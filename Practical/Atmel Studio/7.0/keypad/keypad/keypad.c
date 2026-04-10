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
 
// 
// #define F_CPU 1000000UL
// #include <avr/io.h>
// #include <util/delay.h>
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
// /* Keypad Function */
// char keyfind()
// {
// 	while (1)
// 	{
// 		KEY_DDR = 0xF0;   // Rows output, columns input
// 		KEY_PRT = 0xFF;   // Enable pull-ups
// 
// 		// Wait for key release
// 		do {
// 			KEY_PRT &= 0x0F;
// 			colloc = (KEY_PIN & 0x0F);
// 		} while (colloc != 0x0F);
// 
// 		// Wait for key press
// 		do {
// 			_delay_ms(20);
// 			colloc = (KEY_PIN & 0x0F);
// 		} while (colloc == 0x0F);
// 
// 		_delay_ms(20);
// 
// 		// Detect row
// 		KEY_PRT = 0xEF;
// 		colloc = (KEY_PIN & 0x0F);
// 		if (colloc != 0x0F) { rowloc = 0; break; }
// 
// 		KEY_PRT = 0xDF;
// 		colloc = (KEY_PIN & 0x0F);
// 		if (colloc != 0x0F) { rowloc = 1; break; }
// 
// 		KEY_PRT = 0xBF;
// 		colloc = (KEY_PIN & 0x0F);
// 		if (colloc != 0x0F) { rowloc = 2; break; }
// 
// 		KEY_PRT = 0x7F;
// 		colloc = (KEY_PIN & 0x0F);
// 		if (colloc != 0x0F) { rowloc = 3; break; }
// 	}
// 
// 	if (colloc == 0x0E) return keypad[rowloc][0];
// 	else if (colloc == 0x0D) return keypad[rowloc][1];
// 	else if (colloc == 0x0B) return keypad[rowloc][2];
// 	else return keypad[rowloc][3];
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
// 	LCD_Command(0x38); // 8-bit, 2-line
// 	LCD_Command(0x0C); // Display ON
// 	LCD_Command(0x06); // Cursor increment
// 	LCD_Command(0x01); // Clear display
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
// 
// int main(void)
// {
// 	LCD_Init();
// 
// 	LCD_SetCursor(0,0);
// 	LCD_String("Press Key:");
// 
// 	while (1)
// 	{
// 		char key = keyfind();
// 
// 		//LCD_SetCursor(1,0);
// 		//LCD_String("Key: ");
// 		LCD_SetCursor(1,0);
// 		LCD_Char((KEY_PIN & 0x0F) + '0');
// 		LCD_Char(key);
// 		LCD_String("   "); // clear extra chars
// 	}
//}
// 
// #define F_CPU 1000000UL
// #include <avr/io.h>
// #include <util/delay.h>
// #include<string.h>
// /* LCD Pins */
// #define RS PD0
// #define EN PD1
// void LCD_Init(void);
// void LCD_Command(unsigned char cmnd);
// void LCD_Char(unsigned char data);
// void LCD_String(char *str);
// void LCD_SetCursor(unsigned char row, unsigned char col);
// char keyfind(void);
// /* Keypad */
// #define KEY_PRT PORTB
// #define KEY_DDR DDRB
// #define KEY_PIN PINB
// 
// unsigned char keypad[4][4] = {
// 	{'7','8','9','/'},
// 	{'4','5','6','*'},
// 	{'1','2','3','-'},
// 	{'C','0','=','+'}
// };
// 
// unsigned char colloc, rowloc;
// 
// char keyfind()
// {
// 	while(1)
// 	{
// 		// Step 1: Set rows output, cols input
// 		KEY_DDR = 0xF0;
// 		KEY_PRT = 0xFF;   // enable pull-ups + rows HIGH
// 
// 		// Wait for key press
// 		while((KEY_PIN & 0x0F) == 0x0F);
// 
// 		_delay_ms(20); // debounce
// 
// 		// Scan rows one by one
// 		for(rowloc = 0; rowloc < 4; rowloc++)
// 		{
// 			KEY_PRT = (0xFF & ~(1 << (rowloc + 4))); // one row LOW
// 
// 			colloc = KEY_PIN & 0x0F;
// 
// 			if(colloc != 0x0F)
// 			{
// 				if(colloc == 0x0E) return keypad[rowloc][0];
// 				if(colloc == 0x0D) return keypad[rowloc][1];
// 				if(colloc == 0x0B) return keypad[rowloc][2];
// 				if(colloc == 0x07) return keypad[rowloc][3];
// 			}
// 		}
// 	}
// }
// 
// /* LCD FUNCTIONS */
// 
// void LCD_Command(unsigned char cmnd)
// {
// 	PORTC = cmnd;
// 	PORTD &= ~(1<<RS);
// 
// 	PORTD |= (1<<EN);
// 	_delay_ms(1);
// 	PORTD &= ~(1<<EN);
// 	_delay_ms(3);
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
// 	_delay_ms(1);
// }
// 
// void LCD_String(char *str)
// {
// 	while(*str)
// 	{
// 		LCD_Char(*str++);
// 	}
// }
// 
// 
// void LCD_Init(void)
// {
// 	DDRC = 0xFF;
// 	DDRD |= (1<<RS) | (1<<EN);
// 	_delay_ms(20);
// 	LCD_Command(0x38);
// 	LCD_Command(0x0C);
// 	LCD_Command(0x06);
// 	LCD_Command(0x01);
// 	_delay_ms(2);
// }
// void LCD_SetCursor(unsigned char row, unsigned char col)
// {
// 	if (row == 0)
// 	LCD_Command(0x80 + col);
// 	else
// 	LCD_Command(0xC0 + col);
// }
// 
// /* MAIN */
// 
// int main(void)
// {
// 	DDRC = 0xFF;  // LCD Data lines (D0–D7)
// 	DDRD |= (1<<RS) | (1<<EN);  // Control pins
// 	LCD_Init();
// 	LCD_SetCursor(0,0);
// 	LCD_String("Press Key:");
// 
// 	while(1)
// 	{
// 		char key = keyfind();
// 
// 		LCD_SetCursor(1,0);
// 		LCD_Char(key);
// 		LCD_String("   ");
// 	}
// }


#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include<string.h>
#define RS PD0
#define EN PD1

void LCD_Init(void);
void LCD_Command(unsigned char cmnd);
void LCD_Char(unsigned char data);
void LCD_String(char *str);

char text[] = "  MCI";
char text2[]="  OS";
int main(void)
{
	DDRC = 0xFF;  // LCD Data lines (D0–D7)
	DDRD |= (1<<RS) | (1<<EN);  // Control pins

	LCD_Init();   // Initialize LCD
	
	LCD_Command(0x80); // First line, first position
	//LCD_String("RISHI SAMPAT"); // Your name
	int len = strlen(text);
	int len1=strlen(text2);
	
	while (1)
	{
		for (int i = 0; i < len; i++)
		{
			LCD_Command(0x80); // Go to start

			// Print 16 chars window
			for (int j = 0; j < 16; j++)
			{
				LCD_Char(text[(i + j) % len]);
			}
			
			LCD_Command(0xC0);
			for(int j=0;j<16;j++){
				LCD_Char(text2[(len1+j-i)%len1]);
			}

			_delay_ms(500);
		}
	}


	
	//simple scrolling
	// 	LCD_Command(0x8F); // First line, first position
	// 	LCD_String("RISHI SAMPAT"); // Your name
	//
	// 	while (1)
	//	{
	//		LCD_Command(0x18); // Shift display LEFT
	//		_delay_ms(400);
	//}
}

void LCD_Init(void)
{
	_delay_ms(20);
	LCD_Command(0x38); // 8-bit, 2-line
	LCD_Command(0x0C); // Display ON, cursor OFF
	LCD_Command(0x06); // Auto increment cursor
	LCD_Command(0x01); // Clear display
	_delay_ms(2);
}

void LCD_Command(unsigned char cmnd)
{
	PORTC = cmnd;
	PORTD &= ~(1<<RS);  // Command mode

	PORTD |= (1<<EN);
	_delay_ms(1);
	PORTD &= ~(1<<EN);
	_delay_ms(3);
}

void LCD_Char(unsigned char data)
{
	PORTC = data;
	PORTD |= (1<<RS);   // Data mode

	PORTD |= (1<<EN);
	_delay_ms(1);
	PORTD &= ~(1<<EN);
	_delay_ms(1);
}

void LCD_String(char *str)
{
	for(int i = 0; str[i] != 0; i++)
	{
		LCD_Char(str[i]);
	}
}


