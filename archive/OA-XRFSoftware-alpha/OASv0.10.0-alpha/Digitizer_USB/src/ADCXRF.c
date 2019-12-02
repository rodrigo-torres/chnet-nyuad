#include <stdlib.h> //in mancanza di questo spara una fila di warning...sys/time.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include </usr/include/sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <termio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/file.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/mman.h>
#include <math.h>
#include <sys/ioctl.h>
#include <CAENDPPLib.h>
#include <CAENDPPLibTypes.h>
#include <CAENDigitizer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


//#define INDIVIDUAL_TRIGGER_INPUTS
// The following define must be set to the actual number of connected boards
#define MAXNB   1
// NB: the following define MUST specify the ACTUAL max allowed number of board's channels
// it is needed for consistency inside the CAENDigitizer's functions used to allocate the memory
#define MaxNChannels 2

#define MAXNBITS 14

#include "Functions.h"


#include <../../Shm.h>
#include <../../variables.h>

extern int *shm_cmd, *shmCommand_cmd, shmid_cmd, shmidString_cmd, *shared_memory_cmd; 
extern key_t key_cmd; extern key_t keyString_cmd;

extern int *shm, *shmCommand, shmid, shmidString, *shared_memory; 
extern key_t key; extern key_t keyString;

extern int *shm2, *shmCommand2, shmid2, shmidString2, *shared_memory2; 
extern key_t key2; extern key_t keyString2;

extern int *shm3, *shmCommand3, shmid3, shmidString3, *shared_memory3;
extern key_t key3; extern key_t keyString3;

extern int *shm4, *shmCommand4, shmid4, shmidString4, *shared_memory4;
extern key_t key4; extern key_t keyString4;

extern int *shm_rate, *shm_rate, shmid_rate, shmidString_rate, *shared_memory_rate; 
extern key_t key_rate; extern key_t keyString_rate;

int NSegFault=2000000000;  ///61440000 dovrebbe essere il massimo assoluto


//////////////////////////////////////////
void termination(int);
struct sigaction sigact;
void sigquit();
/////////////////////////////////////////
 int ev=0;
int n=1;int k=0;
FILE *fd,*fds;
int storage[16384]={0};
char *addr_vme = 0;
int vme_handle;
int backup;
int resto;
int w;
int eventi=0;double event_rate=0; int stored_event=0;
int32_t handledpp;
int retdpp;
int32_t bId[MAXNB];
int b, ch;
int handle_global[MAXNB];
uint32_t *EHisto[MAXNB][MaxNChannels]; // Energy Histograms 
char *buffer = NULL; 
CAEN_DGTZ_DPP_PHA_Event_t       *Events[MaxNChannels];  // events buffer
CAEN_DGTZ_DPP_PHA_Waveforms_t   *Waveform=NULL;     // waveforms buffer

//////////////variables for the scanning///////////////////
int Xmin_adc=0; int Xmax_adc=0;
int Ymin_adc=0; int Ymax_adc=0;
int Xstep_adc=0; int Ystep_adc=0;
int velocity_adc=0;
int PositionX_adc=0; int PositionY_adc=0;
int x=0; int Xn=0; int y=0; int Yn=0;
int check=1;

////////////definitions for the timer/////////////

struct itimerval it_val;
double time_seconds=0;
int time_interval; //frequency to which the positions are written in memory
void write_position(); //this function sets a variable to true each time a position needs to be written in memory
bool write_coordinates=false; //write=true if the position must be written; write=false if it needs to wait
int changeposition=0; //a number that is incremented each time a position is writen, it is used to calculate the positions


double millisec;
struct timeval tv;

/* ########################################################################### */
/*   Program the registers of the digitizer with the relevant parameters
/*   //return  0=success; -1=error                                                                  
/* ########################################################################### */

/* --------------------------------------------------------------------------------------------------------- */

