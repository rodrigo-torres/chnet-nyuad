/*
 * daq_minimal_cpp.cpp
 *
 *  Created on: Oct 23, 2019
 *      Author: Rodrigo Torres
 */
#include <src/digitizer.h>
#include <src/namespaces.h>

digitizer::digitizer()
{
	shared_memory_cmd = assign_shm<int>(6900, SHMSZ_CMD_STATUS);
	shared_memory = assign_shm<int>(7000, SHMSZ);

	datum_total = 1;
	flag_newpix = false;
	flag_sync = true;

	mask = 0x3FFF;
	buffer = NULL;
	Waveform = NULL;

	memset(&Params, 0, MAXNB * sizeof(DigitizerParams_t));
	memset(&DPPParams, 0, MAXNB * sizeof(CAEN_DGTZ_DPP_PHA_Params_t));

	shared_memory_cmd[80] = getpid();
	printf("... DAQ started with process ID: %d\n", getpid());
}

digitizer::~digitizer()
{
	if (handle[0])
	{
		close_digitizer();
	}
}

void digitizer::open_digitizer()
{
	set_params();
	start_mca();
	clear_buffers();
}

void digitizer::close_digitizer()
{
	stop_daq();
	stop_mca(0);
}

void digitizer::start_daq()
{
	switch (shared_memory_cmd[300])
	{
	case 0:
		printf("[!] DAQ not yet started\n");
		close_digitizer();
		break;
	case 1:
		daq_point_mode();
		break;
	case 2:
		daq_scan_mode();
		break;
	default:
		printf("[!] Invalid DAQ mode\n");
		close_digitizer();
		break;
	}
}

void digitizer::set_params()
{
	for (int b = 0; b < MAXNB; b++)
	{
		/****************************\
		 * Communication Parameters   *
        \****************************/

		// Direct USB connection
		Params[b].LinkType = CAEN_DGTZ_USB;         // Link Type
		Params[b].VMEBaseAddress = 0;               // For direct USB connection, VMEBaseAddress must be 0
		Params[b].IOlev = CAEN_DGTZ_IOLevel_TTL;

		/****************************\
		 *  Acquisition parameters    *
        \****************************/
		Params[b].AcqMode = CAEN_DGTZ_DPP_ACQ_MODE_List;            // CAEN_DGTZ_DPP_ACQ_MODE_List or CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope
		Params[b].RecordLength = 2500;                              // Num of samples of the waveforms (only for Oscilloscope mode)
		Params[b].ChannelMask = 0x3;                                // Channel enable mask////era 0xF
		Params[b].EventAggr = 1;                                    // number of events in one aggregate (0=automatic)
		Params[b].PulsePolarity = CAEN_DGTZ_PulsePolarityPositive;

		/* Trigger and shaping parameters */
		conf::set_params_channel1(DPPParams[b]);
		conf::set_params_channel2(DPPParams[b]);
	}
}

void digitizer::start_mca()
{

	CAEN_DGTZ_BoardInfo_t BoardInfo;
	for (int b = 0; b < MAXNB; b++)
	{
		ret = CAEN_DGTZ_OpenDigitizer(Params[b].LinkType, b, 0, Params[b].VMEBaseAddress, &handle[b]);
		if (ret)
		{
			printf("[!] Can't open digitizer\n");
			stop_mca(1);
		}

		ret = CAEN_DGTZ_GetInfo(handle[b], &BoardInfo);
		if (ret)
		{
			printf("[!] Can't read board info\n");
			stop_mca(1);
		}
		printf("\nConnected to CAEN Digitizer Model %s, recognized as board %d\n", BoardInfo.ModelName, b);
		printf("ROC FPGA Release is %s\n", BoardInfo.ROC_FirmwareRel);
		printf("AMC FPGA Release is %s\n", BoardInfo.AMC_FirmwareRel);

		int MajorNumber;
		sscanf(BoardInfo.AMC_FirmwareRel, "%d", &MajorNumber);
		if (MajorNumber != 128)
		{
			printf("[!] This digitizer has not a DPP-PHA firmware\n");
			stop_mca(1);
		}
	}

	uint32_t AllocatedSize;
	for (int b = 0; b < MAXNB; b++)
	{
		ret = program_digitizer(handle[b], Params[b], DPPParams[b]);
		if (ret)
		{
			printf("[!] Failed to program the digitizer\n");
			stop_mca(1);
		}
	}

	ret = CAEN_DGTZ_MallocReadoutBuffer(handle[0], &buffer, &AllocatedSize);
	ret |= CAEN_DGTZ_MallocDPPEvents(handle[0], (void**)Events, &AllocatedSize);        /* Allocate memory for the events */
	ret |= CAEN_DGTZ_MallocDPPWaveforms(handle[0], (void**)&Waveform, &AllocatedSize);  /* Allocate memory for the waveforms */
	if (ret)
	{
		printf("[!] Can't allocate memory buffers\n");
		stop_mca(1);
	}
}

