#include <stdlib.h>
#include <string.h>
#include <CAENLogger.h>

static _c_intlogger_t *lGlobal=NULL;
static char *severityMap[] = {
    "[INFO]",
    "[ERROR]",
    "[WARNING]",
    "[DEBUG]",
};


int32_t _isLGlobalConfigured() {
    int32_t result=0;
    if(lGlobal != NULL) {
        result = (lGlobal->status>>LGR_STATUSBIT_CONFIGURED)&0x1;
    }
    return result;
}

int32_t _lvmsgf(c_logger_t *logger, c_logger_Severity s, char *_fmt, va_list args) {
    int32_t res = c_logger_Success;
    char *newFmt = NULL, *fmt=NULL, *tmp, *header=NULL;
    char headerFmt[]="\n%s: %s\n\t";
    char elapsedTime[30];
    uint32_t size;
    uint32_t nReturns = 0, i;

    if(!_isLGlobalConfigured()) {
        res = c_logger_LoggerNotConfigured;
        goto QuitFunction;
    }
    
    fmt = (char *)calloc(strlen(_fmt)+1, sizeof(char));
    if(fmt == NULL) {
        res = c_logger_OutOfMemory;
        goto QuitFunction;
    }
    strcpy(fmt, _fmt);

    // Find the number of '\n' in format, we will make them become '\n\t'
    for(i=0; i<strlen(fmt); i++) {
        if(fmt[i]=='\n') {
            nReturns++;
        }
    }
    sprintf(elapsedTime, "[%.3f]: ", (float)clock()/(float)CLOCKS_PER_SEC);
    size = (uint32_t)strlen(elapsedTime) + (uint32_t)strlen(fmt) + nReturns + 5; // 5 = "\t\n\t\n\0"
    newFmt = (char *)malloc(size*sizeof(char));
    if(newFmt == NULL) {
        res = c_logger_OutOfMemory;
        goto QuitFunction;
    }
    strcpy(newFmt, elapsedTime);
    tmp = strtok(fmt, "\n");
    if(tmp != NULL) {
        strcat(newFmt, tmp);
    }
    else {
        strcat(newFmt, fmt);
    }
    strcat(newFmt, "\n\t");
    while(1) {
        tmp = strtok(NULL, "\n");
        if(tmp != NULL) {
            strcat(newFmt, tmp);
            strcat(newFmt, "\n\t");
        }
        else {
            break;
        }
    }
    //strcat(newFmt, "\n");

    // now we have all we need; we can print the message!
    // NOTE, internal function assumes the mutex is already locked!
    header=(char*)calloc(strlen(severityMap[s])+strlen(logger->moduleTree)+strlen(headerFmt), sizeof(char));
    if(header == NULL) {
        res = c_logger_OutOfMemory;
        goto QuitFunction;
    }
    sprintf(header, headerFmt, severityMap[s], logger->moduleTree);
    if(strcmp(header, logger->logger->lastHeader)) {
        // the headers are different, print the new one and update the last
        fprintf(logger->logger->f, "%s", header);
        if(strlen(logger->logger->lastHeader)<strlen(header)) {
            free(logger->logger->lastHeader);
            logger->logger->lastHeader = (char *)calloc(strlen(header)+1, sizeof(char));
        }
        strcpy(logger->logger->lastHeader, header);
    }
    size = vfprintf(logger->logger->f, newFmt, args);
    fflush(logger->logger->f); // HACK!

QuitFunction:
    if(fmt != NULL) {
        free(fmt);
    }
    if(newFmt != NULL) {
        free(newFmt);
    }
    if(header != NULL) {
        free(header);
    }
    return res;
}

int32_t _allocateCLogger(c_logger_t **l) {
    int32_t res = c_logger_Success;
    void *oldPtr=NULL;

    if(!_isLGlobalConfigured()) {
        res = c_logger_LoggerNotConfigured;
        goto QuitFunction;
    }

    // NOTE, nInst is the number of previously instantiated loggers
    oldPtr=lGlobal->allocatedLoggersAddr;
    // allocatedLoggersAddr is a list of addresses of pointers to c_logger_t structure
    lGlobal->allocatedLoggersAddr = calloc( lGlobal->nInst+1, sizeof(c_logger_t **) );
    if(lGlobal->allocatedLoggersAddr == NULL) {
        res = c_logger_OutOfMemory;
        goto QuitFunction;
    }
    // copy old memory to the new location (if the old memory was not NULL)
    if(oldPtr != NULL) {
        memcpy(lGlobal->allocatedLoggersAddr, oldPtr, (lGlobal->nInst)*sizeof(c_logger_t **));
    }

    // add the new logger address in the last position
    ((c_logger_t ***)lGlobal->allocatedLoggersAddr)[lGlobal->nInst] = l;

QuitFunction:
    if(res) {
        // The function got a problem, roolback
        lGlobal->allocatedLoggersAddr = oldPtr;
    }
    else {
        // The function was ok, we can free the old memory
        free(oldPtr);
        lGlobal->nInst += 1;
    }
    return res;
}

