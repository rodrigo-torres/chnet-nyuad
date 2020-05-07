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

/******************************************************************************
* This file contains the definition of the keyboard commands dpprunner 
* understands.
*
* Commands are defined via a macro system.
*
* The list of commands is specified with the following macro:
*
*		KEYBOARD_COMMANDS { 
*			... 
*		}
*
* A command is defined with the following syntax:
*
*		COMMAND( ACTION_NAME, 'KEY', "ECHO_MESSAGE" );
*
* When the corresponding key is pressed, an action is triggered and an optional
* echo message is displayed (the NO_ECHO_MESSAGE macro suppresses it)
*
* Command actions are defined in this way:
*
*		ACTION( ACTION_NAME ) {
*			...
*		}
*
* An action is supposed to alter the configuration and/or call
* specific flow-altering functions
* The following macros are available for use inside an action:
*		ACTION_GET_DIG() returns a pointer to the Digitizer object
*		ACTION_GET_ANA() returns a pointer to the Analyzer object
*		ACTION_GET_PLO() returns a pointer to the Plotter object
*
******************************************************************************/



#include "Controller.h"

ACTION(aQuit) {
	ACTION_QUIT();
}

ACTION(aRestart) {
    clearHistoBuffers(ACTION_GET_ANA());
    ACTION_RESTART();
}

ACTION(aSelectPlotChannel) {
	char ch;
	userMsg("\nPress 0-7 to select the channel to plot\n");
    ch = getch_()-'0';
	ACTION_GET_ANA()->chplot = ch;
	ACTION_GET_PLO()->signalUpdatePlotterOptions = 1;
	ACTION_GET_ANA()->signalReplot = 1;
	if(!(ACTION_GET_DIG()->enableMask & (1 << ch))) {
		userMsg("Warning: channel %d not enabled for acquisition\n", ch);
		guiMsg(MSG_ERROR,"The selected plot channel is not enabled for acquisition");
	}
}

ACTION(aLogScale) {
    char ch;

    ch=getch_();
    switch(ch) {
    case '0':
        ACTION_GET_PLO()->logScaleEnabled = 0;
        userMsg("Log scale disabled\n");
        break;
    case '1':
        ACTION_GET_PLO()->logScaleEnabled = 1;
        userMsg("Log scale enabled\n");
        break;
    default:
        break;
    }
    ACTION_GET_PLO()->signalUpdatePlotterOptions = 1;
}

ACTION(aContinuousPlot) { 
	ACTION_GET_ANA()->continuousPlot ^= 1; 
	if (ACTION_GET_ANA()->continuousPlot) userMsg("Continuous plotting enabled\n");
	else userMsg("Continuous plotting disabled\n");
}
ACTION(aSinglePlot)	{ 
	ACTION_GET_ANA()->singlePlot = 1; 
}
ACTION(aPlotTypeW) { 
	ACTION_GET_ANA()->plotType = PLOT_WAVEFORMS;
    ACTION_GET_PLO()->logScaleEnabled = 0; // Force logscale disabled in oscilloscope mode
	ACTION_GET_PLO()->signalUpdatePlotterOptions = 1;
	if (ACTION_GET_DIG()->acqStatus == ACQSTATUS_STOPPED)
        ACTION_GET_ANA()->signalReplot = 1;
}
ACTION(aPlotTypeF) { 
	ACTION_GET_ANA()->plotType = PLOT_FFT; 
	ACTION_GET_PLO()->signalUpdatePlotterOptions = 1;
	if (ACTION_GET_DIG()->acqStatus == ACQSTATUS_STOPPED)
        ACTION_GET_ANA()->signalReplot = 1;
}
ACTION(aPlotTypeE){ 
	ACTION_GET_ANA()->plotType = PLOT_EHISTO; 
	ACTION_GET_PLO()->signalUpdatePlotterOptions = 1;
	if (ACTION_GET_DIG()->acqStatus == ACQSTATUS_STOPPED)
        ACTION_GET_ANA()->signalReplot = 1;
}
ACTION(aPlotTypeT) { 
	ACTION_GET_ANA()->plotType = PLOT_THISTO; 
	ACTION_GET_PLO()->signalUpdatePlotterOptions = 1;
	if (ACTION_GET_DIG()->acqStatus == ACQSTATUS_STOPPED)
        ACTION_GET_ANA()->signalReplot = 1;
}
ACTION(aPlotType3d) {
	ACTION_GET_ANA()->plotType = PLOT_HISTO3D;
	ACTION_GET_PLO()->signalUpdatePlotterOptions = 1;
	if (ACTION_GET_DIG()->acqStatus == ACQSTATUS_STOPPED)
        ACTION_GET_ANA()->signalReplot = 1;
}

