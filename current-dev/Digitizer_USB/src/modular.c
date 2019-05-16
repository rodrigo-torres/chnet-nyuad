

/* --------------------------------------------------------------------------------------------------------- */
/*! \fn      int ProgramDigitizer(int handle, DigitizerParams_t params, CAEN_DGTZ_DPP_PHA_Params_t paramsPHA)
*   \brief   This function uses the CAENDigitizer API functions to perform the digitizer's initial configuration
*   \return  0=success; -1=error */
/* --------------------------------------------------------------------------------------------------------- */

uint32_t offset[2] = {32768, 32768};
CAENDPP_InputRange_t inRange[2] = { CAENDPP_InputRange_9_5Vpp, CAENDPP_InputRange_9_5Vpp };

int ProgramDigitizer(int handle, DigitizerParams_t Params, CAEN_DGTZ_DPP_PHA_Params_t DPPParams) {
  
    int ret = 0;
    ret |= CAEN_DGTZ_Reset(handle);    /* Reset the digitizer */
    if (ret) { printf("[!] Can't reset the digitizer\n"); return -1; }

    ret |= CAEN_DGTZ_WriteRegister(handle, 0x8000, 0x010E0114);  // Channel Control Reg (indiv trg, seq readout) ??
    ret |= CAEN_DGTZ_SetDPPAcquisitionMode(handle, Params.AcqMode, CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime);
    ret |= CAEN_DGTZ_SetAcquisitionMode(handle, CAEN_DGTZ_SW_CONTROLLED);
    ret |= CAEN_DGTZ_SetRecordLength(handle, Params.RecordLength);
    ret |= CAEN_DGTZ_SetIOLevel(handle, Params.IOlev);
    ret |= CAEN_DGTZ_SetExtTriggerInputMode(handle, CAEN_DGTZ_TRGMODE_ACQ_ONLY);
    ret |= CAEN_DGTZ_SetChannelEnableMask(handle, Params.ChannelMask);
    ret |= CAEN_DGTZ_SetDPPEventAggregation(handle, Params.EventAggr, 0);
    
    /* Set the mode used to syncronize the acquisition between different boards.
    In this example the sync is disabled */
    ret |= CAEN_DGTZ_SetRunSynchronizationMode(handle, CAEN_DGTZ_RUN_SYNC_Disabled);
    
    // Set the DPP specific parameters for the channels in the given channelMask
    ret |= CAEN_DGTZ_SetDPPParameters(handle, Params.ChannelMask, &DPPParams);

    if (*(shared_memory4+15) == 1) {
        int offset = *(shared_memory4+7);
        printf("[!] Setting DC Offset from DPP interface as:\t%d\n",offset);
        for (int i = 0; i < MaxNChannels; i++) {
            if (Params.ChannelMask & (1<<i)) {
                // Set a DC offset to the input signal to adapt it to digitizer's dynamic range
                ret |= CAEN_DGTZ_SetChannelDCOffset(handle, i, offset);
                ret |= CAEN_DGTZ_SetDPPPreTriggerSize(handle, i, 500);
                ret |= CAEN_DGTZ_SetChannelPulsePolarity(handle, i, Params.PulsePolarity);
            }
        }
    }

    else {
        
        for(int i=0; i<MaxNChannels; i++) {
            if (Params.ChannelMask & (1<<i)) {
                ret |= CAEN_DGTZ_SetChannelDCOffset(handle, i, offset[i]);
                ret |= CAEN_DGTZ_SetDPPPreTriggerSize(handle, i, 500);
                ret |= CAEN_DGTZ_SetChannelPulsePolarity(handle, i, Params.PulsePolarity);
            }
        }
    }
    
    ret |= CAEN_DGTZ_SetDPP_PHA_VirtualProbe(handle, CAEN_DGTZ_DPP_VIRTUALPROBE_DUAL, CAEN_DGTZ_DPP_PHA_VIRTUALPROBE1_trapezoid, CAEN_DGTZ_DPP_PHA_VIRTUALPROBE2_Input, CAEN_DGTZ_DPP_PHA_DIGITAL_PROBE_Peaking);

    if (ret) {
        printf("Warning: errors found during the programming of the digitizer.\nSome settings may not be executed\n");
        return ret;
    }

    /****************************\
    *        INPUT RANGE         *
    \****************************/

    if (*(shared_memory4+15) != 1) { // To load pre-defined values  
        for (ch = 0; ch < MaxNChannels; ch++) {
        	ret = CAENDPP_SetInputRange(handledpp, ch, inRange[ch]);
            if (ret != 0) printf("[!] Can't set InputRange for channel %d, error %d\n",ch,ret);
        }
    }
    else {
	CAENDPP_InputRange_t inRange[4] = { 3L, 2L, 1L, 0L }; // refer to CAENDPPLibTypes
	int choice[2] = { *(shared_memory4+1), *(shared_memory4+16) };
	
        for (int ch = 0; ch < MaxNChannels; ch++) {
        	ret |= CAENDPP_SetInputRange(handledpp, ch, inRange[choice[ch]])
            if (ret != 0) printf("[!] Can't set InputRange for channel %d, error %d\n",ch,ret);
        }
    }

    return ret;
}
