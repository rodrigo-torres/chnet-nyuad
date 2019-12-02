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

#ifndef __CAENSOCKET_H
#define __CAENSOCKET_H

#include <stdio.h>
//#include <CAENTypes.h>
#include <CAENMultiplatform.h>

#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/time.h>
	#include <unistd.h>
	#include <stdint.h>
#endif

#define MAX_BUFFER			16384
#define PARAM_HEADER_SIZE	6   // | PARAM_TYPE | PARAM_SIZE |
								// |     2      |     4      | param size is 4 byte to allow 16384 binned histos
#define INVALID_PARAMSIZE   0xFFFFFFFF

#define ERRORCODE_SIZE		2
#define SHORT_VALUE_SIZE	2
#define LONG_VALUE_SIZE		4
#define MAX_IN_BUFFER_SIZE	16
#define MAX_PARAM_SIZE      32  // 16384 ??
#define PACKET_HEADER_SIZE  16	// |CAEN | PACKETLEN | CMD | NPARAM |
								// |  4  |     4     |  4  |    4   | = 16
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t type;
    uint32_t size;
    void *value;
    uint32_t mustFreeValue;
} sockParam_t;

sockParam_t defineSockParam(uint32_t type, void *value);
int32_t freeSockParam(sockParam_t *param);
void setPacketLength(char *buffer, int32_t size);
//void assignParam(void *dest, sockParam_t param);

char *addheader(char *buffer, int32_t cmd, int32_t npar);
char *addParam(char *buffer, int32_t type, int32_t length, void *param);
int32_t sendPacket(SOCKET sckt, uint32_t cmd, uint32_t argsNum, ...);

int32_t getHeader(SOCKET sckt, uint32_t *length, int32_t *cmd, uint32_t *npar);
int32_t getParam(SOCKET sckt, sockParam_t *param);// int32_t getParam(SOCKET sckt, int32_t *type, uint32_t *length, void *param, int32_t *param_size);
int32_t getPacket(SOCKET sckt, int32_t *cmd, uint32_t *nPar, sockParam_t **params);

#ifdef __cplusplus
}
#endif

#endif