


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


/*
	This file belongs to the PSD version of the DPP software
*/

#include "dppswitch.h"

#ifdef DPP_PSD

#include <stdarg.h>

#include "globs.h"
#include "messages.h"
#include "Analyzer.h"
#include "fft.h"

#define OVERFLOW_MAX 5
#define UNDERFLOW_MAX 5
#define WARNING_TIMEOUT_MS 30000

static void printReadoutRate(uint32_t bufferOccupancy, uint64_t elapsedTime) {
	if (bufferOccupancy == 0) { 
		userMsg("No data...\n"); 
		guiMsg(MSG_ROUT_RATE,0);
	} else {
		userMsg("Readout Rate=%.2f MB/s\n", (float)bufferOccupancy/((float)elapsedTime*1048.576f));
		guiMsg(MSG_ROUT_RATE,(float)bufferOccupancy/((float)elapsedTime*1048.576f));
	}
}


static void printTriggerRate(Analyzer* ana, int ch) {
	if (checkChannelEnabled(ana->dig,ch)) {
        uint64_t nt, np;
        float trgRate, purRatio;
		nt = ana->trgCnt[ch] - ana->trgCntRate[ch]; 
        if (nt == 0) {
            userMsg("ch=%d\tNo trgs\n", ch);
			//guiMsg(MSG_TRG_RATE, ch, 0.0, 0.0, ana->runTime[ch]/1000000000.0);
            guiMsg(MSG_TRG_RATE, ch, 0.0, 0.0, ana->totalElapsedTime/1000000000.0);
            return;
        }
		if (ana->runTime[ch] == 0) return;
		np = ana->PURcnt[ch] - ana->PURcntRate[ch]; 
		trgRate = (float)nt * (float)1000000.0 / (float)(ana->runTime[ch] - ana->runTimeRate[ch]);
		purRatio = ((np+nt) > 0) ? (float)(np*100)/nt : 0;
		userMsg("ch=%d\tICR=%.2fKHz\tPUR=%.2f%%\tt=%.2f\n", ch, trgRate, purRatio, ana->runTime[ch]/1000000000.0);
		ana->trgCntRate[ch] = ana->trgCnt[ch];
		ana->PURcntRate[ch] = ana->PURcnt[ch];
		ana->runTimeRate[ch] = ana->runTime[ch];
		//guiMsg(MSG_TRG_RATE, ch, trgRate, purRatio, ana->runTime[ch]/1000000000.0);
        guiMsg(MSG_TRG_RATE, ch, trgRate, purRatio, ana->totalElapsedTime/1000000000.0);
	}
}


static void printStats(Analyzer* av) {
	static int cumulativeBufferOccupancy = 0;
	uint64_t currentTime, elapsedTime;

	cumulativeBufferOccupancy += getBufferOccupancy(av->dig);
	currentTime = getTime();
    elapsedTime = currentTime - av->prevRateTime;

	if (elapsedTime >= 1000) {
		int ch;
		printReadoutRate(cumulativeBufferOccupancy,elapsedTime);
		if (av->dig->acquisitionMode == CAEN_DGTZ_DPP_ACQ_MODE_List) {
			for(ch=0; ch<MAX_CH; ch++) { printTriggerRate(av, ch); }
		}
		cumulativeBufferOccupancy = 0;
		av->prevRateTime = currentTime;
	}

}


static void histoRebinning(uint32_t *Hin, uint32_t *Hout, float bs) {
    int i, j;
    float h;

    j=0;
    h = 0.0;
    for(i=0; i<16384; i++) {
        while((j+1)*bs <= (float)(i+1)) {
            h += (float)Hin[j++];
            if (j >= 32768) break;
        }
        h += (float)Hin[j] * (((i+1) - (j*bs)) / bs);
        Hout[i] = (uint32_t)(h+0.5);  /* round*/
        h = (float)Hin[j] * (((j+1)*bs - (i+1)) / bs); /* reported to next Hout bin*/
        j++;
        if (j >= 32768) break;/* end of Hin*/
    }
    /* fill the rest of the histogram with zeros*/
    for(j=i; j<16384; j++)
        Hout[j]=0;
}

