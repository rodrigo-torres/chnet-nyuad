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
#include <stdlib.h>
#include <string.h>

#include "globs.h"
#include "Digitizer.h"
#include "messages.h"
#include "CAENLogger.h"

#define LICENSE_REG 0x8158
#define LICENSE_COUNTDOWN_MULT 84

#define DPP_MISC_REG 0X1080
#define PHA_FAKEEVTS_ROLLOVER_REG DPP_MISC_REG

#define PHA_SELFTRIGGER_REG DPP_MISC_REG
#define PHA_SELFTRIGGER_BIT 24

#define PHA_GATEDSTART_MASK 0x2

#define SENDFLUSH_PHA(HANDLE) CAEN_DGTZ_WriteRegister(HANDLE, 0x803C, 0)

INIT_C_LOGGER("DppRunnerLog.txt", "Digitizer_new.c");

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
							EXTTRG, CHENA, EVTAGG, RUNSYN, FREEWRT, DCOFF, 
							SELFTRG, PRETRG, POLAR, DPPPRM, DPPACQMODE }; 
    int i, ee = NOERR;
    //int ret = 0;
	//int eventsPerAggr = (dig->acquisitionMode == CAEN_DGTZ_DPP_ACQ_MODE_List) ? EVENTS_PER_AGGREGATE_HISTO : EVENTS_PER_AGGREGATE_OSC;
    int shouldreset = 1;
    Register_t GWArray[MAX_FW_NUM];
    int FWNum;

    ///* reset the digitizer */
    //if(dig->boardInfo->FamilyCode == CAEN_DGTZ_XX780_FAMILY_CODE) {
    //    /*uint32_t v;
    //    for(i=0; i<2; i++) {
    //        CAEN_DGTZ_ReadRegister(dig->boardHandle, 0x1038|(i<<8), &v);
    //        if(v&0x1)
    //            shouldreset = 0;
    //    }*/
    //    shouldreset = 0;
    //}
    //if(shouldreset)
    //    if ( CAEN_DGTZ_Reset(dig->boardHandle)) ee = RESET;

    if(dig->resetEnabled == 1)
        if ( CAEN_DGTZ_Reset(dig->boardHandle)) ee = RESET;

    if (CAEN_DGTZ_WriteRegister(dig->boardHandle, 0x811C, 0xC003C)) ee = FREEWRT;  /* serve per mandare SW trg individuali e per abilitare il ts reset*/
    if (CAEN_DGTZ_WriteRegister(dig->boardHandle, 0x8000, 0x01000110)) ee = FREEWRT;  /* Channel Control Reg (indiv trg, seq readout)*/
    if (CAEN_DGTZ_WriteRegister(dig->boardHandle, 0x8004, 0x00000004)) ee = FREEWRT;  /* Enable indiv trgin*/

#ifndef DPP_TF_CORE
	if (CAEN_DGTZ_SetDPPAcquisitionMode(dig->boardHandle, dig->acquisitionMode, dig->listParams)) ee = DPPACQMODE;
#else
	if (CAEN_DGTZ_SetDPPAcquisitionMode(dig->boardHandle, CAEN_DGTZ_DPP_ACQ_MODE_List, dig->listParams)) ee = DPPACQMODE; /* always list mode for cores */
