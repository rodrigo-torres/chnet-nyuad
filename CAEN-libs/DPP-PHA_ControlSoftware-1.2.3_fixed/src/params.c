/******************************************************************************
* 
* CAEN SpA - Front End Division
* Via Vetraia, 11 - 55049 - Viareggio ITALY
* +390594388398 - www.caen.it
*
***************************************************************************//**
* \note TERMS OF USE:
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software
* Foundation. This program is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. The user relies on the 
* software, documentation and results solely at his own risk.
******************************************************************************/

/******************************************************************************
* This file contains the definition of the configuration parameters supported
* by the dpprunner program. Each parameter can be assigned a default value so
* that if the configuration file does not redefine it, a sensible default can
* be used.
*
* Parameters are defined via a macro system.
*
* The list of parameters, alongside their default value is specified inside
* the following macro:
*
*		CONFIGURATION_PARAMETERS { 
*			... 
*		}
*
* A parameter is defined either with:
*
*		COMMON_PARAMETER( UPDATE_NAME, "NAME", "DEFAULT_VALUE" );
* or
*
*		CHANNEL_PARAMETER( UPDATE_NAME, "NAME", "DEFAULT_VALUE" );
*
* Common parameters will be looked up only inside the COMMON section, while
* Channel parameters will be firstly looked up inside the COMMON section and
* then in each channel section.
*
* A parameter is assigned an update action that is executed when the parameter
* is parsed in the configuration file or is input via the command line
* An update action is defined as follows:
*
*		UPDATE( UPDATE_NAME ) {
*			...
*		}
*
* An update is supposed to modify the configuration of the program and/or call
* specific configuration-altering functions
* The following macros are available for use inside an update:
*		UPDATE_GET_DIG() returns a pointer to the Digitizer object
*		UPDATE_GET_ANA() returns a pointer to the Analyzer object
*		UPDATE_GET_PLO() returns a pointer to the Plotter object
*		UPDATE_SECTION returns the section the configuration parameter has
*					   been found in, as a const char*
*		UPDATE_KEY returns the name of the configuration parameter, as a
*				   const char*
*		UPDATE_VALUE returns the value of the configuration parameter, as a
*					 const char*
*		UPDATE_RESTART() requires a restart of the dpprunner, so that all the
*						 preliminary operations are performed again. If the
*						 board has not been initialized yet, the statement is
*						 ignored
*
* NOTE: update actions for channel parameters should typically branch on the
*		UPDATE_SECTION, so that the update can discriminate when the parameter
*		is found in the common or channel section and take appropriate action
*
******************************************************************************/

#include "dppswitch.h"

#ifdef DPP_TF_OLD

#include <stdio.h>
#include <string.h>

#include "Configurator.h"



UPDATE(uOpen) {
	Digitizer* dig = UPDATE_GET_DIG();
	char linkTypeString[15];
	int linkNum=0, conetNode=0;
	unsigned int baseAddress=0;
	if (sscanf(UPDATE_VALUE,"%15s %d %d %x",linkTypeString,&linkNum,&conetNode,&baseAddress)>=3) {
		if (strcmp(linkTypeString,"USB")==0) dig->linkType = CAEN_DGTZ_USB;
		else if (strcmp(linkTypeString,"PCI")==0) dig->linkType = CAEN_DGTZ_PCI_OpticalLink;
		else userMsg("Config OPEN: Invalid connection type \"%s\"\n",linkTypeString); 
		dig->linkNum = linkNum;
		dig->conetNode = conetNode;
		dig->baseAddress = baseAddress;
	} else 
		userMsg("Config OPEN: Invalid value string \"%s\"\n",UPDATE_VALUE);
}

UPDATE(uRecordLength) { 
	UPDATE_GET_DIG()->recordLength = atoi(UPDATE_VALUE); 
	UPDATE_RESTART();
}

UPDATE(uPulsePolarity) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_VALUE,"POSITIVE")==0) dig->pulsePolarity = 1;
	else if (_stricmp(UPDATE_VALUE,"NEGATIVE")==0) dig->pulsePolarity = 0;
	else userMsg("Config %s: invalid value string \"%s\"\n", UPDATE_KEY, UPDATE_VALUE);
	UPDATE_RESTART();
}

