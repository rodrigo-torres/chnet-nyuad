/******************************************************************************
* 
* CAEN SpA - Computing Division
* Via Vetraia, 11 - 55049 - Viareggio ITALY
* + 390594388398 - www.caen.it
*
***************************************************************************//**
* \note TERMS OF USE:
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software
* Foundation. This program is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. The user relies on the 
* software, documentation and results solely at his own risk.
*
* \file     Functions.c
* \brief    CAEN - Utility's source of Routines used in CAENDPPLibrary Demo
* \author   Francesco Pepe (support.computing@caen.it)
*
******************************************************************************/

#include <Functions.h> 
#include <keyb.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <time.h> 
#include <limits.h>
#include <string.h>
#include <stdarg.h>

// Definition of the commands  chain
// connected to specific chars
typedef struct timedCmd_t {
    char c;
    uint64_t timeOutMs;
    struct timedCmd_t *next;
    struct timedCmd_t *prev;
    uint64_t startTime;
} timedCmd_t;

static timedCmd_t *WNC_COMMANDS = NULL;

/// \fn      uint64_t getTimeMs();
/// \brief   Returns the current time in ms
/// 
/// \return  the time in ms
uint64_t getTimeMs() {
    uint64_t res;
#ifdef LINUX
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);
    res = (uint64_t)(start.tv_sec * 1000) + (uint64_t)(start.tv_nsec * 0.000001);
#else
    res = (uint64_t)GetTickCount64();
#endif
    return res;
}

/// \fn      void setNextCharTimeout(size_t ms, char c);
/// \brief   Makes the function 'WaitNextChar()' to throw the char defined in 'c'
///          after the Timeout is reached. if ms <= 0 no timeout is set. This
///          feature can be used to perform some kind of automation: for example,
///          the Continuous plot is implemented by "throwing" the single plot char
///          once every plot period. The HV monitoring is implemented in the same
///          way using a different char
void setNextCharTimeout(uint64_t ms, char c) {
    timedCmd_t *TCmd = WNC_COMMANDS;
    timedCmd_t *prev = NULL;
    int32_t init = 0;
    
    // Look for an existing cmd which corresponds to the given char
    while (TCmd != NULL) {
        prev = TCmd;
        if (TCmd->c != c)
            TCmd = TCmd->next;
        else
            break;
    }

    // If no cmd is found, allocate a new one
    if (TCmd == NULL) {
        TCmd = (timedCmd_t *)calloc(1, sizeof(timedCmd_t));
        if (TCmd == NULL)
            return;
        TCmd->c = c;
        TCmd->startTime = getTimeMs();
    }

    // Set cmd values
    TCmd->timeOutMs = ms;

    // Insert it in the right position
    if (prev != TCmd) {
        TCmd->prev = prev;
        if (prev != NULL)
            prev->next = TCmd;
    }
    if (prev == NULL) // if 'prev' is null we need to initialize the chain
        WNC_COMMANDS = TCmd;
}

/// \fn      void getNextCharTimeout(char c, size_t *ms);
/// \brief   gets the values of Timeout and char previously set
///          with 'setNextCharTimeout' function.
void getNextCharTimeout(char c, size_t *ms) {
    timedCmd_t *TCmd = WNC_COMMANDS;
    
    while (TCmd != NULL) {
        if (TCmd->c == c)
            break;
        else
            TCmd = TCmd->next;
    }
    if (TCmd != NULL)
        *ms = TCmd->timeOutMs;
    else
        *ms = 0;
}

/// \fn      int32_t WaitNextChar();
/// \brief   Waits for the user to enter a character. This fuction can
///          automatically "trow" a character depending on the values configured
///          with 'setNextCharTimeout' fuction after the Timeout is reached.
//           If ms <= 0 no timeout is set.
int32_t WaitNextChar() {
    int32_t res = 0;
    while (!_kbhit()) {
        uint64_t CT;
        timedCmd_t *TCmd = WNC_COMMANDS;

        Sleep(50);
        CT = getTimeMs();
        while (TCmd != NULL) {
            if ((TCmd->timeOutMs > 0) && ((CT - TCmd->startTime) > TCmd->timeOutMs)) {
                timedCmd_t *prev = TCmd->prev, *next = TCmd->next;
                
                TCmd->startTime = CT;
                
                // Move this TCmd to the end of the chain (to be sure that it
                // doesn't happen always before the following ones).
                // First remove TCmd by changing pointers of prev/next
                if (prev != NULL)
                    prev->next = next;
                if (next != NULL)
                    next->prev = prev;
                
                // Then move TCmd to the end of the chain
                prev = WNC_COMMANDS;
                while (prev->next != NULL)
                    prev = prev->next;
                if (prev != TCmd) {
                    prev->next = TCmd;
                    TCmd->prev = prev;
                }
                TCmd->next = NULL;

                // Recalculate the first element if needed
                if (WNC_COMMANDS == TCmd) { // This happens if TCmd was the first in the chain
                    prev = TCmd;
                    while (prev->prev != NULL)
                        prev = prev->prev;
                    WNC_COMMANDS = prev;
                }

                // Finally return the corresponding char
                return TCmd->c;
            }
            TCmd = TCmd->next;
        }
    }
    while (_kbhit())
        res += _getch();
    return res;
}

/// \fn      int32_t isPow2(uint32_t val);
/// \brief   Gets if val is a power of 2
/// 
/// \param   [IN]  val     : The value to check
/// 
/// \return  0 = false; 1 = true
int32_t isPow2(uint32_t val) {
    int res = (val & (val >> 1)) == 0 ? 1 : 0;
    if (val == 0)
        res = 1;
    return res;
}

/// \fn      int32_t saveHistogram(char *filename, uint32_t *EHisto, int32_t nbins);
/// \brief   Saves a Histogram to file
/// 
/// \param   [IN]  filename     : The name of the file to write to
/// \param   [IN]  EHisto       : The Pointer to the Histogram Data
/// \param   [IN]  nbins        : The number of bins of the histogram
/// \param   [IN]  RT_ns        : The Real Time in ns (if 0 it is not dumped)
/// \param   [IN]  DT_ns        : The Dead Time in ns (if 0 it is not dumped)
/// 
/// \return  0 = Success; negative numbers are error codes
int32_t saveHistogram(char *filename, uint32_t *EHisto, int32_t nbins, uint64_t RT_ns, uint64_t DT_ns) {
    FILE *fh;
    int32_t i;
    long long unsigned int u64;
    fh = fopen(filename, "w");
    if (fh == NULL)
        return - 1;
    if (RT_ns > 0) {
        u64 = RT_ns;
        fprintf(fh, "RealTime %llu\n", u64);
    }
    if (DT_ns > 0) {
        u64 = DT_ns;
        fprintf(fh, "DeadTime %llu\n", u64);
    }
    for (i = 0; i < nbins; i++) {
        fprintf(fh, "%u\n", EHisto[i]);
    }
    fclose(fh);

    return 0;
}

/// \fn      int32_t loadHistogram(char *filename, uint32_t *EHisto, int32_t *nbins);
/// \brief   Loads a Histogram from file
/// 
/// \param   [IN]  filename     : The name of the file to write to
/// \param   [OUT] EHisto       : The Pointer to the Histogram Data
/// \param   [OUT] nbins        : The number of bins of the histogram
/// \param   [OUT] RT_ns        : The Real Time in ns (0 if missing)
/// \param   [OUT] DT_ns        : The Dead Time in ns (0 if missing)
/// 
/// \return  0 = Success; negative numbers are error codes
int32_t loadHistogram(char *filename, uint32_t *EHisto, int32_t *nbins, uint64_t *RT_ns, uint64_t *DT_ns) {
    FILE *fh;
    int32_t i;
    char buf[100];
    long long unsigned int u64;

    *RT_ns = 0;
    *DT_ns = 0;

    fh = fopen(filename, "r");
    if (fh == NULL)
        return - 1;
    i = 0;
    while (!feof(fh)) {
        if (fscanf(fh, "%s", buf) != 1) {
            if (feof(fh))
                continue;
            fclose(fh);
            return -2;
        }
        if (strcmp(buf, "RealTime") == 0) {
            if (fscanf(fh, "%llu", &u64) != 1) {
                fclose(fh);
                return -3;
            }
            *RT_ns = u64;
        }
        else if (strcmp(buf, "DeadTime") == 0) {
            if (fscanf(fh, "%llu", &u64) != 1) {
                fclose(fh);
                return -4;
            }
            *DT_ns = u64;
        }
        else {
            char *p = buf;
            char *end;
            while (*p == ' ' || *p == '\t') // Remove heading whitespaces and tabs
                p++;
            EHisto[i] = strtoul(p, &end, 0);
            if (EHisto[i] == LONG_MAX || end == p) // Error in strtol
                continue; // skip this read
            i++; // Otherwise go to next bin
        }
    }

    if (!isPow2(i) || i > MAX_HISTO_NBINS) {
        fclose(fh);
        return -5;
    }
    *nbins = i;

    fclose(fh);
    return 0;
}

/// \fn      int32_t saveWaveform(char *filename, int32_t nTraces, void **traces, DPPDemo_TraceType_t *types, int32_t ns);
/// \brief   Saves a Waveform to file
/// 
/// \param   [IN]  filename     : The name of the file to write to
/// \param   [IN]  data         : The Pointer to the Waveform Data
/// \param   [IN]  ns           : The number of samples of the waveform
/// 
/// \return  0 = Success; negative numbers are error codes
int32_t saveWaveform(char *filename, int32_t nTraces, void **traces, DPPDemo_TraceType_t *types, int32_t ns) {
    FILE *fh;
    int32_t i, t;
    char **pTraces = (char**)traces;

    if (nTraces > MAX_NUM_TRACES)
        return -1;

    fh = fopen(filename, "w");
    if (fh == NULL)
        return - 1;
    for (i = 0; i < ns; i++) {
        uint8_t u8;
        int16_t d16;

        fprintf(fh, "%d", i);
        for (t = 0; t < nTraces; t++) {
            switch (types[t]) {
            case DPPDemo_TraceType_Analog1:
                d16 = *(int16_t*)pTraces[t];
                fprintf(fh, " %hd", d16);
                pTraces[t] += sizeof(d16);
                break;
            case DPPDemo_TraceType_Analog2:
                d16 = *(int16_t*)pTraces[t];
                fprintf(fh, " %hd", d16);
                pTraces[t] += sizeof(d16);
                break;
            case DPPDemo_TraceType_Digital1:
                u8 = *(uint8_t*)pTraces[t];
                fprintf(fh, " %hhu", u8);
                pTraces[t] += sizeof(u8);
                break;
            case DPPDemo_TraceType_Digital2:
                u8 = *(uint8_t*)pTraces[t];
                fprintf(fh, " %hhu", u8);
                pTraces[t] += sizeof(u8);
                break;
            default: // Unsupported trace type
                return -2;
            }
        }
        fprintf(fh, "\n");
    }

    fclose(fh);

    return 0;
}