int ProgramDigitizer(int handle, DigitizerParams_t Params, CAEN_DGTZ_DPP_PHA_Params_t DPPParams)
{


    /* This function uses the CAENDigitizer API functions to perform the digitizer's initial configuration */
    int i, ret = 0;

    /* Reset the digitizer */
    ret |= CAEN_DGTZ_Reset(handle);

    if (ret) {
        printf("ERROR: can't reset the digitizer.\n");
        return -1;
    }
    ret |= CAEN_DGTZ_WriteRegister(handle, 0x8000, 0x010E0114);  // Channel Control Reg (indiv trg, seq readout) ??

    /* Set the DPP acquisition mode
    This setting affects the modes Mixed and List (see CAEN_DGTZ_DPP_AcqMode_t definition for details)
    CAEN_DGTZ_DPP_SAVE_PARAM_EnergyOnly        Only energy (DPP-PHA) or charge (DPP-PSD/DPP-CI v2) is returned
    CAEN_DGTZ_DPP_SAVE_PARAM_TimeOnly        Only time is returned
    CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime    Both energy/charge and time are returned
    CAEN_DGTZ_DPP_SAVE_PARAM_None            No histogram data is returned */
    ret |= CAEN_DGTZ_SetDPPAcquisitionMode(handle, Params.AcqMode, CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime);
    
    // Set the digitizer acquisition mode (CAEN_DGTZ_SW_CONTROLLED or CAEN_DGTZ_S_IN_CONTROLLED)
    ret |= CAEN_DGTZ_SetAcquisitionMode(handle, CAEN_DGTZ_SW_CONTROLLED);
    
    // Set the number of samples for each waveform
    ret |= CAEN_DGTZ_SetRecordLength(handle, Params.RecordLength);

    // Set the I/O level (CAEN_DGTZ_IOLevel_NIM or CAEN_DGTZ_IOLevel_TTL)
    ret |= CAEN_DGTZ_SetIOLevel(handle, Params.IOlev);

    /* Set the digitizer's behaviour when an external trigger arrives:

    CAEN_DGTZ_TRGMODE_DISABLED: do nothing
    CAEN_DGTZ_TRGMODE_EXTOUT_ONLY: generate the Trigger Output signal
    CAEN_DGTZ_TRGMODE_ACQ_ONLY = generate acquisition trigger
    CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT = generate both Trigger Output and acquisition trigger

    see CAENDigitizer user manual, chapter "Trigger configuration" for details */
    ret |= CAEN_DGTZ_SetExtTriggerInputMode(handle, CAEN_DGTZ_TRGMODE_ACQ_ONLY);

    // Set the enabled channels
    ret |= CAEN_DGTZ_SetChannelEnableMask(handle, Params.ChannelMask);

    // Set how many events to accumulate in the board memory before being available for readout
    ret |= CAEN_DGTZ_SetDPPEventAggregation(handle, Params.EventAggr, 0);
    
    /* Set the mode used to syncronize the acquisition between different boards.
    In this example the sync is disabled */
    ret |= CAEN_DGTZ_SetRunSynchronizationMode(handle, CAEN_DGTZ_RUN_SYNC_Disabled);
    
    // Set the DPP specific parameters for the channels in the given channelMask
    ret |= CAEN_DGTZ_SetDPPParameters(handle, Params.ChannelMask, &DPPParams);
    
    for(i=0; i<MaxNChannels; i++) {
        if (Params.ChannelMask & (1<<i)) {
            // Set a DC offset to the input signal to adapt it to digitizer's dynamic range
	    ret |= CAEN_DGTZ_SetChannelDCOffset(handle, i, 0xB333); //for emulator and for XR100_medium CH0Dt1
	//	 ret |= CAEN_DGTZ_SetChannelDCOffset(handle, i, 0xB70A); //for detector opd
            //ret |= CAEN_DGTZ_SetChannelDCOffset(handle, i, 0xEB85);   //era 0x8000 cioè la metà di 0xFFFF (l'offset DC è in 16 bit)
                                                                      ///il nostro 35% sarebbe 22938 su 65536----->0x599A   0x7333
                                                                      ////0xF333 è il 95%....NULLA!!!
                                                                       ////0xEB85 è il 92%...viene histo da 250 
                                                                      ////0xE666 è il 90%....viene histo da 600
                                                                      ///0xCCCC è l'80% .....viene histo da 3300
                                                                      ///0xB333 è il 70% ....viene histo da 5000
                                                                      ///0x7333 è il 45%.....viene histo da 8350
                                                                      ////0x599A è il 35%....viene histo da 10050
                                                                      ////0x1999 è il 10%....viene histo da 14320 
            // Set the Pre-Trigger size (in samples)
            ret |= CAEN_DGTZ_SetDPPPreTriggerSize(handle, i, 500);    ///era 80..poco
            
            // Set the polarity for the given channel (CAEN_DGTZ_PulsePolarityPositive or CAEN_DGTZ_PulsePolarityNegative)
            ret |= CAEN_DGTZ_SetChannelPulsePolarity(handle, i, Params.PulsePolarity);
        }
    }

    ret |= CAEN_DGTZ_SetDPP_PHA_VirtualProbe(handle, CAEN_DGTZ_DPP_VIRTUALPROBE_DUAL, CAEN_DGTZ_DPP_PHA_VIRTUALPROBE1_trapezoid, CAEN_DGTZ_DPP_PHA_VIRTUALPROBE2_Input, CAEN_DGTZ_DPP_PHA_DIGITAL_PROBE_Peaking);

    if (ret) {
        printf("[!] Errors found during the programming of the digitizer\n"
               "[!] Some settings may not be executed");
        return ret;
    }
}


