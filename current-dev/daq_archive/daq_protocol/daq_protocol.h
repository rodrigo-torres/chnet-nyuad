/*
 * daq_protocol.h
 *
 *  Created on: Oct 20, 2019
 *      Author: frao
 */

#ifndef DAQ_PROTOCOL_H_
#define DAQ_PROTOCOL_H_

#include <cstddef>
#include <sys/types.h> //types
#include <sys/ipc.h> //shm
#include <sys/shm.h> //shm
#include <sys/time.h> //timer
#include <errno.h>
#include <ctime> //time struct
#include <cmath>

#include <CAENDPPLib.h>
#include <CAENDPPLibTypes.h>
#include <CAENDigitizer.h>
#include "Functions.h"

#include "../Shm.h"

namespace shm
{
	struct {
		const key_t key1 = 7000;
		const key_t key2 = 7200;
		const key_t key4 = 7400;
		const key_t key5 = 8000;
		const key_t key_cmd = 6900;
		const key_t key_rate = 7500;
		const int TOTAL_NO = 6;
	} keys;

	struct {
		int* flags = nullptr;
		int* spectra = nullptr;
		int* map_data = nullptr;
		int* rate = nullptr;
		int* daq = nullptr;
		float* var = nullptr;
	} ptr;

	struct {
		const size_t flags_size = SHMSZ_CMD_STATUS;
		const size_t spectra_size = SHMSZ;
		const size_t map_size = SHMSZBIG;
		const size_t daq_size = SHMSZDIGI;
		const size_t rate_size = SHMSZRATE;
		const size_t var_size = 4096;
	} size;

	template <typename T>
	T* assign_shm(key_t key, size_t size)
	{
		int shm_id = shmget(key, size, IPC_CREAT | 0666);
		if (shm_id == -1)
		{
			printf("[!] Can't obtain an SHM segment ID for key\t%d\n", key);
			printf("%s\n", strerror(errno));
			return nullptr;
		}
		else
			return static_cast<T*>(shmat(shm_id, nullptr, 0));
	}
}

namespace ran
{
	// Macros for the random data distribution protocol
	#define ia 106
	#define ic 1283
	#define im 6075

	long *idum;
	long seed;

	double jlo, jhi;
	int callstorandom = 0;
	void setseed() {

		time_t timer;
		struct tm y2k = {0};
		//double seconds;

		y2k.tm_hour = 0;	y2k.tm_min = 0; y2k.tm_sec = 0;
		y2k.tm_year = 0;	y2k.tm_mon = 0; y2k.tm_mday = 1;

		time(&timer);
		seed = difftime(timer,mktime(&y2k));
		//seed = seconds;
		idum = &seed;
	}

	long ranqd1(long *idum, double jlo, double jhi) {
		if (callstorandom > 50000) {
			setseed();
			callstorandom=0;
		}

		if (jlo < 0) return (long) 0;

		*idum = (*idum * ia + ic) % im;
		double j = jlo + ((jhi - jlo + 1) * (double) *idum) / (double) im;
		callstorandom++;

		if ((j-floor(j)) >= 0.5) return (long) ceil(j);
		else return (long) floor(j);
	}
}


int ProgramDigitizer(int handle, DigitizerParams_t Params, CAEN_DGTZ_DPP_PHA_Params_t DPPParams);

#endif /* DAQ_PROTOCOL_H_ */