/// \fn      int32_t DefineAutosetParameters(CAENDPP_DgtzParams_t *Params);
/// \brief   Initialize the supported parameters to the value used for Autoset
/// 
/// \param   [OUT]  Params      : Pointer to the parameters structure to fill
/// 
/// \return  0 = Success; negative numbers are error codes
int32_t DefineAutosetParameters(CAENDPP_DgtzParams_t *Params) {
    int32_t ch;

    for (ch = 0; ch < MAX_BOARD_CHNUM; ch++) {
        // Channel Parameters
        Params->DCoffset[ch] = AUTOSET_VALUE; // DC Offset (DAC Offset)
        Params->PulsePolarity[ch] = (CAENDPP_PulsePolarity_t)(AUTOSET_VALUE); // Pulse Polarity
        
        // DPP - PHA Parameters
        Params->DPPParams.M[ch] = AUTOSET_VALUE; // Signal Decay Time Constant
        Params->DPPParams.m[ch] = AUTOSET_VALUE; // Trapezoid Flat Top
        Params->DPPParams.k[ch] = AUTOSET_VALUE; // Trapezoid Rise Time
        Params->DPPParams.ftd[ch] = AUTOSET_VALUE; // Flat Top Delay
        Params->DPPParams.a[ch] = AUTOSET_VALUE; // Trigger Filter smoothing factor
        Params->DPPParams.b[ch] = AUTOSET_VALUE; // Input Signal Rise time
        Params->DPPParams.thr[ch] = AUTOSET_VALUE; // Trigger Threshold
        Params->DPPParams.nsbl[ch] = AUTOSET_VALUE; // Number of Samples for Baseline Mean
        Params->DPPParams.nspk[ch] = AUTOSET_VALUE; // Number of Samples for Peak Mean Calculation
        Params->DPPParams.pkho[ch] = AUTOSET_VALUE; // Peak Hold Off
        Params->DPPParams.blho[ch] = AUTOSET_VALUE; // Base Line Hold Off
        Params->DPPParams.trgho[ch] = AUTOSET_VALUE; // Trigger Hold Off
        Params->DPPParams.dgain[ch] = AUTOSET_VALUE; // Digital Probe Gain
        Params->DPPParams.enf[ch] = AUTOSET_VALUE; // Energy Nomralization Factor
        Params->DPPParams.decimation[ch] = AUTOSET_VALUE; // Decimation of Input Signal
        Params->DPPParams.twwdt[ch] = AUTOSET_VALUE; // 
        Params->DPPParams.trgwin[ch] = AUTOSET_VALUE; //
    }
    return 0;
}

/// \fn      int32_t InitDgtzParams(CAENDPP_DgtzParams_t *Params);
/// \brief   Initialize the configuration parameters to their default value
/// 
/// \param   [OUT]  Params      : Pointer to the parameters structure to fill
/// 
/// \return  0 = Success; negative numbers are error codes
int32_t InitDgtzParams(CAENDPP_DgtzParams_t *Params) {
    int32_t ch;

    // set listMode default parameters
    Params->ListParams.enabled = FALSE;
    Params->ListParams.saveMode = CAENDPP_ListSaveMode_FileBinary;
    strcpy(Params->ListParams.fileName, "UNNAMED");
    Params->ListParams.maxBuffNumEvents = 0;
    Params->ListParams.saveMask = 0xF;

    // default board parameters
    Params->ChannelMask = 0; // it will be filled later
    Params->EventAggr = 0;
    Params->IOlev = CAENDPP_IOLevel_NIM;
    
    // Generic Writes to Registers
    Params->GWn = 0;                                    // Number of Generic Writes
    memset(Params->GWaddr, 0, MAX_GW*sizeof(uint32_t)); // List of addresses (length = 'GWn')
    memset(Params->GWdata, 0, MAX_GW*sizeof(uint32_t)); // List of datas (length = 'GWn')
    memset(Params->GWmask, 0, MAX_GW*sizeof(uint32_t)); // List of masks (length = 'GWn')

    // Waveform parameters default settings
    Params->WFParams.dualTraceMode = 1;
    Params->WFParams.vp1 = CAENDPP_PHA_VIRTUALPROBE1_Input;
    Params->WFParams.vp2 = CAENDPP_PHA_VIRTUALPROBE2_TrapBLCorr;
    Params->WFParams.dp1 = CAENDPP_PHA_DigitalProbe1_Armed;
    Params->WFParams.dp2 = CAENDPP_PHA_DigitalProbe2_Trigger;
    Params->WFParams.recordLength = (int32_t)(8192);
    Params->WFParams.preTrigger = (int32_t)(100);
    Params->WFParams.probeSelfTriggerVal = 150;
    Params->WFParams.probeTrigger = CAENDPP_PHA_PROBETRIGGER_MainTrig;
    
    // Channel parameters
    for (ch = 0; ch < MAX_BOARD_CHNUM; ch++) {
        // Channel parameters
        Params->DCoffset[ch] = 7864;
        Params->PulsePolarity[ch] = CAENDPP_PulsePolarityPositive;
        
        // Coicidence parameters between channels
        Params->CoincParams[ch].CoincChMask = 0x0;
        Params->CoincParams[ch].CoincLogic = CAENDPP_CoincLogic_None;
        Params->CoincParams[ch].CoincOp = CAENDPP_CoincOp_OR;
        Params->CoincParams[ch].MajLevel = 0;
        Params->CoincParams[ch].TrgWin = 0;

        // DPP Parameters
        Params->DPPParams.M[ch] = 47500;        // Signal Decay Time Constant
        Params->DPPParams.m[ch] = 5000;         // Trapezoid Flat Top
        Params->DPPParams.k[ch] = 5000;         // Trapezoid Rise Time
        Params->DPPParams.ftd[ch] = 2500;       // Flat Top Delay
        Params->DPPParams.a[ch] = 4;            // Trigger Filter smoothing factor
        Params->DPPParams.b[ch] = 200;          // Input Signal Rise time
        Params->DPPParams.thr[ch] = 1500;       // Trigger Threshold
        Params->DPPParams.nsbl[ch] = 3;         // Number of Samples for Baseline Mean
        Params->DPPParams.nspk[ch] = 0;         // Number of Samples for Peak Mean Calculation
        Params->DPPParams.pkho[ch] = 0;         // Peak Hold Off
        Params->DPPParams.blho[ch] = 1000;      // Base Line Hold Off
        Params->DPPParams.dgain[ch] = 0;        // Digital Probe Gain
        Params->DPPParams.enf[ch] = 1.0;        // Energy Nomralization Factor
        Params->DPPParams.decimation[ch] = 0;   // Decimation of Input Signal   
        Params->DPPParams.trgho[ch] = 100;      // Trigger Hold Off
        Params->DPPParams.twwdt[ch] = 0;        // 
        Params->DPPParams.trgwin[ch] = 0;       //

        // Parameters for X770
        Params->ChannelExtraParameters[ch].analogPath = 0;
		Params->ChannelExtraParameters[ch].CRgain = 0;
		Params->ChannelExtraParameters[ch].InputImpedance = 0;
		Params->ChannelExtraParameters[ch].PRDSgain = 0;
		Params->ChannelExtraParameters[ch].ResetDetector.EnableResetDetector = 0;
        Params->ChannelExtraParameters[ch].ResetDetector.thrhold = 100;
		Params->ChannelExtraParameters[ch].ResetDetector.reslenmin = 2;
		Params->ChannelExtraParameters[ch].ResetDetector.reslenpulse = 2000;
		Params->ChannelExtraParameters[ch].ResetDetector.thrhold = -10000;
		Params->ChannelExtraParameters[ch].SaturationHoldoff = 0;
		
        Params->DPPParams.X770_extraparameters[ch].decK2 = 0;
		Params->DPPParams.X770_extraparameters[ch].decK3 = 0;
		Params->DPPParams.X770_extraparameters[ch].deconvolutormode = 0;
		Params->DPPParams.X770_extraparameters[ch].energyFilterMode = 0;
		Params->DPPParams.X770_extraparameters[ch].PD_thrshld1 = 500;
		Params->DPPParams.X770_extraparameters[ch].PD_thrshld2 = 450;
		Params->DPPParams.X770_extraparameters[ch].PD_winlen = 1000;
		Params->DPPParams.X770_extraparameters[ch].trigK = 30;
		Params->DPPParams.X770_extraparameters[ch].trigm = 10;
		Params->DPPParams.X770_extraparameters[ch].trigMODE = 0;

        Params->SpectrumControl[ch].SpectrumMode = CAENDPP_SpectrumMode_Energy;
        Params->SpectrumControl[ch].TimeScale = 1;
    }
    return 0;
}

