/******************************************************************************
* This file contains the definition of the messages dpprunner can send to the
* GUI via the guiMsg() method
******************************************************************************/


#include <stdarg.h>
#include <stdio.h>

#include "messages.h"
#include "keyb.h"
#include "Controller.h"
#include "Digitizer.h"
#include "HistoMem.h"


char MsgStrings[_MSG_COUNT_][100] = {
	"R",
	"!s",
	"!P",
	"rr %.2f",
	"tr %d %.2f %.2f %.2f",
	"f %c %d %d %f %f %f",
	"!f",
	"E %s",
	"W %s",
	"nc %d",
	"bi",
    "dm %s %s",
    "!D"
};

void guiMsg(int msgCode, ...) {
#ifndef DPP_TF_CORE
	if (!stdinterm()) {
		va_list args;
		printf("$");
		va_start(args,msgCode);
		vprintf(MsgStrings[msgCode],args);
		va_end(args);
		printf("\n");
	}
#endif
}

void guiMsgFmt(const char* format, ...) { /* send a message to the GUI à la printf */
	if (!stdinterm()) {
		va_list args;
		printf("$");
		va_start(args,format);
		vprintf(format,args);
		va_end(args);
		printf("\n");
	}
}


void commandAck() {
#ifndef DPP_TF_CORE
	if (!stdinterm()) printf("%%\n");
#endif
}


void userMsg(const char* fmt, ...) {
#ifndef DPP_TF_CORE	
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
#endif
}



typedef struct RspHdr_t {
	const char tag;
	int ret, len;
} RspHdr;


static void writeRspHdr(int ret, int datalen) {
	char tag = '&';
	fwrite(&tag, 1, 1, stdout);
	fwrite(&ret, sizeof ret, 1, stdout);
	fwrite(&datalen, sizeof datalen, 1, stdout);
}

static void writeRspData(const void* data, int size, int count) {
	fwrite(data, size, count, stdout);
}


void libMsg(int code) {
	writeRspHdr(code, 0);
}


static int getParamInt() {
	int p;
    int tmp_scnfres; // to avoid compiler warnings
	tmp_scnfres = scanf("%d",&p);
	return p;
}

static double getParamDouble() {
	double p;
    int tmp_scnfres; // to avoid compiler warnings
    tmp_scnfres = scanf("%lf",&p);
	return p;
}

static char getParamChar() {
	char p;
    int tmp_scnfres; // to avoid compiler warnings
	tmp_scnfres = scanf("%c",&p);
	return p;
}


ACTION(aAcqStart) {
	ACTION_GET_DIG()->signalTimeReset = 1;
	writeRspHdr(startAcquisition(ACTION_GET_DIG()), 0);
}

ACTION(aAcqStop) {
	writeRspHdr(stopAcquisition(ACTION_GET_DIG()), 0);
}

ACTION(aAcqStatus) {
	writeRspHdr(0, sizeof (int));
	writeRspData(&ACTION_GET_DIG()->acqStatus, sizeof (int), 1);
}

ACTION(aChanMask) {
	writeRspHdr(0, sizeof (uint32_t));
	writeRspData(&ACTION_GET_DIG()->runMask, sizeof (uint32_t), 1);
}

ACTION(aMultiHisto) {
	Analyzer* ana = ACTION_GET_ANA();
	int max  = getParamInt();
	int len  = getParamInt();
	int cond = getParamInt();
	int ch;
	for (ch=0; ch < MAX_CH; ch++)
		setSize(ana->histoMems[ch], max, len);
	ana->histoAutoSwitch = cond;
	ana->histoMemHistoSize = len;
	writeRspHdr(0, 0);
}

ACTION(aAvailHisto) {
	int ch = getParamInt();
	int cnt;
	if (ch >= 0) { 
		cnt = getCount(ACTION_GET_ANA()->histoMems[ch])+1;
		writeRspHdr(0, sizeof cnt);
		writeRspData(&cnt, sizeof cnt, 1); 
	} else {
		writeRspHdr(0, ACTION_GET_DIG()->numChannels * sizeof cnt);
		for (ch=0; ch < ACTION_GET_DIG()->numChannels; ch++) {
			cnt = getCount(ACTION_GET_ANA()->histoMems[ch])+1;
			writeRspData(&cnt, sizeof cnt, 1);
		}
	}
}

ACTION(aNewHisto) {
	int ch = getParamInt();
	ACTION_GET_ANA()->forceHistoSwitch[ch] = 1;
	writeRspHdr(0, 0);
}

