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
#include <avr/eeprom.h>

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
	
	for(uint16_t i = 0; i < 3600; i++)//прогрев полчаса
	{
		displayValue = ((ADCGetVoltage() / savedParametersList[K_AMP]) / savedParametersList[EMF0]) * 100;
		_delay_ms(500);
		if (fabs(ADCGetVoltage() / savedParametersList[K_AMP] - savedParametersList[EMF0]) < 0.03)//если прогрелось быстрее
			break;
	}
	
	displayValue = 0;
	_delay_ms(3000);
	
	displayMode = WORKING;
}

void Working()
{
	displayMode = WORKING;
	float measuredValue = ((pow(10.0, ((ADCGetVoltage() / savedParametersList[K_AMP] - savedParametersList[EMF0]) / savedParametersList[DELTA_EMF]) * (log10(400.0) - log10(1000.0)) + log10(400.0)))/10000);
	uint16_t vall = (uint16_t)(measuredValue * 100);
	if (vall>250)
	{
		displayValue = 0;
		measuredValue = 0;
	}
	else
	{
		displayValue = (uint16_t)vall;
	}
	
	
	if ((measuredValue + 0.001 > savedParametersList[HIGH_LIM]) && (savedParametersList[HIGH_LIM] > 0.0)) 
	{		
		PORTC |= 1<<PORTC1;
		_delay_ms(500);
		PORTC &= ~(1<<PORTC1);
		_delay_ms(50);		
	}
	
	else if(measuredValue < savedParametersList[LOW_LIM])
	{
		PORTC |= 1<<PORTC1;
		_delay_ms(50);
		PORTC &= ~(1<<PORTC1);
		_delay_ms(50);
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
	displayValue = (uint16_t)(savedParametersList[HIGH_LIM] * 100);
	
	uint8_t pressCounter = 0;
	
	while(bit_is_set(PINC, MODE_BUTTON))
	{}
	
	_delay_ms(100);
	
	while(bit_is_clear(PINC, MODE_BUTTON))
	{
		if (bit_is_set(PINC, SET_BUTTON))
		{
			pressCounter++;
			if(savedParametersList[HIGH_LIM] < 0.99)
				savedParametersList[HIGH_LIM] += 0.01;
			else
				savedParametersList[HIGH_LIM] = 0.00;
		}
		else
		{
			pressCounter = 0;
		}
		
		if (pressCounter < 10)
		{
			_delay_ms(500);
		} 
		else
		{
			_delay_ms(100);
		}
		
		displayValue = (uint16_t)(savedParametersList[HIGH_LIM] * 100);
	}
		
	if (eeprom_read_float((float*)(HIGH_LIM * 4)) != savedParametersList[HIGH_LIM])
	{
		eeprom_write_float((float*)(HIGH_LIM * 4), savedParametersList[HIGH_LIM]);
	}
		
	Settings2();
	
	while(bit_is_set(PINC, MODE_BUTTON))
	{}
}

void Settings2()
{
	displayMode = SETTINGS2;
	displayValue = (uint16_t)(savedParametersList[LOW_LIM] * 100);
	
	uint8_t pressCounter = 0;
	
	while(bit_is_set(PINC, MODE_BUTTON))
	{}
	
	_delay_ms(500);
	
	while(bit_is_clear(PINC, MODE_BUTTON))
	{
		if (bit_is_set(PINC, SET_BUTTON))
		{
			pressCounter++;
			if(savedParametersList[LOW_LIM] < 0.99)
				savedParametersList[LOW_LIM] += 0.01;
			else
				savedParametersList[LOW_LIM] = 0.00;
		}
		else
		{
			pressCounter = 0;
		}
		
		if (pressCounter < 10)
		{
			_delay_ms(500);
		}
		else
		{
			_delay_ms(100);
		}
		
		displayValue = (uint16_t)(savedParametersList[LOW_LIM] * 100);
	}
	
	if (eeprom_read_float((const float*)(LOW_LIM * 4)) != savedParametersList[LOW_LIM])
	{
		eeprom_write_float((float*)(LOW_LIM * 4), savedParametersList[LOW_LIM]);
	}
}

void Debugging()
{
	displayMode = DEBUGGING;
	float measuredValue = ADCGetVoltage() / savedParametersList[K_AMP];
	uint16_t vall = (uint16_t)(measuredValue * 1000);
	if (vall>999)
	{
		displayValue = 999;
		measuredValue = 0;
	}
	else
	{
		displayValue = (uint16_t)vall;
	}
	
	_delay_ms(500);	
}