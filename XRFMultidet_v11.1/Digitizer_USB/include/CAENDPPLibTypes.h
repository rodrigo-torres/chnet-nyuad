/******************************************************************************
* 
* CAEN SpA - Computing Division
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
*
* \file     CAENDigitizerType.h
* \brief    CAEN - Digitizer Library types definition
* \author   Francesco Pepe (support.computing@caen.it)
*
* This library provide functions, structures and definitions for the CAEN
* digitizer family with DPP frirmware
******************************************************************************/

#ifndef __CAENDPPLIBRARYTYPES_H
#define __CAENDPPLIBRARYTYPES_H

#include <stdint.h>

#ifdef WIN32
    #include <Windows.h>
    
    // define standard integers if not defined in 'stdint.h'
    #ifndef int8_t
        typedef INT8 int8_t;
    #endif
    #ifndef uint8_t
        typedef UINT8 uint8_t;
    #endif
    #ifndef int16_t
        typedef INT16 int16_t;
    #endif
    #ifndef uint16_t
        typedef UINT16 uint16_t;
    #endif
    #ifndef int32_t
        typedef INT32 int32_t;
    #endif
    #ifndef uint32_t
        typedef UINT32 uint32_t;
    #endif
    #ifndef int64_t
        typedef INT64 int64_t;
    #endif
    #ifndef uint64_t
        typedef UINT64 uint64_t;
    #endif

    #define CAENDPP_API __cdecl
#else
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>
    #define CAENDPP_API
#endif

#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

#define MAX_BRDNAME_LEN         (12)
#define MAX_FWVER_LENGTH        (20)

#ifndef MAX_LICENSE_DIGITS
#define MAX_LICENSE_DIGITS      (8)
#define MAX_LICENSE_LENGTH      (MAX_LICENSE_DIGITS * 2 + 1) // The maximum length of License is uint8_t[8];
                                                             // to plot it as an hex number on a string we need
                                                             // 2 chars for each uint8_t digit, so 8*2=16. With
                                                             // the trailing NULL char we need 17 chars.
#endif

#define MAX_LISTFILE_LENGTH     (155)
#define MAX_LIST_BUFF_NEV       (8192)
#define MAX_NUMB                (20)            // Max Number of boards theuser can connect
#define MAX_NUMCHB              (16)             // Max number of channels per board
#define MAX_NTHR                (MAX_NUMB)      // Max number of threads
#define MAX_ALTHR_NAME_LEN      (50)            // Max Acquisition Loop thread name
#define MAX_NUMCHB_COINCIDENCE  (MAX_NUMCHB+1)  // Max number of channels for coincidences (add exteral channel)
#define MAX_GW                  (1000)          // Max number of generic write register in the Config File
#define DEFAULT_HISTO_NUM       (1)             // Default number of histograms allocated when adding a board
#define DEFAULT_HISTO_SIZE      (16384)         // Default number of bins used to allocate histograms
#define CHANNEL_IDX_ALL         (-1)            // Channel Index to Act an All Channels
#define HISTO_IDX_CURRENT       (-1)            // Histogram Index to Act on Current Channel Histogram
#define HISTO_IDX_ALL           (-2)            // Histogram Index to Act on All Channel Histograms
#define X770_RECORDLENGTH       (2048)          // Fixed RecordLength of X770 boards
#define MAX_INRANGES            (4)             // Maximum number of Input Ranges
#define MAX_PROBES_NUM          (20)            // Maximum Number of supported probes
#define IP_ADDR_LEN             (255)

/***********************************************************
*                  HV Related definitions                  *
***********************************************************/

#define MAX_HVSTATUS_LENGTH     (100)           // Maximum Length for the HV Status String (chars)

// VSet
#define HV_VSET_MAX         5100.0
#define HV_VSET_MIN         0.0
#define HV_VSET_RES         10.0

// ISet
#define HV_ISET_MAX         310.0
#define HV_ISET_MIN         0.0
#define HV_ISET_RES         100.0

// RampUp
#define HV_RAMPUP_MAX       500.0
#define HV_RAMPUP_MIN       1.0
#define HV_RAMPUP_RES       1.0

