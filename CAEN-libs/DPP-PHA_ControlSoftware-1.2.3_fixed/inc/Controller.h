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

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "globs.h"
#include "keyb.h"
#include "messages.h"
#include "Configurator.h"
#include "Digitizer.h"
#include "Analyzer.h"
#include "Plotter.h"


/* this gets bypassed in DPP_TF_CORE mode under WinXp */
#define IDLE_SLEEP_MS 1

#define MAX_MESSAGES 50


typedef enum {
	CTL_DONE = 0,
	CTL_QUIT = 'q',
	CTL_RESTART = 'R',
	CTL_ENDLOOP = '^'
} ControlCodes;

struct Controller_t;

//typedef void (*MessageFunc)(struct Controller_t* ctl, const char* string);
typedef void (*CommandActionFunc)(struct Controller_t* ctl, char key);

typedef struct Message_t {
	char key[50];
	CommandActionFunc func;
} Message;

typedef struct Controller_t {
	int command;
	int requireQuit, signalRestart;
    int restartingEnabled; // This is a flag used to inhibit the restart signal in some condition
	Configurator* con;
	Digitizer*    dig;
	Analyzer*     ana;
	Plotter*      plo;

	CommandActionFunc actionsTable[256];
	Message* messageTable[MAX_MESSAGES];

	char* echoTable[256];

} Controller;


Controller* newController(Configurator*, Digitizer*, Analyzer*, Plotter*);
void deleteController(Controller*);

int doControl(Controller*);
int doControlSequence(Controller*);

void defineKeyboardCommand(Controller* ctl, CommandActionFunc action, int key, const char* echo);

void defineKeyboardCommands(Controller* ctl);

void defineMessage(Controller* ctl, CommandActionFunc action, const char* key);
void lookupMessage(Controller* ctl, const char* key);

void defineMessages(Controller* msg);

#define KEYBOARD_COMMANDS(COMMANDS) \
void defineKeyboardCommands(Controller* ctl) { \
	COMMANDS   \
}

#define COMMAND(ACTION, KEY, ECHOMSG) \
	defineKeyboardCommand(ctl, ACTION, KEY, ECHOMSG);

#define ACTION(NAME) \
void NAME(Controller* ctl, char key)


#define MESSAGES(MSGS) \
void defineMessages(Controller* msg) { \
	MSGS   \
}

#define MESSAGE(ACTION, KEY) \
	defineMessage(msg, ACTION, KEY);

#define NO_ECHO_MESSAGE NULL

#define ACTION_GET_DIG() ctl->dig
#define ACTION_GET_ANA() ctl->ana
#define ACTION_GET_PLO() ctl->plo
#define ACTION_GET_CON() ctl->con
#define ACTION_GET_CTL() ctl

#define ACTION_QUIT() ctl->requireQuit = 1
#define ACTION_RESTART() ctl->signalRestart = ctl->restartingEnabled

#endif