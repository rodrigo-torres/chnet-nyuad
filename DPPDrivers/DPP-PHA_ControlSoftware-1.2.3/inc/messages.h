#ifndef MESSAGES_H
#define MESSAGES_H

typedef enum {
	MSG_RESTART,
	MSG_STOP_ACQ,
	MSG_STOP_CPLOT,
	MSG_ROUT_RATE,
	MSG_TRG_RATE,
	MSG_FIT_PARAMS,
	MSG_FIT_ERROR,
	MSG_ERROR,
	MSG_WARNING,
	MSG_NUM_CHANNELS,
	MSG_BOARD_INITED,
    MSG_DGTZ_FAMILY,
    MSG_DMP_OPEN,
	_MSG_COUNT_
} MessageCodes;

char MsgStrings[_MSG_COUNT_][100];

void guiMsg(int msgCode, ...); /* send a message to the GUI. msgCode specifies a string with printf-style tags that will be replaced with the arguments to this function */

void guiMsgFmt(const char* format, ...);

void commandAck();

void userMsg(const char* fmt, ...);

void libMsg(int code);

#endif