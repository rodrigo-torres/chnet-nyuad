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

#include "Digitizer.h"

int _getFWRel(CAEN_DGTZ_BoardInfo_t *BInfo, int *Maj, int *Min) {
    char str[40];

    sscanf(BInfo->AMC_FirmwareRel, "%d.%d%s", Maj, Min, str);
    return CAEN_DGTZ_Success;
}


#ifdef DPP_TF_OLD

#include <stdio.h>
#include <stdlib.h>

#include "globs.h"
#include "messages.h"

#define LICENSE_REG 0x8158
#define LICENSE_COUNTDOWN_MULT 84

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
    int i, ret = 0, ee = -1;
    /* reset the digitizer */
	if (CAEN_DGTZ_Reset(dig->boardHandle)) ee = 0;

    /* execute generic write commands */
    for(i=0; i<dig->numFreeWrites; i++)
		if (CAEN_DGTZ_WriteRegister(dig->boardHandle, dig->freeWrites[i].address, dig->freeWrites[i].data)) ee = 1;

    if (CAEN_DGTZ_SetRecordLength(dig->boardHandle, dig->recordLength)) ee = 2;
    if (CAEN_DGTZ_SetPostTriggerSize(dig->boardHandle, dig->postTrigger)) ee = 3; 
    if (CAEN_DGTZ_SetIOLevel(dig->boardHandle, dig->fpioType)) ee = 4;
    if (CAEN_DGTZ_SetMaxNumEventsBLT(dig->boardHandle, dig->maxNumEvents)) ee = 5;
    if (CAEN_DGTZ_SetAcquisitionMode(dig->boardHandle, CAEN_DGTZ_SW_CONTROLLED)) ee = 6;
    if (CAEN_DGTZ_SetExtTriggerInputMode(dig->boardHandle, dig->extTriggerMode)) ee = 7;
	if (CAEN_DGTZ_SetChannelEnableMask(dig->boardHandle, dig->enableMask)) ee = 8;

    for(i=0; i<dig->numChannels; i++) {
		dig->tsampl[i] = 10 * (1<<dig->decimation[i]);
		if (dig->enableMask & (1<<i)) {
            uint32_t shf, k, m, M, b, ftd, a, thr, nspk, nsbl, decim, trgho, twwdt, otrej, tpet, decimgain=0, DisablePUR=0;
            uint32_t d32;
			float dgain;

            if (CAEN_DGTZ_SetChannelDCOffset(dig->boardHandle, i, dig->dcOffset[i])) ee = 9;
			if (CAEN_DGTZ_SetChannelSelfTrigger(dig->boardHandle, CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT, (1<<i))) ee = 10;

            k = (dig->dppParams[i].k / dig->tsampl[i]);
            M = (dig->dppParams[i].M / dig->tsampl[i]);
            a = (dig->dppParams[i].a);
            b = (dig->dppParams[i].b / dig->tsampl[i]);
            m = dig->dppParams[i].m / dig->tsampl[i];
            ftd = ((dig->dppParams[i].ftd / dig->tsampl[i]) ) & 0x1FFF;
            nsbl = dig->dppParams[i].nsbl & 0x3;
            nspk = dig->dppParams[i].nspk & 0x3;
            thr = dig->dppParams[i].thr & 0x3FFF;
            decim = dig->decimation[i] & 0x3;
            trgho = (dig->dppParams[i].trgho / (4 * dig->tsampl[i])) & 0x3F;
            if (dig->dppParams[i].trgwin)   twwdt = (dig->dppParams[i].twwdt / dig->tsampl[i]) & 0x3FF;
            else  twwdt = 0;
            otrej = dig->dppParams[i].otrej & 1;
            tpet = (dig->dppParams[i].tpet / (8 * dig->tsampl[i])) & 0xF;
			/*
            //decimgain = 0;
            //dig->ediv = (k * M / dig->dppParams[i].dgain);  // HACK: inserire anche decimgain
            //for (shf=0; shf<32; shf++) {
            //    if ((1<<(shf+1)) >= (int)dig->ediv) break;
            //}*/

			decimgain = 1 << decim;
			dgain = dig->dppParams[i].dgain;
			if (decimgain > 1 && dgain < decimgain) { /* look for the highest possible contribution the decimgain can give towards reaching the dgain */
				for (decimgain>>=1; decimgain > 1; decimgain>>=1) {
					if (dgain >= decimgain) break;
				}
			}

			dig->ediv = k * M * decimgain / dgain; /* ediv now correctly takes into account decimgain */
            for (shf=0; shf<32; shf++) {
                if ((1<<(shf+1)) >= (int)dig->ediv) break;
            }

            d32 = ((b + a) & 0xFF) | ((a & 0x3F) << 8) | ((M & 0xFFFF) << 16) | ((DisablePUR & 1) << 14);
            CAEN_DGTZ_WriteRegister(dig->boardHandle, 0x1024+(0x100*i), d32);
            d32 = (k & 0x3FF) | ((m & 0x3FF) << 12) | ((shf & 0x3F) << 24) | (nsbl << 30);  
            CAEN_DGTZ_WriteRegister(dig->boardHandle, 0x1028+(0x100*i), d32);
            d32 = (thr & 0x3FFF) | (decim << 14) | (ftd << 16) | (nspk << 28) | (decimgain << 30);
            CAEN_DGTZ_WriteRegister(dig->boardHandle, 0x102C+(0x100*i), d32);
            d32 = twwdt | (trgho << 12) | (tpet << 16) | (otrej << 30);
            CAEN_DGTZ_WriteRegister(dig->boardHandle, 0x1030+(0x100*i), d32);
/*			ret |= CAEN_DGTZ_SetDPPParameter(handle, i, CAEN_DGTZ_DPP_Param_k, k);
			ret |= CAEN_DGTZ_SetDPPParameter(handle, i, CAEN_DGTZ_DPP_Param_m, m);
			ret |= CAEN_DGTZ_SetDPPParameter(handle, i, CAEN_DGTZ_DPP_Param_M, M);
			ret |= CAEN_DGTZ_SetDPPParameter(handle, i, CAEN_DGTZ_DPP_Param_a, Dppr.DPPparams[i].a);
			ret |= CAEN_DGTZ_SetDPPParameter(handle, i, CAEN_DGTZ_DPP_Param_b, b);
			ret |= CAEN_DGTZ_SetDPPParameter(handle, i, CAEN_DGTZ_DPP_Param_Decimation, Dppr.Decimation);
			ret |= CAEN_DGTZ_SetDPPParameter(handle, i, CAEN_DGTZ_DPP_Param_NSBaseline, Dppr.DPPparams[i].nsbl);
			ret |= CAEN_DGTZ_SetDPPParameter(handle, i, CAEN_DGTZ_DPP_Param_shf, shf);
			ret |= CAEN_DGTZ_SetDPPParameter(handle, i, CAEN_DGTZ_DPP_Param_NSPeakMean, Dppr.DPPparams[i].nspk);
			ret |= CAEN_DGTZ_SetDPPParameter(handle, i, CAEN_DGTZ_DPP_Param_FlatTopDelay, ftd);
			ret |= CAEN_DGTZ_SetDPPParameter(handle, i, CAEN_DGTZ_DPP_Param_TrgThreshold, Dppr.DPPparams[i].thr);
            //ret |= CAEN_DGTZ_SetDPPParameter(handle, i, CAEN_DGTZ_DPP_Param_TrgWinOffset, 0); //Dppr.DPPparams[i].trgho);
			//ret |= CAEN_DGTZ_SetDPPParameter(handle, i, CAEN_DGTZ_DPP_Param_TrgWinWidth, 0); //Dppr.DPPparams[i].twwdt); */
            if (dig->pulsePolarity == 0)
                if (CAEN_DGTZ_SetDPPParameter(dig->boardHandle, i, CAEN_DGTZ_DPP_Param_InvertInput, 0)) ee = 11;
            else
                if (CAEN_DGTZ_SetDPPParameter(dig->boardHandle, i, CAEN_DGTZ_DPP_Param_InvertInput, 1)) ee = 11;

		}
	}


	if (CAEN_DGTZ_SetDPPAcquisitionMode(dig->boardHandle, dig->acquisitionMode, dig->listParams)) ee = 12;

    
    if (ee!=-1) {
		char text[100];
		sprintf(text,"Warning: errors found while programming of the digitizer. Setting %d may not be executed",ee);
		guiMsg(MSG_ERROR,text);
		userMsg("Warning: errors found while programming of the digitizer.\nSetting %d may not be executed\n",ee);
	}

    return 0;
}