void digitizer::clear_buffers()
{
	/* Clean histogram buffers */
	int mask = 0x3FFF;
	for (int ch = 0; ch < mask; ch++)
	{
		shared_memory[100 + ch] = 0;
		shared_memory[20000 + ch] = 0;
		shared_memory[40000 + ch] = 0;
	}
}

void digitizer::daq_point_mode()
{
	int data = 0x3FFF, caso = 10, eventi = 0;
	double jlo = 0, jhi = 0;
	double calGrad = (double)shared_memory_cmd[101] / (double)shared_memory_cmd[103];
	double calOffs = (double)shared_memory_cmd[102] / (double)shared_memory_cmd[103];
	calGrad = 1.19815;
	calOffs = -17.2263;

	multidet multi(calGrad, calOffs);
	printf("Multidetector parameters: %5.3f %5.3f\n", calGrad, calOffs);
	//printf("Acquisition debug: %d\n", shared_memory_cmd[300]);

	config_timer();
	ret = CAEN_DGTZ_SWStartAcquisition(handle[0]);
	if (ret || !shared_memory_cmd[300])
		printf("[!] Problem starting acquisition, error code: %d\n", ret);
	else
		printf("[!] Acquisition started\n");

	//while (shared_memory_cmd[100]) { // for debug only
	while (shared_memory_cmd[300])
	{
		for (int b = 0; b < MAXNB; b++)
		{
			ret = CAEN_DGTZ_ReadData(handle[b], CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, buffer, &BufferSize);
			if (ret)
				printf("[!] Readout error, code: %d\n", ret);

			if (BufferSize != 0)
			{
				ret |= CAEN_DGTZ_GetDPPEvents(handle[b], buffer, BufferSize, (void**)Events, NumEvents);
				if (ret)
					printf("[!] Data error, code: %d\n", ret);
			}

			for (int ch = 0; ch < MaxNChannels; ch++)
			{
				for (int ev = 0; ev < NumEvents[ch]; ev++)
				{
					if (BufferSize != 0) data = Events[ch][ev].Energy;
					else continue;

					//if (!(data&(~mask))) caso = 4;

					switch (data&(~mask))
					{
					case 0:
						if (ch == 0)
						{
							shared_memory[100 + data] += 1;
							shared_memory[40000 + data] += 1;
						}
						if (ch == 1)
						{
							shared_memory[20000 + data] += 1;
							data = multi.distribute(data);
							shared_memory[40000 + data] += 1;
						}
						eventi++;
						caso = -1;
						break;
					default:
						ev = NumEvents[ch];
						break;
					}
				}
			}
		}
	}
	stop_mca(0);
}

