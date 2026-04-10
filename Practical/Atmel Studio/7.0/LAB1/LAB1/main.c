/*
 * LAB1.c
 *
 * Created: 02-02-2026 14:46:08
 * Author : heermehta
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA=0b00000001;
    /* Replace with your application code */
    while (1) 
    {
		PORTA=0b00000001;
		PORTA=0b00000000;
    }
}