ACTION(aEHistoEU) {
	ACTION_GET_ANA()->eHistoEU ^= 1;
	if (ACTION_GET_ANA()->eHistoEU) {
        userMsg("Energy histogram with energy units\n");
        strcpy(ACTION_GET_ANA()->plot->xlabel, "keV");
    }
	else {
        userMsg("Energy histogram with counts\n");
        strcpy(ACTION_GET_ANA()->plot->xlabel, "ADC Counts");
    }
    fprintf(ACTION_GET_ANA()->plo->plotterPipe, "set xlabel '%s'\n", ACTION_GET_ANA()->plot->xlabel);
}

ACTION(aClearHisto) { 
	clearHistoBuffers(ACTION_GET_ANA()); 
}

ACTION(aSetVProbe0) {
	cycleVProbe(ACTION_GET_ANA(), 0);
}

ACTION(aSetVProbe1) {
	cycleVProbe(ACTION_GET_ANA(), 1);
}

ACTION(aSetVProbe2) {
	cycleVProbe(ACTION_GET_ANA(), 2);
}

ACTION(aSetVProbe3) {
    cycleVProbe(ACTION_GET_ANA(), 3);
}

ACTION(aSetVProbe4) {
    cycleVProbe(ACTION_GET_ANA(), 4);
}

ACTION(aSetVProbe5) {
    cycleVProbe(ACTION_GET_ANA(), 5);
}

ACTION(aSingleTrigger) { 
	sendSingleTrigger(ACTION_GET_DIG()); 
}

ACTION(aContinuousTrigger) { 
	enableContinuousTrigger(ACTION_GET_DIG(), 
		                    !checkContinuousTrigger(ACTION_GET_DIG()) ); 
}

ACTION(aAcquisitionStart) { 
	Digitizer* dig = ACTION_GET_DIG();
	if (dig->acqStatus != ACQSTATUS_RUNNING) {
		dig->signalTimeReset = 1;
		startAcquisition(dig);
	}
}

ACTION(aAcquisitionStop) { 
	Digitizer* dig = ACTION_GET_DIG();
	if (dig->acqStatus != ACQSTATUS_STOPPED) {
		stopAcquisition(dig);
    }
}

ACTION(aPrintHelp) {
    printf("\nBindkey help\n");
    printf("[q]    Quit\n");
    printf("[R]    Restart program\n");
	printf("[F]    Reload configuration file and restart\n");
	printf("[r]    Clear histogram buffers\n");
    printf("[s]    Start acquisition\n");
    printf("[S]    Stop acquisition\n");
    printf("[t]    Send a software trigger (single shot)\n");
    printf("[T]    Enable/Disable continuous software trigger\n");
    printf("[c]    Toggle channel acquisition enable\n");
    printf("[p]    Plot one event\n");
    printf("[P]    Enable/Disable continuous plot\n");
    printf("[o]    Enter waveform plot mode\n");
    printf("[f]    Enter FFT plot mode\n");
    printf("[h]    Enter energy histogram plot mode\n");
    printf("[H]    Enter time histogram plot mode\n");
	printf("[d]    Enter 3d energy histogram plot mode (if available)\n");
	printf("[1-4]  Cycle through virtual probes in waveform mode\n");
    printf("[+]    Increase plotter FPS\n");	
    printf("[-]    Decrease plotter FPS\n");
	printf("[x]	   Extended command mode\n");
    printf("[?]    This help\n\n");
    printf("Press a key to continue\n");
    getch_();
}

ACTION(aParseLine) {
	char line[301];
    char *tmp; // to avoid compiler warnings
    tmp = gets(line);
	parseSingleLine(ACTION_GET_CON(), line);
}


