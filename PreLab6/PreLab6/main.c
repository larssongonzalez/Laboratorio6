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
volatile uint8_t datoTX;
volatile char readchar;
int activar;
int activar1;
int dato;
char lista[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

//************************************************
// Declaration of Functions
//************************************************
void initADC(void);
void initUART(void);
void writeUART(char caracter);
void writeTextUART(char * Texto);

//************************************************
// Setup
//************************************************
int main(void)
{
    
	cli();
	
	DDRC = 0xFF;
	DDRB |= (1<<DDB4)|(1<<DDB5);
	
	initUART();
	initADC();
	
	
	sei();
	//********************************************
	// Loop
	//********************************************
    while (1){
		
		ADCSRA |= (1<<ADSC);
		
		if (activar == 0)
		{
			writeTextUART("\n\r--------Menu--------\n\r");
			writeTextUART("1. Leer Potenciometro\n\r");
			writeTextUART("2. Enviar ASCII\n\r");
			
			activar = 1;
		}
		 
		if (readchar != 0){
			switch(readchar){
				case '1':
				writeTextUART("Mueve el Potenciometro");
				writeTextUART("Su valor actual es: ");
				
				uint8_t lbits = dato & 0b00111111; 
				uint8_t hbits = (dato>>6) & 0b11;
				
				PORTC = lbits;
				
				PORTB = (PORTB & ~0b00110000) | (hbits<<6);
				
				int millares = dato/1000;
				int centenas = (dato-(millares*1000))/100;
				int decenas = (dato-(millares*1000 + centenas*100))/10;
				int unidades = dato-(millares*1000 + centenas*100 + decenas*10);
				switch(centenas){
					case '0':
						writeUART(lista[0]);
						break;
					
					case '1':
						writeUART(lista[1]);
						break;
					
					case '2':
						writeUART(lista[2]);
						break;
					
					case '3':
						writeUART(lista[3]);
						break;
					
					case '4':
						writeUART(lista[4]);
						break;
					
					case '5':
						writeUART(lista[5]);
						break;
					
					case '6':
						writeUART(lista[6]);
						break;
					
					case '7':
						writeUART(lista[7]);
						break;
					
					case '8':
						writeUART(lista[8]);
						break;
					
					case '9':
						writeUART(lista[9]);
						break;
				}
				
				switch(decenas){
					case '0':
						writeUART(lista[0]);
						break;
					
					case '1':
						writeUART(lista[1]);
						break;
					
					case '2':
						writeUART(lista[2]);
						break;
					
					case '3':
						writeUART(lista[3]);
						break;
					
					case '4':
						writeUART(lista[4]);
						break;
					
					case '5':
						writeUART(lista[5]);
						break;
					
					case '6':
						writeUART(lista[6]);
						break;
					
					case '7':
						writeUART(lista[7]);
						break;
					
					case '8':
						writeUART(lista[8]);
						break;
					
					case '9':
						writeUART(lista[9]);
						break;
				}
				
				switch(unidades){
					case '0':
						writeUART(lista[0]);
						break;
					
					case '1':
						writeUART(lista[1]);
						break;
					
					case '2':
						writeUART(lista[2]);
						break;
					
					case '3':
						writeUART(lista[3]);
						break;
					
					case '4':
						writeUART(lista[4]);
						break;
					
					case '5':
						writeUART(lista[5]);
						break;
					
					case '6':
						writeUART(lista[6]);
						break;
					
					case '7':
						writeUART(lista[7]);
						break;
					
					case '8':
						writeUART(lista[8]);
						break;
					
					case '9':
						writeUART(lista[9]);
						break;
				}
				
					activar = 0;
					readchar = 0;
				break;
				
				case '2':
				readchar = 0;
				writeTextUART("\n\rIngresa Texto\n\r");
				activar1 = 1;
				break;
			}
		}
    }
}

//************************************************
// Functions
//************************************************
void initADC(){
	// Seleccion de Canal ADC (A)
	ADMUX = 6;
	
	// Utilizando AVCC = 5V internos
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	
	// Justificacion a la Izquierda
	ADMUX |= (1<<ADLAR);
	
	ADCSRA = 0;
	
	// Habilitando el ADC
	ADCSRA |= (1<<ADEN);
	
	//Habilitamos las interrupciones
	ADCSRA |= (1<<ADIE);
	
	// Habilitamos el Prescaler de 128
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	DIDR0 |= (1<<ADC0D);
}
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

/****************Funcion de Escritura de una cadena de Caracteres****************/
void writeTextUART(char * Texto){
	uint8_t i;
	for (i=0; Texto[i]!='\0'; i++){
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0 = Texto[i];
	}
}

/****************Funcion de Escritura de un Caracter****************/
void writeUART(char caracter){
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = caracter;
}


//************************************************
// ISR
//************************************************
ISR(USART_RX_vect){
	readchar = UDR0;
	
	if (activar1==1){
		uint8_t lbits = readchar & 0b00111111;
		uint8_t hbits = (readchar>>6) & 0b11;
		
		PORTC = lbits;
		
		PORTB = (PORTB & ~0b11000000)|(hbits<<6);
		activar1 = 0;
		activar = 0;
	}
	
	while(!(UCSR0A & (1<<UDRE0)));
		
	UDR0 = readchar;
	
}

ISR(ADC_vect){
	dato = ADCH;
	ADCSRA |= (1<<ADIF);
}