/// \fn      char *getTraceName(char *buf, int32_t type, int32_t tr);
/// \brief   Gets the Name of a Trace of a specific Type
/// 
/// \param   [OUT]  buf     : The string to be fille with the name
/// \param   [IN]   type    : the type of the trace, see enum 'DPPDemo_TraceType_t'
/// \param   [IN]   tr      : the trace definition
/// 
/// \return  a pointer to buf
char *getTraceName(char *buf, DPPDemo_TraceType_t type, int32_t tr) {
    switch (type) {
    case DPPDemo_TraceType_Analog1: // AT1
        switch ((CAENDPP_PHA_VirtualProbe1_t)tr) {
        case CAENDPP_PHA_VIRTUALPROBE1_Input: strcpy(buf, "Input"); break;
        case CAENDPP_PHA_VIRTUALPROBE1_Delta: strcpy(buf, "RC-CR"); break;
        case CAENDPP_PHA_VIRTUALPROBE1_Delta2: strcpy(buf, "RC-CR2"); break;
        case CAENDPP_PHA_VIRTUALPROBE1_Trapezoid: strcpy(buf, "Trapezoid"); break;
        case CAENDPP_PHA_VIRTUALPROBE1_FastTrap: strcpy(buf, "Fast Trapezoid"); break;
        case CAENDPP_PHA_VIRTUALPROBE1_TrapBaseline: strcpy(buf, "Baseline"); break;
        case CAENDPP_PHA_VIRTUALPROBE1_EnergyOut: strcpy(buf, "Energy Out"); break;
        case CAENDPP_PHA_VIRTUALPROBE1_TrapBLCorr: strcpy(buf, "Trap-BL"); break;
        default: strcpy(buf, "UNKNOWN"); break;
        }
        break;
    case DPPDemo_TraceType_Analog2: // AT2
        switch ((CAENDPP_PHA_VirtualProbe2_t)tr) {
        case CAENDPP_PHA_VIRTUALPROBE2_Input: strcpy(buf, "Input"); break;
        case CAENDPP_PHA_VIRTUALPROBE2_S3: strcpy(buf, "Threshold"); break;
        case CAENDPP_PHA_VIRTUALPROBE2_TrapBLCorr: strcpy(buf, "Trap-BL"); break;
        case CAENDPP_PHA_VIRTUALPROBE2_TrapBaseline: strcpy(buf, "Baseline"); break;
        case CAENDPP_PHA_VIRTUALPROBE2_None: strcpy(buf, "None"); break;
        case CAENDPP_PHA_VIRTUALPROBE2_Delta: strcpy(buf, "RC-CR"); break;
        case CAENDPP_PHA_VIRTUALPROBE2_FastTrap: strcpy(buf, "Fast Trapezoid"); break;
        case CAENDPP_PHA_VIRTUALPROBE2_Delta2: strcpy(buf, "RC-CR2"); break;
        case CAENDPP_PHA_VIRTUALPROBE2_Trapezoid: strcpy(buf, "Trapezoid"); break;
        case CAENDPP_PHA_VIRTUALPROBE2_EnergyOut: strcpy(buf, "Energy Out"); break;
        default: strcpy(buf, "UNKNOWN"); break;
        }
        break;
    case DPPDemo_TraceType_Digital1: // DT1
        switch ((CAENDPP_PHA_DigitalProbe1_t)tr) {
        case CAENDPP_PHA_DigitalProbe1_TrgWin: strcpy(buf, "Zero Cross Win."); break;
        case CAENDPP_PHA_DigitalProbe1_Armed: strcpy(buf, "Armed"); break;
        case CAENDPP_PHA_DigitalProbe1_PkRun: strcpy(buf, "Peak Run"); break;
        case CAENDPP_PHA_DigitalProbe1_PURFlag: strcpy(buf, "PileUp Flag"); break;
        case CAENDPP_PHA_DigitalProbe1_Peaking: strcpy(buf, "Peaking"); break;
        case CAENDPP_PHA_DigitalProbe1_TVAW: strcpy(buf, "TRG Validation Win."); break;
        case CAENDPP_PHA_DigitalProbe1_BLHoldoff: strcpy(buf, "Baseline Holdoff"); break;
        case CAENDPP_PHA_DigitalProbe1_TRGHoldoff: strcpy(buf, "TRG Holdoff"); break;
        case CAENDPP_PHA_DigitalProbe1_TRGVal: strcpy(buf, "TRG Hold Off"); break;
        case CAENDPP_PHA_DigitalProbe1_ACQVeto: strcpy(buf, "Saturation Veto"); break;
        case CAENDPP_PHA_DigitalProbe1_BFMVeto: strcpy(buf, "Baseline Flatness Veto"); break;
        case CAENDPP_PHA_DigitalProbe1_ExtTRG: strcpy(buf, "External TRG"); break;
        case CAENDPP_PHA_DigitalProbe1_Trigger: strcpy(buf, "Trigger"); break;
        default: strcpy(buf, "UNKNOWN"); break;
        }
        break;
    case DPPDemo_TraceType_Digital2:
        switch ((CAENDPP_PHA_DigitalProbe1_t)tr) {
        case CAENDPP_PHA_DigitalProbe2_Trigger: strcpy(buf, "Trigger"); break;
        default: strcpy(buf, "UNKNOWN"); break;
        }
    case DPPDemo_TraceType_TriggerSource: // NIAN
        switch (tr) {
        case CAENDPP_PHA_PROBETRIGGER_MainTrig: strcpy(buf, "MainTrig"); break;
        case CAENDPP_PHA_PROBETRIGGER_MainTriggerDelayCompensated: strcpy(buf, "MainTriggerDelayCompensated"); break;
        case CAENDPP_PHA_PROBETRIGGER_MainTriggerAccepedPulse: strcpy(buf, "MainTriggerAccepedPulse"); break;
        case CAENDPP_PHA_PROBETRIGGER_SelfTriggerMUX1: strcpy(buf, "SelfTriggerMUX1"); break;
        case CAENDPP_PHA_PROBETRIGGER_SelfTriggerMUX2: strcpy(buf, "SelfTriggerMUX2"); break;
        case CAENDPP_PHA_PROBETRIGGER_BaselineRestorer: strcpy(buf, "BaselineRestorer"); break;
        case CAENDPP_PHA_PROBETRIGGER_ResetDetector: strcpy(buf, "ResetDetector"); break;
        case CAENDPP_PHA_PROBETRIGGER_FreeRunning: strcpy(buf, "FreeRunning"); break;
        default: strcpy(buf, "UNKNOWN"); break;
        }
        break;
    default: strcpy(buf, "UNKNOWN"); break;
    }
    return buf;
}