#endif

	if (CAEN_DGTZ_SetAcquisitionMode(dig->boardHandle, CAEN_DGTZ_SW_CONTROLLED)) ee = ACQMODE;
	if (CAEN_DGTZ_SetRecordLength(dig->boardHandle, dig->recordLength)) ee = RECLEN;
	if (CAEN_DGTZ_SetIOLevel(dig->boardHandle, dig->fpioType)) ee = IOLEVEL;
    if (CAEN_DGTZ_SetExtTriggerInputMode(dig->boardHandle, dig->extTriggerMode)) ee = EXTTRG;
	if (CAEN_DGTZ_SetChannelEnableMask(dig->boardHandle, dig->enableMask)) ee = CHENA;

    if (CAEN_DGTZ_SetDPPEventAggregation(dig->boardHandle, dig->nEvtAggr, 0)) ee = EVTAGG;
    if(dig->numAggregates != 0) {
        logMsg(c_logger_Severity_INFO, "Will now manually set MAX_NUM_AGGREGATES_BLT to %d", dig->numAggregates);
        CAEN_DGTZ_SetMaxNumAggregatesBLT(dig->boardHandle, dig->numAggregates);
    }
    /*if (CAEN_DGTZ_SetDPPEventAggregation(dig->boardHandle, 1, 0)) ee = EVTAGG;
    if (CAEN_DGTZ_SetMaxNumAggregatesBLT(dig->boardHandle, 1)) ee = EVTAGG;*/

	if (CAEN_DGTZ_SetRunSynchronizationMode(dig->boardHandle, dig->runSyncMode)) ee = RUNSYN;

    // Set DPPParameters BEFORE Self Trigger or CAEN_DGTZ_SetDPPParameters will overwrite register PHA_SELFTRIGGER_REG
    if (CAEN_DGTZ_SetDPPParameters(dig->boardHandle, dig->enableMask, &dig->dppParams)) ee = DPPPRM;
    for(i=0; i<dig->numChannels; i++) {
		dig->tsampl[i] = 10 * (1<<dig->dppParams.decimation[i]);
		if (dig->enableMask & (1<<i)) {
			int shf;
			float enf = dig->dppParams.enf[i];
			int k = dig->dppParams.k[i]/10;
			int M = dig->dppParams.M[i]/10;
            uint32_t val;
            uint32_t addr;

			for (shf=0; shf<64; shf++) {
				dig->ediv[i] = (k*M)/(enf*(uint64_t)(1<<shf));
				if (dig->ediv[i] < 2)
					break;
            }	
			dig->shf = shf;

            if (CAEN_DGTZ_SetChannelDCOffset(dig->boardHandle, i, dig->dcOffset[i])) ee = DCOFF;
            if (CAEN_DGTZ_WriteRegister(dig->boardHandle, 0x1038+(i<<8), dig->preTrigger[i]>>1)) ee = PRETRG;
			if (CAEN_DGTZ_SetChannelPulsePolarity(dig->boardHandle, i, dig->pulsePolarity[i])) ee = POLAR;

            // Self Trigger
            addr = PHA_SELFTRIGGER_REG|(i<<8);
            if (CAEN_DGTZ_ReadRegister(dig->boardHandle, addr, &val)) ee = SELFTRG;
            val &= ~(0x1<<PHA_SELFTRIGGER_BIT);
            val |= ((dig->selfTriggerDisabled[i])&0x1)<<PHA_SELFTRIGGER_BIT;
            if (CAEN_DGTZ_WriteRegister(dig->boardHandle, addr, val)) ee = SELFTRG;
		}
	}

	//for(i=0; i<dig->numChannels; i++) {
	//	if ((dig->enableMask & (1<<i)) && dig->extTriggerMode>0) {
	//		uint32_t d32;
	//		CAEN_DGTZ_ReadRegister(dig->boardHandle, 0x1080+(i<<8), &d32);
	//		CAEN_DGTZ_WriteRegister(dig->boardHandle, 0x1080+(i<<8), d32 |= 0x40000);  /* workaround to ext trg PUR bug */
	//	}
	//}

    /* execute generic write commands */
    for(i=0; i<dig->numFreeWrites; i++) {
		if (dig->freeWrites[i].address == CAEN_DGTZ_BROAD_CH_CTRL_ADD) {
			char text[100];
			sprintf(text, "Skipping free write at 0x%x. This register is the Control Register, "
						  "please use Bit Set (0x%x) or Bit Clear (0x%x) Registers to alter bits in the Control Register",
						  CAEN_DGTZ_BROAD_CH_CTRL_ADD, CAEN_DGTZ_BROAD_CH_CONFIGBIT_SET_ADD, CAEN_DGTZ_BROAD_CH_CLEAR_CTRL_ADD);
			guiMsg(MSG_ERROR, text);
			userMsg(text);
			continue;
		}
		if (CAEN_DGTZ_WriteRegister(dig->boardHandle, dig->freeWrites[i].address, dig->freeWrites[i].data)) ee = FREEWRT;
	}
    
    // Enable Fake Events for TimeTag Rollover (used for low rates applications)
    for(i=0;i<MAX_CH;i++) {
        if (dig->enableMask & (1<<i)) {
            uint32_t val;
            uint32_t addr = PHA_FAKEEVTS_ROLLOVER_REG | (i<<8);
            uint32_t bitMask = 0x04000000; // bit 26
            if (CAEN_DGTZ_ReadRegister(dig->boardHandle, addr, &val)) {
                ee = FREEWRT;
                break;
            }
            val |= bitMask;
            if (CAEN_DGTZ_WriteRegister(dig->boardHandle, addr, val)) {
                ee = FREEWRT;
                break;
            }
        }
    }

    // Manage free writes from file
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

    // Manage broadcast writes from file
    getBroadcastWritesFromFile(FWFilename, GWArray, &FWNum);
    for(i=0; i<FWNum; i++) {
        int ch;
        for(ch=0; ch<dig->numChannels; ch++) {
            if (dig->enableMask & (1<<ch)) {
                int32_t r;
                uint32_t val;
                uint32_t addr = GWArray[i].address | ch<<8;

                r = CAEN_DGTZ_ReadRegister(dig->boardHandle, addr, &val);
                val = (val & ~GWArray[i].mask ) | (GWArray[i].data & GWArray[i].mask);
                r |= CAEN_DGTZ_WriteRegister(dig->boardHandle, addr, val);

                if(r)
                    ee = FREEWRT;
            }
        }
    }

    if (ee!=NOERR) {
		char text[100];
		sprintf(text,"Warning: errors found while programming the digitizer. Setting %d may not be executed",ee);
		guiMsg(MSG_ERROR,text);
		userMsg("Warning: errors found while programming the digitizer.\nSetting %d may not be executed\n",ee);
	}

    return 0;
}





