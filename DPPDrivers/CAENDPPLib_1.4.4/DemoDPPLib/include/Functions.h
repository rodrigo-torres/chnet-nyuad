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
* \file     Functions.h
* \brief    CAEN - Defintion of CAENDPPLibrary Demo Utility's API
* \author   Francesco Pepe (support.computing@caen.it)
*
******************************************************************************/

#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

// LINUX specific definitions
#ifdef LINUX

// Redfine posix deprecated
#define _popen popen
// define function max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
// define function min
#define min(a,b)            (((a) > (b)) ? (b) : (a))

#endif

#include <CAENDPPLib.h> 
#include <DPPLibDemo.h> 

/// \fn      uint64_t getTimeMs();
/// \brief   Returns the current time in ms
/// 
/// \return  the time in ms
uint64_t getTimeMs();


/// \fn      void setNextCharTimeout(uint64_t ms, char c);
/// \brief   Makes the function 'WaitNextChar()' to throw the char defined in 'c'
///          after the Timeout is reached. if ms <= 0 no timeout is set. This
///          feature can be used to perform some kind of automation: for example,
///          the Continuous plot is implemented by "throwing" the single plot char
///          once every plot period. The HV monitoring is implemented in the same
///          way using a different char
void setNextCharTimeout(uint64_t ms, char c);


/// \fn      void getNextCharTimeout(char c, size_t *ms);
/// \brief   gets the values of Timeout and char previously set
///          with 'setNextCharTimeout' function.
void getNextCharTimeout(char c, size_t *ms);


/// \fn      int32_t WaitNextChar();
/// \brief   Waits for the user to enter a character. This fuction can
///          automatically "trow" a character depending on the values configured
///          with 'setNextCharTimeout' fuction.
///          after the Timeout is reached. if ms <= 0 no timeout is set.
int32_t WaitNextChar();

/// \fn      int32_t isPow2(uint32_t val);
/// \brief   Gets if val is a power of 2
/// 
/// \param   [IN]  val     : The value to check
/// 
/// \return  0 = false; 1 = true
int32_t isPow2(uint32_t val);

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
int32_t saveHistogram(char *filename, uint32_t *EHisto, int32_t nbins, uint64_t RT_ns, uint64_t DT_ns);


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
int32_t loadHistogram(char *filename, uint32_t *EHisto, int32_t *nbins, uint64_t *RT_ns, uint64_t *DT_ns);


/// \fn      int32_t saveWaveform(char *filename, int32_t nTraces, void **traces, DPPDemo_TraceType_t *types, int32_t ns);
/// \brief   Saves a Waveform to file
/// 
/// \param   [IN]  filename     : The name of the file to write to
/// \param   [IN]  nTraces      : The number of Traces
/// \param   [IN]  traces       : Pointer to Traces datas
/// \param   [IN]  types        : The Types of the Traces
/// \param   [IN]  ns           : The number of samples of the waveform
/// 
/// \return  0 = Success; negative numbers are error codes
int32_t saveWaveform(char *filename, int32_t nTraces, void **traces, DPPDemo_TraceType_t *types, int32_t ns);


/// \fn      int32_t DefineAutosetParameters(CAENDPP_DgtzParams_t *Params);
/// \brief   Initialize the supported parameters to the value used for Autoset
/// 
/// \param   [OUT]  Params      : Pointer to the parameters structure to fill
/// 
/// \return  0 = Success; negative numbers are error codes
int32_t DefineAutosetParameters(CAENDPP_DgtzParams_t *Params);


/// \fn      int32_t InitDgtzParams(CAENDPP_DgtzParams_t *Params);
/// \brief   Initialize the configuration parameters to their default value
/// 
/// \param   [OUT]  Params      : Pointer to the parameters structure to fill
/// 
/// \return  0 = Success; negative numbers are error codes
int32_t InitDgtzParams(CAENDPP_DgtzParams_t *Params);


/// \fn      int32_t GetModifiedChannelParameters(int32_t ch, CAENDPP_DgtzParams_t *Params);
/// \brief   Prompts the User to change the given Channel Parameters
/// 
/// \param   [IN]   ch      : The channel to modify
/// \param   [I/O]  Params  : Pointer to the parameters structure to fill
/// 
/// \return  0 = Success; negative numbers are error codes
int32_t GetModifiedChannelParameters(int32_t ch, CAENDPP_DgtzParams_t *Params);


/// \fn      int32_t GetModifiedBoardParameters(CAENDPP_DgtzParams_t *Params);
/// \brief   Prompts the User to change the given Channel Parameters
/// 
/// \param   [IN]   ch      : The channel to modify
/// \param   [I/O]  Params  : Pointer to the parameters structure to fill
/// 
/// \return  0 = Success; negative numbers are error codes
int32_t GetModifiedBoardParameters(CAENDPP_AcqMode_t *acqMode, CAENDPP_DgtzParams_t *Params, CAENDPP_Info_t *bInfo);


