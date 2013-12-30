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
	/*
	for (uint8_t i = 0; i < SAVED_PARAMETERS_LIST_SIZE; i++)
	{
		savedParametersList[i] = eeprom_read_float(i * 4);
	}
	*/
	
	savedParametersList[EMF0] = 0.3;
	savedParametersList[DELTA_EMF] = 0.05;
	savedParametersList[K_AMP] = 16.0;
}