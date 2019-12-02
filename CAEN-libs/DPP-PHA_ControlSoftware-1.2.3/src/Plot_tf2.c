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

#ifdef DPP_TF_NEW


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

int makeWaveformPlot(Plot* pl, const Analyzer* ana, void* traceData[4], int traceSize, int ch) {
	const int numTraces = 4;
	static const TraceType waveformTraceTypes[] = { 
		PLOT_DATA_INT16,
		PLOT_DATA_INT16,
		PLOT_DATA_UINT8,
		PLOT_DATA_UINT8
	};

	int i;

	pl->xscale = ana->dig->tsampl[ch] / (float)1000.0;  /* 10 ns */
	pl->xshift = 0.0;
	strcpy(pl->xlabel, "us");
	strcpy(pl->ylabel, "ADC counts");
	sprintf(pl->title, "Waveform Ch%d", ch);
	pl->yautoscale = 0;
	pl->ymin = -2000.0;
	//pl->ymax = 16384;
    pl->ymax = (float)(0x1<<ana->dig->boardInfo->ADC_NBits);
	pl->xautoscale = 1;

	pl->numTraces = numTraces;

	getTrace(pl,0)->gain = (float)ana->traceGains[0] * ((ana->vprobes[0]==VP_TRAPEZOID) ? 1/ana->dig->ediv[ch] : 1);
	getTrace(pl,1)->gain = (float)ana->traceGains[1] * ((ana->vprobes[1]==VP_BASELINE || ana->vprobes[1]==VP_TRAP_BL) ? 1/ana->dig->ediv[ch] : 1);
	getTrace(pl,2)->gain = (float)ana->traceGains[2];
	getTrace(pl,3)->gain = (float)ana->traceGains[3];
	getTrace(pl,0)->offset = ana->traceOffsets[0]; 
	getTrace(pl,1)->offset = ana->traceOffsets[1]; 
	getTrace(pl,2)->offset = ana->traceOffsets[2]; 
	getTrace(pl,3)->offset = ana->traceOffsets[3];

	for (i=0; i<numTraces; i++) {
		Trace* tr = getTrace(pl,i);
		tr->enabled = (i!=1) ? ana->traceEnableWaves[i] : (ana->traceEnableWaves[i] & ana->dualTrace);
		if (tr->enabled) {
            sprintf(tr->name, traceTitles[i]);
			//sprintf(tr->name, "(%d) %s", i+1, vpNames[i][ana->vprobes[i]]);
			tr->length = traceSize;
			tr->data = traceData[i];
			tr->type = waveformTraceTypes[i];
		}
	}

	pl->type = PLOT_WAVEFORMS;

	return 0;
}




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
		ana->rebinHisto,
		ana->rebinCorrHisto,
		ana->rebinHistoF
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
	sprintf(pl->title, "Energy Histogram Ch%d", ana->chplot);
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