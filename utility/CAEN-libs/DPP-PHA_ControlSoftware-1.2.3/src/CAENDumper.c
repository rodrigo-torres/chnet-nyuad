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

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <CAENDumper.h>
#include <CAENDigitizerType.h>
#include <CAENLogger.h>

INIT_C_LOGGER("DumperLog.txt", "CAENDumper.c");

static int _decodeError(const char *functName, int resCode) {
    int ret = c_DPRErrorCode_Success;
    char ErrMsg[100];

    switch(resCode) {
    case c_DPRErrorCode_WrongOpenMode:
        strcpy(ErrMsg, "Invalid Open Mode");
        break;
    case c_DPRErrorCode_OutOfMemory:
        strcpy(ErrMsg, "Out Of Memory");
        break;
    case c_DPRErrorCode_DumperClosed:
        strcpy(ErrMsg, "The Dumper is Closed");
        break;
    case c_DPRErrorCode_InvalidDPPType:
        strcpy(ErrMsg, "Invalid DPP Type");
        break;
    case c_DPRErrorCode_InvalidDPPDescriptor:
        strcpy(ErrMsg, "Invalid DPP Descriptor");
        break;
    case c_DPRErrorCode_InvalidCursorPositin:
        strcpy(ErrMsg, "Invalid Cursor Position");
        break;
    case c_DPRErrorCode_CantOpenFile:
        strcpy(ErrMsg, "Error Opening Dump File");
        break;
    case c_DPRErrorCode_DumperNotReady:
        strcpy(ErrMsg, "Dumper Not Ready");
        break;
    case c_DPRErrorCode_TSSizeNotSupported:
        strcpy(ErrMsg, "Time Tag Size not Supported");
        break;
    case c_DPRErrorCode_EnSizeNotSupported:
        strcpy(ErrMsg, "Energy Size not Supported");
        break;
    case c_DPRErrorCode_ErrorEventRead:
        strcpy(ErrMsg, "Error Reading Event");
        break;
    case c_DPRErrorCode_ZeroDivision:
        strcpy(ErrMsg, "Zero Division Found");
        break;
    case c_DPRErrorCode_WrongOpenFormat:
        strcpy(ErrMsg, "Invalid Open Format");
        break;
    default:
        strcpy(ErrMsg, "Generic Error Occurred!");
        break;
    }
    ret = logMsg(c_logger_Severity_ERROR, "From %s: %s", functName, ErrMsg);

    return ret;
}

void _initEC(c_eventCollection_t *ec) {
    ec->BUF = NULL;
    ec->maxAllocEvts = 0;
    ec->lastEvtPtr = NULL;
    ec->nBuffEvts = 0;
}

int32_t _freeEC(c_eventCollection_t *ec) {
    if(ec->BUF != NULL)
        ec->BUF = c_freeBuffer(ec->BUF);
    _initEC(ec);
    return c_DPRErrorCode_Success;
}

int _mallocEC(c_eventCollection_t *ec, uint32_t maxNEvts, uint32_t evtSize) {
    uint32_t totalSize = maxNEvts*evtSize;
    int ret = 0;

    ec->BUF = c_mallocBuffer(totalSize);
    if(ec->BUF == NULL) {
        // ERROR, can't allocate buffer
        logMsg(c_logger_Severity_ERROR, "Can't allocate buffer for events");
        ret = c_DPRErrorCode_OutOfMemory;
        goto QuitFunction;
    }
    ec->lastEvtPtr = ec->BUF->buffer;
    ec->maxAllocEvts = maxNEvts;
    ec->nBuffEvts = 0;

QuitFunction:
    if(ret) {
        if(ec->BUF != NULL)
            _freeEC(ec);
    }
    return ret;
}

void _initDpr(c_dumper_t *dpr) {
    dpr->DPPDescr = DPP_None_Descr;
    dpr->f = NULL;
    dpr->fileName = NULL;
    
    _initEC(&dpr->evtsBuff);
}

void _setStatusBit(c_dumper_t *dpr, int32_t bitNum, int32_t value) {
    int32_t tmp = 0x1<<bitNum;
    if(value != 0) {
        dpr->status |= tmp;
    }
    else {
        tmp = ~tmp;
        dpr->status &= tmp;
    }

    return;
}

int32_t _getStatusBit(c_dumper_t *dpr, int32_t bitNum) {
    int32_t res = ((dpr->status)>>bitNum)&0x1;
    return res;
}

void _waitDumperFree(c_dumper_t *dpr) {
    int32_t busy = 1;
    while(busy) {
        c_getDumperBusy(dpr, &busy);
    }

    return;
}

int32_t _waitDumperReady(c_dumper_t *dpr, c_mode_t mode) {
    if(c_isDumperOpened(dpr) != 1) {
        // ERROR, the dumper is closed
        logMsg(c_logger_Severity_ERROR, "The dumper is closed");
        return c_DPRErrorCode_DumperClosed;
    }

    switch(mode) {
    case c_mode_R: {
        // if we want read mode, the dumper must not be opened in write only mode
        switch (dpr->mode) {
        case c_mode_A:
        case c_mode_W:
            // ERROR, the dumper is not opened in read mode
            logMsg(c_logger_Severity_ERROR, "The dumper is not opened in read mode");
            return c_DPRErrorCode_WrongOpenMode;
        default:
            // The dumper is opened with a read flag, so we continue
            break;
                   }
        break;
                   }
    case c_mode_A:
    case c_mode_W: {
        // if we want write mode, the dumper must not be opened in read only mode
        switch (dpr->mode) {
        case c_mode_R:
            // ERROR, the dumper is not opened in write mode
            logMsg(c_logger_Severity_ERROR, "The dumper is not opened in write mode");
            return c_DPRErrorCode_WrongOpenMode;
        default:
            // The dumper is opened with a read flag, so we continue
            break;
                   }
        break;
                   }
    default:
        // All open mode checks passed
        break;
    }

    // wait for dumper to be free
    // NOTE, the following function should use semaphores!
    _waitDumperFree(dpr);

    return 0;
}