// RampDown
#define HV_RAMPDOWN_MAX     500.0
#define HV_RAMPDOWN_MIN     1.0
#define HV_RAMPDOWN_RES     1.0

// VMax
#define HV_VMAX_MAX         5100.0
#define HV_VMAX_MIN         0.0
#define HV_VMAX_RES         0.05

// VMon
#define HV_VMON_RES         10.0

// IMon
#define HV_IMON_RES         100.0

// Externals
#define HV_VEXT_RES         1000.0
#define HV_RTMP_RES         10.0

// Dump masks
#define DUMP_MASK_TTT           (0x1)
#define DUMP_MASK_ENERGY        (0x2)
#define DUMP_MASK_EXTRAS        (0x4)
#define DUMP_MASK_ENERGYSHORT   (0x8)

/***********************************************************
*                  enum types definition                   //
***********************************************************/
typedef enum {
    CAENDPP_MultiHisto_SoftwareOnly     = 1L,    
    CAENDPP_MultiHisto_TimeStampReset   = 2L,
} CAENDPP_MultiHistoCondition_t;

typedef enum {
    CAENDPP_RetCode_Ok                  = 0,        // Success
    CAENDPP_RetCode_GenericError        = -100L,    // Generic Error
    CAENDPP_RetCode_TooManyInstances    = -101L,    // Too Many Instances
    CAENDPP_RetCode_ProcessFail         = -102L,    // Process Fail
    CAENDPP_RetCode_ReadFail            = -103L,    // Read Fail
    CAENDPP_RetCode_WriteFail           = -104L,    // Write Fail
    CAENDPP_RetCode_BadMessage          = -105L,    // Invalid Response
    CAENDPP_RetCode_InvalidHandle       = -106L,    // Invalid library handle
    CAENDPP_RetCode_ConfigError         = -107L,    // Configuration Error
    CAENDPP_RetCode_BoardInitFail       = -108L,    // Board Init Failed
    CAENDPP_RetCode_TimeoutError        = -109L,    // Timeout Error
    CAENDPP_RetCode_InvalidParameter    = -110L,    // Invalid Parameter
    CAENDPP_RetCode_NotInWaveMode       = -111L,    // Not in Waveforms Mode
    CAENDPP_RetCode_NotInHistoMode      = -112L,    // Not in Histogram Mode
    CAENDPP_RetCode_NotInListMode       = -113L,    // Not in List Mode
    CAENDPP_RetCode_NotYetImplemented   = -114L,    // Not Yet Implemented
    CAENDPP_RetCode_BoardNotConfigured  = -115L,    // Board Not Configured
    CAENDPP_RetCode_InvalidBoardIndex   = -116L,    // Invalid board index
    CAENDPP_RetCode_InvalidChannelIndex = -117L,    // Invalid channel index
    CAENDPP_RetCode_UnsupportedFirmware = -118L,    // Invalid board firmware
    CAENDPP_RetCode_NoBoardsAdded       = -119L,    // No board added
    CAENDPP_RetCode_AcquisitionRunning  = -120L,    // Acquisition Status is not compliant with the function called.
    CAENDPP_RetCode_OutOfMemory         = -121L,    // OutOfMemory
    CAENDPP_RetCode_BoardChannelIndex   = -122L,    // Invalid board channel index
    CAENDPP_RetCode_HistoAlloc          = -123L,    // No valid histogram allocated
    CAENDPP_RetCode_OpenDumper          = -124L,    // Error Opening the List Dumper
    CAENDPP_RetCode_BoardStart          = -125L,    // Error Starting Acquisition for a Board
    CAENDPP_RetCode_ChannelEnable       = -126L,    // The given channel is not enabled
    CAENDPP_RetCode_InvalidCommand      = -127L,    // Invalid Command
    CAENDPP_RetCode_NumBins             = -128L,    // Invalid Number of Bins
    CAENDPP_RetCode_HistoIndex          = -129L,    // Invalid Hitogram Index
    CAENDPP_RetCode_UnsupportedFeature  = -130L,    // The feature is not supported by the gve board/channel
    CAENDPP_RetCode_BadHistoState       = -131L,    // The given histogram is an invalid state (ex. 'done' while it shouldn't)
    CAENDPP_RetCode_NoMoreHistograms    = -132L,    // Cannot switch to ext histo, no more histograms available.
    CAENDPP_RetCode_NotHVBoard          = -133L,    // The selected board doesn't support HV Channels
    CAENDPP_RetCode_InvalidHVChannel    = -134L,    // Invalid HV channel index
    CAENDPP_RetCode_SocketSend          = -135L,    // Error Sending Message through Socket
    CAENDPP_RetCode_SocketReceive       = -136L,    // Error Receiving Message from Socket
    CAENDPP_RetCode_BoardThread         = -137L,    // Cannot get Board's acquisition thread
    CAENDPP_RetCode_DecodeWaveform      = -138L,    // Cannot decode waveform from buffer
    CAENDPP_RetCode_OpenDigitizer       = -139L,    // Error Opening the digitizer
    CAENDPP_RetCode_BoardModel          = -140L,    // Requested a feature incompatible with board's Manufacture
    CAENDPP_RetCode_AutosetStatus       = -141L,    // Autoset Status is not compliant with the requested feature
    CAENDPP_RetCode_Autoset             = -142L,    // Autoset error looking for signal parameters
} CAENDPP_RetCode_t;

