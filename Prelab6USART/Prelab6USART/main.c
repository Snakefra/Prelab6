/*****************************************************************************
 * Universidad del Valle de Guatemala
 * IE2023: Programaci?n de microcontroladores
 * Autor: Oscar Melchor
 * Proyecto: Comunicación serial USART
 * Archivo: Prelab6USART.c
 * Hardware : ATMEGA328P
 * Created: 24/04/2024 08:16:22
 *****************************************************************************/

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void initUART9600(void);
void writeTextUART(char* texto);

volatile char bufferRX;		


int main(void)
{
	DDRD = 0b11100000;		//PD5 - PD7 Output
	DDRB = 0b00011111;		//PB0 - PB4 Output
	
	initUART9600();
	sei();
	
	
	writeTextUART("Hola Mudo\n");
    while (1) 
    {
		
    }
}

void initUART9600(void){
	//Configuración Rx y Tx
	DDRD &= ~(1 << DDD0);		//Rx entrada
	DDRD |= (1 << DDD1);		//TX salida
	
	//Modo Fast, U2X0
	UCSR0A = 0;
	UCSR0A |= (1 << U2X0);
	
	//Configuración registro B
	UCSR0B = 0;
	UCSR0B |= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0); //Configurando interrupción ISR
	
	//Configuración registro C
	UCSR0C = 0;
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);		//Configuración 8 bits, sin paridad y 1 stop bit
	
	//Baudrate
	UBRR0 = 207;		// 9600
}

void writeTextUART(char* texto){
	uint8_t i;
	for (i = 0; texto[i] != '\0'; i++)
	{
		while(!(UCSR0A & (1 << UDRE0)));
		UDR0 = texto[i];
	}
}
//ISR Leer
ISR(USART_RX_vect){
	bufferRX = UDR0;
	
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = bufferRX;
	PORTD = (0b11100000 & bufferRX);
	PORTB = (0b00011111 & bufferRX);
}