void digitizer::daq_scan_mode()
{
	key_t key2 = 7200, key5 = 8000;
	shared_memory2 = assign_shm<int>(key2, SHMSZBIG);
	shared_memory5 = assign_shm<double>(key5, 4096);
	//memset(shared_memory2, 0, SHMSZBIG);

	/* Set the scan variables */

	float x_start = shared_memory5[0];
	float y_start = shared_memory5[2];
	const float x_step = shared_memory5[4];
	const float y_step = shared_memory5[5];
	const float speed = shared_memory5[6];
	const uint x_pixels = (shared_memory5[1] - x_start) / x_step;
	const uint y_pixels = 1 + (shared_memory5[3] - y_start) / y_step;
	uint current_x = 0;
	uint current_y = 0;

	shared_memory_cmd[309] = static_cast<float>(x_step / speed) * 1000;
	printf("... Setting %d X steps and %d Y steps and %ds interval\n", x_pixels, y_pixels, shared_memory_cmd[309]);

	/* Set other related variables */
	int data = 0x3FFF, eventi = 0;
	double jlo = 0, jhi = 0;
	double calGrad = (double)shared_memory_cmd[101] / (double)shared_memory_cmd[103];
	double calOffs = (double)shared_memory_cmd[102] / (double)shared_memory_cmd[103];
	calGrad = 1.19815;
	calOffs = -17.2263;

	multidet multi(calGrad, calOffs);
	printf("Multidetector parameters: %5.3f %5.3f\n", calGrad, calOffs);

	/* Initialize semaphore for process synchronization */
	sem_probe = sem_open("/daq_probe", 0);
	sem_reply = sem_open("/daq_reply", 0);
	if (false)
	{
		stop_mca(0);
		sem_post(sem_reply);
		sem_close(sem_reply);
		sem_close(sem_probe);
		return;
	}

	ret = CAEN_DGTZ_SWStartAcquisition(handle[0]);
	if (ret || !shared_memory_cmd[300])
		printf("[!] Problem starting acquisition, error code: %d\n", ret);
	else
		printf("[!] Acquisition started\n");


	int x_mask = 0x2FAF080, y_mask = 0x3938700;
	int deta_mask = 0x1312D00, detb_mask = 0x1C9C380;
	while (shared_memory_cmd[300])
	{
		if (flag_newpix)
		{
			uint opt = (current_x / (x_pixels - 1)) << 1 | (current_y % 2);
			switch (opt)
			{
			case 0:	// Writes positions left to right
				x_start += x_step;
				current_x++;
				shared_memory2[10 + (datum_total++)] = x_start * 1000 + x_mask;
				shared_memory2[10 + (datum_total++)] = y_start * 1000 + y_mask;
				shared_memory2[4] = datum_total;
				break;
			case 1:	// Writes positions right to left
				x_start -= x_step;
				current_x++;
				shared_memory2[10 + (datum_total++)] = x_start * 1000 + x_mask;
				shared_memory2[10 + (datum_total++)] = y_start * 1000 + y_mask;
				shared_memory2[4] = datum_total;
				break;
			case 2:
			case 3:	// Handles change of line
				stop_timer();
				flag_sync = true;
				if (++current_y == y_pixels)
				{
					close_digitizer();
					sem_post(sem_reply);
					sem_close(sem_reply);
					sem_close(sem_probe);
					printf("[!] End of scan\n");
					return;
				}
				y_start += y_step;
				current_x = 0;
				break;
			default:
				break;
			}
//			shared_memory[42] = 1; // Informs the Online Map of a new pixel available
			flag_newpix = false;
		}


 		if (flag_sync)	// New scan line
		{
 			//printf("Current x %d and y %d\n", current_x, current_y);
 			CAEN_DGTZ_SWStopAcquisition(handle[0]);
 			shared_memory2[10 + (datum_total++)] = x_start * 1000 + x_mask;
 			shared_memory2[10 + (datum_total++)] = y_start * 1000 + y_mask;
			scan_sync();

			ret = CAEN_DGTZ_SWStartAcquisition(handle[0]);
			if (ret)
				printf("[!] Problem starting acquisition, error code: %d\n", ret);
		}

		for (int b = 0; b < MAXNB; b++)
		{
			ret = CAEN_DGTZ_ReadData(handle[b], CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, buffer, &BufferSize);
			if (ret)
				printf("[!] Readout error, code: %d\n", ret);

			if (BufferSize != 0)
			{
				ret |= CAEN_DGTZ_GetDPPEvents(handle[b], buffer, BufferSize, (void**)Events, NumEvents);
				if (ret)
					printf("[!] Data error, code: %d\n", ret);
			}
			else
				continue;

			for (int ch = 0; ch < MaxNChannels; ch++)
			{
				for (int ev = 0; ev < NumEvents[ch]; ev++)
				{
					if (flag_newpix)
					{
						printf("[!] Event writing interrupted\n");
						break;
					}

					data = Events[ch][ev].Energy;
					int flag = (bool)(data&(0xC000));

					// Discard events when:
					// flag_sync evaluates to true, i.e.:
					// - a new line just started
					// - a new scan just started
					// data&(~mask) evaluates to true, i.e.:
					// - data is larger than 2^14 bits (data&(~mask))
					switch (flag)
					{
					case 0:
						if (ch == 0)
						{
							shared_memory[100 + data] += 1;
							shared_memory[40000 + data] += 1;
							shared_memory2[10 + (datum_total++)] = data + deta_mask;
							shared_memory2[5] = ++eventi;
						}
						if (ch == 1)
						{
							shared_memory[20000 + data] += 1;
							shared_memory2[10 + (datum_total++)] = data + detb_mask;
							shared_memory2[5] = ++eventi;
							data = multi.distribute(data);
							shared_memory[40000 + data] += 1;
						}
						//eventi++;
						break;
					case 1:
						// A method for reject spectra ??
						break;
					default:
						break;
					}
				}
			}
		}
	}
}


