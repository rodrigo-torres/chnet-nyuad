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

#ifdef DPP_PSD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globs.h"
#include "Digitizer.h"
#include "messages.h"
#include "CAENLogger.h"

#define LICENSE_REG 0x8158
#define LICENSE_COUNTDOWN_MULT 84

#define PSD_GATEDSTART_MASK 0x2

#define SENDFLUSH_PSD(HANDLE) CAEN_DGTZ_WriteRegister(HANDLE, 0x803C, 0) // HACK, is ok this reg for flush?

INIT_C_LOGGER("DppRunnerLog.txt", "Digitizer_ng.c");

char *FWFilename = NULL; // filename used for free writes, it is filled in newConfigurator function

static void checkLicense(Digitizer* dig) {
	uint32_t countdown1, countdown2;
	CAEN_DGTZ_ReadRegister(dig->boardHandle, LICENSE_REG, &countdown1);
	sleep_(100);
	CAEN_DGTZ_ReadRegister(dig->boardHandle, LICENSE_REG, &countdown2);
	if (countdown1 != countdown2 || countdown2 == 0) {
		int minleft = countdown2 * LICENSE_COUNTDOWN_MULT / 60000;
		char text[100];
		sprintf(text, "Warning: firmware is unlicensed. Remaining demo mode time: %d minutes", minleft);
		guiMsg(MSG_ERROR,text);
		userMsg("Warning: firmware is unlicensed. Remaining demo mode time: %d minutes\n", minleft);
	}
}


static int programDigitizer(Digitizer* dig) {
	enum ProgrammingError { NOERR, RESET, RECLEN, IOLEVEL, ACQMODE, 
						EXTTRG, CHENA, EVTAGG, FREEWRT, DCOFF, 
						SELFTRG, PRETRG, POLAR, DPPPRM, DPPACQMODE, SYNCMODE }; 
	int i, ret = 0, ee = NOERR;
    int nEvtAggr = 0;
    Register_t GWArray[MAX_FW_NUM];
    int FWNum;

	//int eventsPerAggr = (dig->acquisitionMode == CAEN_DGTZ_DPP_ACQ_MODE_List) ? 1000 : 10;
    /* reset the digitizer */
	if(dig->resetEnabled == 1)
        if ( CAEN_DGTZ_Reset(dig->boardHandle)) ee = RESET;

    // HACK FOR x751, if nEvtAggr != 1 -> event build error!!
    //if (dig->boardInfo->FamilyCode == CAEN_DGTZ_XX751_FAMILY_CODE) nEvtAggr = 1;

    /* execute generic write commands */
    //for(i=0; i<dig->numFreeWrites; i++)
	//	if (CAEN_DGTZ_WriteRegister(dig->boardHandle, dig->freeWrites[i].address, dig->freeWrites[i].data)) ee = FREEWRT;
  
	if (CAEN_DGTZ_SetChannelEnableMask(dig->boardHandle, dig->enableMask)) ee = CHENA;
    if (CAEN_DGTZ_SetDPPAcquisitionMode(dig->boardHandle, dig->acquisitionMode, dig->listParams)) ee = DPPACQMODE;
    if (CAEN_DGTZ_SetAcquisitionMode(dig->boardHandle, CAEN_DGTZ_SW_CONTROLLED)) ee = ACQMODE;
    if (CAEN_DGTZ_SetIOLevel(dig->boardHandle, dig->fpioType)) ee = IOLEVEL;
    if (CAEN_DGTZ_SetExtTriggerInputMode(dig->boardHandle, dig->extTriggerMode)) ee = EXTTRG;
	if (CAEN_DGTZ_SetDPPEventAggregation(dig->boardHandle, dig->nEvtAggr, 0)) ee = EVTAGG;
    if (CAEN_DGTZ_SetRunSynchronizationMode(dig->boardHandle, CAEN_DGTZ_RUN_SYNC_Disabled)) ee = SYNCMODE;
    if (CAEN_DGTZ_SetDPPParameters(dig->boardHandle, dig->enableMask, &dig->dppParams)) ee = DPPPRM;
    for(i=0; i<dig->numChannels; i++) {
		if (dig->enableMask & (1<<i)) {
            int preTrg;
			ret |= CAEN_DGTZ_SetRecordLength(dig->boardHandle, dig->recordLength[i], i);
		    ret |= CAEN_DGTZ_SetChannelDCOffset(dig->boardHandle, i, dig->dcOffset[i]);
            if (dig->acquisitionMode == CAEN_DGTZ_DPP_ACQ_MODE_List)
                preTrg = dig->dppParams.pgate[i]+PRETRG_PREGATE_MINDIFF_PSD; // In list mode, the pretrigger must be forced to be the smallest possible (preGate+mindiff) 
            else
                preTrg = dig->preTrigger[i];
            ret |= CAEN_DGTZ_SetDPPPreTriggerSize(dig->boardHandle, i, preTrg);
            ret |= CAEN_DGTZ_SetChannelPulsePolarity(dig->boardHandle, i, dig->pulsePolarity[i]);
            //ret |= CAEN_DGTZ_SetChannelSelfTrigger(dig->boardHandle, CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT, (1<<i));
		}
	}
	
    if(dig->boardInfo->FamilyCode == CAEN_DGTZ_XX720_FAMILY_CODE)
        ret |= CAEN_DGTZ_SetDPPTriggerMode(dig->boardHandle, dig->trgMode);
	
	//Pre-Trigger Register
	//ret |= CAEN_DGTZ_WriteRegister(dig->boardHandle, 0x8038, (dig->preTrigger[0] & 0x1FF));
	
    getFreeWritesFromFile(FWFilename, GWArray, &FWNum);
    for(i=0; i<FWNum; i++) {
        int32_t r;
        uint32_t val;

        r = CAEN_DGTZ_ReadRegister(dig->boardHandle, GWArray[i].address, &val);
        val = (val & ~GWArray[i].mask ) | (GWArray[i].data & GWArray[i].mask);
        r |= CAEN_DGTZ_WriteRegister(dig->boardHandle, GWArray[i].address, val);

        if(r)
            ee = FREEWRT;
    }

	if (ret) {
		guiMsg(MSG_ERROR, "Errors found during the programming of the digitizer.\nSome settings may not be executed");
		userMsg("Warning: errors found during the programming of the digitizer.\nSome settings may not be executed\n");
	} else if (ee) {
		char text[100];
		sprintf(text,"Warning: errors found while programming the digitizer. Setting %d may not be executed",ee);
		guiMsg(MSG_ERROR,text);
		userMsg("Warning: errors found while programming the digitizer.\nSetting %d may not be executed\n",ee);
	}


    return 0;
}