int32_t _deallocateCLogger(c_logger_t **l) {
    int32_t res = c_logger_Success;
    uint32_t i;
    int32_t loggerIndex=-1;
    c_logger_t ***ptr=NULL;
    
    if(!_isLGlobalConfigured()) {
        res = c_logger_LoggerNotConfigured;
        goto QuitFunction;
    }
    
    ptr = (c_logger_t ***)lGlobal->allocatedLoggersAddr;

    if(ptr == NULL) {
        res = 1;
        goto QuitFunction;
    }

    for(i=0; i<lGlobal->nInst; i++) {
        if(ptr[i] == l) {
            break;
        }
    }
    loggerIndex = (int32_t)i;
    if(loggerIndex == (int32_t)lGlobal->nInst) {
        res = 2; // Logger not found in the list of allocated ones!
        goto QuitFunction;
    }

    if(lGlobal->nInst==1) {
        // That was the last allocated logger
        goto QuitFunction;
    }

    // Realloc the addresses list
    lGlobal->allocatedLoggersAddr = calloc( lGlobal->nInst-1, sizeof(c_logger_t **) );
    if(lGlobal->allocatedLoggersAddr == NULL) {
        res = c_logger_OutOfMemory;
        goto QuitFunction;
    }

    // Now fill in the new allocated list
    for(i=0; i<lGlobal->nInst; i++) {
        if(i<(uint32_t)loggerIndex) {
            ((c_logger_t ***)lGlobal->allocatedLoggersAddr)[i] = ptr[i];
        }
        else {
            if(i!=(uint32_t)loggerIndex) {
                ((c_logger_t ***)lGlobal->allocatedLoggersAddr)[i-1] = ptr[i];
            }
        }
    }

    // Free the old pointer
    free(ptr);

QuitFunction:
    if(res) {
        // We got a problem, rollback
        lGlobal->allocatedLoggersAddr = ptr;
    }
    else {
        if(lGlobal->nInst==1) {
            // That was the last allocated logger, we can free allocatedLoggersAddr and set it to NULL
            free(lGlobal->allocatedLoggersAddr);
            lGlobal->allocatedLoggersAddr = NULL;
        }
        lGlobal->nInst -= 1;
    }
    return res;
}

int32_t c_lcfg(char *filename, c_logger_OpenMode_t om) {
    int32_t res = c_logger_Success;
    int32_t opening = 0;
    char *expandedFileName=NULL;

    if(lGlobal==NULL) {
        char openFlags[4];
        opening=1;
        lGlobal = (_c_intlogger_t*)calloc(1, sizeof(_c_intlogger_t));
        if(lGlobal == NULL) {
            res = c_logger_OutOfMemory;
            goto QuitFunction;
        }

        // set the filename
        lGlobal->filename = NULL;
        // expand env variables in the string filename
        expandedFileName = getExpandedString(filename);
        if(expandedFileName == NULL) {
            res = c_logger_InvalidParameter;
            goto QuitFunction;
        }
        lGlobal->filename = (char*)malloc((strlen(expandedFileName)+1)*sizeof(char));
        if(lGlobal->filename == NULL) {
            res = c_logger_OutOfMemory;
            goto QuitFunction;
        }
        strcpy(lGlobal->filename, expandedFileName);

        // initialize the last header string
        lGlobal->lastHeader = NULL;
        lGlobal->lastHeader = (char*)calloc(2, sizeof(char));
        if(lGlobal->lastHeader == NULL) {
            res = c_logger_OutOfMemory;
            goto QuitFunction;
        }

        // open the file
        lGlobal->f = NULL;
        switch(om) {
        case c_logger_OpenMode_W:  strcpy(openFlags, "w");  break;
        case c_logger_OpenMode_A:  strcpy(openFlags, "a");  break;
        case c_logger_OpenMode_RW: strcpy(openFlags, "r+"); break;
        case c_logger_OpenMode_RA: strcpy(openFlags, "a+"); break;
        default: res = c_logger_WrongOpenMode; goto QuitFunction;
        }
        lGlobal->f = fopen(lGlobal->filename, openFlags);
        if(lGlobal->f == NULL) {
            res = c_logger_FileOpenError;
            goto QuitFunction;
        }
        lGlobal->openMode = om;

        // init the mutex
        initmutex(&(lGlobal->mutex));

        // set configured flag to true
        lGlobal->status |= 0x1<<LGR_STATUSBIT_CONFIGURED;

        // set the number of instances to 0 (it is incremented in function openLogger)
        lGlobal->nInst = 0;

        // init allocatedLoggersAddr to 0
        lGlobal->allocatedLoggersAddr = NULL;
    }

    else {
        // logger already configured
        res = c_logger_LoggerAlreadyConfigured;
        goto QuitFunction;
    }

QuitFunction:
    if(res) {
        if(opening == 1) {
            // ERROR opening the logger, we need to clean....
            // free lglobal if not null and set = null
            if(lGlobal != NULL) {
                // close the file if opened
                if(lGlobal->f != NULL) {
                    fclose(lGlobal->f);
                    lGlobal->f = NULL;
                }
                // free filename if not null and set = null
                if(lGlobal->filename != NULL) {
                    free(lGlobal->filename);
                    lGlobal->filename = NULL;
                }
                // free lastheader if not null and set = null
                if(lGlobal->lastHeader != NULL) {
                    free(lGlobal->lastHeader);
                    lGlobal->lastHeader = NULL;
                }
                free(lGlobal);
                lGlobal = NULL;
            }
        }
    }
    if(expandedFileName != NULL) free(expandedFileName);
    return res;
}