typedef enum {
    CAENDPP_AcqMode_Waveform    = 0L,
    CAENDPP_AcqMode_Histogram   = 1L,
} CAENDPP_AcqMode_t;

typedef enum {
    CAENDPP_USB                 = 0L,
    CAENDPP_PCI_OpticalLink     = 1L,
    CAENDPP_ETH                 = 2L,
    CAENDPP_Serial              = 3L,
} CAENDPP_ConnectionType;

/*! 
 * \brief Defines the digitizer model
 */
typedef enum
{
    CAENDPP_V1724     = 0L,    // !< \brief The board is V1724>
    CAENDPP_DT5724    = 6L,    // !< \brief The board is DT5724>
    CAENDPP_N6724     = 12L,   // !< \brief The board is N6724>
    CAENDPP_DT5780    = 21L,   // !< \brief The board is DT5780>
    CAENDPP_N6780     = 22L,   // !< \brief The board is N6780>
    CAENDPP_V1780     = 23L,   // !< \brief The board is V1780>
    CAENDPP_DT5730    = 30L,   // !< \brief The board is DT5730>
    CAENDPP_N6730     = 31L,   // !< \brief The board is N6730>
    CAENDPP_V1730     = 32L,   // !< \brief The board is V1730>
    CAENDPP_DT5781    = 36L,   // !< \brief The board is DT5781>
    CAENDPP_N6781     = 37L,   // !< \brief The board is N6781>
    CAENDPP_V1781     = 38L,   // !< \brief The board is V1781>
    CAENDPP_DT5770    = -1L,   // !< \brief The board is DT5770>
    CAENDPP_N6770     = -2L,   // !< \brief The board is N6770>
    CAENDPP_V1770     = -3L,   // !< \brief The board is V1770>
} CAENDPP_BoardModel_t;

/*! 
 * \brief Defines the digitizer family
 */
typedef enum {
    CAENDPP_XX724_FAMILY_CODE  = 0L,
    CAENDPP_XX780_FAMILY_CODE  = 7L,
    CAENDPP_XX730_FAMILY_CODE  = 11L,
    CAENDPP_XX781_FAMILY_CODE  = 13L,
    CAENDPP_XX770_FAMILY_CODE  = -1L,
} CAENDPP_BoardFamilyCode_t;

/*! 
 * \brief Defines the digital signals that can be carried by the digital probe 1
 *        in the readout data of the DPP-PHA
 */