static void histoRebinningF(float *Hin, float *Hout, float bs) {
    int i, j;
    float h;

    j=0;
    h = 0.0;
    for(i=0; i<16384; i++) {
        while((j+1)*bs <= (float)(i+1)) {
            h += Hin[j++];
            if (j >= 32768) break;
        }
        h += Hin[j] * (((i+1) - (j*bs)) / bs);
        Hout[i] = h; 
        h = Hin[j] * (((j+1)*bs - (i+1)) / bs); /* reported to next Hout bin*/
        j++;
        if (j >= 32768) break;/*end of Hin*/
    }
    /* fill the rest of the histogram with zeros*/
    for(j=i; j<16384; j++)
        Hout[j]=0;
}
//
//static void rebinAll(Analyzer* av, int ch) {
//	int i;
//	histoRebinning(av->ehisto[ch], av->rebinHisto, 1/av->enorm[ch]);
//	for(i=0; i<(1<<(av->ehistoNbit+1)); i++)
//		av->corrHisto[i] = av->ehisto[ch][i] + (int)av->purHisto[ch][i];
//	histoRebinning(av->corrHisto, av->rebinCorrHisto, 1/av->enorm[ch]);
//	histoRebinningF(av->purHisto[ch], av->rebinHistoF, 1/av->enorm[ch]);
//	av->rebinned[ch] = 1;
//}


static void freeHistoBuffers(Analyzer* av) {
    int ch;
	SAFE_FREE(av->rebinHisto)
	SAFE_FREE(av->rebinHistoF)
	SAFE_FREE(av->corrHisto)
	SAFE_FREE(av->rebinCorrHisto)
    for (ch=0; ch < MAX_CH; ch++) {
		SAFE_FREE(av->ehisto[ch])
		SAFE_FREE(av->ehistoDT[ch])
		SAFE_FREE(av->thisto[ch])
		SAFE_FREE(av->purHisto[ch])
		SAFE_FREE(av->histo3d[ch])
	}
}

void getEHistoCurrent(Analyzer* ana, int ch, uint32_t* histo, uint64_t* counts, uint64_t* rt, uint64_t* dt) { }
void getEHistoMem(Analyzer* ana, int ch, int index, uint32_t* histo, uint64_t* counts, uint64_t* rt, uint64_t* dt) { }


void clearHistoBuffers(Analyzer* av) {
    int ch;
	memset(av->rebinHisto, 0, (uint32_t)(0x1<<av->ehistoNbit)*sizeof(uint32_t));
	memset(av->corrHisto, 0, (uint32_t)(0x1<<(av->ehistoNbit+1))*sizeof(uint32_t));
	memset(av->rebinHistoF, 0, (uint32_t)(0x1<<av->ehistoNbit)*sizeof(float));
	memset(av->rebinCorrHisto, 0, (uint32_t)(0x1<<av->ehistoNbit)*sizeof(uint32_t));

    for (ch=0; ch<MAX_CH; ch++) {
	//	if (av->dig->enableMask & (1<<ch)) {
			if (av->ehisto[ch] != NULL) memset(av->ehisto[ch], 0, (uint32_t)(0x1<<(av->ehistoNbit+1)) * sizeof(uint32_t));
			if (av->ehistoDT[ch] != NULL) memset(av->ehistoDT[ch], 0, (uint32_t)(0x1<<(av->ehistoNbit+1)) * sizeof(uint32_t));
			if (av->purHisto[ch] != NULL) memset(av->purHisto[ch], 0, (uint32_t)(0x1<<(av->ehistoNbit+1)) * sizeof(float));
			if (av->thisto[ch] != NULL) memset(av->thisto[ch], 0, (uint32_t)(0x1<<av->thistoNbit) * sizeof(uint32_t));
			if (av->histo3d[ch] != NULL) memset(av->histo3d[ch], 0, HISTO3D_X*HISTO3D_Y*sizeof(uint32_t));
			av->ecnt[ch] = 0;
			av->tcnt[ch] = 0;
			av->PURcnt[ch] = 0;
			av->ecntCorr[ch] = 0;
			av->PURcntCorr[ch] = 0;
			av->PURcntRate[ch] = 0;
            av->histoWorthy[ch] = 0;
	//	}
	}
}


