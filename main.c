/*
 * Humidity.c
 *
 * Created: 2016-07-12 12:44:25
 * Author : Phil
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <math.h>
#include "ssd1306.h"

//definicja pinów
#define MOTOR (1<<PD4)
#define MOTOR_ON PORTD |= MOTOR
#define MOTOR_OFF PORTD &= ~MOTOR

#define LED (1<<PD0)
#define LED_ON PORTD &= ~LED
#define LED_OFF PORTD |= LED

#define ADCIN1 PA0
#define ADCIN2 PA1

//zmienne
int now=0, set=0;
volatile uint8_t var3=0;
volatile uint16_t var1, var2=0;

//funkcje
void ADC_init();
void PWM_ON();
void PWM_OFF();

int main( void )
{
	
	DDRD |= ( MOTOR | LED );		 //wyjscia
	DDRA &=~ ( ADCIN1 | ADCIN2 );	 //wejscia
						
	//timer na 10MHz/1024/255=38Hz
	TCCR2 |= (1<<WGM21);
	OCR2 = 0xFF;
	TCCR2 |= (1<<CS22) | (1<<CS21) | (1<<CS20);					//preskaler 1024
	TIMSK |= (1<<OCIE2);
	
	ADC_init();
	sei();		//pozwolenie na przerwania
	ADCSRA |= (1<<ADSC);
	
	ssd1306_init( SSD1306_SWITCHCAPVCC, REFRESH_MIN );
	ssd1306_puts( 0, 0, "HUMIDITY ", 2, 1, 0 );
	ssd1306_puts( 0, 16, "Now: ", 2, 1, 0 );
	ssd1306_puts( 0, 32, "Set: ", 2, 1, 0 );
	ssd1306_puts( 114, 16, "%", 2, 1, 0 );
	ssd1306_puts( 114, 32, "%", 2, 1, 0 );
	
    while (1) 
    {
		if( var3==1 )
		{
			for(int i=48, j=0; i<64; i++, j--)
			{
				if (j==5)
				{
					j=0;
				}
				ssd1306_puts( j, i, "/  /  /  /  /  /  /  /", 1, 1, 0 );
				ssd1306_puts( j, i+5, " /  /  /  /  /  /  /  /  ", 1, 1, 0 );
				ssd1306_display();
			}
		}
	
		ssd1306_puts( 58, 32, "    ", 2, 1, 0); //czysczenie kawalka ekranu
		ssd1306_put_int( 58, 32, set, 2, 1, 0 );
		ssd1306_puts( 58, 16, "    ", 2, 1, 0); //czysczenie kawalka ekranu
		ssd1306_put_int( 58, 16, now, 2, 1, 0 );
		ssd1306_display();
		
    }
}

ISR(TIMER2_COMP_vect) //w tym przerwaniu ustalamy co jaki czas mamy porównywaæ set z now i na jaki czas zapalaæ silnik
{
	if (var1==300)
	{
		if(now<set)
		{
			LED_ON;
			PWM_ON();
			var3=1;
			var2++;
			if (var2>30)
			{
				LED_OFF;
				PWM_OFF();
				var1=0;
				var2=0;
				var3=0;
			}
		}
		else
		{
			var1=0;
		}
		
	}
	else
	{
		LED_OFF;
		PWM_OFF();
		var1++;
		var3=0;
	}
}

ISR(ADC_vect)
{
	uint8_t theLow = ADCL;
	uint16_t theTenBitResult = ADCH<<8 | theLow;
	
	switch(ADMUX)
	{
		case 0x40:
			set=theTenBitResult/10.23;
			ADMUX=0x41;
			break;
			
		case 0x41:
			now=theTenBitResult/10.23;
			ADMUX=0x40;
			break;
			
		default:
			break;
	}
	
	ADCSRA |= (1<<ADSC);
}

void ADC_init()
{
	ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);		//wlaczenie ADC i ustawienie f próbkowania
	ADMUX |= (0<<REFS1) | (1<<REFS0);											//ustawienie wartoœci odniesienia dla ADC (AVCC)
}

void PWM_ON()
{
															//ustawienie wszystkiego do PWMa
	TCCR1A |= (1<<WGM11);									//wybranie trybu Fast PWM z wartoscia max w ICR1 i update OCR1A
	TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS10);			//po resecie ICR1 (BOTTOM)
	TCCR1A |= (0<<COM1B0) | (1<<COM1B1);					//wlaczenie PWM
	ICR1 = 10000-1;											//f PWMa=10kHz (10MHz/10000=1kHz)
	OCR1B = 5000;											//Wartoœæ pocz¹tkowa porównania B
															//wype³nienie = 50% (5000/10000=1/2=50%)	
}

void PWM_OFF()
{
	OCR1B = 0;	
}