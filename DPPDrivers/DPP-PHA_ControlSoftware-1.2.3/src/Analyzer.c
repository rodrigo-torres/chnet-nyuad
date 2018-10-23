


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

#ifdef DPP_TF_OLD

#include <stdarg.h>

#include "globs.h"
#include "messages.h"
#include "Analyzer.h"
#include "fft.h"

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
        Hout[i] = (uint32_t)(h+0.5);  /* round */
        h = (float)Hin[j] * (((j+1)*bs - (i+1)) / bs); /* reported to next Hout bin */
        j++;
        if (j >= 32768) break;/* end of Hin */
    }
    /* fill the rest of the histogram with zeros */
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
        h = Hin[j] * (((j+1)*bs - (i+1)) / bs); /* reported to next Hout bin */
        j++;
        if (j >= 32768) break;/* end of Hin */
    }
    /* fill the rest of the histogram with zeros */
    for(j=i; j<16384; j++)
        Hout[j]=0;
}

static void rebinAll(Analyzer* av) {
	int i;
	histoRebinning(av->ehisto[av->chplot], av->rebinHisto, 1/av->enorm[av->chplot]);
	for(i=0; i<(1<<(av->ehistoNbit+1)); i++)
		av->corrHisto[i] = av->ehisto[av->chplot][i] + (int)av->purHisto[av->chplot][i];
	histoRebinning(av->corrHisto, av->rebinCorrHisto, 1/av->enorm[av->chplot]);
	histoRebinningF(av->purHisto[av->chplot], av->rebinHistoF, 1/av->enorm[av->chplot]);
	av->rebinned = 1;
}


static void freeHistoBuffers(Analyzer* av) {
    int ch;
	SAFE_FREE(av->rebinHisto)
	SAFE_FREE(av->rebinHistoF)
	SAFE_FREE(av->corrHisto)
	SAFE_FREE(av->rebinCorrHisto)
    for (ch=0; ch < av->dig->numChannels; ch++) {
		SAFE_FREE(av->ehisto[ch])
		SAFE_FREE(av->ehistoDT[ch])
		SAFE_FREE(av->thisto[ch])
		SAFE_FREE(av->purHisto[ch])
	}
}

void clearHistoBuffers(Analyzer* av) {
    int ch;
	memset(av->rebinHisto, 0, (1<<av->ehistoNbit) * sizeof(uint32_t));
	memset(av->corrHisto, 0, (1<<(av->ehistoNbit+1)) * sizeof(uint32_t));
	memset(av->rebinHistoF, 0, (1<<av->ehistoNbit) * sizeof(float));
	memset(av->rebinCorrHisto, 0, (1<<av->ehistoNbit) * sizeof(uint32_t));

    for (ch=0; ch<av->dig->numChannels; ch++) {
		if (av->dig->enableMask & (1<<ch)) {
			if (av->ehisto[ch] != NULL) memset(av->ehisto[ch], 0, (1<<(av->ehistoNbit+1)) * sizeof(uint32_t));
			if (av->ehistoDT[ch] != NULL) memset(av->ehistoDT[ch], 0, (1<<(av->ehistoNbit+1)) * sizeof(uint32_t));
			if (av->purHisto[ch] != NULL) memset(av->purHisto[ch], 0, (1<<(av->ehistoNbit+1)) * sizeof(float));
			if (av->thisto[ch] != NULL) memset(av->thisto[ch], 0, (1<<av->thistoNbit) * sizeof(uint32_t));
			av->ecnt[ch] = 0;
			av->tcnt[ch] = 0;
			av->PURcnt[ch] = 0;
			av->ecntCorr[ch] = 0;
			av->PURcntCorr[ch] = 0;
			av->PURcntRate[ch] = 0;
		}
	}
}


static int allocateHistoBuffers(Analyzer* av) {
    int ch, ret=0;
	ret |= (av->rebinHisto = (uint32_t*) calloc(1 << av->ehistoNbit, sizeof(uint32_t))) == NULL;
	ret |= (av->corrHisto = (uint32_t*) calloc(1 << (av->ehistoNbit+1), sizeof(uint32_t))) == NULL;
	ret |= (av->rebinHistoF = (float*) calloc(1 << av->ehistoNbit, sizeof(float))) == NULL;
	ret |= (av->rebinCorrHisto = (uint32_t*) calloc(1 << (av->ehistoNbit), sizeof(uint32_t))) == NULL;

    for (ch=0; ch<av->dig->numChannels; ch++) {
		if (av->dig->enableMask & (1<<ch)) {
			ret |= (av->ehisto[ch] = (uint32_t*) calloc(1 << (av->ehistoNbit+1), sizeof(uint32_t))) == NULL;
			ret |= (av->ehistoDT[ch] = (uint32_t*) calloc(1 << (av->ehistoNbit+1), sizeof(uint32_t))) == NULL;
			ret |= (av->purHisto[ch] = (float*) calloc(1 << (av->ehistoNbit+1), sizeof(float))) == NULL;
			ret |= (av->thisto[ch] = (uint32_t*) calloc(1 << (av->ehistoNbit+1), sizeof(uint32_t))) == NULL;
			if (ret) break;
		}
	}
	if (ret) {
		freeHistoBuffers(av);
		return -1;
	}
    return 0;
}