static int allocateHistoBuffers(Analyzer* av) {
    int ch, ret=0;
	ret |= (av->rebinHisto = (uint32_t*) calloc((uint32_t)(0x1<<av->ehistoNbit), sizeof(uint32_t))) == NULL;
	ret |= (av->corrHisto = (uint32_t*) calloc((uint32_t)(0x1<<(av->ehistoNbit+1)), sizeof(uint32_t))) == NULL;
	ret |= (av->rebinHistoF = (float*) calloc((uint32_t)(0x1<<av->ehistoNbit), sizeof(float))) == NULL;
	ret |= (av->rebinCorrHisto = (uint32_t*) calloc((uint32_t)(0x1 << (av->ehistoNbit)), sizeof(uint32_t))) == NULL;

    for (ch=0; ch<MAX_CH; ch++) {
		//if (av->dig->enableMask & (1<<ch)) {
			ret |= (av->ehisto[ch] = (uint32_t*) calloc((uint32_t)(0x1<<(av->ehistoNbit+1)), sizeof(uint32_t))) == NULL;
			ret |= (av->ehistoDT[ch] = (uint32_t*) calloc((uint32_t)(0x1<<(av->ehistoNbit+1)), sizeof(uint32_t))) == NULL;
			ret |= (av->purHisto[ch] = (float*) calloc((uint32_t)(0x1<<(av->ehistoNbit+1)), sizeof(float))) == NULL;
			ret |= (av->thisto[ch] = (uint32_t*) calloc((uint32_t)(0x1<<(av->ehistoNbit+1)), sizeof(uint32_t))) == NULL;
			ret |= (av->histo3d[ch] = (uint32_t*) calloc(HISTO3D_X*HISTO3D_Y, sizeof(uint32_t))) == NULL;
			if (ret) break;
		//}
	}
	if (ret) {
		freeHistoBuffers(av);
		return -1;
	}
    return 0;
}


static void resetTimeCounters(Analyzer* av) {
	int i = 0;
	for (; i < MAX_CH; i++)
		av->startTime[i] = av->runTime[i] = av->extendedTT[i] = av->runTimeCorr[i] = av->totalElapsedTime = 0;
	av->dig->signalTimeReset = 0;
}



