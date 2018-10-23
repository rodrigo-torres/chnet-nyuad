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

#include "dppswitch.h"
#ifdef DPP_CI


#include <stdio.h>
#include <math.h>

#include "globs.h"
#include "Analyzer.h"
#include "Plot.h"

char traceTitles[6][30];

void setTraceTitle(int traceNum, char *title) {
    char tmpTitle[30];
    sprintf(tmpTitle, "(%d) %s", traceNum+1, title);
    strcpy(traceTitles[traceNum], tmpTitle);
}

int makeWaveformPlot(Plot* pl, const Analyzer* ana, void* traceData[6], int traceSize, int ch) {
	const int numTraces = 6;
	/*static const TraceType waveformTraceTypes[] = { 
		PLOT_DATA_INT16,
		PLOT_DATA_INT16,
		PLOT_DATA_UINT8,
		PLOT_DATA_UINT8,
		PLOT_DATA_UINT8,
		PLOT_DATA_UINT8
	};		*/		

	Trace* tr;

	pl->xscale = ana->dig->tsampl[ch] / (float)1000.0;  /* 10 ns */
	pl->xshift = 0.0;
	strcpy(pl->xlabel, "us");
	strcpy(pl->ylabel, "ADC counts");
	sprintf(pl->title, "Waveform Ch%d", ana->chplot);
	pl->yautoscale = 0;
	pl->ymin = -2000;
	//pl->ymax = 16384;
	pl->ymax = (float)(0x1<<ana->dig->boardInfo->ADC_NBits);
    pl->xautoscale = 1;

	pl->numTraces = numTraces;

	tr = getTrace(pl, 0);
    tr->enabled = ana->traceEnableWaves[0];
    if (tr->enabled) {
		tr->gain = (float)ana->traceGains[0];
		tr->offset = ana->traceOffsets[0]; // CUIDADO: gain & offset will have to be properly set one day
        tr->type = PLOT_DATA_INT16;
        tr->length = traceSize;
        sprintf(tr->name, traceTitles[0]);
		//sprintf(tr->name, "(1) %s", "Input Signal");
		tr->data = traceData[0];
    }

	tr = getTrace(pl, 1);
	tr->enabled = ana->traceEnableWaves[1] && ana->dualTrace;
    if (tr->enabled) {
		tr->gain = (float)ana->traceGains[1];
		tr->offset = ana->traceOffsets[1];
        tr->type = PLOT_DATA_INT16;
        tr->length = traceSize;
        sprintf(tr->name, traceTitles[1]);
		//sprintf(tr->name, "(2) %s", VPAname[ana->vprobes[1]]);
		tr->data = traceData[1];
    }

	tr = getTrace(pl, 2);
	tr->enabled = ana->traceEnableWaves[2];
    if (tr->enabled) {
		tr->gain = (float)ana->traceGains[2];
		tr->offset = ana->traceOffsets[2];
        tr->type = PLOT_DATA_UINT8;
        tr->length = traceSize;
        sprintf(tr->name, traceTitles[2]);
		//sprintf(tr->name, "(3) %s", "Trigger");
		tr->data = traceData[2];
    }

	tr = getTrace(pl, 3);
    tr->enabled = ana->traceEnableWaves[3];
    if (tr->enabled) {
		tr->gain = (float)ana->traceGains[3];
		tr->offset = ana->traceOffsets[3];
        tr->type = PLOT_DATA_UINT8;
        tr->length = traceSize;
        sprintf(tr->name, traceTitles[3]);
		//sprintf(tr->name, "(4) %s", "Gate");
		tr->data = traceData[3];
    }

	tr = getTrace(pl, 4);
    tr->enabled = ana->traceEnableWaves[4];
    if (tr->enabled) {
		tr->gain = (float)ana->traceGains[4];
		tr->offset = ana->traceOffsets[4];
        tr->type = PLOT_DATA_UINT8;
        tr->length = traceSize;
        sprintf(tr->name, traceTitles[4]);
		//sprintf(tr->name, "(5) %s", VPDname[ana->vprobes[4]]);
		tr->data = traceData[4];
    }

	tr = getTrace(pl, 5);
    tr->enabled = ana->traceEnableWaves[5];
    if (tr->enabled) {
		tr->gain = (float)ana->traceGains[5];
		tr->offset = ana->traceOffsets[5];
        tr->type = PLOT_DATA_UINT8;
        tr->length = traceSize;
        sprintf(tr->name, traceTitles[5]);
		//sprintf(tr->name, "(6) %s", VPD1name[ana->vprobes[5]]);
		tr->data = traceData[5];
    }

	pl->type = PLOT_WAVEFORMS;

	return 0;
}