static void analyzeEvent(Analyzer* av, CAEN_DGTZ_V1724DPP_TF_EVENT_t* event, CAEN_DGTZ_EventInfo_t* eventInfo) {
    Digitizer* dig = av->dig;
	int ret = 0;
	int ch;
	int debugPrinted = 0;

	av->plotFilter = (av->eSkimEnabled == 0);
	av->rebinned = 0;

	for(ch=0; ch<dig->numChannels; ch++) { 
		int i;
		if (!(eventInfo->ChannelMask & dig->runMask & (1<<ch)))
			continue;

        for(i=0; i<(int)event->ChTimeTagSize[ch]; i++) {
			uint32_t energy, deltaT, tbin, tt;
            uint64_t deltaEcnt, elapsedT;

			tt = event->TimeTag[ch][i];
			av->trgCnt[ch]++;

			if (av->startTime[ch] == 0) {
				//av->startTime[ch] = (uint64_t)(av->dig->tsampl[ch]) * tt;
                av->startTime[ch] = 1; // HACK, this is to use a common startTime for all channels (which is the time of startAcquisition + 1 clock)
				av->totalElapsedTime = av->runTime[ch] = av->runTimeCorr[ch] = av->extendedTT[ch] = av->prevTime[ch] = 0;
            } else {
				if (tt < av->prevTime[ch]) { av->extendedTT[ch]++; }
				//av->runTime[ch] = dig->tsampl[ch] * ((av->extendedTT[ch] << 30) | (uint64_t)tt) - av->startTime[ch];
                av->runTime[ch] = dig->tsampl[ch] * ((av->extendedTT[ch] << 30) | (uint64_t)tt);

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
				deltaT = av->dig->tsampl[ch] * ((tt - av->prevTime[ch]) & 0x7FFFFFFF);
				tbin = (int)(deltaT / av->tnorm);
				if ((tbin > 0) && (tbin < (uint32_t)(1<<av->thistoNbit))) {
					av->thisto[ch][tbin]++;
					av->tcnt[ch]++;
				}
			}
			av->prevTime[ch] = tt;

			energy = event->Energy[ch][i] & ((1<<(av->ehistoNbit+1))-1);

			av->normEn[ch][i] = energy / av->enorm[ch];
			av->ttExt[ch][i] = tt | (av->extendedTT[ch] << 30);

			av->plotFilter = (ch == av->chplot) && ((!av->eSkimEnabled) || (av->eSkimEnabled && av->normEn[ch][i] < av->emaxSkim && av->normEn[ch][i] > av->eminSkim));
			
			if (energy > 0) {
				av->ehisto[ch][energy]++;
				av->ehistoDT[ch][energy]++;
				av->ecnt[ch]++;
			} else {  // PUR
				av->PURcnt[ch]++;
			}

			elapsedT = av->runTime[ch] - av->runTimeCorr[ch];
			deltaEcnt = av->ecnt[ch] - av->ecntCorr[ch];
			if ((deltaEcnt > 0) && ((elapsedT > av->dtcTime) || (deltaEcnt > av->dtcEvents))) {
				int j;
				float deadTime = (float)dig->dppParams[ch].b;  /* forse va aumentato HACK */
				float tratio = deadTime / (float)elapsedT;
                uint64_t no = deltaEcnt + (av->PURcnt[ch] - av->PURcntCorr[ch]);  /* num of pulses in ElapsedT (including PUR)*/
				float cntLoss = (float)(av->PURcnt[ch] - av->PURcntCorr[ch]); /* pulses lost for pile-up */
                for(j=0; j<(1<<(av->ehistoNbit+1)); j++) {
                    av->purHisto[ch][j] += cntLoss * (float)av->ehistoDT[ch][j] / (float)deltaEcnt;
                }
				memset(av->ehistoDT[ch], 0, (1<<(av->ehistoNbit+1)) * sizeof(uint32_t));
				av->PURcntCorr[ch] = av->PURcnt[ch];
                av->ecntCorr[ch] = av->ecnt[ch];
				av->runTimeCorr[ch] = av->runTime[ch];
			}
        }
	}
}


