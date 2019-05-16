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

#ifndef PLOTTER_H
#define PLOTTER_H

#define FPS_STEP_FRACT 0.3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Plot.h"

#include "CAENDigitizerType.h"

#ifdef WIN32
    #include <windows.h>
    #include <process.h>
    #define popen  _popen    /* redefine POSIX 'deprecated' popen as _popen */
    #define pclose  _pclose  /* redefine POSIX 'deprecated' pclose as _pclose */
#else

#endif

#ifdef WIN32
	/******************************************************************************
	* Executable file for 'gnuplot'
	* NOTE: use pgnuplot instead of wgnuplot under Windows, otherwise the pipe 
	*       will not work
	******************************************************************************/
    #define GNUPLOT_COMMAND  "pgnuplot"
#else
    #define GNUPLOT_COMMAND  "gnuplot"
#endif

#define PLOT_DATA_FILE "PlotData.txt"
#define FIT_RES_FILE "FitRes.txt"
#define FIT_LOG_FILE "fit.log"

#define DEFAULT_PLOT_FPS 1.5

#define MAX_PLOT_FILE_FAILS 10

typedef enum {
	PLOTTER_OK,
	PLOTTER_TRACES_ALL_OFF = -1,
	PLOTTER_FAIL_OPEN_FILE = -2
} PlotterMessages;

typedef struct FitParams_t { 
	int xmin, xmax; 
	float a,b,c; 
	int fa, fb, fc; 
} FitParams;

typedef struct Plotter_t {
	char*       plotterPath;
	char        plotFileName[200];
	char        fitResFileName[200];
	char        fitLogFileName[200];
	FILE*       plotterPipe;
	int         busy;
	uint64_t    finishTime;
	double       fps;
	int         signalUpdatePlotterOptions;
	int         fitterBusy;
    int         logScaleEnabled;
} Plotter;


Plotter* newPlotter();
void deletePlotter(Plotter*);
int initPlotter(Plotter*);
void deinitPlotter(Plotter*);
int doPlot(Plotter*, Plot*);
int doPlot3d(Plotter*, Plot*);

int checkPlotterBusy(Plotter*); /* check if the plotter has finished (if it has, also update the busy status) */
void waitPlotter(Plotter* plo); /* blocks until the plotter has finished */

int doManualFit(Plotter*, FitParams*); /* perform curve fitting with user-supplied parameters, block until a fit has been done or 120 secs have elapsed */
int checkFitterFinished(Plotter*);
int parseFitResults(Plotter*, FitParams*);
void startFitter(Plotter*);
void stopFitter(Plotter*);

#endif