//int makeListDump(Plot* pl, const Analyzer* ana, int ch) { /* not really a plot. it is used by the Dumper */
//	Trace* tr;
//	int i;
//
//	pl->xscale = 1;
//	pl->xshift = 0.0;
//
//	for (i=0; i<4; i++) {
//		getTrace(pl,i)->gain = 1;
//		getTrace(pl,i)->offset = 0;
//	}
//
//	pl->numTraces = 2;
//	tr = getTrace(pl,0);
//	//sprintf(tr->name,"Timetags");
//	tr->enabled = 1;
//	tr->data = ana->ttExt[ch];
//	tr->length = ana->ttExtSize[ch];
//	tr->type = PLOT_DATA_UINT64;
//	
//	tr = getTrace(pl,1);
////	sprintf(tr->name,"Energies");
//	tr->enabled = 1;
//
//    tr->type = PLOT_DATA_INT16;
//    tr->data = ana->charge[ch];
//    tr->length = 1;
//
//	pl->type = DUMP_LIST;
//
//	return 0;
//}

int makeEHistoPlot(Plot* pl, const Analyzer* ana, int ch) {
	const int numTraces = 3;
	static const TraceType ehistoTraceTypes[] = {
		PLOT_DATA_UINT32,
		PLOT_DATA_UINT32,
		PLOT_DATA_FLOAT
	};
	static const char* ehistoTraceNames[] = {
		"Ehisto",
		"EhistoCorr",
		"EhistoPUR"
	};
	void* traceData[] = {
		ana->ehisto[ch],
		ana->ehistoDT[ch],
		ana->purHisto[ch]
	};

	uint64_t counts[] = {
		ana->ecnt[ch],
		ana->PURcnt[ch] + ana->ecnt[ch],
		ana->PURcnt[ch]
	};
	

	int i;
	struct Interpolant il = ana->calib[ch];
	if (ana->eHistoEU && fabs(il.x1-il.x0)>0) {
		pl->xscale = (float)(fabs(il.y1 - il.y0)/fabs(il.x1 - il.x0));
		pl->xshift = (float)(fabs(il.y0 - (il.x0*(il.y1-il.y0))/abs(il.x1-il.x0)));
		strcpy(pl->xlabel, "keV");
	} else { 
		strcpy(pl->xlabel, "ADC channels");
		pl->xscale = 1.0;
		pl->xshift = 0.0;
	}
	strcpy(pl->ylabel, "Counts");
	sprintf(pl->title, "Energy Histogram Ch%d", ch);
	pl->yautoscale = 1;
	pl->xautoscale = 1;
	for (i=0; i<4; i++) {
		getTrace(pl,i)->gain = 1;
		getTrace(pl,i)->offset = 0;
	}

	pl->numTraces = numTraces;

	for (i=0; i<numTraces; i++) {
		Trace* tr = getTrace(pl,i);
		tr->enabled = ana->traceEnableHist[i];
		if (tr->enabled) {
			sprintf(tr->name, "(%d) %s (%llu)", i+1, ehistoTraceNames[i], counts[i]);
			tr->data = traceData[i];
			tr->length = (1 << ana->ehistoNbit);
			tr->type = ehistoTraceTypes[i];
		}
	}

	pl->type = PLOT_EHISTO;

	return 0;
}


#endif