#include <stdlib.h> 
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h> 
#include <string.h> 
#include <math.h> 
#include </usr/include/sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
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
#include <sys/ioctl.h>
#include <CAENDPPLib.h> 
#include <CAENDPPLibTypes.h>

#include <../../Shm.h>
#include <../../variables.h>


#define GNUPLOT_CMD "gnuplot"

#define MAX_BOARD_NUMBER MAX_NUMB  //maximum number of boards.(MAX_NUMB is defined in 'CAENDPPLibTypes.h')
#define MAX_BOARD_CHNUM MAX_NUMCHB  //maximum number of board's channels.(MAX_NUMBCH is defined in 'CAENDPPLibTypes.h')

#define MAX_HISTOBITS (14)  // 14 bits ->from bin 0 to bin 16383 
#define MAX_HISTO_NBINS (0x1 << (MAX_HISTOBITS-0))

#define MAX_ERRMSG_LEN 1000
/////////variabili memoria shm///////////////////////////////////////////////////////////
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

int NSegFault=16000000;  ///61440000 dovrebbe essere il massimo assoluto
int eventi=0;
double event_rate=0; int stored_event=0;
int ch, ev, w;
int n=0;
int caso=10;
int firstPos=0;
int data=0;

//////////////variables for the scanning///////////////////
int Xmin_adc=0; int Xmax_adc=0;
int Ymin_adc=0; int Ymax_adc=0;
int Xstep_adc=0; int Ystep_adc=0;
int velocity_adc=0;
int PositionX_adc=0; int PositionY_adc=0;
int x=0; int Xn=0; int y=0; int Yn=0;
int check=1;
int AcqRun = 0;
uint64_t CurrentTime, PrevRateTime, ElapsedTime;
////////////definitions for the timer/////////////

struct itimerval it_val;
double time_seconds=0;
int time_interval; //frequency to which the positions are written in memory
void write_position(); //this function sets a variable to true each time a position needs to be written in memory
bool write_coordinates=false; //write=true if the position must be written; write=false if it needs to wait
int changeposition=0; //a number that is incremented each time a position is writen, it is used to calculate the positions


double millisec;
struct timeval tv;
//////////////////////////////////////////
void termination(int);
struct sigaction sigact;
void sigquit();
/////////////////////////////////////////
////variabili lettura digitalizzatore////////////////////
int32_t currBrd;                                // Current board index
int32_t currCh;                                 // Current channel index
int32_t currHVBrd;                              // Current Board Index controlled in HV Interface
int32_t currHVCh;                               // Current High Voltage Board Channel Index
int32_t currHisto;                              // Current histogram index
int32_t connectedBrdNum=0;                        // Number of connected boards (only one in our case...the value is 0)
int32_t connectedChNum=0;                         // Number of connected channels
int32_t IFMode;                                 // Used to define the active operating mode (ACQ / HV)

FILE *gnuplot;                                  // Pipe to the plotter

    uint32_t *h1;
    uint32_t MIN_REQ_EVTS = 1;
    uint32_t nEvts = 0;
    int32_t quit = 0;
    uint32_t automask;
    int16_t *AT1 = NULL, *AT2 = NULL;
    uint8_t *DT1 = NULL, *DT2 = NULL;
    int32_t d32;
    uint32_t u32;
    uint16_t u16;
    double f64;
    double *pF64;
    uint64_t realTime, deadTime;
    char nextC = 0, chr;
    CAENDPP_PWDownMode_t PWDownMode;
    int32_t SWTriggerEnabled = 0;
    int32_t bFirst, bLast;
    int32_t chFirst, chLast;
    int32_t ContinuousPlot = 0;
    int32_t ContinuousHVMon = 0;
    int32_t histoNBins;
    char codeStr[MAX_ERRMSG_LEN + 1];
///////////////////////////////////////////////////////////////

char *decodeError(char *dest, int32_t code);

