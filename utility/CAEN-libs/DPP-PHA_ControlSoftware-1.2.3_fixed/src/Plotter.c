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
#ifndef linux
#include <io.h>
#endif

#include "globs.h"
#include "messages.h"
#include "Plotter.h"
#include "Analyzer.h"
 

/* size in bytes of the trace types, to be accessed via the TraceType enum */
static const int typeSizes[] = { sizeof(uint8_t),
								 sizeof(uint16_t),
								 sizeof(uint32_t),
								 sizeof(int8_t),
								 sizeof(int16_t),
								 sizeof(int32_t),
								 sizeof(float),
								 sizeof(double) };

/* printf format tokens of the trace types, to be accessed via the TraceType enum */
static const char* typeTokens[] = { "%u\t",
									"%u\t",
									"%u\t",
									"%d\t",
									"%d\t",
									"%d\t",
									"%f\t",
									"%f\t" };


Plotter* newPlotter() {
    char tempPath[200];
    char str[200];
    char *sep = "/";
    Plotter* plo = (Plotter*) calloc(1,sizeof(Plotter));
/*#ifdef LINUX
    //tempPath = (char *)malloc(10*sizeof(char));
    strcpy(tempPath, "/tmp/");
#else
    
    strcpy(tempPath, strcat(getenv("TEMP"), sep));
#endif*/
    strcpy(tempPath, strcat(getenv("TEMP"), sep));
    userMsg("Using temporary path: %s\n", tempPath);
	if (plo == NULL) return NULL;
    strcpy(str, tempPath);
    strcat(str, PLOT_DATA_FILE);
    strcpy(plo->plotFileName, str);
    
    strcpy(str, tempPath);
    strcat(str, FIT_RES_FILE);
	strcpy(plo->fitResFileName, str);
    
    strcpy(str, tempPath);
    strcat(str, FIT_LOG_FILE);
	strcpy(plo->fitLogFileName, str);
	plo->fps = DEFAULT_PLOT_FPS;
	return plo;
}

void deletePlotter(Plotter* plo) {
	SAFE_FREE(plo->plotterPath)
	if (plo->plotterPipe != NULL) { pclose(plo->plotterPipe); }
	SAFE_FREE(plo);
}


int initPlotter(Plotter* plo) {
	char plotterCommand[70];	
	//char plotterCommand[50];
	char settingdisplay[] = "export DISPLAY=:0;";
	if (checkPlotterBusy(plo)) return 0;
	strcpy(plotterCommand,settingdisplay);
	strcat(plotterCommand,plo->plotterPath);	
	//strcpy(plotterCommand,plo->plotterPath);
	strcat(plotterCommand,GNUPLOT_COMMAND);
	if ((plo->plotterPipe = popen(plotterCommand, "w")) == NULL) { 
		userMsg("Unable to open plotter\n"); 
		guiMsg(MSG_ERROR,"Unable to open plotter\n");
		return 0; 
	}

	/* send some plot settings */
    fprintf(plo->plotterPipe, "set grid\n");
    fprintf(plo->plotterPipe, "set mouse\n");
    fprintf(plo->plotterPipe, "bind y 'set yrange [Ymin:Ymax]'\n");
    fprintf(plo->plotterPipe, "bind x 'set xrange [Xmin:Xmax]'\n");
    fflush(plo->plotterPipe);

	plo->busy = 0;
    plo->logScaleEnabled = 0;

	plo->signalUpdatePlotterOptions = 1;
	return 0;
}

void deinitPlotter(Plotter* plo) {
	plo->busy = 0;
	pclose(plo->plotterPipe);
	plo->plotterPipe = NULL;
}

