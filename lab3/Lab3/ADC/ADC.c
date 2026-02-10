/*
 * ADC.c
 *
 * Created: 1/27/2026 4:38:19 PM
 *  Author: diego
 */ 
#define  F_CPU 16000000UL
#include "ADC.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t canal = 0; //bandera de canal a leer
volatile uint8_t adc3 = 0; // valor de adc3 
volatile uint8_t adc4 = 0; // valor de adc4


void init_ADC(void)
{
	cli();
	ADMUX = 0; // LIMPIEZA 
	canal = 0; 
	ADMUX |= (1 << REFS0) | (1 << ADLAR); // AVcc como referencia y justificado hacia la izquierda
	ADCSRA = 0; // LIMPIEZA DEL REGISTRO
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // ENCENDIDO DE ADC, HABILITACION DE INTERRUPCION Y PRESCALER 128 DE ADC
	ADMUX |= (1 << MUX0) | (1 << MUX1); // SELECCION DE CANAL DE ADC
	ADCSRA |= (1 << ADSC);// EMPEZAR CONVERSION DE ADC
	sei();
}

ISR(ADC_vect)
{
	switch(canal)
	{
		case 0:
		ADMUX = (ADMUX & 0xF8) | (1 << MUX0) | (1 << MUX1); //referencia y canal PC3
		adc3 = ADCH;
		canal = 1;//CAMBIO DE BANDERA PARA LEER OTRO CANAL
		break;
		case 1:
		ADMUX = (ADMUX & 0xF8) | (1 << MUX2); // SELECCION DE REFERENCIA Y CANAL PC4 
		adc4 = ADCH;// TOMA EL VALOR DEL REGISTRO ADC Y LO GUARDA EN LA VARIABLE
		canal = 0;
		break;
	}
	ADCSRA |= (1 << ADSC); // COMIENZA LA SIGUIENTE CONVERSION
}