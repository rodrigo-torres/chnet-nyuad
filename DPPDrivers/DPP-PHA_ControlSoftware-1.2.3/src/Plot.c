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

#include <stdio.h>
#include <math.h>

#include "globs.h"
#include "Analyzer.h"
#include "Plot.h"



Plot* newPlot(int maxNumTraces, int maxTraceLength) {
	int i=0;
	Plot* pl = (Plot*) calloc(1,sizeof(Plot));
	if (pl == NULL) { deletePlot(pl); return NULL; }

	pl->title = (char*) calloc(100,sizeof(char));
	if (pl->title == NULL) { deletePlot(pl); return NULL; }

	pl->xlabel = (char*) calloc(100,sizeof(char));
	if (pl->xlabel == NULL) { deletePlot(pl); return NULL; }

	pl->ylabel = (char*) calloc(100,sizeof(char));
	if (pl->ylabel == NULL) { deletePlot(pl); return NULL; }

	*(int*)&pl->maxNumTraces = maxNumTraces;	/* workaround to set the const members. */
	*(int*)&pl->maxTraceLength = maxTraceLength;


    pl->xscale = 1.0;
    pl->yscale = 1.0;
    pl->xmax = 16384;
    pl->ymax = 16384;
    pl->xmin = 0;
    pl->ymin = 0;
    pl->numTraces = maxNumTraces;

	pl->traces = calloc(maxNumTraces,sizeof(Trace));
	for (i=0; i<maxNumTraces; i++) {
		pl->traces[i].name = calloc(100,sizeof(char));
		pl->traces[i].type = PLOT_DATA_DOUBLE;
		pl->traces[i].gain = 1;

	}
	return pl;
}


void deletePlot(Plot* pl) {
	int i;
	for (i=0; i < pl->maxNumTraces; i++) {
		SAFE_FREE(getTrace(pl,i)->name);
	}
	SAFE_FREE(pl->traces)
	SAFE_FREE(pl->title)
	SAFE_FREE(pl->xlabel)
	SAFE_FREE(pl->ylabel)
	SAFE_FREE(pl);
}


Trace* getTrace(Plot* pl, int tr) {
	return &pl->traces[tr];
}


int makeFFTPlot(Plot* pl, const Analyzer* ana, int ch) {
	int i;

	strcpy(pl->xlabel, "MHz");
	strcpy(pl->ylabel, "dB");
	sprintf(pl->title, "FFT Ch%d",  ch);
	pl->yautoscale = 1;
	pl->ymin = -160;
	pl->ymax = 0;
	pl->xautoscale = 1;
	pl->xscale = 1.0;
	pl->xshift = 0.0;

	for (i=0; i<4; i++) {
		getTrace(pl,i)->gain = 1;
		getTrace(pl,i)->offset = 0;
	}

	pl->numTraces = 1;

    getTrace(pl,0)->enabled = 1;
	sprintf(getTrace(pl,0)->name,"FFT");
	getTrace(pl,0)->data = ana->fftData;
	getTrace(pl,0)->length = ana->fftDataLength;
	getTrace(pl,0)->type = PLOT_DATA_DOUBLE;

	pl->type = PLOT_FFT;

	return 0;
}





int makeTHistoPlot(Plot* pl, const Analyzer* ana, int ch) {
	Trace* tr;
	int i;

	pl->xscale = ((float)ana->tmaxHisto / (float)1000.0) / (1<<ana->thistoNbit);
	pl->xshift = 0.0;
	strcpy(pl->xlabel, "Time (us)");
	strcpy(pl->ylabel, "Counts");
	sprintf(pl->title, "Time Histogram Ch %d", ch);
	pl->xautoscale = 1;
	pl->yautoscale = 1;
	for (i=0; i<4; i++) {
		getTrace(pl,i)->gain = 1;
		getTrace(pl,i)->offset = 0;
	}

	pl->numTraces = 1;


	tr = getTrace(pl,0);
	sprintf(tr->name,"THisto");
	tr->enabled = ana->traceEnableHist[0];
	tr->data = ana->thisto[ch];
	tr->length = 1 << ana->thistoNbit;
	tr->type = PLOT_DATA_UINT32;


	pl->type = PLOT_THISTO;

	return 0; 
}

#ifdef DPP_TF_NEW

int makeListDump(Plot* pl, const Analyzer* ana, int ch) { /* not really a plot. it is used by the Dumper */
	//Trace* tr;
	//int i;

	//pl->xscale = 1;
	//pl->xshift = 0.0;

	//for (i=0; i<4; i++) {
	//	getTrace(pl,i)->gain = 1;
	//	getTrace(pl,i)->offset = 0;
	//}

	//pl->numTraces = 2;
	//tr = getTrace(pl,0);
	////sprintf(tr->name,"Timetags");
	//tr->enabled = 1;
 //   tr->data = ana->dig->events[ch] ttExt[ch];
	//tr->length = 1;
	//tr->type = PLOT_DATA_UINT64;
	//
 //   /*********************************************************************************************
 //   ****** HACK, decide here if we want to dump 15 bits integer list or 14 bits float list *******
 //   **********************************************************************************************/
 //   // case 14 bits float
	////tr = getTrace(pl,1);
	//////sprintf(tr->name,"Energies");
	////tr->enabled = 1;
	////tr->type = PLOT_DATA_FLOAT;
 //   //tr->data = ana->normEn[ch];
 //   //tr->length = ana->normEnSize[ch];

 //   // case 15 bits int16_t
 //   tr = getTrace(pl,1);
	////sprintf(tr->name,"Energies");
	//tr->enabled = 1;
	//tr->type = PLOT_DATA_INT16;
 //   tr->data = ana->charge[ch];
 //   tr->length = ana->chargeSize[ch];

	//pl->type = DUMP_LIST;

	//return 0;

    return 0;
}

#endif