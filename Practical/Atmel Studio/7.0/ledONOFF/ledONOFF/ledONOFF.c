/*
 * ledONOFF.c
 *
 * Created: 10-04-2026 22:46:56
 * Author : heermehta
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

//---------------- UART ----------------//

void UART_Init(long baud)
{
	int ubrr = F_CPU/16/baud - 1;
	UBRRH = (ubrr >> 8);
	UBRRL = ubrr;

	UCSRB = (1<<RXEN)|(1<<TXEN);  // Enable RX & TX
	UCSRC = (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
}

char UART_RxChar()
{
	while(!(UCSRA & (1<<RXC)));
	return UDR;
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

//---------------- MAIN ----------------//

int main(void)
{
	char data;

	DDRD |= (1<<PD5);  // LED output

	UART_Init(9600);

	UART_TxString("Bluetooth Ready\r\n");

	while(1)
	{
		data = UART_RxChar();  // receive from mobile

		if(data == '0')
		{
			PORTD |= (1<<PD5);   // ON
			UART_TxString("Lamp ON\r\n");
		}
		else if(data == '1')
		{
			PORTD &= ~(1<<PD5);  // OFF
			UART_TxString("Lamp OFF\r\n");
		}
	}
}