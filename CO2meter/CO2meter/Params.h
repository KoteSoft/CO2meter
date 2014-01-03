/*
 * Params.h
 *
 * Created: 19.12.2013 21:04:46
 *  Author: Слава
 */ 


#ifndef PARAMS_H_
#define PARAMS_H_

enum
{
	WORKING		=	0,
	PREHEATING	=	1,
	SETTINGS1	=	2,
	SETTINGS2	=	3	
};

typedef enum
{
	EMF0,
	DELTA_EMF,
	K_AMP,
	LOW_LIM,
	HIGH_LIM,
	
	SAVED_PARAMETERS_LIST_SIZE
}savedParametersEnum;

typedef enum
{
	MODE_BUTTON	= 2,
	SET_BUTTON	= 3,
}buttons;

extern float savedParametersList[SAVED_PARAMETERS_LIST_SIZE];

void LoadSavedParameters();

#endif /* PARAMS_H_ */