static void analyzeEvent(Analyzer* av, CAEN_DGTZ_DPP_PSD_Event_t* event, int ch, int i) {
    Digitizer* dig = av->dig;
	//int ret = 0;
	//int debugPrinted = 0;
	uint32_t energy, deltaT, tbin, tt;
    uint64_t deltaEcnt, elapsedT;
	uint16_t qshort, qlong, baseline;
	float qshortNorm, qlongNorm, psd;
	int hx, hy;
	//static int overflowCount = 0;
	//static int underflowCount = 0;
//	static int maxi = 0;

	av->plotFilter = (av->eSkimEnabled == 0);
//	av->rebinned[ch] = 0;
	av->histoWorthy[ch] = 1;

	tt = event->TimeTag;
	av->trgCnt[ch]++;

	if (av->startTime[ch] == 0) {
		//av->startTime[ch] = (uint64_t)(av->dig->tsampl[ch]) * tt;
		av->startTime[ch] = 1; // HACK, this is to use a common startTime for all channels (which is the time of startAcquisition + 1 clock)
		av->runTime[ch] = 0;
        av->totalElapsedTime = 0;
		av->runTimeCorr[ch] = 0;
		av->extendedTT[ch] = 0;
		av->prevTime[ch] = 0;

    } else {
		if (tt < av->prevTime[ch]) av->extendedTT[ch]++;

		if (av->dig->acquisitionMode == CAEN_DGTZ_DPP_ACQ_MODE_List) {
			//av->runTime[ch] = dig->tsampl[ch] * ((av->extendedTT[ch] << 32) | (uint64_t)tt) - av->startTime[ch];
            av->runTime[ch] = dig->tsampl[ch] * ((av->extendedTT[ch] << 32) | (uint64_t)tt);
            if(av->totalElapsedTime < av->runTime[ch])
                av->totalElapsedTime = av->runTime[ch];
        }

		deltaT = av->dig->tsampl[ch] * ((tt - av->prevTime[ch]) & 0x7FFFFFFF);
		tbin = (int)(deltaT / av->tnorm);
		if ((tbin > 0) && (tbin < (uint32_t)(1<<av->thistoNbit))) {
			av->thisto[ch][tbin]++;
			av->tcnt[ch]++;
		}
	}
	av->prevTime[ch] = tt;

	//energy = event->Energy & ((1<<(av->ehistoNbit+1))-1);
	qshort = event->ChargeShort;
    qlong = event->ChargeLong;
	baseline = event->Baseline;
#if 0
	if ((qshort == 0xFFFF || qlong == 0xFFFF) /*&& !av->overflowShown*/ && ++overflowCount >= OVERFLOW_MAX) {
		static uint32_t prevTime = 0;
		uint32_t currentTime = getTime();
		if (currentTime - prevTime >= WARNING_TIMEOUT_MS) {
			overflowCount = 0;
			av->overflowShown = 1;
			guiMsg(MSG_ERROR, "Charge saturation. Check Charge Sensitivity.\n");
			userMsg("Charge saturation. Check Charge Sensitivity.\n");
			prevTime = currentTime;
		}
	} 


	if ((qshort == 0x0 || qlong == 0x0) /*&& !av->underflowShown*/ && ++underflowCount >= UNDERFLOW_MAX) {
		static uint32_t prevTime = 0;
		uint32_t currentTime = getTime();
		if (currentTime - prevTime >= WARNING_TIMEOUT_MS) {		
			underflowCount = 0;
			av->underflowShown = 1;
			guiMsg(MSG_ERROR, "Negative charge. Check Baseline Threshold.\n");
			userMsg("Negative charge. Check Baseline Threshold.\n");
			prevTime = currentTime;
		}
	}
#endif
    /* Update the histograms */
	qshortNorm = (float)qshort/QNORM;
	qlongNorm = (float)qlong/QNORM;
	energy = (uint32_t)qlong;
	psd = (qlongNorm - qshortNorm)/qlongNorm;
				

	hx = (int)(qlongNorm*HISTO3D_X);
	hy = (int)(HISTO3D_Y*psd);

	/*if ((psd >=0.35) && (hx*4096/HISTO3D_X > 1181) && (hx*4096/HISTO3D_X < 1478))
			plotThis = 1;*/

    if ((hx>=0) && (hx < HISTO3D_X) && (hy>=0) && (hy < HISTO3D_Y))	{
		av->histo3d[ch][hy*HISTO3D_X+hx]++;
    }

	//av->normEn[ch][0] = energy / av->enorm[ch];
	av->ttExt[ch][0] = tt | (av->extendedTT[ch] << 32);
    av->chargeShort[ch][0] = qshort;
    av->chargeLong[ch][0] = qlong;
    av->psd[ch][0] = psd;

	if ((ch == av->chplot) && 
		((!av->eSkimEnabled) ||
		(av->eSkimEnabled && (av->normEn[ch][0]) < av->emaxSkim && (av->normEn[ch][0]) > av->eminSkim)))
        av->plotFilter = 1;
	else av->plotFilter = 0;
	if (energy > 0) {
		av->ehisto[ch][energy]++;
		av->ehistoDT[ch][energy]++;
		av->ecnt[ch]++;
	} 
	
	if (event->Pur)  // PUR
		av->PURcnt[ch]++;

	elapsedT = av->runTime[ch] - av->runTimeCorr[ch];
	deltaEcnt = av->ecnt[ch] - av->ecntCorr[ch];
	//if ((deltaEcnt > 0) && ((elapsedT > av->dtcTime) || (deltaEcnt > av->dtcEvents))) {
	//	int j;
	//	float deadTime = (float)dig->dppParams[ch].b;  /* forse va aumentato HACK*/
	//	float tratio = deadTime / (float)elapsedT;
 //       uint64_t no = deltaEcnt + (av->PURcnt[ch] - av->PURcntCorr[ch]);  /* num of pulses in ElapsedT (including PUR)*/
	//	float cntLoss = (float)(av->PURcnt[ch] - av->PURcntCorr[ch]); /* pulses lost for pile-up */
	//	//CntLoss += (No*No*Tratio) / (1 - No*Tratio);  /* pulses lost for dead time*/
 //       for(j=0; j<(1<<(av->ehistoNbit+1)); j++) {
 //           av->purHisto[ch][j] += cntLoss * (float)av->ehistoDT[ch][j] / (float)deltaEcnt;
 //       }
	//	memset(av->ehistoDT[ch], 0, (1<<(av->ehistoNbit+1)) * sizeof(uint32_t));
	//	av->PURcntCorr[ch] = av->PURcnt[ch];
 //       av->ecntCorr[ch] = av->ecnt[ch];
	//	av->runTimeCorr[ch] = av->runTime[ch];
	//}
}