int main(int argc, char *argv[])
{

int32_t ret = 0;
int32_t ch;
uint32_t RecordLength = 2500;
int32_t handle; // Handler to the Library 
int32_t bId[MAX_BOARD_NUMBER], brd;  

CAENDPP_ConnectionParams_t connParam;
CAENDPP_Info_t info[MAX_BOARD_NUMBER];
CAENDPP_DgtzParams_t dgtzParams; 
 
CAENDPP_AcqMode_t acqMode;
CAENDPP_StopCriteria_t StopCriteria;
 uint64_t StopCriteriaValue;
CAENDPP_HVChannelConfig_t HVConfig;
CAENDPP_InputRange_t InRange[MAX_BOARD_CHNUM];
///////////////////////////////////////shared memory begin//////////////////////////////////

   key_cmd = 6900;
   shmid_cmd = shmget (6900, SHMSZ_CMD_STATUS, IPC_CREAT | 0666);
   if (shmid_cmd == -1)  printf("Errore %s\n ", strerror(errno));
   shared_memory_cmd =(int *) shmat(shmid_cmd, NULL, 0);
   printf("Data Memory attached at :%x  with shmid %d\n", (int *)shared_memory_cmd,  shmid_cmd);

   key = 7000;
   shmid = shmget (7000, SHMSZ, IPC_CREAT | 0666);
   if (shmid == -1)  printf("Errore %s\n ", strerror(errno));
   shared_memory =(int *) shmat(shmid, NULL, 0);
   printf("Data Memory attached at :%x  with shmid %d\n", (int *)shared_memory,  shmid);

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


   for (w=0;w<16384;w++)
   *(shared_memory+100+w)=0;          ///Azzera il vettore da plottare per mostrare lo spettro
// *(shared_memory2+1)=0;    //// Posiz X
// *(shared_memory2+2)=0;    //// Posiz Y
// *(shared_memory2+3)=0;    //// Posizione Ready
   *(shared_memory2+4)=0;    //// Numero dati in memoria (posizioni e energie)
   *(shared_memory2+5)=0;     ///NEvents totali ADC per mappa online
   *(shared_memory2+6)=0;     ///N vicino a SegFault


  /********************/
  /* Signals handling */
  /********************/  

  sigact.sa_handler=termination;
  sigemptyset(&sigact.sa_mask);
  sigact.sa_flags=0;
  sigaction(SIGTERM,&sigact,NULL);
  sigaction(SIGINT,&sigact,NULL);
  sigaction(SIGQUIT,&sigact,NULL);

  /********************/
  /* On ^C            */
  /********************/ 

int pid=getpid();
*(shared_memory_cmd+80)=pid;

/////////////reads from the shared memory the variables for the scanning///////////////

Xmin_adc=*(shared_memory_cmd+50); //printf("Xmin_adc:%d\n", Xmin_adc);
PositionX_adc=Xmin_adc;
Xmax_adc=*(shared_memory_cmd+51); //printf("Xmax_adc:%d\n", Xmax_adc);
Ymin_adc=*(shared_memory_cmd+52); //printf("Ymin_adc:%d\n", Ymin_adc);
PositionY_adc=Ymin_adc;
Ymax_adc=*(shared_memory_cmd+53); //printf("Ymax_adc:%d\n", Ymax_adc);
Xstep_adc=*(shared_memory_cmd+60); //printf("Xstep_adc:%d\n", Xstep_adc);
Ystep_adc=*(shared_memory_cmd+61); //printf("Ystep_adc:%d\n", Ystep_adc);
velocity_adc=*(shared_memory_cmd+67); //printf("Velocity_adc:%d\n", velocity_adc);
time_seconds=(double)Xstep_adc/(double)velocity_adc;
//printf("time_seconds:%f\n", time_seconds);
time_interval=(int)(time_seconds*1000); //time in milliseconds, for now we take only the Xstep assuming that it is the same as the Ystep in case of YX scan
//printf("time_interval:%d\n", time_interval);
Xn=((Xmax_adc-Xmin_adc)/Xstep_adc)+1;
Yn=((Ymax_adc-Ymin_adc)/Ystep_adc)+1;

/////////////////////////timer////////////////////////////////////

if (signal(SIGALRM, (void (*)(int)) write_position) == SIG_ERR) {
    perror("Unable to catch SIGALRM");
    exit(1);
  }



// Init the library
ret = CAENDPP_InitLibrary(&handle);
if(ret)printf("Error initing CAENDPP Library! (err %d)\n", ret);

connParam.LinkType = CAENDPP_PCI_OpticalLink;  ///connection parameters
connParam.LinkNum = 0;            
connParam.ConetNode = 0;          
connParam.VMEBaseAddress = 0x0;   

ret = CAENDPP_AddBoard(handle, connParam, &bId[connectedBrdNum]); // Add board to the Library
   if (ret != CAENDPP_RetCode_Ok) {
            printf("Error adding board with selected connection parameters: %s", decodeError(codeStr, ret));
            return 0;
            }
            
ret = CAENDPP_GetDPPInfo(handle, bId[connectedBrdNum], &info[connectedBrdNum]);// Get Board Info
   if (ret != CAENDPP_RetCode_Ok) {
            printf("Error getting board info: %s", decodeError(codeStr, ret));
            return 0;
            }
            
printf("Board %s_%u added succesfully", info[connectedBrdNum].ModelName, info[connectedBrdNum].SerialNumber);

connectedChNum += info[connectedBrdNum].Channels;
    // Alloc the histogram to the given number of bins.--------------------->>>serve???
    h1 = (uint32_t *)calloc(MAX_HISTO_NBINS, sizeof(uint32_t));
    if (h1 == NULL) {
        ret = - 1;
        { printf("Error allocating memory for histograms\n");return 0;}
    }

dgtzParams.ChannelMask = 0x1;
    
//acqMode = CAENDPP_AcqMode_Waveform;   // Oscilloscope mode (waves + histogram)
acqMode = CAENDPP_AcqMode_Histogram;    // Histogram mode (only energy)

//InRange = CAENDPP_InputRange_9_5Vpp;
//InRange = CAENDPP_InputRange_3_7Vpp;
//InRange = CAENDPP_InputRange_1_4Vpp;
//InRange = CAENDPP_InputRange_0_6Vpp;
InRange[0] = CAENDPP_InputRange_3_7Vpp;  //Input range for ch 0
InRange[1] = CAENDPP_InputRange_3_7Vpp; //Input range for ch 1

for (ch = 0; ch < MAX_BOARD_CHNUM; ch++) {

        dgtzParams.DCoffset[ch] = 0x8018; // DC Offset 
        dgtzParams.PulsePolarity[ch] = CAENDPP_PulsePolarityPositive; // Pulse Polarity
        
        // DPP - PHA Parameters
        dgtzParams.DPPParams.M[ch] = 12000; // Signal Decay Time Constant (ns)
        dgtzParams.DPPParams.m[ch] = 2500; // Trapezoid Flat Top (ns)
        dgtzParams.DPPParams.k[ch] = 800; // Trapezoid Rise Time (ns)
        dgtzParams.DPPParams.ftd[ch] = 1100; // Flat Top Delay (ns)
        dgtzParams.DPPParams.a[ch] = 5; // Trigger Filter smoothing factor(position in the menu)
        dgtzParams.DPPParams.b[ch] = 200; // Input Signal Rise time (ns)
        dgtzParams.DPPParams.thr[ch] = 40; // Trigger Threshold
        dgtzParams.DPPParams.nsbl[ch] = 4; // Number of Samples for Baseline Mean (position in the menu)
        dgtzParams.DPPParams.nspk[ch] = 1; // Number of Samples for Peak Mean Calculation
        dgtzParams.DPPParams.pkho[ch] = 2700; // Peak Hold Off (ns)
        dgtzParams.DPPParams.blho[ch] = 160; // Base Line Hold Off
        dgtzParams.DPPParams.trgho[ch] = 3600; // Trigger Hold Off
        dgtzParams.DPPParams.dgain[ch] = 0; // Digital Probe Gain
        dgtzParams.DPPParams.enf[ch] = 10; // Energy Nomralization Factor
        dgtzParams.DPPParams.decimation[ch] = 0; // Decimation of Input Signal
        dgtzParams.DPPParams.twwdt[ch] = 200; //Zero crossing acceptance window for the Rise Time Discriminator (Pile-up Rejection).
                                       //(ns) starting from the RC-CR2overthreshold which arms the acquisition
       // dgtzParams.DPPParams.trgwin[ch] = AUTOSET_VALUE; //Trigger acceptance window in coincidence mode

        ret = CAENDPP_SetInputRange(handle, ch, InRange[ch]);
            if ( ret!= CAENDPP_RetCode_Ok) {
               printf("Can't set Input Range for Channel %d: %s", ch, decodeError(codeStr, ret));
                break;
            }
    }

ret = CAENDPP_SetBoardConfiguration(handle, bId[brd], acqMode, dgtzParams);
    if (ret != CAENDPP_RetCode_Ok)
        {printf("Can't configure board %s_%u: %s", info[brd].ModelName, info[brd].SerialNumber, decodeError(codeStr, ret)); return 0;}
    else
        printf("Board Configured!\n");
            
for (ch = 0; ch < MAX_BOARD_CHNUM; ch++) ///start acq for active channels
{
  if (dgtzParams.ChannelMask & (1<<ch))
  ret = CAENDPP_StartAcquisition(handle, ch);
    if (ret != CAENDPP_RetCode_Ok)
        printf("Error Starting Acquisition for channel %d: %s", ch, decodeError(codeStr, ret));
    else 
        printf("Acquisition Started for channel %d", ch);
} 
/////Se qui si leggesse una memoria per decidere Oscilloscopio/Acq normale/HV???////////////////////

  /////caso acq////////////////////////////

/////Malloc///////
CAENDPP_ListEvent_t *listEvents; 
listEvents = (CAENDPP_ListEvent_t *)calloc(MAX_LIST_BUFF_NEV, sizeof(CAENDPP_ListEvent_t));//MAX_LIST_BUFF_NEV definito in CAENDPPLibTypes.h (vale 8192)
if (listEvents == NULL) {
        ret = - 1;
        {printf("Error allocating memory for lists\n"); goto QuitProgram; }
}


///////////////////////////
   for(;;)    ////READOUT
   {

        if(*(shared_memory_cmd+70)==0)
	   {
	    printf("shared_memory_cmd+70=0\n");
	   goto QuitProgram;	
	   }
        if(check)
	{
		if(*(shared_memory2+3)==1)
		{ 
			if(y%2==0)
			{
				  
	 			*(shared_memory2+10+n)= Xmin_adc+50000000;
				//printf("Xmin_adc:%d\n", Xmin_adc);
         			n++;
				*(shared_memory2+10+n)= PositionY_adc+60000000;
				//printf("PositionY_adc:%d\n", PositionY_adc);
         			n++;
				x++;
				//firstPos=false;
			}
			if(y%2!=0)
			{
				  
	 			*(shared_memory2+10+n)= Xmax_adc+50000000;
				//printf("Xmax_adc:%d\n", Xmax_adc);
         			n++;
				*(shared_memory2+10+n)= PositionY_adc+60000000;
				//printf("PositionY_adc:%d\n", PositionY_adc);
         			n++;
				x++;
			}
			
			check=false;
			
			*(shared_memory2+3)=0;	
			//printf("time_interval:%d\n", time_interval);		
			it_val.it_value.tv_sec = time_interval/1000;
			//printf("it_val.it_value.tv_sec:%d\n", it_val.it_value.tv_sec);
  			it_val.it_value.tv_usec = (time_interval*1000) % 1000000;
			//printf("it_val.it_value.tv_usec:%d\n", it_val.it_value.tv_usec);	
  			it_val.it_interval = it_val.it_value;
  			if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
    				perror("error calling setitimer()");
    				exit(1);}
			AcqRun=1; //printf("AcqRun=1\n");
		}
	}

			/* Calculate throughput and trigger rate (every second) */
       			CurrentTime = get_time();
        		ElapsedTime = CurrentTime - PrevRateTime; /* milliseconds */
        		if (ElapsedTime > 1000)
        		{
            		//system(CLEARSCR);
           		//PrintInterface();
            		PrevRateTime = CurrentTime;
           		*(shared_memory_rate)=(eventi-stored_event); stored_event=eventi;
			}
       			gettimeofday(&tv,NULL);
         		millisec=tv.tv_usec;
        		printf ("time before board ReadData:%lu,%lu\n", tv, millisec);

if(AcqRun==1)
{

    for (ch = 0; ch < MAX_BOARD_CHNUM; ch++) 
     {
        if (!(dgtzParams.ChannelMask & (1<<ch)))
                    continue;
        CAENDPP_GetListEvents(handle,ch, listEvents, &nEvts);
        
           for(ev=0; ev<nEvts; ev++) 
            {
              if (listEvents[ev].Energy > 10 && listEvents[ev].Energy < 16000  && n<NSegFault) caso=1;     //Scrive le energie
              if(write_coordinates==true && x<Xn && (y%2==0)) caso=2;       //Cancella le prime energie
              if(write_coordinates==true && (x==Xn)) caso=3;    //Scrive le posizioni 
              if(write_coordinates==true && x<Xn && (y%2!=0))caso=4;
            }
     switch (caso)
     {
       case 1:
       {
       
	   data=listEvents[ev].Energy;
	   
		   eventi++; //numero di dati dell'adc
                   *(shared_memory2+10+n)=data;
                   n++;                                                       //numero totale dati in memoria (energie e posizioni)
                   if(n%500000==0 && n>0) {printf("Dati in memoria %d\n", n);   *(shared_memory2+4)=n;} 
		   *(shared_memory2+5)=eventi;
                   *(shared_memory+100+data)=*(shared_memory+100+data)+1;    //spettro x XRF2
		   gettimeofday(&tv,NULL);
         	   millisec=tv.tv_usec;
        	   printf ("time after writing data to memory:%lu,%lu\n", tv, millisec);
        caso=10;
        break;
        }
       case 2:  ///aggiunto per eliminare prime energie scritte...rimette n=1
       {
	x++;
	PositionX_adc=PositionX_adc+Xstep_adc;
        *(shared_memory2+10+n)= PositionX_adc+50000000;
	//printf("PositionX_adc:%d\n", PositionX_adc);
        n++;
        if(n%500000==0 && n>0) {printf("Dati in memoria %d\n", n);*(shared_memory2+4)=n;} 
	*(shared_memory2+10+n)= PositionY_adc+60000000;
	//printf("PositionY_adc:%d\n", PositionY_adc);
        n++;
	if(n%500000==0 && n>0) {printf("Dati in memoria %d\n", n);*(shared_memory2+4)=n;} 
        caso=10;
	write_coordinates=false;
        break;
        }
       case 3:                                                              //prende le posizioni dalla mem condivisa in cui le ha scritte 
       {                                                                    //motore.cpp e le scrive di seguito alle energie
	it_val.it_value.tv_sec = 0/1000;
  	it_val.it_value.tv_usec = (0*1000)/1000000;	
  	it_val.it_interval = it_val.it_value;
  	if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
    	perror("error calling setitimer()");
    	exit(1);}
	AcqRun=0;
	y++;
	x=0;
	PositionY_adc=PositionY_adc+Ystep_adc;
	write_coordinates=false;
	check=true;
	caso=10;	
	//printf("Changing direction...\n");
	*(shared_memory2+8)=1;//motori possono spostarea Y
	if(y==Yn+1) goto QuitProgram;	
	break;
        }
       case 4:
       {
	x++;
	PositionX_adc=PositionX_adc-Xstep_adc;
        *(shared_memory2+10+n)= PositionX_adc+50000000;
	//printf("PositionX_adc:%d\n", PositionX_adc);
        n++;
        if(n%500000==0 && n>0) {printf("Dati in memoria %d\n", n);*(shared_memory2+4)=n;} 
	*(shared_memory2+10+n)= PositionY_adc+60000000;
	//printf("PositionY_adc:%d\n", PositionY_adc);
        n++;
	if(n%500000==0 && n>0) {printf("Dati in memoria %d\n", n);*(shared_memory2+4)=n;} 
        caso=10;
	write_coordinates=false;
        break;     
       }

     }///chiude switch
 *(shared_memory2+4)=n;  

     }//chiude for ch

  }//chiude AcqRun
   }///chiude for ;;