static void pipePlotterOptions(Plotter* plo, Plot* pl) {
#ifdef WIN32	
	fprintf(plo->plotterPipe, "set terminal windows\n");
#else
	fprintf(plo->plotterPipe, "set terminal x11\n");
#endif
	fprintf(plo->plotterPipe, "set xlabel '%s'\n", pl->xlabel);
    fprintf(plo->plotterPipe, "set ylabel '%s'\n", pl->ylabel);
    fprintf(plo->plotterPipe, "set title '%s'\n", pl->title);
    fprintf(plo->plotterPipe, "Xs = %f\n", pl->xscale);
    fprintf(plo->plotterPipe, "Ys = %f\n", pl->yscale);
    fprintf(plo->plotterPipe, "Xmax = %f\n", pl->xmax);
    fprintf(plo->plotterPipe, "Ymax = %f\n", pl->ymax);
    fprintf(plo->plotterPipe, "Xmin = %f\n", pl->xmin);
    fprintf(plo->plotterPipe, "Ymin = %f\n", pl->ymin);
	if (pl->xautoscale) {
        fprintf(plo->plotterPipe, "set autoscale x\n");
		fprintf(plo->plotterPipe, "bind x 'set autoscale x'\n");
	} else {
        fprintf(plo->plotterPipe, "set xrange [Xmin:Xmax]\n");
		fprintf(plo->plotterPipe, "bind x 'set xrange [Xmin:Xmax]'\n");
	}
	if (pl->yautoscale) {
        fprintf(plo->plotterPipe, "set autoscale y\n");
		fprintf(plo->plotterPipe, "bind y 'set autoscale y'\n");
	} else {
        fprintf(plo->plotterPipe, "set yrange [Ymin:Ymax]\n");
		fprintf(plo->plotterPipe, "bind y 'set yrange [Ymin:Ymax]'\n");
	}
    /* fprintf(gnuplot, "load 'PlotSettings.cfg'\n"); */
	fprintf(plo->plotterPipe, "PlotData = '%s'\n", plo->plotFileName);
	fprintf(plo->plotterPipe, "FitRes = '%s'\n", plo->fitResFileName);
    if(plo->logScaleEnabled == 1)
        fprintf(plo->plotterPipe, "set logscale y\n");
    else
        fprintf(plo->plotterPipe, "set nologscale y\n");
        //fprintf(plo->plotterPipe, "! /bin/echo $DISPLAY\n");
        //fprintf(plo->plotterPipe, "pwd\n");
	fflush(plo->plotterPipe);
}

