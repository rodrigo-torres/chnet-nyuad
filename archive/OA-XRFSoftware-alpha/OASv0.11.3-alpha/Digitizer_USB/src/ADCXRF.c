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
#include "Functions.h"
#include <../../Shm.h>
#include <../../variables.h>

#define MAXNB   1
#define MAXNBITS 14
#define MaxNChannels 2
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

key_t key_cmd, key, key2, key4, key_rate;
int *shared_memory_cmd, *shared_memory, *shared_memory2, *shared_memory4, *shared_memory_rate;


int NSegFault=2000000000;  ///61440000 dovrebbe essere il massimo assoluto


//////////////////////////////////////////
void termination(int);
struct sigaction sigact;

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


void configTimer(int seconds);

int ProgramDigitizer(int handle, DigitizerParams_t Params, CAEN_DGTZ_DPP_PHA_Params_t DPPParams) {
    /* This function uses the CAENDigitizer API functions to perform the digitizer's initial configuration */

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

    int offset = 0;
    if (*(shared_memory4+15) == 1) {
        offset = *(shared_memory4+7);
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
                ret |= CAEN_DGTZ_SetChannelDCOffset(handle, i, 19661);
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

    if (*(shared_memory4+15) != 1) // To load pre-defined values
        for (ch = 0; ch < MaxNChannels; ch++) {
            if ((ret = CAENDPP_SetInputRange(handledpp, ch, CAENDPP_InputRange_0_6Vpp)) != 0)
                printf("[!] Can't set InputRange for channel %d, error %d\n",ch,ret);
        }
    else {
        CAENDPP_InputRange_t inRange;
        if (*(shared_memory4+1) == 0 || NULL) inRange = CAENDPP_InputRange_0_6Vpp;
        else if (*(shared_memory4+1) == 1) inRange = CAENDPP_InputRange_1_4Vpp;
        else if (*(shared_memory4+1) == 2) inRange = CAENDPP_InputRange_3_7Vpp;
        else if (*(shared_memory4+1) == 3) inRange = CAENDPP_InputRange_9_5Vpp;

        for (ch = 0; ch < MaxNChannels; ch++) {
            if ((ret = CAENDPP_SetInputRange(handledpp, ch, inRange)) != 0)
                printf("[!] Can't set InputRange for channel %d, error %d\n",ch,ret);
        }
    }

    return ret;
}

int* assignSHM(key_t key, size_t size, int id) {
    int shmID = shmget(key, size, IPC_CREAT | 0666);
    if (shmID == -1) {
        printf("[!] Couldn't obtain a shared memory segment ID for key:\t%d\n", key);
        printf("%s\n", strerror(errno));
        return NULL;
    }
    else return (int*)(shmat(shmID, NULL, 0));
}

int main(int argc, char *argv[]) {
    key_cmd = 6900, key = 7000, key2 = 7200, key4 = 7400, key_rate = 7500;

    shared_memory_cmd = assignSHM(key_cmd, SHMSZ_CMD_STATUS, 0);
    shared_memory = assignSHM(key, SHMSZ, 1);
    shared_memory2 = assignSHM(key2, SHMSZBIG, 2);
    shared_memory4 = assignSHM(key4, SHMSZDIGI, 4);
    shared_memory_rate = assignSHM(key_rate, SHMSZRATE, 6);

    for (int w = 0; w < 16384; w++) { // Clear histograms in shared memory
        *(shared_memory+100+w)=0;
        *(shared_memory+20000+w)=0;
        *(shared_memory+40000+w)=0;
        //if ( w == 0 ) printf("%d %d %d\n", *(shared_memory+100+w), *(shared_memory+20000+w), *(shared_memory+40000+w));
    }

    for (int w = 0; w < *(shared_memory2+4); w++) *(shared_memory2+11+w)=0; // Clears loadSHM array for scan files

    *(shared_memory2+4)=0;  // Number of records in memory (position and energy)
    *(shared_memory2+5)=0;  // Number of total events per acquisition
    *(shared_memory2+6)=0;  // Warning, number of records in memory close to SegmentationFault

    *(shared_memory_cmd+80) = getpid();
    printf("... ADC started with pid:\t%d\n", getpid());

    /* DIGITIZER PARAMETERS
                                  *(shared_memory4)=10;        // Trigger Threshold
                                  *(shared_memory4+1)=0;       // Input range digitalizzatore (0 -> 0.6V // 1 -> 1.4V
                                  *(shared_memory4+2)=10000;   // Trapezoid Rise Time (ns)
                                  *(shared_memory4+3)=2000;    // Trapezoid Flat Top  (ns)
                                  *(shared_memory4+4)=300000;  // Decay Time Constant (ns) HACK-FPEP the one expected from fitting algorithm?
                                  *(shared_memory4+5)=2000;    // Peaking delay  (ns)
                                  *(shared_memory4+6)=2;       // Trigger Filter smoothing factor
                                  *(shared_memory4+7)=0;       // OFFSET
                                  *(shared_memory4+8)=3000;    // Trigger Hold Off
                                  *(shared_memory4+9)=256;     // BaseLine Mean - ?? 3 = bx10 = 64 samples (baseline mean del trapezio) (...?? 3)
                                  *(shared_memory4+10)=1;      // peak mean (numero punti su cui mediare per peaking del trapezio)
                                  *(shared_memory4+11)=3000;   // peak holdoff (min intervallo tra 2 trapezi...minimo=k+m)
                                  *(shared_memory4+12)=1;      // Trapezoid Gain
                                  *(shared_memory4+13)=100;    // NOT USED: Delay(b) Input Signal Rise time (ns)...sarebbe delay (b)
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

    // Dereferences from shared memory the scanning variable values
    int Xmin_adc = *(shared_memory_cmd+50);
    int Xmax_adc = *(shared_memory_cmd+51);
    int Ymin_adc = *(shared_memory_cmd+52);
    int Ymax_adc = *(shared_memory_cmd+53);
    int Xstep_adc= *(shared_memory_cmd+60);
    int Ystep_adc= *(shared_memory_cmd+61);

    int PositionX_adc = Xmin_adc;
    int PositionY_adc = Ymin_adc;
    int Xn = ((Xmax_adc - Xmin_adc) / Xstep_adc) + 1;
    int Yn = ((Ymax_adc - Ymin_adc) / Ystep_adc) + 1;


    double time_seconds = (double)Xstep_adc / (double)*(shared_memory_cmd+67);
    int time_interval = (int)(time_seconds*1000);

    printf("... Setting %d X steps and %d Y steps and %d interval\n", Xn, Yn, time_interval);
    /********************/
    /*      Timer       */
    /********************/

    // SIGALRM is sent when real or clock time elapses (defined in struct itimerval)
    if ( signal(SIGALRM, (void (*)(int)) write_position) == SIG_ERR) {
        perror("[!] Unable to catch SIGALRM");
        exit(1);
    }


    /********************/
    /* On ^C            */
    /********************/

    CAEN_DGTZ_ErrorCode ret;
    buffer = NULL;

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
    uint32_t NumEvents[MaxNChannels];
    uint32_t AllocatedSize, BufferSize;
    uint64_t CurrentTime, PrevRateTime, ElapsedTime;
    CAEN_DGTZ_BoardInfo_t BoardInfo;

    int MajorNumber;
    int AcqRun = 0, BitMask = 0, data = 0, caso = 10;

    k = 0;
    n = 1;

    memset(&Params, 0, MAXNB*sizeof(DigitizerParams_t));
    memset(&DPPParams, 0, MAXNB*sizeof(CAEN_DGTZ_DPP_PHA_Params_t));

    for (b = 0; b < MAXNB; b++) { // Initialize DPP parameters
        for (ch = 0; ch < MaxNChannels; ch++) EHisto[b][ch] = NULL;


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
        *      DPP parameters        *
        \****************************/

        if (*(shared_memory4+15) != 1) { // To load pre-defined values
            /* Trigger and shaping paramters for channel 0 */
            DPPParams[b].thr[0] = 400;// Trigger Threshold
            DPPParams[b].k[0] =  1000;// Trapezoid Rise Time (ns)
            DPPParams[b].m[0] = 1000;// Trapezoid Flat Top  (ns)
            DPPParams[b].M[0] = 3500;// Decay Time Constant (ns)
            DPPParams[b].ftd[0] = 600;// Peaking delay  (ns)
            DPPParams[b].a[0] = 32; // Trigger Filter smoothing factor
            DPPParams[b].b[0] = 200; // Delay(b)
            DPPParams[b].trgho[0] = 1300;// Trigger Hold Off
            DPPParams[b].nsbl[0] = 6;// Baseline mean del trapezio (ordine di comparsa nel menu)
            DPPParams[b].nspk[0] = 3;// Peak mean (ordine di comparsa nel menu)
            DPPParams[b].pkho[0] = 1100;// Peak holdoff
            DPPParams[b].blho[0] = 3100;// Baseline holdoff del trapezio
            DPPParams[b].enf[0] = 1;// Energy Normalization Factor
            DPPParams[b].dgain[0] = 0;//Digital Gain (ordine di comparsa nel menu)
            DPPParams[b].decimation[0] = 0;// Decimation

            /* Trigger and shaping paramters for channel 1 */
            DPPParams[b].thr[1] = 1000;// Trigger Threshold
            DPPParams[b].k[1] =  1000;// Trapezoid Rise Time (ns)
            DPPParams[b].m[1] = 1000;// Trapezoid Flat Top  (ns)
            DPPParams[b].M[1] = 8500;// Decay Time Constant (ns)
            DPPParams[b].ftd[1] = 500;// Peaking delay  (ns)
            DPPParams[b].a[1] = 1; // Trigger Filter smoothing factor
            DPPParams[b].b[1] = 200; // Delay(b)
            DPPParams[b].trgho[1] = 1100;// Trigger Hold Off
            DPPParams[b].nsbl[1] = 6;// Baseline mean del trapezio (ordine di comparsa nel menu)
            DPPParams[b].nspk[1] = 3;// Peak mean (ordine di comparsa nel menu)
            DPPParams[b].pkho[1] = 1100;// Peak holdoff
            DPPParams[b].blho[1] = 3100;// Baseline holdoff del trapezio
            DPPParams[b].enf[1] = 1;// Energy Normalization Factor
            DPPParams[b].dgain[1] = 0;//Digital Gain (ordine di comparsa nel menu)
            DPPParams[b].decimation[1] = 0;// Decimation
        }

        else { // For importing DPP parameters from Digitizer interface
            printf("... Loading parameters from DPP interface\n");
            for (ch = 0; ch < MaxNChannels; ch++) {

                DPPParams[b].thr[ch] = 400;// Trigger Threshold
                DPPParams[b].k[ch] =  1000;// Trapezoid Rise Time (ns)
                DPPParams[b].m[ch] = 1000;// Trapezoid Flat Top  (ns)
                DPPParams[b].M[ch] = 8500;// Decay Time Constant (ns)
                DPPParams[b].ftd[ch] = 500;// Peaking delay  (ns)
                DPPParams[b].a[ch] = 1; // Trigger Filter smoothing factor
                DPPParams[b].b[ch] = 200; // Delay(b)
                DPPParams[b].trgho[ch] = 1100;// Trigger Hold Off
                DPPParams[b].nsbl[ch] = 6;// Baseline mean del trapezio (ordine di comparsa nel menu)
                DPPParams[b].nspk[ch] = 3;// Peak mean (ordine di comparsa nel menu)
                DPPParams[b].pkho[ch] = 1100;// Peak holdoff
                DPPParams[b].blho[ch] = 3100;// Baseline holdoff del trapezio
                DPPParams[b].enf[ch] = 1;// Energy Normalization Factor
                DPPParams[b].dgain[ch] = 0;//Digital Gain (ordine di comparsa nel menu)
                DPPParams[b].decimation[ch] = 0;// Decimation
                //                DPPParams[b].thr[ch]        = *(shared_memory4);    // Threshold
                //                DPPParams[b].k[ch]          = *(shared_memory4+2);  // Rise time
                //                DPPParams[b].m[ch]          = *(shared_memory4+3);  // Flat top
                //                DPPParams[b].M[ch]          = *(shared_memory4+4);  // Decay time
                //                DPPParams[b].ftd[ch]        = *(shared_memory4+5);  // Peaking time
                //                DPPParams[b].a[ch]          = *(shared_memory4+6);  // Smoothing factor
                //                DPPParams[b].b[ch]          = 200;                  // Delay(b)
                //                DPPParams[b].trgho[ch]      = *(shared_memory4+8);  // Trigger holdoff
                //                DPPParams[b].nsbl[ch]       = *(shared_memory4+9);  // Baseline mean samples
                //                DPPParams[b].nspk[ch]       = *(shared_memory4+10); // Peak mean samples
                //                DPPParams[b].pkho[ch]       = *(shared_memory4+11); // Peak holdoff
                //                DPPParams[b].blho[ch]       = 2100;                  // Trapezium baseline holdoff (at least m+2*k)
                //                DPPParams[b].enf[ch]        = 1;                   // Energy Normalization Factor
                //                DPPParams[b].dgain[ch]      = 0;
                //                DPPParams[b].decimation[ch] = 0;
            }
        }
    }
    retdpp = CAENDPP_InitLibrary(&handledpp);

    for (b = 0; b < MAXNB; b++) { // Open the digitizer and get its info
        ret = CAEN_DGTZ_OpenDigitizer(Params[b].LinkType, b, 0, Params[b].VMEBaseAddress, &handle[b]);
        if (ret) { printf("[!] Can't open digitizer\n"); goto QuitProgram; }

        ret = CAEN_DGTZ_GetInfo(handle[b], &BoardInfo);
        if (ret) { printf("[!] Can't read board info\n"); goto QuitProgram; }
        printf("\nConnected to CAEN Digitizer Model %s, recognized as board %d\n", BoardInfo.ModelName, b);
        printf("ROC FPGA Release is %s\n", BoardInfo.ROC_FirmwareRel);
        printf("AMC FPGA Release is %s\n", BoardInfo.AMC_FirmwareRel);

        sscanf(BoardInfo.AMC_FirmwareRel, "%d", &MajorNumber);
        if (MajorNumber != 128) { printf("[!] This digitizer has not a DPP-PHA firmware\n"); goto QuitProgram; }
        ret = CAENDPP_AddBoard(handledpp, connParam, &bId[0]);
    }

    for (b = 0; b < MAXNB; b++) { // Program the Digitizer
        ret = ProgramDigitizer(handle[b], Params[b], DPPParams[b]);
        if (ret) { printf("[!] Failed to program the digitizer\n"); goto QuitProgram; }
    }

    ret = CAEN_DGTZ_MallocReadoutBuffer(handle[0], &buffer, &AllocatedSize);
    ret |= CAEN_DGTZ_MallocDPPEvents(handle[0], Events, &AllocatedSize);        /* Allocate memory for the events */
    ret |= CAEN_DGTZ_MallocDPPWaveforms(handle[0], &Waveform, &AllocatedSize);  /* Allocate memory for the waveforms */
    if (ret) { printf("[!] Can't allocate memory buffers\n"); goto QuitProgram; }



    /* *************************************************************************************** */
    /* Readout Loop                                                                            */
    /* *************************************************************************************** */

    for (b = 0; b < MAXNB; b++) { // Clear histograms and counters
        for (ch = 0; ch < MaxNChannels; ch++)  {
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
    if (*(shared_memory2+9) || *(shared_memory_cmd+70)) AcqRun = 1;

    ret=CAEN_DGTZ_SWStartAcquisition(handle[0]);
    if (ret || !AcqRun) {printf("[!] Problem starting acquisition, error code: %d\n", ret);}
    else printf("[!] Acquisition started\n");

    setseed(); // Sets the first seed for the acquisition.

    int codePosX = 50000000, codePosY = 60000000;
    int codeDetA = 20000000, codeDetB = 30000000;
    bool discardfirst = true;
    double calGrad = (double)(*(shared_memory_cmd+101)) / (double)(*(shared_memory_cmd+103));
    double calOffs = (double)(*(shared_memory_cmd+102)) / (double)(*(shared_memory_cmd+103));

    printf("Multidetector paremeters: %5.3f %5.3f\n", calGrad, calOffs);

    while (*(shared_memory_cmd+70) != 0) { // While acquisition is active

        /* !! This condition can be simplified, the need for 'check' can be eliminated if *(shared_memory2+3) is better placed in MainWindow::ScanXY() */
        if (*(shared_memory2+3) == 1 && check) { // Handles changes in scan lines
            *(shared_memory2+10+n)= PositionX_adc+codePosX; n++;
            *(shared_memory2+10+n)= PositionY_adc+codePosY; n++; x++;

            *(shared_memory2+3) = 0; check = false;
            configTimer(time_interval);
            AcqRun=1;
        }

        if (write_coordinates) {
            if (x < (Xn - 1) && (y % 2 == 0))       caso=1; // Writes positions (from left-to-right)
            else if (x == (Xn - 1))                 caso=2; // Changes scan line
            else if (x < (Xn - 1) && (y % 2 != 0))  caso=3; // Writes positions (from right-to-left)

            switch (caso) {
            case 1:
                PositionX_adc += Xstep_adc; x++;
                *(shared_memory2+10+n) = PositionX_adc+codePosX; n++;
                *(shared_memory2+10+n) = PositionY_adc+codePosY; n++;
                *(shared_memory2+4) = n;

                *(shared_memory+42) = 1;    // Informs the Online Map of new data available
                write_coordinates = false;
                break;
            case 2:
                configTimer(0);
                PositionY_adc += Ystep_adc;
                x = 0; y++; AcqRun = 0;

                write_coordinates = false; check = true;
                *(shared_memory2+8) = 1;
                if (y == Yn+1) goto QuitProgram;
                break;
            case 3:
                PositionX_adc -= Xstep_adc; x++;
                *(shared_memory2+10+n) = PositionX_adc+codePosX; n++;
                *(shared_memory2+10+n) = PositionY_adc+codePosY; n++;
                *(shared_memory2+4) = n;

                *(shared_memory+42) = 1;
                write_coordinates = false;
                break;
            default:
                printf("[!] Case not defined\n");
                break;
            }
        }

        CurrentTime = get_time();
        ElapsedTime = CurrentTime - PrevRateTime; // In milliseconds
        if (ElapsedTime > 1000) { // Calculate trigger rate (per second)
            PrevRateTime = CurrentTime;
            *(shared_memory_rate) = (eventi - stored_event);
            stored_event = eventi;
        }

        for (b = 0; b < MAXNB; b++) { // Read data from the boards
            ret = CAEN_DGTZ_ReadData(handle[b], CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, buffer, &BufferSize);
            if (ret) printf("[!] Readout error, code: %d\n", ret);

            if (BufferSize != 0) {
                ret |= CAEN_DGTZ_GetDPPEvents(handle[b], buffer, BufferSize, Events, NumEvents);
                if (ret) printf("[!] Data error, code: %d\n", ret);
            }

            for (ch = 0; ch < MaxNChannels; ch++) {
                if ( !(Params[b].ChannelMask & (1 << ch)) ) continue; // Always false for ch = 0 and ch = 1
                if (AcqRun == 1) {
                    for (int ev = 0; ev < NumEvents[ch]; ev++) {
                        if (BufferSize != 0) data = Events[ch][ev].Energy;
                        else continue;

                        TrgCnt[b][ch]++;


                        if (data >= 0 && data < 16385  && n<NSegFault)          caso=4; // Writes the energies
                        if (discardfirst)                                       caso=5;
                        switch (caso) {
                        case 4:
                            if (ch == 0) {
                                *(shared_memory2+10+n) = data+codeDetA; n++;

                                *(shared_memory2+5) = ++eventi;
                                *(shared_memory+100+data) += 1;
                                *(shared_memory+40000+data) +=1;
                            }
                            if (ch == 1) {
                                *(shared_memory2+10+n) = data+codeDetB; n++;

                                *(shared_memory2+5) = ++eventi;
                                *(shared_memory+20000+data) += 1;

                                jlo = (double)(data - 1) * calGrad + calOffs;
                                jhi = jlo + calGrad;
                                data = ranqd1(idum,jlo,jhi);

                                *(shared_memory+40000+data) += 1;
                            }
                            break;
                        case 5:
                            ev = NumEvents[ch];
                            discardfirst = false;
                            break;
                        }
                    }
                }
            }
        }
    }

    goto QuitProgram;

QuitProgram:
    configTimer(0); // Stops acquisition, closes digitizer, and frees buffers
    for (int b = 0; b < MAXNB; b++) {
        CAEN_DGTZ_SWStopAcquisition(handle[b]);
        for (int ch = 0; ch < MaxNChannels; ch++)
            if (EHisto[b][ch] != NULL) free(EHisto[b][ch]);
    }
    CAEN_DGTZ_FreeReadoutBuffer(&buffer);
    CAEN_DGTZ_FreeDPPEvents(handle[0], Events);
    CAEN_DGTZ_FreeDPPWaveforms(handle[0], Waveform);
    CAEN_DGTZ_ClearData(handle[0]);
    CAENDPP_EndLibrary(handledpp);

    printf("[!] Closing digitizer\n");
    CAEN_DGTZ_CloseDigitizer(handle[b]);

    *(shared_memory2+4) = n;
    printf("... Total no. of records in memory: %d\n", *(shared_memory2+4));
    return ret;
}

void configTimer(int seconds) {
    it_val.it_value.tv_sec = seconds / 1000;
    it_val.it_value.tv_usec = (seconds * 1000) % 1000000;
    it_val.it_interval = it_val.it_value;
    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
        perror("[!] Error setting interval timer");
        exit(1);
    }
}

void write_position() {
    printf("Current x step is %d, y step is %d\n", x, y);
    if (write_coordinates == false) write_coordinates=true;
    else printf("write_coordinates is already true\n");
}


void termination(int pippo) {
    printf("\n... Recorded events:%d\n", eventi);
    *(shared_memory2+4)=n;
    *(shared_memory_cmd+70)=0;
    printf("... Exiting on termination\n");
    exit(2);
}