static void plotEvent(Analyzer* av, CAEN_DGTZ_DPP_PSD_Event_t* event) {
	if (!checkChannelEnabled(av->dig,av->chplot)) return;
	if (checkPlotterBusy(av->plo)) return;
	if (av->continuousPlot || av->singlePlot) {
		int plotThisWave = 1; /* legacy. was: event->ChTraceSize[av->chplot] > 2;*/
		if (av->plotType == PLOT_WAVEFORMS && ((plotThisWave && av->plotFilter) || av->signalReplot) && event != NULL) {
			int traceSize; void* traceData[6]; 
			getDecodedWaveforms(av->dig, event, traceData, &traceSize);
			makeWaveformPlot(av->plot,av, traceData, traceSize, av->chplot);
		} else if (av->plotType == PLOT_FFT && ((plotThisWave && av->plotFilter) || av->signalReplot) && event != NULL) {
			int traceSize; void* traceData[6]; 
			getDecodedWaveforms(av->dig, event, traceData, &traceSize);		
			av->fftDataLength = FFT((unsigned short*)traceData[0], av->fftData, traceSize, HANNING_FFT_WINDOW);
			makeFFTPlot(av->plot,av, av->chplot);
		} else if (av->plotType == PLOT_EHISTO) {
		//	rebinAll(av, av->chplot);
			makeEHistoPlot(av->plot,av, av->chplot);
		} else if (av->plotType == PLOT_THISTO) {
			makeTHistoPlot(av->plot,av, av->chplot);
		} else if (av->plotType == PLOT_HISTO3D) {
			make2dHistoPlot(av->plot,av, av->chplot);
		}
				
		av->singlePlot = 0;
		av->signalReplot = 0;

		if (av->dig->acqStatus != ACQSTATUS_STOPPED) {
			if ((av->plotType == PLOT_HISTO3D && doPlot3d(av->plo, av->plot)==PLOTTER_FAIL_OPEN_FILE) || 
				(av->plotType != PLOT_HISTO3D && doPlot(av->plo, av->plot)==PLOTTER_FAIL_OPEN_FILE)) {
				av->continuousPlot = 0;
				userMsg("Too many failures while trying to write to the plot file. Plotting stopped.\n");
				guiMsg(MSG_ERROR,"Too many failures while trying to write to the plot file. Plotting stopped.");
				guiMsg(MSG_STOP_CPLOT);
			}
		}
	}
}



static void curveFit(Analyzer* ana) {
	if (ana->signalFitNeedsPlot) {
		int i;
		for (i=0; i<ana->plot->numTraces; i++)
			getTrace(ana->plot, i)->enabled = (i==ana->fitTrace);
		doPlot(ana->plo,ana->plot);
		ana->signalFitNeedsPlot = 0;
	}
	if (ana->signalManualFit) { doManualFit(ana->plo,&ana->fitParams); ana->signalManualFit=0; }
	if (checkFitterFinished(ana->plo)) {
		int res = parseFitResults(ana->plo,&ana->fitParams);
		if (res == 1) { exitFitMode(ana); }
		guiMsg(MSG_FIT_PARAMS, res?'a':'y', ana->fitParams.xmin, ana->fitParams.xmax, ana->fitParams.a, ana->fitParams.b, ana->fitParams.c);
	}

	/* startFitMode() -- gnuplot goes in fit mode
	 * each time user presses 'f' in gnuplot a roi is defined and a fit is made. results are written to a file
 	 * maybe signalManualFit()
	 * whenever the runner sees a fit res file, it parses it and sends the contents to the gui, then deletes the file
	 * 
	 * either stopFitMode() -- gnuplot exits fit mode, exit loop
	 * or loop over*/
}