int doPlot(Plotter* plo, Plot* pl) {
	int i, s=0, comma=0, c, tmask = 0;
    //int npts=0;
	static int fplotFailCount = 0;
	FILE *fplot;

	if (checkPlotterBusy(plo)) {
		return 0;
	}

    for(i=0; i<pl->numTraces; i++)
		tmask |= getTrace(pl,i)->length>0 ? getTrace(pl,i)->enabled << i : 0;
    if (tmask == 0)  { /* all traces disabled */
		/*printf("Plotter: All the traces are disabled. Nothing to plot.\n");*/
        return PLOTTER_TRACES_ALL_OFF;
	}

	plo->busy = 1;	

	if (plo->signalUpdatePlotterOptions) {
		pipePlotterOptions(plo,pl);
		plo->signalUpdatePlotterOptions = 0;
	}

	fplot = fopen(plo->plotFileName, "w");
    if (fplot == NULL) {
        plo->busy = 0;
		userMsg("Plotter: Unable to open plot data file.\n");
		fplotFailCount++;
		if (fplotFailCount>MAX_PLOT_FILE_FAILS) return PLOTTER_FAIL_OPEN_FILE;
		else return 0; // just a glitch (it happens)
    } else {
		fplotFailCount = 0;
	}

	for (s=0; tmask > 0; s++) {
		fprintf(fplot, "%d\t", s);
		for(i=0; i<pl->numTraces; i++) {
			Trace* tr = getTrace(pl,i);
            if (tmask & (1<<i)) {
				switch (tr->type) {
                    case PLOT_DATA_UINT8 : 
                        {uint8_t *data = (uint8_t *)tr->data;
                        fprintf(fplot, "%u\t", (uint32_t)(data[s] * tr->gain + tr->offset));}
                        break;
                    case PLOT_DATA_UINT16 :
                        {uint16_t *data = (uint16_t *)tr->data;
                        fprintf(fplot, "%u\t", (uint32_t)(data[s] * tr->gain + tr->offset));}
                        break;
                    case PLOT_DATA_UINT32 :
                        {uint32_t *data = (uint32_t *)tr->data;
                        fprintf(fplot, "%u\t", (uint32_t)(data[s] * tr->gain + tr->offset));}
                        break;
                    case PLOT_DATA_INT8 :
                        {int8_t *data = (int8_t *)tr->data;
                        fprintf(fplot, "%d\t", (int32_t)(data[s] * tr->gain + tr->offset));}
                        break;
                    case PLOT_DATA_INT16 :
                        {int16_t *data = (int16_t *)tr->data;
                        fprintf(fplot, "%d\t", (int32_t)(data[s] * tr->gain + tr->offset));}
                        break;
                    case PLOT_DATA_INT32 :
                        {int32_t *data = (int32_t *)tr->data;
                        fprintf(fplot, "%d\t", (int32_t)(data[s] * tr->gain + tr->offset));}
                        break;
					case PLOT_DATA_FLOAT :
						{float *data = (float *)tr->data;
						fprintf(fplot, "%f\t", data[s] * tr->gain + tr->offset);}
						break;
                    case PLOT_DATA_DOUBLE:
                        {double *data = (double *)tr->data;
                        fprintf(fplot, "%f\t", data[s] * tr->gain + tr->offset);}
                        break;
                    default :
						userMsg("Unsupported plot data type\n");
                        return -10;
				}

				/* It didn't work so well.....
				fprintf(fplot,"%f\t",(double)*((char*)tr->data + s*typeSizes[tr->type]) * tr->gain + tr->offset); 
				npts++;	// we use pointer arithmetics and an array with the sizes of the allowed types to lock on the correct position
						// we did away with the cast and we always print numbers as double (makes sense?) */
			}
			if (s == (tr->length-1))
				tmask &= ~(1<<i);
		}
		fprintf(fplot, "\n");
	}
	fclose(fplot);


    /* str contains the plot command for gnuplot */
	fprintf(plo->plotterPipe, " plot ");	
	//fprintf(plo->plotterPipe, " plot ");
    c = 2; /* first column of data */
    for(i=0; i<pl->numTraces; i++) {
		Trace* tr = getTrace(pl,i);
        if (tr->enabled == 0) continue;
	    if (comma)
			fprintf(plo->plotterPipe, ", ");
		fprintf(plo->plotterPipe, "'%s' using ($1*%f+%f):($%d*%f) title '%s' with steps linecolor %d", plo->plotFileName, pl->xscale, pl->xshift, c++, pl->yscale, tr->name, i+1);
	    comma = 1;
    }

	fprintf(plo->plotterPipe, "\n"); 
	fflush(plo->plotterPipe);

	if (pl->type == PLOT_WAVEFORMS || pl->type == PLOT_FFT) 
		plo->finishTime = getTime() + (long)(1000.0/plo->fps); /* sets the time we give the plotter to finish (based on an FPS setting) */
	else 
		plo->finishTime = getTime() + 1000;

	return 0;
}

