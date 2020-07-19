#include "MAXRF/mainwindow.h"

int shmid[8] = { 0 };
int * shared_memory_cmd;
int * shared_memory;
//double*shared_memory5;

static int * assignSHM(key_t key, size_t size, int id) {
    int shmID = shmget(key, size, IPC_CREAT | 0666);
    shmid[id] = shmID;
    if (shmID == -1) {
        printf("[!] Couldn't obtain a shared memory segment ID for key:\t%d\n", key);
        printf("%s\n", strerror(errno));
        return nullptr;
    }
    else return static_cast<int *>(shmat(shmID, nullptr, 0));
}

void MainWindow::CreateSHMResources() {

    key_t key_cmd = 6900, key = 7000;
//    key_t key5 = 8000;

    shared_memory_cmd = assignSHM(key_cmd, SHMSZ_CMD_STATUS, 0);
    shared_memory = assignSHM(key, SHMSZ, 1);
//    shared_memory5 = assignSHM<double>(key5, 4096, 5);


    ////////////////   SHARED MEMORY STARTING CONFIGURATION   ///////////////////////////////////////


    shared_memory_cmd[70] = 0;                   /// VME/ADCXRF STATUS (default=0)
    shared_memory_cmd[71] = 0;                   /// XRF SPECTRUM STATUS (default=0)

//   shared_memory_cmd[72] = 0;                   /// Digitiser_interface STATUS (default=0)
    shared_memory_cmd[73] = 0;                   /// Rate mater STATUS (default=0)
    shared_memory_cmd[74] = 0;                   /// Xray Table STATUS (default=0)
    shared_memory_cmd[75] = 0;                   /// OnLineMap STATUS (default=0)
    shared_memory_cmd[76] = 0;                   /// Motor test tool STATUS (default=0)
    shared_memory_cmd[77] = 0;                   /// PI parameter table STATUS (default=0)

    shared_memory_cmd[80] = 10000;               /// PID ADCXRF (default=10000)
    shared_memory_cmd[81] = 10000;               /// PID XRF SPECTRUM (default=10000)
//    shared_memory_cmd[82] = 10000;               /// PID Digitiser_interface (default=10000)
    shared_memory_cmd[83] = 10000;               /// PID Rate Meter (default=10000)
    shared_memory_cmd[84] = 10000;               /// PID Xray Table (default=10000)
    shared_memory_cmd[85] = 10000;               /// PID OnLineMap (default=10000)
    shared_memory_cmd[86] = 10000;               /// PID Motor test tool STATUS (default=10000)
    shared_memory_cmd[87] = 10000;               /// PID PI parameter table (default=10000)

    shared_memory_cmd[100] = 0; // Active display channel for multidetector

    // Shared Memory CMD segment offsets 101->103 inclusive have been deprecated
    // in favor of energy calibration parameters

    shared_memory_cmd[200] = 0; // Motors emergency stop

    // To be deprecated with the new DAQ UI and DAQ daemon
    shared_memory_cmd[300] = 0; // DAQ on AND DAQ mode
    shared_memory_cmd[301] = 0; // DAQ duration in point mode
 //   This flag is deprecated
 //   shared_memory_cmd[400] = 0; // Override default parameters
    shared_memory_cmd[600] = 1; // Default rate for event count rate meter


    shared_memory[24] = 0;    // ADC bin or energy units
//    shared_memory[42] = 0;  // News for Online Map

    shared_memory[99] = 0;    // News from XRF image to spectrum display

    // XRF spectrum for channel 0
    memset(shared_memory + 100, '\0', 16384 * sizeof (int));
    // XRF spectrum for channel 1
    memset(shared_memory + 20000, '\0', 16384 * sizeof (int));
    // XRF spectrum for sum of channels 0 and 1
    memset(shared_memory + 40000, '\0', 16384 * sizeof (int));


    // Shared Memory segments 2 and 3 have been deprecated
    // Only OnlineMap still uses them so TODO fix it

    // Shared Memory segment 4 has been deprecated
    // Shared Memory segment 4 flag, at offset 15 (enable digitiser reading)
    // was remapped to shared memory CMD segment at offset 400


    // Shared Memory 5 has been deprecated
//    *(shared_memory5) = 100.;       // Scan x min. limit
//    *(shared_memory5+1) = 100.;     // Scan x max. limit
//    *(shared_memory5+2) = 100.;     // Scan y min. limit
//    *(shared_memory5+3) = 100.;     // Scan y max. limit
//    *(shared_memory5+4) = 1.;       // Scan x step
//    *(shared_memory5+5) = 1.;       // Scan y step
//    *(shared_memory5+6) = 1.;       // Scan velocity

    // Shared Memory Rate segment has been deprecated
}