UPDATE(uFreeWrite) {
	Digitizer* dig = UPDATE_GET_DIG();
	uint32_t address=0, value=0;
	if (!freeWritesFull(dig) && sscanf(UPDATE_VALUE, "%x %x", &address, &value)==2) {
		addFreeWrite(dig,address,value);
	}
}

UPDATE(uExternalTrigger) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_VALUE,"DISABLED")==0) dig->extTriggerMode = CAEN_DGTZ_TRGMODE_DISABLED; 
	else if (_stricmp(UPDATE_VALUE,"ACQUISITION_ONLY")==0) dig->extTriggerMode = CAEN_DGTZ_TRGMODE_ACQ_ONLY;
	else if (_stricmp(UPDATE_VALUE,"ACQUISITION_AND_TRGOUT")==0) dig->extTriggerMode = CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT;
	else userMsg("Config %s: invalid value string \"%s\"\n",UPDATE_KEY, UPDATE_VALUE);
}

UPDATE(uAcqMode) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_VALUE,"OSCILLOSCOPE")==0) { dig->acquisitionMode = CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope; UPDATE_GET_ANA()->plotType = PLOT_WAVEFORMS; }
	else if (_stricmp(UPDATE_VALUE,"LIST")==0) { dig->acquisitionMode = CAEN_DGTZ_DPP_ACQ_MODE_List; UPDATE_GET_ANA()->plotType = PLOT_EHISTO; }
	else if (_stricmp(UPDATE_VALUE,"MIXED")==0) dig->acquisitionMode = CAEN_DGTZ_DPP_ACQ_MODE_Mixed;
	else userMsg("Config %s: invalid value string \"%s\"\n",UPDATE_KEY, UPDATE_VALUE);
	UPDATE_RESTART();
}

UPDATE(uListMode) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_VALUE,"ENERGY_ONLY")==0) dig->listParams = CAEN_DGTZ_DPP_SAVE_PARAM_EnergyOnly;
	else if (_stricmp(UPDATE_VALUE, "TIME_ONLY")==0) dig->listParams = CAEN_DGTZ_DPP_SAVE_PARAM_TimeOnly;
	else if (strcmp(UPDATE_VALUE, "ENERGY_AND_TIME")==0) dig->listParams = CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime;
    else userMsg("Config %s: Invalid value string \"%s\"\n", UPDATE_KEY, UPDATE_VALUE);
	UPDATE_RESTART();
}

UPDATE(uMaxEvents) {
	UPDATE_GET_DIG()->maxNumEvents = atoi(UPDATE_VALUE);
}

UPDATE(uPostTrigger) {
	Digitizer* dig = UPDATE_GET_DIG();
	dig->postTrigger = atoi(UPDATE_VALUE);
	UPDATE_RESTART();
}

UPDATE(uDppTrgThreshold) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) dig->dppParams[i].thr = atoi(UPDATE_VALUE);
	} else {
		dig->dppParams[atoi(UPDATE_SECTION)].thr = atoi(UPDATE_VALUE);
	}
	UPDATE_RESTART();
}

UPDATE(uDppDecimation) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) dig->decimation[i] = atoi(UPDATE_VALUE);
	} else {
		dig->decimation[atoi(UPDATE_SECTION)] = atoi(UPDATE_VALUE);
	}
	UPDATE_RESTART();
}

UPDATE(uDppDecayTau) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) dig->dppParams[i].M = (int)(atof(UPDATE_VALUE)*1000);
	} else {
		dig->dppParams[atoi(UPDATE_SECTION)].M = (int)(atof(UPDATE_VALUE)*1000);
	}
	UPDATE_RESTART();
}

UPDATE(uDppShapingTime) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) dig->dppParams[i].k = (int)(atof(UPDATE_VALUE)*1000);
	} else {
		dig->dppParams[atoi(UPDATE_SECTION)].k = (int)(atof(UPDATE_VALUE)*1000);
	}
	UPDATE_RESTART();
}