typedef enum {
    CAENDPP_PHA_DigitalProbe1_TrgWin      = 0L,
    CAENDPP_PHA_DigitalProbe1_Armed       = 1L,
    CAENDPP_PHA_DigitalProbe1_PkRun       = 2L,
    CAENDPP_PHA_DigitalProbe1_PURFlag     = 3L,
    CAENDPP_PHA_DigitalProbe1_Peaking     = 4L,
    CAENDPP_PHA_DigitalProbe1_TVAW        = 5L,
    CAENDPP_PHA_DigitalProbe1_BLHoldoff   = 6L,
    CAENDPP_PHA_DigitalProbe1_TRGHoldoff  = 7L,
    CAENDPP_PHA_DigitalProbe1_TRGVal      = 8L,
    CAENDPP_PHA_DigitalProbe1_ACQVeto     = 9L,
    CAENDPP_PHA_DigitalProbe1_BFMVeto     = 10L,
    CAENDPP_PHA_DigitalProbe1_ExtTRG      = 11L,
    CAENDPP_PHA_DigitalProbe1_Trigger     = 12L,
} CAENDPP_PHA_DigitalProbe1_t;

/*! 
 * \brief Defines the digital signals that can be carried by the digital probe 1
 *        in the readout data of the DPP-PHA
 */
typedef enum {
    CAENDPP_PHA_DigitalProbe2_Trigger   = 0L,
} CAENDPP_PHA_DigitalProbe2_t;

/*! 
 * \brief Defines the signals that can be carried by the virtual analog probe 1
 *        in the readout data of the DPP-PHA
 */
typedef enum {
    CAENDPP_PHA_VIRTUALPROBE1_Input             = 0L, // BOTH X724 AND X770
    CAENDPP_PHA_VIRTUALPROBE1_Delta             = 1L, // BOTH X724 AND X770
    CAENDPP_PHA_VIRTUALPROBE1_Delta2            = 2L, // BOTH X724 AND X770
    CAENDPP_PHA_VIRTUALPROBE1_Trapezoid         = 3L, // BOTH X724 AND X770
    CAENDPP_PHA_VIRTUALPROBE1_FastTrap          = 4L, // X770 ONLY
    CAENDPP_PHA_VIRTUALPROBE1_TrapBaseline      = 5L, // X770 ONLY
    CAENDPP_PHA_VIRTUALPROBE1_EnergyOut         = 6L, // X770 ONLY
    CAENDPP_PHA_VIRTUALPROBE1_TrapBLCorr        = 7L, // X770 ONLY
    //CAENDPP_PHA_VIRTUALPROBE1_Deconvolved       = 8L, // X770 ONLY
    //CAENDPP_PHA_VIRTUALPROBE1_Dev2FastTrap      = 9L, // X770 ONLY
} CAENDPP_PHA_VirtualProbe1_t;

/*! 
 * \brief Defines the signals that can be carried by the virtual analog probe 2
 *        in the readout data of the DPP-PHA
 */
typedef enum {
    CAENDPP_PHA_VIRTUALPROBE2_Input             = 0L, // BOTH X724 AND X770
    CAENDPP_PHA_VIRTUALPROBE2_S3                = 1L, // X724 ONLY
    CAENDPP_PHA_VIRTUALPROBE2_TrapBLCorr        = 2L, // BOTH X724 AND X770
    CAENDPP_PHA_VIRTUALPROBE2_TrapBaseline      = 3L, // BOTH X724 AND X770
    CAENDPP_PHA_VIRTUALPROBE2_None              = 4L, // X724 ONLY
    CAENDPP_PHA_VIRTUALPROBE2_Delta             = 5L, // X770 ONLY
    CAENDPP_PHA_VIRTUALPROBE2_FastTrap          = 6L, // X770 ONLY
    CAENDPP_PHA_VIRTUALPROBE2_Delta2            = 7L, // X770 ONLY
    CAENDPP_PHA_VIRTUALPROBE2_Trapezoid         = 8L, // X770 ONLY
    CAENDPP_PHA_VIRTUALPROBE2_EnergyOut         = 9L, // X770 ONLY
} CAENDPP_PHA_VirtualProbe2_t;

/*! 
 * \brief Defines the signals that can be used as a trigger for the X770 digital probes
 *        in the readout data of the DPP-PHA
 */
typedef enum {
    CAENDPP_PHA_PROBETRIGGER_MainTrig                       = 0L,
    CAENDPP_PHA_PROBETRIGGER_MainTriggerDelayCompensated    = 1L,
    CAENDPP_PHA_PROBETRIGGER_MainTriggerAccepedPulse        = 2L,
    CAENDPP_PHA_PROBETRIGGER_SelfTriggerMUX1                = 3L,
    CAENDPP_PHA_PROBETRIGGER_SelfTriggerMUX2                = 4L,
    CAENDPP_PHA_PROBETRIGGER_BaselineRestorer               = 5L,
    CAENDPP_PHA_PROBETRIGGER_ResetDetector                    = 6L,
    CAENDPP_PHA_PROBETRIGGER_FreeRunning                    = 7L,
} CAENDPP_PHA_ProbeTrigger_t;

