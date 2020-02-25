/*
 * daq_minimal_cpp.cpp
 *
 *  Created on: Oct 23, 2019
 *      Author: Rodrigo Torres
 */
#include "src/namespaces.h"
#include "src/digitizer.h"

namespace conf
{
void set_params_channel1(CAEN_DGTZ_DPP_PHA_Params_t& DPPParams)
{
	DPPParams.thr[0]		= 880;	// Trigger Threshold
	DPPParams.k[0]			= 1100; // Trapezoid Rise Time (ns)
	DPPParams.m[0]			= 1000;	// Trapezoid Flat Top  (ns)
	DPPParams.M[0]			= 3100;	// Decay Time Constant (ns)
	DPPParams.ftd[0]		= 670;	// Peaking delay  (ns)
	DPPParams.a[0]			= 4; 	// Trigger Filter smoothing factor
	DPPParams.b[0]			= 200;	// Delay(b)
	DPPParams.trgho[0]		= 1300;	// Trigger Hold Off
	DPPParams.nsbl[0]		= 4;	// Baseline mean del trapezio (ordine di comparsa nel menu)
	DPPParams.nspk[0]		= 2;	// Peak mean (ordine di comparsa nel menu)
	DPPParams.pkho[0]		= 770;	// Peak holdoff
	DPPParams.blho[0]		= 100;	// Baseline holdoff del trapezio
	DPPParams.enf[0]		= 0.3;	// Energy Normalization Factor
	DPPParams.dgain[0]		= 0;	// Digital Gain (ordine di comparsa nel menu)
	DPPParams.decimation[0]	= 0;	// Decimation
}
void set_params_channel2(CAEN_DGTZ_DPP_PHA_Params_t& DPPParams)
{
	DPPParams.thr[1]		= 85;	// Trigger Threshold
	DPPParams.k[1]			= 810;// Trapezoid Rise Time (ns)
	DPPParams.m[1]			= 1000;	// Trapezoid Flat Top  (ns)
	DPPParams.M[1]			= 8000;	// Decay Time Constant (ns)
	DPPParams.ftd[1]		= 670;	// Peaking delay  (ns)
	DPPParams.a[1]			= 4; 	// Trigger Filter smoothing factor
	DPPParams.b[1]			= 100;	// Delay(b)
	DPPParams.trgho[1]		= 1300;	// Trigger Hold Off
	DPPParams.nsbl[1]		= 5;	// Baseline mean del trapezio (ordine di comparsa nel menu)
	DPPParams.nspk[1]		= 3;	// Peak mean (ordine di comparsa nel menu)
	DPPParams.pkho[1]		= 770;	// Peak holdoff
	DPPParams.blho[1]		= 100;	// Baseline holdoff del trapezio
	DPPParams.enf[1]		= 5;	// Energy Normalization Factor
	DPPParams.dgain[1]		= 0;	// Digital Gain (ordine di comparsa nel menu)
	DPPParams.decimation[1]	= 0;	// Decimation
}
} // namespace conf

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
// -------------------- CLASS METHODS: Digitizer ---------------------------- //
// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //

digitizer::digitizer()
{
	set_signals();

	common::SharedMemory<int> shared_memory_cmd{};
	if (shared_memory_cmd.open(6900, SHMSZ_CMD_STATUS))
	{
		daq_mode = shared_memory_cmd[300];
		//DEBUG
		daq_mode = 2;
		daq_mode == 0 ?
				daq_enable = false :
				daq_enable = true;
		// TODO Maybe assign this to a different variable
		timer_interval = shared_memory_cmd[301] * 1000;

		shared_memory_cmd[80] = getpid();
	}

	memset(&Params, 0, MAXNB * sizeof(DigitizerParams_t));
	memset(&DPPParams, 0, MAXNB * sizeof(CAEN_DGTZ_DPP_PHA_Params_t));
	printf("... DAQ started with process ID: %d\n", getpid());
}

digitizer::~digitizer() {
	cleanup();
}

void digitizer::open_digitizer()
{
	set_params();
	start_mca();
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
			exitWithCleanup();
		}

		ret = CAEN_DGTZ_GetInfo(handle[b], &BoardInfo);
		if (ret)
		{
			printf("[!] Can't read board info\n");
			exitWithCleanup();
		}
		printf("\nConnected to CAEN Digitizer Model %s, recognized as board %d\n", BoardInfo.ModelName, b);
		printf("ROC FPGA Release is %s\n", BoardInfo.ROC_FirmwareRel);
		printf("AMC FPGA Release is %s\n", BoardInfo.AMC_FirmwareRel);

		int MajorNumber;
		sscanf(BoardInfo.AMC_FirmwareRel, "%d", &MajorNumber);
		if (MajorNumber != 128)
		{
			printf("[!] This digitizer has not a DPP-PHA firmware\n");
			exitWithCleanup();
		}
	}

	for (int b = 0; b < MAXNB; b++)
	{
		ret = program_digitizer(handle[b], Params[b], DPPParams[b]);
		if (ret)
		{
			printf("[!] Failed to program the digitizer\n");
			exitWithCleanup();
		}
	}

	uint32_t AllocatedSize;
	ret = CAEN_DGTZ_MallocReadoutBuffer(handle[0], &buffer, &AllocatedSize);
	ret |= CAEN_DGTZ_MallocDPPEvents(handle[0], (void**)Events, &AllocatedSize);        /* Allocate memory for the events */
	ret |= CAEN_DGTZ_MallocDPPWaveforms(handle[0], (void**)&Waveform, &AllocatedSize);  /* Allocate memory for the waveforms */
	if (ret)
	{
		printf("[!] Can't allocate memory buffers\n");
		exitWithCleanup();
	}
}