size_t _dwrite(const void * _Str, size_t _Size, size_t _Count, c_dumper_t *dpr) {
    int32_t res;
    size_t size;

    /**********************************************/
    // Perform some initial checks
    /**********************************************/
    res = _waitDumperReady(dpr, c_mode_W);
    if(res) {
        // ERROR, can't initialize dumper
        logMsg(c_logger_Severity_ERROR, "Can't initialize dumper");
        return 0;
    }

    /**********************************************/
    // Lock the dumper
    /**********************************************/
    c_setDumperBusy(dpr, 1);

    res = c_setWriteCursorPosition(dpr, &dpr->wPos);
    if(res) {
        size = 0;
        goto QuitFunction;
    }
    size = fwrite(_Str, _Size, _Count, dpr->f);

    // update the cursor position
    res = fgetpos(dpr->f, &dpr->wPos);

QuitFunction:
    /**********************************************/
    // Free the dumper
    /**********************************************/
    c_setDumperBusy(dpr, 0);

    return size;
}

size_t _dread(void * _Dst, size_t _Size, size_t _Count, c_dumper_t *dpr) {
    int32_t res;
    size_t size;

    /**********************************************/
    // Perform some initial checks
    /**********************************************/
    res = _waitDumperReady(dpr, c_mode_R);
    if(res) {
        // ERROR, can't initialize dumper
        logMsg(c_logger_Severity_ERROR, "Can't initialize dumper");
        return 0;
    }

    /**********************************************/
    // Lock the dumper
    /**********************************************/
    c_setDumperBusy(dpr, 1);

    if(feof(dpr->f)) {
        /* We had the end of file flag */
        size = 0;
        goto QuitFunction;
    }

    res = c_setReadCursorPosition(dpr, &dpr->rPos);
    if(res) {
        // ERROR add log message
        // can't set cursor position
        // return that we didn't read a byte
        logMsg(c_logger_Severity_ERROR, "Can't set read cursor position");
        size = 0;
        goto QuitFunction;
    }
    size = fread(_Dst, _Size, _Count, dpr->f);

    // update the cursor position
    res = fgetpos(dpr->f, &dpr->rPos);

QuitFunction:
    /**********************************************/
    // Free the dumper
    /**********************************************/
    c_setDumperBusy(dpr, 0);

    return size;
}

size_t _dprintf(c_dumper_t *dpr, const char* _Format, ...) {
    int32_t res;
    int32_t size;
    va_list args;

    /**********************************************/
    // Perform some initial checks
    /**********************************************/
    res = _waitDumperReady(dpr, c_mode_W);
    if(res) {
        // ERROR, can't initialize dumper
        logMsg(c_logger_Severity_ERROR, "Can't initialize dumper");
        return 0;
    }

    /**********************************************/
    // Lock the dumper
    /**********************************************/
    c_setDumperBusy(dpr, 1);

    res = c_setWriteCursorPosition(dpr, &dpr->wPos);
    if(res) {
        size = 0;
        goto QuitFunction;
    }
    
	va_start(args, _Format);
    size = vfprintf(dpr->f, _Format, args);
	va_end(args);


    // update the cursor position
    res = fgetpos(dpr->f, &dpr->wPos);

QuitFunction:
    /**********************************************/
    // Free the dumper
    /**********************************************/
    c_setDumperBusy(dpr, 0);

    return size;
}

void _fillFakeEvent(c_event_t *evt, int32_t TSSize, int32_t EnSize) {
    uint64_t fakeTS = FAKE_EVT_TS;
    uint32_t fakeEn = FAKE_EVT_EN;
    
    memcpy(evt->TimeStamp, &fakeTS, TSSize);
    memcpy(evt->Energy, &fakeEn, EnSize);

    return;
}

int32_t _isDPPDescriptorValid(c_DPPDescriptor_t DPPDescr) {
    switch(DPPDescr.DPPType) {
    case c_DPPType_CI:
    case c_DPPType_PHA:
    case c_DPPType_PSD: {
        switch(DPPDescr.TSType) {
        case TYPE_UINT32:
        case TYPE_UINT64: {
            break;
                          }
        default: {
            return -1;
                 }
        }

        switch(DPPDescr.ENType) {
        case TYPE_INT16:
        case TYPE_UINT16:
        case TYPE_INT32:
        case TYPE_UINT32: {
            break;
                          }
        default: {
            return -1;
                 }
        }
    break;
                        }
    default:
        // No valid DPPType Specified
        return -2;
    }

    return 0;
}

c_dumper_t *c_openDumper(char *filename, c_mode_t MODE, c_saveFormat_t FMT, c_DPPDescriptor_t DPPDescr) {
    int32_t ret=c_DPRErrorCode_Success;
    uint32_t totalEvtSize = 0;

    c_dumper_t *dpr = (c_dumper_t *)calloc(1, sizeof(c_dumper_t));
    if(dpr==NULL) {
        ret = c_DPRErrorCode_OutOfMemory;
        logMsg(c_logger_Severity_ERROR, "Can't Allocate Memory for a Dumper!");
        goto QuitFunction;
    }

    _initDpr(dpr);

    /*if(c_isDumperOpened(dpr)) {
        // HACK add error log message
        // ERROR, dumper already opened
        logMsg(c_logger_Severity_ERROR, "The dumper is already opened");
        dpr = NULL;
        return dpr;
    }*/

    // Validate the DPP Descriptor (only if it has a valid DPP Type)
    switch(DPPDescr.DPPType) {
    case c_DPPType_CI:
    case c_DPPType_PHA:
    case c_DPPType_PSD: {
        if( _isDPPDescriptorValid(DPPDescr) != 0 ) {
            ret = c_DPRErrorCode_InvalidDPPDescriptor;
            goto QuitFunction;
        }
                        }
    break;
    default: {
        // No valid DPPType Specified
        DPPDescr.DPPType = c_DPPType_None;
        break;
             }
    }

    // Manage  file open flags
    switch(MODE) {
    case c_mode_R: {
        strcpy(dpr->openFlags, "r");
        break;
                   }
    case c_mode_W: {
        strcpy(dpr->openFlags, "w");
        break;
                   }
    case c_mode_A: {
        strcpy(dpr->openFlags, "a");
        break;
                   }
    case c_mode_RW: {
        strcpy(dpr->openFlags, "w+");
        break;
                   }
    case c_mode_RA: {
        strcpy(dpr->openFlags, "a+");
        break;
                   }
    }
    strcat(dpr->openFlags, "b");
    totalEvtSize = c_getSizeOfType(DPPDescr.TSType)+c_getSizeOfType(DPPDescr.ENType);
    ret = _mallocEC(&dpr->evtsBuff, 1023*1024, totalEvtSize); // HACK, size fissa!
    if(ret) {
        // ERROR, can't allocate buffer
        logMsg(c_logger_Severity_ERROR, "Can't allocate buffer for binary write");
        ret = c_DPRErrorCode_OutOfMemory;
        goto QuitFunction;
    }

    // Open the file
    dpr->f = fopen(filename, dpr->openFlags);
    if(dpr->f == NULL) {
        // ERROR, can't open file
        ret = c_DPRErrorCode_CantOpenFile;
        goto QuitFunction;
    }

    ret = fgetpos(dpr->f, &dpr->wPos);
    ret |= fgetpos(dpr->f, &dpr->rPos);
    if(ret) {
        // ERROR can't get initial positions
        logMsg(c_logger_Severity_ERROR, "Can't get initial positions");
        ret = c_DPRErrorCode_InvalidCursorPositin;
        goto QuitFunction;
    }

    dpr->format = FMT;
    dpr->mode = MODE;
    dpr->DPPDescr = DPPDescr;
    dpr->fileName=(char*)malloc((strlen(filename)+1)*sizeof(char));
    if(dpr->fileName == NULL) {
        // HACK add error message
        // ERROR can't get initial positions
        logMsg(c_logger_Severity_ERROR, "Can't allocate dumper filename");
        ret = c_DPRErrorCode_OutOfMemory;
        goto QuitFunction;
    }

    strcpy(dpr->fileName, filename);

    // set dumper status opened
    _setStatusBit(dpr, DPR_STATUSBIT_OPENED, 1);

QuitFunction:
    if(ret<c_DPRErrorCode_Success) {
        // In case of error, we deinit all variables
        if(dpr != NULL) {
            if(dpr->fileName != NULL) {
                free(dpr->fileName);
            }
            if(dpr->f != NULL) {
                fclose(dpr->f);
            }
            if(dpr->evtsBuff.BUF != NULL) {
                _freeEC(&dpr->evtsBuff);
            }
            free(dpr);
            dpr=NULL;
        }
        _decodeError("c_openDumper", ret);
    }
    return dpr;
}