UPDATE(uDppFlatTop) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) dig->dppParams[i].m = (int)(atof(UPDATE_VALUE)*1000);
	} else {
		dig->dppParams[atoi(UPDATE_SECTION)].m = (int)(atof(UPDATE_VALUE)*1000);
	}
	UPDATE_RESTART();
}

UPDATE(uDppPeakingDelay) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) dig->dppParams[i].ftd = (int)(atof(UPDATE_VALUE)*1000);
	} else {
		dig->dppParams[atoi(UPDATE_SECTION)].ftd = (int)(atof(UPDATE_VALUE)*1000);
	}
	UPDATE_RESTART();
}

UPDATE(uDppNsMean) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) dig->dppParams[i].a = atoi(UPDATE_VALUE);
	} else {
		dig->dppParams[atoi(UPDATE_SECTION)].a = atoi(UPDATE_VALUE);
	}
	UPDATE_RESTART();
}

UPDATE(uDppRiseTime) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) dig->dppParams[i].b = (int)(atof(UPDATE_VALUE) * 1000);
	} else {
		dig->dppParams[atoi(UPDATE_SECTION)].b = (int)(atof(UPDATE_VALUE) * 1000);
	}
	UPDATE_RESTART();
}

UPDATE(uDppTrapExtraTime) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) dig->dppParams[i].tpet = (int)(atof(UPDATE_VALUE) * 1000);
	} else {
		dig->dppParams[atoi(UPDATE_SECTION)].tpet = (int)(atof(UPDATE_VALUE) * 1000);
	}
	UPDATE_RESTART();
}

UPDATE(uDppTrgHoldoff) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) dig->dppParams[i].trgho = (int)(atof(UPDATE_VALUE) * 1000);
	} else {
		dig->dppParams[atoi(UPDATE_SECTION)].trgho = (int)(atof(UPDATE_VALUE) * 1000);
	}
	UPDATE_RESTART();
}

UPDATE(uDppTrgWinWidth) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) dig->dppParams[i].twwdt = (int)(atof(UPDATE_VALUE) * 1000);
	} else {
		dig->dppParams[atoi(UPDATE_SECTION)].twwdt = (int)(atof(UPDATE_VALUE) * 1000);
	}
	UPDATE_RESTART();
}

UPDATE(uDppOptBaseline) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) dig->dppParams[i].nsbl = atoi(UPDATE_VALUE);
	} else {
		dig->dppParams[atoi(UPDATE_SECTION)].nsbl = atoi(UPDATE_VALUE);
	}
	UPDATE_RESTART();
}

UPDATE(uDppOptPeakMean) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) dig->dppParams[i].nspk = atoi(UPDATE_VALUE);
	} else {
		dig->dppParams[atoi(UPDATE_SECTION)].nspk = atoi(UPDATE_VALUE);
	}
	UPDATE_RESTART();
}

UPDATE(uDppDigitalGain) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) dig->dppParams[i].dgain = (float)atof(UPDATE_VALUE);
	} else {
		dig->dppParams[atoi(UPDATE_SECTION)].dgain = (float)atof(UPDATE_VALUE);
	}
	UPDATE_RESTART();
}

UPDATE(uDppEnableOvltrap) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) dig->dppParams[i].otrej = _stricmp(UPDATE_VALUE,"YES")==0;
	} else {
		dig->dppParams[atoi(UPDATE_SECTION)].otrej = _stricmp(UPDATE_VALUE,"YES")==0;
	}
	UPDATE_RESTART();
}

UPDATE(uDppEnableRTDiscr) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) dig->dppParams[i].trgwin = _stricmp(UPDATE_VALUE,"YES")==0;
	} else {
		dig->dppParams[atoi(UPDATE_SECTION)].trgwin = _stricmp(UPDATE_VALUE,"YES")==0;
	}
	UPDATE_RESTART();
}

