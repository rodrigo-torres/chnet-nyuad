/******************************************************************************
* 
* CAEN SpA - Front End Division
* Via Vetraia, 11 - 55049 - Viareggio ITALY
* +390594388398 - www.caen.it
*
***************************************************************************//**
* \note TERMS OF USE:
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software
* Foundation. This program is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. The user relies on the 
* software, documentation and results solely at his own risk.
******************************************************************************/

#ifndef PARAMETER_H
#define PARAMETER_H

#include "Configurator.h"

struct Parameter_t;

typedef struct Parameter_t { /* A Parameter represents a configuration parameter */
	char* section;
	char* key;
	char* value;
	struct Parameter_t* next; /* b/c parameters are kept in a linked list */
	configFunc action; /* pointer to function to perform update actions */
} Parameter;

Parameter* newParameter(const char* s, const char* k, const char* v, configFunc action);
void deleteParameter(Parameter* param);

void setParameterValue(Parameter* param, const char* newval);





#endif