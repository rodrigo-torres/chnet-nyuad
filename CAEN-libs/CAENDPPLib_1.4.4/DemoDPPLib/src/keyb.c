/******************************************************************************
* 
* CAEN SpA - Computing Division
* Via Vetraia, 11 - 55049 - Viareggio ITALY
* + 390594388398 - www.caen.it
*
***************************************************************************//**
* \note TERMS OF USE:
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software
* Foundation. This program is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. The user relies on the 
* software, documentation and results solely at his own risk.
*
* \file     CAENDigitizerType.h
* \brief    CAEN - Digitizer Library types definition
* \author   Francesco Pepe (support.computing@caen.it)
*
******************************************************************************/

#include <keyb.h> 

#ifdef LINUX

#include <sys/time.h>   // struct timeval, select()
#include <termios.h>    // tcgetattr(), tcsetattr()
#include <stdlib.h>     // atexit(), exit()
#include <unistd.h>     // read()
#include <stdio.h>      // printf()
#include <string.h>     // memcpy()
#include <fcntl.h>      // fcntl()

/*****************************************************************************/
/*  SLEEP  */
/*****************************************************************************/
void Sleep(int t) {
    usleep( t*1000 );
}

/*****************************************************************************/
/*  KBHIT  */
/*****************************************************************************/
int _kbhit()
{
    struct termios oldt, newt;
    int ch;
    int oldf;
 
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
    ch = getchar();
 
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
 
    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }
 
    return 0;
}

/* Read 1 character - echo defines echo mode */
static int getch_(int echo) 
{
  struct termios old, new;
  char ch;

  /* Initialize new terminal i/o settings */
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new = old; /* make new settings same as old settings */
  new.c_lflag &= ~ICANON; /* disable buffered i/o */
  new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */

  ch = getchar();

  /* Restore old terminal i/o settings */
  tcsetattr(0, TCSANOW, &old);

  return ch;
}

/*****************************************************************************/
/*  GETCH  */
/*****************************************************************************/
int _getch(void) 
{
  return getch_(0);
}

/*****************************************************************************/
/*  GETCHE  */
/*****************************************************************************/
int _getche(void) 
{
  return getch_(1);
}

void saveCursorPosition() {
    printf("\033[s");
}

void restoreCursorPosition() {
    printf("\033[u");
}

#else

void saveCursorPosition() {
    CONSOLE_SCREEN_BUFFER_INFO BuffInfo;

    if (hStdout == NULL) {
        hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hStdout == NULL)
            return; // ERROR
    }
    
    if (GetConsoleScreenBufferInfo(hStdout, &BuffInfo) == 0)
        return;

    hCursorPosition = BuffInfo.dwCursorPosition;
    saved = 1;
}

void restoreCursorPosition() {
    if (saved == 1)
        SetConsoleCursorPosition(hStdout, hCursorPosition);
}

#endif