QuitProgram:
    /* stop the acquisition, close the device and free the buffers */
    for (ch = 0; ch < MAX_BOARD_CHNUM; ch++) ///start acq for active channels
     {
       CAENDPP_AcqStatus_t acquiring;
       ret = CAENDPP_IsChannelAcquiring(handle,ch, &acquiring);
       if (ret != CAENDPP_RetCode_Ok)
        printf("Error reading acquisition status for channel %d: %s", ch, decodeError(codeStr, ret));
       if(acquiring)
       ret = CAENDPP_StopAcquisition(handle, ch);
       if (ret != CAENDPP_RetCode_Ok)
        printf("Error Stopping Acquisition for channel %d: %s", ch, decodeError(codeStr, ret));
       else 
        printf("Acquisition Sopped for channel %d", ch);
     } 

    ret = CAENDPP_EndLibrary(handle);
    if(ret)printf("Error ending CAENDPP Library! (err %d)\n", ret);
   printf("ntot_adcxrf:%d\n", *(shared_memory2+4));
    free(listEvents);
 exit(1);

} ///chiude main


void write_position()
{

	if(write_coordinates==false) write_coordinates=true;
	else printf("write_coordinates is already true\n");
	//printf("write_coordinates=true\n");
//return write_coordinates;
}


void sigquit()
{   
printf("..Exiting..\n\n");
 exit(1);
}

