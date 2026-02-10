	/*
 * laboratorio3_digital2.c
 *
 * Created: 2/02/2026 22:17:30
 * Author : jose_
 */ 
//Librerias
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>													//uints, ints, etc
#include <stdio.h>													//snprintf
#include <stdlib.h>													//atoi



//Incluir librerias hechas
#include "SPI/SPI.h"
#include "UART/UART.h"

//Variables

uint16_t valorSPI = 0;
uint16_t valorSPI2 = 0;
char buffer1[30];
char buffer2[30];

//----UART---//
char signal;
char buffer_datos[4];
uint8_t contenido;
uint8_t numero_8bits;
uint8_t leer;
uint8_t caracter;


//Function prototypes
void refreshPORT(uint8_t valor);
void setup();
void menu();


//Main

int main(void)
{
    setup();
	spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_LOW, SPI_CLOCK_FIRST_EDGE);
	init_UART();
	sei();
	menu();
    while (1) 
    {
		if (leer == 1)
		{
			leer = 0;
			PORTC &= ~(1 << PORTC5);						//poner en 0 al esclavo
			//------------POT 1------------------------//
			spiWrite('c');
			spiWrite(0x00);									//byte adicional (basura)
			valorSPI = spiRead();
			//while (!(SPSR & (1 << SPIF)));
			//valorSPI =  SPDR;
			
			snprintf(buffer1, sizeof(buffer1), "\n P1:%3u \n ", valorSPI);
			writeString(buffer1);
			//refreshPORT(valorSPI);
			//-----------POT 2 ----------------------//
			spiWrite('a');
			spiWrite(0x00);
			valorSPI2 = spiRead();
			//while (!(SPSR & (1 << SPIF)));
			//valorSPI2 =  SPDR;
			
			snprintf(buffer2, sizeof(buffer2), "\n P2:%3u \n ", valorSPI2);
			writeString(buffer2);
			//refreshPORT(valorSPI2);

			
			PORTC |= (1 << PORTC5);							//poner en 1 al esclavo
			_delay_ms(250);
			menu();
		}
				
    }
}

//NON-Interrupt subroutines
void setup(){
	buffer_datos[0] = '\0';
	DDRC |= (1 << PORTC5);																								//SALIDA ESCLAVO
	DDRB |= (1 << PORTB0) | (1 << PORTB1);																				//SALIDA LEDS
	DDRD |= (1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7);				//SALIDA LEDS
	
	PORTC &= ~(1 << PORTC5);
	PORTB &= ~((1 << PORTB0) | (1 << PORTB1));
	PORTD &= ~((1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7));
}

void menu()
{
	writeString("\n");
	writeString("Bienvenido al menú de la hiperterminal.\n");
	writeString("¿Qué desea hacer?\n");
	writeString("Para leer los potenciometros presione 'g'.\n");
	writeString("Para enviar un valor de 0 a 255 presione 'h'.\n");
	writeString(" Escoja una opción: ");
}

void refreshPORT(uint8_t valor){
	if (valor & 0b10000000)
	{
		PORTB |= (1 << PORTB1);
	}else{
		PORTB &= ~(1 << PORTB1);
	}
	
	if (valor & 0b01000000)
	{
		PORTB |= (1 << PORTB0);
	}else{
		PORTB &= ~(1 << PORTB0);
	}
	
	if (valor & 0b00100000)
	{
		PORTD |= (1 << PORTD7);
	}else{
		PORTD &= ~(1 << PORTD7);
	}
	
	if (valor & 0b00010000)
	{
		PORTD |= (1 << PORTD6);
		}else{
		PORTD &= ~(1 << PORTD6);
	}
	
	if (valor & 0b00001000)
	{
		PORTD |= (1 << PORTD5);
		}else{
		PORTD &= ~(1 << PORTD5);
	}
	
	if (valor & 0b00000100)
	{
		PORTD |= (1 << PORTD4);
		}else{
		PORTD &= ~(1 << PORTD4);
	}
	
	if (valor & 0b00000010)
	{
		PORTD |= (1 << PORTD3);
		}else{
		PORTD &= ~(1 << PORTD3);
	}
	
	if (valor & 0b00000001)
	{
		PORTD |= (1 << PORTD2);
		}else{
		PORTD &= ~(1 << PORTD2);
	}
}

//Interrupt routines

ISR(USART_RX_vect)
{
	signal = UDR0;
	
	if (numero_8bits == 1)
	{
		if (signal == '\n')
		{
			contenido = atoi(buffer_datos);
			writeString("\nNumero recibido\r\n");
			PORTC &= ~(1 << PORTC5);						// slave en 0
			spiWrite('w');
			spiWrite(contenido);
			refreshPORT(contenido);
			PORTC |= (1 << PORTC5);							//poner en 1 al esclavo
			
			caracter = 0;
			buffer_datos[0] = '\0';
			numero_8bits = 0;
			menu();
		}else if (caracter < 3)
		{
			buffer_datos[caracter++] = signal;
			buffer_datos[caracter] = '\0';
		}
	
	} 
	
	
	// ====== MODO MENU ======
	if (signal == 0x67)												// g en ascii
	{
		writeString("\n");
		writeString("------------------------------------");
		writeString("\n");
		leer = 1;
	}
	else if (signal == 0x68)										//h en ascii
	{
		numero_8bits = 1;
		writeString("\n");
		writeString("------------------------------------");
		writeString("\n");
		writeString("Ingrese un valor 0-255: ");
		
	}
}