UPDATE(uDcOffset) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) {
			dig->dcOffset[i]  = (int)((atof(UPDATE_VALUE)+50) * 65535 / 100);
			if (checkBoardInited(dig)) CAEN_DGTZ_SetChannelDCOffset(dig->boardHandle, i, dig->dcOffset[i]);
		}
	} else {
		dig->dcOffset[atoi(UPDATE_SECTION)] = (int)((atof(UPDATE_VALUE)+50) * 65535 / 100);
		if (checkBoardInited(dig)) CAEN_DGTZ_SetChannelDCOffset(dig->boardHandle, atoi(UPDATE_SECTION), dig->dcOffset[atoi(UPDATE_SECTION)]);
	}
}

UPDATE(uFPIOLevel) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_VALUE,"TTL")==0) dig->fpioType = 1;
	else if (_stricmp(UPDATE_VALUE, "NIM")==0) dig->fpioType = 0;
    else userMsg("Config %s: Invalid value string \"%s\"\n", UPDATE_KEY, UPDATE_VALUE);
}

UPDATE(uChannelEnable) {
	Digitizer* dig = UPDATE_GET_DIG();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		dig->enableMask = (_stricmp(UPDATE_VALUE,"YES")==0) ? 0xFF : 0;
	} else {
		if (_stricmp(UPDATE_VALUE,"YES")==0) dig->enableMask |= (1 << atoi(UPDATE_SECTION));
		else dig->enableMask &= ~(1 << atoi(UPDATE_SECTION));
	}
	UPDATE_RESTART(); /* fiddling with channel enable requires a runner restart b/c memory has to be allocated when a channel is newly enabled */
}



UPDATE(uVirtualProbeA1) {
	UPDATE_GET_ANA()->vprobes[0] = atoi(UPDATE_VALUE);
	updateVProbes(UPDATE_GET_ANA());
}

UPDATE(uVirtualProbeA2) {
	UPDATE_GET_ANA()->vprobes[1] = atoi(UPDATE_VALUE);
	updateVProbes(UPDATE_GET_ANA());
}

UPDATE(uVirtualProbeD) {
	UPDATE_GET_ANA()->vprobes[2] = atoi(UPDATE_VALUE);
	updateVProbes(UPDATE_GET_ANA());
}

UPDATE(uDualTrace) {
	if (_stricmp(UPDATE_VALUE,"YES")==0) UPDATE_GET_ANA()->dualTrace = 1;
	else if (_stricmp(UPDATE_VALUE,"NO")==0) UPDATE_GET_ANA()->dualTrace = 0;
	else userMsg("Config %s: Invalid value string\n", UPDATE_KEY);
	UPDATE_RESTART();
}

UPDATE(uTimeHistoMax) {
	UPDATE_GET_ANA()->tmaxHisto = (int)(atof(UPDATE_VALUE)*1000);
}

UPDATE(uDTCTimeInterval) {
	UPDATE_GET_ANA()->dtcTime = (int)(atof(UPDATE_VALUE)*1000);
}

UPDATE(uDTCEventInterval) {
	UPDATE_GET_ANA()->dtcEvents = atoi(UPDATE_VALUE); 
}

UPDATE(uEminSkim) {
	UPDATE_GET_ANA()->eminSkim = (float)atof(UPDATE_VALUE);
}

UPDATE(uEmaxSkim) {
	UPDATE_GET_ANA()->emaxSkim = (float)atof(UPDATE_VALUE);
}

UPDATE(uEnableEnergySkim) {
	UPDATE_GET_ANA()->eSkimEnabled = _stricmp(UPDATE_VALUE,"YES")==0;
}

UPDATE(uStopTime) {
	Analyzer* ana = UPDATE_GET_ANA();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) ana->stopTime[i]  = (uint64_t)atof(UPDATE_VALUE)*1000000000;
	} else {
		ana->stopTime[atoi(UPDATE_SECTION)] = (uint64_t)atof(UPDATE_VALUE)*1000000000;
	}
}


UPDATE(uGnuplotPath) {
	UPDATE_GET_PLO()->plotterPath = calloc(strlen(UPDATE_VALUE)+1,sizeof(char));
	strncpy(UPDATE_GET_PLO()->plotterPath,&UPDATE_VALUE[1],strlen(UPDATE_VALUE)-2);
}

