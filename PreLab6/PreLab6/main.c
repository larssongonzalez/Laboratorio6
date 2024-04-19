/*************************************************
 * Universidad del Valle de Guatemala
 * IE2023 Programacion de Microcontroladores
 * PreLab6.c
 * Autor: Larsson Gonzalez
 * Hardware: ATMega328P
 * Proyecto:
 * Created: 19/4/2024 09:17:45
 ************************************************/ 

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

//************************************************
// Declaration of Variables
//************************************************
volatile uint8_t bufferTx;
unsigned char PuertoC;

//************************************************
// Declaration of Functions
//************************************************
void initUART(void);
/*void writeUART(char caracter);*/
void writeTextUART(char * Texto);
unsigned char UART_read();
//************************************************
// Setup
//************************************************
int main(void)
{
    
	cli();
	
	DDRC = 0xFF;
	DDRB |= (1<<DDB4)|(1<<DDB5);
	
	initUART();
	
	/*writeTextUART("1. Leer pot");
	writeUART(10);
	writeUART(13);
	writeTextUART("2. Enviar ascii");
	writeUART(10);
	writeUART(13);*/
	
	sei();
	//********************************************
	// Loop
	//********************************************
    while (1){
		bufferTx = UART_read();
		if (bufferTx != 0){
			PORTC = bufferTx & 0x7F;
			
			if (bufferTx & 0x80)
			{
				PORTB |= (1<<PORTB4);
			}
			else
			{
				PORTB &= ~(1<<PORTB4);
			}
			
			if (bufferTx & 0x40)
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

//************************************************
// Functions
//************************************************

/****************Funcion de inicio****************/
void initUART(void){	
	DDRD &= ~(1<<DDD0);		// RX como entrada
	DDRD |= (1<<DDD1);		// TX como salida
	
	// Configuration UCSR0A
	UCSR0A = 0;
	
	// Configuration UCSR0B when enable ISR Reception, enable RX and TX
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0); 
	
	// Configuration UCSR0C - Mode asynchronous, Parity = None, One bit Stop, Data bits = 8bits
	UCSR0C = 0;
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	
	// Configuration velocity baudrate = 9600B
	UBRR0 = 103;
}

/****************Funcion de Escritura****************/
void writeTextUART(char * Texto){
	uint8_t i;
	for (i=0; Texto[i]!='\0'; i++){
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0 = Texto[i];
	}
}

/****************Funcion de Lectura****************/
/*void writeUART(char caracter){
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = caracter;
}*/

unsigned char UART_read(){
	if (UCSR0A & (1<<7)){
		return UDR0;
	} else {
		return 0;
	}
}
//************************************************
// ISR
//************************************************
ISR(USART_RX_vect){
	bufferTx = UDR0;
	UDR0 = bufferTx;
	UART_read(UDR0);
}