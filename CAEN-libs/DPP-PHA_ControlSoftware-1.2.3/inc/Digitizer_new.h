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


#ifndef DIGITIZER_H
#define DIGITIZER_H

#include <CAENDigitizer.h>
#include <CAENDigitizerType.h>

#include "globs.h"
#include <CAENMultiplatform.h>

#define MAX_CH 8
#define MAX_FREE_WRITES 100

#define FLUSH_TIMEOUT_MS 1000

#define EVENTS_PER_AGGREGATE_HISTO 1000
#define EVENTS_PER_AGGREGATE_OSC 1

#define PHA_PROBES_VER_2_AMC_FW_REL 6

#define EHISTO_NBITS_PHA 14

//typedef struct {
//	int M;
//	int m;
//	int k;
//	int ftd;
//	int a;
//	int b;
//	int thr;
//	int nsbl;
//	int nspk;
//	int pkho;
//	int blho;
//    int otrej;
//    int trgho;
//    int twwdt;
//    int trgwin;
//	int dgain;
//	float enf;
//} DppParams;

typedef struct {
	int signalRestart;
    int restartingEnabled; // This is a flag used to inhibit the restart signal in some condition
    int resetEnabled;
	int boardOpen;
	int boardHandle;
	char* readoutBuffer;
	uint32_t bufferSize, bufferOccupancy;
	CAEN_DGTZ_ConnectionType linkType;
	int linkNum;
	int conetNode;
	int baseAddress;
	CAEN_DGTZ_BoardInfo_t* boardInfo;
	int numChannels;
	int enableMask, runMask;
	int tsampl[MAX_CH];
	CAEN_DGTZ_DPP_PHA_Params_t dppParams;
	int acqStatus;
	int acquisitionMode;
	int listParams;
	int continuousTrigger;
	uint64_t prevRateTime;
	int recordLength;
	int numFreeWrites;
	Register_t freeWrites[MAX_FREE_WRITES]; /* to directly write to the board's registers*/
	int fpioType;
	int maxNumEvents;
	CAEN_DGTZ_TriggerMode_t extTriggerMode;
    int selfTriggerDisabled[MAX_CH];
	CAEN_DGTZ_RunSyncMode_t runSyncMode;

	int dcOffset[MAX_CH];
	float ediv[MAX_CH];
	int signalTimeReset; /* used to signal the analyzer that start/extended timestamps be reset*/
	int signalTimeIsUp; /* used to signal the analyzer that the last operations be done as acquisition is stopping */
	uint64_t flushTimeout; /* used to force a flush of the readout buffer every sec*/

	/* new fields */
	CAEN_DGTZ_DPP_PHA_Event_t* events[MAX_CH];
	uint32_t numEvents[MAX_CH];
	CAEN_DGTZ_DPP_PHA_Waveforms_t* waveforms;
	int preTrigger[MAX_CH], pulsePolarity[MAX_CH];
	int numAggregates;
    int nEvtAggr;
    int gatedStartEnabled;

	int shf; // CUIDADO: debug only
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

int getNumEvents(Digitizer* dig, uint32_t* numEvents, int ch);

int getBufferOccupancy(Digitizer* dig);

Register_t* getFreeWrite(Digitizer* dig, int reg);
void addFreeWrite(Digitizer* dig, uint32_t address, uint32_t data);
int freeWritesFull(Digitizer* dig);

int checkBoardInited(Digitizer* dig); /* whether the board resources have been requested, the dpp has been programmed and buffers have allocated*/

int checkChannelEnabled(Digitizer* dig, int ch);

int getDecodedWaveforms(Digitizer* dig, CAEN_DGTZ_DPP_PHA_Event_t* event, void* traceData[4], int* traceSize);

#endif