c_dumper_t *_closeDumper_(c_dumper_t *_dpr) {
	int32_t r = c_DPRErrorCode_Success;

	if(_dpr == NULL)
		return _dpr;

    fclose(_dpr->f);
    free(_dpr->fileName);

    if(_dpr->format == c_format_Binary) {
        r = _freeEC(&_dpr->evtsBuff);
    }

    // SET dumper status closed
    _setStatusBit(_dpr, DPR_STATUSBIT_OPENED, 0);
    
    free(_dpr);

    _dpr = NULL;

    if(r != c_DPRErrorCode_Success)
        _decodeError("_closeDumper_", r);
    return _dpr;
}

int32_t c_setWriteCursorPosition(c_dumper_t *dpr, fpos_t *pos) {
    int32_t res = c_DPRErrorCode_Success;

    if(!c_isDumperOpened(dpr)) {
		res=c_DPRErrorCode_DumperClosed;
		goto QuitFunction;
	}

    res = fsetpos(dpr->f, pos);
    if(res != c_DPRErrorCode_Success) {
        // if we had an error, go back to previous position
        fsetpos(dpr->f, &dpr->wPos);
    }
    else {
        // if we don't have errors, update the cursor position
        dpr->wPos = *pos;
    }

QuitFunction:
    if(res != c_DPRErrorCode_Success)
        _decodeError("c_setWriteCursorPosition", res);
    return res;
}

int32_t c_getWriteCursorPosition(c_dumper_t *dpr, fpos_t *pos) {
    int32_t res=c_DPRErrorCode_Success;

    if(!c_isDumperOpened(dpr)) {
		res=c_DPRErrorCode_DumperClosed;
		goto QuitFunction;
	}

    *pos = dpr->wPos;

QuitFunction:
    if(res != c_DPRErrorCode_Success)
        _decodeError("c_getWriteCursorPosition", res);
    return res;
}

int32_t c_setReadCursorPosition(c_dumper_t *dpr, fpos_t *pos) {
    int32_t res=c_DPRErrorCode_Success;

    if(!c_isDumperOpened(dpr)) {
		res=c_DPRErrorCode_DumperClosed;
		goto QuitFunction;
	}

    res = fsetpos(dpr->f, pos);
    if(res!=c_DPRErrorCode_Success) {
        // if we had an error, go back to previous position
        fsetpos(dpr->f, &dpr->rPos);
    }
    else {
        // if we don't have errors, update the cursor position
        dpr->rPos = *pos;
    }

QuitFunction:
    if(res != c_DPRErrorCode_Success)
        _decodeError("c_setReadCursorPosition", res);
    else // HACK is good to do that? what happens if someone set the pointer to the EOF?
        clearerr(dpr->f);
    return res;
}

int32_t c_getReadCursorPosition(c_dumper_t *dpr, fpos_t *pos) {
    int32_t res=c_DPRErrorCode_Success;

    if(!c_isDumperOpened(dpr)) {
		res=c_DPRErrorCode_DumperClosed;
		goto QuitFunction;
	}

    *pos = dpr->rPos;

QuitFunction:
    if(res != c_DPRErrorCode_Success)
        _decodeError("c_getReadCursorPosition", res);
    return res;
}

int32_t c_isDumperOpened(c_dumper_t *dpr) {
	int32_t res;
	
	if(dpr==NULL)
		return 0;

    res = _getStatusBit(dpr, DPR_STATUSBIT_OPENED);
    if(res < c_DPRErrorCode_Success) // Do not print the error if the dumper is opened
        _decodeError("c_isDumperOpened", res);
    return res;
}

int32_t c_setDumperBusy(c_dumper_t *dpr, int32_t busy) {
    int32_t res=c_DPRErrorCode_Success;

    if(c_isDumperOpened(dpr)==0) {
		res=c_DPRErrorCode_DumperClosed;
		goto QuitFunction;
	}

    _setStatusBit(dpr, DPR_STATUSBIT_BUSY, busy);

QuitFunction:
    if(res != c_DPRErrorCode_Success)
        _decodeError("c_setDumperBusy", res);
    return res;
}

int32_t c_getDumperBusy(c_dumper_t *dpr, int32_t *busy) {
    int32_t res=c_DPRErrorCode_Success;

    if(c_isDumperOpened(dpr)==0) {
		res=c_DPRErrorCode_DumperClosed;
		goto QuitFunction;
	}

    *busy = _getStatusBit(dpr, DPR_STATUSBIT_BUSY);
    
QuitFunction:
    if(res != c_DPRErrorCode_Success)
        _decodeError("c_getDumperBusy", res);
    return res;
}


