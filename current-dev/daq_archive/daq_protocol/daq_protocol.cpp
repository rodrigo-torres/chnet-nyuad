/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include "daq_protocol.h"
#include <array>
#include <numeric>
#include <stdio.h>
#include <csignal> //signals
#include <iostream> //cout

#define MAXNB   1
#define MAXNBITS 14
#define MaxNChannels 2

class digitizer
{
public:
	digitizer();
	~digitizer();
	void raise_flag();

	void set_timer(int);
	const int get_total();
	const int get_events();

	void daq_point_mode();
	void daq_scan_mode();
	void stop_daq();
	void stop_mca(bool);


private:
	void import_dpp_params();
	void set_dpp_params();
	void set_adc_params();
	void start_mca();
	void alloc_spectra();
	int program_digitizer(int);


	int32_t ret;
	int32_t handle_dpp;
	int32_t handle_board[MAXNB];

	CAEN_DGTZ_ErrorCode retdpp;

	int datum_total = 1, eventi = 0;

	/* Scan mode timer */
	struct itimerval it_val;
	bool write_coordinates = false, daq_run = false;

	int32_t board_id[MAXNB];

	char* buffer = nullptr;
	DigitizerParams_t params[MAXNB];
	CAENDPP_ConnectionParams_t con_params;
	CAEN_DGTZ_DPP_PHA_Params_t dpp_params[MAXNB];

	/* Data buffers */
	uint32_t* spectra[MAXNB][MaxNChannels];
	CAEN_DGTZ_DPP_PHA_Event_t* events[MaxNChannels];
	CAEN_DGTZ_DPP_PHA_Waveforms_t* waveform = nullptr;
};

digitizer::digitizer()
{
	size_t size = MAXNB * sizeof(DigitizerParams_t);
	memset(&params, 0, size);
	size = MAXNB * sizeof(CAEN_DGTZ_DPP_PHA_Params_t);
	memset(&dpp_params, 0, size);

	set_adc_params();
	if (shm::ptr.daq[15] != 1)
		set_dpp_params();
	else
		import_dpp_params();
	start_mca();

	alloc_spectra();
}

digitizer::~digitizer()
{
	//raise_quit();
}

void digitizer::set_timer(int timer_inter)
{
	it_val.it_value.tv_sec = timer_inter / 1000;
	it_val.it_value.tv_usec = (timer_inter * 1000) % 1000000;
	it_val.it_interval = it_val.it_value;
	if (setitimer(ITIMER_REAL, &it_val, NULL) == -1)
	{
		perror("[!] Error setting interval timer");
		exit(1);
	}
}

void digitizer::raise_flag()
{
	if (!write_coordinates)
		write_coordinates=true;
	else
		printf("write_coordinates is already true\n");
}

const int digitizer::get_total()
{
	return datum_total;
}

const int digitizer::get_events()
{
	return eventi;
}

void digitizer::set_adc_params()
{
	for (int b = 0; b < MAXNB; b++) {
		for (int ch = 0; ch < MaxNChannels; ch++)
			spectra[b][ch] = NULL;

		/* Communication parameters */
		params[b].LinkType = CAEN_DGTZ_USB;			// Bus type
		params[b].VMEBaseAddress = 0;				// For USB, VMEBaseAddress must be 0
		params[b].IOlev = CAEN_DGTZ_IOLevel_TTL;

		/* Acquisition parameters */
		params[b].AcqMode = CAEN_DGTZ_DPP_ACQ_MODE_List;            // CAEN_DGTZ_DPP_ACQ_MODE_List or CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope
		params[b].RecordLength = 2500;                              // Num of samples of the waveforms (only for Oscilloscope mode)
		params[b].ChannelMask = 0x3;                                // Channel enable mask////era 0xF
		params[b].EventAggr = 1;                                    // number of events in one aggregate (0=automatic)
		params[b].PulsePolarity = CAEN_DGTZ_PulsePolarityPositive;
	}
}