int main(int argc, char *argv[]) {



/* ########################### ALLOCATE MEMORY ########################### */

    key_cmd = 6900;
    shmid_cmd = shmget (6900, SHMSZ_CMD_STATUS, IPC_CREAT | 0666);
    if (shmid_cmd == -1)  printf("Errore %s\n ", strerror(errno));
    shared_memory_cmd =(int *) shmat(shmid_cmd, NULL, 0);
    printf("Data Memory attached at :%x  with shmid %d\n", (int *)shared_memory_cmd,  shmid_cmd);

    key = 7000;
    shmid = shmget (7000, SHMSZ, IPC_CREAT | 0666);
    if (shmid == -1)  printf("Errore %s\n ", strerror(errno));
    printf("Data Memory attached at :%x  with shmid %d\n", (int *)shared_memory,  shmid);
    shared_memory =(int *) shmat(shmid, NULL, 0);

    key2 = 7200;
    shmid2 = shmget (7200, SHMSZBIG, IPC_CREAT | 0666);
    if (shmid2 == -1) printf("Errore %s\n ", strerror(errno));
    shared_memory2 =(int *) shmat(shmid2, NULL, 0);
    printf("Data Memory attached at :%x  with shmid %d\n", (int *)shared_memory2,  shmid2);

    key4 = 7400;
    shmid4 = shmget (7400, SHMSZDIGI, IPC_CREAT | 0666);
       if (shmid4 == -1) printf("Errore %s\n ",strerror(errno));
    shared_memory4 =(int *) shmat(shmid4, NULL, 0);

    key_rate = 7500;
    shmid_rate = shmget (7500, SHMSZRATE, IPC_CREAT | 0666);
       if (shmid_rate == -1) printf("Errore %s\n ",strerror(errno));
    shared_memory_rate =(int *) shmat(shmid_rate, NULL, 0);
    printf("Data Memory attached at : %x with shmid4 %d\n", (int *)shared_memory_rate, shmid_rate);


    for (w=0;w<16384;w++){
    *(shared_memory+100+w)=0;          ///Azzera il vettore da plottare per mostrare lo spettro.
    *(shared_memory+20000+w)=0;}
    for(w=0;w<*(shared_memory2+4);w++){
        *(shared_memory2+11+w)=0;}

    *(shared_memory2+4)=0;   // Size of data arrays stored in memory (energy and motor position)
    *(shared_memory2+5)=0;   // Number of total spectrum events per online map
    *(shared_memory2+6)=0;   // Warning, close to data overflow (SegFault soon to be called)

/* ########################## SHARED MEMORY END ########################## */

    int pid=getpid();
    *(shared_memory_cmd+80)=pid;

    printf("pidADC\%d\n", pid);


/* ####################################################################### */
/* ######################### VARIABLES & PARAMETERS ######################### */
/* ####################################################################### */

/*
    *(shared_memory4)=10;        // Trigger Threshold
    *(shared_memory4+1)=0;       // Digitizer Input Range (0 -> 0.6V || 1 -> 1.4V)
    *(shared_memory4+2)=10000;   // Trapezoid Rise Time (ns)
    *(shared_memory4+3)=2000;    // Trapezoid Flat Top  (ns)
    *(shared_memory4+4)=300000;  // Decay Time Constant (ns)
    *(shared_memory4+5)=2000;    // Peaking delay  (ns)
    *(shared_memory4+6)=2;       // Trigger Filter Smoothing Factor
    *(shared_memory4+7)=0;       // Offset
    *(shared_memory4+8)=3000;    // Trigger Hold Off
    *(shared_memory4+9)=256;     // Trapezoid BaseLine Mean (bx10 == 64 samples)
    *(shared_memory4+10)=1;      // Peak Mean (number of points to measure the trapezoid peak)
    *(shared_memory4+11)=3000;   // Peak Hold Off
    *(shared_memory4+12)=1;      // Trapezoid Gain
    *(shared_memory4+13)=100;    // NOT USED: Delay(b) , i.e. Input Signal Rise time (ns)b)
    *(shared_memory4+14)=10;     // NOT USED: Energy Normalization Factor
    *(shared_memory4+15)=0;      // Enable digitiser reading
*/

  /********************/
  /* Signals handling */
  /********************/  

    sigact.sa_handler=termination;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags=0;
    sigaction(SIGTERM,&sigact,NULL);
    sigaction(SIGINT,&sigact,NULL);
    sigaction(SIGQUIT,&sigact,NULL);

    /* ########## SHARED MEMORY SCAN VARIABLES ########## */

    Xmin_adc=*(shared_memory_cmd+50);
    PositionX_adc=Xmin_adc;
    Xmax_adc=*(shared_memory_cmd+51);

    Ymin_adc=*(shared_memory_cmd+52);
    PositionY_adc=Ymin_adc;
    Ymax_adc=*(shared_memory_cmd+53);
    Xstep_adc=*(shared_memory_cmd+60);
    Ystep_adc=*(shared_memory_cmd+61);

    velocity_adc=*(shared_memory_cmd+67);
    time_seconds=(double)Xstep_adc/(double)velocity_adc;

    time_interval=(int)(time_seconds*1000);

    Xn=((Xmax_adc-Xmin_adc)/Xstep_adc)+1;
    Yn=((Ymax_adc-Ymin_adc)/Ystep_adc)+1;

/* ################################ TIMER ################################ */

    if (signal(SIGALRM, (void (*)(int)) write_position) == SIG_ERR) {
        perror("[!] Unable to catch SIGALRM");
        exit(1);
      }


  /********************/
  /* On ^C            */
  /********************/  

/* ####################################################################### */
/* ########################## SHARED MEMORY END ########################## */
/* ####################################################################### */




    /*Here begins the CAEN MCA control and data aquisition section*/

    /* The following variable is the type returned from most of CAENDigitizer
    library functions and is used to check if there was an error in function
    execution. For example:
    ret = CAEN_DGTZ_some_function(some_args);
    if(ret) printf("Some error"); */
    CAEN_DGTZ_ErrorCode error;

    /* Buffers to store the data. The memory must be allocated using the appropriate
    CAENDigitizer API functions (see below), so they must not be initialized here
    NB: you must use the right type for different DPP analysis (in this case PHA) */
    buffer = NULL;                                 // readout buffer
    

    /* The following variables will store the digitizer configuration parameters */
    CAEN_DGTZ_DPP_PHA_Params_t DPPParams[MAXNB];
    DigitizerParams_t Params[MAXNB];

    CAENDPP_ConnectionParams_t connParam;

    /* Arrays for data analysis */
    uint64_t PrevTime[MAXNB][MaxNChannels];
    uint64_t ExtendedTT[MAXNB][MaxNChannels];
    int ECnt[MAXNB][MaxNChannels];
    int TrgCnt[MAXNB][MaxNChannels];
    int PurCnt[MAXNB][MaxNChannels];

    /* The following variable will be used to get an handler for the digitizer. The
    handler will be used for most of CAENDigitizer functions to identify the board */
    int handle[MAXNB];

    /* Other variables */
    int i, ev;
    int Quit=0;
    int AcqRun = 0;
    uint32_t AllocatedSize, BufferSize;
    int Nb=0;
    int DoSaveWave[MAXNB][MaxNChannels];
    int MajorNumber;
    int BitMask = 0;
    uint64_t CurrentTime, PrevRateTime, ElapsedTime;
    uint32_t NumEvents[MaxNChannels];
    CAEN_DGTZ_BoardInfo_t           BoardInfo;
    FILE *f;

   int pippo = 0;		
   int data=0; ////uint16_t
   int EvToStore=1;
   int caso=10;
   int AdcData;
   int ok=0;
   int firstPos=1;
   bool datazero=false;
  
   k=0; n=1;
 
   int pluto;
   int ValidAdcEvent=0;
                        

    memset(DoSaveWave, 0, MAXNB*MaxNChannels*sizeof(int));
    for (i=0; i<MAXNBITS; i++)
        BitMask |= 1<<i; /* Create a bit mask based on number of bits of the board */

    /* *************************************************************************************** */
    /* Set Parameters                                                                          */
    /* *************************************************************************************** */
    memset(&Params, 0, MAXNB*sizeof(DigitizerParams_t));
    memset(&DPPParams, 0, MAXNB*sizeof(CAEN_DGTZ_DPP_PHA_Params_t));
    for(b=0; b<MAXNB; b++) {
        for(ch=0; ch<MaxNChannels; ch++)
            EHisto[b][ch] = NULL; //set all histograms pointers to NULL (we will allocate them later)

        /*************************** Communication Parameters   *****************************/
        // Direct USB connection

        Params[b].LinkType = CAEN_DGTZ_USB;  // Link Type
        Params[b].VMEBaseAddress = 0;  // For direct USB connection, VMEBaseAddress must be 0

        Params[b].IOlev = CAEN_DGTZ_IOLevel_TTL;
        /****************************\
        *  Acquisition parameters    *
        \****************************/
        Params[b].AcqMode = CAEN_DGTZ_DPP_ACQ_MODE_List;            // CAEN_DGTZ_DPP_ACQ_MODE_List or CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope
        Params[b].RecordLength = 2500;                              // Num of samples of the waveforms (only for Oscilloscope mode)
        Params[b].ChannelMask = 0x3;                                // Channel enable mask////era 0xF
        Params[b].EventAggr = 1;                                    // number of events in one aggregate (0=automatic)
        Params[b].PulsePolarity = CAEN_DGTZ_PulsePolarityPositive;

        // The following is for direct OPTLINK connection
        //connParam.LinkType = CAEN_DGTZ_USB;
        //connParam.LinkNum = 0;            // This defines the PCI(e) link to use
        //connParam.ConetNode = 0;          // This must increase with board number in dasy chain
        //connParam.VMEBaseAddress = 0x0;   // For direct connection the address must be 0

        /****************************\
        *      DPP parameters        *
        \****************************/


        if(*(shared_memory4+15) != 1) {

            /* ########## Channel 0 Parameters ########## */

            DPPParams[b].thr[0] = 90;      // Trigger Threshold
            DPPParams[b].k[0] =  7400;      // Trapezoid Rise Time (ns)
            DPPParams[b].m[0] = 5000;       // Trapezoid Flat Top  (ns)
            DPPParams[b].M[0] = 6300;       // Decay Time Constant (ns)
            DPPParams[b].ftd[0] = 2600;     // Peaking delay  (ns)
            DPPParams[b].a[0] = 32;         // Trigger Filter Smoothing Factor
            DPPParams[b].b[0] = 300;        // Delay(b)
            DPPParams[b].trgho[0] = 1100;   // Trigger Hold Off
            DPPParams[b].nsbl[0] = 5;       // Trapezoid Baseline Mean (entry number in menu)
            DPPParams[b].nspk[0] = 2;       // Peak Mean (entry number in menu)
            DPPParams[b].pkho[0] = 1100;    // Peak Hold Off
            DPPParams[b].blho[0] = 50;      // Trapezoid Baseline Hold Off
            DPPParams[b].enf[0] = 1;        // Energy Normalization Factor
            DPPParams[b].dgain[0] = 0;      // Digital Gain (entry number in menu)
            DPPParams[b].decimation[0] = 0; // Decimation

            /* ########## Channel 1 Parameters ########## */

            DPPParams[b].thr[1] = 90;      // Trigger Threshold
            DPPParams[b].k[1] =  7400;      // Trapezoid Rise Time (ns)
            DPPParams[b].m[1] = 5000;       // Trapezoid Flat Top  (ns)
            DPPParams[b].M[1] = 6300;       // Decay Time Constant (ns)
            DPPParams[b].ftd[1] = 2600;     // Peaking delay  (ns)
            DPPParams[b].a[1] = 32;         // Trigger Filter Smoothing Factor
            DPPParams[b].b[1] = 300;        // Delay(b)
            DPPParams[b].trgho[1] = 1100;   // Trigger Hold Off
            DPPParams[b].nsbl[1] = 5;       // Trapezoid Baseline Mean (entry number in menu)
            DPPParams[b].nspk[1] = 2;       // Peak Mean (entry number in menu)
            DPPParams[b].pkho[1] = 1100;    // Peak Hold Off
            DPPParams[b].blho[1] = 50;      // Trapezoid Baseline Hold Off
            DPPParams[b].enf[1] = 1;        // Energy Normalization Factor
            DPPParams[b].dgain[1] = 0;      // Digital Gain (entry number in menu)
            DPPParams[b].decimation[1] = 0; // Decimation
        }

        /* ########## Digitizer Interface ########## */

        if(*(shared_memory4+15) == 1) {

            printf("... Loading parameters from digitizer interface");

            CAENDPP_InputRange_t InRange_2;

            //int Input_Range=*(shared_memory4+1); doesn't appear to be used

            if (*(shared_memory4+1) == 0)       InRange_2=CAENDPP_InputRange_0_6Vpp;
            else if (*(shared_memory4+1) == 1)  InRange_2=CAENDPP_InputRange_1_4Vpp;
            else if (*(shared_memory4+1) == 2)  InRange_2=CAENDPP_InputRange_3_7Vpp;
            else if (*(shared_memory4+1) == 3)  InRange_2=CAENDPP_InputRange_9_5Vpp;

            for(ch=0; ch<MaxNChannels; ch++) {

                if ((error = CAENDPP_SetInputRange(handledpp, 0, InRange_2)) != 0) {
                    printf("[!] Cannot set channel input range, error %d", error);
                }

                DPPParams[b].thr[ch] = *(shared_memory4);
                DPPParams[b].k[ch] = *(shared_memory4+2);
                DPPParams[b].m[ch] = *(shared_memory4+3);
                DPPParams[b].M[ch] = *(shared_memory4+4);
                DPPParams[b].ftd[ch] = *(shared_memory4+5);
                DPPParams[b].a[ch] = *(shared_memory4+6);
                DPPParams[b].b[ch] = 200;
                DPPParams[b].trgho[ch] = *(shared_memory4+8);
                DPPParams[b].nsbl[ch] = *(shared_memory4+9);
                DPPParams[b].nspk[ch] = *(shared_memory4+10);
                DPPParams[b].pkho[ch] = *(shared_memory4+11);
                DPPParams[b].blho[ch] = 160;
                DPPParams[b].enf[ch] = 10;
                DPPParams[b].dgain[ch] = 0;
                DPPParams[b].decimation[ch] = 0;
            }
          }
    }

// Init the library
    retdpp = CAENDPP_InitLibrary(&handledpp);
    printf("Return Init: %d\n", retdpp);

    /* *************************************************************************************** */
    /* Open the digitizer and read board information                                           */
    /* *************************************************************************************** */

    /* The following function is used to open the digitizer with the given connection parameters
    and get the handler to it */

    for(b=0; b<MAXNB; b++) {

        /* IMPORTANT: The following function identifies the different boards with a system which may change
         * for different connection methods (USB, Conet, ecc). Refer to CAENDigitizer user manual for more info.
         * Some examples below
         */
        
        /* The following is for b boards connected via b USB direct links
         * in this case you must set Params[b].LinkType = CAEN_DGTZ_USB and Params[b].VMEBaseAddress = 0
         */

        error = CAEN_DGTZ_OpenDigitizer(Params[b].LinkType, b, 0, Params[b].VMEBaseAddress, &handle[b]);


        if (error) {
            printf("[!] Can't open digitizer\n");
            goto QuitProgram;
        }
        
        /* Once we have the handler to the digitizer, we use it to call the other functions */
        error = CAEN_DGTZ_GetInfo(handle[b], &BoardInfo);
        if (error) {
            printf("Can't read board info\n");
            goto QuitProgram;
        }

        printf("Connected to CAEN Digitizer Model %s, recognized as board %d\n", BoardInfo.ModelName, b);
        printf("ROC FPGA Release is %s\n", BoardInfo.ROC_FirmwareRel);
        printf("AMC FPGA Release is %s\n", BoardInfo.AMC_FirmwareRel);

        /* Check firmware revision (only DPP firmwares can be used with this Demo) */

        sscanf(BoardInfo.AMC_FirmwareRel, "%d", &MajorNumber);
        if (MajorNumber != 128) {
            printf("This digitizer has not a DPP-PHA firmware\n");
            goto QuitProgram;
        }

        // Rodrigo, Jun 15, commented this out.
        //error = CAENDPP_AddBoard(handledpp, connParam, &bId[0]);
    }

    /* *************************************************************************************** */
    /* Program the digitizer (see function ProgramDigitizer)                                   */
    /* *************************************************************************************** */
    for(b=0; b<MAXNB; b++) {
        error = ProgramDigitizer(handle[b], Params[b], DPPParams[b]);
        if (error) {
            printf("Failed to program the digitizer\n");
            goto QuitProgram;
        }
    }

    // Input Range
    CAENDPP_InputRange_t InRange;
    //InRange=CAENDPP_InputRange_3_7Vpp;
    InRange=CAENDPP_InputRange_0_6Vpp;

    // Set input range
    if ((error = CAENDPP_SetInputRange(handledpp, 0, InRange)) != 0) {
        printf("Can't set Input Range for Channel 0, error %d\n", error);
    }

    else {printf("Input Range of Channel 0 successfully set\n");}
    if ((error = CAENDPP_SetInputRange(handledpp, 1, InRange)) != 0) {
        printf("Can't set Input Range for Channel 1, error %d\n", error);
    }
    else {printf("Input Range of Channel 1 successfully set\n");}


    /* WARNING: The mallocs MUST be done after the digitizer programming,
     * because the following functions needs to know the digitizer configuration
     * to allocate the right memory amount
     */

    /* Allocate memory for the readout buffer */
    error = CAEN_DGTZ_MallocReadoutBuffer(handle[0], &buffer, &AllocatedSize);

    /* Allocate memory for the events */
    error |= CAEN_DGTZ_MallocDPPEvents(handle[0], Events, &AllocatedSize);

    /* Allocate memory for the waveforms */
    error |= CAEN_DGTZ_MallocDPPWaveforms(handle[0], &Waveform, &AllocatedSize);

    if (error){
        printf("Can't allocate memory buffers\n");
        goto QuitProgram;
    }

         
    /* *************************************************************************************** */
    /* Readout Loop                                                                            */
    /* *************************************************************************************** */

    // Clear Histograms and counters
    for(b=0; b<MAXNB; b++)
     {
        for(ch=0; ch<MaxNChannels; ch++) 
          {
            EHisto[b][ch] = (uint32_t *)malloc( (1<<MAXNBITS)*sizeof(uint32_t) );
            memset(EHisto[b][ch], 0, (1<<MAXNBITS)*sizeof(uint32_t));
            TrgCnt[b][ch] = 0;
            ECnt[b][ch] = 0;
            PrevTime[b][ch] = 0;
            ExtendedTT[b][ch] = 0;
            PurCnt[b][ch] = 0;
          }
    }
    PrevRateTime = get_time();
    AcqRun = 0;

    error=CAEN_DGTZ_SWStartAcquisition(handle[0]);
    if (error) ("Problem starting acquisition, ret:%d\n", error);
    printf("Acquisition Started for Board 0\n");
	
        if(*(shared_memory2+9)==1) {
            AcqRun=1;
            printf("AcqRun=1\n");
        }
   
    for(;;) {

        if(*(shared_memory_cmd+70)==0) {
            //AcqRun=0;
            printf("shared_memory_cmd+70=0\n");
            goto QuitProgram;
        }

	
        
        if(check) {
            if(*(shared_memory2+3)==1) {
                //if(y%2==0) {
                *(shared_memory2+10+n)= PositionX_adc+50000000;
                n++;
                *(shared_memory2+10+n)= PositionY_adc+60000000;
                n++;
                x++;

                /*  Statement has no effect than above
                else if( y%2 != 0) {
                    *(shared_memory2+10+n)= PositionX_adc+50000000;
                    n++;
                    *(shared_memory2+10+n)= PositionY_adc+60000000;
                    n++;
                    x++;
                }
                */

                check=false;
                *(shared_memory2+3)=0;
                it_val.it_value.tv_sec = time_interval/1000;
                it_val.it_value.tv_usec = (time_interval*1000) % 1000000;
                it_val.it_interval = it_val.it_value;
                if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
                    perror("[!] Error calling setitimer()");
                    exit(1);
                }
                AcqRun=1;
            }
        }

        /* Calculate throughput and trigger rate (every second) */
        CurrentTime = get_time();
        ElapsedTime = CurrentTime - PrevRateTime; /* milliseconds */
        if (ElapsedTime > 1000) {
            PrevRateTime = CurrentTime;
            *(shared_memory_rate)=(eventi-stored_event);
            stored_event=eventi;
        }

        /* Read data from the boards */
        for(b=0; b<MAXNB; b++) {

            error = CAEN_DGTZ_ReadData(handle[b], CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, buffer, &BufferSize);
            if (error) {
                printf("Readout Error: %d\n", error);
                //goto QuitProgram;
            }

            if (BufferSize == 0) {data=0; datazero=true;}
            else {
                datazero=false;
                error |= CAEN_DGTZ_GetDPPEvents(handle[b], buffer, BufferSize, Events, NumEvents);
                if (error) {
                    printf("Data Error: %d\n", error);
                }
            }

            /* Analyze data */
            for(ch=0; ch<MaxNChannels; ch++) {

                if (!(Params[b].ChannelMask & (1<<ch))) continue;

                if(AcqRun==1) {
                    for(ev=0; ev<NumEvents[ch]; ev++) {
                        if(datazero==false){data=Events[ch][ev].Energy;datazero=true;}
                        TrgCnt[b][ch]++;

                        printf("The value fo x and Xn are respectively: %d, %d\n", x, Xn);
                        printf("The boolean value of write_coordinates is: %d\n", write_coordinates);

                        if(data > 10 && data < 16000  && n<NSegFault)       caso=1;     // Writes the energies
                        if(write_coordinates==true && x<(Xn-1) && (y%2==0)) caso=2;     // Writes the position from left to right
                        if(write_coordinates==true && (x==(Xn-1)))          caso=3;     // Motors change direction
                        if(write_coordinates==true && x<(Xn-1) && (y%2!=0)) caso=4;     // Writes the position from right to left


                        switch (caso) {
                        case 1:
                            printf("Hey! I'm in case 1");
                            if(ch==0) {
                                // Number of events in the digitizer
                                eventi++;
                                *(shared_memory2+10+n)=data+20000000;
                                n++;
                                // Total size of data arrays in memory
                                *(shared_memory2+5)=eventi;
                                // Passes spectra data to the external program XRF for visualization
                                *(shared_memory+100+data)=*(shared_memory+100+data)+1;
                            }
                            if(ch==1) {
                                eventi++;
                                *(shared_memory2+10+n)=data+30000000;
                                n++;
                                *(shared_memory2+5)=eventi;
                                /* Applies linear transformation to align the spectra
                                 * this is where the alignment and distribution algorithm should be implememented
                                 */
                                if(*(shared_memory_cmd+100)==2) {
                                    data=(int)round(((data*(*(shared_memory_cmd+101)))+(*(shared_memory_cmd+102)))/(*(shared_memory_cmd+103)));
                                    printf("The registered channel for the second detector is : %d", data);
                                }
                                // Passes spectra data to the external program XRF for visualization
                                *(shared_memory+20000+data)=*(shared_memory+20000+data)+1;
                                printf("Data passed to main program for visualization succesfully");
                            }
                            caso=10;
                            break;

                        case 2:
                            printf("Hey! I'm in case 2");
                            x++;
                            PositionX_adc=PositionX_adc+Xstep_adc;
                            *(shared_memory2+10+n)= PositionX_adc+50000000;
                            n++;

                            *(shared_memory2+10+n)= PositionY_adc+60000000;
                            *(shared_memory2+4)=n;
                            n++;
                            caso=10;
                            write_coordinates=false;
                            *(shared_memory+42)=1;
                            break;

                        case 3:
                            printf("Hey! I'm in case 3");
                            it_val.it_value.tv_sec = 0/1000;
                            it_val.it_value.tv_usec = (0*1000)/1000000;
                            it_val.it_interval = it_val.it_value;
                            if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
                                perror("[!] Error calling setitimer()");
                                exit(1);
                            }

                            AcqRun=0;
                            y++;
                            x=0;
                            PositionY_adc=PositionY_adc+Ystep_adc;
                            write_coordinates=false;
                            check=true;
                            caso=10;
                            *(shared_memory2+8)=1;          // Allows the motors to change direction. !! This is why the program freezes
                            if(y==Yn+1) goto QuitProgram;
                            break;

                        case 4:
                            printf("Hey! I'm in case 4");
                            x++;
                            PositionX_adc=PositionX_adc-Xstep_adc;
                            *(shared_memory2+10+n)= PositionX_adc+50000000;
                            n++;

                            *(shared_memory2+10+n)= PositionY_adc+60000000;
                            *(shared_memory2+4)=n;
                            n++;
                            caso=10;
                            write_coordinates=false;
                            *(shared_memory+42)=1;
                            break;
                        }   // Closes switch statement

                        /* Get Waveforms (only from 1st event in the buffer) */
                        if ((Params[b].AcqMode != CAEN_DGTZ_DPP_ACQ_MODE_List) && DoSaveWave[b][ch] && (ev == 0)) {
                            int size;
                            int16_t *WaveLine;
                            uint8_t *DigitalWaveLine;
                            CAEN_DGTZ_DecodeDPPWaveforms(handle[b], &Events[ch][ev], Waveform);

                            // Use waveform data here...
                            size = (int)(Waveform->Ns);             // Number of samples
                            WaveLine = Waveform->Trace1;            // First trace (VIRTUALPROBE1 set with CAEN_DGTZ_SetDPP_PSD_VirtualProbe)
                            SaveWaveform(b, ch, 1, size, WaveLine);

                            WaveLine = Waveform->Trace2;            // Second Trace (if single trace mode, it is a sequence of zeroes)
                            SaveWaveform(b, ch, 2, size, WaveLine);

                            DigitalWaveLine = Waveform->DTrace1;    // First Digital Trace (DIGITALPROBE1 set with CAEN_DGTZ_SetDPP_PSD_VirtualProbe)
                            SaveDigitalProbe(b, ch, 1, size, DigitalWaveLine);

                            DigitalWaveLine = Waveform->DTrace2;    // Second Digital Trace (for DPP-PHA it is ALWAYS Trigger)
                            SaveDigitalProbe(b, ch, 2, size, DigitalWaveLine);
                            DoSaveWave[b][ch] = 0;
                            printf("Waveforms saved to 'Waveform_<board>_<channel>_<trace>.txt'\n");

                        }   // Close if statement to save waveforms
                    }       // Close for loop to write events
                }           // Close if statement to verify acquisition is running
            }               // Close loop on all channels
        }                   // Close loop on boards
    }                       // Close readout loop

    *(shared_memory2+4)=n;

