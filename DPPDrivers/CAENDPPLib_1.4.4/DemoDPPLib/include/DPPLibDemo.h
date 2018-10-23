/******************************************************************************
* 
* CAEN SpA - Computing Division
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
*
* \file     DPPLibDemo.h
* \brief    CAEN - Definitions for CAENDPPLibrary' Demo program
* \author   Francesco Pepe (support.computing@caen.it)
*
******************************************************************************/

#ifndef __DPPLIB_DEMO_H
#define __DPPLIB_DEMO_H

#include <stdlib.h> 
#include <stdio.h> 

// GNUPlot Command
#ifdef WIN32
#define GNUPLOT_CMD "pgnuplot.exe"
#else
#define GNUPLOT_CMD "gnuplot"
#endif

// The maximum number of board's channels.
// NOTE: MAX_NUMCHB is defined in 'CAENDPPLibTypes.h'
#define MAX_BOARD_CHNUM MAX_NUMCHB

// The maximum number of board's channels.
// NOTE: MAX_NUMB is defined in 'CAENDPPLibTypes.h'
#define MAX_BOARD_NUMBER MAX_NUMB

// The maximum number of bits for the histograms
// 14 bits ->from bin 0 to bin 16383 (2^14-1)
#define MAX_HISTOBITS (14)

// The maximum number of bins of the histograms
// (depending on their maximum number of bits)
#define MAX_HISTO_NBINS (0x1 << (MAX_HISTOBITS-0))

// Other defines
#define PLOT_PERIOD_MS      (1000)  // Continuous plot interval (ms)
#define SINGLE_PLT_CHAR     ('p')   // Definition of the character used for single shot plotting
#define HVMON_PERIOD_MS     (1000)  // Continuous plot interval (ms)
#define HVMON_PLT_CHAR      ('m')   // Definition of the character used for single HV monitoring
#define AUTOSET_VALUE       (-1)    // The value used for Autoset of parameters
#define MAX_STATUSMSG_LEN   (200)   // Maximum length in chars of the status string
#define MAX_ERRMSG_LEN      (100)    // Maximum length in chars of the error message string
#define IFMODE_ACQ          (0)     // Acquisition Interface Mode Definition
#define IFMODE_HV           (1)     // High Voltage Interface Mode Definition
#define MAX_HVNAME_LEN      (29)    // Maximum Length of Active HV Channel Name
#define MAX_TRACENAME_LEN   (50)    // Maximum Length of Traces Name
#define MAX_HISTOFNAME_LEN  (100)   // Maximum Length of Histograms File Name
#define MAX_NUM_TRACES      (4)     // The Maximum Number of Traces to be saved/plotted
#define ABORT_AUTOSET_CHAR  ('Q')   // Character to Abort Autoset Functionality

