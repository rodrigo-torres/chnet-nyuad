/*
 * daq_minimal_cpp.h
 *
 *  Created on: Oct 23, 2019
 *      Author: Rodrigo Torres
 */
#ifndef SRC_DIGITIZER_H_
#define SRC_DIGITIZER_H_

#include <atomic>
#include <ctime>		// clock() for benchmark analysis
#include <csignal>		// Synchronization and abort signals
#include <memory>
#include <vector>

// POSIX libraries
#include <semaphore.h>	// Semaphore with main program
// CAEN libraries
#include <CAENDigitizer.h>
#include "Functions.h"	// CAEN types definitions and utility methods
// XRF-OA libraries
#include "../../Shm.h"	// Common shared memory variables
#include "src/buffer.h"

#define MAXNB   1
#define MAXNBITS 14
#define MaxNChannels 2
#define NBins 16384


class signals
{
	//constructor
	//destructor
	//thread
	//single shot
	//recurrent shot
		// for the recurrent i could either ignore or ask the timer to send a
		// signal an x quantity of times

};


class digitizer
{
public:
	// TODO Only interface functions here should be start and stop DAQ
	digitizer();
	~digitizer();

	void cleanup();
	void exitWithCleanup();

	void open_digitizer();
	void start_daq();
	void handle_alarm();

	void set_signals();
private:
	int daq_mode {0};
	std::atomic_bool daq_enable {false};
	std::atomic_bool daq_stopped {false};
	std::atomic_bool ignore_timer {false};


	void set_params();
	void start_mca();
	int program_digitizer(int, DigitizerParams_t, CAEN_DGTZ_DPP_PHA_Params_t);

	void readDataFromDGTZ();


	void configTimer();
	void stopTimer();
	/* Shared memory segments */
	sem_t* sem_reply {nullptr};
	sem_t* sem_probe {nullptr};

	/* Other variables */
	struct itimerval it_val;
	int mask {0x3FFF};
	int ret {0};
	bool flag_newpix {false};
	/* DPP Parameters */
	int handle[MAXNB];
	DigitizerParams_t Params[MAXNB];
	CAEN_DGTZ_DPP_PHA_Params_t DPPParams[MAXNB];
	/* DAQ variables */
	char* buffer {nullptr};
	uint32_t BufferSize {0};
	uint32_t NumEvents[MaxNChannels];
	CAEN_DGTZ_DPP_PHA_Event_t *Events[MaxNChannels];
	CAEN_DGTZ_DPP_PHA_Waveforms_t *Waveform {nullptr};


	// TODO check the variables below are well implemented
	int eventi {1};
	unsigned short int data {0};
	int current_pix {0};
	int timer_interval {0};
	std::unique_ptr<daq_writer::FileWriter> writer {nullptr};
	std::unique_ptr<daq_writer::DataPacket> data_packet {nullptr};
};

#endif /* SRC_DIGITIZER_H_ */