/// \fn      int32_t GetModifiedChannelParameters(int32_t ch, CAENDPP_DgtzParams_t *Params);
/// \brief   Prompts the User to change the given Channel Parameters
/// 
/// \param   [IN]   ch      : The channel to modify
/// \param   [I/O]  Params  : Pointer to the parameters structure to fill
/// 
/// \return  0 = Success; negative numbers are user abort
int32_t GetModifiedChannelParameters(int32_t ch, CAENDPP_DgtzParams_t *Params) {
    int32_t ret = 0;
    int32_t quit = 0;
    while(quit == 0) {
        char c;
        float f32;
        double f64;
        int32_t d32;

        // Print the Interface Help
        d32 = (int32_t)system(CLEARSCR);
        printf("\n\n");
        printf("------------ X724/80/81 PARAMS ------------\n");
        printf("M : Change Signal Decay Time Constant\n");
        printf("m : Change Trapezoid Flat Top\n");
        printf("k : Change Trapezoid Rise Time\n");
        printf("f : Change Trapezoid Peaking Delay\n");
        printf("a : Change Trigger Filter Smoothing Factor\n");
        printf("b : Change Input Signal Rise time\n");
        printf("t : Change Trigger Threshold\n");
        printf("n : Change Number of Samples for Baseline Mean\n");
        printf("p : Change Number of Samples for Peak Mean\n");
        printf("H : Change Peak Hold Off\n");
        printf("B : Change Baseline Hold Off\n");
        printf("T : Change Trigger Hold Off\n");
        printf("g : Change Digital Probe Gain\n");
        printf("e : Change Energy Normalization Factor\n");
        printf("d : Change Decimation of Input Signal\n");
        printf("P : Change Pulse Polarity\n");
        printf("o : Change DC Offset\n");
        printf("-------------- X770 PARAMS --------------\n");
        printf("1 : Change Analog Datapath\t\t\t= %d\n", Params->ChannelExtraParameters[ch].analogPath );
		printf("2 : Input impedance\t\t\t\t= %d\n", Params->ChannelExtraParameters[ch].InputImpedance);
		printf("3 : CR gain\t\t\t\t\t= %d\n", Params->ChannelExtraParameters[ch].CRgain);
		printf("4 : PRDS gain\t\t\t\t\t= %d\n", Params->ChannelExtraParameters[ch].PRDSgain);
		printf("5 : Saturation holdoff\t\t\t\t= %d\n", Params->ChannelExtraParameters[ch].SaturationHoldoff);
		printf("---- RESET ----\n");
		printf("6 : Reset Enable\t\t\t\t= %d\n", Params->ChannelExtraParameters[ch].ResetDetector.EnableResetDetector);
		printf("7 : Reset Threhold\t\t\t\t= %d\n", Params->ChannelExtraParameters[ch].ResetDetector.thrhold );
		printf("8 : Reset minimum length\t\t\t= %d\n", Params->ChannelExtraParameters[ch].ResetDetector.reslenmin);
		printf("9 : Reset hold off\t\t\t\t= %d\n", Params->ChannelExtraParameters[ch].ResetDetector.reslenpulse);
		printf("---- DECONVOLUTOR ----\n");
		printf("R : Deconvolutor K2\t\t\t\t= %g\n", (double) Params->DPPParams.X770_extraparameters[ch].decK2 );
		printf("Y : Deconvolutor K3\t\t\t\t= %g\n", (double) Params->DPPParams.X770_extraparameters[ch].decK3 );
		printf("---- TRIGGER ----\n");
		printf("U : Mode\t\t\t\t\t= %d\n", Params->DPPParams.X770_extraparameters[ch].trigMODE);
		printf("I : Trapezoidal Rise Time\t\t\t= %f\n", (float)Params->DPPParams.X770_extraparameters[ch].trigK/1000.0);
		printf("O : Trapezoidal Flat Top\t\t\t= %f\n", (float)Params->DPPParams.X770_extraparameters[ch].trigm/1000.0);
		printf("---- ENERGY FILTER MODE ----\n");
		printf("A : Mode\t\t\t\t\t= %d\n", Params->DPPParams.X770_extraparameters[ch].energyFilterMode);
		printf("S : Peek Detector Arm threshold\t\t\t= %d\n", Params->DPPParams.X770_extraparameters[ch].PD_thrshld1);
		printf("F : Peek Detector DisArm threshold\t\t= %d\n", Params->DPPParams.X770_extraparameters[ch].PD_thrshld2 );
		printf("G : Peek Detector Windows Len\t\t\t= %g\n", ((double)Params->DPPParams.X770_extraparameters[ch].PD_winlen)/1000.0);

        printf("D : Done\n");
        printf("Q : Abort\n\n");

        // Print the Configuration
        printf("\nDecay Time\t\t\t= %.2f us\n", (float)Params->DPPParams.M[ch] / 1000.0);
        printf("Flat Top\t\t\t= %.2f us\n", (float)Params->DPPParams.m[ch] / 1000.0);
        printf("Trapezoid Rise Time\t\t= %.2f us\n", (float)Params->DPPParams.k[ch] / 1000.0);
        printf("Peaking Delay\t\t\t= %.2f us\n", (float)Params->DPPParams.ftd[ch] / 1000.0);
        printf("Smoothing Factor\t\t= %d\n", Params->DPPParams.a[ch]);
        printf("Input Rise time\t\t\t= %.2f us\n", (float)Params->DPPParams.b[ch] / 1000.0);
        printf("Threshold\t\t\t= %d\n", Params->DPPParams.thr[ch]);
        printf("Baseline Mean\t\t\t= %d\n", Params->DPPParams.nsbl[ch]);
        printf("Peak Mean\t\t\t= %d\n", Params->DPPParams.nspk[ch]);
        printf("Peak Hold Off\t\t\t= %.2f us\n", (float)Params->DPPParams.pkho[ch] / 1000.0);
        printf("Baseline Hold Off\t\t= %.2f us\n", (float)Params->DPPParams.blho[ch] / 1000.0);
        printf("Trigger Hold Off\t\t= %.2f us\n", (float)Params->DPPParams.trgho[ch] / 1000.0);
        printf("Digital Gain\t\t\t= %d\n", Params->DPPParams.dgain[ch]);
        printf("Energy Normalization Factor\t= %.2f\n", Params->DPPParams.enf[ch]);
        printf("Decimation\t\t\t= %d\n", Params->DPPParams.decimation[ch]);
        printf("Polarity\t\t\t= %s\n", Params->PulsePolarity[ch] == CAENDPP_PulsePolarityPositive ? "POS" : "NEG");
        printf("DCOffset\t\t\t= %.2f\n", ((float)(Params->DCoffset[ch]) / 655.35) - 50.0);
        printf("\n\nEnter a Command: ");

        // Prompt for a command
        c = _getch();
        printf("\n");

        // Execute the Command
        switch (c) {
        case 'M':
            printf("Enter new Decay Time (us): ");
            if (scanf("%f", &f32) == 1)
                Params->DPPParams.M[ch] = (int32_t)(f32 * 1000.0);
            break;
        case 'm':
            printf("Enter new Flat Top (us): \n");
            if (scanf("%f", &f32) == 1)
                Params->DPPParams.m[ch] = (int32_t)(f32 * 1000.0);
            break;
        case 'k':
            printf("Enter new Trapezoid Rise Time (us): ");
            if (scanf("%f", &f32) == 1)
                Params->DPPParams.k[ch] = (int32_t)(f32 * 1000.0);
            break;
        case 'f':
            printf("Enter new Peaking Delay (us): ");
            if (scanf("%f", &f32) == 1)
                Params->DPPParams.ftd[ch] = (int32_t)(f32 * 1000.0);
            break;
        case 'a':
            printf("Enter new Smoothing Factor: ");
            if (scanf("%d", &d32) == 1)
                Params->DPPParams.a[ch] = d32;
            break;
        case 'b':
            printf("Enter new Input Rise time (us): ");
            if (scanf("%f", &f32) == 1)
                Params->DPPParams.b[ch] = (int32_t)(f32 * 1000.0);
            break;
        case 't':
            printf("Enter new Threshold (LSB): ");
            if (scanf("%d", &d32) == 1)
                Params->DPPParams.thr[ch] = d32;
            break;
        case 'n':
            printf("Enter new Baseline Mean: ");
            if (scanf("%d", &d32) == 1)
                Params->DPPParams.nsbl[ch] = d32;
            break;
        case 'p':
            printf("Enter new Peak Mean: ");
            if (scanf("%d", &d32) == 1)
                Params->DPPParams.nspk[ch] = d32;
            break;
        case 'H':
            printf("Enter new Peak Hold Off (us): ");
            if (scanf("%f", &f32) == 1)
                Params->DPPParams.pkho[ch] = (int32_t)(f32 * 1000.0);
            break;
        case 'B':
            printf("Enter new Baseline Hold Off (us): ");
            if (scanf("%f", &f32) == 1)
                Params->DPPParams.blho[ch] = (int32_t)(f32 * 1000.0);
            break;
        case 'T':
            printf("Enter new Trigger Hold Off (us): ");
            if (scanf("%f", &f32) == 1)
                Params->DPPParams.trgho[ch] = (int32_t)(f32 * 1000.0);
            break;
        case 'g':
            printf("Enter new Digital Probe Gain: ");
            if (scanf("%d", &d32) == 1)
                Params->DPPParams.dgain[ch] = d32;
            break;
        case 'e':
            printf("Enter new Energy Normalization Factor: ");
            if (scanf("%f", &f32) == 1)
                Params->DPPParams.enf[ch] = f32;
            break;
        case 'd':
            printf("Enter new Decimation of Input Signal: ");
            if (scanf("%d", &d32) == 1)
                Params->DPPParams.decimation[ch] = d32;
            break;
        case 'P':
            printf("Choose new Pulse Polarity:\n\n  1) Positive\n  2) Negative\n\n");
            c = _getch();
            if (c == '1')
                Params->PulsePolarity[ch] = CAENDPP_PulsePolarityPositive;
            else if (c == '2')
                Params->PulsePolarity[ch] = CAENDPP_PulsePolarityNegative;
            break;
        case 'o':
            printf("Enter new DCOffset ]-50.0 - 50.0[: ");
            if (scanf("%f", &f32) == 1)
                Params->DCoffset[ch] = (int32_t)((f32 + 50.0) * 655.35);
            break;
        case '1':
			printf("Enter new Analog Datapath:\n\n 1) Continuos Reset 2) Transient Reset 3) Direct Detector \n\n ");
			c = _getch();
            if (c == '1')
                Params->ChannelExtraParameters[ch].analogPath = 0;
            else if (c == '2')
                Params->ChannelExtraParameters[ch].analogPath = 1;
			else if (c == '3')
                Params->ChannelExtraParameters[ch].analogPath = 2;
            break;
		case '2':
			printf("Enter Input Impedance:\n\n 1) 50R 2) 10K\n\n ");
			c = _getch();
            if (c == '1')
                Params->ChannelExtraParameters[ch].InputImpedance = 0;
            else if (c == '2')
                 Params->ChannelExtraParameters[ch].InputImpedance = 1;
            break;
        case '3':
            printf("Enter Continuos Reset attenuator:\n\n 1) 1   2) 1/2  3) 1/4\n\n");
			c = _getch();
            if (c == '1')
                Params->ChannelExtraParameters[ch].CRgain = 0;
            else if (c == '2')
                Params->ChannelExtraParameters[ch].CRgain = 1;
			else if (c == '3')
                Params->ChannelExtraParameters[ch].CRgain = 2;
            break;
        case '4':
            printf("Enter Transient Reset/Direct Sampling gain: ");
            if (scanf("%d", &d32) == 1)
                Params->ChannelExtraParameters[ch].PRDSgain = d32;
            break;

		case '5':
            printf("Enter Saturation Hold off: (us)");
            if (scanf("%f", &f32) == 1)
                Params->ChannelExtraParameters[ch].SaturationHoldoff = (int32_t)(f32 * 1000.0);
            break;
		case '6':
			printf("Enter Reset Detector:\n\n 1) ON 2) OFF\n\n ");
			c = _getch();
            if (c == '1')
                Params->ChannelExtraParameters[ch].ResetDetector.EnableResetDetector = 1;
            else if (c == '2')
                Params->ChannelExtraParameters[ch].ResetDetector.EnableResetDetector = 0;
            break;
		case '7':
            printf("Enter Reset minimum len: (us)");
            if (scanf("%d", &f32) == 1)
                Params->ChannelExtraParameters[ch].ResetDetector.thrhold = (int32_t)(f32 * 1000.0);
            break;
		case '8':
            printf("Enter Reset threshold: (us)");
            if (scanf("%d", &f32) == 1)
                Params->ChannelExtraParameters[ch].ResetDetector.reslenmin = (int32_t)(f32 * 1000.0);
            break;
		case '9':
            printf("Enter Reset holdoff: (us)");
            if (scanf("%d", &f32) == 1)
               Params->ChannelExtraParameters[ch].ResetDetector.reslenpulse = (int32_t)(f32 * 1000.0);
            break;
		case 'R':
            printf("Enter deconvolutor K2: ");
            if (scanf("%lf", &f64) == 1)
               Params->DPPParams.X770_extraparameters[ch].decK2 = f64;
            break;
		case 'Y':
            printf("Enter deconvolutor K3: ");
            if (scanf("%lf", &f64) == 1)
               Params->DPPParams.X770_extraparameters[ch].decK3 = f64;
            break;
		case 'U':
			printf("Trigger Mode:\n\n 1) On fast trapezoid 2) On sec dev of trapezoid\n\n ");
			c = _getch();
            if (c == '1')
                Params->DPPParams.X770_extraparameters[ch].trigMODE = 0;
            else if (c == '2')
                Params->DPPParams.X770_extraparameters[ch].trigMODE = 1;
            break;
		case 'I':
            printf("Enter fast trap rise time (us)");
            if (scanf("%f", &f32) == 1)
               Params->DPPParams.X770_extraparameters[ch].trigK =(int32_t)(f32 * 1000.0);
            break;
		case 'O':
            printf("Enter fast trap flat top (us)");
            if (scanf("%f", &f32) == 1)
               Params->DPPParams.X770_extraparameters[ch].trigm =(int32_t)(f32 * 1000.0);
            break;
		case 'A':
			printf("Energy Filter Mode:\n\n 1) Trapezoid 2) Peak Detector\n\n ");
			c = _getch();
            if (c == '1')
                Params->DPPParams.X770_extraparameters[ch].energyFilterMode = 0;
            else if (c == '2')
                Params->DPPParams.X770_extraparameters[ch].energyFilterMode = 1;
            break;
		case 'S':
            printf("Enter arm threshold: LSB");
            if (scanf("%d", &d32) == 1)
                 Params->DPPParams.X770_extraparameters[ch].PD_thrshld1 = (int32_t)(f32 * 1000.0);
            break;
		case 'F':
            printf("Enter disarm threshold: LSB");
            if (scanf("%d", &d32) == 1)
                 Params->DPPParams.X770_extraparameters[ch].PD_thrshld2 = (int32_t)(f32 * 1000.0);
            break;
		case 'G':
            printf("Enter windows len: (us)");
            if (scanf("%f", &f32) == 1)
               Params->DPPParams.X770_extraparameters[ch].PD_winlen =(int32_t)(f32 * 1000.0);
            break;
        case 'D':
            return 0;
        case 'Q':
            return -1;
        default:
            break;
        }
    }
    return -2;
}

