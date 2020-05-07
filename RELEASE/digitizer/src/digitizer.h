/*
 * daq_minimal_cpp.h
 *
 *  Created on: Oct 23, 2019
 *      Author: Rodrigo Torres
 */

#ifndef SRC_DIGITIZER_H_
#define SRC_DIGITIZER_H_

#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <csignal>
/* Random assignment protocol */
#include <time.h>
#include <math.h>
/* Synchronization MUTEX */
#include <fcntl.h>		//mode constants
#include <semaphore.h>
/* CAEN dependencies*/
#include <CAENDigitizer.h>
#include "Functions.h"

/* XRF DAQ software dependencies */
#include <sys/ipc.h>
#include <sys/shm.h>
#include "../../Shm.h"

#define MAXNB   1
#define MAXNBITS 14
#define MaxNChannels 2

class digitizer
{
public:
	digitizer();
	~digitizer();
	void open_digitizer();
	void start_daq();
	void handle_alarm();
	void handle_termination(int);

private:
	/* Private methods */
	template <typename T>
	T* assign_shm(key_t, size_t);
	void set_params();
	void start_mca();
	void clear_buffers();
	int program_digitizer(int, DigitizerParams_t, CAEN_DGTZ_DPP_PHA_Params_t);
	void daq_point_mode();
	void daq_scan_mode();
	void config_timer();
	void stop_timer();
	void stop_daq();
	int stop_mca(int);
	void scan_sync();
	void close_digitizer();

	/* Shared memory segments */
	sem_t* sem_reply;
	sem_t* sem_probe;
	int* shared_memory_cmd;
	int* shared_memory;
	int* shared_memory2;
	double* shared_memory5;
	/* Other variables */
	struct itimerval it_val;
	int mask;
	int ret;
	int datum_total;
	int flag_discard;
	bool flag_newpix;
	bool flag_sync;
	/* DPP Parameters */
	int handle[MAXNB];
	DigitizerParams_t Params[MAXNB];
	CAEN_DGTZ_DPP_PHA_Params_t DPPParams[MAXNB];
	/* DAQ variables */
	char* buffer;
	uint32_t BufferSize;
	uint32_t NumEvents[MaxNChannels];
	CAEN_DGTZ_DPP_PHA_Event_t *Events[MaxNChannels];
	CAEN_DGTZ_DPP_PHA_Waveforms_t *Waveform;

};

#endif /* SRC_DIGITIZER_H_ */
