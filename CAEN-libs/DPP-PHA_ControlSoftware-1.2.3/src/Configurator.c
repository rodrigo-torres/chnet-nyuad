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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>


#include "globs.h"
#include "messages.h"
#include "Configurator.h"
#include "Parameter.h"
#include "CAENMultiplatform.h"

#include "CAENLogger.h"
INIT_C_LOGGER("$(TEMP)/DPPRunnerLog.txt", "Configurator.c");

extern void defineGenericParams(Configurator* co);

extern char *FWFilename;

static Parameter* rewindCursor(Configurator* cfo) {
	return cfo->current = cfo->head;
}

static Parameter* advance(Configurator* cfo) {
	return cfo->current = cfo->current->next;
}

static Parameter* current(Configurator* cfo) {
	return cfo->current;
}

static int empty(Configurator* cfo) {
	return cfo->head == NULL;
}

static void appendParameter(Configurator* cfo, Parameter* param) {
	if (empty(cfo)) { cfo->head = cfo->tail = param; }
	else { cfo->tail->next = param; cfo->tail = param; }
}

static Parameter* removeParameter(Configurator* cfo) { /* careful: removal might leave the current cursor in an inconsistent state (always rewindCursor after performing delete operations) */
	Parameter* param = cfo->head;
	if (cfo->head != NULL) cfo->head = cfo->head->next;
	return param;
}

static Parameter* lookupParameter(Configurator* cfo, const char* s, const char* k) { /* O(n*len(s)*len(k)) worst-case complexity and no duplicate warning, but entries are limited in number */
	for(rewindCursor(cfo); current(cfo); advance(cfo)) {
		if (!strcmp(current(cfo)->section,s) && !strcmp(current(cfo)->key,k))
			return current(cfo);
	}
	return NULL;
}

static void stringRightTrim(char* str) {
	size_t i;
	for (i=strlen(str)-1; i >= 0; i--) {
		if (isspace(str[i])) str[i] = 0;
		else break;
	}
}


void executeParameter(Parameter* param, Configurator* cfo) {
	if (param->value != NULL) param->action(cfo,param->section,param->key,param->value);
}


void defineParameter(Configurator* cfo, configFunc a, const char* k, const char* v, const char* s, ...) {
	va_list args;
	Parameter* param;

	char patterned[20]; /* s can be patterned à la printf and it will be sprintf'ed into here */
	
	va_start(args,s);
	vsprintf(patterned,s,args); /* see above comment */
	va_end(args);
	
	param = newParameter(patterned,k,v,a);
	appendParameter(cfo,param);
}


Configurator* newConfigurator(const char* filename, Digitizer* dig, Analyzer* ana, Plotter* plo, Dumper* dpr) {
    //int FWFNameLen = 0;
    char *FWBaseFileName = "FreeWrites.txt";
	Configurator* cv = (Configurator*) calloc(1,sizeof(Configurator));

    logMsg(c_logger_Severity_DEBUG, "Configurator Allocated");

	cv->dig = dig;
	cv->ana = ana;
	cv->plo = plo;
	cv->dpr = dpr;
	if (cv == NULL) return NULL;
	if (filename == NULL) {
		cv->_cmdLineFileName = 0;
		cv->filename = DEFAULT_CONFIG_FILENAME;
	} else {
		cv->_cmdLineFileName = 1;
		cv->filename = (char*) calloc(strlen(filename)+1,sizeof(char));
		if (cv->filename == NULL) { free(cv); return NULL; }
		strcpy(cv->filename, filename);
	}
	
    logMsg(c_logger_Severity_DEBUG, "Configurator Filename Set Up: %s", cv->filename);

    FWFilename = (char*)calloc(strlen(filename)+strlen(FWBaseFileName)+1,sizeof(char));
    if(FWFilename != NULL) {
        getBaseName(FWFilename, filename);
        strcat(FWFilename, FWBaseFileName);
    }

    logMsg(c_logger_Severity_DEBUG, "FreeWrites Filename Set Up: %s", FWFilename);

	defineGenericParams(cv); /* this function is defined in the params.c file via macros */

	return cv;
}

void deleteAllParameters(Configurator* cfo) {
    while(!empty(cfo))
		deleteParameter(removeParameter(cfo));
	rewindCursor(cfo);
}

void clearAllParameters(Configurator* cfo) {
    deleteAllParameters(cfo); // Delete all Parameters
    defineGenericParams(cfo); // And redefine them
}

void deleteConfigurator(Configurator* cfo) {
	if (cfo->_cmdLineFileName) SAFE_FREE(cfo->filename);
	deleteAllParameters(cfo);
}

int parseConfiguration(Configurator* cv) { /* parses all the configuration parameters from the config file */
	char skiptag[5], section[51], key[51], value[101], line[201];
	int skip = 0;
	FILE* file;

    clearAllParameters(cv);

	userMsg("Reading configuration file...\n");
#ifndef DPP_TF_CORE
	file = fopen(cv->filename,"r");
#else
	file = stdin;
#endif
	if (file == NULL) return ERR_CONF_FILE_NOT_FOUND;

	memset(section,0,sizeof(section));

	while (fgets(line, sizeof(line), file) != NULL) {
#ifdef DPP_TF_CORE
		if (line[0] == '^') break;
#endif
		if (sscanf(line,"%4s",skiptag) && !_stricmp(skiptag,"@OFF"))
			skip = 1;

		if (!skip) {
			memset(value,0,sizeof(value)); /* we fill value with '\0' because the data we are about to read into it does not end with a string termination char */
			sscanf(line,"[%50[^]]]",section);
			if (sscanf(line,"%50s %100[^\r\n]", key, value) == 2 && key[0] != '#') {
				Parameter* param = lookupParameter(cv,section,key); 
				if (param == NULL) { 
					userMsg("Unknown parameter \"%s/%s\" ignored\n",section,key);
					continue;
				}
				stringRightTrim(value); /* gets rid of pesky whitespaces at the end of the value string to easen up comparisons later on */
				setParameterValue(param,value);
			}
		} else if (sscanf(line,"%3s",skiptag) && !_stricmp(skiptag,"@ON")) 
			skip = 0;
	}
#ifndef DPP_TF_CORE
	fclose(file);
#endif
	rewindCursor(cv);
	for (; current(cv) != NULL; advance(cv)) {	/* applies all the configuration parameters */
		executeParameter(current(cv),cv);
	}
	return 0;
}

int parseSingleLine(Configurator* cv, const char* line) { /* parses a single line in "SECTION/KEY VALUE" format */
	char section[51], key[51], value[101];
	memset(value, 0, sizeof(value));
	if (sscanf(line,"%50[^/]/%50s %100c",section, key, value) == 3) {
		Parameter* param = lookupParameter(cv,section,key);
		if (param == NULL) { 
			userMsg("Unknown parameter \"%s/%s\" ignored\n",section,key);
			return -1;
		}
		stringRightTrim(value); /* gets rid of pesky whitespaces at the end of the value string to easen up comparisons later on */
		setParameterValue(param,value);
        //userMsg("Parameter: S: %s, K: %s, V: %s\n",param->section,param->key,param->value);
		executeParameter(param,cv);
		return 0;
	}
	userMsg("Unrecognized config line \"%s\" ignored\n",line);
	return -2;
}

/* Getters to access variables and methods belonging to different modules */

Digitizer* getDig(Configurator* co) {
	return co->dig;
}

Analyzer* getAna(Configurator* co) {
	return co->ana;
}

Plotter* getPlo(Configurator* co) {
	return co->plo;
}

Dumper* getDpr(Configurator* co) {
	return co->dpr;
}