/// \fn      int32_t GetModifiedBoardParameters(CAENDPP_DgtzParams_t *Params);
/// \brief   Prompts the User to change the given Channel Parameters
/// 
/// \param   [IN]   ch      : The channel to modify
/// \param   [I/O]  Params  : Pointer to the parameters structure to fill
/// 
/// \return  0 = Success; negative numbers are error codes
int32_t GetModifiedBoardParameters(CAENDPP_AcqMode_t *acqMode, CAENDPP_DgtzParams_t *Params, CAENDPP_Info_t *bInfo) {
    int32_t ret = 0;
    int32_t quit = 0;
    while(quit == 0) {
        char c;
        int32_t d32;
        uint32_t u32;
        char str[MAX_TRACENAME_LEN];

        // Print the Interface Help
        d32 = (int32_t)system(CLEARSCR);
        printf("\n\n");
        printf("m : Change Channel Enable Mask\n");
        printf("e : Change Number of Events for Readout\n");
        printf("i : Change I/O level\n");
        printf("q : Select Digital Trace 1\n");
        printf("w : Select Digital Trace 2\n");
        printf("1 : Select Analog Trace 1\n");
        printf("2 : Select Analog Trace 2\n");
        printf("L : Set self trigger level\n");
		printf("t : Set trigger mode\n");
        printf("T : Toggle Dual Trace Enabled\n");
        printf("p : Change PreTrigger\n");
        printf("r : Change RecordLength\n");
        printf("l : Toggle List Mode Enabled\n");
        printf("f : Set Filename for List Saving\n");
        printf("n : Change List Event Buffer Size\n");
        printf("A : Change Acquisiton Mode\n");
        printf("D : Done\n");
        printf("Q : Abort\n\n");

        // Print the Configuration
        printf("\nChannel Enable Mask\t= 0x%02X\n", Params->ChannelMask);
        printf("Events for Readout\t= %u\n", Params->EventAggr);
        printf("I/O level\t\t= %s\n", Params->IOlev == CAENDPP_IOLevel_NIM? "NIM" : "TTL");
        printf("Digital Trace 1\t\t= %s\n", getTraceName(str, DPPDemo_TraceType_Digital1, Params->WFParams.dp1));
        printf("Digital Trace 1\t\t= %s\n", getTraceName(str, DPPDemo_TraceType_Digital2, Params->WFParams.dp2));
        printf("Analog Trace 1\t\t= %s\n", getTraceName(str, DPPDemo_TraceType_Analog1, Params->WFParams.vp1));
        printf("Analog Trace 2\t\t= %s\n", getTraceName(str, DPPDemo_TraceType_Analog2, Params->WFParams.vp2));
        printf("Trace Trigger\t\t= %s\n", getTraceName(str, DPPDemo_TraceType_TriggerSource, Params->WFParams.probeTrigger));
        printf("Self trigger level\t= %d\n", Params->WFParams.probeSelfTriggerVal);
        printf("Dual Trace\t\t= %s\n", Params->WFParams.dualTraceMode == 0 ? "OFF" : "ON");
        printf("PreTrigger\t\t= %d\n", Params->WFParams.preTrigger);
        printf("RecordLength\t\t= %d\n", Params->WFParams.recordLength);
        printf("List Mode\t\t= %s\n", Params->ListParams.enabled == FALSE ? "OFF" : "ON");
        printf("Filename\t\t= %s\n", Params->ListParams.fileName);
        printf("List Buffer Size\t= %u\n", Params->ListParams.maxBuffNumEvents);
        printf("Acquisition Mode\t= %s\n", *acqMode == CAENDPP_AcqMode_Waveform ? "Waveform" : "Histogram");
        printf("\n\nEnter a Command: ");

        // Prompt for a command
        c = _getch();
        printf("\n");

        // Execute the Command
        switch (c) {
        case 'm':
            printf("Enter new Channel Enable Mask: 0x");
            if (scanf("%x", &d32) == 1)
                Params->ChannelMask = d32;
            break;
        case 'e':
            printf("Enter new Number of Events: ");
            if (scanf("%d", &d32) == 1)
                Params->EventAggr = d32;
            break;
        case 'i':
            printf("Choose new I/O level: \n\n  1) NIM\n  2) TTL\n");
            c = _getch();
            if (c == '1')
                Params->IOlev = CAENDPP_IOLevel_NIM;
            else if (c == '2')
                Params->IOlev = CAENDPP_IOLevel_TTL;
            break;
        case 'q':
            if (bInfo->NumDigitalProbes1 <= 0)
                continue;
            printf("Choose new Digital Trace 1: \n\n");
            for (u32 = 0; u32 < bInfo->NumDigitalProbes1; u32++)
                printf("  %d : %s\n", u32, getTraceName(str, DPPDemo_TraceType_Digital1, bInfo->SupportedDigitalProbes1[u32]));
            c = _getch();
            u32 = (int)c - (int)'0';
            if (u32 < bInfo->NumDigitalProbes1)
                Params->WFParams.dp1 = (CAENDPP_PHA_DigitalProbe1_t)bInfo->SupportedDigitalProbes1[u32];
            break;
        case 'w':
            if (bInfo->NumDigitalProbes2 <= 0)
                continue;
            printf("Choose new Digital Trace 2: \n\n");
            for (u32 = 0; u32 < bInfo->NumDigitalProbes2; u32++)
                printf("  %d : %s\n", u32, getTraceName(str, DPPDemo_TraceType_Digital2, bInfo->SupportedDigitalProbes2[u32]));
            c = _getch();
            u32 = (int)c - (int)'0';
            if (u32 < bInfo->NumDigitalProbes2)
                Params->WFParams.dp2 = (CAENDPP_PHA_DigitalProbe2_t)bInfo->SupportedDigitalProbes2[u32];
            break;
        case '1':
            if (bInfo->NumVirtualProbes1 <= 0)
                continue;
            printf("Choose new Analog Trace 1: \n\n");
            for (u32 = 0; u32 < bInfo->NumVirtualProbes1; u32++)
                printf("  %d : %s\n", u32, getTraceName(str, DPPDemo_TraceType_Analog1, bInfo->SupportedVirtualProbes1[u32]));
            c = _getch();
            u32 = (int)c - (int)'0';
            if (u32 < bInfo->NumVirtualProbes1)
                Params->WFParams.vp1 = (CAENDPP_PHA_VirtualProbe1_t)bInfo->SupportedVirtualProbes1[u32];
            break;
        case '2':
            if (bInfo->NumVirtualProbes2 <= 0)
                continue;
            printf("Choose new Analog Trace 2: \n\n");
            for (u32 = 0; u32 < bInfo->NumVirtualProbes2; u32++)
                printf("  %d : %s\n", u32, getTraceName(str, DPPDemo_TraceType_Analog2, bInfo->SupportedVirtualProbes2[u32]));
            c = _getch();
            u32 = (int)c - (int)'0';
            if (u32 < bInfo->NumVirtualProbes2)
                Params->WFParams.vp2 = (CAENDPP_PHA_VirtualProbe2_t)bInfo->SupportedVirtualProbes2[u32];
            break;
        case 'L':
            printf("Enter self trigger level in LSB: ");
            if (scanf("%d", &d32) == 1)
                Params->WFParams.probeSelfTriggerVal = d32;
            break;
        case 't':
            printf("Choose scope trigger source: \n\n");
			for (d32 = 0; d32 <= 7; d32++)
				printf("  %d : %s\n", d32, getTraceName(str, DPPDemo_TraceType_TriggerSource, d32));
			c = _getch();
			d32 = (int)c - (int)'0';
			if (d32 >= 0 && d32 <= 7)
                Params->WFParams.probeTrigger = (CAENDPP_PHA_ProbeTrigger_t)d32;
            break;
        case 'T':
            Params->WFParams.dualTraceMode = (Params->WFParams.dualTraceMode != 0) ? 0 : 1;
            break;
        case 'p':
            printf("Enter new PreTrigger: ");
            if (scanf("%d", &d32) == 1)
                Params->WFParams.preTrigger = d32;
            break;
        case 'r':
            printf("Enter new RecordLength: ");
            if (scanf("%d", &d32) == 1)
                Params->WFParams.recordLength = d32;
            break;
        case 'l':
            Params->ListParams.enabled = (Params->ListParams.enabled != FALSE) ? FALSE : TRUE;
            break;
        case 'f':
            printf("Enter new Filename: ");
            d32 = (int32_t)scanf("%s", Params->ListParams.fileName);
            break;
        case 'n':
            printf("Enter new  Size: ");
            if (scanf("%d", &d32) == 1)
                Params->ListParams.maxBuffNumEvents = d32;
            break;
        case 'D':
            // Hardcode parameters still not implemented
            Params->ListParams.saveMode = CAENDPP_ListSaveMode_FileBinary;
            Params->ListParams.saveMask = 0xF;
            return 0;
        case 'A':
            printf("Choose new Acquisition Mode: \n\n  1) Waveform\n  2) Histogram\n");
            c = _getch();
            if (c == '1')
                *acqMode = CAENDPP_AcqMode_Waveform;
            else if (c == '2')
                *acqMode = CAENDPP_AcqMode_Histogram;
            break;
        case 'Q':
            return -1;
        default:
            break;
        }
    }
    return -2;
}

