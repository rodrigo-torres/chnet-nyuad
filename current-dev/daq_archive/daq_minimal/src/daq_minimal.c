#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>

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


int* assign_shm(key_t key, size_t size);
int ProgramDigitizer(int handle, DigitizerParams_t Params, CAEN_DGTZ_DPP_PHA_Params_t DPPParams);

int main(int argc, char *argv[]) {
	int *shared_memory_cmd = assign_shm(6900, SHMSZ_CMD_STATUS);
	int *shared_memory = assign_shm(7000, SHMSZ);


	CAEN_DGTZ_ErrorCode ret;
	char* buffer = NULL;

	CAEN_DGTZ_DPP_PHA_Event_t *Events[MaxNChannels];
	CAEN_DGTZ_DPP_PHA_Waveforms_t *Waveform=NULL;
	CAEN_DGTZ_DPP_PHA_Params_t DPPParams[MAXNB];
	DigitizerParams_t Params[MAXNB];



	/* The following variable will be used to get an handler for the digitizer. The
    handler will be used for most of CAENDigitizer functions to identify the board */
	int handle[MAXNB];

	/* Other variables */
	uint32_t NumEvents[MaxNChannels];
	uint32_t AllocatedSize, BufferSize;
	CAEN_DGTZ_BoardInfo_t BoardInfo;



	memset(&Params, 0, MAXNB*sizeof(DigitizerParams_t));
	memset(&DPPParams, 0, MAXNB*sizeof(CAEN_DGTZ_DPP_PHA_Params_t));

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

		/****************************\
		 * Trigger and Shaping params *
        \****************************/

		for (int ch = 0; ch < MaxNChannels; ch++)
		{
			DPPParams[b].thr[ch] = 500;// Trigger Threshold
			DPPParams[b].k[ch] =  1500;// Trapezoid Rise Time (ns)
			DPPParams[b].m[ch] = 1000;// Trapezoid Flat Top  (ns)
			DPPParams[b].M[ch] = 3500;// Decay Time Constant (ns)
			DPPParams[b].ftd[ch] = 600;// Peaking delay  (ns)
			DPPParams[b].a[ch] = 2; // Trigger Filter smoothing factor
			DPPParams[b].b[ch] = 100; // Delay(b)
			DPPParams[b].trgho[ch] = 1300;// Trigger Hold Off
			DPPParams[b].nsbl[ch] = 6;// Baseline mean del trapezio (ordine di comparsa nel menu)
			DPPParams[b].nspk[ch] = 2;// Peak mean (ordine di comparsa nel menu)
			DPPParams[b].pkho[ch] = 1100;// Peak holdoff
			DPPParams[b].blho[ch] = 1100;// Baseline holdoff del trapezio
			DPPParams[b].enf[ch] = 1;// Energy Normalization Factor
			DPPParams[b].dgain[ch] = 0;//Digital Gain (ordine di comparsa nel menu)
			DPPParams[b].decimation[ch] = 0;// Decimation
		}


	}

	for (int b = 0; b < MAXNB; b++)
	{
		ret = CAEN_DGTZ_OpenDigitizer(Params[b].LinkType, b, 0, Params[b].VMEBaseAddress, &handle[b]);
		if (ret)
		{
			printf("[!] Can't open digitizer\n");
			goto QuitProgram;
		}

		ret = CAEN_DGTZ_GetInfo(handle[b], &BoardInfo);
		if (ret)
		{
			printf("[!] Can't read board info\n");
			goto QuitProgram;
		}
		printf("\nConnected to CAEN Digitizer Model %s, recognized as board %d\n", BoardInfo.ModelName, b);
		printf("ROC FPGA Release is %s\n", BoardInfo.ROC_FirmwareRel);
		printf("AMC FPGA Release is %s\n", BoardInfo.AMC_FirmwareRel);

		int MajorNumber;
		sscanf(BoardInfo.AMC_FirmwareRel, "%d", &MajorNumber);
		if (MajorNumber != 128)
		{
			printf("[!] This digitizer has not a DPP-PHA firmware\n");
			goto QuitProgram;
		}
		int32_t bId[MAXNB];
	}

	for (int b = 0; b < MAXNB; b++)
	{
		ret = ProgramDigitizer(handle[b], Params[b], DPPParams[b]);
		if (ret)
		{
			printf("[!] Failed to program the digitizer\n");
			goto QuitProgram;
		}
	}

	ret = CAEN_DGTZ_MallocReadoutBuffer(handle[0], &buffer, &AllocatedSize);
	ret |= CAEN_DGTZ_MallocDPPEvents(handle[0], Events, &AllocatedSize);        /* Allocate memory for the events */
	ret |= CAEN_DGTZ_MallocDPPWaveforms(handle[0], &Waveform, &AllocatedSize);  /* Allocate memory for the waveforms */
	if (ret)
	{
		printf("[!] Can't allocate memory buffers\n");
		goto QuitProgram;
	}
	printf("Acquisition debug: %d\n", shared_memory_cmd[100]);

	/* Clean histogram buffers */
	int mask = 0x3FFF;
	for (int ch = 0; ch < mask; ch++)
	{
		shared_memory[100 + ch] = 0;
		shared_memory[20000 + ch] = 0;
		shared_memory[40000 + ch] = 0;
	}


	ret = CAEN_DGTZ_SWStartAcquisition(handle[0]);
	if (ret || !shared_memory_cmd[100])
		printf("[!] Problem starting acquisition, error code: %d\n", ret);
	else
		printf("[!] Acquisition started\n");

	int data = 0x3FFF, caso = 10, eventi = 0;
	while (shared_memory_cmd[100]) {
		for (int b = 0; b < MAXNB; b++)
		{
			ret = CAEN_DGTZ_ReadData(handle[b], CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, buffer, &BufferSize);
			if (ret)
				printf("[!] Readout error, code: %d\n", ret);

			if (BufferSize != 0)
			{
				ret |= CAEN_DGTZ_GetDPPEvents(handle[b], buffer, BufferSize, Events, NumEvents);
				if (ret)
					printf("[!] Data error, code: %d\n", ret);
			}

			for (int ch = 0; ch < MaxNChannels; ch++)
			{
				for (int ev = 0; ev < NumEvents[ch]; ev++)
				{
					if (BufferSize != 0) data = Events[ch][ev].Energy;
					else continue;

					if (!(data&(~mask))) caso = 4;

					switch (caso)
					{
					case 4:
						if (ch == 0)
						{
							*(shared_memory+100+data) += 1;
							*(shared_memory+40000+data) += 1;
						}
						if (ch == 1)
						{
							printf("Data: %d\n", data);
							*(shared_memory+20000+data) += 1;
							*(shared_memory+40000+data) += 1;
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

	goto QuitProgram;

	QuitProgram:
	for (int b = 0; b < MAXNB; b++)
	{
		CAEN_DGTZ_SWStopAcquisition(handle[b]);
	}
	CAEN_DGTZ_FreeReadoutBuffer(&buffer);
	CAEN_DGTZ_FreeDPPEvents(handle[0], Events);
	CAEN_DGTZ_FreeDPPWaveforms(handle[0], Waveform);
	CAEN_DGTZ_ClearData(handle[0]);
	int b = 0;
	printf("[!] Closing digitizer\n");
	CAEN_DGTZ_CloseDigitizer(handle[b]);

	return ret;
}

int* assign_shm(key_t key, size_t size)
{
	int shm_id = shmget(key, size, IPC_CREAT | 0666);
	if (shm_id == -1)
	{
		printf("[!] Can't obtain an SHM segment ID for key\t%d\n", key);
		printf("%s\n", strerror(errno));
		return NULL;
	}
	else
		return (int*)shmat(shm_id, NULL, 0);
}

int ProgramDigitizer(int handle, DigitizerParams_t Params, CAEN_DGTZ_DPP_PHA_Params_t DPPParams) {
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

	uint32_t offset[2] = {32768, 32768};
	for(int i=0; i<MaxNChannels; i++)
	{
		if (Params.ChannelMask & (1<<i))
		{
			ret |= CAEN_DGTZ_SetChannelDCOffset(handle, i, offset[i]);
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
}