typedef enum {
    CAENDPP_PulsePolarityPositive        = 0L,
    CAENDPP_PulsePolarityNegative        = 1L,
} CAENDPP_PulsePolarity_t;

typedef enum {
    CAENDPP_IOLevel_NIM        = 0L,
    CAENDPP_IOLevel_TTL        = 1L,
} CAENDPP_IOLevel_t;

typedef enum {
    CAENDPP_AnalogProbeFirst      = 0L,    
    CAENDPP_AnalogProbeSecond     = 1L,
    CAENDPP_DigitalProbeFirst     = 2L,    
    CAENDPP_DigitalProbeSecond    = 3L,
} CAENDPP_ProbeNumber_t;

typedef enum {
    CAENDPP_ListSaveMode_Memory     = 0L, // Keep the list events in a memory buffer of maximum size = MAX_LIST_BUFF_NEV
    CAENDPP_ListSaveMode_FileBinary = 1L, // Save list events in a binary file.
    CAENDPP_ListSaveMode_FileASCII  = 2L, // Save list events in a ASCII file.
} CAENDPP_ListSaveMode_t;

typedef enum {
    CAENDPP_PWDownMode_Ramp = 0L,
    CAENDPP_PWDownMode_Kill = 1L,
} CAENDPP_PWDownMode_t;

typedef enum {
    CAENDPP_AcqStatus_Stopped = 0L,
    CAENDPP_AcqStatus_Running = 1L,
    CAENDPP_AcqStatus_Armed   = 2L,
    CAENDPP_AcqStatus_Unknown = 3L,
} CAENDPP_AcqStatus_t;

typedef enum {
    CAENDPP_StopCriteria_Manual = 0L,
    CAENDPP_StopCriteria_LiveTime = 1L,
    CAENDPP_StopCriteria_RealTime = 2L,
    CAENDPP_StopCriteria_Counts = 3L,
} CAENDPP_StopCriteria_t;

typedef enum {
    CAENDPP_CoincOp_OR  = 0L,
    CAENDPP_CoincOp_AND = 1L,
    CAENDPP_CoincOp_MAJ = 2L,
} CAENDPP_CoincOp_t;

typedef enum {
    CAENDPP_CoincLogic_None             = 0L,
    CAENDPP_CoincLogic_Coincidence      = 2L,
    CAENDPP_CoincLogic_Anticoincidence  = 3L,
} CAENDPP_CoincLogic_t;

typedef enum {
    CAENDPP_InputRange_9_5Vpp   = 0L, // X780
    CAENDPP_InputRange_3_7Vpp   = 1L, // X780
    CAENDPP_InputRange_1_4Vpp   = 2L, // X780
    CAENDPP_InputRange_0_6Vpp   = 3L, // X780
    CAENDPP_InputRange_3_0Vpp   = 4L, // X781
    CAENDPP_InputRange_1_0Vpp   = 5L, // X781
    CAENDPP_InputRange_0_3Vpp   = 6L, // X781
    CAENDPP_InputRange_10_0Vpp  = 7L, // X781 / X770
    CAENDPP_InputRange_5_0Vpp   = 8L, // X770
    CAENDPP_InputRange_2_0Vpp   = 9L, // X724 / X730
    CAENDPP_InputRange_0_5Vpp   = 10L, // X730
    CAENDPP_InputRange_UNKN     = 99L, 
} CAENDPP_InputRange_t;

typedef enum {
    CAENDPP_SpectrumMode_Energy     = 0L,
    CAENDPP_SpectrumMode_Time        = 1L
} CAENDPP_SpectrumMode_t;