/// \fn      char *decodeError(char *dest, int32_t ret);
/// \brief   Decodes the given error code into a message
/// 
/// \param   [IN] dest   : the string to be filled
/// \param   [IN] ret    : the error code
/// 
/// \return  a pointer to the string if success, otherwise NULL
char *decodeError(char *dest, int32_t code) {
    char codeStr[10];
    size_t nc = MAX_ERRMSG_LEN;
    sprintf(codeStr, " [%d]", code);
    nc -= strlen(codeStr);
    switch(code) {
    case CAENDPP_RetCode_Ok: strncpy(dest, "Success", nc); break;
    case CAENDPP_RetCode_GenericError: strncpy(dest, "Generic Error", nc); break;
    case CAENDPP_RetCode_TooManyInstances: strncpy(dest, "Too Many Instances", nc); break;
    case CAENDPP_RetCode_ProcessFail: strncpy(dest, "Process Fail", nc); break;
    case CAENDPP_RetCode_ReadFail: strncpy(dest, "Read Fail", nc); break;
    case CAENDPP_RetCode_WriteFail: strncpy(dest, "Write Fail", nc); break;
    case CAENDPP_RetCode_BadMessage: strncpy(dest, "Bad Message", nc); break;
    case CAENDPP_RetCode_InvalidHandle: strncpy(dest, "Invalid Handle", nc); break;
    case CAENDPP_RetCode_ConfigError: strncpy(dest, "Configuration Error", nc); break;
    case CAENDPP_RetCode_BoardInitFail: strncpy(dest, "Board Initialization Failed", nc); break;
    case CAENDPP_RetCode_TimeoutError: strncpy(dest, "Timeout Error", nc); break;
    case CAENDPP_RetCode_InvalidParameter: strncpy(dest, "Invalid Parameter", nc); break;
    case CAENDPP_RetCode_NotInWaveMode: strncpy(dest, "Not in Waveform Mode", nc); break;
    case CAENDPP_RetCode_NotInHistoMode: strncpy(dest, "Not in Histogram Mode", nc); break;
    case CAENDPP_RetCode_NotInListMode: strncpy(dest, "Not in List Mode", nc); break;
    case CAENDPP_RetCode_NotYetImplemented: strncpy(dest, "Not Yet Implemented", nc); break;
    case CAENDPP_RetCode_BoardNotConfigured: strncpy(dest, "Board not Configured", nc); break;
    case CAENDPP_RetCode_InvalidBoardIndex: strncpy(dest, "Invalid Board Index", nc); break;
    case CAENDPP_RetCode_InvalidChannelIndex: strncpy(dest, "Invalid Channel Index", nc); break;
    case CAENDPP_RetCode_UnsupportedFirmware: strncpy(dest, "Unsupported Firmware", nc); break;
    case CAENDPP_RetCode_NoBoardsAdded: strncpy(dest, "No Boards Added", nc); break;
    case CAENDPP_RetCode_AcquisitionRunning: strncpy(dest, "Acquisition Running", nc); break;
    case CAENDPP_RetCode_OutOfMemory: strncpy(dest, "Out of Memory", nc); break;
    case CAENDPP_RetCode_BoardChannelIndex: strncpy(dest, "Invalid Board Channel Index", nc); break;
    case CAENDPP_RetCode_HistoAlloc: strncpy(dest, "Histogram Allocation Failed", nc); break;
    case CAENDPP_RetCode_OpenDumper: strncpy(dest, "Dumper Open Failed", nc); break;
    case CAENDPP_RetCode_BoardStart: strncpy(dest, "Board Start Failed", nc); break;
    case CAENDPP_RetCode_ChannelEnable: strncpy(dest, "Channel not Enabled", nc); break;
    case CAENDPP_RetCode_InvalidCommand: strncpy(dest, "Invalid Command", nc); break;
    case CAENDPP_RetCode_NumBins: strncpy(dest, "Invalid Number of Bins", nc); break;
    case CAENDPP_RetCode_HistoIndex: strncpy(dest, "Invalid Histogram Index", nc); break;
    case CAENDPP_RetCode_UnsupportedFeature: strncpy(dest, "Unsupported Feature", nc); break;
    case CAENDPP_RetCode_BadHistoState: strncpy(dest, "Incompatible Histogram Status", nc); break;
    case CAENDPP_RetCode_NoMoreHistograms: strncpy(dest, "No more Histograms Available", nc); break;
    case CAENDPP_RetCode_NotHVBoard: strncpy(dest, "Not an HV Board", nc); break;
    case CAENDPP_RetCode_InvalidHVChannel: strncpy(dest, "Invalid HV Channel", nc); break;
    case CAENDPP_RetCode_SocketSend: strncpy(dest, "Socket Send Error", nc); break;
    case CAENDPP_RetCode_SocketReceive: strncpy(dest, "Socket Receive Error", nc); break;
    case CAENDPP_RetCode_BoardThread: strncpy(dest, "Board Acquisition Thread Error", nc); break;
    case CAENDPP_RetCode_DecodeWaveform: strncpy(dest, "Cannot Decode Waveform from Buffer", nc); break;
    case CAENDPP_RetCode_OpenDigitizer: strncpy(dest, "Error Opening the digitizer", nc); break;
    case CAENDPP_RetCode_BoardModel: strncpy(dest, "Unsupported Board Model", nc); break;
    case CAENDPP_RetCode_AutosetStatus: strncpy(dest, "Incompatible Autoset Status", nc); break;
    case CAENDPP_RetCode_Autoset: strncpy(dest, "Autoset Error", nc); break;
    default: strncpy(dest, "UNKNOWN Error", nc);
    }
    strcat(dest, codeStr);
    return dest;
}

/// \fn      void setStatusMessage(char *msg);
/// \brief   Sets (if last status already printed) or Adds (if last status still not printed)
///          the given message on the status message
/// 
/// \param   [IN] msg   : Message to set/add
void setStatusMessage(const char* _Format, ...) {
    va_list args;
    char msg[MAX_STATUSMSG_LEN + 1];
    int32_t size;
    size_t len;
    size_t availNChars = MAX_STATUSMSG_LEN;
    size_t nChars = 0;
    char *ptr = NULL;
    
    va_start(args, _Format);
    size = vsnprintf(msg, MAX_STATUSMSG_LEN, _Format, args);
    va_end(args);

    len = strlen(msg);
    if (statusMsgPrinted == 1) {
        // Point to the beginning of the status length
        ptr = statusMessage;
        statusMsgPrinted = 0;
    }
    else {
        // Add a '\n' to the status message and point to the end of it
        if (strlen(statusMessage) < MAX_STATUSMSG_LEN)
            strcat(statusMessage, "\n");
        availNChars -= strlen(statusMessage);
        ptr = statusMessage + strlen(statusMessage);
    }
    // Print everything on ptr
    nChars = min(len + 1, availNChars);
    strncpy(ptr, msg, nChars);
    if (len > availNChars) {
        // Fill the last part of the status string with '...'
        char strEnd[5] = "...";
        strcat(strEnd, msg + len - 1); // Print the last msg letter
        strcpy(ptr + availNChars - strlen(strEnd), strEnd);
    }
}

/// \fn      void printStatusMessage();
/// \brief   Prints the last set status msg to screen
/// 
/// \param   [IN] msg   : Message to display
void printStatusMessage() {
    printf("%s", statusMessage);
    statusMsgPrinted = 1;
}

/// \fn      int32_t printAcquisitionInterface();
/// \brief   Prints the available commands list
/// 
/// \param   [IN] msg   : Message to display
void printAcquisitionInterface() {
    int res;
    res = system(CLEARSCR);
    printf("\n");
    printf("\tActive board [%d] - Active channel [%d] - Active Histo [%d]\n", currBrd, currCh, currHisto);
    printf("\n##################################################\n");
    printf("#####         CONFIGURATION COMMANDS         #####\n\n");
    printf("C : Configure Boards\n");
    printf("M : Change 'Active Board' Configuration\n");
    printf("m : Change 'Active Channel' Configuration\n");
    printf("r : Change 'Active Channel' Input Range\n");
    printf("A : Autofind Acquisition Parameters\n");
    printf("\n##################################################\n");
    printf("#####          ACQUISITION COMMANDS          #####\n\n");
    printf("s : Start acquisition\n");
    printf("S : Stop acquisition\n");
    printf("E : Change Stop Criteria\n");
    printf("e : Get Stop Criteria\n");
    printf("T : Toggle Continuous Software Trigger\n");
    printf("\n##################################################\n");
    printf("#####          HISTOGRAM MANAGEMENT          #####\n\n");
    printf("c : Clear Histogram\n");
    printf("R : Reset All Histograms\n");
    printf("y : Get 'Active Histo' status\n");
    printf("Y : Change 'Active Histo' status\n");
    printf("u : Get Current Histogram Index\n");
    printf("U : Set Current Histogram Index to 'Active Histo'\n");
    printf("N : Switch to Next Histogram\n");
    printf("d : Change 'Active Histo' Size\n");
    printf("n : Get Number of Histograms\n");
    printf("a : Add new Histogram\n");
    printf("h : Set 'Active Histogram' from file\n");
    printf("%c : Current Histogram Single Plot\n", SINGLE_PLT_CHAR);
    printf("P : Toggle Current Histogram Continuous Plot\n");
    printf("H : Save Histograms to file\n");
    printf("\n##################################################\n");
    printf("#####            CONTROL COMMANDS            #####\n\n");
    printf("9 : Increase Active Histogram\n");
    printf("8 : Decrease Active Histogram\n");
    printf("+ : Increase Active Channel\n");
    printf("- : Decrease Active Channel\n");
    printf("* : Increase Active Board\n");
    printf("/ : Decrease Active Board\n");
    printf("\n##################################################\n");
    printf("#####             OTHER COMMANDS             #####\n\n");
    printf("w : Waveform single plot\n");
    printf("W : Save waveforms to file\n");
    printf("V : Switch to HV Interface\n");
    printf("! : Enumerate Available Devices\n");
}

