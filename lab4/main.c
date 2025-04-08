/*
 * lab4.c
 *
 * Created: 3/31/2025 6:27:21 PM
 * Author : laloj
 */ 

/****************************************/// Encabezado (Libraries)#define  F_CPU 16000000#include <avr/io.h>#include <avr/interrupt.h>uint8_t cuenta8bit;uint8_t test;uint8_t ADCDig;uint8_t ADCDec;int SieteSeg[16] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6D, 0x7D, 0x07, 0x7f,0x67, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};//uint8_t counter_10ms;/****************************************/// Function prototypesvoid setup();void initADC();/****************************************/// Main Functionint main(void)
{
	setup();
	while (1)
	{
		if (PORTB == 0x04) {
			PORTD = cuenta8bit;
		}
		if (PORTB == 0x01)
		{
			PORTD = ADCDec;
		}
		if (PORTB == 0x02)
		{
			PORTD = ADCDig;
		}
		
	}
}
/****************************************/// NON-Interrupt subroutinesvoid setup(){	cli();	DDRD	= 0xff;	DDRB	= 0Xff;	PORTD	= 0x00;	PORTB	= 0x01;					initADC();	DDRC	= 0x00;	PORTC	|= 0x07;		PCICR	= 0x02;	PCMSK1	= 0b00000011;	cuenta8bit = 0x00;			CLKPR = (1 << CLKPCE);	CLKPR = (1 << CLKPS2); // Prescales principal = 16	TCCR0A	= 0X00;	TCCR0B	= (1 << CS01) | (1 << CS00);	TCNT0 = 178;	TIMSK0 = (1 << TOIE0);	//counter_10ms = 0;		test = 0b00111111;	ADCDec = 0;	ADCDig = 0;	sei();}void initADC(){	ADMUX = 0;	ADMUX |= (1 << REFS0); //referencia = avcc	ADMUX |= (1 << ADLAR);	ADMUX |= (1 << MUX1) | (1 << MUX0); // ACtivando ADC3		ADCSRA = 0;	ADCSRA |= (1 << ADEN);	ADCSRA |= (1 << ADIE);	ADCSRA |= (1 << ADPS1) | (1 << ADPS0);		ADCSRA |= (1 << ADSC);}/****************************************/// Interrupt routines
ISR(PCINT1_vect){
		if (PINC & (1<<0))
		{
			cuenta8bit++;
		}
		if (PINC & (1<<1))
		{
			cuenta8bit--;
		}
	
	
}

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 178;
	//counter_10ms++;
		//counter_10ms = 0;
		PORTB = PORTB * 2;
		if (PORTB == 8)
		{
			PORTB = 0x01;
		}
	
	
}

ISR(ADC_vect)
{
	test = ADCH;
	ADCDig = test & 0x0F;
	ADCDig = SieteSeg[ADCDig];
	ADCDec = test & 0xF0;
	ADCDec = ADCDec / 16;
	ADCDec = SieteSeg[ADCDec];
	
	if (test > cuenta8bit)
	{
		PORTC |= 0b00000100;
	}
	else
	{
		PORTC &= 0b11111011;
	}
	
	ADCSRA |= (1 << ADSC);
}


