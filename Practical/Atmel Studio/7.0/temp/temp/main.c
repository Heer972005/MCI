/*
 * temp.c
 *
 * Created: 10-04-2026 20:01:43
 * Author : heermehta
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

//---------------- UART ----------------//

void UART_Init(long baud)
{
	int ubrr = F_CPU/16/baud - 1;
	UBRRH = (ubrr >> 8);
	UBRRL = ubrr;

	UCSRB = (1<<TXEN);              // Enable transmitter
	UCSRC = (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1); // 8-bit data
}

void UART_TxChar(char data)
{
	while(!(UCSRA & (1<<UDRE)));
	UDR = data;
}

void UART_TxString(char *str)
{
	while(*str)
	{
		UART_TxChar(*str++);
	}
}

//---------------- ADC ----------------//

void ADC_Init()
{
	ADMUX = (1<<REFS0);  // AVCC reference
	ADCSRA = (1<<ADEN)|(1<<ADPS1)|(1<<ADPS0); // Enable ADC
}

uint16_t ADC_Read(uint8_t ch)
{
	ch &= 0b00000111;
	ADMUX = (ADMUX & 0xF8) | ch;

	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));

	return ADC;
}

//---------------- MAIN ----------------//

int main(void)
{
	char buffer[10];
	uint16_t adc_val;
	float temp;

	UART_Init(9600);
	ADC_Init();

	while(1)
	{
		adc_val = ADC_Read(0); // LM35 on ADC0

		temp = (adc_val * 5.0 * 100.0) / 1023.0;

		dtostrf(temp, 4, 2, buffer); // float to string

		UART_TxString("Temp: ");
		UART_TxString(buffer);
		UART_TxString(" C\r\n");

		_delay_ms(1000);
	}
}