int32_t c_dumpEvent(c_dumper_t *dpr, c_event_t evt, c_type_t timeStampType, c_type_t energyType) {
    int32_t res = c_DPRErrorCode_Success;
    int32_t EnSize;
    int32_t TSSize;
    int32_t isPSDEvent = 0;
    
	if(c_isDumperOpened(dpr)==0) {
		res=c_DPRErrorCode_DumperClosed;
		goto QuitFunction;
	}

    /**********************************************/
    // Dump the event
    /**********************************************/
    // Check if we are in binary or ASCII mode
    switch(dpr->format) {
    case c_format_Binary: {

        // Get the timestamp size
        switch(timeStampType) {
        case TYPE_UINT32: {
            TSSize = sizeof(uint32_t);
            break;
                          }
        case TYPE_UINT64: {
            TSSize = sizeof(uint64_t);
            break;
                          }
        default: {
            // timeStamp Size not supported
            res = c_DPRErrorCode_TSSizeNotSupported;
            goto QuitFunction;
                 }
        }
        // Get the energy size
        switch(energyType) {
        case TYPE_INT16:
        case TYPE_UINT16: {
            EnSize = sizeof(uint16_t);
            break;
                          }
        case TYPE_INT32:
        case TYPE_UINT32: {
            EnSize = sizeof(uint32_t);
            break;
                          }
        default: {
            // energy Size not supported
            res = c_DPRErrorCode_EnSizeNotSupported;
            goto QuitFunction;
                 }
        }

        /**********************************************/
        // Now perform the binary dump
        /**********************************************/
        _dwrite(evt.TimeStamp, TSSize, 1, dpr);
        _dwrite(evt.Energy, EnSize, 1, dpr);

        break;
                          }
    case c_format_ASCII: {
        char format[20];
        uint64_t *TS = (uint64_t *)evt.TimeStamp;
        uint32_t *EN = (uint32_t *)evt.Energy;

        // Get the timestamp size
        switch(timeStampType) {
        case TYPE_UINT32: {
            strcpy(format, C_FMT_U32);
            break;
                          }
        case TYPE_UINT64: {
            strcpy(format, C_FMT_U64);
            break;
                          }
        default: {
            // timeStamp Size not supported
            res = c_DPRErrorCode_TSSizeNotSupported;
            goto QuitFunction;
                 }
        }

        // Get the energy size
        strcat(format, " ");
        switch(energyType) {
        case TYPE_INT16: {
            strcat(format, C_FMT_I16);
            break;
                         }
        case TYPE_UINT16: {
            strcat(format, C_FMT_U16);
            break;
                          }
        // 32 bits Energies are allowed only for DPP-PSD, so we dump it as LONG SHORT PSD
        case TYPE_UINT32: 
        case TYPE_INT32: { 
            char *FMT;

            isPSDEvent = 1;
            
            if(energyType == TYPE_UINT32) FMT=C_FMT_U16; // if the format is unsigned, we break it into 2 pieces of unsigned 16 bits int
            else if(energyType == TYPE_INT32) FMT=C_FMT_I16; // if the format is signed, we break it into 2 pieces of signed 16 bits int
            strcat(format, FMT);
            strcat(format, " ");
            strcat(format, FMT);
            break;
                         }
        default: {
            // energy Size not supported
            res = c_DPRErrorCode_EnSizeNotSupported;
            goto QuitFunction;
                 }
        }

        if(isPSDEvent == 1) {
            strcat(format, " ");
            strcat(format, "%.5f");
        }
        strcat(format, "\n");

        // Dump the event
        if( !isPSDEvent ) {
            uint64_t TmStmp = *TS;
            uint32_t Ener = *EN;
            // HACK is ok to use a syntax like *value for fprintf? with PSD it didn't work!
            _dprintf(dpr, format, TmStmp, Ener);
        }
        else {
            uint16_t El, Es;
            uint32_t TmStmp;
            double PSD;

            El = (uint16_t)( ((uint32_t)(*EN)) & 0xFFFF);
            Es = (uint16_t)( (((uint32_t)(*EN))>>DPP_PSD_ENOFFSET) & 0xFFFF);
            if(El != 0) {
                PSD = ((double)(El-Es))/(double)El;
            }
            else {
                res = c_DPRErrorCode_ZeroDivision;
                PSD = 1.0; // HACK, we assume here that (0-Es)/0 = 1.0 (since if El=0 we suppose 0<=Es<=El -> Es=0)
                //goto QuitFunction;
            }

            TmStmp = (uint32_t)*TS;
            _dprintf(dpr, format, TmStmp, Es, El, PSD);
        }

        break;
                         }
    }

QuitFunction:
    if(res < c_DPRErrorCode_Success)
        _decodeError("c_dumpEvent", res);
    return res;
}

