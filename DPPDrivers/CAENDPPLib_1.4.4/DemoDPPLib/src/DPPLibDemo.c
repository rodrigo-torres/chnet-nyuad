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
* \file     DPPLibDemo.c
* \brief    CAEN - Main source of CAENDPPLibrary Demo program
* \author   Francesco Pepe (support.computing@caen.it)
*
******************************************************************************/

#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <math.h> 
#include <CAENDPPLib.h> 
#include <DPPLibDemo.h> 
#include <Functions.h> 
#include <keyb.h> 

/// \fn      int32_t main();
/// \brief   Main Program
int32_t main() {
    int32_t ret = 0;
    
    //  ch is the variable used to identify a board's channel inside 
    //  DPPLibrary.
    //  EXAMPLE: you have N boards with 4 channels each one. Then you
    //  have:
    //          |     |   ch    |
    // -------- |-----|---------|
    //          | ch0 |    0    |
    //          |-----|---------|
    //          | ch1 |    1    |
    //  BOARD 0 |-----|---------|
    //          | ch2 |    2    |
    //          |-----|---------|
    //          | ch3 |    3    |
    // -------- |-----|---------|
    //          | ch0 |    4    |
    //          |-----|---------|
    //          | ch1 |    5    |
    //  BOARD 1 |-----|---------|
    //          | ch2 |    6    |
    //          |-----|---------|
    //          | ch3 |    7    |
    // -------- |-----|---------|
    //     ..................     
    // -------- |-----|---------|
    //          | ch0 | 4*N - 4 |
    //          |-----|---------|
    //          | ch1 | 4*N - 3 |
    //  BRD N-1 |-----|---------|
    //          | ch2 | 4*N - 4 |
    //          |-----|---------|
    //          | ch3 | 4*N - 1 |
    // -------- |-----|---------|
    // NOTE: since the added boards can have different number of channels
    // it is not possible to define a general formula!
    int32_t ch;
    
    // The Record Length is used to specify how many samples you want
    // in Waveform mode when you require a Waveform.
    uint32_t RecLen = 40000;
    
    // Handler to the Library
    int32_t handle;

    // Board Identifier
    int32_t bId[MAX_BOARD_NUMBER], brd;

    // Connection Parameters - Used to connect to the board.
    // They are the same as the ones described in CAENDigitizer.ddl
    // Manual. Refer to it for details.
    CAENDPP_ConnectionParams_t connParam;

    // Board Infos Structure
    CAENDPP_Info_t info[MAX_BOARD_NUMBER];

    // Acquisition Parameters - Used to set the acquisition
    // properties.
    CAENDPP_DgtzParams_t dgtzParams;

    // Acquisition Mode (Histogram or Waveforms)
    CAENDPP_AcqMode_t acqMode;

    // Stop Criteria definition
    CAENDPP_StopCriteria_t StopCriteria;
    uint64_t StopCriteriaValue;

    // HV Channel Configuration
    CAENDPP_HVChannelConfig_t HVConfig;

    // Input Range
    CAENDPP_InputRange_t InRange;

    // Other Variable definition
    uint32_t *h1;
    uint32_t MIN_REQ_EVTS = 100000;
    uint32_t nEvts = 0;
    int32_t quit = 0;
    uint32_t automask;
    int16_t *AT1 = NULL, *AT2 = NULL;
    uint8_t *DT1 = NULL, *DT2 = NULL;
    int32_t d32;
    uint32_t u32;
    uint16_t u16;
    double f64;
    double *pF64;
    uint64_t realTime, deadTime;
    char nextC = 0, chr;
    CAENDPP_PWDownMode_t PWDownMode;
    int32_t SWTriggerEnabled = 0;
    int32_t bFirst, bLast;
    int32_t chFirst, chLast;
    int32_t ContinuousPlot = 0;
    int32_t ContinuousHVMon = 0;
    int32_t histoNBins;
    char codeStr[MAX_ERRMSG_LEN + 1];
    char histoFName[MAX_HISTOFNAME_LEN];
    DPPDemo_Cmd_t cmd;
    
    // Init global variables
    currBrd = - 1;
    currCh = - 1;
    currHisto = - 1;
    currHVBrd = -1;
    currHVCh = -1;
    strcpy(currHVChName, "NONE");
    connectedBrdNum = 0;
    connectedChNum = 0;
    gnuplot = NULL;
    statusMsgPrinted = 1;
    IFMode = IFMODE_ACQ;

    // Init digitizer parameters to default value
    // 
    // NOTE: those prarameters are probably NOT GOOD for you!
    // Refer to the DPP - PHA and CAENDigitizer.dll manuals
    // for details. You can also try to use the AutoSet functionality
    // or to change the parameters run-time
    ret = InitDgtzParams(&dgtzParams);

    // The channel mask is used to set the enabled Acquisition Input Channels
    dgtzParams.ChannelMask = 0x1;
    
    // Set the Acquisition Mode
    //acqMode = CAENDPP_AcqMode_Histogram;    // For Histogram mode (no waveforms)
    acqMode = CAENDPP_AcqMode_Waveform;   // For Oscilloscope mode (waves + histogram)

    // Alloc the histogram to the given number of bins.
    h1 = (uint32_t *)calloc(MAX_HISTO_NBINS, sizeof(uint32_t));
    if (h1 == NULL) {
        ret = - 1;
        goto QuitProgram;
    }

    // Init the library
    ret = CAENDPP_InitLibrary(&handle); // The handle will be used to command the library
    printf("Return Init: %d\n", ret);

    //**********************************************************************//
    //                       BEGIN INTERFACE LOOP                           //
    //**********************************************************************//
    setStatusMessage("System Inited");
    while (!quit) {
        unsigned char c;
        
        //printInterface();
        if (nextC != (char)0) {
            c = nextC;
            nextC = (char)0;
        }
        else {
            printInterface();
            c = (unsigned char)WaitNextChar();
        }

        if (c == '\n')
            continue;
        else printf("\n");

        cmd = getCommad(c);
        switch (cmd) {
        CAENDPP_EnumeratedDevices_t DeviceList;
        int32_t i;

        // ################################################################## //
        // ##       BEGIN OF COMMANDS WHICH USES DPPLIBRARY FUNTIONS       ## //
        // ################################################################## //
        
        // DPPLibrary API used in DPPDemo_Cmd_AddBoard:
        //   - CAENDPP_AddBoard(int32_t handle, CAENDPP_ConnectionParams_t connParams, int32_t *boardId);
        //   - CAENDPP_GetDPPInfo(int32_t handle, int32_t boardId, CAENDPP_Info_t* info);
        case DPPDemo_Cmd_AddBoard:
            // HACK
            ret = getConnectionParams(&connParam);
            if (ret != 0) {
                setStatusMessage("User Abort");
                ret = 0;
                break;
            }
            
            // Add board to the Library
            ret = CAENDPP_AddBoard(handle, connParam, &bId[connectedBrdNum]);
            if (ret != CAENDPP_RetCode_Ok) {
                setStatusMessage("Error adding board with selected connection parameters: %s", decodeError(codeStr, ret));
                ret = CAENDPP_RetCode_Ok;
                break;
            }
            
            // Get Board Info
            ret = CAENDPP_GetDPPInfo(handle, bId[connectedBrdNum], &info[connectedBrdNum]);
            if (ret != CAENDPP_RetCode_Ok) {
                setStatusMessage("Error getting board info: %s", decodeError(codeStr, ret));
                ret = CAENDPP_RetCode_Ok;
                break;
            }
            
            setStatusMessage("Board %s_%u added succesfully", info[connectedBrdNum].ModelName, info[connectedBrdNum].SerialNumber);

            // Increase globals
            connectedChNum += info[connectedBrdNum].Channels;
            connectedBrdNum++;
            break; // DPPDemo_Cmd_AddBoard

        // DPPLibrary API used in DPPDemo_Cmd_SetStopCriteria:
        //   - CAENDPP_SetStopCriteria(int32_t handle, int32_t channel, CAENDPP_StopCriteria_t stopCrit, uint64_t value);
        case DPPDemo_Cmd_SetStopCriteria: // Set Stop Criteria
            ret = getStopCriteria(&StopCriteria, &StopCriteriaValue);
            if (ret != 0) {
                setStatusMessage("User Abort");
                ret = CAENDPP_RetCode_Ok;
                break;
            }
            ret = CAENDPP_SetStopCriteria(handle, currCh, StopCriteria, StopCriteriaValue);
            if (ret != CAENDPP_RetCode_Ok)
                setStatusMessage("Can't set Stop Criteria for channel %d: %s", currCh, decodeError(codeStr, ret));
            else
                setStatusMessage("Stop Criteria successfully set");
            
            break; // DPPDemo_Cmd_SetStopCriteria

        // DPPLibrary API used in DPPDemo_Cmd_GetStopCriteria:
        //   - CAENDPP_GetStopCriteria(int32_t handle, int32_t channel, CAENDPP_StopCriteria_t *stopCrit, uint64_t *value);
        case DPPDemo_Cmd_GetStopCriteria: // Get Stop Criteria
            ret = CAENDPP_GetStopCriteria(handle, currCh, &StopCriteria, &StopCriteriaValue);
            if (ret != CAENDPP_RetCode_Ok)
                setStatusMessage("Can't get Stop Criteria for channel %d: %s", currCh, decodeError(codeStr, ret));
            else {
                char SCstring[15];
                char units[10];
                if (StopCriteria == CAENDPP_StopCriteria_Manual) {
                    strcpy(SCstring, "MANUAL");
                    strcpy(units, "I");
                }
                else if (StopCriteria == CAENDPP_StopCriteria_LiveTime) {
                    strcpy(SCstring, "LIVE TIME");
                    strcpy(units, "seconds");
                }
                if (StopCriteria == CAENDPP_StopCriteria_RealTime) {
                    strcpy(SCstring, "REAL TIME");
                    strcpy(units, "seconds");
                }
                if (StopCriteria == CAENDPP_StopCriteria_Counts) {
                    strcpy(SCstring, "COUNTS");
                    strcpy(units, "counts");
                }
                setStatusMessage("Stop Criteria for channel %d are: %s - %llu %s", currCh, SCstring, StopCriteriaValue, units);
            }
            break; // DPPDemo_Cmd_GetStopCriteria
        
        // DPPLibrary API used in DPPDemo_Cmd_ClearHisto:
        //   - CAENDPP_ClearHistogram(int32_t handle, int32_t channel, int32_t histoIndex);
        case DPPDemo_Cmd_ClearHisto:
            printf("Will clear histogram %d:%d\n", currCh, currHisto);
            printf("Press [1] to continue or other key to Abort: ");
            if (_getch() != '1') {
                setStatusMessage("User Abort");
                break;
            }
            ret = CAENDPP_ClearHistogram(handle, currCh, currHisto);
            if (ret != CAENDPP_RetCode_Ok)
                setStatusMessage("Can't clear Histogram %d%d: %s", currCh, currHisto, decodeError(codeStr, ret));
            else
                setStatusMessage("Histogram successfully cleared");
            break; // DPPDemo_Cmd_ClearHisto
        
        // DPPLibrary API used in DPPDemo_Cmd_Configure:
        //   - CAENDPP_SetBoardConfiguration(int32_t handle, int32_t boardId, CAENDPP_AcqMode_t acqMode, CAENDPP_DgtzParams_t dgtz_params);
        case DPPDemo_Cmd_Configure: // Configure Boards
            // Write the configuration to the boards
            ret = CAENDPP_RetCode_Ok;
            getBrdRange(&bFirst, &bLast);
            if (bFirst == bLast) { // No board Added
                setStatusMessage("Can't configure boards: No board Added");
                break;
            }
            for (brd = bFirst; brd < bLast; brd++) {
                ret = CAENDPP_SetBoardConfiguration(handle, bId[brd], acqMode, dgtzParams);
                if (ret != CAENDPP_RetCode_Ok)
                    setStatusMessage("Can't configure board %s_%u: %s", info[brd].ModelName, info[brd].SerialNumber, decodeError(codeStr, ret));
            }

            if (ret == CAENDPP_RetCode_Ok)
                setStatusMessage("Boards Configured");
            break; // DPPDemo_Cmd_Configure
        
        // DPPLibrary API used in DPPDemo_Cmd_ResetAllHistos:
        //   - CAENDPP_ResetAllHistograms(int32_t handle, int32_t channel);
        case DPPDemo_Cmd_ResetAllHistos: // Reset all histograms
            // Reset all histograms contents, stats, etc.
            ret = CAENDPP_ResetAllHistograms(handle, currCh);
            if (ret != CAENDPP_RetCode_Ok)
                setStatusMessage("Error resetting all histograms: %s", decodeError(codeStr, ret));
            else
                setStatusMessage("All Histograms Resetted");
            break;
        
        // DPPLibrary API used in DPPDemo_Cmd_StartAcq:
        //   - CAENDPP_StartAcquisition(int32_t handle, int32_t channel);
        case DPPDemo_Cmd_StartAcq: // Start Acquisition for all boards
            ret = CAENDPP_StartAcquisition(handle, currCh);
            if (ret != CAENDPP_RetCode_Ok)
                setStatusMessage("Error Starting Acquisition for channel %d: %s", currCh, decodeError(codeStr, ret));
            else {
                setStatusMessage("Acquisition Started for channel %d", currCh);
                if (ContinuousPlot == 1)
                    setContinuousPlotPeriod(PLOT_PERIOD_MS);
            }
            break; // DPPDemo_Cmd_ResetAllHistos
        
        // DPPLibrary API used in DPPDemo_Cmd_StopAcq:
        //   - CAENDPP_StopAcquisition(int32_t handle, int32_t channel);
        case DPPDemo_Cmd_StopAcq: // Stop Acquisition for all boards
            ret = CAENDPP_StopAcquisition(handle, currCh);
            if (ret != CAENDPP_RetCode_Ok)
                setStatusMessage("Error Stopping Acquisition for channel %d: %s", currCh, decodeError(codeStr, ret));
            else
                setStatusMessage("Acquisition Stopped for channel %d", currCh);
            break; // DPPDemo_Cmd_StopAcq
        
        // DPPLibrary API used in DPPDemo_Cmd_SwTrgToggle:
        //   - CAENDPP_DisableSwTriggers(int32_t handle);
        //   - CAENDPP_EnableSwTriggers(int32_t handle);
        case DPPDemo_Cmd_SwTrgToggle: // Toggle SW Trigger
            if (SWTriggerEnabled == 1) {
                SWTriggerEnabled = 0;
                ret = CAENDPP_DisableSwTriggers(handle);
                if (ret == CAENDPP_RetCode_Ok)
                    setStatusMessage("Software Trigger disabled");
            } else {
                SWTriggerEnabled = 1;
                ret = CAENDPP_EnableSwTriggers(handle);
                if (ret == CAENDPP_RetCode_Ok)
                    setStatusMessage("Software Trigger enabled");
            }
            if (ret != CAENDPP_RetCode_Ok)
                setStatusMessage("Error Toggling Software Trigger for all boards: %s", decodeError(codeStr, ret));
            break; // DPPDemo_Cmd_SwTrgToggle
        
        // DPPLibrary API used in DPPDemo_Cmd_SwitchHisto:
        //   - CAENDPP_ForceNewHistogram(int32_t handle, int32_t channel);
        case DPPDemo_Cmd_SwitchHisto: // Switch to next histo for all channels
            getChRange(&chFirst, &chLast);
            for (ch = chFirst; ch < chLast; ch++) {
                if (isChannelDisabled(handle, ch))
                    continue;

                ret = CAENDPP_ForceNewHistogram(handle, ch);
                if (ret != CAENDPP_RetCode_Ok) {
                    setStatusMessage("WARNING: Can't switch Next Histo for ch %d: %s", ch, decodeError(codeStr, ret));
                    continue;
                }
            }
            if (ret == CAENDPP_RetCode_Ok)
                setStatusMessage("Histogram successfully switched for channel %d", currCh);
            break; // DPPDemo_Cmd_SwitchHisto
        
        // DPPLibrary API used in DPPDemo_Cmd_SaveHisto:
        //   - CAENDPP_GetCurrentHistogram(int32_t handle, int32_t channel, void *histo, uint32_t *counts, uint64_t *realTime_ns, uint64_t *deadTime_ns, CAENDPP_AcqStatus_t *acqStatus);
        //   - CAENDPP_GetHistogramSize(int32_t handle, int32_t channel, int32_t histoIndex, int32_t *size);
        case DPPDemo_Cmd_SaveHisto:
            ret = 0;
            getChRange(&chFirst, &chLast);
            for (ch = chFirst; ch < chLast; ch++) {
                CAENDPP_AcqStatus_t acqStatus;

                if (isChannelDisabled(handle, ch))
                    continue;

                // Get The Histogram
                ret = CAENDPP_GetCurrentHistogram(handle, ch, h1, &u32, &realTime, &deadTime, &acqStatus);
                if (ret != CAENDPP_RetCode_Ok) {
                    setStatusMessage("ERROR: Can't get current Histogram for ch %d: %s\n", ch, decodeError(codeStr, ret));
                    continue;
                }
                // Now you can do what you want with that histogram. We just print it to file
                ret = CAENDPP_GetHistogramSize(handle, ch, HISTO_IDX_CURRENT, &histoNBins);
                if (ret != CAENDPP_RetCode_Ok) {
                    setStatusMessage("ERROR: Can't get current Histogram size for ch %d: %s\n", ch, decodeError(codeStr, ret));
                    continue;
                }
                sprintf(histoFName, "histo_ch%d.txt", ch);
                saveHistogram(histoFName, h1, histoNBins, realTime, deadTime);
                setStatusMessage("CH%d", ch);
                setStatusMessage(" RT\t= %.3f s", realTime / 1000000000.0);
                setStatusMessage(" DT\t= %.3f ms", deadTime / 1000000.0);
                setStatusMessage(" Good\t= %u CNT", u32);
                setStatusMessage(" Tot.\t= %u CNT", (uint64_t)(u32 * (1.0 + (double)deadTime / (double)(realTime))));
                setStatusMessage(" ICR\t= %.3f CNT/s", (realTime - deadTime) > 0 ? (double)u32 / (double)((realTime - deadTime) / 1000000000.0) : 0);
            }
            if (ret == CAENDPP_RetCode_Ok)
                setStatusMessage("All Histograms saved successfully.");
            break; // DPPDemo_Cmd_SaveHisto
        
        // DPPLibrary API used in DPPDemo_Cmd_HistoSinglePlot:
        //   - CAENDPP_GetCurrentHistogram(int32_t handle, int32_t channel, void *histo, uint32_t *counts, uint64_t *realTime_ns, uint64_t *deadTime_ns, CAENDPP_AcqStatus_t *acqStatus);
        //   - CAENDPP_GetHistogramSize(int32_t handle, int32_t channel, int32_t histoIndex, int32_t *size);
        case DPPDemo_Cmd_HistoSinglePlot:
            if (currCh < 0) {
                setStatusMessage("Cannot plot histogram for the given channel.");
                break;
            }
            else {
                CAENDPP_AcqStatus_t acqStatus;
                char title[100];
                
                ret = CAENDPP_GetCurrentHistogram(handle, currCh, h1, &u32, &realTime, &deadTime, &acqStatus);
                if (ret != CAENDPP_RetCode_Ok) {
                    setStatusMessage("Cannot get current histogram for the channel %d: %s", currCh, decodeError(codeStr, ret));
                    break;
                }
                // Now you can do what you want with that histogram. We just print it to file
                ret = CAENDPP_GetHistogramSize(handle, currCh, HISTO_IDX_CURRENT, &histoNBins);
                if (ret != CAENDPP_RetCode_Ok) {
                    setStatusMessage("ERROR: Can't get current Histogram size for channel %d: %s", currCh, decodeError(codeStr, ret));
                    break;
                }
                sprintf(title, "Histogram Ch. %d", currCh);
                plotHisto(title, h1, histoNBins, u32, realTime / 1000000000.0, deadTime / 1000000000.0);
                if (acqStatus != CAENDPP_AcqStatus_Stopped && ContinuousPlot == 1)
                    setContinuousPlotPeriod(PLOT_PERIOD_MS);
                else // Avoid Replot by disabling automatic character issuing
                    setContinuousPlotPeriod(0);
            }
            break; // DPPDemo_Cmd_HistoSinglePlot

        // DPPLibrary API used in DPPDemo_Cmd_SaveWave/DPPDemo_Cmd_WaveSinglePlot:
        //   - CAENDPP_GetBoardConfiguration(int32_t handle, int32_t boardId, CAENDPP_AcqMode_t *acqMode, CAENDPP_DgtzParams_t *dgtz_params);
        //   - CAENDPP_GetWaveform(int32_t handle, int32_t channel, int16_t *analogTrace1, int16_t *analogTrace2, uint8_t *digitalTrace1, uint8_t *digitalTrace2, uint32_t *ns);
        case DPPDemo_Cmd_SaveWave:
        case DPPDemo_Cmd_WaveSinglePlot: // Get a Waveform
            getBrdRange(&bFirst, &bLast);
            for (brd = bFirst; brd < bLast; brd++) {
                ret = CAENDPP_GetBoardConfiguration(handle, bId[brd], &acqMode, &dgtzParams);
                if (ret != CAENDPP_RetCode_Ok) {
                    setStatusMessage("Can't get board configuration: %s", decodeError(codeStr, ret));
                    continue;
                }
                AT1 = (int16_t *)realloc(AT1, dgtzParams.WFParams.recordLength*sizeof(int16_t));
                AT2 = (int16_t *)realloc(AT2, dgtzParams.WFParams.recordLength*sizeof(int16_t));
                DT1 = (uint8_t *)realloc(DT1, dgtzParams.WFParams.recordLength*sizeof(uint8_t));
                DT2 = (uint8_t *)realloc(DT2, dgtzParams.WFParams.recordLength*sizeof(uint8_t));
                if ( AT1 == NULL || AT2 == NULL || DT1 == NULL || DT2 == NULL ) {
                    setStatusMessage("Can't allocate memory for waveforms");
                    continue;
                }
            }
            getChRange(&chFirst, &chLast);
            for (ch = chFirst; ch < chLast; ch++) {
                uint32_t ns = 0;
                double tsample;
                void *pTraces[MAX_NUM_TRACES];
                DPPDemo_TraceType_t types[MAX_NUM_TRACES];

                if (isChannelDisabled(handle, ch))
                    continue;

                // Get the waveform using Oscilloscope-like AUTO mode.
                if ((ret = CAENDPP_GetWaveform(handle, ch, 1, AT1, AT2, DT1, DT2, &ns, &tsample)) != CAENDPP_RetCode_Ok) {
                    setStatusMessage("Can't get Waveform for ch %d: %s", ch, decodeError(codeStr, ret));
                    continue;
                }
                // DO HERE WHAT YOU WANT WITH THE WAVEFORM
                if (ns > 0) {
                    char filename[30];
                    
                    // Setup Traces Pointers
                    pTraces[0] = AT1;
                    pTraces[1] = AT2;
                    pTraces[2] = DT1;
                    pTraces[3] = DT2;
                    
                    // Setup Traces Types
                    types[0] = DPPDemo_TraceType_Analog1;
                    types[1] = DPPDemo_TraceType_Analog2;
                    types[2] = DPPDemo_TraceType_Digital1;
                    types[3] = DPPDemo_TraceType_Digital2;

                    if (cmd == DPPDemo_Cmd_SaveWave) {
                        // Setup Filename
                        sprintf(filename, "waveform_ch%d.txt", ch);

                        // save the waveform
                        saveWaveform(filename, MAX_NUM_TRACES, pTraces, types, ns);
                    }
                    else if (cmd == DPPDemo_Cmd_WaveSinglePlot) {
                        int32_t traces[MAX_NUM_TRACES];
                        if ((ret = CAENDPP_GetBoardConfiguration(handle, bId[currBrd], &acqMode, &dgtzParams)) != CAENDPP_RetCode_Ok) {
                            setStatusMessage("Can't plot waveform: %s", decodeError(codeStr, ret));
                            break;
                        }
                        traces[0] = dgtzParams.WFParams.vp1;
                        traces[1] = dgtzParams.WFParams.vp2;
                        traces[2] = dgtzParams.WFParams.dp1;
                        traces[3] = dgtzParams.WFParams.dp2;
                        plotWaveform(MAX_NUM_TRACES, traces, pTraces, types, ns);
                    }
                }
                else // If ns == 0, we didn't had a waveform in this read
                    setStatusMessage("No Waveform found for channel %d", ch);
            }
            break; // DPPDemo_Cmd_SaveWave/DPPDemo_Cmd_WaveSinglePlot
        
        // DPPLibrary API used in DPPDemo_Cmd_Autoset:
        //   - CAENDPP_GetBoardConfiguration(int32_t handle, int32_t boardId, CAENDPP_AcqMode_t *acqMode, CAENDPP_DgtzParams_t *dgtz_params);
        //   - CAENDPP_GuessBoardParameters(int32_t handle, int32_t boardId, uint32_t channelMask, CAENDPP_DgtzParams_t *Params);
        case DPPDemo_Cmd_Autoset: // Try Parameters Autoset
            getBrdRange(&bFirst, &bLast);
            if (bFirst == bLast) { // No board Added
                setStatusMessage("Can't Autoset Board: No board Added");
                break;
            }
            printf("Write the channelmask to search parameters for (hex): ");
            if (scanf("%x", &automask) == 1) {
                for (brd = bFirst; brd < bLast; brd++) {
                    CAENDPP_DgtzParams_t autosetDgtzParams;
                    CAENDPP_GuessConfigStatus_t guessStatus;
                    uint32_t succMask;
                    ret = CAENDPP_GetBoardConfiguration(handle, bId[brd], &acqMode, &autosetDgtzParams);
                    if (ret != CAENDPP_RetCode_Ok) {
                        setStatusMessage("Can't get board configuration: %s", decodeError(codeStr, ret));
                        continue;
                    }
                    DefineAutosetParameters(&autosetDgtzParams); // This instructs autoset to search all parameters
                    ret = CAENDPP_StartBoardParametersGuess(handle, bId[brd], automask, &autosetDgtzParams);
                    if (ret != CAENDPP_RetCode_Ok)
                        setStatusMessage("Can't Find Acquisition Parameters Automatically. Please Reconfigure the board.");
                    else {
                        int32_t aborted = 0;
                        printf("Starting parameters Autoset. This may take a lot of time. Press '%c' to abort.\n", ABORT_AUTOSET_CHAR);
                        ret = CAENDPP_GetBoardParametersGuessStatus(handle, bId[brd], &guessStatus);
                        while (ret == CAENDPP_RetCode_Ok && guessStatus != CAENDPP_GuessConfigStatus_Ready) {
                            char abortChar;
                            if (_kbhit()) {
                                abortChar = _getch();
                                if (abortChar == ABORT_AUTOSET_CHAR) {
                                    ret = CAENDPP_StopBoardParametersGuess(handle, bId[brd]);
                                    if (ret == CAENDPP_RetCode_Ok) {
                                        aborted = 1;
                                        setStatusMessage("Autoset User Abort. Please Reconfigure the board.");
                                        break;
                                    }
                                    else
                                        printf("Error Aborting Autoset: %\n", decodeError(codeStr, ret));
                                }
                            }
                            Sleep(100);
                            ret = CAENDPP_GetBoardParametersGuessStatus(handle, bId[brd], &guessStatus);
                        }
                        if (aborted == 0) {
                            if (ret == CAENDPP_RetCode_Ok)
                                ret = CAENDPP_GetBoardParametersGuessResult(handle, bId[brd], &autosetDgtzParams, &succMask);
                            if (ret == CAENDPP_RetCode_Ok) {
                                // Adjust Here the parameters you want to set manually
                                dgtzParams = autosetDgtzParams;
                                nextC = 'C'; // Reconfigure the board with new parameters
                            }
                            else 
                                setStatusMessage("Autoset failed: %s", decodeError(codeStr, ret));
                        }
                    }
                }
            }
            else
                setStatusMessage("Invalid Mask");
            break; // DPPDemo_Cmd_Autoset
        
        // DPPLibrary API used in DPPDemo_Cmd_SetBoardConfig/DPPDemo_Cmd_SetChannelConfig:
        //   - CAENDPP_GetBoardConfiguration(int32_t handle, int32_t boardId, CAENDPP_AcqMode_t *acqMode, CAENDPP_DgtzParams_t *dgtz_params);
        //   - CAENDPP_SetBoardConfiguration(int32_t handle, int32_t boardId, CAENDPP_AcqMode_t acqMode, CAENDPP_DgtzParams_t dgtz_params);
        case DPPDemo_Cmd_SetBoardConfig:
        case DPPDemo_Cmd_SetChannelConfig:
            // Calulate board and channel indexes
            if (cmd == DPPDemo_Cmd_SetBoardConfig) {
                if (currBrd < 0 || currBrd >= connectedBrdNum) {
                    setStatusMessage("Invalid Board Index");
                    break;
                }
                brd = currBrd;
            }
            else if (cmd == DPPDemo_Cmd_SetChannelConfig) {
                if (currCh < 0 || currCh >= connectedChNum) {
                    setStatusMessage("Invalid Channel Index");
                    break;
                }
                // Find the board and channel idx corresponding to currCh
                brd = 0;
                ch = info[brd].Channels;
                while (ch < currCh) {
                    ch += info[brd].Channels;
                    brd++;
                }
                ch = info[brd].Channels - (ch - currCh);
            }
            else {
                setStatusMessage("Unmanaged Command!");
                break;
            }

            if ((ret = CAENDPP_GetBoardConfiguration(handle, bId[brd], &acqMode, &dgtzParams)) != CAENDPP_RetCode_Ok) {
                setStatusMessage("Can't get Board Configuration: %s", decodeError(codeStr, ret));
                break;
            }

            if (cmd == DPPDemo_Cmd_SetBoardConfig) {
                if (GetModifiedBoardParameters(&acqMode, &dgtzParams, &info[brd]) != 0) {
                    setStatusMessage("User Abort");
                    break;
                }
            }
            else {
                if (GetModifiedChannelParameters(ch, &dgtzParams) != 0) {
                    setStatusMessage("User Abort");
                    break;
                }
            }
            if ((ret = CAENDPP_SetBoardConfiguration(handle, bId[brd], acqMode, dgtzParams)) != CAENDPP_RetCode_Ok) {
                setStatusMessage("Can't set Board Configuration: %s", decodeError(codeStr, ret));
                break;
            }
            setStatusMessage("Board Configured with new parameters");
            break; // DPPDemo_Cmd_SetBoardConfig
                   // DPPDemo_Cmd_SetChannelConfig
        
        // DPPLibrary API used in DPPDemo_Cmd_SetInputRange:
        //   - CAENDPP_SetInputRange(int32_t handle, int32_t channel, CAENDPP_InputRange_t iputLevel);
        case DPPDemo_Cmd_SetInputRange:
            printf("Select new Input Range for channel %d\n\n", currCh);
            printf("  1) 9.5 Vpp\n");
            printf("  2) 3.7 Vpp\n");
            printf("  3) 1.4 Vpp\n");
            printf("  4) 0.6 Vpp\n");
            printf("  5) Abort\n");
            chr = _getch();
            if (chr == '1')
                InRange = CAENDPP_InputRange_9_5Vpp;
            else if (chr == '2')
                InRange = CAENDPP_InputRange_3_7Vpp;
            else if (chr == '3')
                InRange = CAENDPP_InputRange_1_4Vpp;
            else if (chr == '4')
                InRange = CAENDPP_InputRange_0_6Vpp;
            else {
                setStatusMessage("User Abort");
                break;
            }
            if ((ret = CAENDPP_SetInputRange(handle, currCh, InRange)) != CAENDPP_RetCode_Ok) {
                setStatusMessage("Can't set Input Range for Channel %d: %s", currCh, decodeError(codeStr, ret));
                break;
            }
            setStatusMessage("Input Range of Channel %d successfully set", currCh);
            break;

        // DPPLibrary API used in DPPDemo_Cmd_SetCurrentHisto:
        //   - CAENDPP_SetCurrentHistogramIndex(int32_t handle, int32_t channel, int32_t histoIndex);
        case DPPDemo_Cmd_SetCurrentHisto:
            ret = CAENDPP_SetCurrentHistogramIndex(handle, currCh, currHisto);
            if (ret != CAENDPP_RetCode_Ok)
                setStatusMessage("Cannot set Current Histogram index to %d for channel %d : %s", currHisto, currCh, decodeError(codeStr, ret));
            else
                setStatusMessage("Current Histogram Successfully set");
            break; // DPPDemo_Cmd_SetCurrentHisto
        
        // DPPLibrary API used in DPPDemo_Cmd_GetCurrentHisto:
        //   - CAENDPP_GetCurrentHistogramIndex(int32_t handle, int32_t channel, int32_t *histoIndex);
        case DPPDemo_Cmd_GetCurrentHisto:
            ret = CAENDPP_GetCurrentHistogramIndex(handle, currCh, &d32);
            if (ret != CAENDPP_RetCode_Ok)
                setStatusMessage("Cannot get Current Histogram index for channel %d : %s", currCh, decodeError(codeStr, ret));
            else
                setStatusMessage("Current Histogram Index of channel %d is %d", currCh, d32);
            break; // DPPDemo_Cmd_GetCurrentHisto
        
        // DPPLibrary API used in DPPDemo_Cmd_GetHistoNum:
        //   - CAENDPP_GetTotalNumberOfHistograms(int32_t handle, int32_t channel, int32_t *numHisto);
        case DPPDemo_Cmd_GetHistoNum:
            if (currCh < 0) {
                setStatusMessage("Unsupported channel number %d", currCh);
                break;
            }
            ret = CAENDPP_GetTotalNumberOfHistograms(handle, currCh, &d32);
            if (ret != CAENDPP_RetCode_Ok)
                setStatusMessage("Cannot get Number of Histograms for channel %d : %s", currCh, decodeError(codeStr, ret));
            else
                setStatusMessage("Number of Histograms of channel %d is %d", currCh, d32);
            break; // DPPDemo_Cmd_GetHistoNum
        
        // DPPLibrary API used in DPPDemo_Cmd_AddHisto:
        //   - CAENDPP_AddHistogram(int32_t handle, int32_t channel, int32_t size);
        case DPPDemo_Cmd_AddHisto:
            printf("Enter the size of the histogram to add and press [Enter]:\n");
            printf("Size (pow of 2 <= 16384): ");
            if (scanf("%u", &u32) == 1) {
                ret = CAENDPP_AddHistogram(handle, currCh, (int32_t)u32);
                if (ret != CAENDPP_RetCode_Ok)
                    setStatusMessage("Cannot add Histogram for channel %d : %s", currCh, decodeError(codeStr, ret));
                else
                    setStatusMessage("Histogram of size %u successfully added for channel %d", u32, currCh);
            }
            else
                setStatusMessage("Invalid Size");
            break; // DPPDemo_Cmd_AddHisto
        
        // DPPLibrary API used in DPPDemo_Cmd_SetHisto:
        //   - CAENDPP_SetHistogram(int32_t handle, int32_t channel, int32_t histoIndex, uint64_t realTime_ns, uint64_t deadTime_ns, uint32_t nbins, uint32_t *histo);
        case DPPDemo_Cmd_SetHisto:
            printf("Enter the filename of the histogram file to load (\"Q\" to Abort): ");
            if (scanf("%s", histoFName) == 1) {
                if (strcmp(histoFName, "Q") == 0) {
                    setStatusMessage("User Abort");
                    break;
                }
                if ((ret = loadHistogram(histoFName, h1, &histoNBins, &realTime, &deadTime)) != 0) {
                    setStatusMessage("Error %d loading histogram from %s", ret, histoFName);
                    break;
                }
                if ((ret = CAENDPP_SetHistogram(handle, currCh, currHisto, realTime, deadTime, histoNBins, h1)) != CAENDPP_RetCode_Ok) {
                    setStatusMessage("Cannot set histogram %d:%d: %s", currCh, currHisto, decodeError(codeStr, ret));
                    break;
                }
                setStatusMessage("Histogram %d:%d succesfully set from %s", currCh, currHisto, histoFName);
            }
            else
                setStatusMessage("Invalid Filename");
            break;

        // DPPLibrary API used in DPPDemo_Cmd_SetHistoSize:
        //   - CAENDPP_SetHistogramSize(int32_t handle, int32_t channel, int32_t histoIndex, int32_t size);
        case DPPDemo_Cmd_SetHistoSize:
            printf("WARNING: if you change a histogram size, the histogram will be cleared!\n");
            printf("Enter the size of the histogram to add and press [Enter] (0 = Abort):\n");
            printf("Size (pow of 2 <= 16384): ");
            if (scanf("%u", &u32) == 1) {
                if (u32 == 0) {
                    setStatusMessage("User Abort");
                    break;
                }
                ret = CAENDPP_SetHistogramSize(handle, currCh, currHisto, (int32_t)u32);
                if (ret != CAENDPP_RetCode_Ok)
                    setStatusMessage("Cannot set size of Histogram %d for channel %d : %s", currHisto, currCh, decodeError(codeStr, ret));
                else
                    setStatusMessage("Size of Histogram %d for channel %d successfully set to %u", currHisto, currCh, u32);
            }
            else
                setStatusMessage("Invalid Size");
            break; // DPPDemo_Cmd_SetHistoSize
        
        // DPPLibrary API used in DPPDemo_Cmd_GetHistoStatus:
        //   - CAENDPP_GetHistogramStatus(int32_t handle, int32_t channel, int32_t histoIndex, int32_t *completed);
        case DPPDemo_Cmd_GetHistoStatus:
            ret = CAENDPP_GetHistogramStatus(handle, currCh, currHisto, &d32);
            if (ret != CAENDPP_RetCode_Ok)
                setStatusMessage("Cannot get Status of Histogram %d for channel %d : %s", currHisto, currCh, decodeError(codeStr, ret));
            else
                setStatusMessage("Status of Histogram %d:%d is %s", currCh, currHisto, d32 == 0 ? "NOT COMPLETED" : "COMPLETED");
            break; // DPPDemo_Cmd_GetHistoStatus
        
        // DPPLibrary API used in DPPDemo_Cmd_SetHistoStatus:
        //   - CAENDPP_SetHistogramStatus(int32_t handle, int32_t channel, int32_t histoIndex, int32_t completed);
        case DPPDemo_Cmd_SetHistoStatus:
            printf("Choose the status of Histogram %d:%d\n", currCh, currHisto);
            printf("\n\t1) NOT COMPLETED\n");
            printf("\t2) COMPLETED\n");
            printf("\t3) Abort\n");
            chr = _getch();
            if (chr == '1')
                d32 = 0;
            else if (chr == '2')
                d32 = 1;
            else {
                setStatusMessage("User Abort");
                break;
            }
            ret = CAENDPP_SetHistogramStatus(handle, currCh, currHisto, d32);
            if (ret != CAENDPP_RetCode_Ok)
                setStatusMessage("Cannot set Status of Histogram %d:%d : %s", currCh, currHisto, decodeError(codeStr, ret));
            else
                setStatusMessage("Status of Histogram %d:%d successfully set to %s", currCh, currHisto, d32 == 0 ? "NOT COMPLETED" : "COMPLETED");
            break; // DPPDemo_Cmd_SetHistoStatus
        
        // DPPLibrary API used in DPPDemo_Cmd_SingleHVMonitor:
        //   - CAENDPP_ReadHVChannelMonitoring(int32_t handle, int32_t bId, int32_t ch, double *VMon, double *IMon);
        case DPPDemo_Cmd_SingleHVMonitor:
            ch = 0;
            for (brd = 0; brd < connectedBrdNum; brd++) {
                int32_t chmax = ch + info[brd].HVChannels, b_ch = 0;
                setStatusMessage("%s_%u:", info[brd].ModelName, info[brd].SerialNumber);
                for (; ch < chmax; ch++) {
                    double VMon, IMon;
                    char msg[MAX_HVSTATUS_LENGTH];
                    int32_t err = 0;

                    if ((ret = CAENDPP_ReadHVChannelMonitoring(handle, bId[brd], ch, &VMon, &IMon)) != CAENDPP_RetCode_Ok)
                        setStatusMessage("\tHV%d: ERR HVMon: %s", b_ch, decodeError(codeStr, ret));
                    else if ((ret = CAENDPP_GetHVChannelStatus(handle, bId[brd], ch, &u16)) != CAENDPP_RetCode_Ok)
                        setStatusMessage("\tHV%d: ERR HVStat: %s", b_ch, decodeError(codeStr, ret));
                    else if ((ret = CAENDPP_GetHVStatusString(u16, msg)) != CAENDPP_RetCode_Ok)
                        setStatusMessage("\tHV%d: ERR HVStr: %s", b_ch, decodeError(codeStr, ret));
                    else
                        setStatusMessage("\tHV%d: VMon=%.2f V, IMon=%.3f uA, St=%s", b_ch, VMon, IMon, msg);
                    b_ch++;
                }
            }
            break; // DPPDemo_Cmd_SingleHVMonitor
        
        // DPPLibrary API used in DPPDemo_Cmd_GetHVConfig:
        //   - CAENDPP_GetHVChannelConfiguration(int32_t handle, int32_t bId, int32_t ch, CAENDPP_HVChannelConfig_t *config);
        case DPPDemo_Cmd_GetHVConfig:
            if ((ret = CAENDPP_GetHVChannelConfiguration(handle, bId[currHVBrd], currHVCh, &HVConfig)) != CAENDPP_RetCode_Ok) {
                setStatusMessage("Cannot Get HV Configuration: %s", decodeError(codeStr, ret));
                break;
            }
            setStatusMessage("  VSet\t\t= %.2f V", HVConfig.VSet);
            setStatusMessage("  ISet\t\t= %.3f uA", HVConfig.ISet);
            setStatusMessage("  VMax\t\t= %.0f V", HVConfig.VMax);
            setStatusMessage("  RampUp\t= %.0f V/s", HVConfig.RampUp);
            setStatusMessage("  RampDown\t= %.0f V/s", HVConfig.RampDown);
            setStatusMessage("  PWDownMode\t= %s", HVConfig.PWDownMode == CAENDPP_PWDownMode_Ramp ? "RAMP" : "KILL");
            break; // DPPDemo_Cmd_GetHVConfig
        
        // DPPLibrary API used in DPPDemo_Cmd_SetHVConfig:
        //   - CAENDPP_GetHVChannelConfiguration(int32_t handle, int32_t bId, int32_t ch, CAENDPP_HVChannelConfig_t *config);
        //   - CAENDPP_SetHVChannelConfiguration(int32_t handle, int32_t bId, int32_t ch, CAENDPP_HVChannelConfig_t config);
        case DPPDemo_Cmd_SetHVConfig:
            // NOTE: This command is actually not connected to any
            // control character. This point is reached only through
            // 'goto SetHVConfiguration' calls.
SetHVConfiguration:
            // Get the current HV Configuration
            if ((ret = CAENDPP_GetHVChannelConfiguration(handle, bId[currHVBrd], currHVCh, &HVConfig)) != CAENDPP_RetCode_Ok) {
                setStatusMessage("Cannot Get HV Configuration: %s", decodeError(codeStr, ret));
                break;
            }
            // Update the pointed parameter with the new value
            if (cmd == DPPDemo_Cmd_PWDownMode)
                HVConfig.PWDownMode = PWDownMode;
            else
                *pF64 = f64;
            // Write the new configuration
            if((ret = CAENDPP_SetHVChannelConfiguration(handle, bId[currHVBrd], currHVCh, HVConfig)) != CAENDPP_RetCode_Ok) {
                setStatusMessage("Cannot Set HV Configuration: %s", decodeError(codeStr, ret));
                break;
            }
            setStatusMessage("New HV Channel Configuration:");
            nextC = 'c'; // Automatically reprint HV Config
            break; // DPPDemo_Cmd_SetHVConfig
        
        // DPPLibrary API used in DPPDemo_Cmd_VMax:
        //   - CAENDPP_SetHVChannelVMax(int32_t handle, int32_t bId, int32_t ch, double VMax);
        case DPPDemo_Cmd_VMax:
            // Ask for the new Value
            printf("Enter new VMax Value (V): ");
            if (scanf("%lf", &f64) == 1) {
                if (f64 < 0) {
                    setStatusMessage("User Abort");
                    break;
                }
                else if (f64 < HV_VMAX_MIN || f64 > HV_VMAX_MAX) {
                    setStatusMessage("Invalid Value %.2lf: VMax should be between %.2f and %.2f", f64, HV_VMAX_MIN, HV_VMAX_MAX);
                    break;
                }
                // Write the new VMax value
                if((ret = CAENDPP_SetHVChannelVMax(handle, bId[currHVBrd], currHVCh, f64)) != CAENDPP_RetCode_Ok) {
                    setStatusMessage("Cannot Set HV Channel's VMax: %s", decodeError(codeStr, ret));
                    break;
                }
                setStatusMessage("New HV Channel Configuration:");
                nextC = 'c'; // Automatically reprint HV Config
            }
            else
                setStatusMessage("Invalid VMax");
            break; // DPPDemo_Cmd_VMax
        
        // DPPLibrary API used in DPPDemo_Cmd_TurnHVON:
        //   - CAENDPP_SetHVChannelPowerOn(int32_t handle, int32_t bId, int32_t ch, int32_t on);
        case DPPDemo_Cmd_TurnHVON:
            if((ret = CAENDPP_SetHVChannelPowerOn(handle, bId[currHVBrd], currHVCh, 1)) != CAENDPP_RetCode_Ok) {
                setStatusMessage("Cannot Power ON HV Channel: %s", decodeError(codeStr, ret));
                break;
            }
            setStatusMessage("HV Channel successfully turned ON", decodeError(codeStr, ret));
            break; // DPPDemo_Cmd_TurnHVON
        
        // DPPLibrary API used in DPPDemo_Cmd_TurnHVOFF:
        //   - CAENDPP_SetHVChannelPowerOn(int32_t handle, int32_t bId, int32_t ch, int32_t on);
        case DPPDemo_Cmd_TurnHVOFF:
            if((ret = CAENDPP_SetHVChannelPowerOn(handle, bId[currHVBrd], currHVCh, 0)) != CAENDPP_RetCode_Ok) {
                setStatusMessage("Cannot Power OFF HV Channel: %s", decodeError(codeStr, ret));
                break;
            }
            setStatusMessage("HV Channel successfully turned OFF", decodeError(codeStr, ret));
            break; // DPPDemo_Cmd_TurnHVOFF

        // DPPLibrary API used in DPPDemo_Cmd_Enumerate:
        //   - CAENDPP_EnumerateDevices(int32_t handle, CAENDPP_EnumeratedDevices_t* DeviceList);
        case DPPDemo_Cmd_Enumerate:
			if((ret = CAENDPP_EnumerateDevices(handle, &DeviceList)) != CAENDPP_RetCode_Ok) {
                setStatusMessage("Cannot Get HV Channel's Power Status: %s", decodeError(codeStr, ret));
                break;
            }

            if (DeviceList.ddcount > 0) {
			    for (i = 0; i < DeviceList.ddcount; i++) {
				    if (DeviceList.Device[i].ConnectionMode == CAENDPP_USB) {
					    printf("USB %5d", DeviceList.Device[i].id );
                        printf("\t%s", DeviceList.Device[i].SerialNUMBER);
					    printf("\t\t\t\t\t", DeviceList.Device[i].ProductDescription);
				    }
				    if (DeviceList.Device[i].ConnectionMode == CAENDPP_ETH) {
					    printf("ETH      ");
					    printf("\t%s", DeviceList.Device[i].SerialNUMBER);
					    printf("\t%s\t%s",DeviceList.Device[i].ETHAddress ,DeviceList.Device[i].ProductDescription);
				    }

				    printf("\n");
			    }
            }
            else
                printf("No device found.\n");
            printf("Press any key to continue...");
			fflush(stdin);
			_getch();
			break; // DPPDemo_Cmd_Enumerate
        // ################################################################## //
        // ##  BEGIN OF CONTROL COMMANDS WHICH DOESN'T USE DPPLIBRARY API  ## //
        // ################################################################## //
        case DPPDemo_Cmd_IncrActiveChannel:
            if (currCh < connectedChNum - 1)
                currCh++;
            break;
        case DPPDemo_Cmd_DecrActiveChannel:
            if (currCh >= 0)
                currCh--;
            break;
        case DPPDemo_Cmd_IncrActiveHVChannel:
            {
            int32_t newBrd = currHVBrd;
            int32_t newCh = currHVCh;
            if (newBrd < 0)
                newBrd = 0;
            if (newCh < 0)
                newCh = 0;
            else
                newCh++;
CheckIncreasedHVChannel:
            if (newCh >= (int32_t)info[newBrd].HVChannels) {
                newBrd++;
                if (newBrd >= connectedBrdNum)
                    break; // This breaks the switch whitout do nothing
                newCh = 0;
                goto CheckIncreasedHVChannel;
            }
            currHVBrd = newBrd;
            currHVCh = newCh;
            sprintf(currHVChName, "%s_%u - HV%d", info[currHVBrd].ModelName, info[currHVBrd].SerialNumber, currHVCh);
            break;
            }
        case DPPDemo_Cmd_DecrActiveHVChannel:
            {
            int32_t newBrd = currHVBrd;
            int32_t newCh = currHVCh;
            if (newBrd < 0)
                newBrd = 0;
            if (newCh < 0)
                break;
            else
                newCh--;
CheckDecreasedHVChannel:
            if (newCh < 0) {
                newBrd--;
                if (newBrd < 0)
                    break; // This breaks the switch whitout do nothing
                newCh = (int32_t)info[newBrd].HVChannels - 1;
                goto CheckDecreasedHVChannel;
            }
            currHVBrd = newBrd;
            currHVCh = newCh;
            sprintf(currHVChName, "%s_%u - HV%d", info[currHVBrd].ModelName, info[currHVBrd].SerialNumber, currHVCh);
            break;
            }
            break;
        case DPPDemo_Cmd_IncrActiveBoard:
            if (currBrd < connectedBrdNum - 1)
                currBrd++;
            break;
        case DPPDemo_Cmd_DecrActiveBoard:
            if (currBrd >= 0)
                currBrd--;
            break;
        case DPPDemo_Cmd_IncrActiveHisto:
            currHisto++;
            break;
        case DPPDemo_Cmd_DecrActiveHisto:
            if (currHisto >= 0)
                currHisto--;
            break;
        case DPPDemo_Cmd_HistoContinuousPlot:
            ContinuousPlot = !ContinuousPlot;
            if (ContinuousPlot == 1)
                setContinuousPlotPeriod(PLOT_PERIOD_MS);
            else
                setContinuousPlotPeriod(0);
            setStatusMessage("Continuous Plot %s", ContinuousPlot == 1 ? "Enabled" : "Disabled");
            break;
        case DPPDemo_Cmd_ContinuousHVMonitor:
            ContinuousHVMon = !ContinuousHVMon;
            if (ContinuousHVMon == 1)
                setContinuousHVMonPeriod(PLOT_PERIOD_MS);
            else
                setContinuousHVMonPeriod(0);
            setStatusMessage("Continuous HV Monitoring %s", ContinuousHVMon == 1 ? "Enabled" : "Disabled");
            break;
        case DPPDemo_Cmd_VSet:
            // Ask for the new Value
            printf("Enter new VSet Value (V): ");
            if (scanf("%lf", &f64) != 1 || f64 < 0) {
                setStatusMessage("User Abort");
                break;
            }
            else if (f64 < HV_VSET_MIN || f64 > HV_VSET_MAX) {
                setStatusMessage("Invalid Value %.2lf: VSet should be between %.2f and %.2f", f64, HV_VSET_MIN, HV_VSET_MAX);
                break;
            }
            // Point the Selected Parameter
            pF64 = &HVConfig.VSet;
            goto SetHVConfiguration;
        case DPPDemo_Cmd_ISet:
            // Ask for the new Value
            printf("Enter new ISet Value (uA): ");
            if (scanf("%lf", &f64) != 1 || f64 < 0) {
                setStatusMessage("User Abort");
                break;
            }
            else if (f64 < HV_ISET_MIN || f64 > HV_ISET_MAX) {
                setStatusMessage("Invalid Value %.2lf: ISet should be between %.2f and %.2f", f64, HV_ISET_MIN, HV_ISET_MAX);
                break;
            }
            // Point the Selected Parameter
            pF64 = &HVConfig.ISet;
            goto SetHVConfiguration;
        case DPPDemo_Cmd_RampDown:
            // Ask for the new Value
            printf("Enter new RampDown Value (V/s): ");
            if (scanf("%lf", &f64) != 1 || f64 < 0) {
                setStatusMessage("User Abort");
                break;
            }
            else if (f64 < HV_RAMPDOWN_MIN || f64 > HV_RAMPDOWN_MAX) {
                setStatusMessage("Invalid Value %.2lf: RampDown should be between %.2f and %.2f", f64, HV_RAMPDOWN_MIN, HV_RAMPDOWN_MAX);
                break;
            }
            // Point the Selected Parameter
            pF64 = &HVConfig.RampDown;
            goto SetHVConfiguration;
        case DPPDemo_Cmd_RampUp:
            // Ask for the new Value
            printf("Enter new RampUp Value (V/s): ");
            if (scanf("%lf", &f64) != 1 || f64 < 0) {
                setStatusMessage("User Abort");
                break;
            }
            else if (f64 < HV_RAMPUP_MIN || f64 > HV_RAMPUP_MAX) {
                setStatusMessage("Invalid Value %.2lf: RampUp should be between %.2f and %.2f", f64, HV_RAMPUP_MIN, HV_RAMPUP_MAX);
                break;
            }
            // Point the Selected Parameter
            pF64 = &HVConfig.RampUp;
            goto SetHVConfiguration;
        case DPPDemo_Cmd_PWDownMode:
            // Ask for the new Value
            printf("Choose new Power Down Mode:\n\n");
            printf("  1) RAMP\n");
            printf("  2) KILL\n");
            printf("  3) User Abort\n");
            chr = _getch();
            if (chr == '1')
                PWDownMode = CAENDPP_PWDownMode_Ramp;
            else if (chr == '2')
                PWDownMode = CAENDPP_PWDownMode_Kill;
            else {
                setStatusMessage("User Abort");
                break;
            }
            goto SetHVConfiguration;
        case DPPDemo_Cmd_SwitchHVPwr:
            if((ret = CAENDPP_GetHVChannelPowerOn(handle, bId[currHVBrd], currHVCh, &u32)) != CAENDPP_RetCode_Ok) {
                setStatusMessage("Cannot Get HV Channel's Power Status: %s", decodeError(codeStr, ret));
                break;
            }
            // Set the next character to turn ON of OFF the HV Channel
            if (u32 == 0)
                nextC = 'O';
            else
                nextC = 'o';
            break;
        case DPPDemo_Cmd_HVInterface:
            IFMode = IFMODE_HV;
            // Disable Continuous Plot
            ContinuousPlot = 0;
            setContinuousPlotPeriod(0);
            setStatusMessage("Switched to High Voltage Mode");
            break;
        case DPPDemo_Cmd_ACQInterface:
            IFMode = IFMODE_ACQ;
            // Disable Continuous HV Monitoring
            ContinuousHVMon = 0;
            setContinuousHVMonPeriod(0);
            setStatusMessage("Switched to Acquisition Mode");
            break;
        case DPPDemo_Cmd_Quit:
            quit = 1;
            break;
        default:
            setStatusMessage("Unrecognized Command");
            break;
        }
    }

QuitProgram:
    // End the library
    CAENDPP_EndLibrary(handle);

    // Dealloc histogram and probes
    if (h1 != NULL) free(h1);
    if (AT1 != NULL) free(AT1);
    if (AT2 != NULL) free(AT2);
    if (DT1 != NULL) free(DT1);
    if (DT2 != NULL) free(DT2);

    return ret;
}