static void plotEvent(Analyzer* av, CAEN_DGTZ_V1724DPP_TF_EVENT_t* event, CAEN_DGTZ_EventInfo_t* eventInfo) {
	if (!checkChannelEnabled(av->dig,av->chplot)) return;
	if (av->continuousPlot || av->singlePlot) {
		int plotThisWave = event->ChTraceSize[av->chplot] > 2;
		if (av->plotType == PLOT_WAVEFORMS && ((plotThisWave && av->plotFilter) || av->signalReplot)) {
			void* traceData[] = {event->DataTraceA[av->chplot], event->DataTraceB[av->chplot], event->DigitalTraceC[av->chplot], event->DigitalTraceD[av->chplot]};
			makeWaveformPlot(av->plot, av, traceData, event->ChTraceSize[av->chplot]);
		} else if (av->plotType == PLOT_FFT && ((plotThisWave && av->plotFilter) || av->signalReplot)) {
			av->fftDataLength = FFT(event->DataTraceA[av->chplot], av->fftData, event->ChTraceSize[av->chplot], HANNING_FFT_WINDOW);
			makeFFTPlot(av->plot, av);
		} else if (av->plotType == PLOT_EHISTO) {
			rebinAll(av);
			makeEHistoPlot(av->plot, av);
		} else if (av->plotType == PLOT_THISTO) {
			makeTHistoPlot(av->plot, av);
		}
				
		av->singlePlot = 0;
		av->signalReplot = 0;

		if (av->dig->acqStatus != ACQSTATUS_STOPPED) {
			if (doPlot(av->plo, av->plot)==PLOTTER_FAIL_OPEN_FILE) {
				av->continuousPlot = 0;
				userMsg("Too many failures while trying to write to the plot file. Plotting stopped.\n");
				guiMsg(MSG_ERROR,"Too many failures while trying to write to the plot file. Plotting stopped.");
				guiMsg(MSG_STOP_CPLOT);
			}
		}
	}
}



