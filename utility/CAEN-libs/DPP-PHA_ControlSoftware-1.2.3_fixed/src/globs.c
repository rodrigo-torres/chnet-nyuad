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

#ifdef WIN32

    #include <time.h>
    #include <sys/timeb.h>
    #include <conio.h>
    #include <process.h>
	#include <windows.h>

#else
    #include <unistd.h>
    #include <stdint.h>   /* C99 compliant compilers: uint64_t */
    #include <ctype.h>    /* toupper() */
    #include <sys/time.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "globs.h"
#include "Errors.h"
#include "keyb.h"


long getTime() {
    long time_ms;
#ifdef WIN32
    struct _timeb timebuffer;
    _ftime( &timebuffer );
    time_ms = (long)timebuffer.time * 1000 + (long)timebuffer.millitm;
#else
    struct timeval t1;
    struct timezone tz;
    gettimeofday(&t1, &tz);
    time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
#endif
    return time_ms;
}


void quitProgram(int ret) {
	if (ret>0 && ret<_ERR_COUNT_) printf("FATAL: %s (error %d)\n", ErrMsg[ret], ret);
	else if (ret == 0) printf("Exiting without errors\n");
	else printf("Unknown fatal error (error %d)\n", ret);
	if (ret && stdinterm()) getch_();
	exit(ret);
}

#ifdef WIN32

void sleep_(unsigned ms) {
	Sleep(ms);
}

#else 

void sleep_(unsigned ms) { /* can't sleep_ for longer than 999 ms, because of limitations in the usleep function */
	usleep(ms*1000);
}

#endif