/// \fn      int32_t printHVInterface();
/// \brief   Prints the available commands list for High Voltage Mode
/// 
/// \param   [IN] msg   : Message to display
void printHVInterface() {
    int res;
    res = system(CLEARSCR);
    printf("\n");
    printf("\t\tActive HV channel [%s]\n\n", currHVChName);
    printf("\n##################################################\n");
    printf("#####          MONITORING COMMANDS           #####\n\n");
    printf("%c : Single HV Monitor Cycle\n", HVMON_PLT_CHAR);
    printf("C : Toggle Continuous HV Monitoring\n");
    printf("\n##################################################\n");
    printf("#####         CONFIGURATION COMMANDS         #####\n\n");
    printf("c : Get HV Channel Configuration\n");
    printf("V : Change VSet\n");
    printf("I : Change ISet\n");
    printf("M : Change VMax\n");
    printf("D : Change Ramp Down\n");
    printf("U : Change Ramp Up\n");
    printf("P : Change Power Down Mode\n");
    printf("\n##################################################\n");
    printf("#####            POWER COMMANDS              #####\n\n");
    printf("O : Turn HV Channel ON\n");
    printf("o : Turn HV Channel OFF\n");
    printf("S : Switch HV Channel ON/OFF\n");
    printf("\n##################################################\n");
    printf("#####           CONTROL COMMANDS             #####\n\n");
    printf("+ : Increase Active HV Channel\n");
    printf("- : Decrease Active HV Channel\n");
    printf("\n##################################################\n");
    printf("#####            OTHER COMMANDS              #####\n\n");
    printf("A : Switch to Acquisition Interface\n");
}

/// \fn      int32_t printInterface();
/// \brief   Prints the available commands list
/// 
/// \param   [IN] msg   : Message to display
void printInterface() {
    switch(IFMode) {
    case IFMODE_HV:
        printHVInterface();
        break;
    default:
    case IFMODE_ACQ:
        printAcquisitionInterface();
        break;
    }

    // Write also the commads common to both interfaces
    printf("B : Add Board\n");
    printf("q : Quit\n\n\n");
    printStatusMessage();
    printf("\n\n\nType a command: ");
}

/// \fn      DPPDemo_Cmd_t getACQCommad(char c);
/// \brief   coverts the character 'c' to the corresponding
///          'DPPDemo_Cmd_t' command in Acquisition Mode
/// 
/// \param   [IN] c   : The character to convert
DPPDemo_Cmd_t getACQCommad(char c) {
    switch (c) {
        case 'A': return DPPDemo_Cmd_Autoset;
        case 'M': return DPPDemo_Cmd_SetBoardConfig;
        case 'm': return DPPDemo_Cmd_SetChannelConfig;
        case 'r': return DPPDemo_Cmd_SetInputRange;
        case 'C': return DPPDemo_Cmd_Configure;
        case 'E': return DPPDemo_Cmd_SetStopCriteria;
        case 'e': return DPPDemo_Cmd_GetStopCriteria;
        case 's': return DPPDemo_Cmd_StartAcq;
        case 'S': return DPPDemo_Cmd_StopAcq;
        case 'T': return DPPDemo_Cmd_SwTrgToggle;
        case 'c': return DPPDemo_Cmd_ClearHisto;
        case 'R': return DPPDemo_Cmd_ResetAllHistos;
        case 'N': return DPPDemo_Cmd_SwitchHisto;
        case 'H': return DPPDemo_Cmd_SaveHisto;
        case SINGLE_PLT_CHAR: return DPPDemo_Cmd_HistoSinglePlot;
        case 'P': return DPPDemo_Cmd_HistoContinuousPlot;
        case 'W': return DPPDemo_Cmd_SaveWave;
        case 'w': return DPPDemo_Cmd_WaveSinglePlot;
        case 'U': return DPPDemo_Cmd_SetCurrentHisto;
        case 'u': return DPPDemo_Cmd_GetCurrentHisto;
        case 'n': return DPPDemo_Cmd_GetHistoNum;
        case 'a': return DPPDemo_Cmd_AddHisto;
        case 'h': return DPPDemo_Cmd_SetHisto;
        case 'd': return DPPDemo_Cmd_SetHistoSize;
        case 'y': return DPPDemo_Cmd_GetHistoStatus;
        case 'Y': return DPPDemo_Cmd_SetHistoStatus;
        case 'V': return DPPDemo_Cmd_HVInterface;
        case '9': return DPPDemo_Cmd_IncrActiveHisto;
        case '8': return DPPDemo_Cmd_DecrActiveHisto;
        case '+': return DPPDemo_Cmd_IncrActiveChannel;
        case '-': return DPPDemo_Cmd_DecrActiveChannel;
        case '*': return DPPDemo_Cmd_IncrActiveBoard;
        case '/': return DPPDemo_Cmd_DecrActiveBoard;
        default: break;
    }
    return DPPDemo_Cmd_Unknown;
}

/// \fn      DPPDemo_Cmd_t getHVCommad(char c);
/// \brief   coverts the character 'c' to the corresponding
///          'DPPDemo_Cmd_t' command in Acquisition Mode
/// 
/// \param   [IN] c   : The character to convert
DPPDemo_Cmd_t getHVCommad(char c) {
    switch (c) {
    case HVMON_PLT_CHAR: return DPPDemo_Cmd_SingleHVMonitor;
    case 'C': return DPPDemo_Cmd_ContinuousHVMonitor;
    case 'c': return DPPDemo_Cmd_GetHVConfig;
    case 'V': return DPPDemo_Cmd_VSet;
    case 'I': return DPPDemo_Cmd_ISet;
    case 'M': return DPPDemo_Cmd_VMax;
    case 'D': return DPPDemo_Cmd_RampDown;
    case 'U': return DPPDemo_Cmd_RampUp;
    case 'P': return DPPDemo_Cmd_PWDownMode;
    case 'O': return DPPDemo_Cmd_TurnHVON;
    case 'o': return DPPDemo_Cmd_TurnHVOFF;
    case 'S': return DPPDemo_Cmd_SwitchHVPwr;
    case 'A': return DPPDemo_Cmd_ACQInterface;
    case '+': return DPPDemo_Cmd_IncrActiveHVChannel;
    case '-': return DPPDemo_Cmd_DecrActiveHVChannel;
    default: break;
    }
    return DPPDemo_Cmd_Unknown;
}

/// \fn      DPPDemo_Cmd_t getCommonCommad(char c);
/// \brief   coverts the character 'c' to the corresponding
///          'DPPDemo_Cmd_t' command in bith Interface Modes
/// 
/// \param   [IN] c   : The character to convert
DPPDemo_Cmd_t getCommad(char c) {
    DPPDemo_Cmd_t res;

    switch(IFMode) {
    case IFMODE_HV:
        res = getHVCommad(c);
        break;
    default:
    case IFMODE_ACQ:
        res = getACQCommad(c);
        break;
    }
    if (res != DPPDemo_Cmd_Unknown)
        return res;
    
    switch (c) {
    case 'B': return DPPDemo_Cmd_AddBoard;
    case '!': return DPPDemo_Cmd_Enumerate;
    case 'q': return DPPDemo_Cmd_Quit;
    default: break;
    }
    return DPPDemo_Cmd_Unknown;
}

/// \fn      int32_t getBrdRange(int32_t *f, int32_t *l);
/// \brief   Gets the board range depending on 'currBrd' value.
///          This function gives the complete board range if 'currBrd' < 0
///          or the single board range otherwise. The resulting
///          range is aimed to be used inside a 'for' loop
/// 
/// \param   [OUT] f    : Index of the First board in range
/// \param   [OUT] l    : Index of the Last board in range
/// 
/// \return  0 = Success; negative numbers are error codes
int32_t getBrdRange(int32_t *f, int32_t *l) {
    int32_t ret = 0;
    if (currBrd < 0) {
        *f = 0;
        *l = connectedBrdNum;
    }
    else if (currBrd < connectedBrdNum) {
        *f = currBrd;
        *l = currBrd + 1;
    }
    else { // Invalid currBrd value
        ret = - 1;
        *f = 0;
        *l = 0;
    }
    return ret;
}

/// \fn      int32_t getChRange(int32_t *f, int32_t *l) {
/// \brief   Gets the channel range depending on 'currCh' value.
///          This function gives the complete channel range if 'currCh' < 0
///          or the single channel range otherwise. The resulting
///          range is aimed to be used inside a 'for' loop
/// 
/// \param   [OUT] f    : Index of the First channel in range
/// \param   [OUT] l    : Index of the Last channel in range
/// 
/// \return  0 = Success; negative numbers are error codes
int32_t getChRange(int32_t *f, int32_t *l) {
    int32_t ret = 0;
    if (currCh < 0) {
        *f = 0;
        *l = connectedChNum;
    }
    else if (currCh < connectedChNum) {
        *f = currCh;
        *l = currCh + 1;
    }
    else { // Invalid currCh value
        ret = - 1;
        *f = 0;
        *l = 0;
    }
    return ret;
}

/// \fn      int32_t isChannelDisabled(int32_t handle, int32_t ch);
/// \brief   Get if a channel is Enabled
/// 
/// \param   [IN] handle    : Handle to the Library
/// \param   [IN] ch        : Channel index to check
/// 
/// \return  0 if the channel is enabled, 1 if it is disabled
int32_t isChannelDisabled(int32_t handle, int32_t ch) {
    int32_t enabled;
    int32_t ret;
    ret = CAENDPP_IsChannelEnabled(handle, ch, &enabled);
    if (ret != CAENDPP_RetCode_Ok)
        return ret;
    return !enabled;
}

