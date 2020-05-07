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

#define ACQSTATUS_STOPPED  0                // Acquisition is not running
#define ACQSTATUS_STOPPING_FLUSHED 1        // Acquisition is stopping and datas have been flushed. We assume here that at the next readout no data is left on the board
#define ACQSTATUS_STOPPING_NOTFLUSHED 2     // Acquisition is stopping but data are still not flushed. We may still have complete aggregates to readout
#define ACQSTATUS_RUNNING  3                // Acquisition is running

#if defined(DPP_TF_NEW)
	#include "Digitizer_new.h"
#elif defined(DPP_PSD)
	#include "Digitizer_ng.h"
#elif defined(DPP_CI)
	#include "Digitizer_ci.h"
#elif defined(DPP_TF_OLD)

#ifndef DIGITIZER_H
#define DIGITIZER_H

#include <CAENDigitizer.h>
#include <CAENDigitizerType.h>

#include "_CAENDigitizer.h"

#define MAX_CH 8
#define MAX_FREE_WRITES 100

typedef struct {
	int M;
	int m;
	int k;
	int ftd;
	int a;
	int b;
	int thr;
	int nsbl;
	int nspk;
	int pkho;
	int tpet;
    int otrej;
    int trgho;
    int twwdt;
    int trgwin;
	float dgain;
} DppParams;

typedef struct {
	int signalRestart;
	int boardOpen;
	int boardHandle;
	char* readoutBuffer;
	uint32_t bufferSize, bufferOccupancy;
	CAEN_DGTZ_ConnectionType linkType;
	int linkNum;
	int conetNode;
	unsigned int baseAddress;
	CAEN_DGTZ_BoardInfo_t* boardInfo;
	int numChannels;
	int enableMask, runMask;
	int tsampl[MAX_CH];
	int decimation[MAX_CH];
	DppParams dppParams[MAX_CH];
	int acqStatus;
	int acquisitionMode;
	int listParams;
	int continuousTrigger;
	uint64_t prevRateTime;
	int recordLength;
	int numFreeWrites;
	Register_t freeWrites[MAX_FREE_WRITES]; /* to directly write to the board's registers*/
	int postTrigger, fpioType, pulsePolarity;
	int maxNumEvents;
	CAEN_DGTZ_TriggerMode_t extTriggerMode;
	int dcOffset[MAX_CH];
	float ediv;
	int signalTimeReset; /* used to signal the analyzer that start/extended timestamps be reset*/
	uint64_t flushTimeout; /* used to force a flush of the readout buffer every sec*/
	_CAEN_DGTZ_Waveforms_t* waveforms;
} Digitizer;


Digitizer* newDigitizer();
void deleteDigitizer(Digitizer* dig);
int initDigitizer(Digitizer* dig, int reopen); /* OPEN DIGI ...*/
void deinitDigitizer(Digitizer* dig, int close);

int doAcquisition(Digitizer* dig);

int startAcquisition(Digitizer* dig);
int stopAcquisition(Digitizer* dig);

int sendSingleTrigger(Digitizer* dig);
void enableContinuousTrigger(Digitizer* dig, int enabled);
int checkContinuousTrigger(Digitizer* dig);

int getNumEvents(Digitizer* dig, uint32_t* numEvents);
int getEvent(Digitizer* dig, CAEN_DGTZ_V1724DPP_TF_EVENT_t** event, CAEN_DGTZ_EventInfo_t** eventInfo, int ev);
void freeEvent(Digitizer* dig, CAEN_DGTZ_V1724DPP_TF_EVENT_t** event, CAEN_DGTZ_EventInfo_t** eventInfo);

int getBufferOccupancy(Digitizer* dig);

Register_t* getFreeWrite(Digitizer* dig, int reg);
void addFreeWrite(Digitizer* dig, uint32_t address, uint32_t data);
int freeWritesFull(Digitizer* dig);

int checkBoardInited(Digitizer* dig); /* whether the board resources have been requested, the dpp has been programmed and buffers have allocated*/

int checkChannelEnabled(Digitizer* dig, int ch);

#endif

#endif

int _getFWRel(CAEN_DGTZ_BoardInfo_t *BInfo, int32_t *Maj, int32_t *Min);
