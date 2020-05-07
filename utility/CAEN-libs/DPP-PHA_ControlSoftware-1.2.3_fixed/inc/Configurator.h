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


#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include "globs.h"
#include "Digitizer.h"
#include "Analyzer.h"
#include "Plotter.h"
#include "Dumper.h"


#define DEFAULT_CONFIG_FILENAME "DppRunnerConfig.txt"

struct Parameter_t;

typedef struct Configurator_t {
	struct Parameter_t* head;	/* parameter list management */
	struct Parameter_t* tail;
	struct Parameter_t* current;

	char* filename;
	int _cmdLineFileName; /* internal variable used to discriminate whether the config file name
						     has been passed via the cmd line or it is defaulted to the macro value */
	Digitizer* dig;
	Analyzer* ana;
	Plotter* plo;
	Dumper* dpr;
} Configurator;


Configurator* newConfigurator(const char* filename, Digitizer*, Analyzer*, Plotter*, Dumper*);
void deleteConfigurator(Configurator* co);

int parseConfiguration(Configurator* co);
int parseSingleLine(Configurator* co, const char* line);


Digitizer* getDig(Configurator* co);
Analyzer* getAna(Configurator* co);
Plotter* getPlo(Configurator* co);
Dumper* getDpr(Configurator* co);

typedef void (*configFunc)(struct Configurator_t* co, const char* s, const char* k, const char* v);
void defineParameter(Configurator* co, configFunc action, const char* key, const char* val, const char* sectpat, ...);


/********************************************************************************
*  Macros for the definition of the configuration parameters. See file params.c
********************************************************************************/

#define NO_DEFAULT_VALUE NULL

#define CONFIGURATION_PARAMETERS(PARAMS) \
void defineGenericParams(Configurator* co) { \
	int i=0; \
	PARAMS   \
}

#define COMMON_PARAMETER(UPDATE, KEY, DEFVAL) \
	defineParameter(co, UPDATE, KEY, DEFVAL, "COMMON");

#define CHANNEL_PARAMETER(UPDATE, KEY, DEFVAL) \
	defineParameter(co, UPDATE, KEY, DEFVAL, "COMMON"); \
	for (i=0; i< MAX_CH; i++) \
		defineParameter(co, UPDATE, KEY, NO_DEFAULT_VALUE, "%d", i);

#define UPDATE(NAME) \
void NAME(Configurator* UPDATE_CON, const char* UPDATE_SECTION, const char* UPDATE_KEY, const char* UPDATE_VALUE)

#define UPDATE_GET_DIG() getDig(UPDATE_CON)
#define UPDATE_GET_ANA() getAna(UPDATE_CON)
#define UPDATE_GET_PLO() getPlo(UPDATE_CON)
#define UPDATE_GET_DPR() getDpr(UPDATE_CON)
#define UPDATE_RESTART() if (checkBoardInited(UPDATE_GET_DIG())) UPDATE_GET_DIG()->signalRestart = UPDATE_GET_DIG()->restartingEnabled


#endif