typedef enum {
    CAENDPP_GuessConfigStatus_NotRunning = 0L,
    CAENDPP_GuessConfigStatus_Started = 1L,
    CAENDPP_GuessConfigStatus_PulsePolarity = 2L,
    CAENDPP_GuessConfigStatus_DCOffset = 3L,
    CAENDPP_GuessConfigStatus_SignalRise = 4L,
    CAENDPP_GuessConfigStatus_Threshold =  5L,
    CAENDPP_GuessConfigStatus_DecayTime = 6L,
    CAENDPP_GuessConfigStatus_Trapezoid = 7L,
    CAENDPP_GuessConfigStatus_Baseline = 8L,
    CAENDPP_GuessConfigStatus_Ready = 9L
} CAENDPP_GuessConfigStatus_t;

typedef enum {
	CAENDPP_COMStatus_Online        = 0L,
    CAENDPP_COMStatus_NotAvailable  = 1L,
	CAENDPP_COMStatus_Error         = 2L,
	CAENDPP_COMStatus_BootLoader    = 3L
} CAENDPP_COMStatus_t;

typedef enum {
    CAENDPP_ParamID_RecordLength = 0L,
    CAENDPP_ParamID_PreTrigger = 1L,
    CAENDPP_ParamID_Decay = 2L,
    CAENDPP_ParamID_TrapRise = 3L,
    CAENDPP_ParamID_TrapFlat = 4L,
    CAENDPP_ParamID_TrapFlatDelay = 5L,
    CAENDPP_ParamID_Smoothing = 6L,
    CAENDPP_ParamID_InputRise = 7L,
    CAENDPP_ParamID_Threshold = 8L,
    CAENDPP_ParamID_NSBL = 9L,
    CAENDPP_ParamID_NSPK = 10L,
    CAENDPP_ParamID_PKHO = 11L,
    CAENDPP_ParamID_BLHO = 12L,
    CAENDPP_ParamID_TRGHO = 13L,
    CAENDPP_ParamID_DGain = 14L,
    CAENDPP_ParamID_ENF = 15L,
    CAENDPP_ParamID_Decimation = 16L,
    CAENDPP_ParamID_TWWDT = 17L,
    CAENDPP_ParamID_TRGWin = 18L,
    CAENDPP_ParamID_PulsePol = 19L,
    CAENDPP_ParamID_DCOffset = 20L,
    CAENDPP_ParamID_IOLev = 21L,
} CAENDPP_ParamID_t;

typedef enum {
    CAENDPP_Units_NanoSeconds = 0L,
    CAENDPP_Units_Samples = 1L,
    CAENDPP_Units_Adimensional = 2L,
} CAENDPP_Units_t;

/***********************************************************
*                  struct types definition                 //
***********************************************************/

typedef struct {
    double decK2; // Deconvolutor k2
    double decK3; // Deconvolutor k3
    int32_t deconvolutormode;
    int32_t trigK; // trigger fast trapezoid rising time
    int32_t trigm; // trigger fast trapezoid flat top
    int32_t trigMODE; // 0 threshold on fast trapeziodal
                      // 1 threshold on filtered second deriv of fast trapeziodal
    int32_t energyFilterMode; // 0 trapezoidal
                              // 1 peak detector
    int32_t PD_thrshld1; // peak detector arm threshold 
    int32_t PD_thrshld2; // peak detector disarm threshold 
    int32_t PD_winlen; // peak detector inspection window length
} CAENDPP_ExtraParameters;

/*! 
 * \brief DPP parameter structure to be initialized and passed to CAENDPP_SetBoardConfiguration
 * \note1 Only for DPP-PHA
 */