Digitizer* newDigitizer() {
	Digitizer* dig = (Digitizer*) calloc(1,sizeof(Digitizer));
	if (dig == NULL) return NULL;
	dig->boardInfo = (CAEN_DGTZ_BoardInfo_t*) calloc(1,sizeof(CAEN_DGTZ_BoardInfo_t));
    dig->resetEnabled = 0; // Reset disabled by default (for x780 protection)
    dig->restartingEnabled = 1; // HACK Disable restarting mode at startup
    dig->nEvtAggr = 0;
    dig->gatedStartEnabled = 0;
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
	int ret; 
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
		if (majorNumber != 128) {
			userMsg("This digitizer doesn't have a DPP-TF firmware (major %d)\n",majorNumber);
			return ERR_INVALID_BOARD_TYPE;
		}
		
		//dig->numChannels = ((dig->boardInfo->FormFactor == CAEN_DGTZ_VME64_FORM_FACTOR) || (dig->boardInfo->FormFactor == CAEN_DGTZ_VME64X_FORM_FACTOR)) ? 8 : 4; // KACK gestire caso con 2 ch.
        dig->numChannels = dig->boardInfo->Channels;
		dig->enableMask = (1<<dig->numChannels)-1;

		dig->boardOpen = 1;
		guiMsg(MSG_BOARD_INITED);

		guiMsg(MSG_NUM_CHANNELS,dig->numChannels);

        switch(dig->boardInfo->FamilyCode) {
        case CAEN_DGTZ_XX724_FAMILY_CODE:
            strcpy(dgtzFamily, "x724");
            break;
        case CAEN_DGTZ_XX780_FAMILY_CODE:
            strcpy(dgtzFamily, "x780");
            break;
        default:
            strcpy(dgtzFamily, "None");
            break;
        }
        _getFWRel(dig->boardInfo, &FW_MAJ, &FW_MIN);
        if(FW_MIN<PHA_PROBES_VER_2_AMC_FW_REL-1)
            sprintf(amcver, "AMCREL%d", 0);
        else
            sprintf(amcver, "AMCREL%d", PHA_PROBES_VER_2_AMC_FW_REL);

        guiMsg(MSG_DGTZ_FAMILY,dgtzFamily, amcver); // send the model family to the GUI

#ifdef DPP_TF_CORE
		libMsg(MSG_BOARD_INITED);
#endif
		checkLicense(dig);
	}

    ret = programDigitizer(dig);
	if (ret) return ret;
    ret = CAEN_DGTZ_MallocReadoutBuffer(dig->boardHandle, &dig->readoutBuffer, &dig->bufferSize); /* WARNING: This malloc must be done after the digitizer programming */

	ret |= CAEN_DGTZ_MallocDPPEvents(dig->boardHandle, (void **)dig->events, &size); 
    /* Allocate memory for the waveforms */
    ret |= CAEN_DGTZ_MallocDPPWaveforms(dig->boardHandle, (void **)&dig->waveforms, &size); 
	
	if (ret) return ERR_MALLOC;
	
	dig->runMask = dig->enableMask;
	dig->prevRateTime = getTime();

	if (dig->acqStatus != ACQSTATUS_STOPPED) {
		// stopAcquisition(dig); /* HACK REMOVED because the stop is always done in deinitDigitizer */
		startAcquisition(dig); /* to start acquiring data again upon restart */
	}
	dig->signalRestart = 0;

	return ERR_NONE;
}