void digitizer::readDataFromDGTZ()
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
		else {
			continue;
		}

		for (int ch = 0; ch < MaxNChannels; ch++)
		{
			for (int ev = 0; ev < NumEvents[ch]; ev++)
			{
				if (flag_newpix)
				{
					printf("[!] Event writing interrupted\n");
					break;
				}

				int flag = (bool)(data&(0xFFFFC000));
				data = Events[ch][ev].Energy;

				switch (data&(~mask))
				{
				case 0:
					data_packet->m_buffer.at(ch).at(data) += 1;
					eventi++;
					break;
				case 1:
					// TODO a method for reject spectra ??
					break;
				default:
					ev = NumEvents[ch];
					break;
				}
			}
		}
	}
}

void digitizer::start_daq()
{
	// DEBUG only
	daq_mode = 2;

	using daq_writer::DataPacket;
	using daq_writer::FileWriter;

	if (daq_mode == 2)
	{
		writer = std::make_unique<FileWriter>();
		// Check for valid status from FileWriter
//		if () {
//
//		}
		data_packet = writer->getEmptyBufferForPixel(current_pix);
		timer_interval = writer->params_.getDwellTime() * 1000;

		/* Initialize semaphore for process synchronization */
		sem_probe = sem_open("/daq_probe", 0);
		sem_reply = sem_open("/daq_reply", 0);
	}


	configTimer();
	ret = CAEN_DGTZ_SWStartAcquisition(handle[0]);
	if (ret || !daq_mode) {
		printf("[!] Problem starting acquisition, error code: %d\n", ret);
	}
	else {
		printf("[!] Acquisition started\n");
	}

	while (daq_enable)
	{
		if (flag_newpix)
		{
			// Cost of calling these two functions is ~ 10ms
			writer->pushToQueue(std::move(data_packet));
			data_packet = writer->getEmptyBufferForPixel(++current_pix);

			if (current_pix / writer->params_.getPixels()) {
				printf("[!] End of scan\n");
				sem_post(sem_reply);
				sem_close(sem_reply);
				sem_close(sem_probe);
				daq_enable = false;
				continue;
			}

			if ( !(current_pix % writer->params_.getXDim()) )
			{
				// Sync new line
				stopTimer();
				CAEN_DGTZ_SWStopAcquisition(handle[0]);
				configTimer();
				ret = CAEN_DGTZ_SWStartAcquisition(handle[0]);
				if (ret) {
					printf("[!] Can't start acquisition, error: %d\n", ret);
				}

			}
			flag_newpix = false;
		}
		readDataFromDGTZ();
	}

	daq_stopped = true;
	cleanup();
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

void digitizer::cleanup()
{
	// Thread-safe clean-up routine
	// TODO ignore timer?
	stopTimer();

	// Stop DAQ
	daq_enable = false;

	// Wait for DAQ to finish last reading
	int timeout {5};
	while (!daq_stopped && timeout) {
		// TODO sleep_for not thread-safe
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		--timeout;
	}

	if (handle[0])
	{
		// TODO this works for 1 board, it'll have to be revisited
		// when we add more boards
		for (int b = 0; b < MAXNB; b++)
		{
			printf("[!] Closing digitizer board %d\n", b);
			CAEN_DGTZ_SWStopAcquisition(handle[b]);
			CAEN_DGTZ_FreeReadoutBuffer(&buffer);
			CAEN_DGTZ_FreeDPPEvents(handle[b], (void**)Events);
			CAEN_DGTZ_FreeDPPWaveforms(handle[b], Waveform);
			CAEN_DGTZ_ClearData(handle[b]);
			CAEN_DGTZ_CloseDigitizer(handle[b]);
		}
	}

	if (writer == nullptr)
	{
		daq_writer::FileWriter* ptr = writer.release();
		delete ptr;
	}

}

void digitizer::exitWithCleanup()
{
	cleanup();
	exit(1);
}





void digitizer::configTimer()
{
	switch (daq_mode)
	{
	case 2:
		// Wait for the motors in position, ready for new line
		sem_post(sem_reply);
		sem_wait(sem_probe);
		break;
	default:
		printf("[!] Invalid DAQ mode\n");
		break;
	}

	it_val.it_value.tv_sec = timer_interval / 1000;
	it_val.it_value.tv_usec = (timer_interval * 1000) % 1000000;
	it_val.it_interval = it_val.it_value;
	if (setitimer(ITIMER_REAL, &it_val, NULL) == -1)
	{
		perror("[!] Error setting interval timer");
		exit(1);
	}
}

void digitizer::stopTimer()
{
	it_val.it_value.tv_sec = 0;
	it_val.it_value.tv_usec = 0;
	it_val.it_interval = it_val.it_value;
	if (setitimer(ITIMER_REAL, &it_val, NULL) == -1)
	{
		perror("[!] Error setting interval timer");
		exit(1);8
	}
}

void digitizer::handle_alarm()
{
	daq_mode = 2;
	switch (daq_mode)
	{
	case 0:
		printf("[!] DAQ is not active\n");
		break;
	case 1:
		stopTimer();
		daq_enable = false;
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