Digitizer* newDigitizer() {
	int i;
	Digitizer* dig = (Digitizer*) calloc(1,sizeof(Digitizer));
	if (dig == NULL) return NULL;
	dig->boardInfo = (CAEN_DGTZ_BoardInfo_t*) calloc(1,sizeof(CAEN_DGTZ_BoardInfo_t));
    dig->resetEnabled = 1; // Set here to avoid overwrite at each param change
	dig->restartingEnabled = 1; // HACK Disable restarting mode at startup
    dig->nEvtAggr = 0;
    dig->gatedStartEnabled = 0;
	for (i=0; i < MAX_CH; i++) dig->tsampl[i] = 4;
	return dig;
}

void deleteDigitizer(Digitizer* dig) {
	if (dig->boardOpen) {
		stopAcquisition(dig);
		CAEN_DGTZ_CloseDigitizer(dig->boardHandle);
		dig->boardOpen = 0;
	}
	SAFE_FREE(dig->boardInfo)
	free(dig);
}


int initDigitizer(Digitizer* dig, int reopen) {
	int ret, tsampl, i; 
	int majorNumber;
    char dgtzFamily[20];
    char amcver[20];
	uint32_t size;
	if (!dig->boardOpen || reopen) {
        int32_t FW_MAJ, FW_MIN;

		ret = CAEN_DGTZ_OpenDigitizer(dig->linkType, dig->linkNum, dig->conetNode, dig->baseAddress, &dig->boardHandle);
		if (ret) return ERR_DGZ_OPEN;
	
		ret = CAEN_DGTZ_GetInfo(dig->boardHandle, dig->boardInfo);
		if (ret) return ERR_BOARD_INFO_READ;

		userMsg("Connected to CAEN Digitizer Model %s\n", dig->boardInfo->ModelName);
		userMsg("ROC FPGA Release is %s\n", dig->boardInfo->ROC_FirmwareRel);
		userMsg("AMC FPGA Release is %s\n", dig->boardInfo->AMC_FirmwareRel);

		/* Check firmware rivision (Non-DPP firmwares cannot be used with DPPrunner */
		sscanf(dig->boardInfo->AMC_FirmwareRel, "%d", &majorNumber);
		if (majorNumber != 131 && majorNumber != 132) {
			userMsg("This digitizer doesn't have a DPP-PSD firmware (major %d)\n",majorNumber);
			return ERR_INVALID_BOARD_TYPE;
		}
		
        switch(dig->boardInfo->FamilyCode) {
        case CAEN_DGTZ_XX724_FAMILY_CODE:
            tsampl = 4;
            break;
        case CAEN_DGTZ_XX751_FAMILY_CODE:
            tsampl = 1;
            break;
        default:
            tsampl = 4;
            break;
        }
	    for (i=0; i < MAX_CH; i++) dig->tsampl[i] = tsampl;

		dig->numChannels = dig->boardInfo->Channels;
		dig->enableMask = (1<<dig->numChannels)-1;

		dig->boardOpen = 1;
		guiMsg(MSG_BOARD_INITED);

		guiMsg(MSG_NUM_CHANNELS,dig->numChannels);

        switch(dig->boardInfo->FamilyCode) {
        case CAEN_DGTZ_XX720_FAMILY_CODE: {
            // Read FWRel from ch 0 to see if it a x790
            int is790 = 0;
            uint32_t d32;
            if(strcmp(dig->boardInfo->ModelName, "DT5720B") == 0) {
                userMsg("Is this a DT5790?\n");
                if (CAEN_DGTZ_ReadRegister(dig->boardHandle, CAEN_DGTZ_CHANNEL_AMC_FPGA_FW_BASE_ADDRESS, &d32) == CAEN_DGTZ_Success)
                    if ( (uint16_t)((d32 & 0xffff0000)>> 16) == 0x0000 )
                        is790 = 1;
	        }
            if(is790==1)
                strcpy(dgtzFamily, "x790");
            else
                strcpy(dgtzFamily, "x720");
            break;
        }
        case CAEN_DGTZ_XX751_FAMILY_CODE:
            strcpy(dgtzFamily, "x751");
            break;
        default:
            strcpy(dgtzFamily, "None");
            break;
        }

        _getFWRel(dig->boardInfo, &FW_MAJ, &FW_MIN);
        if(FW_MIN<PSD_PROBES_VER_2_AMC_FW_REL-1)
            sprintf(amcver, "AMCREL%d", 0);
        else
            sprintf(amcver, "AMCREL%d", PSD_PROBES_VER_2_AMC_FW_REL);

        guiMsg(MSG_DGTZ_FAMILY,dgtzFamily, amcver); // send the model family to the GUI

		checkLicense(dig);
	}

    ret = programDigitizer(dig);
	if (ret) return ret;

    //sprintf(deb, "ciao\n");
    //OutputDebugString(deb);

    ret = CAEN_DGTZ_MallocReadoutBuffer(dig->boardHandle, &dig->readoutBuffer, &dig->bufferSize); /* WARNING: This malloc must be done after the digitizer programming */
    //sprintf(deb, "readOutBuffer: %llu, bufferSize: %u\n", dig->readoutBuffer, dig->bufferSize);
    //OutputDebugString(deb);

	ret |= CAEN_DGTZ_MallocDPPEvents(dig->boardHandle, (void **)dig->events, &size);
    //sprintf(deb, "eventsBuffer: %llu, totalSize: %u\n", dig->events, size);
    //OutputDebugString(deb);

    /* Allocate memory for the waveforms */
    ret |= CAEN_DGTZ_MallocDPPWaveforms(dig->boardHandle, (void **)&dig->waveforms, &size);
    //sprintf(deb, "waveforms: %llu, size: %u\n", dig->waveforms, size);
    //OutputDebugString(deb);
	
	if (ret) return ERR_MALLOC;
	
	dig->runMask = dig->enableMask;
	dig->prevRateTime = getTime();

	if (dig->acqStatus != ACQSTATUS_STOPPED) {
		stopAcquisition(dig);
		startAcquisition(dig); /* to start acquiring data again upon restart */
	}
	dig->signalRestart = 0;
    
	return ERR_NONE;
}


