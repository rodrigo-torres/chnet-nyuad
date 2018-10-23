/******************************************************************************
*
* CAEN SpA - Front End Division
* Via Vetraia, 11 - 55049 - Viareggio ITALY
* +390594388398 - www.caen.it
*
*******************************************************************************
* \note TERMS OF USE:
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software
* Foundation. This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. The user relies on the
* software, documentation and results solely at his own risk.
******************************************************************************/

/******************************************************************************
*
*	Module: CAENDumper
*	
*	Author: Francesco
*
*	Date: May 2012
*	
*	Description:
*		This module provides an interface useful to dump and read list events
*		to/from file. Each event is made of a TimeStamp and an Energy. For a
*		better usability, a specific function to dump/read DPP events is
*		provided. The dumper can manage events from every kind of DPP defined
*		in CAENDigitizer.h header file (as far as now, DPP-CI, DPP-PHA and
*		DPP-PSD). The dumper is able to read/write in ASCII or in Binary mode,
*		and a single Dumper can be used for simoultaneous Read and Write
*		operations. The dumper is also able to perform block Read and Write
*		operations. In this way, using binary mode the peformance are better
*		then single events W/R. The dumper needs an initial configuration.
*		
*			The following is the list of types and functions defined in this
*		header.
*		
*		TYPES:
*			c_saveFormat_t
*			c_DPPType_t
*			c_DPPDescriptor_t
*			c_mode_t
*			c_event_t
*			c_dumper_t
*		
*		FUNCTIONS:
*           1)  int32_t c_openDumper(c_dumper_t *dpr, char *filename,
*                                       c_mode_t MODE, c_saveFormat_t FMT,
*                                       c_DPPDescriptor_t DPPDescr);
*			2)	int32_t c_closeDumper(c_dumper_t *dpr);
*			3)	int32_t c_setWriteCursorPosition(c_dumper_t *dpr, fpos_t *pos);
*			4)	int32_t c_getWriteCursorPosition(c_dumper_t *dpr, fpos_t *pos);
*			5)	int32_t c_setReadCursorPosition(c_dumper_t *dpr, fpos_t *pos);
*			6)	int32_t c_getReadCursorPosition(c_dumper_t *dpr, fpos_t *pos);
*			7)	int32_t c_isDumperOpened(c_dumper_t *dpr);
*			8)	int32_t c_setDumperBusy(c_dumper_t *dpr, int32_t busy);
*			9)	int32_t c_getDumperBusy(c_dumper_t *dpr, int32_t *busy);
*			10)	int32_t c_dumpEvent(c_dumper_t *dpr, c_event_t evt,
*										c_type_t timeStampType,
*										c_type_t energyType);
*			11)	int32_t c_dumpBlockEvent(c_dumper_t *dpr, uint32_t nevts,
*											c_event_t *evts,
*											c_type_t timeStampType,
*											c_type_t energyType);
*			12)	int32_t c_dumpDPPEvent(c_dumper_t *dpr, void *evt);
*			13)	int32_t c_dumpDPPBlockEvent(c_dumper_t *dpr, uint32_t nevts,
*												void *evts);
*			14)	int32_t c_readEvent(c_dumper_t *dpr, c_event_t *evt,
*										c_type_t timeStampType,
*										c_type_t energyType);
*			15)	int32_t c_readBlockEvent(c_dumper_t *dpr, uint32_t nevts,
*											c_event_t *evts,
*											c_type_t timeStampType,
*											c_type_t energyType);
*			16)	int32_t c_readDPPEvent(c_dumper_t *dpr, void *evt);
*			17)	int32_t c_readDPPBlockEvent(c_dumper_t *dpr, uint32_t nevts,
*												void *evts);
*		
*			See the comment before each function for a complete functionality
*		decription.
*
        TODO:
            - Implement Waveforms dump (we should create a module CAENWaveform ??).
            - Implement Histogram dump (we should create a module CAENHistogram !!).
            - Add logging functionality (we should create a module CAENLogger).
******************************************************************************/

#ifndef __CAENDUMPER_H
#define __CAENDUMPER_H