void digitizer::set_dpp_params()
{
	for (int b = 0; b < MAXNB; b++) {
		/* Trigger and shaping parameters for channel 0 */
		dpp_params[b].thr[0] = 500;// Trigger Threshold
		dpp_params[b].k[0] =  1500;// Trapezoid Rise Time (ns)
		dpp_params[b].m[0] = 1000;// Trapezoid Flat Top  (ns)
		dpp_params[b].M[0] = 3500;// Decay Time Constant (ns)
		dpp_params[b].ftd[0] = 600;// Peaking delay  (ns)
		dpp_params[b].a[0] = 2; // Trigger Filter smoothing factor
		dpp_params[b].b[0] = 100; // Delay(b)
		dpp_params[b].trgho[0] = 1300;// Trigger Hold Off
		dpp_params[b].nsbl[0] = 6;// Baseline mean del trapezio (ordine di comparsa nel menu)
		dpp_params[b].nspk[0] = 2;// Peak mean (ordine di comparsa nel menu)
		dpp_params[b].pkho[0] = 1100;// Peak holdoff
		dpp_params[b].blho[0] = 1100;// Baseline holdoff del trapezio
		dpp_params[b].enf[0] = 1;// Energy Normalization Factor
		dpp_params[b].dgain[0] = 0;//Digital Gain (ordine di comparsa nel menu)
		dpp_params[b].decimation[0] = 0;// Decimation

		/* Trigger and shaping parameters for channel 1 */
		dpp_params[b].thr[1] = 250;// Trigger Threshold
		dpp_params[b].k[1] =  1500;// Trapezoid Rise Time (ns)
		dpp_params[b].m[1] = 1000;// Trapezoid Flat Top  (ns)
		dpp_params[b].M[1] = 8000;// Decay Time Constant (ns)
		dpp_params[b].ftd[1] = 600;// Peaking delay  (ns)
		dpp_params[b].a[1] = 2; // Trigger Filter smoothing factor
		dpp_params[b].b[1] = 100; // Delay(b)
		dpp_params[b].trgho[1] = 1300;// Trigger Hold Off
		dpp_params[b].nsbl[1] = 6;// Baseline mean del trapezio (ordine di comparsa nel menu)
		dpp_params[b].nspk[1] = 2;// Peak mean (ordine di comparsa nel menu)
		dpp_params[b].pkho[1] = 1200;// Peak holdoff
		dpp_params[b].blho[1] = 1100;// Baseline holdoff del trapezio
		dpp_params[b].enf[1] = 1;// Energy Normalization Factor
		dpp_params[b].dgain[1] = 0;//Digital Gain (ordine di comparsa nel menu)
		dpp_params[b].decimation[1] = 0;// Decimation
	}
}

void digitizer::import_dpp_params()
{

}


void digitizer::stop_mca(bool exit_flag = 0)
{
	set_timer(0);

	/* Stop acquisition and free resources*/
	printf("[!] Closing digitizer\n");
	for (int b = 0; b < MAXNB; b++)
	{
		CAEN_DGTZ_SWStopAcquisition(handle_board[b]);
		CAEN_DGTZ_CloseDigitizer(handle_board[b]);
		for (int ch = 0; ch < MaxNChannels; ch++)
			if (spectra[b][ch] != nullptr)
				std::free(spectra[b][ch]);
	}
	void* ptr = events;
	CAEN_DGTZ_FreeReadoutBuffer(&buffer);
	CAEN_DGTZ_FreeDPPEvents(handle_board[0], &ptr);
	CAEN_DGTZ_FreeDPPWaveforms(handle_board[0], waveform);
	CAEN_DGTZ_ClearData(handle_board[0]);
	CAENDPP_EndLibrary(handle_dpp);

	shm::ptr.map_data[4] = datum_total;
	printf("[!] Total number of records in memory: %d\n", datum_total);
	if (exit_flag)
	{
		exit(3);
	}
}

