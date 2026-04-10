#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

// LCD command
void lcd_cmd(unsigned char cmd)
{
	// Send high nibble to PORTB
	PORTB = (PORTB & 0x0F) | (cmd & 0xF0);

	PORTD &= ~(1<<PD0); // RS = 0
	PORTD |= (1<<PD1);  // EN = 1
	_delay_ms(1);
	PORTD &= ~(1<<PD1);

	// Send low nibble
	PORTB = (PORTB & 0x0F) | (cmd << 4);

	PORTD |= (1<<PD1);
	_delay_ms(1);
	PORTD &= ~(1<<PD1);
}

// LCD data
void lcd_data(unsigned char data)
{
	// High nibble
	PORTB = (PORTB & 0x0F) | (data & 0xF0);

	PORTD |= (1<<PD0); // RS = 1
	PORTD |= (1<<PD1);
	_delay_ms(1);
	PORTD &= ~(1<<PD1);

	// Low nibble
	PORTB = (PORTB & 0x0F) | (data << 4);

	PORTD |= (1<<PD1);
	_delay_ms(1);
	PORTD &= ~(1<<PD1);
}

void lcd_init()
{
	DDRB = 0xF0; // PB4–PB7 output
	DDRD |= (1<<PD0) | (1<<PD1); // RS, EN output

	_delay_ms(20);

	lcd_cmd(0x02);
	lcd_cmd(0x28);
	lcd_cmd(0x0C);
	lcd_cmd(0x06);
	lcd_cmd(0x01);
}

void lcd_string(char *str)
{
	while (*str)
	lcd_data(*str++);
}

// ADC
void ADC_init()
{
	ADMUX = (1<<REFS0);
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);
}

uint16_t ADC_read()
{
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	return ADC;
}

int main(void)
{
	uint16_t adc_val;
	int temp;
	char buffer[10];

	lcd_init();
	ADC_init();

	while (1)
	{
		adc_val = ADC_read();
		temp = (adc_val * 500) / 1024;

		lcd_cmd(0x80);

		buffer[0] = (temp/10) + '0';
		buffer[1] = (temp%10) + '0';
		buffer[2] = 'C';
		buffer[3] = '\0';

		lcd_string("Temp: ");
		lcd_string(buffer);

		_delay_ms(1000);
	}
}