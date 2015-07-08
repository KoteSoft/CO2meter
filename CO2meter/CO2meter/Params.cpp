/*
 * Params.cpp
 *
 * Created: 20.12.2013 12:20:14
 *  Author: Слава
 */ 
#include "Params.h"
#include <avr/eeprom.h>

float savedParametersList[SAVED_PARAMETERS_LIST_SIZE];

void LoadSavedParameters()
{
	if (eeprom_read_float((float*)(HIGH_LIM * 4)) > 0.99 || eeprom_read_float((float*)(HIGH_LIM * 4)) < 0.00)
	{
		eeprom_write_float((float*)(HIGH_LIM * 4), 0.5);
	}
	
	if (eeprom_read_float((float*)(LOW_LIM * 4)) > 0.99 || eeprom_read_float((float*)(LOW_LIM * 4)) < 0.00)
	{
		eeprom_write_float((float*)(LOW_LIM * 4), 0.01);
	}
	
	
	for (uint8_t i = 0; i < SAVED_PARAMETERS_LIST_SIZE; i++)
	{
		savedParametersList[i] = eeprom_read_float((const float*)(i * 4));
	}
		
	savedParametersList[EMF0] = 0.162;
	savedParametersList[DELTA_EMF] = 0.037;//0,05
	savedParametersList[K_AMP] = 6.5;
}