int digitizer::program_digitizer(int handle, DigitizerParams_t Params, CAEN_DGTZ_DPP_PHA_Params_t DPPParams)
{
	/* This function uses the CAENDigitizer API functions to perform the digitizer's initial configuration */

	int ret = 0;
	ret |= CAEN_DGTZ_Reset(handle);    /* Reset the digitizer */
	if (ret)
	{
		printf("[!] Can't reset the digitizer\n");
		return -1;
	}

	ret |= CAEN_DGTZ_WriteRegister(handle, 0x8000, 0x010E0114);  // Channel Control Reg (indiv trg, seq readout) ??
	ret |= CAEN_DGTZ_SetDPPAcquisitionMode(handle, Params.AcqMode, CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime);
	ret |= CAEN_DGTZ_SetAcquisitionMode(handle, CAEN_DGTZ_SW_CONTROLLED);
	ret |= CAEN_DGTZ_SetRecordLength(handle, Params.RecordLength);
	ret |= CAEN_DGTZ_SetIOLevel(handle, Params.IOlev);
	ret |= CAEN_DGTZ_SetExtTriggerInputMode(handle, CAEN_DGTZ_TRGMODE_ACQ_ONLY);
	ret |= CAEN_DGTZ_SetChannelEnableMask(handle, Params.ChannelMask);
	ret |= CAEN_DGTZ_SetDPPEventAggregation(handle, Params.EventAggr, 0);

	/* Set the mode used to synchronize the acquisition between different boards.
    In this example the sync is disabled */
	ret |= CAEN_DGTZ_SetRunSynchronizationMode(handle, CAEN_DGTZ_RUN_SYNC_Disabled);

	// Set the DPP specific parameters for the channels in the given channelMask
	ret |= CAEN_DGTZ_SetDPPParameters(handle, Params.ChannelMask, &DPPParams);

	uint32_t off[2];
	int32_t p_off[2] = { 44 , 44 };
	if ( abs(p_off[0]) > 50 || abs(p_off[1]) > 50)
	{
		printf("[!] Invalid DC offset value\n");
		off[1] = off[0] = 0x8000;
	}
	else
	{
		off[0] = 32767 * (1 + (double)p_off[0] / 50);
		off[1] = 32767 * (1 + (double)p_off[1] / 50);
		printf("Offsets set at %d ad %d\n", off[0], off[1]);
	}


	for(int i = 0; i < MaxNChannels; i++)
	{
		if (Params.ChannelMask & (1<<i))
		{
			ret |= CAEN_DGTZ_SetChannelDCOffset(handle, i, off[i]);
			ret |= CAEN_DGTZ_SetDPPPreTriggerSize(handle, i, 500);
			ret |= CAEN_DGTZ_SetChannelPulsePolarity(handle, i, Params.PulsePolarity);
		}
	}


	ret |= CAEN_DGTZ_SetDPP_PHA_VirtualProbe(handle, CAEN_DGTZ_DPP_VIRTUALPROBE_DUAL, CAEN_DGTZ_DPP_PHA_VIRTUALPROBE1_trapezoid, CAEN_DGTZ_DPP_PHA_VIRTUALPROBE2_Input, CAEN_DGTZ_DPP_PHA_DIGITAL_PROBE_Peaking);
	if (ret)
	{
		printf("Warning: errors found during the programming of the digitizer.\nSome settings may not be executed\n");
		return ret;
	}
	return ret;

	/* CAEN_DGTZ_WriteRegister(int handle, unsigned int address, unsigned int value);
	 * Sets the input range with the CAENDigitizer library
	 * IMPORTANT: the values of address and value must be EXACTLY the ones here below
	 * ADDRESS:
	 * 0x10B4 -> for input range channel 0
	 * 0x11B4 -> for input range channel 1
	 * VALUE:
	 * 0xA -> 9.5 Vpp
	 * 0x9 -> 3.7 Vpp
	 * 0x6 -> 1.4 Vpp
	 * 0x5 -> 0.6 Vpp
	 *
	 */
	CAEN_DGTZ_WriteRegister(handle, 0x10B4, 0xA);
	CAEN_DGTZ_WriteRegister(handle, 0x11B4, 0xA);
}