int32_t c_dumpBlockEvent(c_dumper_t *dpr, uint32_t nevts, c_event_t *evts, c_type_t timeStampType, c_type_t energyType) {
    int32_t res=c_DPRErrorCode_Success;
    int32_t EnSize;
    int32_t TSSize;
    uint32_t e;
    uint32_t nDumped=0;

	if(c_isDumperOpened(dpr)==0) {
		res=c_DPRErrorCode_DumperClosed;
		goto QuitFunction;
	}

    /**********************************************/
    // Dump the events block
    /**********************************************/
    // Check if we are in binary or ASCII mode
    switch(dpr->format) {
    case c_format_Binary: {
        void *p;

        // Get the timestamp size
        switch(timeStampType) {
        case TYPE_UINT32: {
            TSSize = sizeof(uint32_t);
            break;
                          }
        case TYPE_UINT64: {
            TSSize = sizeof(uint64_t);
            break;
                          }
        default: {
            // timeStamp Size not supported
            res = c_DPRErrorCode_TSSizeNotSupported;
            goto QuitFunction;
                 }
        }
        // Get the energy size
        switch(energyType) {
        case TYPE_INT16:
        case TYPE_UINT16: {
            EnSize = sizeof(uint16_t);
            break;
                          }
        case TYPE_INT32:
        case TYPE_UINT32: {
            EnSize = sizeof(uint32_t);
            break;
                          }
        default: {
            // energy Size not supported
            res = c_DPRErrorCode_EnSizeNotSupported;
            goto QuitFunction;
                 }
        }

        /**********************************************/
        // Now perform the binary block dump
        /**********************************************/

        // check if the buffer has enough space
        if( (dpr->evtsBuff.BUF->allocated = 1) && (dpr->evtsBuff.BUF->allocatedSize > (TSSize+EnSize)*nevts)) {
            // The buffer has enough space, we will dump the entire block (higher performance)
            p = dpr->evtsBuff.BUF->buffer;
            // Fill the buffer
            for(e=0; e<nevts; e++) {
                memcpy( p, evts[e].TimeStamp, TSSize);
                p = (char *)p + TSSize;
                memcpy( p, evts[e].Energy, EnSize);
                p = (char *)p + EnSize;
            }
            nDumped = (uint32_t)_dwrite(dpr->evtsBuff.BUF->buffer, TSSize+EnSize, nevts, dpr);
        }
        else {
            // The buffer doesn't have enough space, we will dump one event at a time (slower performance)
            for(e=0; e<nevts; e++) {
                res = c_dumpEvent(dpr, evts[e], timeStampType, energyType);
                if(res) {
                    goto QuitFunction;
                }
                else {
                    nDumped++;
                }
            }
        }

        break;
                          } // end of binary dump
    case c_format_ASCII: {
        uint32_t n;

        // For format ASCII we don't care too much about performance
        // and recall the single dump  function nevt times
        for(n=0; n<nevts; n++) {
            res = c_dumpEvent(dpr, evts[n], timeStampType, energyType);
            if(res) {
                goto QuitFunction;
            }
            else {
                nDumped++;
            }
        }

        break;
                         }
    }

QuitFunction:
    if(res < c_DPRErrorCode_Success) {
        _decodeError("c_dumpBlockEvent", res);
        return res;
    }

    return nDumped;
}

int32_t c_dumpDPPEvent(c_dumper_t *dpr, void *evt) {
    int32_t res=c_DPRErrorCode_Success;
    c_event_t e;
    c_type_t TSType, EnType;
    CAEN_DGTZ_DPP_CI_Event_t *EVTCI;
    CAEN_DGTZ_DPP_PHA_Event_t *EVTPHA;
    CAEN_DGTZ_DPP_PSD_Event_t *EVTPSD;
    uint16_t chargeL, chargeS;
    uint32_t chargeTot; /* chargeTot -> | 31 ... 16 | 15 ... 0 |
                                        |  chargeS  |  chargeL |
                           NOTE: only for DPP-PSD */

	if(!c_isDumperOpened(dpr)) {
		res=c_DPRErrorCode_DumperClosed;
		goto QuitFunction;
	}

    switch(dpr->DPPDescr.DPPType) {
    case c_DPPType_CI: {
        EVTCI = (CAEN_DGTZ_DPP_CI_Event_t *)evt;
        e.TimeStamp = &EVTCI->TimeTag;
        e.Energy =  &EVTCI->Charge;
        break;
                       }
    case c_DPPType_PHA: {
        EVTPHA = (CAEN_DGTZ_DPP_PHA_Event_t *)evt;
        e.TimeStamp = &EVTPHA->TimeTag;
        e.Energy =  &EVTPHA->Energy;
        break;
                        }
    case c_DPPType_PSD: {
        // For DPP-PSD we need to manage the ASCII dump
        EVTPSD = (CAEN_DGTZ_DPP_PSD_Event_t *)evt;
        e.TimeStamp = &EVTPSD->TimeTag;
        chargeL = (uint16_t)EVTPSD->ChargeLong;
        chargeS = (uint16_t)EVTPSD->ChargeShort;
        chargeTot = (chargeS<<DPP_PSD_ENOFFSET) | chargeL;
        e.Energy =  &chargeTot;
        break;
                        }
    default:
        // Uknown DPP type
        res = c_DPRErrorCode_InvalidDPPType;
        goto QuitFunction;
    }

    TSType = dpr->DPPDescr.TSType;
    EnType = dpr->DPPDescr.ENType;

    res = c_dumpEvent(dpr, e, TSType, EnType);

QuitFunction:
    if(res < c_DPRErrorCode_Success)
        _decodeError("c_dumpDPPEvent", res);
    return res;
}

int32_t c_dumpDPPBlockEvent(c_dumper_t *dpr, uint32_t nevts, void *evts) {
    int32_t res=c_DPRErrorCode_Success;
    uint32_t i;
    c_type_t TSType, EnType;
    c_event_t *e = NULL;
    CAEN_DGTZ_DPP_CI_Event_t *EVTCI;
    CAEN_DGTZ_DPP_PHA_Event_t *EVTPHA;
    CAEN_DGTZ_DPP_PSD_Event_t *EVTPSD;
    int16_t chargeL, chargeS;
    uint32_t *chargeTot = NULL; /* chargeTot -> | 31 ... 16 | 15 ... 0 |
                                                |  chargeS  |  chargeL |*/

	if(!c_isDumperOpened(dpr)) {
		res=c_DPRErrorCode_DumperClosed;
		goto QuitFunction;
	}

    e = (c_event_t *)calloc(nevts, sizeof(c_event_t));
    if(e==NULL) {
        // can't allocate event buffer
        res = c_DPRErrorCode_OutOfMemory;
        goto QuitFunction;
    }
    switch(dpr->DPPDescr.DPPType) {
    case c_DPPType_CI: {
        EVTCI = (CAEN_DGTZ_DPP_CI_Event_t *)evts;
        for(i=0; i<nevts; i++) {
            e[i].TimeStamp = &EVTCI[i].TimeTag;
            e[i].Energy =  &EVTCI[i].Charge;
        }
        break;
                       }
    case c_DPPType_PHA: {
        EVTPHA = (CAEN_DGTZ_DPP_PHA_Event_t *)evts;
        for(i=0; i<nevts; i++) {
            e[i].TimeStamp = &EVTPHA[i].TimeTag;
            e[i].Energy =  &EVTPHA[i].Energy;
        }
        break;
                        }
    case c_DPPType_PSD: {
        EVTPSD = (CAEN_DGTZ_DPP_PSD_Event_t *)evts;
        chargeTot = (uint32_t *)calloc(nevts, sizeof(uint32_t));
        if(chargeTot == NULL) {
            // can't allocate buffer for chargeTot
            res = c_DPRErrorCode_OutOfMemory;
            goto QuitFunction;
        }
        for(i=0; i<nevts; i++) {
            e[i].TimeStamp = &EVTPSD[i].TimeTag;
            chargeL = (int16_t)EVTPSD[i].ChargeLong;
            chargeS = (int16_t)EVTPSD[i].ChargeShort;
            chargeTot[i] = (chargeS<<DPP_PSD_ENOFFSET)+chargeL;
            e[i].Energy =  &chargeTot[i];
        }
        break;
                        }
    default:
        // Uknown DPP type
        res = c_DPRErrorCode_InvalidDPPType;
        goto QuitFunction;
    }

    TSType = dpr->DPPDescr.TSType;
    EnType = dpr->DPPDescr.ENType;

    res = c_dumpBlockEvent(dpr, nevts, e, TSType, EnType);

QuitFunction:
    if(e != NULL)
        free(e);
    if(chargeTot != NULL)
        free(chargeTot);
    if(res < c_DPRErrorCode_Success)
        _decodeError("c_dumpDPPBlockEvent", res);
    return res;
}