void manualCurveFit(Analyzer* ana, int fitTrace) {
	if (ana->fitTrace != fitTrace) {
		ana->signalFitNeedsPlot = 1;
		ana->fitTrace = fitTrace;
	}
	if (!ana->fitMode) {
		enterFitMode(ana,fitTrace);
	}
	ana->signalManualFit = 1;
	userMsg("Manual curve fit\n");
}

void enterFitMode(Analyzer* ana, int fitTrace) {
	if (ana->plot->type != PLOT_EHISTO && ana->plot->type != PLOT_THISTO) { 
		userMsg("You must plot a histogram before entering fit mode\n");
		guiMsg(MSG_ERROR,"You must plot a histogram before entering fit mode");
		return; /* you can't fit on empty, waveform, fft or dump plots*/
	}
	ana->prevAcqStatus = ana->dig->acqStatus;
	if (ana->prevAcqStatus) stopAcquisition(ana->dig);
	ana->fitMode = 1;
	ana->fitTrace = fitTrace;
	ana->signalFitNeedsPlot = 1;
	startFitter(ana->plo);
	userMsg("Fit mode enabled\n");
}

void exitFitMode(Analyzer* ana) {
	if (ana->prevAcqStatus) startAcquisition(ana->dig);
	ana->fitMode = 0;
	ana->signalFitNeedsPlot = 0;
	stopFitter(ana->plo);
	userMsg("Fit mode disabled\n");
}


static void dumpEvent(Analyzer* av, CAEN_DGTZ_DPP_PSD_Event_t* event, int ch) {
	int plotThisWave;
	if (!checkChannelEnabled(av->dig, ch)) return;
	plotThisWave = 1; /* legacy*/
	if (av->dumpWf && plotThisWave && 
	    (av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope || 
		 av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_Mixed)) {
		int traceSize; void* traceData[6]; 
		getDecodedWaveforms(av->dig, event, traceData, &traceSize);
		makeWaveformPlot(av->dump, av, traceData, traceSize, ch);
		doDump(av->dpr, av->dump, ch);
	}
	if (av->dumpLs &&
	    (av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_List || 
		 av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_Mixed)) {
             //c_dumpDPPEvent(av->dumper[ch], event);
             int32_t ChargeTot=(event->ChargeShort<<DPP_PSD_ENOFFSET)|event->ChargeLong;
             // NOTE, here we only add the event to the buffer. The actual dump
             // is perfomed in function doAnalysis usin function c_ddumpb(...)
             c_dadde(av->dumper[ch], &event->TimeTag, &ChargeTot);
	}
}

static void dumpChannelHisto(Analyzer* av, int ch) {
	if (!checkChannelEnabled(av->dig, ch)) return;
	if (!av->histoWorthy[ch]) return;
	//av->histoWorthy[ch] = 0;
	if ((av->dumpEh || av->dumpEhSingle) && 
	    (av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_List || 
		 av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_Mixed)) {
	//	if (!av->rebinned[ch]) rebinAll(av,ch);
		makeEHistoPlot(av->dump, av, ch);
		doDump(av->dpr, av->dump, ch);
	}
	if ((av->dumpTh || av->dumpThSingle) && 
	    (av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_List || 
		 av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_Mixed)) {
		makeTHistoPlot(av->dump, av, ch);
		doDump(av->dpr, av->dump, ch);
	}
}

void dumpHistos(Analyzer* av) {
    int ch;
    // check if should dump histos
    if ( !(av->dumpEh || av->dumpEhSingle || av->dumpTh || av->dumpThSingle) )
        return;
    for(ch=0; ch<av->dig->numChannels; ch++) {
        dumpChannelHisto(av, ch);
    }
    av->dumpEhSingle = 0;
    av->dumpThSingle = 0;
}