#include <stdio.h>
#include <CAENTypes.h>
#include <CAENBuffer.h>

#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/time.h>
	#include <unistd.h>
	#include <stdint.h>
#endif

#define DPR_STATUSBIT_OPENED 0   // 0 -> Closed; 1 -> Opened;
#define DPR_STATUSBIT_BUSY   1   // 0 -> Free; 1 -> Busy;

// Definition of fake events TimeStamp and Charge.
// It is used to build fill events structures with
// unphysical datas (see for exemple readBlockEvent())
#define FAKE_EVT_TS ~0x0    // all bits at 1
#define FAKE_EVT_EN 0x0     // all bits at 0

// Definition of DPP's Sizes
#define DPP_CI_TSSIZE uint32_t
#define DPP_CI_ENSIZE uint16_t
#define DPP_PHA_TSSIZE uint64_t
#define DPP_PHA_ENSIZE uint16_t
#define DPP_PSD_TSSIZE uint32_t
#define DPP_PSD_ENSIZEL uint16_t
#define DPP_PSD_ENSIZES uint16_t
#define DPP_PSD_ENOFFSET sizeof(DPP_PSD_ENSIZEL)*8 // 16 bit

typedef enum {
    c_DPRErrorCode_Success              = 0L,
    c_DPRErrorCode_GenericError         = -1L,
    c_DPRErrorCode_WrongOpenMode        = -2L,
    c_DPRErrorCode_DumperClosed         = -3L,
    c_DPRErrorCode_OutOfMemory          = -4L,
    c_DPRErrorCode_InvalidDPPDescriptor = -5L,
    c_DPRErrorCode_InvalidCursorPositin = -6L,
    c_DPRErrorCode_CantOpenFile         = -7L,
    c_DPRErrorCode_DumperNotReady       = -8L,
    c_DPRErrorCode_TSSizeNotSupported   = -9L,
    c_DPRErrorCode_EnSizeNotSupported   = -10L,
    c_DPRErrorCode_InvalidDPPType       = -12L,
    c_DPRErrorCode_ErrorEventRead       = -13L,
    c_DPRErrorCode_ZeroDivision         = -14L,
    c_DPRErrorCode_WrongOpenFormat      = -15L,   
} c_DPRErrorCode_t;

/* Definition of generic events structure. Both timestamp and energy
are pointer to void in order to gain generality.
The use should be: 
	1) Declare the event
	2) Set the timestamp pointer to the address of the actual TS
		you want to read/write (for example DPP-PSD event Timestamp)
	3) Use the specific function to read/write the event.

For example: 
	CAEN_DGTZ_DPP_CI_Event_t CIEVT;
	c_event_t e;
	e.TimeStamp = &CIEVT.TimeTag;
	e.Energy = &CIEVT.Charge;
	c_readEvent(dpr, e, TYPE_UINT32, TYPE_UINT16);
	// Now CIEVT is filled with read TimeTag and Charge
	
Otherwise one can allocate the void* poiters of c_event_t and then
use the values pointed. */
typedef struct {
    // The TS and Energy are void * because they change between different DPP's
    void *TimeStamp;
    void *Energy;
} c_event_t;

typedef struct {
    c_buffer_t *BUF;   // The buffer used for block write
    void *lastEvtPtr;
    int maxAllocEvts;
    int nBuffEvts;
} c_eventCollection_t;

/* Definition of dumper structure */
typedef struct {
    c_saveFormat_t format;
    c_mode_t mode;
    char *fileName;
    char openFlags[4]; // The flags used to open the file (ex: "w", "r", "wb", etc)
    FILE *f;
    int16_t status;    /* | 15...2 |   1  |    0   |
						  |  NONE  | BUSY | OPENED | */ // See DPR_STATUSBIT_* definitions
    
    fpos_t wPos; 	   // Position of write cursor
    fpos_t rPos;       // Position of read cursor

    c_eventCollection_t evtsBuff;

    c_DPPDescriptor_t DPPDescr; // The DPP descriptor, used to perform DPP events dumps
} c_dumper_t;

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
* \fn  		c_dumper_t *c_openDumper(char *filename, c_mode_t MODE, c_saveFormat_t FMT, c_DPPDescriptor_t DPPDescr);
* \brief 	Opens the dumper
*
* \param 	[IN] filename:	the name of the file to use
			[IN] MODE: 		the mode used to open the file (Read/Write/Both)
			[IN] FMT: 		the format used to open the file (ASCII/Binary)
			[IN] DPPDescr:	The descriptor of the DPP for this dumper
