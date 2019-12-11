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

#ifndef __CAENHISTOGRAM_H
#define __CAENHISTOGRAM_H

#include <stdio.h>
#include <CAENTypes.h>
#include <math.h>

#define MAX_BININDEX 15
#define MIN_BININDEX 2
#define MAX_NBINS (1<<MAX_BININDEX)

#define _H_NB_T      uint16_t
#define _H_DATA_T    uint32_t
#define _H_RT_T      uint64_t
#define _H_DT_T      uint64_t
#define _H_SB_T      int32_t
#define _H_RF_T      int16_t

typedef enum {
    c_histoFormat_Binary    = 0L, /*(uint16_t)nBins;
                                        (int32_t)startBin;
                                        (nBins*uint32_t)data;
                                        (uint64_t)realTime_ns;
                                        (uint64_t)deadTime_ns;*/
    c_histoFormat_CAEN      = 1L,
} c_histoFormat_t;

typedef enum {
    c_histoErrorCode_Success            =  0L,
    c_histoErrorCode_GenericError       = -1L,
    c_histoErrorCode_NotYetImplemented  = -2L,
    c_histoErrorCode_OutOfMemory        = -3L,
    c_histoErrorCode_CantOpenFile       = -4L,
    c_histoErrorCode_FileWriteError     = -5L,
    c_histoErrorCode_InvalidHisto       = -6L,
    c_histoErrorCode_WrongBinNum        = -7L,
    c_histoErrorCode_HistoInit          = -8L,
} c_histoErrorCode_t;

typedef struct {
    _H_DATA_T  *data;
    _H_NB_T    nBins;
    _H_RT_T    realTime_ns;
    _H_DT_T    deadTime_ns;
    _H_SB_T    startBin;
    _H_RF_T    rFactor;
} c_histogram_t;

/*
- startBin can be any value (represents the numerical start of histo)
- nBins can be a power of 2 (up to MAX_NBINS)
- endBin must be startBin+(power of 2 >= nBins) (represents the numerical end of histo)
*/
c_histogram_t *c_hinit(_H_SB_T startBin, _H_NB_T nBins);

/* NOTE: if 'h' is NULL, the function does nothing; if 'h' is not NULL, the
   function assumes that the histo is a valid histo allocated with c_hinit */
int32_t c_hfree(c_histogram_t *h);

/* NOTE: the function assumes that the histo is a valid histo allocated with c_hinit */
int32_t c_hclear(c_histogram_t *h);

/* NOTE: the function assumes that the histo is a valid histo allocated with c_hinit */
int32_t c_hsave(const c_histogram_t *h, char *filename, c_histoFormat_t format);

/* NOTE: if 'h' is NULL, the function will init a histo with the min
   required number of bins. If 'h' is not NULL the fuction will assume
   that it is an histogram correctly allocated and will use it
   (without memoy leaks). */
int32_t c_hload(c_histogram_t *h, char *filename, c_histoFormat_t format);

/* NOTE: if 'h' is NULL, the function will init a histo with the min
   required number of bins. If 'h' is not NULL the fuction will assume
   that it is an histogram correctly allocated and will use it
   (without memoy leaks). */
int32_t c_hcopy(c_histogram_t *dest, const c_histogram_t *source);

/* NOTE1: 'source' and 'dest' can be the same histo
   NOTE2: if 'dest' is NULL, the function will init a histo with the min
   required number of bins. If 'dest' != NULL and != 'source', the function
   will assume that it is an histogram correctly allocated and will use it
   (without memoy leaks). */
int32_t c_hrebin(c_histogram_t *dest, const c_histogram_t *source, _H_NB_T nbins);

/* checks to do:
    - bins must be aligned:
        - source start >= dest start
        - source end <= dest end
        - source binning finer than dest binning: (sE-Ss)/sNb <= (dE-dS)/nNb
*/
int32_t c_hsum(c_histogram_t *dest, const c_histogram_t *source);

/* NOTE: if 'dest' is NULL, the function will init a histo with the min
   required number of bins based on bs (bin scale). If 'dest' != NULL
   and != 'source', the function will assume that it is an histogram
   correctly allocated and will use it (without memoy leaks). */
int32_t c_hshift(c_histogram_t *dest, const c_histogram_t *source, _H_SB_T startBin);

/* NOTE: the function assumes that the histo is a valid histo allocated with c_hinit */
int32_t c_hfillv(c_histogram_t *dest, _H_DATA_T value);

/* NOTE: the function assumes that the histo is a valid histo allocated with c_hinit */
int32_t c_hfilla(c_histogram_t *dest, _H_DATA_T *values, uint32_t nVals);

/* NOTE: the function assumes that the histo is a valid histo allocated with c_hinit */
int32_t c_hint(const c_histogram_t *h, uint32_t *integral);

/* histogram stretch down, from a histo with NBinsIn > NBinsOut
   NOTE: if 'dest' is NULL, the function will init a histo with the min
   required number of bins based on bs (bin scale). If 'dest' is not
   NULL the fuction will assume that it is an histogram correctly allocated
   to a valid number of bins with the function c_hinit(...) */
int32_t c_hstrd(c_histogram_t *dest, const c_histogram_t *source, double bs);

/* histogram stretch up, from a histo with NBinsIn < NBinsOut
   NOTE: if 'dest' is NULL, the function will init a histo with the min
   required number of bins based on bs (bin scale). If 'dest' is not
   NULL the fuction will assume that it is an histogram correctly allocated
   to a valid number of bins with the function c_hinit(...) */
int32_t c_hstru(c_histogram_t *dest, const c_histogram_t *source, double bs);


#endif