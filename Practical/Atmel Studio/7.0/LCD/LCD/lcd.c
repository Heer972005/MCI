/*
 * LCD.c
 *
 * Created: 10-04-2026 09:04:46
 * Author : heermehta
 */ 
// #define F_CPU 1000000UL
// #include <avr/io.h>
// #include <util/delay.h>
// #include<string.h>
// #define RS PD0
// #define EN PD1
//
// void LCD_Init(void);
// void LCD_Command(unsigned char cmnd);
// void LCD_Char(unsigned char data);
// void LCD_String(char *str);
// char Keypad_GetKey();
// // char text[] = "  MCI";
// //char text2[]="  OS";
// int main(void)
// {
// 	DDRA = 0xFF;  // LCD Data lines (D0–D7)
// 	DDRD |= (1<<RS) | (1<<EN);  // Control pins
//
// 	DDRC = 0x0F;   // PC0–PC3 OUTPUT (rows), PC4–PC7 INPUT (columns)
//	PORTC = 0xF0;  // Enable pull-ups on columns
// 	LCD_Init();   // Initialize LCD
//
// 	LCD_Command(0x80); // First line, first position
// 	//LCD_String("RISHI SAMPAT"); // Your name
// 	//int len = strlen(text);
// 	//int len1=strlen(text2);
//
// //int paused = 0;
//
// // while (1)
// // {
// // 	for (int i = 0; i < len; i++)
// // 	{
// // 		char key = Keypad_GetKey();
// //
// // 		if (key)   // Any key pressed
// // 		{
// // 			paused = !paused;   // Toggle pause
// // 			_delay_ms(300);   // prevent multiple toggles
// // 		}
// //
// // 		if (paused)
// // 		{
// // 			i--;   // Stay on same frame
// // 			_delay_ms(100);
// // 			continue;
// // 		}
// //
// // 		// LINE 1 (Left rotation)
// // 		LCD_Command(0x80);
// // 		for (int j = 0; j < 16; j++)
// // 		{
// // 			LCD_Char(text[(i + j) % len]);
// // 		}
// //
// // 		// LINE 2 (Right rotation)
// // 		LCD_Command(0xC0);
// // 		for (int j = 0; j < 16; j++)
// // 		{
// // 			LCD_Char(text2[(len1 + j - i) % len1]);
// // 		}
// //
// // 		_delay_ms(300);
// // 	}
// // }
// while (1)
// {
// 	char key = Keypad_GetKey();
//
// 	if (key != 0)   // If any key is pressed
// 	{
// 		LCD_Command(0x01);   // Clear screen
// 		_delay_ms(2);
//
// 		LCD_Command(0x80);   // First line
// 		LCD_Char(key);       // Show pressed key
// 	}
// }
//
// 	//simple scrolling
// 	// 	LCD_Command(0x8F); // First line, first position
// 	// 	LCD_String("RISHI SAMPAT"); // Your name
// 	//
// 	// 	while (1)
//	{
//		LCD_Command(0x18); // Shift display LEFT
//		_delay_ms(400);
//}
// }
//
// void LCD_Init(void)
// {
// 	_delay_ms(20);
// 	LCD_Command(0x38); // 8-bit, 2-line
// 	LCD_Command(0x0C); // Display ON, cursor OFF
// 	LCD_Command(0x06); // Auto increment cursor
// 	LCD_Command(0x01); // Clear display
// 	_delay_ms(2);
// }
//
// void LCD_Command(unsigned char cmnd)
// {
// 	PORTA = cmnd;
// 	PORTD &= ~(1<<RS);  // Command mode
//
// 	PORTD |= (1<<EN);
// 	_delay_ms(1);
// 	PORTD &= ~(1<<EN);
// 	_delay_ms(3);
// }
//
// void LCD_Char(unsigned char data)
// {
// 	PORTA = data;
// 	PORTD |= (1<<RS);   // Data mode
//
// 	PORTD |= (1<<EN);
// 	_delay_ms(1);
// 	PORTD &= ~(1<<EN);
// 	_delay_ms(1);
// }
//
// void LCD_String(char *str)
// {
// 	for(int i = 0; str[i] != 0; i++)
// 	{
// 		LCD_Char(str[i]);
// 	}
// }
//
// char Keypad_GetKey()
// {
// 	char key_map[4][4] = {
// 		{'1','2','3','A'},
// 		{'4','5','6','B'},
// 		{'7','8','9','C'},
// 		{'*','0','#','D'}
// 	};
//
// 	for (int row = 0; row < 4; row++)
// 	{
// 		//PORTC = ~(1 << row);  // Make one row LOW
// 		//PORTC = (0xF0) | (~(1 << row) & 0x0F);
// 		PORTC = 0xF0 | (0x0F & ~(1 << row));
// 		_delay_ms(1);
//
// 		for (int col = 0; col < 4; col++)
// 		{
// 			if (!(PINC & (1 << (col + 4)))) // Check column
// 			{
// 				_delay_ms(20); // debounce
// 				while (!(PINC & (1 << (col + 4)))); // wait release
// 				return key_map[row][col];
// 			}
// 		}
// 	}
// 	return 0; // No key
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