* \return   A pointer to the dumper if success; NULL in case of errors.
******************************************************************************/
c_dumper_t *c_openDumper(char *filename, c_mode_t MODE, c_saveFormat_t FMT, c_DPPDescriptor_t DPPDescr);

/**************************************************************************//**
* \fn  		int32_t c_closeDumper(c_dumper_t *dpr);
* \brief 	Closes the dumper
*
* \param 	[IN] dpr:	the pointer to the dumper to close
* \return   0 = Success; negative numbers = Failure (NOT IMPLEMENTED);

* NOTE: the dumper pointer is free-ed and set to NULL, a new pointer may be
* taken using c_openDumper(...) function
******************************************************************************/
c_dumper_t *_closeDumper_(c_dumper_t *_dpr);
#define c_closeDumper(dpr) (dpr=_closeDumper_(dpr))==NULL?c_DPRErrorCode_Success:c_DPRErrorCode_GenericError;

/**************************************************************************//**
* \fn  		int32_t c_setWriteCursorPosition(c_dumper_t *dpr, fpos_t *pos);
* \brief 	Sets the position where the next event will be written
*
* \param 	[IN] dpr:	the pointer to the dumper
			[IN] pos:	the pointer to the write position to set
* \return   0 = Success; negative numbers = Failure;
******************************************************************************/
int32_t c_setWriteCursorPosition(c_dumper_t *dpr, fpos_t *pos);

/**************************************************************************//**
* \fn  		int32_t c_getWriteCursorPosition(c_dumper_t *dpr, fpos_t *pos);
* \brief 	Gets the position where the next event will be written
*
* \param 	[IN]  dpr:	the pointer to the dumper
			[OUT] pos:	the pointer to the write position to get
* \return   0 = Success; negative numbers = Failure;
******************************************************************************/
int32_t c_getWriteCursorPosition(c_dumper_t *dpr, fpos_t *pos);

/**************************************************************************//**
* \fn  		int32_t c_setReadCursorPosition(c_dumper_t *dpr, fpos_t *pos);
* \brief 	Sets the position from where the next event will be read
*
* \param 	[IN] dpr:	the pointer to the dumper
			[IN] pos:	the pointer to the read position to set
* \return   0 = Success; negative numbers = Failure;
******************************************************************************/
int32_t c_setReadCursorPosition(c_dumper_t *dpr, fpos_t *pos);

/**************************************************************************//**
* \fn  		int32_t c_getReadCursorPosition(c_dumper_t *dpr, fpos_t *pos);
* \brief 	Gets the position from where the next event will be read
*
* \param 	[IN]  dpr:	the pointer to the dumper
			[OUT] pos:	the pointer to the read position to get
* \return   0 = Success; negative numbers = Failure;
******************************************************************************/
int32_t c_getReadCursorPosition(c_dumper_t *dpr, fpos_t *pos);

/**************************************************************************//**
* \fn  		int32_t c_isDumperOpened(c_dumper_t *dpr);
* \brief 	Gets whether the dumper is opened or not.
*
* \param 	[IN]  dpr:	the pointer to the dumper
* \return   0 = Closed; 1 = Opened;
******************************************************************************/
int32_t c_isDumperOpened(c_dumper_t *dpr);

/**************************************************************************//**
* \fn  		int32_t c_setDumperBusy(c_dumper_t *dpr, int32_t busy);
* \brief 	Sets the dumper "busy" bit to the specified value
*
* \param 	[IN] dpr:	the pointer to the dumper
			[IN] busy:	The value of the busy bit to set
* \return   0 = Success; negative numbers = Failure;
******************************************************************************/
int32_t c_setDumperBusy(c_dumper_t *dpr, int32_t busy);