int initAnalyzer(Analyzer* ana) {
	int i;
	int maxrl=1<<15;
	for (i=0; i<ana->dig->numChannels; i++) {
		ana->enorm[i] = ana->dig->ediv[i]; /* was: / (float)(1<<shf); */
		ana->normEn[i] = calloc(1,sizeof(float));
		ana->normEnSize[i] = 1;
		ana->ttExt[i] = calloc(1,sizeof(uint64_t));
		ana->ttExtSize[i] = 1;

        ana->chargeShort[i] = calloc(1,sizeof(uint16_t));
		ana->chargeShortSize[i] = 1;
        ana->chargeLong[i] = calloc(1,sizeof(uint16_t));
		ana->chargeLongSize[i] = 1;
        ana->psd[i] = calloc(1,sizeof(float));
		ana->psdSize[i] = 1;
        ana->histoWorthy[i] = 0;
	}
	for (i=0; i<MAX_CH; i++) {
		maxrl = maxrl > ana->dig->recordLength[i] ? maxrl : ana->dig->recordLength[i];
	}
	ana->plot = newPlot(MAX_NUM_TRACES, maxrl);
	if (ana->plot == NULL) return -1;

	ana->dump = newPlot(MAX_NUM_TRACES, maxrl);
	if (ana->dump == NULL) return -1;
	
	//for (i=0; i<MAX_NUM_TRACES; i++)
	//	ana->traceEnable[i]=1;

	updateVProbes(ana);
	
	ana->fftData = calloc(maxrl,sizeof(double));
	ana->tnorm = (float)ana->tmaxHisto / (1<<ana->thistoNbit);

	//allocateHistoBuffers(ana);

    ana->dumpEhSingle=0;
    ana->dumpThSingle=0;

    resetTimeCounters(ana);

	return 0; 
}

void deinitAnalyzer(Analyzer* ana) {
	int i;
	//clearHistoBuffers(ana);
	//freeHistoBuffers(ana);
	SAFE_FREE(ana->fftData);
	for (i=0; i<ana->dig->numChannels; i++) {
		SAFE_FREE(ana->normEn[i]);
		SAFE_FREE(ana->ttExt[i]);
	}
	deletePlot(ana->plot);
	deletePlot(ana->dump);

    // Close the event dumper (only if it is opened)
    for(i=0; i<MAX_CH; i++) {
        //c_closeDumper(&ana->dumper[i]);
        c_closeDumper(ana->dumper[i]);
        ana->dumper[i]=NULL;
    }
}


Analyzer* newAnalyzer(Digitizer* dig, Plotter* plo, Dumper* dpr) {
	int i=0;
	Analyzer* av = (Analyzer*) calloc(1,sizeof(Analyzer));
	if (av == NULL) return NULL;
	av->dig = dig;
	av->plo = plo;
	av->dpr = dpr;
	av->ehistoNbit = EHISTO_NBITS_PSD;
	av->thistoNbit = 16;
	for (i=0; i<MAX_NUM_TRACES; i++) av->traceEnableWaves[i] = 1;
	av->traceEnableHist[0] = 1;
	if (allocateHistoBuffers(av)) { free(av); return NULL; }
	av->xhisto3d = HISTO3D_X;
	av->yhisto3d = HISTO3D_Y;
	return av;
}


void deleteAnalyzer(Analyzer* av) {
	freeHistoBuffers(av);
	free(av);
	return;
}