int32_t c_readEvent(c_dumper_t *dpr, c_event_t *evt, c_type_t timeStampType, c_type_t energyType) {
    int32_t res = c_DPRErrorCode_Success;
    int32_t EnSize;
    int32_t TSSize;

	if(!c_isDumperOpened(dpr)) {
		res=c_DPRErrorCode_DumperClosed;
		goto QuitFunction;
	}

    /**********************************************/
    // Read the event
    /**********************************************/
    // Check if we are in binary or ASCII mode
    switch(dpr->format) {
    case c_format_Binary: {
        size_t readSize=0;

        // Get the timestamp size
        switch(timeStampType) {
        case TYPE_UINT32: {
            TSSize = sizeof(uint32_t);
            break;
                          }
        case TYPE_UINT64: {
            TSSize = sizeof(uint64_t);
            break;
                          }
        default: {
            // timeStamp Size not supported
            res = c_DPRErrorCode_TSSizeNotSupported;
            goto QuitFunction;
                 }
        }
        // Get the energy size
        switch(energyType) {
        case TYPE_INT16: {
            EnSize = sizeof(int16_t);
            break;
                          }
        case TYPE_UINT16: {
            EnSize = sizeof(uint16_t);
            break;
                          }
        case TYPE_INT32: {
            EnSize = sizeof(int32_t);
            break;
                          }
        case TYPE_UINT32: {
            EnSize = sizeof(uint32_t);
            break;
                          }
        default: {
            // energy Size not supported
            res = c_DPRErrorCode_EnSizeNotSupported;
            goto QuitFunction;
                 }
        }

        /**********************************************/
        // Now perform the binary read
        /**********************************************/
        readSize = _dread(evt->TimeStamp, TSSize, 1, dpr);
        readSize += _dread(evt->Energy, EnSize, 1, dpr);
        if(readSize != 2) {
            // We didn' read the expected number of events (1 TS and 1 En), maybe eof()
            _fillFakeEvent(evt, TSSize, EnSize);
            res = c_DPRErrorCode_ErrorEventRead;
        }
        break;
                          }
    case c_format_ASCII: {
        char format[20];
        int32_t isPSDEvent=0;
        int32_t expectedNRead=2;

        res=c_setReadCursorPosition(dpr, &dpr->rPos);
        if(res) {
            // Error, can't set read cursor position
            return c_DPRErrorCode_InvalidCursorPositin;
        }

        switch(timeStampType) {
        case TYPE_UINT32: {
            strcpy(format, C_FMT_U32);
            break;
                          }
        case TYPE_UINT64: {
            strcpy(format, C_FMT_U64);
            break;
                          }
        default: {
            // timeStamp Size not supported
            res = c_DPRErrorCode_TSSizeNotSupported;
            goto QuitFunction;
                 }
        }

        strcat(format, " ");

        // Get the energy size
        switch(energyType) {
        case TYPE_UINT16: {
            strcat(format, C_FMT_U16);
            break;
                          }
        case TYPE_INT32: {
            // This is for DPP-PSD
            isPSDEvent=1;
            expectedNRead=3; // TimeStamp, ChargeShort, ChargeLong
            strcat(format, C_FMT_I16);
            strcat(format, " ");
            strcat(format, C_FMT_I16);
            break;
                          }
        default: {
            // energy Size not supported
            res = c_DPRErrorCode_EnSizeNotSupported;
            goto QuitFunction;
                 }
        }

        // Since in windows we don't have a function vfscanf we can't create a function like _dscanf(...)
        // We need to do "by hand" inline operations like lockdumper, setReadCursorPosition, freedumper, etc
        /**********************************************/
        // Perform some initial checks
        /**********************************************/
        res = _waitDumperReady(dpr, c_mode_R);
        if(res) {
            // ERROR, can't initialize dumper
            res = c_DPRErrorCode_DumperNotReady;
            goto QuitFunction;
        }

        /**********************************************/
        // Lock the dumper
        /**********************************************/
        c_setDumperBusy(dpr, 1);

        if(isPSDEvent == 0) {
            res = fscanf(dpr->f, format, evt->TimeStamp, evt->Energy);
        }
        else {
            char tmpstr[100];
            char *tmp; // to avoid compiler warnings
            res = fscanf(dpr->f, format, evt->TimeStamp, evt->Energy, ((char*)evt->Energy+sizeof(DPP_PSD_ENSIZES)));
            tmp = fgets(tmpstr, 100, dpr->f);
        }
        res|=fgetpos(dpr->f, &dpr->rPos);
        if(res != expectedNRead) {
            res = c_DPRErrorCode_ErrorEventRead;
        }
        else {
            res = 0;
        }

        /**********************************************/
        // Free the dumper
        /**********************************************/
        c_setDumperBusy(dpr, 0);

        break;
                         }
    }

QuitFunction:
    if(res < c_DPRErrorCode_Success)
        _decodeError("c_readEvent", res);
    return res;
}