typedef struct {
    int32_t M           [MAX_NUMCHB]; // Signal Decay Time Constant
    int32_t m           [MAX_NUMCHB]; // Trapezoid Flat Top
    int32_t k           [MAX_NUMCHB]; // Trapezoid Rise Time
    int32_t ftd         [MAX_NUMCHB]; // Trapezoid Peaking Delay
    int32_t a           [MAX_NUMCHB]; // Trigger Filter smoothing factor
    int32_t b           [MAX_NUMCHB]; // Input Signal Rise time
    int32_t thr         [MAX_NUMCHB]; // Trigger Threshold
    int32_t nsbl        [MAX_NUMCHB]; // Number of Samples for Baseline Mean
    int32_t nspk        [MAX_NUMCHB]; // Number of Samples for Peak Mean Calculation
    int32_t pkho        [MAX_NUMCHB]; // Peak Hold Off
    int32_t blho        [MAX_NUMCHB]; // Base Line Hold Off
    int32_t trgho       [MAX_NUMCHB]; // Trigger Hold Off
    int32_t dgain       [MAX_NUMCHB]; // Digital Probe Gain
    float   enf         [MAX_NUMCHB]; // Energy Nomralization Factor
    int32_t decimation  [MAX_NUMCHB]; // Decimation of Input Signal
    int32_t twwdt       [MAX_NUMCHB]; // 
    int32_t trgwin      [MAX_NUMCHB]; // 
    CAENDPP_ExtraParameters X770_extraparameters[MAX_NUMCHB];    //parameters for X770 products only 
} CAENDPP_PHA_Params_t;

// Connection Config Parameters
typedef struct {
    CAENDPP_ConnectionType LinkType;
    int32_t LinkNum;
    int32_t ConetNode;
    uint32_t VMEBaseAddress;
    char ETHAddress[IP_ADDR_LEN + 1];
} CAENDPP_ConnectionParams_t;

// Waveform mode config parameters
typedef struct {
    int32_t dualTraceMode; // if true dual trace is enabled
    CAENDPP_PHA_VirtualProbe1_t vp1; // First Analog Probe
    CAENDPP_PHA_VirtualProbe2_t vp2; // Second Analog Probe, ignored if dualTraceMode=false
    CAENDPP_PHA_DigitalProbe1_t dp1; // First Digital probe
    CAENDPP_PHA_DigitalProbe2_t dp2; // Second Digital probe

    int32_t recordLength;
    int32_t preTrigger;

    // Only for X770
    CAENDPP_PHA_ProbeTrigger_t probeTrigger;
    int32_t probeSelfTriggerVal;
} CAENDPP_WaveformParams_t;

// List mode config parameters
typedef struct {
    uint8_t                 enabled; // 1 = ListMode Enabled, 0 = ListMode Disabled
    CAENDPP_ListSaveMode_t  saveMode; // see CAENDPP_ListSaveMode_t
    char                    fileName[MAX_LISTFILE_LENGTH]; // the filename used for binary writing (see CAENDPP_ListSaveMode_t)
    uint32_t                maxBuffNumEvents; // the maximum number of events to keep in the buffer if in memory mode (see CAENDPP_ListSaveMode_t)
    uint32_t                saveMask; // The mask of the object to be dumped as defined from 'DUMP_MASK_*' macros.
} CAENDPP_ListParams_t;

// List mode event structure
typedef struct {
    uint64_t TimeTag;
    uint16_t Energy;
    uint32_t Extras;
} CAENDPP_ListEvent_t;

// Coincidence parameters
typedef struct {
    uint32_t                CoincChMask;
    uint32_t                MajLevel;
    uint32_t                TrgWin;
    CAENDPP_CoincOp_t       CoincOp;
    CAENDPP_CoincLogic_t    CoincLogic;
} CAENDPP_CoincParams_t;

// Only for X770
typedef struct {
    CAENDPP_SpectrumMode_t          SpectrumMode;       // 0 Energy
                                                        // 1 Time distribution
    uint32_t                        TimeScale;          // Scale in time distribution
} CAENDPP_SpectrumControl;

// Only for X770
typedef struct {
    uint8_t                EnableResetDetector; // Enable the detection circuit for the reset
    uint32_t               thrhold;             // Reset negative threshold
    uint32_t               reslenmin;           // Minimum length of the reset spike to trigger the reset inhibit
    uint32_t               reslenpulse;         // Inhibit length
} CAENDPP_TRReset;

// Only for X770
typedef struct {
    uint32_t analogPath;        // 0 Continuos Reset
                                // 1 Pulsed reset
                                // 2 Direct detector sampling
    
    uint32_t InputImpedance;    // 0 50R
                                // 1 10K

    uint32_t CRgain;            // 0 1
                                // 1 1/2
                                // 2 1/4
    uint32_t PRDSgain;          // Analog gain of the Pulsed Reset / Direct Sampling
    
    uint32_t SaturationHoldoff;    // Saturation detector holdoff

    CAENDPP_TRReset ResetDetector; // Transient Reset Detector
} CAENDPP_ChannelExtraParameters;

