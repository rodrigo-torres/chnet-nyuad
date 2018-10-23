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


#ifndef PLOT_H
#define PLOT_H

#include <CAENDigitizerType.h>

struct Analyzer_t;

#define DIGITAL_TRACE_HEIGHT 100

#define VP_TRAPEZOID 3
#define VP_TRAP_BL 2
#define VP_BASELINE 3

typedef enum {
    PLOT_DATA_UINT8,
    PLOT_DATA_UINT16,
    PLOT_DATA_UINT32,
	PLOT_DATA_UINT64,
    PLOT_DATA_INT8,
    PLOT_DATA_INT16,
    PLOT_DATA_INT32,
	PLOT_DATA_INT64,
    PLOT_DATA_FLOAT,
    PLOT_DATA_DOUBLE
} TraceType;

typedef enum {
	PLOT_EMPTY, /* when a plot is created it gets this value*/
	PLOT_WAVEFORMS,
	PLOT_FFT, /* this is only used by the Plotter*/
	PLOT_EHISTO,
	PLOT_THISTO,
	PLOT_HISTO3D,
	DUMP_LIST, /* this is only used to create list dumps, thus only used by the Dumper*/
	_NUM_PLOT_TYPES_
} PlotType;

typedef struct Trace_t {
	char*		name;
    int         enabled;
    float       gain;
    int         offset;
	TraceType	type;
	int         length;
    void*   	data;
} Trace;



typedef struct Plot_t {
	PlotType	type;	
    char*       title;
    char*		xlabel;
	char*		ylabel;
    int         xautoscale, yautoscale;
    float       xscale, yscale;
	float		xshift;
    float       xmax, ymax;
    float       xmin, ymin;
	Trace*		traces;
    int         numTraces;
	const int   maxNumTraces;
	const int	maxTraceLength;

} Plot;



Plot* newPlot(int maxNumTraces, int maxTraceLength);
void deletePlot(Plot*);

Trace* getTrace(Plot* pl, int tr);

int makeWaveformPlot(Plot* pl, const struct Analyzer_t* ana, void* eventData[4], int dataSize, int ch);
int makeFFTPlot(Plot* pl, const struct Analyzer_t* ana, int ch);
int makeEHistoPlot(Plot* pl, const struct Analyzer_t* ana, int ch);
int makeTHistoPlot(Plot* pl, const struct Analyzer_t* ana, int ch);
int make2dHistoPlot(Plot* pl, const struct Analyzer_t* ana, int ch);
int makeListDump(Plot* pl, const struct Analyzer_t* ana, int ch);
void setTraceTitle(int traceNum, char *title);

#endif