int32_t c_lopen(c_logger_t **logger, char *mt) {
    int32_t res = c_logger_Success;
    c_logger_t *result = NULL;

    // First of all check if the global internal logger is configured
    if(!_isLGlobalConfigured()) {
        res = c_logger_LoggerNotConfigured;
        goto QuitFunction;
    }

    result = (c_logger_t*)calloc(1, sizeof(c_logger_t));
    if(result == NULL) {
        // can't open the logger
        res = c_logger_OutOfMemory;
        goto QuitFunction;
    }
    result->opened = 0;

    result->moduleTree = NULL;
    result->moduleTree = (char *)malloc( (strlen(mt)+1)*sizeof(char));
    if(result->moduleTree == NULL) {
        // can't allocate the moduletree
        res = c_logger_OutOfMemory;
        goto QuitFunction;
    }
    strcpy(result->moduleTree, mt);

    result->logger = lGlobal;
    lock(&(result->logger->mutex));
    // add this logger to the list of allocated loggers
    // realloc the list
    res = _allocateCLogger( logger );
    if(res) {
        res = c_logger_GenericError;
        unlock(&(result->logger->mutex));
        goto QuitFunction;
    }
    unlock(&(result->logger->mutex));
    initmutex(&(result->mutex)); // HACK CONTROL ON FUNCTION SUCCESS??
    result->opened = 1;

QuitFunction:
    if(res) {
        if(result != NULL) {
            if(result->moduleTree != NULL) {
                free(result->moduleTree);
                result->moduleTree = NULL;
            }
            free(result);
            result = NULL;
        }
    }
    *logger = result;
    return res;
}

int32_t c_lclose(c_logger_t **logger) {
    int32_t res = c_logger_Success;
    c_logger_t *lg = *logger;

    if(lg != NULL) {
        if(lg->opened == 1) {
            // close the logger
            lock(&(lg->mutex));
            lg->opened = 0;
            lock(&(lg->logger->mutex));
            res = _deallocateCLogger(logger);
            if(res) {
                res = c_logger_GenericError;
                unlock(&(lg->logger->mutex));
                goto QuitFunction;
            }
            unlock(&(lg->logger->mutex));
            if(lg->moduleTree != NULL) {
                free(lg->moduleTree);
                lg->moduleTree = NULL;
            }
            unlock(&(lg->mutex));
            free(lg);
            lg = NULL;
        }
        else {
            res = c_logger_InvalidParameter;
            goto QuitFunction;
        }
    }
    else {
        res = c_logger_InvalidParameter;
        goto QuitFunction;
    }

QuitFunction:
    *logger = lg;
    return res;
}