UPDATE(uVirtualProbeA1Gain) {
	UPDATE_GET_ANA()->traceGains[0] = atoi(UPDATE_VALUE);
}

UPDATE(uVirtualProbeA2Gain) {
	UPDATE_GET_ANA()->traceGains[1] = atoi(UPDATE_VALUE);
}

UPDATE(uVirtualProbeDGain) {
	UPDATE_GET_ANA()->traceGains[2] = atoi(UPDATE_VALUE);
}

UPDATE(uVirtualProbeD2Gain) {
	UPDATE_GET_ANA()->traceGains[3] = atoi(UPDATE_VALUE);
}

UPDATE(uVirtualProbeA1Off) {
	UPDATE_GET_ANA()->traceOffsets[0] = atoi(UPDATE_VALUE);
}

UPDATE(uVirtualProbeA2Off) {
	UPDATE_GET_ANA()->traceOffsets[1] = atoi(UPDATE_VALUE);
}

UPDATE(uVirtualProbeDOff) {
	UPDATE_GET_ANA()->traceOffsets[2] = atoi(UPDATE_VALUE);
}

UPDATE(uVirtualProbeD2Off) {
	UPDATE_GET_ANA()->traceOffsets[3] = atoi(UPDATE_VALUE);
}

UPDATE(uVirtualProbeA1En) {
	UPDATE_GET_ANA()->traceEnable[0] = !_stricmp(UPDATE_VALUE,"YES");
}

UPDATE(uVirtualProbeA2En) {
	UPDATE_GET_ANA()->traceEnable[1] = !_stricmp(UPDATE_VALUE,"YES");
}

UPDATE(uVirtualProbeDEn) {
	UPDATE_GET_ANA()->traceEnable[2] = !_stricmp(UPDATE_VALUE,"YES");
}

UPDATE(uVirtualProbeD2En) {
	UPDATE_GET_ANA()->traceEnable[3] = !_stricmp(UPDATE_VALUE,"YES");
}

UPDATE(uCalibrationX1) {
	Analyzer* ana = UPDATE_GET_ANA();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) { 
			ana->calib[i].x0 = atoi(UPDATE_VALUE);
		}
	} else {
		ana->calib[atoi(UPDATE_SECTION)].x0 = atoi(UPDATE_VALUE);
	}
}

UPDATE(uCalibrationX2) {
	Analyzer* ana = UPDATE_GET_ANA();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) { 
			ana->calib[i].x1 = atoi(UPDATE_VALUE);
		}
	} else {
		ana->calib[atoi(UPDATE_SECTION)].x1 = atoi(UPDATE_VALUE);
	}
}

UPDATE(uCalibrationY1) {
	Analyzer* ana = UPDATE_GET_ANA();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) { 
			ana->calib[i].y0 = (float)atof(UPDATE_VALUE);
		}
	} else {
		ana->calib[atoi(UPDATE_SECTION)].y0 = (float)atof(UPDATE_VALUE);
	}
}

UPDATE(uCalibrationY2) {
	Analyzer* ana = UPDATE_GET_ANA();
	if (_stricmp(UPDATE_SECTION,"COMMON")==0) {
		int i=0;
		for (i=0; i<MAX_CH; i++) { 
			ana->calib[i].y1 = (float)atof(UPDATE_VALUE);
		}
	} else {
		ana->calib[atoi(UPDATE_SECTION)].y1 = (float)atof(UPDATE_VALUE);
	}
}

UPDATE(uDumpFileName) {
	UPDATE_GET_DPR()->dumpFilePrefix = calloc(strlen(UPDATE_VALUE)+1,sizeof(char));
	strcpy(UPDATE_GET_DPR()->dumpFilePrefix,UPDATE_VALUE);
}

UPDATE(uDumpWaveforms) {
	UPDATE_GET_ANA()->dumpWf = !_stricmp(UPDATE_VALUE,"YES");
}

