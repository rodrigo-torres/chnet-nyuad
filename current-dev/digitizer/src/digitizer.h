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

#include <vector>
#include <deque>
#include <fstream>

#define MAXNB   1
#define MAXNBITS 14
#define MaxNChannels 2
#define NBins 16384

struct params_t
{
	params_t(double* ptr) :
		x_start(ptr[0]), x_end(ptr[1]), x_step(ptr[4]),
		y_start(ptr[2]), y_end(ptr[3]), y_step(ptr[5]), speed(ptr[6]),
		x_dim((x_end - x_start) / x_step), y_dim(1 + (y_end - y_start) / y_dim),
		pixels(x_dim * y_dim), dwell(x_step / speed) {}
	params_t(const params_t &par2) :
		x_start(par2.x_start), x_end(par2.x_end), x_step(par2.x_step),
		y_start(par2.y_start), y_end(par2.y_end), y_step(par2.y_step),
		x_dim(par2.x_dim), y_dim(par2.y_dim), speed(par2.speed), dwell(par2.dwell),
		pixels(par2.pixels) {}
	const uint x_dim;
	const uint y_dim;
	const uint pixels;
	const float x_end;
	const float y_end;
	const float x_start;
	const float y_start;
	const float x_step;
	const float y_step;
	const float speed;
	const float dwell;
};

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

	/* Buffer to write to file */
	std::vector<std::vector<uint>> buff;

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

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include <list>



class worker
{
public:
	worker(const params_t&);
	~worker();
	void set_buff(std::vector<std::vector<uint>>&&);
	void create_thread(uint);
private:
	void write_pixel(uint);

	params_t params;
	std::ofstream file;
	std::vector<std::string> line_buff;
	std::vector<std::thread> threads;
	std::vector<std::vector<uint>> buff;
};

#endif /* SRC_DIGITIZER_H_ */