/**************************************************************************//**
* \fn  		int32_t c_getDumperBusy(c_dumper_t *dpr, int32_t *busy);
* \brief 	Gets the dumper "busy" bit
*
* \param 	[IN]  dpr:	the pointer to the dumper
			[OUT] busy:	The value of the busy bit to get
* \return   0 = Closed; 1 = Opened;
******************************************************************************/
int32_t c_getDumperBusy(c_dumper_t *dpr, int32_t *busy);

/**************************************************************************//**
* \fn  		int32_t c_dumpEvent(c_dumper_t *dpr, c_event_t evt, c_type_t timeStampType, c_type_t energyType);
* \brief 	Dumps a single generic event, using timeStampType and energyType
			as types for TimeStamp and Energy respectively
*
* \param 	[IN]  dpr:				the pointer to the dumper
			[IN]  evt:				the event to dump
			[IN]  timeStampType:	the type of TimeStamp field
			[IN]  energyType:		the type of Energy field
* \return   0 = Success; negative numbers = Failure;
******************************************************************************/
int32_t c_dumpEvent(c_dumper_t *dpr, c_event_t evt, c_type_t timeStampType, c_type_t energyType);

/**************************************************************************//**
* \fn  		int32_t c_dumpBlockEvent(c_dumper_t *dpr, uint32_t nevts, c_event_t *evts, c_type_t timeStampType, c_type_t energyType);
* \brief 	Dumps a block of nevts generic events, using timeStampType and
			energyType as types for TimeStamp and Energy respectively
*
* \param 	[IN]  dpr:				the pointer to the dumper
			[IN]  nevts				the number of events to dump
			[IN]  evts:				the events list to dump (must contain valid data)
			[IN]  timeStampType:	the type of TimeStamp field
			[IN]  energyType:		the type of Energy field
* \return   The number of events effectively dumped; negative numbers = Failure;
******************************************************************************/
int32_t c_dumpBlockEvent(c_dumper_t *dpr, uint32_t nevts, c_event_t *evts, c_type_t timeStampType, c_type_t energyType);

/**************************************************************************//**
* \fn  		int32_t c_dumpDPPEvent(c_dumper_t *dpr, void *evt);
* \brief 	Dumps a single DPP event, using the DPP Descriptor passed to the
			function c_openDumper(...). If the descriptor wasn't correctly
			initialized, the function will return an error
*
* \param 	[IN]  dpr:				the pointer to the dumper
			[IN]  evt:				the event to dump
			[IN]  timeStampType:	the type of TimeStamp field
			[IN]  energyType:		the type of Energy field
* \return   0 = Success; negative numbers = Failure;
******************************************************************************/
int32_t c_dumpDPPEvent(c_dumper_t *dpr, void *evt);

/**************************************************************************//**
* \fn  		int32_t c_dumpDPPBlockEvent(c_dumper_t *dpr, uint32_t nevts, void *evts);
* \brief 	Dumps a block of nevts DPP event, using the DPP Descriptor passed
			to the function c_openDumper(...). If the descriptor wasn't correctly
			initialized, the function will return an error.
*
* \param 	[IN]  dpr:		the pointer to the dumper
			[IN]  nevts:	the number of events to dump
			[IN]  evts:		the events list to dump. It should be the list of
							DPP events as defined in CAENDigitizer.h (for ex:
							CAEN_DGTZ_DPP_CI_Params_t * for DPP-CI).
									
* \return   The number of events effectively dumped; negative numbers = Failure;
******************************************************************************/
int32_t c_dumpDPPBlockEvent(c_dumper_t *dpr, uint32_t nevts, void *evts);

/**************************************************************************//**
* \fn  		c_readEvent(c_dumper_t *dpr, c_event_t *evt, c_type_t timeStampType, c_type_t energyType);
* \brief 	Reads a single generic event, using timeStampType and energyType
			as types for TimeStamp and Energy respectively
*
* \param 	[IN]  dpr:				the pointer to the dumper
			[OUT] evt:				the event to read
			[IN]  timeStampType:	the type of TimeStamp field
			[IN]  energyType:		the type of Energy field
* \return   0 = Success; negative numbers = Failure;
******************************************************************************/
int32_t c_readEvent(c_dumper_t *dpr, c_event_t *evt, c_type_t timeStampType, c_type_t energyType);

