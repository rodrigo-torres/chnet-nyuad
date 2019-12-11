#ifndef __CAENLOGGER_H
#define __CAENLOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <CAENTypes.h>
#include <CAENMultiplatform.h>
#include <time.h>

#define LGR_STATUSBIT_CONFIGURED    0   // 0 -> Not conf; 1 -> conf;
#define LGR_STATUSBIT_OPENED        1   // 0 -> Closed; 1 -> Opened;

typedef enum {
    c_logger_OpenMode_W    = 0L,
    c_logger_OpenMode_A    = 1L,
    c_logger_OpenMode_RW   = 2L,
    c_logger_OpenMode_RA   = 3L,
} c_logger_OpenMode_t;

typedef enum {
    c_logger_Success                    =  0L,
    c_logger_GenericError               = -1L,
    c_logger_FilenameAlreadySet         = -2L,
    c_logger_OpenmodeAlreadySet         = -3L,
    c_logger_LoggerAlreadyConfigured    = -4L,
    c_logger_OutOfMemory                = -5L,
    c_logger_WrongOpenMode              = -6L,
    c_logger_FileOpenError              = -7L,
    c_logger_LoggerNotConfigured        = -8L,
    c_logger_InvalidParameter           = -9L,
    c_logger_LoggerAlreadyOpened        = -10L,
    c_logger_LoggerOpenError            = -11L,
    c_logger_LoggerStillInUse           = -12L,
} c_logger_ErrorCode_t;

// NOTE, if you change the following struct, check if you have the change the severityMap!!
typedef enum {
    c_logger_Severity_INFO      = 0L,
    c_logger_Severity_ERROR     = 1L,
    c_logger_Severity_WARNING   = 2L,
    c_logger_Severity_DEBUG     = 3L,
} c_logger_Severity;

typedef struct {
    FILE *f;
    int32_t status; /* | 31 ... 2 |    1   |      0     | 
                       | not used | opened | configured |*/
    char *filename;
    char *lastHeader;
    mutex_t mutex;
    uint32_t nInst;
    c_logger_OpenMode_t openMode;
    // This is the list of addresses of pointers to c_logger_t structure
    void *allocatedLoggersAddr;
} _c_intlogger_t;

typedef struct {
    _c_intlogger_t *logger;
    uint32_t opened;
    char *moduleTree;
    mutex_t mutex;
} c_logger_t;

#ifdef __cplusplus
extern "C" {
#endif

int32_t c_lopen(c_logger_t **logger, char *mt);

int32_t c_lclose(c_logger_t **logger);

int32_t c_lcfg(char *filename, c_logger_OpenMode_t om);

/* clear the configured internal global logger */
int32_t c_lclear();

int32_t c_lvmsgf(c_logger_t *logger, c_logger_Severity s, char *fmt, va_list args);

int32_t c_lmsgf(c_logger_t *logger, c_logger_Severity s, char *fmt, ...);

int32_t c_lopened(c_logger_t *logger, uint32_t *opened);

int32_t c_lconfigured(uint32_t *configured);

/* Get FileName
   NOTE, if 'fn' is NULL, it will be allocated
   to the right dimension. Then it must be free */
int32_t c_lgetfn(c_logger_t *logger, char *fn);

/* Get Module Tree
   NOTE, if 'mt' is NULL, it will be allocated
   to the right dimension. Then it must be free */
int32_t c_lgetmt(c_logger_t *logger, char *mt);

/* Get Open Mode */
int32_t c_lgetom(c_logger_t *logger, c_logger_OpenMode_t *om);

#define INIT_C_LOGGER(FNAME, MNAME)                                     \
    static c_logger_t *__logger;                                        \
    static char *__loggerFileName = FNAME;                              \
    static char *__loggerModuleTree = MNAME;                            \
    static c_logger_OpenMode_t __loggerOpenMode = c_logger_OpenMode_W;  \
    static int logEnabled = 1;                                          \
    static int logMsg(c_logger_Severity s, char *fmt, ...) {            \
        int32_t res = 0;                                                \
        if(logEnabled) {                                                \
            uint32_t lOpened = 0;                                       \
            uint32_t lConfigured = 0;                                   \
            va_list args;                                               \
            res = c_lconfigured(&lConfigured);                          \
            if( !lConfigured ) {                                        \
                res = c_lcfg(__loggerFileName, __loggerOpenMode);       \
                if(res) {                                               \
                    goto QuitFunction;                                  \
                }                                                       \
            }                                                           \
            res = c_lopened(__logger, &lOpened);                        \
            if( !lOpened ) {                                            \
                res = c_lopen(&__logger, __loggerModuleTree);           \
                if(res) {                                               \
                    goto QuitFunction;                                  \
                }                                                       \
            }                                                           \
            va_start(args, fmt);                                        \
            res = c_lvmsgf(__logger, s, fmt, args);                     \
	        va_end(args);                                               \
        }                                                               \
        QuitFunction:                                                   \
            return res;                                                 \
    }                                                                   \

int32_t DEINIT_C_LOGGER();

#ifdef __cplusplus
}
#endif

#endif