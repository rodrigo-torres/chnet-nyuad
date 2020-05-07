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


#ifndef GLOBS_H
#define GLOBS_H



#include "Errors.h"

void quitProgram(int ret);


long getTime();
void sleep_(unsigned ms); /* can't sleep_ for longer than 999 ms, because of limitations in the usleep function */

#define SAFE_FREE(x) \
	if (x != NULL) { free(x); x=NULL; } \

#define max(a,b) (((a) > (b)) ? (a) : (b))

#ifdef linux
#define _stricmp strcasecmp
#define _access access
#endif

#endif