/**************************************************************************//**
* \fn  		int32_t c_readBlockEvent(c_dumper_t *dpr, uint32_t nevts, c_event_t *evts, c_type_t timeStampType, c_type_t energyType);
* \brief 	Reads a block of nevts generic events, using timeStampType and
			energyType as types for TimeStamp and Energy respectively
*
* \param 	[IN]  dpr:				the pointer to the dumper
			[IN]  nevts				the number of events to read
			[OUT] evts:				the events list to fill (must point to valid addresses)
			[IN]  timeStampType:	the type of TimeStamp field
			[IN]  energyType:		the type of Energy field
* \return   The number of events effectively read; negative numbers = Failure;
******************************************************************************/
int32_t c_readBlockEvent(c_dumper_t *dpr, uint32_t nevts, c_event_t *evts, c_type_t timeStampType, c_type_t energyType);

/**************************************************************************//**
* \fn  		int32_t c_readDPPEvent(c_dumper_t *dpr, void *evt);
* \brief 	reads a single DPP event, using the DPP Descriptor passed to the
			function c_openDumper(...). If the descriptor wasn't correctly
			initialized, the function will return an error
*
* \param 	[IN]  dpr:				the pointer to the dumper
			[OUT]  evt:				the event to fill with read data. It should
									be the address of a DPP event as defined in
									CAENDigitizer.h (for ex:
									CAEN_DGTZ_DPP_CI_Params_t for DPP-CI).
* \return   0 = Success; negative numbers = Failure;
******************************************************************************/
int32_t c_readDPPEvent(c_dumper_t *dpr, void *evt);

/**************************************************************************//**
* \fn  		int32_t c_readDPPBlockEvent(c_dumper_t *dpr, uint32_t nevts, void *evts);
* \brief 	Reads a block of nevts DPP event, using the DPP Descriptor passed
			to the function c_openDumper(...). If the descriptor wasn't correctly
			initialized, the function will return an error.
*
* \param 	[IN]  dpr:		the pointer to the dumper
			[IN]  nevts:	the number of events to read
			[OUT] evts:		the events list to dump. It should be the list of
							DPP events as defined in CAENDigitizer.h (for ex:
							CAEN_DGTZ_DPP_CI_Params_t * for DPP-CI).
									
* \return   The number of events effectively read; negative numbers = Failure;
******************************************************************************/
int32_t c_readDPPBlockEvent(c_dumper_t *dpr, uint32_t nevts, void *evts);

/**************************************************************************//**
* \fn  		int32_t c_dadde(c_dumper_t *dpr, void *TTPtr, void *ENPtr)
* \brief 	Add an event to the dumper's internal buffer. The accumulated
            events can then be effectively dumped with c_ddumpb(..) funct
*
* \param 	[IN]  dpr:		the pointer to the dumper
			[IN]  TTPtr :   pointer to event's TimeTag
            [IN]  ENPtr :   pointer to event's Energy

NOTE1: The TimeTag and Energy are memcopyed in the internal dumper's buffer
       accordingly to the sizes provided in the dumper's DPPDescriptor. There
       is then no need to preserve the values pointed by TTPtr and ENPtr.
NOTE2: For DPP-PSD the ENPtr should point to a INT32 whose first 0:15 bits are
       ELong and bits 16:31 are EShort

* \return   0 = Success; negative numbers = Failure;
******************************************************************************/
int32_t c_dadde(c_dumper_t *dpr, void *TTPtr, void *ENPtr);

/**************************************************************************//**
* \fn  		int32_t c_ddumpb(c_dumper_t *dpr)
* \brief 	Dumps the events accumulated in the internal buffer.
*
* \param 	[IN]  dpr:	the pointer to the dumper

* \return   0 = Success; negative numbers = Failure;
******************************************************************************/
int32_t c_ddumpb(c_dumper_t *dpr);

#ifdef __cplusplus
}
#endif

#endif