int32_t c_readBlockEvent(c_dumper_t *dpr, uint32_t nevts, c_event_t *evts, c_type_t timeStampType, c_type_t energyType) {
    int32_t res=c_DPRErrorCode_Success;
    int32_t EnSize;
    int32_t TSSize;
    uint32_t e;
    uint32_t nRead=0;

	if(!c_isDumperOpened(dpr)) {
		res=c_DPRErrorCode_DumperClosed;
		goto QuitFunction;
	}

    /**********************************************/
    // Read the events block
    /**********************************************/
    // Check if we are in binary or ASCII mode
    switch(dpr->format) {
    case c_format_Binary: {
        void *p;

        // Get the timestamp size
        switch(timeStampType) {
        case TYPE_UINT32: {
            TSSize = sizeof(uint32_t);
            break;
                          }
        case TYPE_UINT64: {
            TSSize = sizeof(uint64_t);
            break;
                          }
        default: {
            // timeStamp Size not supported
            res = c_DPRErrorCode_TSSizeNotSupported;
            goto QuitFunction;
                 }
        }
        // Get the energy size
        switch(energyType) {
        case TYPE_INT16: {
            EnSize = sizeof(int16_t);
            break;
                          }
        case TYPE_UINT16: {
            EnSize = sizeof(uint16_t);
            break;
                          }
        case TYPE_INT32: {
            EnSize = sizeof(int32_t);
            break;
                          }
        case TYPE_UINT32: {
            EnSize = sizeof(uint32_t);
            break;
                          }
        default: {
            // energy Size not supported
            res = c_DPRErrorCode_EnSizeNotSupported;
            goto QuitFunction;
                 }
        }

        /**********************************************/
        // Now perform the binary block read
        /**********************************************/

        // check if the buffer has enough space
        if( (dpr->evtsBuff.BUF->allocated = 1) && (dpr->evtsBuff.BUF->allocatedSize > (TSSize+EnSize)*nevts)) {
            // The buffer has enough space, we will read the entire block (higher performance)
            p = dpr->evtsBuff.BUF->buffer;
            nRead = (uint32_t)_dread(p, TSSize+EnSize, nevts, dpr);

            // Fill the events
            for(e=0; e<nevts; e++) {
                if(e<nRead) {
                    // This is an event effectively read from the file, so we use it
                    memcpy( evts[e].TimeStamp, p, TSSize);
                    p = (char *)p + TSSize;
                    memcpy( evts[e].Energy, p, EnSize);
                    p = (char *)p + EnSize;
                }
                else {
                    // This event wasn't read from the file, so we mark it as fake event
                    _fillFakeEvent(&evts[e], TSSize, EnSize);
                }
            }
        }
        else {
            // The buffer doesn't have enough space, we will dump one event at a time (slower performance)
            for(e=0; e<nevts; e++) {
                if(c_readEvent(dpr, &evts[e], timeStampType, energyType) == 0) {
                    // This is an event effectively read from the file, so we use it
                    nRead++;
                }
            }
        }

        break;
                          } // end of binary read
    case c_format_ASCII: {
        uint32_t n;

        // For format ASCII we don't care too much about performance
        // and recall the single read function nevt times
        for(n=0; n<nevts; n++) {
            int32_t readRes=0;
            readRes = c_readEvent(dpr, &evts[n], timeStampType, energyType);
            if(readRes) {
                goto QuitFunction;
            }
            else {
                nRead++;
            }
        }
        break;
                         }
    }

QuitFunction:
    if(res < c_DPRErrorCode_Success) {
        _decodeError("c_readBlockEvent", res);
        return res;
    }

    return nRead;
}

int32_t c_readDPPEvent(c_dumper_t *dpr, void *evt) {
    int32_t res=c_DPRErrorCode_Success;
    c_type_t TSType, EnType;
    int32_t TSSize, EnSize;
    int32_t ENTot=0;
    c_event_t e;
    
    /* chargeTot -> | 31 ... 16 | 15 ... 0 |
                    |  chargeS  |  chargeL |*/
    //uint32_t *chargeTot = NULL;

	if(!c_isDumperOpened(dpr)) {
		res=c_DPRErrorCode_DumperClosed;
		goto QuitFunction;
	}

    TSType = dpr->DPPDescr.TSType;
    EnType = dpr->DPPDescr.ENType;
    
    TSSize = c_getSizeOfType(TSType);
    EnSize = c_getSizeOfType(EnType);
    
    //res = c_readEvent(dpr, &e, TSType, EnType);

    switch(dpr->DPPDescr.DPPType) {
    case c_DPPType_CI: {
        //CAEN_DGTZ_DPP_CI_Event_t *EVT = (CAEN_DGTZ_DPP_CI_Event_t *)evt;
        memset(evt, 0, sizeof(CAEN_DGTZ_DPP_CI_Event_t));
        /*memcpy(&EVT->TimeTag, e.TimeStamp, TSSize);
        memcpy(&EVT->Charge, e.Energy, EnSize);*/
        e.TimeStamp = &((CAEN_DGTZ_DPP_CI_Event_t *)evt)->TimeTag;
        e.Energy = &((CAEN_DGTZ_DPP_CI_Event_t *)evt)->Charge;
        break;
                       }
    case c_DPPType_PHA: {
        //CAEN_DGTZ_DPP_PHA_Event_t *EVT = (CAEN_DGTZ_DPP_PHA_Event_t *)evt;
        memset(evt, 0, sizeof(CAEN_DGTZ_DPP_PHA_Event_t));
        /*memcpy(&EVT->TimeTag, e.TimeStamp, TSSize);
        memcpy(&EVT->Energy, e.Energy, EnSize);*/
        e.TimeStamp = &((CAEN_DGTZ_DPP_PHA_Event_t *)evt)->TimeTag;
        e.Energy = &((CAEN_DGTZ_DPP_PHA_Event_t *)evt)->Energy;
        break;
                        }
    case c_DPPType_PSD: {
        //CAEN_DGTZ_DPP_PSD_Event_t *EVT = (CAEN_DGTZ_DPP_PSD_Event_t *)evt;
        memset(evt, 0, sizeof(CAEN_DGTZ_DPP_PSD_Event_t));
        /*memcpy(&EVT->TimeTag, e.TimeStamp, TSSize);
        memcpy(&EVT->ChargeLong, (DPP_PSD_ENSIZEL*)e.Energy, sizeof(DPP_PSD_ENSIZEL));
        memcpy(&EVT->ChargeShort, (DPP_PSD_ENSIZES*)((char*)e.Energy+sizeof(DPP_PSD_ENSIZEL)), sizeof(DPP_PSD_ENSIZES));*/
        e.TimeStamp = &((CAEN_DGTZ_DPP_PSD_Event_t *)evt)->TimeTag;
        e.Energy = &ENTot;
        break;
                        }
    default:
        // Uknown DPP type
        res = c_DPRErrorCode_InvalidDPPType;
        goto QuitFunction;
    }

    res = c_readEvent(dpr, &e, TSType, EnType);
    if(res!=c_DPRErrorCode_Success)
        goto QuitFunction;

    if(dpr->DPPDescr.DPPType==c_DPPType_PSD) {
        CAEN_DGTZ_DPP_PSD_Event_t *EVT = (CAEN_DGTZ_DPP_PSD_Event_t *)evt;
        EVT->ChargeLong = ENTot&((0x1<<DPP_PSD_ENOFFSET)-1);
        EVT->ChargeShort = ((ENTot>>DPP_PSD_ENOFFSET)&((0x1<<(sizeof(DPP_PSD_ENSIZES)*8))-1));
    }


QuitFunction:
    if(res < c_DPRErrorCode_Success)
        _decodeError("c_readDPPEvent", res);
    return res;
}