void digitizer::start_mca()
{
	ret = CAENDPP_InitLibrary(&handle_dpp);
	for (int b = 0; b < MAXNB; b++)
	{
		retdpp = CAEN_DGTZ_OpenDigitizer(params[b].LinkType, b, 0, params[b].VMEBaseAddress, &handle_board[b]);
		if (retdpp)
		{
			printf("[!] Can't open CAEN digitizer\n");
			stop_mca(1);
		}

		CAEN_DGTZ_BoardInfo_t BoardInfo;
		retdpp = CAEN_DGTZ_GetInfo(handle_board[b], &BoardInfo);
		if (retdpp)
		{
			printf("[!] Can't read board info\n");
			stop_mca(1);
		}
		printf("\nConnected to CAEN Digitizer Model %s, recognized as board %d\n", BoardInfo.ModelName, b);
		printf("ROC FPGA Release is %s\n", BoardInfo.ROC_FirmwareRel);
		printf("AMC FPGA Release is %s\n", BoardInfo.AMC_FirmwareRel);

		int number = 128;
		sscanf(BoardInfo.AMC_FirmwareRel, "%d", &number);
		if (number != 128)
		{
			printf("[!] This digitizer has not a DPP-PHA firmware\n");
			stop_mca(1);
		}
		ret = CAENDPP_AddBoard(handle_dpp, con_params, &board_id[0]);

		ret = program_digitizer(b);
		if (ret)
		{
			printf("[!] Failed to program the digitizer\n");
			stop_mca(1);
		}
	}

	/* Memory allocation for the events and waveforms */
	uint32_t size_alloc;
	void* temp = &events[0];
	void** ptr_events = &temp;
	void* ptr_waveform = waveform;
	ret = CAEN_DGTZ_MallocReadoutBuffer(handle_board[0], &buffer, &size_alloc);
	ret |= CAEN_DGTZ_MallocDPPEvents(handle_board[0], ptr_events, &size_alloc);
	ret |= CAEN_DGTZ_MallocDPPWaveforms(handle_board[0], &ptr_waveform, &size_alloc);
	if (ret)
	{
		printf("[!] Can't allocate memory buffers\n");
		stop_mca(1);
	}
}

void digitizer::stop_daq()
{
	daq_run = 0;
}