/// \fn      char *decodeError(char *dest, int32_t ret);
/// \brief   Decodes the given error code into a message
/// 
/// \param   [IN] dest   : the string to be filled
/// \param   [IN] ret    : the error code
/// 
/// \return  a pointer to the string if success, otherwise NULL
char *decodeError(char *dest, int32_t code);


/// \fn      void setStatusMessage(char *msg);
/// \brief   Sets (if last status already printed) or Adds (if last status still not printed)
///          the given message on the status message
/// 
/// \param   [IN] msg   : Message to set/add
void setStatusMessage(const char* _Format, ...);


/// \fn      void printStatusMessage();
/// \brief   Prints the last set status msg to screen
/// 
/// \param   [IN] msg   : Message to display
void printStatusMessage();


/// \fn      void printAcquisitionInterface();
/// \brief   Prints the available commands list for Acquistion Mode
/// 
/// \param   [IN] msg   : Message to display
void printAcquisitionInterface();


/// \fn      void printHVInterface();
/// \brief   Prints the available commands list for High Voltage Mode
/// 
/// \param   [IN] msg   : Message to display
void printHVInterface();


/// \fn      void printInterface();
/// \brief   Prints the available commands list
/// 
/// \param   [IN] msg   : Message to display
void printInterface();


/// \fn      DPPDemo_Cmd_t getACQCommad(char c);
/// \brief   coverts the character 'c' to the corresponding
///          'DPPDemo_Cmd_t' command in Acquisition Mode
/// 
/// \param   [IN] c   : The character to convert
DPPDemo_Cmd_t getACQCommad(char c);


/// \fn      DPPDemo_Cmd_t getHVCommad(char c);
/// \brief   coverts the character 'c' to the corresponding
///          'DPPDemo_Cmd_t' command in Acquisition Mode
/// 
/// \param   [IN] c   : The character to convert
DPPDemo_Cmd_t getHVCommad(char c);


/// \fn      DPPDemo_Cmd_t getCommonCommad(char c);
/// \brief   coverts the character 'c' to the corresponding
///          'DPPDemo_Cmd_t' command in bith Interface Modes
/// 
/// \param   [IN] c   : The character to convert
DPPDemo_Cmd_t getCommad(char c);


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
int32_t getBrdRange(int32_t *f, int32_t *l);


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
int32_t getChRange(int32_t *f, int32_t *l);


/// \fn      int32_t isChannelDisabled(int32_t handle, int32_t ch);
/// \brief   Get if a channel is Enabled
/// 
/// \param   [IN] handle    : Handle to the Library
/// \param   [IN] ch        : Channel index to check
/// 
/// \return  0 if the channel is enabled, 1 if it is disabled
int32_t isChannelDisabled(int32_t handle, int32_t ch);


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
int32_t plotHisto(char *title, uint32_t *EHisto, int32_t nbins, uint32_t counts, double RT_s, double DT_s);


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
int32_t plotWaveform(int32_t nTraces, int32_t *tr, void **traces, DPPDemo_TraceType_t *types, int32_t ns);


/// \fn      void setContinuousPlotPeriod(uint64_t Timeout);
/// \brief   Sets the period of continuous plot
/// 
/// \param   [IN] Timeout   : Timeout in ms of the continuous plot.
///                           NOTE: if Timeout == 0 ->Continuous plot disabled
void setContinuousPlotPeriod(uint64_t Timeout);


/// \fn      void setContinuousHVMonPeriod(uint64_t Timeout);
/// \brief   Sets the period of continuous HV Monitoring
/// 
/// \param   [IN] Timeout   : Timeout in ms of the continuous monitoring.
///                           NOTE: if Timeout == 0->Continuous monitoring disabled
void setContinuousHVMonPeriod(uint64_t Timeout);


/// \fn      int32_t getConnectionParams(CAENDPP_ConnectionParams_t *connParam);
/// \brief   Asks the user to enter connection parameters.
/// 
/// \param   [OUT] connParam    : The parameters etered by the user
/// 
/// \return  0 = Success; negative numbers = User Abort
int32_t getConnectionParams(CAENDPP_ConnectionParams_t *connParam);


/// \fn      int32_t getStopCriteria(CAENDPP_StopCriteria_t *StopCriteria, uint64_t *StopCriteriaValue);
/// \brief   Asks the user to enter Stop Criteria parameters.
/// 
/// \param   [OUT] StopCriteria         : The Stop Criteria type choosen
/// \param   [OUT] StopCriteriaValue    : The Stop Criteria value
/// 
/// \return  0 = Success; negative numbers = User Abort
int32_t getStopCriteria(CAENDPP_StopCriteria_t *StopCriteria, uint64_t *StopCriteriaValue);

#endif