QuitProgram:

    it_val.it_value.tv_sec = 0/1000;
    it_val.it_value.tv_usec = (0*1000)/1000000;
    it_val.it_interval = it_val.it_value;
    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
        perror("[!] Error calling setitimer()");
        exit(1);
    }


    /* Stop the acquisition, close the device and free the buffers */
    for(b=0; b<MAXNB; b++) {
        CAEN_DGTZ_SWStopAcquisition(handle[b]);
        for (ch=0; ch<MaxNChannels; ch++)
            if (EHisto[b][ch] != NULL)
                free(EHisto[b][ch]);
    }

    printf("... Closing digitizer\n");

    CAEN_DGTZ_FreeReadoutBuffer(&buffer);
    CAEN_DGTZ_FreeDPPEvents(handle[0], Events);
    CAEN_DGTZ_FreeDPPWaveforms(handle[0], Waveform);
    CAEN_DGTZ_ClearData(handle[0]);
    CAENDPP_EndLibrary(handledpp);
    CAEN_DGTZ_CloseDigitizer(handle[b]);

    //*(shared_memory2+4)=n;
    printf("Number of recorded position and energy snippets: %d\n", n);
    return error;
}
    
void write_position() {
        if (write_coordinates==false)
            write_coordinates=true;
        else
            printf("write_coordinates is already true\n"
                   "[!] The coordinates are not being flushed into memory");
}

void sigquit() {
    printf("... Exiting\n\n");
    exit(1);
}

void termination(int pippo) {

    printf("\n recorded events:%d\n",eventi);
    *(shared_memory2+4)=n;
    *(shared_memory_cmd+70)=0;
    printf("..Exiting..on termination\n");
    exit(2);
}
