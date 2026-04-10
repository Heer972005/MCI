/*
 * temp.c
 *
 * Created: 10-04-2026 20:01:43
 * Author : heermehta
 */ 
// #define F_CPU 1000000UL
// #include <avr/io.h>
// #include <util/delay.h>
// #include <stdlib.h>
// 
// //---------------- UART ----------------//
// 
// void UART_Init(long baud)
// {
// 	int ubrr = F_CPU/16/baud - 1;
// 	UBRRH = (ubrr >> 8);
// 	UBRRL = ubrr;
// 
// 	UCSRB = (1<<TXEN);              // Enable transmitter
// 	UCSRC = (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1); // 8-bit data
// }
// 
// void UART_TxChar(char data)
// {
// 	while(!(UCSRA & (1<<UDRE)));
// 	UDR = data;
// }
// 
// void UART_TxString(char *str)
// {
// 	while(*str)
// 	{
// 		UART_TxChar(*str++);
// 	}
// }
// 
// //---------------- ADC ----------------//
// 
// void ADC_Init()
// {
// 	ADMUX = (1<<REFS0);  // AVCC reference
// 	ADCSRA = (1<<ADEN)|(1<<ADPS1)|(1<<ADPS0); // Enable ADC
// }
// 
// uint16_t ADC_Read(uint8_t ch)
// {
// 	ch &= 0b00000111;
// 	ADMUX = (ADMUX & 0xF8) | ch;
// 
// 	ADCSRA |= (1<<ADSC);
// 	while(ADCSRA & (1<<ADSC));
// 
// 	return ADC;
// }
// 
// //---------------- MAIN ----------------//
// 
// int main(void)
// {
// 	char buffer[10];
// 	uint16_t adc_val;
// 	float temp;
// 
// 	UART_Init(4800);
// 	ADC_Init();
// 
// 	while(1)
// 	{
// 		adc_val = ADC_Read(0); // LM35 on ADC0
// 
// 		temp = (adc_val * 5.0 * 100.0) / 1023.0;
// 
// 		dtostrf(temp, 4, 2, buffer); // float to string
// 
// 		UART_TxString("Temp: ");
// 		UART_TxString(buffer);
// 		UART_TxString(" C\r\n");
// 
// 		_delay_ms(1000);
// 	}
//
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define RS PD0
#define EN PD2

//------------- LCD FUNCTIONS ------------//

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

//------------- ADC ------------//

void ADC_Init()
{
	ADMUX = (1<<REFS0);  // AVCC
	ADCSRA = (1<<ADEN)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t ADC_Read(uint8_t ch)
{
	ch &= 0b00000111;
	ADMUX = (ADMUX & 0xF8) | ch;

	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));

	return ADC;
}

//------------- MAIN ------------//

int main(void)
{
	char buffer[10];
	uint16_t adc_val;
	float temp;

	LCD_Init();
	ADC_Init();

	while(1)
	{
		adc_val = ADC_Read(0);

		temp = (adc_val * 5.0 * 100.0) / 1023.0;

		dtostrf(temp, 4, 2, buffer);

		LCD_Clear();
		LCD_String("Temp:");

		LCD_Command(0xC0);
		LCD_String(buffer);
		LCD_String(" C");

		_delay_ms(1000);
	}
}