static void curveFit(Analyzer* ana) {
	if (ana->fitMode) {
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
	}

	/* startFitMode() -- gnuplot goes in fit mode
	 * each time user presses 'f' in gnuplot a roi is defined and a fit is made. results are written to a file
	 * maybe signalManualFit()
	 * whenever the runner sees a fit res file, it parses it and sends the contents to the gui, then deletes the file
	 * 
	 * either stopFitMode() -- gnuplot exits fit mode, exit loop
	 * or loop over */
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


static void dumpEvent(Analyzer* av, CAEN_DGTZ_V1724DPP_TF_EVENT_t* event, CAEN_DGTZ_EventInfo_t* eventInfo) {
	int plotThisWave;
	if (!checkChannelEnabled(av->dig,av->chplot)) return;
	plotThisWave = event->ChTraceSize[av->chplot] > 2;
	if (av->dumpWf && plotThisWave && 
	    (av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope || 
		 av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_Mixed)) {
		void* traceData[] = {event->DataTraceA[av->chplot], event->DataTraceB[av->chplot], event->DigitalTraceC[av->chplot], event->DigitalTraceD[av->chplot]};
		makeWaveformPlot(av->dump, av, traceData, event->ChTraceSize[av->chplot]);
		doDump(av->dpr, av->dump);
	}
	if (av->dumpLs && 
	    (av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_List || 
		 av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_Mixed)) {
		makeListDump(av->dump, av);
		doDump(av->dpr, av->dump);
	}
	if (av->dumpEh && 
	    (av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_List || 
		 av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_Mixed)) {
		if (!av->rebinned) rebinAll(av);
		makeEHistoPlot(av->dump, av);
		doDump(av->dpr, av->dump);
	}
	if (av->dumpTh && 
	    (av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_List || 
		 av->dig->acquisitionMode==CAEN_DGTZ_DPP_ACQ_MODE_Mixed)) {
		makeTHistoPlot(av->dump, av);
		doDump(av->dpr, av->dump);
	}
}


int initAnalyzer(Analyzer* ana) {
	int i,shf;
	for (i=0; i<ana->dig->numChannels; i++) {
		for (shf=0; shf<32; shf++) {
			if ((1<<(shf+1)) >= (int)ana->dig->ediv) break;
		}
		ana->enorm[i] = ana->dig->ediv / (float)(1<<shf);
		ana->normEn[i] = calloc(1<<15,sizeof(uint32_t));
		ana->normEnSize[i] = 1<<15;
		ana->ttExt[i] = calloc(1<<15,sizeof(uint32_t));
		ana->ttExtSize[i] = 1<<15;
	}
	ana->plot = newPlot(MAX_NUM_TRACES, max(ana->dig->recordLength, 1<<15));
	if (ana->plot == NULL) return -1;

	ana->dump = newPlot(MAX_NUM_TRACES, max(ana->dig->recordLength, 1<<15));
	if (ana->dump == NULL) return -1;
	
	for (i=0; i<MAX_NUM_TRACES; i++)
		ana->traceEnable[i]=1;

	updateVProbes(ana);
	
	ana->fftData = calloc(max(ana->dig->recordLength, 1<<15),sizeof(double));
	ana->tnorm = (float)ana->tmaxHisto / (1<<ana->thistoNbit);

	return allocateHistoBuffers(ana);
}

void deinitAnalyzer(Analyzer* ana) {
	int i;
	clearHistoBuffers(ana);
	freeHistoBuffers(ana);
	SAFE_FREE(ana->fftData);
	for (i=0; i<ana->dig->numChannels; i++) {
		SAFE_FREE(ana->normEn[i]);
		SAFE_FREE(ana->ttExt[i]);
	}
	deletePlot(ana->plot);
	deletePlot(ana->dump);

}


Analyzer* newAnalyzer(Digitizer* dig, Plotter* plo, Dumper* dpr) {
	Analyzer* av = (Analyzer*) calloc(1,sizeof(Analyzer));
	if (av == NULL) return NULL;
	av->dig = dig;
	av->plo = plo;
	av->dpr = dpr;
	av->ehistoNbit = 14;
	av->thistoNbit = 12;
	
	return av;
}


void deleteAnalyzer(Analyzer* av) {
	freeHistoBuffers(av);
	free(av);
	return;
}



int doAnalysis(Analyzer* av) {
	Digitizer* dig = av->dig;
	CAEN_DGTZ_EventInfo_t* eventInfo = NULL;
	CAEN_DGTZ_V1724DPP_TF_EVENT_t* event = NULL;
	uint32_t numEvents = 0;
	static int cumulativeBufferOccupancy = 0;
	int ev;
	int ret;
	uint64_t currentTime, elapsedTime;

	/*if (av->signalReplot) 
		doPlot(getPlotter(),av->plot); // signalReplots
	av->signalReplot = 0;*/

	if (dig->signalTimeReset) {
		int i = 0;
		for (; i < MAX_CH; i++)
			av->startTime[i] = av->runTime[i] = av->extendedTT[i] = av->runTimeCorr[i] = av->totalElapsedTime = 0;
		dig->signalTimeReset = 0;
		clearHistoBuffers(av);
	}

	curveFit(av);

	if (dig->acqStatus == ACQSTATUS_STOPPED) {
		return 0;
	}

	currentTime = getTime();
    elapsedTime = currentTime - av->prevRateTime;
	
	/* Calculate throughput and trigger rate (every second) */
	cumulativeBufferOccupancy += getBufferOccupancy(dig);
	if (elapsedTime >= 1000) {
		int ch;
		printReadoutRate(cumulativeBufferOccupancy,elapsedTime);
		for(ch=0; ch<MAX_CH; ch++) { printTriggerRate(av, ch); }
		cumulativeBufferOccupancy = 0;
		av->prevRateTime = currentTime;
    }



	if (ret = getNumEvents(dig, &numEvents)) return ret;
    for(ev = 0; ev < (int)numEvents; ev++) {
		/* Get one event from the readout buffer */
		if (ret = getEvent(dig,&event,&eventInfo,ev)) return ret;
		analyzeEvent(av, event, eventInfo);
		plotEvent(av, event, eventInfo);
		dumpEvent(av, event, eventInfo);
		freeEvent(dig,&event,&eventInfo);
    }
	return 0;

}

void updateVProbes(Analyzer* ana) {
	if (checkBoardInited(ana->dig)) {
		int wasEnabled = ana->dig->acqStatus;
		if (wasEnabled != ACQSTATUS_STOPPED) stopAcquisition(ana->dig);  /* HACK: si puo'levare quando metto i virt. probe nei dati */
		CAEN_DGTZ_SetDPP_TF_VirtualProbe(ana->dig->boardHandle, ana->dualTrace, ana->vprobes[0], ana->vprobes[1], ana->vprobes[2]);
		if (wasEnabled != ACQSTATUS_STOPPED) startAcquisition(ana->dig);
	}
}


#endif