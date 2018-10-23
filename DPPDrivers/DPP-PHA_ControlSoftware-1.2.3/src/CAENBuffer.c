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

#include <CAENBuffer.h>
#include <stdlib.h>
#include <string.h>

int32_t _checkBuf(c_buffer_t *BUF) {
    if(BUF != NULL)
        return 0;
    else return 1;
}

c_buffer_t *c_mallocBuffer(uint32_t size) {
    int32_t ret = 0;
    c_buffer_t *BUF = NULL;

    BUF = (c_buffer_t *)calloc(1, sizeof(c_buffer_t));
    if(BUF == NULL) {
        // ERROR, can't allocate the c_buffer_t
        ret = -1;
        goto FreeVariables;
    }

    /*if(BUF->allocated == 1) {
        return -1;
    }*/
    BUF->buffer = NULL;
    BUF->allocatedSize = 0;
    BUF->allocated = 0;

    BUF->buffer = malloc(size);
    if(BUF->buffer == NULL) {
        // ERROR can't allocate buffer
        ret = -1;
        goto FreeVariables;
    }

    BUF->allocatedSize = size;
    BUF->allocated = 1;
    if(ret == 0) {
        goto QuitFunction;
    }

FreeVariables:
    BUF->allocatedSize = 0;
    BUF->allocated = 0;
    if(BUF->buffer != NULL) {
        free(BUF->buffer);
        BUF->buffer = NULL;
    }
    if(BUF != NULL) {
        free(BUF);
        BUF = NULL;
    }

QuitFunction:
    return BUF;
}

c_buffer_t *c_callocBuffer(uint32_t Counts, uint32_t Size) {
    c_buffer_t *BUF = c_mallocBuffer(Counts*Size);
    if(BUF!=NULL)
        memset(BUF->buffer, 0, BUF->allocatedSize);
    return BUF;
}

c_buffer_t *c_reallocBuffer(c_buffer_t *BUF, uint32_t size) {
    if(_checkBuf(BUF)) {
        if(BUF->allocated != 0) {
            if(BUF->allocatedSize >= size)
                goto QuitFunction; // HACK, if the buffer is already big enough we give it back
            else
                BUF = c_freeBuffer(BUF);
        }
    }
    BUF = c_mallocBuffer(size);

QuitFunction:
    return BUF;
}

c_buffer_t *c_freeBuffer(c_buffer_t *BUF) {
    if(_checkBuf(BUF))
        return NULL;
    if(BUF->allocated != 1) {
        // BUFFER NOT ALLOCATED!
        if(BUF != NULL) {
            BUF = NULL;
            return BUF;
        }
    }

    if(BUF->buffer != NULL) {
        free(BUF->buffer);
    }

    BUF->allocated = 0;
    BUF->allocatedSize = 0;
    free(BUF);
    return NULL;
}
