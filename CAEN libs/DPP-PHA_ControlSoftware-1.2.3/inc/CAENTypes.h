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

#ifndef __CAENTYPES_H
#define __CAENTYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#define THROW_EXCEPTION() free(0x1);
#define MAX_CH 8

#include <stdio.h>
#ifdef WIN32
	#include <windows.h>

	#ifndef int8_t
        //#define int8_t  INT8
        #define int8_t  char
    #endif
    #ifndef int16_t
        //#define int16_t INT16
        #define int16_t short
    #endif
    #ifndef int32_t
        //#define int32_t INT32
        #define int32_t long
    #endif
    #ifndef int64_t
        #define int64_t INT64
    #endif
    #ifndef uint8_t
        //#define uint8_t  UINT8
        #define uint8_t  unsigned char
    #endif
    #ifndef uint16_t
        //#define uint16_t UINT16
        #define uint16_t unsigned short
    #endif
    #ifndef uint32_t
        //#define uint32_t UINT32
        #define uint32_t unsigned long
    #endif
    #ifndef uint64_t
        #define uint64_t UINT64
    #endif
#else
    #include <stdint.h>
	#include <sys/types.h>
#endif

//#define	TYPE_STRING		0
//#define	TYPE_LONG		1
//#define TYPE_DOUBLE		2
//#define	TYPE_CHAR		3
//#define	TYPE_RESCODE	4
//#define	TYPE_SHORT		5
//#define TYPE_MEMORY		6
//#define TYPE_UINT64		7

typedef enum {
/*    TYPE_STRING     = 0L,
    TYPE_LONG		= 1L,
    TYPE_DOUBLE		= 2L,
    TYPE_CHAR		= 3L,
    TYPE_RESCODE	= 4L,
    TYPE_SHORT		= 5L,
    TYPE_MEMORY		= 6L,
    TYPE_UINT64		= 7L, */

    TYPE_INT8       = 0L,
    TYPE_UINT8      = 1L,
    TYPE_INT16      = 2L,     
    TYPE_UINT16     = 3L,
    TYPE_INT32      = 4L,
    TYPE_UINT32     = 5L,
    TYPE_INT64      = 6L,
    TYPE_UINT64     = 7L,
    TYPE_STRING     = 8L,
    TYPE_LONG		= 9L,
    TYPE_DOUBLE		= 10L,
    TYPE_CHAR		= 11L,
    TYPE_RESCODE	= 12L,
    TYPE_SHORT		= 13L,
    TYPE_MEMORY		= 14L,

    TYPE_NONE,
} c_type_t;

/* Definition of the format used to open files (ASCII or
binary). It is used when the dumper is opened. If a dumper
is opened with the flag ASCII, it will dump data in a text
file, otherwise datas will be dumped in binary format */
typedef enum {
    c_format_ASCII    = 0L,
    c_format_Binary   = 1L,
} c_saveFormat_t;

/* Definition of the mode used to open files. It is used when
the dumper is opened. The dumper can be contmporary opened in
both read and write (or append) modes. A dumper opened in a 
single write/read mode should have better performance (NOT TESTED) */
typedef enum {
    c_mode_R    = 0L,
    c_mode_W    = 1L,
    c_mode_A    = 2L,
    c_mode_RW   = 3L,
    c_mode_RA   = 4L,
} c_mode_t;

/* Definition of different DPP types. It is used to
recognize different DPPs by the function which directly
writes and read DPP events (and not generic events).
See for exemple read/writeDPPEvent. */
typedef enum {
    c_DPPType_CI      = 0L,
    c_DPPType_PHA     = 1L,
    c_DPPType_PSD     = 2L,

    c_DPPType_None  = 255L,
} c_DPPType_t;

/* Definition of DPPs descriptor. It includes the DPP
type and the size of TimeStamp and Energy (32/64 bits).
It is used to recognize different DPPs by the function
which directly writes and read DPP events (and not
generic events). See for exemple read/writeDPPEvent.
It is passed to the dumper when it is opened. Once
opened, the dumper will use this descriptor until it is
closed. The descriptor must be manually initialized
it is passed to the dumper. If it is not initialized, the
dumper will not be able to directly dump DPP events, and a
call to that functions will return an error.
Accepted values:
    (BO = Binary Only; AO = ASCII Only)
	TSType: TYPE_UINT32; TYPE_UINT64;
	ENType: TYPE_INT16; TYPE_UINT16; TYPE_INT32(BO); TYPE_UINT32(BO);
    NOTE: For binary dump, signed types are threated as unsigned

For DPP-PSD the ENType must be TYPE_INT32, because the
chargeShort takes bits 0->15 and chargeLong bits 16->31. */
typedef struct {
    c_DPPType_t DPPType;
    c_type_t TSType;
    c_type_t ENType;
} c_DPPDescriptor_t;

c_DPPDescriptor_t DPP_CI_Descr;
c_DPPDescriptor_t DPP_PSD_Descr;
c_DPPDescriptor_t DPP_PHA_Descr;
c_DPPDescriptor_t DPP_None_Descr;

//#define C_FMT_I8    "%hhd"
//#define C_FMT_U8    "%hhu"
#define C_FMT_I8    "%c"
#define C_FMT_U8    "%hu" // NOTE, should be "%hhu" but is not supported in C90. This causes problems for example in sscanf on void*
#define C_FMT_I16   "%hd"
#define C_FMT_U16   "%hu"
#define C_FMT_I32   "%d"
#define C_FMT_U32   "%u"
#ifdef WIN32
#define C_FMT_I64   "%I64d"
#define C_FMT_U64   "%I64u"
#else
#define C_FMT_I64   "%lld"
#define C_FMT_U64   "%llu"
#endif

int32_t c_getSizeOfType(c_type_t t);
int32_t c_initDPPDescr();

#ifdef __cplusplus
}
#endif

#endif