/// \fn      int32_t plotHisto(char *title, uint32_t *EHisto, int32_t nbins, uint32_t counts, double RT_s, double DT_s);
/// \brief   Plots an Histogram
/// 
/// \param   [IN] title     : Title of the histogram
/// \param   [IN] EHisto    : Histogram Datas
/// \param   [IN] nbins     : Histogram Number of Bins
/// \param   [IN] counts    : Histogram Total Counts
/// \param   [IN] RT_s      : Histogram Real Time
/// \param   [IN] DT_s      : Histogram Dead Time
/// 
/// \return  0 = Success; negative numbers are error codes
int32_t plotHisto(char *title, uint32_t *EHisto, int32_t nbins, uint32_t counts, double RT_s, double DT_s) {
    char *pltDataName = "PlotData.txt";
    if (gnuplot == NULL) {
        gnuplot = _popen(GNUPLOT_CMD, "w");
        if (gnuplot == NULL) {
            setStatusMessage("Can't open GNUPLOT");
            return - 1;
        }
    }
    if (saveHistogram(pltDataName, EHisto, nbins, 0, 0)) { // Avoid RealTime & DeadTime dumping
        setStatusMessage("Can't save Plot Data to file.");
        return - 2;
    }
    
    fprintf(gnuplot, "set ylabel 'Counts'\n");
    fprintf(gnuplot, "set xlabel 'ADC Channels'\n");
    fprintf(gnuplot, "plot '%s' title '%s: ### C = %u (RT/DT) = (%.2f/%.2f)s ###' w step linecolor 1\n", pltDataName, title, counts, RT_s, DT_s);
    
    fflush(gnuplot);
    return 0;
}

/// \fn      int32_t plotWaveform(int32_t nTraces, int32_t *tr, void **traces, DPPDemo_TraceType_t *types, int32_t ns);
/// \brief   Plot a Waveform
/// 
/// \param   [IN]  filename     : The name of the file to write to
/// \param   [IN]  nTraces      : The number of Traces
/// \param   [IN]  traces       : Pointer to Traces datas
/// \param   [IN]  types        : The Types of the Traces
/// \param   [IN]  ns           : The number of samples of the waveform
/// 
/// \return  0 = Success; negative numbers are error codes
int32_t plotWaveform(int32_t nTraces, int32_t *tr, void **traces, DPPDemo_TraceType_t *types, int32_t ns) {
    int32_t ret = 0, t;
    char *pltDataName = "PlotData.txt";
    char titles[MAX_NUM_TRACES][MAX_TRACENAME_LEN];
    float gains[] = {1.0, 1.0, 1000.0, 1000.0};
    float offsets[] = {0.0, 0.0, 0.0, gains[2] + 100};

    if (gnuplot == NULL) {
        gnuplot = _popen(GNUPLOT_CMD, "w");
        if (gnuplot == NULL) {
            setStatusMessage("Can't open GNUPLOT");
            return - 1;
        }
    }

    if ((ret = saveWaveform(pltDataName, nTraces, traces, types, ns) != 0)) {
        setStatusMessage("Can't Plot Waveform: cannot create data file");
        return ret;
    }
    
    fprintf(gnuplot, "set ylabel 'ADC Counts'\n");
    fprintf(gnuplot, "set xlabel 'Samples'\n");
    fprintf(gnuplot, "plot ");
    for (t = 0; t < nTraces; t++) {
        if (t > 0)
            fprintf(gnuplot, ", ");
        sprintf(titles[t], "%s", getTraceName(titles[t], types[t], tr[t]));
        fprintf(gnuplot, "'%s' u ($1):($%d * %f + %f) title '%s' w step linecolor %d", pltDataName, t + 2, gains[t], offsets[t], titles[t], t + 1);
    }
    fprintf(gnuplot, "\n");
    
    fflush(gnuplot);
    return 0;
}

/// \fn      void setContinuousPlotPeriod(size_t Timeout);
/// \brief   Sets the period of continuous plot
/// 
/// \param   [IN] Timeout   : Timeout in ms of the continuous plot.
///                           NOTE: if Timeout == 0->Continuous plot disabled
void setContinuousPlotPeriod(uint64_t Timeout) { // NOTE: Timeout = 0 means disabled
    setNextCharTimeout(Timeout, SINGLE_PLT_CHAR);
}

/// \fn      void setContinuousHVMonPeriod(size_t Timeout);
/// \brief   Sets the period of continuous HV Monitoring
/// 
/// \param   [IN] Timeout   : Timeout in ms of the continuous monitoring.
///                           NOTE: if Timeout == 0->Continuous monitoring disabled
void setContinuousHVMonPeriod(uint64_t Timeout) { // NOTE: Timeout = 0 means disabled
    setNextCharTimeout(Timeout, HVMON_PLT_CHAR);
}

/// \fn      int32_t getConnectionParams(CAENDPP_ConnectionParams_t *connParam);
/// \brief   Asks the user to enter connection parameters.
/// 
/// \param   [OUT] connParam    : The parameters etered by the user
/// 
/// \return  0 = Success; negative numbers = User Abort
int32_t getConnectionParams(CAENDPP_ConnectionParams_t *connParam) {
    //Some Examples:

    // The following is for direct USB connection
    //connParam.LinkType = CAENDPP_USB;
    //connParam.LinkNum = 0;              // This defines the USB port to use and must increase with board number; ex: for board 3 must be 3
    //connParam.ConetNode = 0;            // This MUST be 0
    //connParam.VMEBaseAddress = 0x0;     // For direct connection the address must be 0

    // The following is for USB connection through VME Bridge
    //connParam.LinkType = CAENDPP_USB;
    //connParam.LinkNum = 0;                    // This defines the USB port to use and must increase with bridge number
    //connParam.ConetNode = 0;                  // This MUST be 0
    //connParam.VMEBaseAddress = 0xFFFFFFFF;    // This must be the VME base address of the board you want to access
    
    // The following is for direct OPTLINK connection
    //connParam.LinkType = CAENDPP_PCI_OpticalLink;
    //connParam.LinkNum = 0;            // This defines the PCI(e) link to use
    //connParam.ConetNode = 0;          // This must increase with board number in dasy chain
    //connParam.VMEBaseAddress = 0x0;   // For direct connection the address must be 0
    
    // The following is for OPTLINK connection through VME Bridge
    //connParam.LinkType = CAENDPP_PCI_OpticalLink;
    //connParam.LinkNum = 0;                    // This defines the PCI(e) link to use
    //connParam.ConetNode = 0;                  // This must increase with bridges number in dasy chain
    //connParam.VMEBaseAddress = 0xFFFFFFFF;    // This must be the VME base address of the board you want to access
    
    // NOTE: The above examples doesn't cover the full operative modes,
    // but the most common ones.

    char c;
    CAENDPP_ConnectionType type;
    int32_t defLink;
    int32_t defNode;
    int32_t defAddr;
    char defIP[IP_ADDR_LEN + 1];
    char strInput[100];
    char *inStr;

    printf("Select the Connection Type:\n\n");
    printf("\t1) USB\n");
    printf("\t2) Optical Link\n");
    printf("\t3) ETHERNET\n");
    printf("\t4) Serial\n");
    printf("\t5) Cancel\n");
    c = _getch();
    printf("Enter the parameters and press [ENTER]\n");
    if (c == '1') {
        type = CAENDPP_USB;
        defLink = connectedBrdNum;
        defNode = 0;
        defAddr = 0x0;
        printf("Format: < LINKNUM (dec) > < VME_ADDR (8 - digs hex) > ");
        printf("Default:  %d %#08x\nParameters:", defLink, defAddr);
        inStr = fgets(strInput, sizeof(strInput), stdin);
        if (inStr != NULL)
            sscanf(strInput, "%d %x", &defLink, &defAddr);
    }
    else if (c == '2') {
        type = CAENDPP_PCI_OpticalLink;
        defLink = 0;
        defNode = connectedBrdNum;
        defAddr = 0x0;
        printf("Format: < LINKNUM (dec) > < LINKNODE (dec) > < VME_ADDR (8 - digs hex) > ");
        printf("Default:  %d %d %#08x\nParameters:", defLink, defNode, defAddr);
        inStr = fgets(strInput, sizeof(strInput), stdin);
        if (inStr != NULL)
            sscanf(strInput, "%d %d %x", &defLink, &defNode, &defAddr);
    }
    else if (c == '3') {
        type = CAENDPP_ETH;
        sprintf(defIP, "192.168.1.125");
        printf("Format: < IPADDR > ");
        printf("Default: %s\nParameters:", defIP);
        inStr = fgets(strInput, sizeof(strInput), stdin);
        if (inStr != NULL)
            sscanf(strInput, "%s", defIP);
    }
    else if (c == '4') {
        type = CAENDPP_Serial;
        defLink = connectedBrdNum;
        defNode = 0;
        defAddr = 0x0;
        printf("Format: < PORT (dec) > ");
        printf("Default:  %d\nParameters:", defLink);
        inStr = fgets(strInput, sizeof(strInput), stdin);
        if (inStr != NULL)
            sscanf(strInput, "%d", &defLink);
    }
    else
        return - 1;

    connParam->LinkType = type;
    connParam->LinkNum = defLink;
    connParam->ConetNode = defNode;
    connParam->VMEBaseAddress = defAddr;
    strncpy(connParam->ETHAddress, defIP, IP_ADDR_LEN + 1);

    return 0;
}


/// \fn      int32_t getStopCriteria(CAENDPP_StopCriteria_t *StopCriteria, uint64_t *StopCriteriaValue);
/// \brief   Asks the user to enter Stop Criteria parameters.
/// 
/// \param   [OUT] StopCriteria         : The Stop Criteria type choosen
/// \param   [OUT] StopCriteriaValue    : The Stop Criteria value
/// 
/// \return  0 = Success; negative numbers = User Abort
int32_t getStopCriteria(CAENDPP_StopCriteria_t *StopCriteria, uint64_t *StopCriteriaValue) {
    char c;
    long long unsigned int u64;

    printf("Choose the Stop Criteria for channel %d and press [Enter]:\n", currCh);
    printf("\t0 - Manual\n");
    printf("\t1 - Live Time (seconds)\n");
    printf("\t2 - Real Time (seconds)\n");
    printf("\t3 - Counts Time (counts)\n");
    printf("\t4 - Abort\n");
    c = _getch();
    switch(c) {
    case '0': *StopCriteria = CAENDPP_StopCriteria_Manual; break;
    case '1': *StopCriteria = CAENDPP_StopCriteria_LiveTime; break;
    case '2': *StopCriteria = CAENDPP_StopCriteria_RealTime; break;
    case '3': *StopCriteria = CAENDPP_StopCriteria_Counts; break;
    default: return - 1;
    }
    printf("Type the value to set (in units of the choosen StopCriteria) and press [Enter]: ");
    if(scanf("%llu", &u64) != 1)
        return -2;
    *StopCriteriaValue = u64;

    return 0;
}
