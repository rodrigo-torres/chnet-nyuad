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

#include "Errors.h"


char ErrMsg[][100] = {
    "No Error",                                         /* ERR_NONE */
    "Configuration File not found",                     /* ERR_CONF_FILE_NOT_FOUND */
    "Can't open the digitizer",                         /* ERR_DGZ_OPEN */
    "Can't read the Board Info",                        /* ERR_BOARD_INFO_READ */
    "Can't run DPPrunner for this digitizer",           /* ERR_INVALID_BOARD_TYPE */
    "Can't program the digitizer",                      /* ERR_DGZ_PROGRAM */
    "Can't allocate the memory for the readout buffer", /* ERR_MALLOC */
    "Restarting Error",                                 /* ERR_RESTART */
    "Interrupt Error",                                  /* ERR_INTERRUPT */
    "Readout Error",                                    /* ERR_READOUT */
    "Event Build Error",                                /* ERR_EVENT_BUILD */
    "Can't allocate the memory fro the histograms",     /* ERR_HISTO_MALLOC */
    "Unhandled board type",                             /* ERR_UNHANDLED_BOARD */
    "Output file write error",                          /* ERR_OUTFILE_WRITE */

};