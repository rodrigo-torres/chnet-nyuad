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

#include "Controller.h"
#include "messages.h"

#ifdef DPP_TF_CORE
	#include <Windows.h>

	static int isWin7() {
		OSVERSIONINFO osver;
		osver.dwOSVersionInfoSize = sizeof(osver);
		GetVersionEx(&osver);
		return osver.dwMajorVersion == 6;
	}

#endif


Controller* newController(Configurator* con, Digitizer* dig, Analyzer* ana, Plotter* plo) {
	Controller* ctl = calloc(1,sizeof(Controller));
	if (ctl == NULL) return NULL;
	ctl->con = con;
	ctl->dig = dig;
	ctl->ana = ana;
	ctl->plo = plo;
    ctl->restartingEnabled = 1;

	memset(ctl->actionsTable, 0, sizeof(CommandActionFunc)*256);
	memset(ctl->echoTable, 0, sizeof(char*)*256);
	
	funcsetup(); /* to assign the correct functions to getch_ and kbhit_ depending on whether the program is attached to a console or to pipes*/	
	if (stdinterm())
        userMsg("Running in console mode\n");

	defineKeyboardCommands(ctl);
	defineMessages(ctl);

#ifdef DPP_TF_CORE
	if (!isWin7()) {
		SetPriorityClass( GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS );
	}
#endif

	return ctl;
}

void deleteController(Controller* ctl) {
	int i=0;
	for (; i<256; i++)
		SAFE_FREE(ctl->echoTable[i]);
	free(ctl);
}


int doControl(Controller* cv) {
	int i=0;
	while(1) { /* an endless loop is used to consume all the commands that may have been queued*/
		cv->command = CTL_DONE;
		if (!kbhit_()) break;
		i++;
		cv->command = getch_();
		if (cv->command<0) break; /* values < 0 may result from errors in the getch_ function. we strap up and try to carry on.*/
		else if (cv->command == CTL_ENDLOOP) return CTL_ENDLOOP;

		if (cv->echoTable[cv->command] != NULL) userMsg("%s\n",cv->echoTable[cv->command]);
		if (cv->actionsTable[cv->command] != NULL) { 
			cv->actionsTable[cv->command](cv, cv->command);
			commandAck(); /* acknowledge the command to the GUI*/
		}
	}

	if (cv->signalRestart || cv->dig->signalRestart) {
		cv->signalRestart = 0;
		cv->dig->signalRestart = 0;
		guiMsg(MSG_RESTART); /* inform the GUI that the runner is restarting*/
		return CTL_RESTART;
	} else if (cv->requireQuit) {
		cv->requireQuit = 0;
		return CTL_QUIT;
	} else {
#ifdef DPP_TF_CORE
		if (isWin7()) {
#endif
			if (cv->dig->acqStatus == ACQSTATUS_STOPPED) sleep_(IDLE_SLEEP_MS); /* if acquisition is not enabled put the runner to sleep_ for a bit, to avoid busy-cycling*/
			else if (cv->dig->acquisitionMode == CAEN_DGTZ_DPP_ACQ_MODE_List) sleep_(IDLE_SLEEP_MS); /* if acquiring histograms we can safely sleep_ for a bit, to bring down the CPU usage */
#ifdef DPP_TF_CORE
		}
#endif
		return CTL_DONE;
	}
}


int doControlSequence(Controller* ctl) {
	int loopctl;
	do {
		loopctl = doControl(ctl);
	} while (loopctl == CTL_DONE); // exits only if the control wants to quit, restart or ends the sequence
	return loopctl;
}



void defineKeyboardCommand(Controller* ctl, CommandActionFunc action, int key, const char* echo) {
	ctl->actionsTable[key] = action;
	if (echo != NULL) { 
		ctl->echoTable[key] = calloc(strlen(echo)+1,sizeof(char));
		strcpy(ctl->echoTable[key],echo);
	}
}


void lookupMessage(Controller* ctl, const char* key) {
	int i;
	for (i=0; i<MAX_MESSAGES && ctl->messageTable[i] != NULL; i++) {
		if (strcmp(key, ctl->messageTable[i]->key)==0) {
			ctl->messageTable[i]->func(ctl, 0);
		}
	}
}


void defineMessage(Controller* ctl, CommandActionFunc action, const char* key) {
	static int index = 0;
	ctl->messageTable[index] = calloc(1, sizeof (Message));
	ctl->messageTable[index]->func = action;
	strcpy(ctl->messageTable[index++]->key , key);
}

