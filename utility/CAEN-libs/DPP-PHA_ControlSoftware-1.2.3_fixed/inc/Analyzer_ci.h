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

#ifndef ANALYSIS_CI_H
#define ANALYSIS_CI_H

#include "Digitizer.h"

#include "Plotter.h"
#include "Dumper.h"
#include "Plot.h"
#include "HistoMem.h"
#include "CAENDumper.h"

struct Plot;

#define MAX_NUM_TRACES 8
#define MAX_HISTOS 1500

typedef struct Analyzer_t {
	Digitizer* dig;
	Plotter* plo;
	Dumper* dpr;
	PlotType plotType;
	Plot *plot, *dump;
	int plotEnabled, continuousPlot, singlePlot, dualTrace, signalReplot;
	int chplot;
	int vprobes[MAX_NUM_TRACES];
	int traceEnableWaves[MAX_NUM_TRACES];
	int traceEnableHist[3];
	int traceGains[MAX_NUM_TRACES];
	int traceOffsets[MAX_NUM_TRACES];
	double* fftData; 
	int fftDataLength;
	uint32_t *ehisto[MAX_CH], *ehistoDT[MAX_CH], *thisto[MAX_CH], *histo3d[MAX_CH]; /* histo3d is supported only by NG firmwares */
	uint32_t xhisto3d, yhisto3d;
	uint32_t *corrHisto, *rebinHisto, *rebinCorrHisto;
	uint32_t dtcEvents, dtcTime;
	float* purHisto[MAX_CH];
	float* rebinHistoF;
	float enorm[MAX_CH], tnorm;
	uint32_t ehistoNbit, thistoNbit, tmaxHisto;
	uint64_t ecnt[MAX_CH], tcnt[MAX_CH], trgCnt[MAX_CH], PURcnt[MAX_CH];  /* counters */
	uint64_t trgCntRate[MAX_CH], PURcntRate[MAX_CH];  /* counter markers for the rate calculation */
	uint64_t ecntCorr[MAX_CH], PURcntCorr[MAX_CH];  /* counter markers for the rate calculation */
	int eSkimEnabled;
	float emaxSkim, eminSkim;
	uint64_t prevTime[MAX_CH];
	uint64_t runTime[MAX_CH], extendedTT[MAX_CH], startTime[MAX_CH], stopTime[MAX_CH];
    uint64_t totalElapsedTime; // This is to use a total elapsed time common to all channels
    uint64_t runTimeCorr[MAX_CH], runTimeRate[MAX_CH];
	uint64_t prevRateTime; /* used for trigger rate printing */
	int plotFilter; /* user for energy skim */
	struct Interpolant { int x0,x1; float y0,y1; } calib[MAX_CH];
	int eHistoEU; /* energy histo with energy units rather than counts */
	int dumpWf, dumpLs, dumpEh, dumpTh, dumpEhSingle, dumpThSingle;
	int rebinned[MAX_CH]; /* whether the histos for the current event have already been rebinned */
	float* normEn[MAX_CH];
    int16_t *charge[MAX_CH]; int chargeSize[MAX_CH]; //NB, we will use those variables even on DPP-PHA to store 15 bits energy
	uint32_t normEnSize[MAX_CH];
	uint64_t* ttExt[MAX_CH];
	uint32_t ttExtSize[MAX_CH];

    uint16_t *chargeLong[MAX_CH], *chargeShort[MAX_CH], chargeShortSize[MAX_CH], chargeLongSize[MAX_CH];
    float *psd[MAX_CH], psdSize[MAX_CH];

	/* Fit mode variables */

	int fitMode; /* when set, normal plotting is bypassed. note: acquisition must be stopped for the fitMode to engage */
	int signalManualFit; /* when in fitMode, if set commands a refit based on parameters supplied by the user */
	int signalFitNeedsPlot; /* when fitMode is first entered, this is set to plot just the requested trace and freeze it. after that, no more plots are made until fitMode is exited.*/
	FitParams fitParams; /* user supplied fit parameters*/
	int fitTrace; /* which trace to make a fit plot of*/
	int prevAcqStatus;

	int overflowShown, underflowShown; /* must be reset to 0 everytime the DC offset is adjusted */
	int histoWorthy[MAX_CH]; /* it is set to 1 whenever an event is received and to 0 when an event is dumped, this way we avoid remaking histograms when no new events have been received */
	int histoAutoSwitch, forceHistoSwitch[MAX_CH];
	HistoMem* histoMems[MAX_CH];
	int histoMemHistoSize, histoMemLowFilter[MAX_CH], histoMemHighFilter[MAX_CH];

    c_dumper_t *dumper[MAX_CH];
    c_saveFormat_t listOutMode;
} Analyzer;


Analyzer* newAnalyzer(Digitizer* dig, Plotter* plo, Dumper* dpr); /* "New" methods instantiate new modules, initializing them with default configuration values and create basic, non-configuration-dependent (const) data structures*/
void deleteAnalyzer(Analyzer* ana); /* "Delete" methods deinitialize and deallocate all the data structures, perform finalization operations and then delete the module.*/

int initAnalyzer(Analyzer* ana); /* "Init" methods are to be called after "New" methods and before "Do" methods. They need the module to be already configured and they guarantee that configuration-dependent data structures are initialized and preliminary operations are performed before entering the "Do" loop*/
void deinitAnalyzer(Analyzer* ana); /* "Deinit" methods are to be called after the "Do" loop and they deallocate configuration-dependend data structure and perform post-loop operations*/

int doAnalysis(Analyzer* ana); /* "Do" methods are meant to be called inside a loop as each time they advance the processing one step at a time */

void clearHistoBuffers(Analyzer* ana);
void updateVProbes(Analyzer* ana);

void enterFitMode(Analyzer* ana, int fitTrace);
void exitFitMode(Analyzer* ana);
void manualCurveFit(Analyzer* ana, int fitTrace);
void cycleVProbe(Analyzer* ana, int vp);


void getEHistoCurrent(Analyzer* ana, int ch, uint32_t* histo, uint64_t* counts, uint64_t* rt, uint64_t* dt);
void getEHistoMem(Analyzer* ana, int ch, int index, uint32_t* histo, uint64_t* counts, uint64_t* rt, uint64_t* dt);

void dumpHistos(Analyzer* av);

#endif 
