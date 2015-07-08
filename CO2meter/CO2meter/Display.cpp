/*
 * Display.cpp
 *
 * Created: 19.12.2013 19:25:08
 *  Author: Слава
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Params.h"

uint16_t displayValue = 123;
uint8_t displayDigit = 0;
uint8_t displayMode = PREHEATING;
uint8_t displayAnim = 0;
uint32_t displayAnimPre = 0;

uint8_t GetDigit(uint8_t digit);
uint8_t OutWorking(uint16_t val, uint8_t digit);
uint8_t OutSettings1(uint16_t val, uint8_t digit);
uint8_t OutSettings2(uint16_t val, uint8_t digit);
uint8_t OutPreheating(uint16_t val, uint8_t digit);
uint8_t OutDebugging(uint16_t val, uint8_t digit);

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

uint8_t OutWorking(uint16_t val, uint8_t digit)
{	
	if (digit > 3 || digit < 1)
	{
		return 1;
	}	
	
	uint8_t A, B, C;
	
	A = (uint8_t)(val / 100);
	B = (uint8_t)((val % 100) / 10);
	C = (uint8_t)(val % 10);
	
	PORTD = 1 << (digit + 1);
	PORTB = 0xFF;
	
	switch (digit)
	{
		case 1: PORTB = GetDigit(A) & 127U; PORTD |= PORTB & 0b11000000; return 0;
		case 2: PORTB = GetDigit(B); PORTD |= PORTB & 0b11000000; return 0;
		case 3: PORTB = GetDigit(C); PORTD |= PORTB & 0b11000000; return 0;
	}
	
	return 2;	 
}

uint8_t OutDebugging(uint16_t val, uint8_t digit)
{
	if (digit > 3 || digit < 1)
	{
		return 1;
	}
	
	uint8_t A, B, C;
	
	A = (uint8_t)(val / 100);
	B = (uint8_t)((val % 100) / 10);
	C = (uint8_t)(val % 10);
	
	PORTD = 1 << (digit + 1);
	PORTB = 0xFF;
	
	switch (digit)
	{
		case 1: PORTB = GetDigit(A); PORTD |= PORTB & 0b11000000; return 0;
		case 2: PORTB = GetDigit(B); PORTD |= PORTB & 0b11000000; return 0;
		case 3: PORTB = GetDigit(C); PORTD |= PORTB & 0b11000000; return 0;
	}
	
	return 2;
}

uint8_t OutSettings1(uint16_t val, uint8_t digit)
{
	if (digit > 3 || digit < 1)
	{
		return 1;
	}
	
	uint8_t B, C;
	
	B = (uint8_t)((val % 100) / 10);
	C = (uint8_t)(val % 10);
	
	PORTD = 1 << (digit + 1);
	PORTB = 0xFF;
	
	switch (digit)
	{
		case 1: PORTB = 0b11111110; PORTD |= PORTB & 0b11000000; return 0;
		case 2: PORTB = GetDigit(B); PORTD |= PORTB & 0b11000000; return 0;
		case 3: PORTB = GetDigit(C); PORTD |= PORTB & 0b11000000; return 0;
	}
	
	return 2;
}

uint8_t OutSettings2(uint16_t val, uint8_t digit)
{
	if (digit > 3 || digit < 1)
	{
		return 1;
	}
	
	uint8_t B, C;
	
	B = (uint8_t)((val % 100) / 10);
	C = (uint8_t)(val % 10);
	
	PORTD = 1 << (digit + 1);
	PORTB = 0xFF;
	
	switch (digit)
	{
		case 1: PORTB = 0b11110111; PORTD |= PORTB & 0b11000000; return 0;
		case 2: PORTB = GetDigit(B); PORTD |= PORTB & 0b11000000; return 0;
		case 3: PORTB = GetDigit(C); PORTD |= PORTB & 0b11000000; return 0;
	}
	
	return 2;
}

uint8_t OutPreheating(uint16_t val, uint8_t digit)
{
	if (displayAnimPre > 70)
	{
		displayAnimPre = 0;
		if (displayAnim<5)
		{
			displayAnim++;
		} 
		else
		{
			displayAnim = 0;
		}
	} 
	else
	{
		displayAnimPre++;
	}
	
	if (digit > 3 || digit < 1)
	{
		return 1;
	}
	
	uint8_t B, C;
	
	B = (uint8_t)((val % 100) / 10);
	C = (uint8_t)(val % 10);
	
	PORTD = 1 << (digit + 1);
	PORTB = 0xFF;
	
	if (val > 0)
	{
		switch (digit)
			{
				case 1: PORTB = 0xFF & ~(1 << displayAnim); PORTD |= PORTB & 0b11000000; return 0;
				case 2: PORTB = GetDigit(B); PORTD |= PORTB & 0b11000000; return 0;
				case 3: PORTB = GetDigit(C); PORTD |= PORTB & 0b11000000; return 0;
			}
		
	}
	
	else if (digit == 1)
	{
		PORTB = 0xFF & ~(1 << displayAnim); 
		PORTD |= 0b11000000; 
		return 0;
	}
	
	else
	{
		PORTD |= 0b11000000;
		return 0;
	}
		
	return 2;
}

void DisplayTimer0Init()
{
	TCCR0 = 0<<CS02 | 1<<CS01 | 1<<CS00;
	TIMSK = 1<<TOV0;
}

ISR(TIMER0_OVF_vect)
{
	switch(displayMode)
	{
		case WORKING: OutWorking(displayValue, displayDigit); break;
		case PREHEATING: OutPreheating(displayValue, displayDigit); break;
		case SETTINGS1: OutSettings1(displayValue, displayDigit); break;
		case SETTINGS2:	OutSettings2(displayValue, displayDigit); break;
		case DEBUGGING: OutDebugging(displayValue, displayDigit);		
	}
	
	if (displayDigit<3)
	{
		displayDigit++;
	}
	else
	{
		displayDigit = 1;
	}
}