Digitizer* newDigitizer() {
	Digitizer* dig = (Digitizer*) calloc(1,sizeof(Digitizer));
	if (dig == NULL) return NULL;
	dig->boardInfo = (CAEN_DGTZ_BoardInfo_t*) calloc(1,sizeof(CAEN_DGTZ_BoardInfo_t));
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
	if (!dig->boardOpen || reopen) {
		ret = CAEN_DGTZ_OpenDigitizer(dig->linkType, dig->linkNum, dig->conetNode, dig->baseAddress, &dig->boardHandle);
		if (ret) return ERR_DGZ_OPEN;
	
		ret = CAEN_DGTZ_GetInfo(dig->boardHandle, dig->boardInfo);
		if (ret) return ERR_BOARD_INFO_READ;

		userMsg("Connected to CAEN Digitizer Model %s\n", dig->boardInfo->ModelName);
		userMsg("ROC FPGA Release is %s\n", dig->boardInfo->ROC_FirmwareRel);
		userMsg("AMC FPGA Release is %s\n", dig->boardInfo->AMC_FirmwareRel);

		/* Check firmware rivision (DPP firmwares cannot be used with DPPrunner */
		sscanf(dig->boardInfo->AMC_FirmwareRel, "%d", &majorNumber);
		if (majorNumber != 128) {
			userMsg("This digitizer doesn't have a DPP-TF firmware (major %d)\n",majorNumber);
			return ERR_INVALID_BOARD_TYPE;
		}
	
		dig->numChannels = ((dig->boardInfo->FormFactor == CAEN_DGTZ_VME64_FORM_FACTOR) || (dig->boardInfo->FormFactor == CAEN_DGTZ_VME64X_FORM_FACTOR)) ? 8 : 4; // KACK gestire caso con 2 ch.
		dig->enableMask &= (1<<dig->numChannels)-1;

		dig->boardOpen = 1;
		guiMsg(MSG_BOARD_INITED);

		guiMsg(MSG_NUM_CHANNELS,dig->numChannels);

		checkLicense(dig);
	}
	ret = programDigitizer(dig);
	if (ret) return ret;
    ret = CAEN_DGTZ_MallocReadoutBuffer(dig->boardHandle, &dig->readoutBuffer, &dig->bufferSize); /* WARNING: This malloc must be done after the digitizer programming */
	
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
	if (close) {
		stopAcquisition(dig);
		CAEN_DGTZ_CloseDigitizer(dig->boardHandle);
		dig->boardOpen = 0;
	}
}