ACTION(aMoreFps) {
	ACTION_GET_PLO()->fps += ACTION_GET_PLO()->fps*FPS_STEP_FRACT;
	userMsg("fps: %.2f\n",ACTION_GET_PLO()->fps);
}

ACTION(aLessFps) {
	//if (ACTION_GET_PLO()->fps>1) ACTION_GET_PLO()->fps--;
	ACTION_GET_PLO()->fps -= ACTION_GET_PLO()->fps*FPS_STEP_FRACT;
	userMsg("fps: %.2f\n",ACTION_GET_PLO()->fps);
}

ACTION(aReloadFile) {
    stopAcquisition(ACTION_GET_DIG());
	parseConfiguration(ACTION_GET_CON());
	ACTION_RESTART();
}

ACTION(aExtendedCmd) {
	char cmd = getch_();
    int tmp_scnfres; // to avoid compiler warnings
	switch(cmd) {
	case 'o': {
		int trace=-1, offset=-1;
		tmp_scnfres = scanf("%d %d",&trace,&offset);
		ACTION_GET_ANA()->traceOffsets[trace] = offset;
		break;
	}
	case 'g': {
		int trace=-1, gain=-1;
		tmp_scnfres = scanf("%d %d",&trace,&gain);
		ACTION_GET_ANA()->traceGains[trace] = gain;
		break;
	}
	case 't': {
		int trace=-1;
        int enabled;
		tmp_scnfres = scanf("%d %d",&trace, &enabled);
		if (ACTION_GET_ANA()->plotType == PLOT_WAVEFORMS ||
			ACTION_GET_ANA()->plotType == PLOT_FFT) {
				ACTION_GET_ANA()->traceEnableWaves[trace] = enabled;
		} else {
				ACTION_GET_ANA()->traceEnableHist[trace%3] = enabled; // mod operation to avoid crashes if erroneously given command "xt3" while in histo mode
		}
		break;
	}
    case 'T': {
        int traceNum;
        char traceName[30];
        tmp_scnfres = scanf("%d %[^\n]s", &traceNum, traceName);
        setTraceTitle(traceNum, traceName);
        break;
    }
	case 'f': {
		int trace=-1;
		if (!ACTION_GET_ANA()->fitMode) {
			tmp_scnfres = scanf("%d",&trace);
			enterFitMode(ACTION_GET_ANA(),trace);
		} else {
			exitFitMode(ACTION_GET_ANA());
		}
		break;
	}
	case 'F': {
		int trace=-1;
		FitParams* fp = &ACTION_GET_ANA()->fitParams;	
		tmp_scnfres = scanf("%d %d %d %f %f %f %d %d %d",&trace, &fp->xmin, &fp->xmax, &fp->a, &fp->b, &fp->c, &fp->fa, &fp->fb, &fp->fc);
		manualCurveFit(ACTION_GET_ANA(),trace);
		break;
	}
    case 'W': {
		uint32_t address, value;
		tmp_scnfres = scanf("%x %u",&address, &value);
        CAEN_DGTZ_WriteRegister(ACTION_GET_ANA()->dig->boardHandle, address, value);
		break;
	}
    case 'R': {
		uint32_t address, value;
		tmp_scnfres = scanf("%x",&address);
        CAEN_DGTZ_ReadRegister(ACTION_GET_ANA()->dig->boardHandle, address, &value);
        guiMsgFmt("readRegister %u", value);
		break;
	}
    case 'B': {
        char rw = getch_();
        switch(rw) {
        case 'R': {
            int nR, i;
            char *responseString;

            tmp_scnfres = scanf("%d",&nR);
            responseString = (char*) calloc(15+(nR*10), sizeof(char));
            for(i=0; i<nR; i++) {
                char val[10];
                uint32_t address, regValue;

                tmp_scnfres = scanf("%x",&address);
                CAEN_DGTZ_ReadRegister(ACTION_GET_ANA()->dig->boardHandle, address, &regValue);
                //userMsg("Value of address %X: %X\n", address, regValue);
                sprintf(val, "%u ", regValue);
                strcat(responseString, val);
            }
            guiMsgFmt("blockReadRegister %d %s", nR, responseString);
            free(responseString);
            break;
        }
        case 'W': {
            // TODO (block write registers)
            // int nW;
            break;
        }
        }
        break;
    }
    case 'r': {
		uint32_t restartEnabled;
		tmp_scnfres = scanf("%d",&restartEnabled);
        ctl->restartingEnabled = restartEnabled;
        ACTION_GET_DIG()->restartingEnabled = restartEnabled;
		break;
	}
    case 'X': {
		uint32_t resetEnabled;
		tmp_scnfres = scanf("%d",&resetEnabled);
        ACTION_GET_DIG()->resetEnabled = resetEnabled;
		break;
	}
	default: userMsg("Command not recognized\n");
	}
}