void termination(int pippo){

  printf("\n recorded events:%d\n",eventi);

  *(shared_memory2+4)=n;
  *(shared_memory_cmd+70)=0;
printf("..Exiting..on termination\n");
 exit(2);
}


char *decodeError(char *dest, int32_t code) {
    char codeStr[10];
    size_t nc = MAX_ERRMSG_LEN;
    sprintf(codeStr, " [%d]", code);
    nc -= strlen(codeStr);
    switch(code) {
    case CAENDPP_RetCode_Ok: strncpy(dest, "Success", nc); break;
    case CAENDPP_RetCode_GenericError: strncpy(dest, "Generic Error", nc); break;
    case CAENDPP_RetCode_TooManyInstances: strncpy(dest, "Too Many Instances", nc); break;
    case CAENDPP_RetCode_ProcessFail: strncpy(dest, "Process Fail", nc); break;
    case CAENDPP_RetCode_ReadFail: strncpy(dest, "Read Fail", nc); break;
    case CAENDPP_RetCode_WriteFail: strncpy(dest, "Write Fail", nc); break;
    case CAENDPP_RetCode_BadMessage: strncpy(dest, "Bad Message", nc); break;
    case CAENDPP_RetCode_InvalidHandle: strncpy(dest, "Invalid Handle", nc); break;
    case CAENDPP_RetCode_ConfigError: strncpy(dest, "Configuration Error", nc); break;
    case CAENDPP_RetCode_BoardInitFail: strncpy(dest, "Board Initialization Failed", nc); break;
    case CAENDPP_RetCode_TimeoutError: strncpy(dest, "Timeout Error", nc); break;
    case CAENDPP_RetCode_InvalidParameter: strncpy(dest, "Invalid Parameter", nc); break;
    case CAENDPP_RetCode_NotInWaveMode: strncpy(dest, "Not in Waveform Mode", nc); break;
    case CAENDPP_RetCode_NotInHistoMode: strncpy(dest, "Not in Histogram Mode", nc); break;
    case CAENDPP_RetCode_NotInListMode: strncpy(dest, "Not in List Mode", nc); break;
    case CAENDPP_RetCode_NotYetImplemented: strncpy(dest, "Not Yet Implemented", nc); break;
    case CAENDPP_RetCode_BoardNotConfigured: strncpy(dest, "Board not Configured", nc); break;
    case CAENDPP_RetCode_InvalidBoardIndex: strncpy(dest, "Invalid Board Index", nc); break;
    case CAENDPP_RetCode_InvalidChannelIndex: strncpy(dest, "Invalid Channel Index", nc); break;
    case CAENDPP_RetCode_UnsupportedFirmware: strncpy(dest, "Unsupported Firmware", nc); break;
    case CAENDPP_RetCode_NoBoardsAdded: strncpy(dest, "No Boards Added", nc); break;
    case CAENDPP_RetCode_AcquisitionRunning: strncpy(dest, "Acquisition Running", nc); break;
    case CAENDPP_RetCode_OutOfMemory: strncpy(dest, "Out of Memory", nc); break;
    case CAENDPP_RetCode_BoardChannelIndex: strncpy(dest, "Invalid Board Channel Index", nc); break;
    case CAENDPP_RetCode_HistoAlloc: strncpy(dest, "Histogram Allocation Failed", nc); break;
    case CAENDPP_RetCode_OpenDumper: strncpy(dest, "Dumper Open Failed", nc); break;
    case CAENDPP_RetCode_BoardStart: strncpy(dest, "Board Start Failed", nc); break;
    case CAENDPP_RetCode_ChannelEnable: strncpy(dest, "Channel not Enabled", nc); break;
    case CAENDPP_RetCode_InvalidCommand: strncpy(dest, "Invalid Command", nc); break;
    case CAENDPP_RetCode_NumBins: strncpy(dest, "Invalid Number of Bins", nc); break;
    case CAENDPP_RetCode_HistoIndex: strncpy(dest, "Invalid Histogram Index", nc); break;
    case CAENDPP_RetCode_UnsupportedFeature: strncpy(dest, "Unsupported Feature", nc); break;
    case CAENDPP_RetCode_BadHistoState: strncpy(dest, "Incompatible Histogram Status", nc); break;
    case CAENDPP_RetCode_NoMoreHistograms: strncpy(dest, "No more Histograms Available", nc); break;
    case CAENDPP_RetCode_NotHVBoard: strncpy(dest, "Not an HV Board", nc); break;
    case CAENDPP_RetCode_InvalidHVChannel: strncpy(dest, "Invalid HV Channel", nc); break;
    case CAENDPP_RetCode_SocketSend: strncpy(dest, "Socket Send Error", nc); break;
    case CAENDPP_RetCode_SocketReceive: strncpy(dest, "Socket Receive Error", nc); break;
    case CAENDPP_RetCode_BoardThread: strncpy(dest, "Board Acquisition Thread Error", nc); break;
    case CAENDPP_RetCode_DecodeWaveform: strncpy(dest, "Cannot Decode Waveform from Buffer", nc); break;
    case CAENDPP_RetCode_OpenDigitizer: strncpy(dest, "Error Opening the digitizer", nc); break;
    case CAENDPP_RetCode_BoardModel: strncpy(dest, "Unsupported Board Model", nc); break;
    case CAENDPP_RetCode_AutosetStatus: strncpy(dest, "Incompatible Autoset Status", nc); break;
    case CAENDPP_RetCode_Autoset: strncpy(dest, "Autoset Error", nc); break;
    default: strncpy(dest, "UNKNOWN Error", nc);
    }
    strcat(dest, codeStr);
    return dest;
}






























