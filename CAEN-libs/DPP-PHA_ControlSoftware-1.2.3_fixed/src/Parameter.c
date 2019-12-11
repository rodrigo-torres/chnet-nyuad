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

#include "globs.h"
#include <stdlib.h>
#include <string.h>

#include "Parameter.h"



Parameter* newParameter(const char* s, const char* k, const char* v, configFunc action) {
	Parameter* param = (Parameter*) calloc(1,sizeof(Parameter));
	param->section = (char*) calloc(strlen(s)+1,sizeof(char));
	param->key = (char*) calloc(strlen(k)+1,sizeof(char));
	if (v != NULL) param->value = (char*) calloc(strlen(v)+1,sizeof(char));
	strcpy(param->section,s);
	strcpy(param->key,k);
	if (v  != NULL) strcpy(param->value,v);
	param->action = action;
	return param;
}

void deleteParameter(Parameter* param) {
	SAFE_FREE(param->section)
	SAFE_FREE(param->key)
	SAFE_FREE(param->value)
	free(param);
}

void setParameterValue(Parameter* param, const char* newval) {
	SAFE_FREE(param->value)
	param->value = (char*)calloc(strlen(newval)+1,sizeof(char));
	strcpy(param->value,newval);
}
