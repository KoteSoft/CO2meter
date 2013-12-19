/*
 * CO2meter.cpp
 *
 * Created: 19.12.2013 19:16:00
 *  Author: Слава
 */ 


#include <avr/io.h>
#include "Display.h"
#include <avr/interrupt.h>

int main(void)
{
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;
	
	DisplayTimer0Init();
	
	sei();
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}