void deinitDigitizer(Digitizer* dig, int close) {
	CAEN_DGTZ_FreeReadoutBuffer(&dig->readoutBuffer);
	CAEN_DGTZ_FreeDPPEvents(dig->boardHandle, (void **)dig->events);
	CAEN_DGTZ_FreeDPPWaveforms(dig->boardHandle,dig->waveforms);
    stopAcquisition(dig);
	if (close) {
		CAEN_DGTZ_CloseDigitizer(dig->boardHandle);
		dig->boardOpen = 0;
	}
}



int doAcquisition(Digitizer* dig) {
	int ret;
	//uint32_t numEvents = 0, ev = 0;

	if (dig->acqStatus == ACQSTATUS_STOPPED) return 0;
	

	if (dig->continuousTrigger && dig->acquisitionMode != CAEN_DGTZ_DPP_ACQ_MODE_List) CAEN_DGTZ_SendSWtrigger(dig->boardHandle);

	//if (dig->continuousTrigger) CAEN_DGTZ_SendSWtrigger(dig->boardHandle);

    /* Read data from the board */

    ret = CAEN_DGTZ_ReadData(dig->boardHandle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, dig->readoutBuffer, &dig->bufferOccupancy);
	if (ret) return ret;

	if (dig->bufferOccupancy > 0) {
		ret = CAEN_DGTZ_GetDPPEvents(dig->boardHandle, dig->readoutBuffer, dig->bufferOccupancy, (void **)dig->events, dig->numEvents);
		if (ret) return ret;
	}

	return 0;
}


int getNumEvents(Digitizer* dig, uint32_t* numEvents, int ch) {
	if (dig->bufferOccupancy > 0) *numEvents = dig->numEvents[ch];
	else *numEvents = 0;
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
            d32 |= PHA_GATEDSTART_MASK;
        else
            d32 &= ~PHA_GATEDSTART_MASK;
        CAEN_DGTZ_WriteRegister(dig->boardHandle, CAEN_DGTZ_ACQ_CONTROL_ADD, d32);

		//dig->signalTimeReset = 1;
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
            if((ret = SENDFLUSH_PHA(dig->boardHandle))!=0) // Sent flush command
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
		userMsg("Single Software Trigger issued\n");
        return CAEN_DGTZ_SendSWtrigger(dig->boardHandle);
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


int getDecodedWaveforms(Digitizer* dig, CAEN_DGTZ_DPP_PHA_Event_t* event, void* eventData[4], int* dataSize) {
	int res;
	if ( (res = CAEN_DGTZ_DecodeDPPWaveforms(dig->boardHandle, event, dig->waveforms)) != 0) return res;
	*dataSize = dig->waveforms->Ns;
	eventData[0] = dig->waveforms->Trace1;
	eventData[1] = dig->waveforms->Trace2;
	eventData[2] = dig->waveforms->DTrace1;
	eventData[3] = dig->waveforms->DTrace2;
	return 0;
}


#endif