/******************************************************************************
* 
* CAEN SpA - Computing Division
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
*
* \file     keyb.h
* \brief    CAEN - Definition of Multiplatform keys management
* \author   Francesco Pepe (support.computing@caen.it)
*
******************************************************************************/

#ifndef __KEYB_H
#define __KEYB_H

#ifdef LINUX
#define CLEARSCR "clear"

/*****************************************************************************/
/*  SLEEP  */
/*****************************************************************************/
void Sleep(int t);

/*****************************************************************************/
/*  GETCH  */
/*****************************************************************************/
int _getch(void);

/*****************************************************************************/
/*  KBHIT  */
/*****************************************************************************/
int _kbhit();


#else  // Windows

#include <Windows.h>
#include <conio.h> 
#define CLEARSCR "cls"

static HANDLE hStdout = NULL;
static COORD  hCursorPosition;
static int    saved = 0;

#endif

void saveCursorPosition();

void restoreCursorPosition();

#endif