// parameters for a single digitizer
typedef struct {
    // --------------------
    // Board Settings
    // --------------------

    // Generic Write
    int32_t GWn;
    uint32_t GWaddr[MAX_GW];
    uint32_t GWdata[MAX_GW];
    uint32_t GWmask[MAX_GW];

    // Channel settings
    int32_t ChannelMask;
    CAENDPP_PulsePolarity_t PulsePolarity[MAX_NUMCHB];
    int32_t DCoffset[MAX_NUMCHB];
    CAENDPP_ChannelExtraParameters ChannelExtraParameters[MAX_NUMCHB]; // Only for X770
    
    int32_t EventAggr;
    CAENDPP_PHA_Params_t DPPParams;
    CAENDPP_IOLevel_t IOlev;

    // Waveform Mode Settings, they only affect waveforms acquisition mode
    CAENDPP_WaveformParams_t WFParams;
    
    // List Mode Settings
    CAENDPP_ListParams_t ListParams;

    // Parameters for coincidence mode
    CAENDPP_CoincParams_t CoincParams[MAX_NUMCHB_COINCIDENCE];

    //Spectrum Control setting (Only for X770)
    CAENDPP_SpectrumControl    SpectrumControl[MAX_NUMCHB];
} CAENDPP_DgtzParams_t;

typedef struct {
    double VSet;
    double ISet;
    double RampUp;
    double RampDown;
    double VMax;
    CAENDPP_PWDownMode_t PWDownMode;
} CAENDPP_HVChannelConfig_t;

typedef struct {
    char        ModelName[MAX_BRDNAME_LEN];
    int32_t    Model;
    uint32_t    Channels;
    char        ROC_FirmwareRel[MAX_FWVER_LENGTH];
    char        AMC_FirmwareRel[MAX_FWVER_LENGTH];
    char        License[MAX_LICENSE_LENGTH];
    uint32_t    SerialNumber;
    uint8_t     Status;
    int32_t     FamilyCode;
    uint32_t    HVChannels;
	uint32_t    FormFactor;
	uint32_t    PCB_Revision;
	uint32_t    ADC_NBits;
	uint32_t    USBOption;
	uint32_t    ETHOption;
	uint32_t    WIFIOption;
	uint32_t    BTOption;
	uint32_t    POEOption;
    uint32_t    InputRangeNum;
    CAENDPP_InputRange_t InputRanges[MAX_INRANGES];
    double      Tsample;
    uint32_t    SupportedVirtualProbes1[MAX_PROBES_NUM];
    uint32_t    NumVirtualProbes1;
    uint32_t    SupportedVirtualProbes2[MAX_PROBES_NUM];
    uint32_t    NumVirtualProbes2;
    uint32_t    SupportedDigitalProbes1[MAX_PROBES_NUM];
    uint32_t    NumDigitalProbes1;
    uint32_t    SupportedDigitalProbes2[MAX_PROBES_NUM];
    uint32_t    NumDigitalProbes2;
    int32_t     DPPCodeMaj;
} CAENDPP_Info_t;

typedef struct {
    double ThroughputRate;      // data throughput rate in MB/s
    uint32_t SaturationFlag;    // Flag set to 1 if there were saturations from the last call
    double SaturationPerc;      // Total percentage of saturations
} statistics_t;

typedef struct {
	uint32_t id;
	CAENDPP_ConnectionType ConnectionMode;
	char SerialNUMBER[128];
	char ProductDescription[256];
	char ETHAddress[256];
	uint16_t TCPPORT;
	uint16_t UDPPORT;
	CAENDPP_COMStatus_t cStatus;
} CAENDPP_EnumerationSingleDevice_t;

typedef struct {
	int ddcount;
	CAENDPP_EnumerationSingleDevice_t Device[64];
} CAENDPP_EnumeratedDevices_t;

typedef struct {
    double minimum;
    double maximum;
    double resolution;
    CAENDPP_Units_t units;
} CAENDPP_ParamInfo_t;

#endif