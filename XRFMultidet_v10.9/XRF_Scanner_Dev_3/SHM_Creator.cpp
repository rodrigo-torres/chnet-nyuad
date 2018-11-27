#include <mainwindow.h>
#include <../Header.h>
#include <../Shm.h>

int shmid[8] = { 0 };
key_t key, key2, key3, key4, key5, key_cmd, key_rate, key_laser;
int *shared_memory, *shared_memory2, *shared_memory3, *shared_memory4;
char*shared_memory5;
int *shared_memory_cmd, *shared_memory_rate, *shared_memory_laser;

extern int MotoreWindowStatus, CentralWindowStatus;
extern int portX, portY, portZ, IniX, IniY, IniZ, IniXready, IniYready, IniZready, SerialiStatus, pixel_Xstep, pixel_Ystep, PassoZ;

template <typename T> T* assignSHM(key_t key, size_t size, int id) {
    int shmID = shmget(key, size, IPC_CREAT | 0666);
    shmid[id] = shmID;
    if (shmID == -1) {
        printf("[!] Couldn't obtain a shared memory segment ID for key:\t%d\n", key);
        printf("%s\n", strerror(errno));
        return nullptr;
    }
    else return static_cast<T*>(shmat(shmID, nullptr, 0));
}

void MainWindow::SHM_CREATOR() {

    key_cmd = 6900, key = 7000, key2 = 7200, key3 = 7300, key4 = 7400, key5 = 8000;
    key_rate = 7500, key_laser = 7600;

    shared_memory_cmd = assignSHM<int>(key_cmd, SHMSZ_CMD_STATUS, 0);
    shared_memory = assignSHM<int>(key, SHMSZ, 1);
    shared_memory2 = assignSHM<int>(key2, SHMSZBIG, 2);
    shared_memory3 = assignSHM<int>(key3, SHMSZHISTO, 3);
    shared_memory4 = assignSHM<int>(key4, SHMSZDIGI, 4);
    shared_memory5 = assignSHM<char>(key5, 4096, 5);
    shared_memory_rate = assignSHM<int>(key_rate, SHMSZRATE, 6);
    shared_memory_laser = assignSHM<int>(key_laser, 4096, 7);


    ////////////////   SHARED MEMORY STARTING CONFIGURATION   ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////// REDEFINED (DEC-2016) SHM_CMD INTRODUCED   ///////////////////////////////////////

                    *shared_memory_cmd=MotoreWindowStatus;       /// XY Motor status
                    *(shared_memory_cmd+1)=MotoreWindowStatus;   /// Z Motor status
                    *(shared_memory_cmd+2)=SerialiStatus;        /// not used

                    *(shared_memory_cmd+10)=portX;               /// X Port (default=0) initialization in variables.h
                    *(shared_memory_cmd+11)=portY;               /// Y Port (default=1)
                    *(shared_memory_cmd+12)=portZ;               /// Z Port (default=2)

                    *(shared_memory_cmd+20)=IniX;                /// X motor not inited (default=0) initialization in variables.h
                    *(shared_memory_cmd+21)=IniY;                /// Y motor not inited (default=0)
                    *(shared_memory_cmd+22)=IniZ;                /// Z motor not inited (default=0)

                    *(shared_memory_cmd+30)=IniXready;           /// X motor status (default=0) initialization in variables.h
                    *(shared_memory_cmd+31)=IniYready;           /// Y motor status (default=0)
                    *(shared_memory_cmd+32)=IniZready;           /// Z motor status (default=0)

                    *(shared_memory_cmd+40)=0;                   /// X[point] Position (default=0)
                    *(shared_memory_cmd+41)=0;                   /// Y[point] Position (default=0)
                    *(shared_memory_cmd+42)=0;                   /// Z Position (default=0)
                    *(shared_memory_cmd+43)=0;                   /// Integral[point] (default=0)

                    *(shared_memory_cmd+50)=100000;                   /// Xmin (default=0)
                    *(shared_memory_cmd+51)=110000;                   /// Xmax (default=0)
                    *(shared_memory_cmd+52)=100000;                   /// Ymin (default=0)
                    *(shared_memory_cmd+53)=110000;                   /// Ymax (default=0)
                    //*(shared_memory_cmd+54)=0;                   /// Zmin (default=0)
                    //*(shared_memory_cmd+55)=0;                   /// Zmax (default=0)

                    *(shared_memory_cmd+60)=pixel_Xstep;              /// X step (default=0)
                    *(shared_memory_cmd+61)=pixel_Ystep;              /// Y step (default=0)
                    //*(shared_memory_cmd+62)=PassoZ;              /// Z step (default=0)
                    *(shared_memory_cmd+64)=0;                   /// X movement (default=0)
                    *(shared_memory_cmd+65)=0;                   /// Y movement (default=0)
                    *(shared_memory_cmd+66)=0;                   /// Z movement (default=0)
                    *(shared_memory_cmd+67)=1000;                   /// Scan velocity (default=1 mm/s)

                    *(shared_memory_cmd+70)=0;                   /// VME/ADCXRF STATUS (default=0)
                    *(shared_memory_cmd+71)=0;                   /// XRF SPECTRUM STATUS (default=0)
                    *(shared_memory_cmd+72)=0;                   /// Digitiser_interface STATUS (default=0)
                    *(shared_memory_cmd+73)=0;                   /// Rate mater STATUS (default=0)
                    *(shared_memory_cmd+74)=0;                   /// Xray Table STATUS (default=0)
                    *(shared_memory_cmd+75)=0;                   /// OnLineMap STATUS (default=0)
                    *(shared_memory_cmd+76)=0;                   /// Motor test tool STATUS (default=0)
                    *(shared_memory_cmd+77)=0;                   /// PI parameter table STATUS (default=0)

                    *(shared_memory_cmd+80)=10000;               /// PID ADCXRF (default=10000)
                    *(shared_memory_cmd+81)=10000;               /// PID XRF SPECTRUM (default=10000)
                    *(shared_memory_cmd+82)=10000;               /// PID Digitiser_interface (default=10000)
                    *(shared_memory_cmd+83)=10000;               /// PID Rate Meter (default=10000)
                    *(shared_memory_cmd+84)=10000;               /// PID Xray Table (default=10000)
                    *(shared_memory_cmd+85)=10000;               /// PID OnLineMap (default=10000)
                    *(shared_memory_cmd+86)=10000;               /// PID Motor test tool STATUS (default=10000)
                    *(shared_memory_cmd+87)=10000;               /// PID PI parameter table (default=10000)

                    *(shared_memory_cmd+100)=0;                  /// Used for multidetector version, if 0, then the digitizer's channel 0 is active, 										if 1 then channel 1 is active, if 2 then both are active and we are working 										with the sum of both
                    *(shared_memory_cmd+101)=1;                  /// Multidetector angular (A) calibration parameter (for sum mode)
                    *(shared_memory_cmd+102)=0;                  /// Multidetector linear (B) calibration parameter (for sum mode)
                    *(shared_memory_cmd+103)=1;                  /// Multidetector scale factor calibration parameter (number to which parameters A and B parameters must be divided since shared memory does not accept double values)
                    *(shared_memory_cmd+105)=0;
                    *(shared_memory_laser+5)=0;
                    *(shared_memory_laser+10)=20;
                    *(shared_memory_laser+20)=0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////


                    *(shared_memory+13)=0;          //...News per mappa online
                    *(shared_memory+19)=0;         //Avviso spostamento motore
                    *(shared_memory+22)=0;          //MergePos x sommare diverse scansioni
                    *(shared_memory+24)=0;        //Spettro in canale o energia (0 canale) (1 energia)


                    *(shared_memory+39)=0;  //Rate meter
                    *(shared_memory+40)=10; //Low energy cut


                    *(shared_memory+42)=0; //MAP_ON_LINE: News
                    *(shared_memory+43)=0; //MAP_ON_LINE: Integral
                    *(shared_memory+44)=0; //MAP_ON_LINE: X[point]
                    *(shared_memory+45)=0; //MAP_ON_LINE: Y[point]




                    *(shared_memory+99)=0;           //...News per show pixel histo
                    *(shared_memory+100)=0;          //Da questo punto inizia il vettore da plottare per mostrare lo spettro



                    *(shared_memory2+1)=0;    // Posiz X
                    *(shared_memory2+2)=0;    // Posiz Y
                    *(shared_memory2+3)=0;    // ontarget if 1
                   *(shared_memory2+4)=0;    // NEvents totali ADC per mappa online


                    *(shared_memory2+8)=0; //usato per indicare timer stop prime di cambiare righa
                    *(shared_memory2+9)=0; //usato quando l'acquisizione viene chiamata senza scanzione
	            *(shared_memory2+10)=0;	//From this position onwards, starts the map data array from
////////////////////////////////////////////// DIGITISER PARAMETERS


                     *(shared_memory4)=40;        // Trigger Threshold                                   
                     *(shared_memory4+1)=0;       // Input range digitalizzatore (0 -> 0.6V // 1 -> 1.4V                                  
                     *(shared_memory4+2)=800;     // Trapezoid Rise Time (ns) 
                     *(shared_memory4+3)=2500;    // Trapezoid Flat Top  (ns)
                     *(shared_memory4+4)=12000;   // Decay Time Constant (ns) HACK-FPEP the one expected from fitting algorithm?
                     *(shared_memory4+5)=1500;    // Peaking delay  (ns)
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

}

void MainWindow::SHM_delete() {
    qDebug()<<"!!!";

}


