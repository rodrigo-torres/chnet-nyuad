#ifndef MAIN_H
#define MAIN_H

#include "DPPrunner.h"

typedef struct {
	char configFileName[100];
	// Parameter for the communication
    int LinkType;
    int LinkNum;
    int ConetNode;
    uint32_t BaseAddress;

	// Parameter for the digitizer setting
    int Nch;
    int NumEvents;
    int RecordLength;
    int PostTrigger;
    int PulsePolarity;
    int FPIOtype;
    int Vprobe_a1;
    int Vprobe_a2;
    int Vprobe_d;
    int AcqMode;
    int DualTrace;
    int ListParams;
	int Decimation;
	DPPparams_t DPPparams[MAX_CH];
    uint8_t EnableMask;
    char GnuPlotPath[1000];
    CAEN_DGTZ_TriggerMode_t ExtTriggerMode;
    CAEN_DGTZ_TriggerMode_t ChannelTriggerMode[MAX_CH];
    uint32_t DCoffset[MAX_CH];



    // Variables for the plots
    int SetPlotOptions;
    int PlotType;
    int ContinuousPlot;
    int SinglePlot;
    int DoPlot;
    int RePlot;
    int ChPlot;
    int TraceEnable[4];

	// Data buffers and variables for the analysis
    uint32_t *EHisto[MAX_CH], *EHistoDT[MAX_CH], *THisto[MAX_CH], *RebinHisto, *CorrHisto;  // Histograms
	float *purHisto[MAX_CH], *RebinHistoFloat;  // rejected pulses histogram
	uint64_t Ecnt[MAX_CH], Tcnt[MAX_CH], TrgCnt[MAX_CH], PURcnt[MAX_CH];  // counters
	uint64_t TrgCntRate[MAX_CH], PURcntRate[MAX_CH];  // counter markers for the rate calculation
	uint64_t EcntCorr[MAX_CH], PURcntCorr[MAX_CH];  // counter markers for the rate calculation
	uint32_t PrevTime[MAX_CH];
	uint64_t RunTime[MAX_CH], ExtendedTT[MAX_CH], StartTime[MAX_CH], StopTime[MAX_CH];
    uint64_t RunTimeCorr[MAX_CH], RunTimeRate[MAX_CH];
    uint32_t EHistoNbit, THistoNbit;
    uint32_t DTCevents, DTCtime;
	float Enorm[MAX_CH], Tnorm;
	uint32_t TmaxHisto;  // Full scale range of the time stamp for the histogram (in usec)
    float EmaxSkim, EminSkim;
    int EnableESkim;
    float ECalibQ[MAX_CH], ECalibM[MAX_CH];  // energy calibration
    uint32_t Tsampl;  // sampling period
} DPPRunnerConfig;


void quitProgram(int ret);




#endif