UPDATE(uDumpList) {
	UPDATE_GET_ANA()->dumpLs = !_stricmp(UPDATE_VALUE,"YES");
}

UPDATE(uDumpEHisto) {
	UPDATE_GET_ANA()->dumpEh = !_stricmp(UPDATE_VALUE,"YES");
}

UPDATE(uDumpTHisto) {
	UPDATE_GET_ANA()->dumpTh = !_stricmp(UPDATE_VALUE,"YES");
}

UPDATE(uPlotFps) {
	UPDATE_GET_PLO()->fps = atof(UPDATE_VALUE);
}

/* Configuration parameters definition */

CONFIGURATION_PARAMETERS (

	COMMON_PARAMETER(	uOpen,				"OPEN",					"USB 0 0"			);
	COMMON_PARAMETER(	uRecordLength,		"RECORD_LENGTH",		"16384"				);
	COMMON_PARAMETER(	uPulsePolarity,		"PULSE_POLARITY",		"POSITIVE"			);
	COMMON_PARAMETER(	uFreeWrite,			"WRITE_REGISTER",		NO_DEFAULT_VALUE	);
	COMMON_PARAMETER(	uExternalTrigger,	"EXTERNAL_TRIGGER",		"DISABLED"			);
	COMMON_PARAMETER(	uAcqMode,			"ACQUISITION_MODE",		"OSCILLOSCOPE"		);
	COMMON_PARAMETER(	uListMode,			"LIST_PARAMS",			"ENERGY_ONLY"		);
	COMMON_PARAMETER(	uMaxEvents,			"MAX_NUM_EVENTS_BLT",	"1023"				);
	COMMON_PARAMETER(	uPostTrigger,		"POST_TRIGGER",			"80"				);

	CHANNEL_PARAMETER(	uChannelEnable,		"ENABLE_INPUT",			"YES"				);
	
	CHANNEL_PARAMETER(	uDcOffset,			"DC_OFFSET",			"0"					);
	COMMON_PARAMETER(	uFPIOLevel,			"FPIO_LEVEL",			"NIM"				);

	CHANNEL_PARAMETER(	uDppDecimation,		"DECIMATION",			"0"					);
	CHANNEL_PARAMETER(	uDppTrgThreshold,	"TRG_THRESHOLD",		"0"					);
	CHANNEL_PARAMETER(	uDppDecayTau,		"TF_DECAY_TAU",			"0"					);
	CHANNEL_PARAMETER(	uDppShapingTime,	"TF_SHAPING_TIME",		"0"					);
	CHANNEL_PARAMETER(	uDppFlatTop,		"TF_FLAT_TOP",			"0"					);
	CHANNEL_PARAMETER(	uDppPeakingDelay,	"TF_PEAKING_DELAY",		"0"					);
	CHANNEL_PARAMETER(	uDppNsMean,			"NS_MOV_WIN_AVG",		"0"					);
	CHANNEL_PARAMETER(	uDppRiseTime,		"RISE_TIME",			"0"					);
	CHANNEL_PARAMETER(	uDppTrapExtraTime,	"TF_BL_HOLDOFF",		"0"					);
	CHANNEL_PARAMETER(	uDppTrgHoldoff,		"TRG_HOLDOFF",			"0"					);
	CHANNEL_PARAMETER(	uDppOptBaseline,	"TF_SEL_BASELINE",		"0"					);
	CHANNEL_PARAMETER(	uDppOptPeakMean,	"TF_SEL_PEAKMEAN",		"0"					);
	CHANNEL_PARAMETER(	uDppDigitalGain,	"DIGITAL_GAIN",			"0"					);
	CHANNEL_PARAMETER(	uDppEnableOvltrap,	"TF_ENABLE_OVLTRAP_REJ","NO"				);
	CHANNEL_PARAMETER(	uDppEnableRTDiscr,	"ENABLE_RT_DISCR",		"NO"				);
	CHANNEL_PARAMETER(	uDppTrgWinWidth,	"RT_DISCR_WINDOW",		"0"					);

	COMMON_PARAMETER(	uDualTrace,			"DUAL_TRACE",			"NO"				);
	COMMON_PARAMETER(	uTimeHistoMax,		"TIME_HISTO_MAX",		"0"					);
	COMMON_PARAMETER(	uDTCTimeInterval,	"DTC_TIME_INTERVAL",	"0"					);
	COMMON_PARAMETER(	uDTCEventInterval,	"DTC_EVENT_INTERVAL",	"0"					);
	COMMON_PARAMETER(	uEminSkim,			"EMIN_SKIM",			"0"					);
	COMMON_PARAMETER(	uEmaxSkim,			"EMAX_SKIM",			"0"					);
	COMMON_PARAMETER(	uEnableEnergySkim,	"ENABLE_ENERGY_SKIM",	"NO"				);
	CHANNEL_PARAMETER(	uStopTime,			"STOP_TIME",			"0"					);			

	COMMON_PARAMETER(	uGnuplotPath,		"GNUPLOT_PATH",			""					);

	COMMON_PARAMETER(	uVirtualProbeA1,	"VIRTUAL_PROBE_A1",		"0"					);
	COMMON_PARAMETER(	uVirtualProbeA2,	"VIRTUAL_PROBE_A2",		"0"					);	
	COMMON_PARAMETER(	uVirtualProbeD,		"VIRTUAL_PROBE_D",		"0"					);

	COMMON_PARAMETER(	uVirtualProbeA1En,	"PLOT_TRACE_1_ENABLED",	"YES"				);
	COMMON_PARAMETER(	uVirtualProbeA1Gain,"PLOT_TRACE_1_GAIN",	"1"					);
	COMMON_PARAMETER(	uVirtualProbeA1Off,	"PLOT_TRACE_1_OFFSET",	"0"					);
	COMMON_PARAMETER(	uVirtualProbeA2En,	"PLOT_TRACE_2_ENABLED",	"YES"				);	
	COMMON_PARAMETER(	uVirtualProbeA2Gain,"PLOT_TRACE_2_GAIN",	"1"					);
	COMMON_PARAMETER(	uVirtualProbeA2Off,	"PLOT_TRACE_2_OFFSET",	"0"					);
	COMMON_PARAMETER(	uVirtualProbeDEn,	"PLOT_TRACE_3_ENABLED",	"YES"				);	
	COMMON_PARAMETER(	uVirtualProbeDGain,	"PLOT_TRACE_3_GAIN",	"100"				);
	COMMON_PARAMETER(	uVirtualProbeDOff,	"PLOT_TRACE_3_OFFSET",	"200"				);
	COMMON_PARAMETER(	uVirtualProbeD2En,	"PLOT_TRACE_4_ENABLED",	"YES"				);	
	COMMON_PARAMETER(	uVirtualProbeD2Gain,"PLOT_TRACE_4_GAIN",	"100"				);
	COMMON_PARAMETER(	uVirtualProbeD2Off,	"PLOT_TRACE_4_OFFSET",	"200"				);

	CHANNEL_PARAMETER(	uCalibrationX1,		"CALIBRATION_X1",		"0"					);
	CHANNEL_PARAMETER(	uCalibrationX2,		"CALIBRATION_X2",		"0"					);
	CHANNEL_PARAMETER(	uCalibrationY1,		"CALIBRATION_Y1",		"0.0"				);
	CHANNEL_PARAMETER(	uCalibrationY2,		"CALIBRATION_Y2",		"0.0"				);

	COMMON_PARAMETER(	uDumpFileName,		"DUMPFILE",				"dump"				);
	COMMON_PARAMETER(	uDumpWaveforms,		"DUMP_WAVEFORMS",		"NO"				);
	COMMON_PARAMETER(	uDumpList,			"DUMP_LIST",			"NO"				);
	COMMON_PARAMETER(	uDumpEHisto,		"DUMP_EHISTO",			"NO"				);
	COMMON_PARAMETER(	uDumpTHisto,		"DUMP_THISTO",			"NO"				);
	
	COMMON_PARAMETER(	uPlotFps,			"PLOT_FPS",				"1.5"					);
)


#endif