// Enumeration to define different commands
typedef enum {
    DPPDemo_Cmd_Unknown,                // Unknow command ('.')
    DPPDemo_Cmd_Autoset,                // Autofind Acquisition Parameters ('A')
    DPPDemo_Cmd_SetBoardConfig,         // Change 'Active Board' Configuration ('M')
    DPPDemo_Cmd_SetChannelConfig,       // Change 'Active Channel' Configuration ('m')
    DPPDemo_Cmd_SetInputRange,          // Change 'Active Channel' Input Range ('r')
    DPPDemo_Cmd_Configure,              // Configure Boards ('C')
    DPPDemo_Cmd_SetStopCriteria,        // Change Stop Criteria ('E')
    DPPDemo_Cmd_GetStopCriteria,        // Get Stop Criteria ('e')
    DPPDemo_Cmd_StartAcq,               // Start acquisition ('s')
    DPPDemo_Cmd_StopAcq,                // Stop acquisition ('S')
    DPPDemo_Cmd_SwTrgToggle,            // Toggle Continuous Software Trigger ('T')
    DPPDemo_Cmd_ClearHisto,             // Clear Histogram ('c')
    DPPDemo_Cmd_ResetAllHistos,         // Reset All Histograms ('R')
    DPPDemo_Cmd_SwitchHisto,            // Switch to Next Histogram ('N')
    DPPDemo_Cmd_SaveHisto,              // Save Histograms to file ('H')
    DPPDemo_Cmd_HistoSinglePlot,        // Current Histogram Single Plot ('p')
    DPPDemo_Cmd_HistoContinuousPlot,    // Toggle Current Histogram Continuous Plot ('P')
    DPPDemo_Cmd_SaveWave,               // Save waveforms to file ('W')
    DPPDemo_Cmd_WaveSinglePlot,         // Waveform Single Plot ('w')
    DPPDemo_Cmd_SetCurrentHisto,        // Set Current Histogram Index to 'Active Histo' ('U')
    DPPDemo_Cmd_GetCurrentHisto,        // Get Current Histogram Index ('u')
    DPPDemo_Cmd_GetHistoNum,            // Get Number of Histograms ('n')
    DPPDemo_Cmd_AddHisto,               // Add new Histogram ('a')
    DPPDemo_Cmd_SetHisto,               // Set 'Active Histogram' from file ('h')
    DPPDemo_Cmd_SetHistoSize,           // Change 'Active Histo' Size ('d')
    DPPDemo_Cmd_GetHistoStatus,         // Get 'Active Histo' status ('y')
    DPPDemo_Cmd_SetHistoStatus,         // Change 'Active Histo' status ('Y')
    DPPDemo_Cmd_HVInterface,            // Switch to HV Interface ('V')
    DPPDemo_Cmd_IncrActiveHisto,        // Increase Active Histogram ('9')
    DPPDemo_Cmd_DecrActiveHisto,        // Decrease Active Histogram ('8')
    DPPDemo_Cmd_SingleHVMonitor,        // Single HV Monitor Cycle ('m')
    DPPDemo_Cmd_ContinuousHVMonitor,    // Toggle Continuous HV Monitoring ('C')
    DPPDemo_Cmd_GetHVConfig,            // Get HV Channel Configuration ('c')
    DPPDemo_Cmd_SetHVConfig,            // Set HV Channel Configuration
    DPPDemo_Cmd_VSet,                   // Change VSet ('V')
    DPPDemo_Cmd_ISet,                   // Change ISet ('I')
    DPPDemo_Cmd_VMax,                   // Change VMax ('M')
    DPPDemo_Cmd_RampDown,               // Change Ramp Down ('D')
    DPPDemo_Cmd_RampUp,                 // Change Ramp Up ('U')
    DPPDemo_Cmd_PWDownMode,             // Change Power Down Mode ('P')
    DPPDemo_Cmd_TurnHVON,               // Turn HV Channel ON ('O')
    DPPDemo_Cmd_TurnHVOFF,              // Turn HV Channel OFF ('o')
    DPPDemo_Cmd_SwitchHVPwr,            // Switch HV Channel ON/OFF ('S')
    DPPDemo_Cmd_ACQInterface,           // Switch to Acquisition Interface ('A')
    DPPDemo_Cmd_IncrActiveBoard,        // Increase Active Board ('*')
    DPPDemo_Cmd_DecrActiveBoard,        // Decrease Active Board ('/')
    DPPDemo_Cmd_IncrActiveChannel,      // Increase Active Channel ('+')
    DPPDemo_Cmd_DecrActiveChannel,      // Decrease Active Channel ('-')
    DPPDemo_Cmd_IncrActiveHVChannel,    // Increase Active HV Channel ('+')
    DPPDemo_Cmd_DecrActiveHVChannel,    // Decrease Active HV Channel ('-')
    DPPDemo_Cmd_AddBoard,               // Add Board ('B')
    DPPDemo_Cmd_Enumerate,              // Enumerate devices
    DPPDemo_Cmd_Quit,                   // Quit ('q')
} DPPDemo_Cmd_t;

// Definition of the Trace Type
typedef enum {
    DPPDemo_TraceType_Analog1,
    DPPDemo_TraceType_Analog2,
    DPPDemo_TraceType_Digital1,
    DPPDemo_TraceType_Digital2,
    DPPDemo_TraceType_TriggerSource,
} DPPDemo_TraceType_t;

// Some globals used to manage number of bins, active board/channel, plotting, etc.
int32_t currBrd;                                // Current board index
int32_t currCh;                                 // Current channel index
int32_t currHVBrd;                              // Current Board Index controlled in HV Interface
int32_t currHVCh;                               // Current High Voltage Board Channel Index
char currHVChName[MAX_HVNAME_LEN];              // Name of the Current HV Channel Index
int32_t currHisto;                              // Current histogram index
int32_t connectedBrdNum;                        // Number of connected boards
int32_t connectedChNum;                         // Number of connected channels
int32_t IFMode;                                 // Used to define the active operating mode (ACQ / HV)
char statusMessage[MAX_STATUSMSG_LEN + 1];      // Status Message String
int32_t statusMsgPrinted;                       // Flag used for status msg printing
FILE *gnuplot;                                  // Pipe to the plotter

#endif