#ifdef DPP_PSD
int doPlot3d(Plotter* plo, Plot* pl) {
	int i, j, tmask = 0;
    //int npts=0, comma=0, s=0;
	int xlen, ylen;
	static int fplotFailCount = 0;
	FILE *fplot;

	if (checkPlotterBusy(plo)) {
		return 0;
	}

    for(i=0; i<pl->numTraces; i++)
		tmask |= getTrace(pl,i)->length>0 ? getTrace(pl,i)->enabled << i : 0;
    if (tmask == 0)  { /* all traces disabled */
		/*printf("Plotter: All the traces are disabled. Nothing to plot.\n");*/
        return PLOTTER_TRACES_ALL_OFF;
	}

	plo->busy = 1;	

	if (plo->signalUpdatePlotterOptions) {
		pipePlotterOptions(plo,pl);
		plo->signalUpdatePlotterOptions = 0;
	}
	
	fplot = fopen(plo->plotFileName, "w");
    if (fplot == NULL) {
        plo->busy = 0;
		userMsg("Plotter: Unable to open plot data file.\n");
		fplotFailCount++;
		if (fplotFailCount>MAX_PLOT_FILE_FAILS) return PLOTTER_FAIL_OPEN_FILE;
		else return 0; // just a glitch (it happens)
    } else {
		fplotFailCount = 0;
	}

	xlen = getTrace(pl,0)->length;
	ylen = getTrace(pl,1)->length;
			
	for(i=0; i < xlen; i++) {
		for(j=0; j < ylen; j++) {
			uint32_t* data = (uint32_t*)getTrace(pl,2)->data;
			fprintf(fplot, "%d %f %d\n", (int)(((double)i/(double)xlen)*QNORM), (double)j/ylen, data[j*xlen+i]);
		}
		fprintf(fplot, "\n");
	}
	fclose(fplot);
	fprintf(plo->plotterPipe, "unset grid; set palette model CMY rgbformulae 7,5,15; set pm3d map\n");
    fprintf(plo->plotterPipe, "plot '%s' with image\n", plo->plotFileName);
    //fprintf(plo->plotterPipe, "splot '%s'\n", plo->plotFileName);
	fflush(plo->plotterPipe);	

	plo->finishTime = getTime() + 1000;
	return 0;
}
#else
int doPlot3d(Plotter* plo, Plot* pl) {}
#endif

int checkPlotterBusy(Plotter* plo) {
	if (getTime() > plo->finishTime) plo->busy = 0;
	return plo->busy;
}

void waitPlotter(Plotter* plo) {
	while(checkPlotterBusy(plo)) sleep_(100);
}


int doManualFit(Plotter* plo, FitParams* fv) {
	fprintf(plo->plotterPipe, "xmin = %d\n", fv->xmin);
	fprintf(plo->plotterPipe, "xmax = %d\n", fv->xmax);
	fprintf(plo->plotterPipe, "a = %f\n", fv->a);
	fprintf(plo->plotterPipe, "b = %f\n", fv->b);
	fprintf(plo->plotterPipe, "c = %f\n", fv->c);
	fprintf(plo->plotterPipe, "fa = %d\n", fv->fa);
	fprintf(plo->plotterPipe, "fb = %d\n", fv->fb);
	fprintf(plo->plotterPipe, "fc = %d\n", fv->fc);
	fprintf(plo->plotterPipe, "load 'manualfit.cmd'\n");
	fflush(plo->plotterPipe);
	return 0;
}

int checkFitterFinished(Plotter* plo) {
	return !_access(plo->fitResFileName,4);
}

int parseFitResults(Plotter* plo, FitParams* fp) { /* returns 0 if fit converged, 1 if fit mode was abandoned, -1 if error found (this one not yet implemented) */
	FILE* ffit;
	char outcome;
    int tmp_fscnfres; // to avoid compiler warnings
	ffit = fopen(plo->fitResFileName,"r");
	if (ffit == NULL) {
		perror("Error while opening FitRes.txt: ");
		return -1;
	}
	tmp_fscnfres = fscanf(ffit, "%c %d %d %f %f %f", &outcome, &fp->xmin, &fp->xmax, &fp->a, &fp->b, &fp->c);
	fclose(ffit);
	remove(plo->fitResFileName);
	if (outcome=='y') return 0;
	else if (outcome=='a') return 1;
	else return -1;
}

void startFitter(Plotter* plo) {
	fprintf(plo->plotterPipe, "load 'fit.cmd'\n");
	fflush(plo->plotterPipe);
}

void stopFitter(Plotter* plo) {
	fprintf(plo->plotterPipe, "load 'nofit.cmd'\n");
	fflush(plo->plotterPipe);
}
