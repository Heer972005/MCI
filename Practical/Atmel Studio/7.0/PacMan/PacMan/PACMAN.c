/*
 * PacMan.c
 *
 * Created: 10-04-2026 19:20:44
 * Author : heermehta
 */ 
/*  
   LCD16x2 8 bit ATmega16 interface
   http://www.electronicwings.com
*/

#define F_CPU 1000000UL		/* Define CPU Frequency e.g. here its 8MHz */
#include <avr/io.h>		/* Include AVR std. library file */
#include <util/delay.h>		/* Include inbuilt defined Delay header file */

#define LCD_Data_Dir DDRC	/* Define LCD data port direction */
#define LCD_Command_Dir DDRD	/* Define LCD command port direction register */
#define LCD_Data_Port PORTC	/* Define LCD data port */
#define LCD_Command_Port PORTD	/* Define LCD data port */
void lcd_built(void);
#define RS PD0			/* Define Register Select signal pin */
#define RW PD1			/* Define Read/Write signal pin */
#define EN PD2			/* Define Enable signal pin */
 
unsigned char addr;
unsigned char i;
void LCD_Command(unsigned char cmnd)
{
	LCD_Data_Port= cmnd;
	LCD_Command_Port &= ~(1<<RS);	/* RS=0 command reg. */
	LCD_Command_Port &= ~(1<<RW);	/* RW=0 Write operation */
	LCD_Command_Port |= (1<<EN);	/* Enable pulse */
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(2);
}

void LCD_Char (unsigned char char_data)  /* LCD data write function */
{
	LCD_Data_Port= char_data;
	LCD_Command_Port |= (1<<RS);	/* RS=1 Data reg. */
	LCD_Command_Port &= ~(1<<RW);	/* RW=0 write operation */
	LCD_Command_Port |= (1<<EN);	/* Enable Pulse */
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(2);			/* Data write delay */
}

void LCD_Init (void)			/* LCD Initialize function */
{
	LCD_Command_Dir = 0xFF;		/* Make LCD command port direction as o/p */
	LCD_Data_Dir = 0xFF;		/* Make LCD data port direction as o/p */
	_delay_ms(20);			/* LCD Power ON delay always >15ms */
	
	LCD_Command (0x38);		/* Initialization of 16X2 LCD in 8bit mode */
	LCD_Command (0x0C);		/* Display ON Cursor OFF */
	LCD_Command (0x06);		/* Auto Increment cursor */
	LCD_Command (0x01);		/* clear display */
	_delay_ms(2);			/* Clear display command delay> 1.63 ms */
	LCD_Command (0x80);		/* Cursor at home position */
}


void LCD_String (char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)  /* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD_String(str);		/* Call LCD string function */
}

void LCD_Clear()
{
	LCD_Command (0x01);		/* clear display */
	LCD_Command (0x80);		/* cursor at home position */
}

void LCD_Custom_Char (unsigned char loc, unsigned char *msg)
{
	unsigned char j;
	if(loc<8)
	{
		LCD_Command (0x40 + (loc*8));  /* Command 0x40 and onwards forces the device to point CGRAM address */
		for(j=0;j<8;j++)	/* Write 8 byte for generation of 1 character */
		LCD_Char(msg[j]);
	}
}


void show_set1()
{
	LCD_Char(0x0);
	LCD_Char(0x01);
}

void show_set2()
{
	LCD_Char(0x2);
	LCD_Char(0x3);
}


void show_dots(char j)
{
	LCD_Command(0x80|(addr&0x0f));
	LCD_Char(j);
}

int main(void)
{
	
	LCD_Init();
	LCD_String("Ewings");
	LCD_Command(0xc3);
	LCD_String("Animation");
	_delay_ms(500);
	lcd_built();
	addr=0xc1;
	
	while(1)
	{
		addr=0xc1;
		i=4;
		do{                      /* showing set 1 left to right rolling */
			LCD_Command(addr++);
			show_set1();
			show_dots(i);
			if(i<7)
			i++;
			else
			i=4;
			_delay_ms(200);
			LCD_Clear();
			
			LCD_Command(addr++);
			show_set2();
			show_dots(i);
			if(i<7)
			i++;
			else
			i=7;
			_delay_ms(200);
			LCD_Clear();
		}while(addr<0xce);
		
		do{			/* showing set 2 right to left rolling */
			LCD_Command(addr--);
			show_set1();
			show_dots(i);
			if(i<7)
			i++;
			else
			i=4;
			_delay_ms(200);
			LCD_Clear();
			
			LCD_Command(addr--);
			show_set2();
			show_dots(i);
			if(i<7)
			i++;
			else
			i=7;
			_delay_ms(200);
			LCD_Clear();
		}while(addr>0xc2);
		
	}
}


void lcd_built(void)
{
	unsigned char Character1[8] = { 0x01, 0x03, 0x07, 0x0D, 0x0F, 0x02, 0x05, 0x0A };
	unsigned char Character2[8] = { 0x10, 0x18, 0x1C, 0x16, 0x1E, 0x08, 0x14, 0x0A };
	unsigned char Character3[8] = { 0x01, 0x03, 0x07, 0x0D, 0x0F, 0x05, 0x08, 0x04 };
	unsigned char Character4[8] = { 0x10, 0x18, 0x1C, 0x16, 0x1E, 0x14, 0x02, 0x04 };
	unsigned char Character5[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18 };
	unsigned char Character6[8] = { 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00 };
	unsigned char Character7[8] = { 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00 };
	unsigned char Character8[8] = { 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 };

	/* ---------- Build Custom Characters -----------------*/
	
	LCD_Custom_Char(0, Character1);  /* Build Character1 at position 0 */
	LCD_Custom_Char(1, Character2);  /* Build Character2 at position 1 */
	LCD_Custom_Char(2, Character3);  /* Build Character3 at position 2 */
	LCD_Custom_Char(3, Character4);  /* Build Character4 at position 3 */
	LCD_Custom_Char(4, Character5);  /* Build Character5 at position 4 */
	LCD_Custom_Char(5, Character6);  /* Build Character6 at position 5 */
	LCD_Custom_Char(6, Character7);  /* Build Character6 at position 6 */
	LCD_Custom_Char(7, Character8);  /* Build Character6 at position 7 */
}
