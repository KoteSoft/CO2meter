/*
 * CO2meter.cpp
 *
 * Created: 19.12.2013 19:16:00
 *  Author: Слава
 */ 


#include <avr/io.h>
#include "Display.h"
#include <avr/interrupt.h>
#include "Measurements.h"
#include "Params.h"

int main(void)
{
	DDRB = 0xFF;
	DDRC = 0b11110010;
	DDRD = 0xFF;
	
	LoadSavedParameters();
	DisplayTimer0Init();
	ADCInit();
	
	sei();
	
	Preheating();
	
    while(1)
    {
        Working();
    }
}