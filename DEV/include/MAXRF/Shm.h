#ifndef INC_SHM
#define INC_SHM

#define SHMSZ_CMD_STATUS 2048   // 1        page    interi ---> CMD and applications status
#define SHMSZ 409600            // 100      pages   interi --->
#define SHMSZDIGI 2048          // 1        page    interi --->
#define SHMSZRATE 128           // 1        page    interi ---> RateMeter


#include "sys/types.h"
#include "sys/shm.h"

#endif
