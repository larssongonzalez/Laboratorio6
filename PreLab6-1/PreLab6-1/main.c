/*************************************************
 * Universidad del Valle de Guatemala
 * IE2023 Programacion de Microcontroladores
 * PreLab6-1.c
 * Autor: Larsson Gonzalez
 * Hardware: ATMega328P
 * Proyecto:
 * Created: 19/4/2024 12:31:00
 ************************************************/ 

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "UART.h"


unsigned char dato;


int main(void)
{
	
	DDRC = 0xFF;
	DDRB |= (1 << DDD4) | (1 << DDB5);
	UART_init();
    
    while (1) {
		dato = UART_read();
		if (dato != 0){
			PORTC = dato & 0x7F;
			if (dato & 0x80)
			{
				PORTB |= (1<<PORTB4); 
			} 
			else
			{
				PORTB &= ~(1<<PORTB4);
			}
			
			if (dato & 0x40)
			{
				PORTB |= (1<<PORTB5);
			} 
			else
			{
				PORTB &= ~(1<<PORTB5);
			}
		}
		
    }
}

