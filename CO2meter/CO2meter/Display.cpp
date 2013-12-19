﻿/*
 * Display.cpp
 *
 * Created: 19.12.2013 19:25:08
 *  Author: Слава
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t displayValue = 123;
uint8_t displayDigit = 0;

uint8_t GetDigit(uint8_t digit)
{
	switch (digit)
	{
		case 0: return 0b11000000; break;
		case 1: return 0b11111001; break;
		case 2: return 0b10100100; break;
		case 3: return 0b10110000; break;
		case 4: return 0b10011001; break;
		case 5: return 0b10010010; break;
		case 6: return 0b10000010; break;
		case 7: return 0b11011000; break;
		case 8: return 0b10000000; break;
		case 9: return 0b10010000; break;
		default:return 0b10110110; break;
	}
}

uint8_t OutDigit(uint8_t val, uint8_t digit)
{	
	if (digit > 3 || digit < 1)
	{
		return 1;
	}	
	
	uint8_t A, B, C;
	
	A = val / 100;
	B = (val % 100) / 10;
	C = val % 10;
	
	PORTD = 1 << (digit - 1);
	PORTB = 0xFF;
	
	switch (digit)
	{
		case 1: PORTB = GetDigit(A) & 127U; return 0;
		case 2: PORTB = GetDigit(B); return 0;
		case 3: PORTB = GetDigit(C); return 0;
	}
	
	return 2;	 
}

void DisplayTimer0Init()
{
	TCCR0 = 0<<CS02 | 1<<CS01 | 0<<CS00;
	TIMSK = 1<<TOV0;
}

ISR(TIMER0_OVF_vect)
{
	OutDigit(displayValue, displayDigit);
	
	if (displayDigit<3)
	{
		displayDigit++;
	} 
	else
	{
		displayDigit = 1;
	}
}