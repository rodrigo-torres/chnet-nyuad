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

#ifndef __CAENBUFFER_H
#define __CAENBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <CAENTypes.h>

typedef struct {
    void *buffer;
    uint32_t allocatedSize;
    uint16_t allocated;
} c_buffer_t;

c_buffer_t *c_mallocBuffer(uint32_t size);
c_buffer_t *c_callocBuffer(uint32_t Counts, uint32_t Size);
c_buffer_t *c_freeBuffer(c_buffer_t *BUF);

#ifdef __cplusplus
}
#endif

#endif