void deinitDigitizer(Digitizer* dig, int close) {
	CAEN_DGTZ_FreeReadoutBuffer(&dig->readoutBuffer);
	CAEN_DGTZ_FreeDPPEvents(dig->boardHandle, (void **)dig->events);
	CAEN_DGTZ_FreeDPPWaveforms(dig->boardHandle, dig->waveforms);
	if (close) {
		stopAcquisition(dig);
		CAEN_DGTZ_CloseDigitizer(dig->boardHandle);
		dig->boardOpen = 0;
	}
}



int doAcquisition(Digitizer* dig) {
	int ret;
	//uint32_t numEvents = 0, ev = 0;

	if (dig->acqStatus == ACQSTATUS_STOPPED) return 0;
	
	if (dig->continuousTrigger && dig->acquisitionMode != CAEN_DGTZ_DPP_ACQ_MODE_List) CAEN_DGTZ_SendSWtrigger(dig->boardHandle);

    /* Read data from the board */

    ret = CAEN_DGTZ_ReadData(dig->boardHandle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, dig->readoutBuffer, &dig->bufferOccupancy);
	if (ret) 
		return ret;

	if (dig->bufferOccupancy > 0) {
		ret = CAEN_DGTZ_GetDPPEvents(dig->boardHandle, dig->readoutBuffer, dig->bufferOccupancy, (void **)dig->events, dig->numEvents);
		if (ret) 
			return ret;
	}

	return 0;
}