void digitizer::daq_point_mode()
{
	ret = CAEN_DGTZ_SWStartAcquisition(handle_board [0]);
	if (ret)
		printf("[!] Problem starting acquisition, error code: %d\n", ret);
	else
	{
		daq_run = 1;
		printf("[!] Acquisition started\n");
	}


	void* temp = &events[0];
	void** ptr_events = &temp;

	int data = 0, caso = 5;
	uint16_t read_data = 0;
	int codeDetA = 20000000, codeDetB = 30000000;
	uint32_t NumEvents[MaxNChannels];
	uint32_t buff_size2 = 0;
	while (daq_run) // check for acquisition active
	{
		for (int b = 0; b < MAXNB; b++) { // Read data from the boards
			ret = CAEN_DGTZ_ReadData(handle_board[b], CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, buffer, &buff_size2);
			printf("Buffer Size: %d\n", buff_size2);
			if (ret)
				printf("[!] Readout error, code: %d\n", ret);

			if (buff_size2 != 0)
			{
				ret |= CAEN_DGTZ_GetDPPEvents(handle_board[b], buffer, buff_size2, ptr_events, NumEvents);
				printf("[!] NumEvents: %d %d\n", NumEvents[0], NumEvents[1]);
				if (ret)
					printf("[!] Data error, code: %d\n", ret);
			}
			else
				continue;

			for (int ch = 0; ch < MaxNChannels; ch++)
			{
				if ( !(params[b].ChannelMask & (1 << ch)) ) // Always false for channels 0 and 1
					continue;
				for (int ev = 0; ev < NumEvents[ch]; ev++)
				{
					if (buff_size2 != 0)
					{
						printf("what the fuck\n");
						data = events[ch][ev].Energy;
						//data = (int)read_data;
						printf("Data: %u\n", read_data);
					}
					else
						continue;

					printf("Data processing\n");
					if (data > 0 && data < 16384)	caso = 4; 	// Writes the energies
					else							caso = -1;

					switch (caso) {
					case 4:
						std::cout<<data<<std::endl;
						if (ch == 0)
						{
							shm::ptr.map_data[10 + datum_total] = data + codeDetA;
							datum_total++;

							shm::ptr.map_data[5] = ++eventi;
							shm::ptr.spectra[100 + data] += 1;
							shm::ptr.spectra[40000 + data] += 1;
						}
						if (ch == 1)
						{
							shm::ptr.map_data[10 + datum_total] = data + codeDetA;
							datum_total++;

							shm::ptr.map_data[5] = ++eventi;
							shm::ptr.spectra[100 + data] += 1;
							shm::ptr.spectra[20000 + data] += 1;

							//jlo = (double)(data - 1) * calGrad + calOffs;
							//jhi = jlo + calGrad;
							//data = ranqd1(idum,jlo,jhi);
							shm::ptr.spectra[40000 + data] += 1;
						}
						break;
					case 5:
						ev = NumEvents[ch];
						break;
					default:
						break;
					}
				}
			}
		}
	}
	stop_mca();
	printf("Exited the point mode loop\n");
}


namespace CAEN
{
/* For count rate calculations */
int eventi = 0;
int stored_event=0;
uint64_t CurrentTime, PrevRateTime, ElapsedTime;

/* For DAQ loop calculations */
uint32_t NumEvents[MaxNChannels];
uint32_t BufferSize;
int AcqRun = 0, BitMask = 0, data = 0;


}

static digitizer* ptr_daq = nullptr;
static void signal_handler(int signal)
{
	switch(signal)
	{
	case SIGALRM:
		if (ptr_daq != nullptr)
			ptr_daq->stop_daq();
		else
			printf("write_coordinates is already true\n");
		break;
	case SIGTERM:
	case SIGINT:
	case SIGQUIT:
		if (ptr_daq != nullptr)
			ptr_daq->stop_daq();
		else
		{
			printf("[!] Exiting on termination\n");
			exit(2);
		}
		break;
	default:
		break;
	}
}

int main(int argc, char* argv[])
{
	using namespace shm;
	ptr.flags 	= assign_shm<int>(keys.key_cmd, size.flags_size);
	ptr.spectra = assign_shm<int>(keys.key1, size.spectra_size);
	ptr.map_data= assign_shm<int>(keys.key2, size.map_size);
	ptr.daq 	= assign_shm<int>(keys.key4, size.daq_size);
	ptr.rate 	= assign_shm<int>(keys.key_rate, size.rate_size);
	ptr.var 	= assign_shm<float>(keys.key5, size.var_size);

	using namespace std;
	std::array<int, 16384> chan;
	std::iota(chan.begin(), chan.end(), 0);
	for (auto i: chan)
	{
		ptr.spectra[100 + i] = 0;
		ptr.spectra[20000 + i] = 0;
		ptr.spectra[40000 + i] = 0;
	}

	/* Signal handling (including timer for scan mode) */
	signal(SIGALRM, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);

	ptr_daq = new digitizer;
	if (true)
	{
		ptr_daq->stop_mca(1);
		return 0;
	}
	ptr_daq->set_timer(10000);
	ptr_daq->daq_point_mode();
	printf("\n[!] Recorded events: %d\n", ptr_daq->get_events());
	shm::ptr.map_data[4] = ptr_daq->get_total();
	shm::ptr.flags[70] = 0;

	delete ptr_daq;
	return 0;
}