ACTION(aGetHisto) {
	Analyzer* ana = ACTION_GET_ANA();
	int ch = getParamInt();
	int index = getParamInt();
	uint32_t* histo = (uint32_t*)calloc(ana->histoMemHistoSize, sizeof(uint32_t));
	uint64_t counts, rt, dt;
	if (index == getCount(ana->histoMems[ch])) {
		getEHistoCurrent(ana, ch, histo, &counts, &rt, &dt);
	} else {
		getEHistoMem(ana, ch, index, histo, &counts, &rt, &dt);
	}
	writeRspHdr(0, ana->histoMemHistoSize*sizeof(uint32_t) + 3*sizeof(uint64_t));
	writeRspData(&counts, sizeof(uint64_t), 1);
	writeRspData(&rt,     sizeof(uint64_t), 1);
	writeRspData(&dt,     sizeof(uint64_t), 1);
	writeRspData(histo,   sizeof(uint32_t), ana->histoMemHistoSize);
	free(histo);
}


ACTION(aClearHistoMsg) {
	Analyzer* ana = ACTION_GET_ANA();
	int ch = getParamInt();
	int index = getParamInt();
	if (index < 0) {
		clearAllHistos(ana->histoMems[ch]);
		writeRspHdr(0, 0);
	} else if (index == getCount(ana->histoMems[ch])) {
		memset(ana->ehisto[ch], 0, (uint32_t)(0x1<<(ana->ehistoNbit+1)) * sizeof(uint32_t));
		ana->ecnt[ch] = 0;
		writeRspHdr(0, 0);
	} else {
		int ret = clearHisto(ana->histoMems[ch], index);
		writeRspHdr(ret, 0);
	}
}

ACTION(aFreeHisto) {
	Analyzer* ana = ACTION_GET_ANA();
	int ch = getParamInt();
	int index = getParamInt();
	if (index < 0) {
		removeAllHistos(ana->histoMems[ch]);
		writeRspHdr(0, 0);
	} else if (index == getCount(ana->histoMems[ch])) {
		writeRspHdr(-1, 0); /* can't free the current histo */
	} else {
		int ret = removeHisto(ana->histoMems[ch], index);
		writeRspHdr(ret, 0);
	}
}


ACTION(aGetInfo) {
	int s = 0;
	//Digitizer* dig = ACTION_GET_DIG();
	CAEN_DGTZ_BoardInfo_t* binfo = ACTION_GET_DIG()->boardInfo;
	writeRspHdr(0, 68);
	writeRspData(binfo->ModelName,       1, 12);
	writeRspData(&binfo->Model,          4, 1);
	writeRspData(&binfo->Channels,       4, 1);
	writeRspData(binfo->ROC_FirmwareRel, 1, 20);
	writeRspData(binfo->AMC_FirmwareRel, 1, 20);
	writeRspData(&binfo->SerialNumber,   4, 1);
	writeRspData(&s,				     4, 1);
}


ACTION(aSetUld) {
	Analyzer* ana = ACTION_GET_ANA();
	int ch = getParamInt();
	ana->histoMemLowFilter[ch] = getParamInt();
	ana->histoMemHighFilter[ch] = getParamInt();
	writeRspHdr(0, 0);
}

ACTION(aGetUld) {
	Analyzer* ana = ACTION_GET_ANA();
	int ch = getParamInt();
	writeRspHdr(0, sizeof(int)*2);
	writeRspData(&ana->histoMemLowFilter[ch], sizeof(int), 1);
	writeRspData(&ana->histoMemHighFilter[ch], sizeof(int), 1);
}

//
//ACTION(aEnableSwTrg) {
//	ACTION_GET_DIG()->continuousTrigger = 1;
//	writeRspHdr(0, 0);
//}
//
//ACTION(aDisableSwTrg) {
//	ACTION_GET_DIG()->continuousTrigger = 0;
//	writeRspHdr(0, 0);
//}

ACTION(aQuitCore) {
	ACTION_QUIT();
}


MESSAGES (
	MESSAGE(aAcqStart,   "start-acq");
	MESSAGE(aAcqStop,    "stop-acq" );
	MESSAGE(aAcqStatus,  "acq-status");
	MESSAGE(aChanMask,   "channel-mask");
	MESSAGE(aMultiHisto, "multi-histo");
	MESSAGE(aAvailHisto, "avail-histos");
	MESSAGE(aNewHisto,   "new-histo");
	MESSAGE(aGetHisto,   "get-histo");
	MESSAGE(aClearHistoMsg, "clear-histo");
	MESSAGE(aFreeHisto,  "free-histo");
	MESSAGE(aGetInfo,    "get-info");
	MESSAGE(aSetUld,     "set-uld");
	MESSAGE(aGetUld,     "get-uld");
	//MESSAGE(aEnableSwTrg,"enable-sw-trg");
	//MESSAGE(aDisableSwTrg,"disable-sw-trg");
	MESSAGE(aQuitCore,   "quit");
)