int doAnalysis(Analyzer* av) {
	Digitizer* dig = av->dig;
	CAEN_DGTZ_DPP_PSD_Event_t* event = NULL;
	uint32_t numEvents = 0;
	int ev;
	int ret;
	int ch;

	if (av->fitMode) curveFit(av);

	if (av->dig->signalTimeIsUp) {
		printStats(av);
		waitPlotter(av->plo);
		if (av->plotType == PLOT_EHISTO) {
	//		rebinAll(av, av->chplot);
			makeEHistoPlot(av->plot, av, av->chplot);
		} else if (av->plotType == PLOT_THISTO) {
			makeTHistoPlot(av->plot, av, av->chplot);
		}
		doPlot(av->plo, av->plot);
		av->dig->signalTimeIsUp = 0;
	}

	if (dig->signalTimeReset) {
		resetTimeCounters(av);	
		dig->signalTimeReset = 0;
	}


	if (dig->acqStatus == ACQSTATUS_STOPPED) return 0;

	/* Calculate throughput and trigger rate (every second) */		
	printStats(av);

	for (ch = 0; ch < dig->numChannels; ch++) {
		if (!checkChannelEnabled(dig,ch)) continue;
		if ((ret = getNumEvents(dig, &numEvents, ch)) != 0) return ret;
		//av->ttExtSize[ch] = av->normEnSize[ch] = numEvents;
        if(numEvents == 0 && (av->plotType == PLOT_EHISTO || av->plotType == PLOT_THISTO || av->plotType == PLOT_HISTO3D)) {
            // If we don't have events, we call plotEvent anyway for the
            // active plot channel because, if we are in histo mode,
            // we have anyway something to plot (the histogram).
            if (av->chplot == ch) {
				plotEvent(av, event);
			}
        }
        else {
		    for (ev = 0; ev < (int)numEvents; ev++) {
			    if ((ret = getEvent(dig, &event, ch, ev)) != 0) return ret;
			    analyzeEvent(av, event, ch, ev);
			    if (av->chplot == ch) {
				    plotEvent(av, event);
			    }
			    dumpEvent(av, event, ch);
			    /*if ((av->stopTime[ch]>0) && (av->runTime[ch] >= av->stopTime[ch])) {
				    userMsg("End of Run for channel %d.\n", ch);
				    dig->runMask &= ~(1<<ch);
				    break;					
			    }*/
                /* HACK, to make channel wise stoptime to work, we check for each chenabled
                if the total elapsed time is >= stoptime for that channel; if yes we set runMask
                to zero and the acq will stop */
                if ((av->stopTime[ch]>0) && (av->totalElapsedTime >= av->stopTime[ch])) {
				    userMsg("End of Run ( from channel %d)\n", ch);
				    dig->runMask = 0;
				    break;					
			    }
		    }
            if (av->dumpLs && // DEBUG HACK in this way we have much more performance in BINARY mode!!
	            (av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_List || 
		         av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_Mixed)) {
                     //c_dumpDPPBlockEvent(av->dumper[ch], numEvents, dig->events[ch]); // Deprecated
                     c_ddumpb(av->dumper[ch]);
	        }
        }
		//if (av->chplot == ch)
        if ( av->dumpEh ||  av->dumpTh )
            dumpHistos(av);
	}

    // Here we manage the acquisition status if necessary.
    // See function stopAcquisition
    if(av->dig->acqStatus == ACQSTATUS_STOPPING_NOTFLUSHED ||
        av->dig->acqStatus == ACQSTATUS_STOPPING_FLUSHED) {
        
        // Print last trigger rate to notify last flushed time
        if(av->dig->acqStatus == ACQSTATUS_STOPPING_FLUSHED) {
            if (av->dig->acquisitionMode == CAEN_DGTZ_DPP_ACQ_MODE_List) {
			    for(ch=0; ch<MAX_CH; ch++) { printTriggerRate(av, ch); }
		    }
            if(dig->runMask == 0) {
                guiMsg(MSG_STOP_ACQ);
                resetTimeCounters(av);
            }
        }
        if((ret = stopAcquisition(av->dig))!=0)
            return ret;
    } else if (dig->runMask == 0) { /* time's up*/
        userMsg("Time is up.\n");
        if((ret = stopAcquisition(av->dig))!=0)
            return ret;
    }

	return 0;

}

void updateVProbes(Analyzer* ana) {
    if (checkBoardInited(ana->dig)) {
		CAEN_DGTZ_SetDPP_PSD_VirtualProbe(ana->dig->boardHandle, ana->dualTrace, ana->vprobes[1], ana->vprobes[4], ana->vprobes[5]);
	}
}


void cycleVProbe(Analyzer* ana, int vp) {
	static int vprobeMax[] = { 1, 4, 1, 1, 8, 8 };
	if (ana->plotType == PLOT_WAVEFORMS || ana->plotType == PLOT_FFT) {
        if (!ana->traceEnableWaves[vp]) {
            ana->traceEnableWaves[vp] = 1;
			ana->vprobes[vp] = 0;
        } else if (ana->vprobes[vp] == vprobeMax[vp]-1) {
            ana->traceEnableWaves[vp] = 0;
        } else {
            ana->vprobes[vp]++;
        }
		updateVProbes(ana);
    } else {
        ana->traceEnableHist[vp] ^= 1;
    }
}


#endif