int32_t c_lclear() {
    int32_t res = c_logger_Success;

    if(!_isLGlobalConfigured()) {
        goto QuitFunction;
    }

    lock(&(lGlobal->mutex));

    if(lGlobal->nInst > 0) {
        res = c_logger_LoggerStillInUse;
        goto ReleaseLogger;
    }

    fclose(lGlobal->f);
    free(lGlobal->filename);
    free(lGlobal->lastHeader);
    lGlobal->status = 0;

ReleaseLogger:
    unlock(&(lGlobal->mutex));
    if(res == c_logger_Success) {
        // deinit the mutex ??

        // free the logger
        free(lGlobal);
        lGlobal = NULL;
    }

QuitFunction:
    return res;
}

int32_t c_lvmsgf(c_logger_t *logger, c_logger_Severity s, char *fmt, va_list args) {
    int32_t res = c_logger_Success;

    lock(&(logger->logger->mutex));
    res = _lvmsgf(logger, s, fmt, args);
    unlock(&(logger->logger->mutex));

//QuitFunction:
    return res;
}

int32_t c_lmsgf(c_logger_t *logger, c_logger_Severity s, char *fmt, ...) {
    int32_t res = c_logger_Success;
    va_list args;

    lock(&(logger->logger->mutex));
    va_start(args, fmt);
    res = _lvmsgf(logger, s, fmt, args);
	va_end(args);
    unlock(&(logger->logger->mutex));

//QuitFunction:
    return res;
}

int32_t c_lopened(c_logger_t *logger, uint32_t *opened) {
    int32_t res = c_logger_Success;
    *opened = 0;

    if(logger != NULL) {
        lock(&(logger->logger->mutex));
        *opened = logger->opened;
        unlock(&(logger->logger->mutex));
    }
    else {
        goto QuitFunction;
    }

QuitFunction:
    return res;
}

int32_t c_lconfigured(uint32_t *configured) {
    int32_t res = c_logger_Success;
    *configured = 0;

    if(lGlobal != NULL) {
        lock(&(lGlobal->mutex));
        *configured = _isLGlobalConfigured();
        unlock(&(lGlobal->mutex));
    }
    else {
        goto QuitFunction;
    }

QuitFunction:
    return res;
}


/* Get FileName */
int32_t c_lgetfn(c_logger_t *logger, char *fn) {
    int32_t res = c_logger_Success;

    if(logger != NULL) {
        lock(&(logger->logger->mutex));
        if(fn == NULL) {
            fn = (char*)malloc( (strlen(logger->logger->filename)+1)*sizeof(char) );
            if(fn == NULL) {
                res = c_logger_OutOfMemory;
                goto ReleaseLogger;
            }
        }
        strcpy(fn, logger->logger->filename);
    }
    else {
        res = c_logger_InvalidParameter;
        goto QuitFunction;
    }

ReleaseLogger:
    unlock(&(logger->logger->mutex));

QuitFunction:
    return res;
}

/* Get Module Tree*/
int32_t c_lgetmt(c_logger_t *logger, char *mt) {
    int32_t res = c_logger_Success;

    if(logger != NULL) {
        lock(&(logger->logger->mutex));
        if(mt == NULL) {
            mt = (char*)malloc( (strlen(logger->logger->filename)+1)*sizeof(char) );
            if(mt == NULL) {
                res = c_logger_OutOfMemory;
                goto ReleaseLogger;
            }
        }
        strcpy(mt, logger->logger->filename);
    }
    else {
        res = c_logger_InvalidParameter;
        goto QuitFunction;
    }

ReleaseLogger:
    unlock(&(logger->logger->mutex));

QuitFunction:
    return res;
}

/* Get Open Mode*/
int32_t c_lgetom(c_logger_t *logger, c_logger_OpenMode_t *om) {
    int32_t res = c_logger_Success;

    if(logger != NULL) {
        lock(&(logger->logger->mutex));
        *om = logger->logger->openMode;
        unlock(&(logger->logger->mutex));
    }
    else {
        res = c_logger_InvalidParameter;
        goto QuitFunction;
    }

QuitFunction:
    return res;
}

int32_t DEINIT_C_LOGGER() {
    int32_t res = c_logger_Success;
    uint32_t i, nInstances=0;
    c_logger_t **l = NULL;
    
    // deinit all allocated c_logger_t*
    if(_isLGlobalConfigured()) nInstances = lGlobal->nInst;
    for(i=0; i<nInstances; i++) {
        l = ((c_logger_t ***)lGlobal->allocatedLoggersAddr)[nInstances-i-1];
        res |= c_lclose( l );
    }

    // deinit lGlobal
    res |= c_lclear();

    return res;
}