int doAcquisition(Digitizer* dig) {
	int ret;
	uint32_t numEvents = 0, ev = 0;

	if (dig->acqStatus == ACQSTATUS_STOPPED) return 0;
	
	if (dig->continuousTrigger && 
		(dig->acquisitionMode == CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope ||
		 dig->acquisitionMode == CAEN_DGTZ_DPP_ACQ_MODE_Mixed)) CAEN_DGTZ_SendSWtrigger(dig->boardHandle);

    /* Read data from the board */

    ret = CAEN_DGTZ_ReadData(dig->boardHandle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, dig->readoutBuffer, &dig->bufferOccupancy);
	if (ret) return ret;

	if (dig->runMask == 0) { /* time's up*/
	    userMsg("Time is up.\n");
		guiMsg(MSG_STOP_ACQ);
		stopAcquisition(dig);
	}
	return 0;
}


int getNumEvents(Digitizer* dig, uint32_t* numEvents) {
	int ret;
	if (dig->bufferOccupancy != 0) { 
        ret = CAEN_DGTZ_GetNumEvents(dig->boardHandle, dig->readoutBuffer, dig->bufferOccupancy, numEvents);
		if (ret) return ret;
	}
	return 0;
}

int getEvent(Digitizer* dig, CAEN_DGTZ_V1724DPP_TF_EVENT_t** event, CAEN_DGTZ_EventInfo_t** eventInfo, int ev) {
	int ret;
	char* eventPtr;
	*eventInfo = (CAEN_DGTZ_EventInfo_t*) calloc(1,sizeof(CAEN_DGTZ_EventInfo_t));
	/* Get one event from the readout buffer */
	ret = CAEN_DGTZ_GetEventInfo(dig->boardHandle, dig->readoutBuffer, dig->bufferOccupancy, ev, *eventInfo, &eventPtr);
	if (ret) return ERR_EVENT_BUILD;
	/* decode the event */
	ret = CAEN_DGTZ_DecodeEvent(dig->boardHandle, eventPtr, event);
	if (ret) return ERR_EVENT_BUILD;
	return 0;
}

void freeEvent(Digitizer* dig, CAEN_DGTZ_V1724DPP_TF_EVENT_t** event, CAEN_DGTZ_EventInfo_t** eventInfo) {
	free(*eventInfo);
	CAEN_DGTZ_FreeEvent(dig->boardHandle, event); 
}


int startAcquisition(Digitizer* dig) {
	if (dig->acqStatus == ACQSTATUS_STOPPED) {
		dig->acqStatus = ACQSTATUS_RUNNING;
		userMsg("Acquisition started\n");
		dig->runMask = dig->enableMask;
		dig->signalTimeReset = 1;
		return CAEN_DGTZ_SWStartAcquisition(dig->boardHandle);
	} else return 0;
}

int stopAcquisition(Digitizer* dig) {
	if (dig->acqStatus != ACQSTATUS_STOPPED) {
		dig->acqStatus = ACQSTATUS_STOPPED; // HACK manage flush
		userMsg("Acquisition stopped\n");
		return CAEN_DGTZ_SWStopAcquisition(dig->boardHandle);
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


#endif