int digitizer::stop_mca(int raise_flag = 0)
{
	stop_timer();
	for (int b = 0; b < MAXNB; b++)
	{
		CAEN_DGTZ_SWStopAcquisition(handle[b]);
	}
	CAEN_DGTZ_FreeReadoutBuffer(&buffer);
	CAEN_DGTZ_FreeDPPEvents(handle[0], (void**)Events);
	CAEN_DGTZ_ClearData(handle[0]);
	CAEN_DGTZ_FreeDPPWaveforms(handle[0], Waveform);
	int b = 0;
	printf("[!] Closing digitizer\n");
	CAEN_DGTZ_CloseDigitizer(handle[b]);
	shared_memory2[4] = datum_total;
	printf("[!] %d records in map memory.\n", shared_memory2[4]);
	if (raise_flag)
	{
		exit(1);
	}

	return ret;
}


inline void digitizer::scan_sync()
{
	/* Synchronization block */
	// Wait for the motors in position, ready for new line
	flag_sync = 0;
	sem_post(sem_reply);
	sem_wait(sem_probe);
	config_timer();
}

void digitizer::stop_daq()
{	/* Signal DAQ end*/
	stop_timer();
	shared_memory_cmd[300] = 0;
}

inline void digitizer::config_timer()
{
	int seconds = 0;
	switch (shared_memory_cmd[300])
	{
	case 0:
		printf("[!] DAQ is not active\n");
		break;
	case 1:
		// Sets interval to the DAQ duration
		seconds = shared_memory_cmd[301] * 1000;
		break;
	case 2:
		// Sets the interval to the scan dwell time
		seconds = shared_memory_cmd[309];
		break;
	default:
		printf("[!] Unknown DAQ mode in timer");
		break;
	}

	it_val.it_value.tv_sec = seconds / 1000;
	it_val.it_value.tv_usec = (seconds * 1000) % 1000000;
	it_val.it_interval = it_val.it_value;
	if (setitimer(ITIMER_REAL, &it_val, NULL) == -1)
	{
		perror("[!] Error setting interval timer");
		exit(1);
	}
}

void digitizer::stop_timer()
{
	it_val.it_value.tv_sec = 0;
	it_val.it_value.tv_usec = 0;
	it_val.it_interval = it_val.it_value;
	if (setitimer(ITIMER_REAL, &it_val, NULL) == -1)
	{
		perror("[!] Error setting interval timer");
		exit(1);
	}
}

void digitizer::handle_alarm()
{
	switch (shared_memory_cmd[300])
	{
	case 0:
		printf("[!] DAQ is not active\n");
		break;
	case 1:
		stop_daq();
		break;
	case 2:
		if (flag_newpix)
			printf("[!] New pixel flag already set to true\n");
		flag_newpix = true;
		break;
	default:
		break;
	}
}

void digitizer::handle_termination(int signal)
{
	switch (signal)
	{
	case SIGINT:
	case SIGTERM:
	case SIGQUIT:
		printf("[!] Exiting on termination\n");
		close_digitizer();
		exit(2);
		break;
	default:
		printf("[!] Unknown signal");
		break;
	}
}


template <typename T>
T* digitizer::assign_shm(key_t key, size_t size)
{
	int shm_id = shmget(key, size, IPC_CREAT | 0666);
	if (shm_id == -1)
	{
		printf("[!] Can't obtain an SHM segment ID for key\t%d\n", key);
		printf("%s\n", strerror(errno));
		return nullptr;
	}
	else
		return (T*)shmat(shm_id, nullptr, 0);
}