void digitizer::alloc_spectra()
{
	for (int b = 0; b < MAXNB; b++)
	{
		for (int ch = 0; ch < MaxNChannels; ch++)
		{
			spectra[b][ch] = (uint32_t *)malloc((1 << MAXNBITS) * sizeof(uint32_t));
			memset(spectra[b][ch], 0, (1 << MAXNBITS) * sizeof(uint32_t));
		}
	}
}



int digitizer::program_digitizer(int b) {
	/* This function uses the CAENDigitizer API functions to perform the digitizer's initial configuration */
	ret = 0;

	/* Reset the digitizer */
	ret |= CAEN_DGTZ_Reset(handle_board[b]);
	if (ret)
	{
		printf("[!] Can't reset the digitizer\n");
		return -1;
	}

	ret |= CAEN_DGTZ_WriteRegister(handle_board[b], 0x8000, 0x010E0114);  // Channel Control Reg (indiv trg, seq readout) ??
	ret |= CAEN_DGTZ_SetDPPAcquisitionMode(handle_board[b], params[b].AcqMode, CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime);
	ret |= CAEN_DGTZ_SetAcquisitionMode(handle_board[b], CAEN_DGTZ_SW_CONTROLLED);
	ret |= CAEN_DGTZ_SetRecordLength(handle_board[b], params[b].RecordLength);
	ret |= CAEN_DGTZ_SetIOLevel(handle_board[b], params[b].IOlev);
	ret |= CAEN_DGTZ_SetExtTriggerInputMode(handle_board[b], CAEN_DGTZ_TRGMODE_ACQ_ONLY);
	ret |= CAEN_DGTZ_SetChannelEnableMask(handle_board[b], params[b].ChannelMask);
	ret |= CAEN_DGTZ_SetDPPEventAggregation(handle_board[b], params[b].EventAggr, 0);

	/* Set the mode used to synchronize the acquisition between different boards.
    In this example the sync is disabled */
	ret |= CAEN_DGTZ_SetRunSynchronizationMode(handle_board[b], CAEN_DGTZ_RUN_SYNC_Disabled);

	// Set the DPP specific parameters for the channels in the given channelMask
	ret |= CAEN_DGTZ_SetDPPParameters(handle_board[b], params[b].ChannelMask, &dpp_params[b]);


	uint32_t offset[2] = { 32768 };
	for (int i = 0; i < MaxNChannels; i++) {
		if (params[b].ChannelMask & (1 << i)) {
			ret |= CAEN_DGTZ_SetChannelDCOffset(handle_board[b], i, offset[i]);
			ret |= CAEN_DGTZ_SetDPPPreTriggerSize(handle_board[b], i, 500);
			ret |= CAEN_DGTZ_SetChannelPulsePolarity(handle_board[b], i, params[b].PulsePolarity);
		}
	}


	ret |= CAEN_DGTZ_SetDPP_PHA_VirtualProbe(handle_board[b], CAEN_DGTZ_DPP_VIRTUALPROBE_DUAL,
			CAEN_DGTZ_DPP_PHA_VIRTUALPROBE1_trapezoid, CAEN_DGTZ_DPP_PHA_VIRTUALPROBE2_Input, CAEN_DGTZ_DPP_PHA_DIGITAL_PROBE_Peaking);
	if (ret)
	{
		printf("Warning: errors found during the programming of the digitizer.\n"
				"Some settings may not be executed\n");
		return ret;
	}

	/* Input Range */
	CAENDPP_InputRange_t inRange[2] = { CAENDPP_InputRange_9_5Vpp };
	for (int ch = 0; ch < MaxNChannels; ch++)
	{
		ret = CAENDPP_SetInputRange(handle_board[b], ch, inRange[ch]);
		if (ret != 0)
			printf("[!] Can't set InputRange for channel %d, error %d\n", ch, ret);
	}
	return ret;
}