int32_t c_readDPPBlockEvent(c_dumper_t *dpr, uint32_t nevts, void *evts) {
    int32_t res=c_DPRErrorCode_Success;
    uint32_t i;
    uint32_t nRead=0;

    CAEN_DGTZ_DPP_CI_Event_t *EVTCI;
    CAEN_DGTZ_DPP_PHA_Event_t *EVTPHA;
    CAEN_DGTZ_DPP_PSD_Event_t *EVTPSD;

	if(!c_isDumperOpened(dpr)) {
		res=c_DPRErrorCode_DumperClosed;
		goto QuitFunction;
	}

    switch(dpr->DPPDescr.DPPType) {
    case c_DPPType_CI: {
        EVTCI = (CAEN_DGTZ_DPP_CI_Event_t *)evts;
        for(i=0; i<nevts; i++) {
            if(c_readDPPEvent(dpr, &EVTCI[i])==0) {
                nRead++;
            }
        }
        break;
                       }
    case c_DPPType_PHA: {
        EVTPHA = (CAEN_DGTZ_DPP_PHA_Event_t *)evts;
        for(i=0; i<nevts; i++) {
            if(c_readDPPEvent(dpr, &EVTPHA[i])==0) {
                nRead++;
            }
        }
        break;
                        }
    case c_DPPType_PSD: {
        EVTPSD = (CAEN_DGTZ_DPP_PSD_Event_t *)evts;
        for(i=0; i<nevts; i++) {
            if(c_readDPPEvent(dpr, &EVTPSD[i])==0) {
                nRead++;
            }
        }
        break;
                        }
    default:
        // Uknown DPP type
        res = c_DPRErrorCode_InvalidDPPType;
        goto QuitFunction;
    }

QuitFunction:
    if(res < c_DPRErrorCode_Success) {
        _decodeError("c_readDPPBlockEvent", res);
        return res;
    }

    return nRead;
}

int32_t c_dadde(c_dumper_t *dpr, void *TTPtr, void *ENPtr) {
    int32_t res=c_DPRErrorCode_Success;
    char *p;
    uint32_t TTSize;
    uint32_t ENSize;

    if(!c_isDumperOpened(dpr)) {
		res=c_DPRErrorCode_DumperClosed;
		goto QuitFunction;
	}
    if(dpr->evtsBuff.BUF->buffer == NULL) {
		res=c_DPRErrorCode_OutOfMemory;
		goto QuitFunction;
	}

    p = (char *)dpr->evtsBuff.lastEvtPtr;
    TTSize=c_getSizeOfType(dpr->DPPDescr.TSType);
    ENSize=c_getSizeOfType(dpr->DPPDescr.ENType);

    if(dpr->evtsBuff.nBuffEvts >= dpr->evtsBuff.maxAllocEvts) {
        logMsg(c_logger_Severity_ERROR, "Dumper Buffer Full");
        res = c_DPRErrorCode_OutOfMemory;
        goto QuitFunction;
    }
    if(p+TTSize+ENSize > (char *)(dpr->evtsBuff.BUF->buffer)+dpr->evtsBuff.BUF->allocatedSize ) {
        logMsg(c_logger_Severity_ERROR, "Too few memory in buffer!");
        res = c_DPRErrorCode_OutOfMemory;
        goto QuitFunction;
    }

    memcpy(p, TTPtr, TTSize);
    p += TTSize;
    memcpy(p, ENPtr, ENSize);
    p += ENSize;
    dpr->evtsBuff.lastEvtPtr = p;
    dpr->evtsBuff.nBuffEvts++;

QuitFunction:
    if(res < c_DPRErrorCode_Success)
        _decodeError("c_dadde", res);
    return res;
}

int32_t c_ddumpb(c_dumper_t *dpr) {
    int32_t res=c_DPRErrorCode_Success;
    uint32_t TTSize;
    uint32_t totEvtSize;

    if(!c_isDumperOpened(dpr)) {
		res=c_DPRErrorCode_DumperClosed;
		goto QuitFunction;
	}
    if(dpr->evtsBuff.BUF->buffer == NULL) {
		res=c_DPRErrorCode_OutOfMemory;
		goto QuitFunction;
	}

    TTSize = c_getSizeOfType(dpr->DPPDescr.TSType);
    totEvtSize = TTSize + c_getSizeOfType(dpr->DPPDescr.ENType);

    if(dpr->format == c_format_ASCII) {
        c_event_t ev;
        int32_t i;

        for(i=0; i<dpr->evtsBuff.nBuffEvts; i++) {
            ev.TimeStamp = (char *)dpr->evtsBuff.BUF->buffer+(i*totEvtSize);
            ev.Energy = (char *)dpr->evtsBuff.BUF->buffer+((i*totEvtSize)+TTSize);
            res = c_dumpEvent(dpr, ev, dpr->DPPDescr.TSType, dpr->DPPDescr.ENType);
            if(res) {
                if(res==c_DPRErrorCode_ZeroDivision)
                    logMsg(c_logger_Severity_WARNING, "Zero Division dumping Event");
                else
                    goto QuitFunction;
            }
        }
    }
    else {
        size_t nevts;
        nevts = _dwrite(dpr->evtsBuff.BUF->buffer, totEvtSize, dpr->evtsBuff.nBuffEvts, dpr);
        if(nevts != dpr->evtsBuff.nBuffEvts) {
            goto QuitFunction;
        }
    }

    dpr->evtsBuff.nBuffEvts = 0;
    dpr->evtsBuff.lastEvtPtr = dpr->evtsBuff.BUF->buffer;
QuitFunction:
    if(res < c_DPRErrorCode_Success)
        _decodeError("c_ddumpb", res);
    return res;
}
