/*
 * Lab3.c
 *
 * Created: 2/3/2026 4:07:23 PM
 * Author : diego
 */ 
//Librerias
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "ADC/ADC.h"
#include "SPI/SPI.h"



//Variables
uint8_t valor1 = 0;
uint8_t valor2 = 0; 
uint8_t v_serial = 0;
uint8_t turno = 0;
uint8_t bandera;
uint8_t temporal;


//Function prototypes
void refreshPORT(uint8_t valor);
void setup();
//Main

int main(void)
{
	cli();
	setup();
	spiInit(SPI_SLAVE_SS, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_LOW, SPI_CLOCK_FIRST_EDGE);
	init_ADC();
	SPCR |= (1 << SPIE);
	sei();
	while (1)
	{											
	}
}

//NON-Interrupt subroutines
void setup(){
	//UCSR0B = 0x00;
	DDRD |= (1 << PORTD0)| (1 << PORTD1) |(1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7);				//SALIDA LEDS
	
	PORTD &= ~((1 << PORTD0) | (1 << PORTD1) | (1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7));
}

void refreshPORT(uint8_t valor){
	PORTD = valor;
	/*
	if (valor & 0b10000000)
	{
		PORTD |= (1 << PORTD0);
		}else{
		PORTD &= ~(1 << PORTD0);
	}
	
	if (valor & 0b01000000)
	{
		PORTD |= (1 << PORTD1);
		}else{
		PORTD &= ~(1 << PORTD1);
	}
	
	if (valor & 0b00100000)
	{
		PORTD |= (1 << PORTD2);
		}else{
		PORTD &= ~(1 << PORTD2);
	}
	
	if (valor & 0b00010000)
	{
		PORTD |= (1 << PORTD3);
		}else{
		PORTD &= ~(1 << PORTD3);
	}
	
	if (valor & 0b00001000)
	{
		PORTD |= (1 << PORTD4);
		}else{
		PORTD &= ~(1 << PORTD4);
	}
	
	if (valor & 0b00000100)
	{
		PORTD |= (1 << PORTD5);
		}else{
		PORTD &= ~(1 << PORTD5);
	}
	
	if (valor & 0b00000010)
	{
		PORTD |= (1 << PORTD6);
		}else{
		PORTD &= ~(1 << PORTD6);
	}
	
	if (valor & 0b00000001)
	{
		PORTD |= (1 << PORTD7);
		}else{
		PORTD &= ~(1 << PORTD7);
	}
	*/
}

ISR(SPI_STC_vect)
{
	uint8_t rx = SPDR;
	
	if (bandera == 1)
	{
		refreshPORT(rx);
		bandera=0;
	}else
	{
		if (rx == 'c')
		{
			SPDR = adc3;
		}
		if (rx == 'a')
		{
			SPDR = adc4;
		}
		if (rx == 'w')
		{
			bandera = 1;
		}
	}
	//temporal = rx;
	/*
	switch(turno)
	{
		case 0:
			turno = 1;
			
			if (rx == 'c')
			{
				valor1 = adc3;
				SPDR = valor1;
			}
			
			if (rx == 'a')
			{
				valor2 = adc4;
				SPDR = valor2;
			}
			break;
			
		case 1:
			turno = 0;
			if (rx == 'p')
			{
				v_serial = rx;
				refreshPORT(v_serial);
			}
			
			break;
	}
	*/
}