int getNumEvents(Digitizer* dig, uint32_t* numEvents, int ch) {
	if (dig->bufferOccupancy > 0) *numEvents = dig->numEvents[ch];
	else *numEvents = 0;
	return 0;
}

int getEvent(Digitizer* dig, CAEN_DGTZ_DPP_PSD_Event_t** event, int ch, int ev) {
	*event = &dig->events[ch][ev];
	return 0;
}

int startAcquisition(Digitizer* dig) {
	if (dig->acqStatus != ACQSTATUS_RUNNING) {
        uint32_t d32;
		dig->acqStatus = ACQSTATUS_RUNNING;
		userMsg("Acquisition started\n");
		dig->runMask = dig->enableMask;
        
        // Clear unclean data (without this changing RecordLength at RunTime Freezes the digitizer)
        CAEN_DGTZ_ClearData(dig->boardHandle);

        // Manage gated start (bit 1 of 0x8100)
        CAEN_DGTZ_ReadRegister(dig->boardHandle, CAEN_DGTZ_ACQ_CONTROL_ADD, &d32);
        if(dig->gatedStartEnabled == 1)
            d32 |= PSD_GATEDSTART_MASK;
        else
            d32 &= ~PSD_GATEDSTART_MASK;
        CAEN_DGTZ_WriteRegister(dig->boardHandle, CAEN_DGTZ_ACQ_CONTROL_ADD, d32);
        
        CAEN_DGTZ_SWStartAcquisition(dig->boardHandle);
	    return 0;
	} else return 0;
}

int stopAcquisition(Digitizer* dig) {
	if (dig->acqStatus != ACQSTATUS_STOPPED) {
        int ret=0;
        if(dig->acqStatus == ACQSTATUS_RUNNING) {
            dig->acqStatus = ACQSTATUS_STOPPING_NOTFLUSHED;
		    if((ret = CAEN_DGTZ_SWStopAcquisition(dig->boardHandle)) != 0)
                return ret;
        }
        else if(dig->acqStatus == ACQSTATUS_STOPPING_NOTFLUSHED) {
            dig->acqStatus = ACQSTATUS_STOPPING_FLUSHED;
            if((ret = SENDFLUSH_PSD(dig->boardHandle))!=0) // Sent flush command
                return ret;
            userMsg("Data Flushed\n");
        }
        else if(dig->acqStatus == ACQSTATUS_STOPPING_FLUSHED) {
            dig->acqStatus = ACQSTATUS_STOPPED;
            //ret = CAEN_DGTZ_ClearData(dig->boardHandle); // Once flush is supported, this is not necessary
            userMsg("Acquisition stopped\n");
        }
        return ret;
	} else return 0;
}

int sendSingleTrigger(Digitizer* dig) {
    if (!dig->continuousTrigger) {
		int ret = 0;
		userMsg("Single Software Trigger issued\n");
        ret |= CAEN_DGTZ_SendSWtrigger(dig->boardHandle);
    }
	return -1;
}

void enableContinuousTrigger(Digitizer* dig, int enabled) {
	dig->continuousTrigger = enabled;
	if (enabled) userMsg("Continuous trigger is enabled\n");
    else userMsg("Continuous trigger is disabled\n");
}

int checkContinuousTrigger(Digitizer* dig) {
	return dig->continuousTrigger;
}

Register_t* getFreeWrite(Digitizer* dig, int reg) {
	return &dig->freeWrites[reg];
}

void addFreeWrite(Digitizer* dig, uint32_t address, uint32_t data) {
	getFreeWrite(dig, dig->numFreeWrites)->address = address;
	getFreeWrite(dig, dig->numFreeWrites)->data = data;
	dig->numFreeWrites++;
}

int freeWritesFull(Digitizer* dig) {
	return dig->numFreeWrites < MAX_FREE_WRITES;
}

int checkBoardInited(Digitizer* dig) {
	return dig->boardOpen;
}

int getBufferOccupancy(Digitizer* dig) {
	return dig->bufferOccupancy;
}

int checkChannelEnabled(Digitizer* dig, int ch) {
	return dig->enableMask>>ch & 1;
}


int getDecodedWaveforms(Digitizer* dig, CAEN_DGTZ_DPP_PSD_Event_t* event, void* eventData[6], int* dataSize) {
	int res;
	if (res = CAEN_DGTZ_DecodeDPPWaveforms(dig->boardHandle, event, dig->waveforms))
		return res;
	*dataSize = dig->waveforms->Ns;
	eventData[0] = dig->waveforms->Trace1;
	eventData[1] = dig->waveforms->Trace2;
	eventData[2] = dig->waveforms->DTrace1;
	eventData[3] = dig->waveforms->DTrace2;
	eventData[4] = dig->waveforms->DTrace3;
	eventData[5] = dig->waveforms->DTrace4;
	return 0;
}


#endif