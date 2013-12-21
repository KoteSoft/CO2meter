/*
 * Measurements.cpp
 *
 * Created: 20.12.2013 12:53:56
 *  Author: Слава
 */ 

#include "Measurements.h"
#include <avr/io.h>
#include "Params.h"
#include "Display.h"
#include <util/delay.h>
#include <math.h>
#include <stddef.h>

void Settings1();
void Settings2();

void ADCInit()
{
	ADMUX = 0<<REFS1 | 0<<REFS0; 
	ADCSRA = 1<<ADEN | 1<<ADSC | 1<<ADFR | 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0;
}

float ADCGetVoltage()
{
	return (ADC * 5.0) / 1023.0;
}

void Preheating()
{
	displayMode = PREHEATING;
	displayValue = 0;
	_delay_ms(3000);
	
	while(ADCGetVoltage() / savedParametersList[K_AMP] < savedParametersList[EMF0])
	{
		displayValue = ((ADCGetVoltage() / savedParametersList[K_AMP]) / savedParametersList[EMF0]) * 100;
	}
	
	displayValue = 0;
	_delay_ms(3000);
	
	displayMode = WORKING;
}

void Working()
{
	displayMode = WORKING;
	uint16_t vall = (uint8_t)((pow(10.0, ((ADCGetVoltage() / savedParametersList[K_AMP] - savedParametersList[EMF0]) / savedParametersList[DELTA_EMF]) * (log10(400.0) - log10(1000.0)) + log10(400.0)))/100);
	if (vall>250)
	{
		displayValue = 0;
	}
	else
	{
		displayValue = (uint8_t)vall;
	}
	
	_delay_ms(1000);
	
	if (bit_is_set(PINC, MODE_BUTTON))
	{
		Settings1();
	}
}

void Settings1()
{
	displayMode = SETTINGS1;
	
	while(bit_is_set(PINC, MODE_BUTTON))
	{}
	
	_delay_ms(20);
	
	while(bit_is_clear(PINC, MODE_BUTTON))
	{}
		
	Settings2();
}

void Settings2()
{
	displayMode = SETTINGS2;
	
	while(bit_is_set(PINC, MODE_BUTTON))
	{}
	
	_delay_ms(20);
	
	while(bit_is_clear(PINC, MODE_BUTTON))
	{}
}