ACTION(aCoreMsg) {
	char str[50];
    int tmp_scnfres; // to avoid compiler warnings
	tmp_scnfres = scanf("%50s",str);
	lookupMessage(ACTION_GET_CTL(), str);
}


ACTION(aSingleDump) {
    Analyzer *an = ACTION_GET_ANA();
    char cmd = getch_();
	switch(cmd) {
    case 'e': {
        //userMsg("setting eh=1\n");
        an->dumpEhSingle = 1;
        break;
    }
    case 't': {
        //userMsg("setting th=1\n");
        an->dumpThSingle = 1;
        break;
    }
    default: {
        break;
    }
    }
    dumpHistos(an);
}

KEYBOARD_COMMANDS(

	COMMAND(	aQuit,				'q',	"Quitting..."					);
	COMMAND(	aRestart,			'R',	"Restarting..."					);

	COMMAND(	aSingleTrigger,		't',	NO_ECHO_MESSAGE					);
	COMMAND(	aContinuousTrigger,	'T',	NO_ECHO_MESSAGE					);
	COMMAND(	aAcquisitionStart,	's',	NO_ECHO_MESSAGE					);
    COMMAND(	aAcquisitionStop,	'S',	NO_ECHO_MESSAGE					);

	COMMAND(	aContinuousPlot,	'P',	NO_ECHO_MESSAGE					);
	COMMAND(	aSinglePlot,		'p',	"Drawing a single plot..."		);
	COMMAND(	aPlotTypeW,			'o',	"Plot type: waveforms"			);
	COMMAND(	aPlotTypeF,			'f',	"Plot type: FFT"				);
	COMMAND(	aPlotTypeE,			'h',	"Plot type: energy histogram"	);
	COMMAND(	aPlotTypeT,			'H',	"Plot type: time histogram"		);
	COMMAND(	aPlotType3d,		'd',	"Plot type: 2d energy histogram");
    COMMAND(	aSingleDump,		'D',	NO_ECHO_MESSAGE                 );
	COMMAND(	aEHistoEU,			'e',	NO_ECHO_MESSAGE					);
	COMMAND(	aSelectPlotChannel,	'c',	NO_ECHO_MESSAGE					);
    COMMAND(	aLogScale,			'L',	NO_ECHO_MESSAGE					);
	COMMAND(	aClearHisto,		'r',	"Clearing histo buffers..."		);
	COMMAND(	aSetVProbe0,		'1',	NO_ECHO_MESSAGE					);
	COMMAND(	aSetVProbe1,		'2',	NO_ECHO_MESSAGE					);
	COMMAND(	aSetVProbe2,		'3',	NO_ECHO_MESSAGE					);
	COMMAND(	aSetVProbe3,		'4',	NO_ECHO_MESSAGE					);
	COMMAND(	aSetVProbe4,		'5',	NO_ECHO_MESSAGE					);
	COMMAND(	aSetVProbe5,		'6',	NO_ECHO_MESSAGE					);

	COMMAND(	aPrintHelp,			'?',	NO_ECHO_MESSAGE					);
	COMMAND(	aParseLine,			'$',	"Reading config line..."		);
	COMMAND(	aExtendedCmd,		'x',	NO_ECHO_MESSAGE         		);

	COMMAND(	aMoreFps,			'+',	"Increasing plotter FPS..."		);
	COMMAND(	aLessFps,			'-',	"Decreasing plotter FPS..."		);

	COMMAND(	aReloadFile,		'F',	NO_ECHO_MESSAGE					);

	COMMAND(	aCoreMsg,			'&',	NO_ECHO_MESSAGE					);

)