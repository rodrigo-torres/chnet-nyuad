/** MAXRF_Main: Main point of access to control the MAXRF instrument
 *  Copyright (C) 2020 Rodrigo Torres and contributors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"

template class maxrf::ipc::shmarray<int>;

//int shmid[8] = { 0 };
int * shared_memory_cmd {nullptr};
int * shared_memory     {nullptr};
maxrf::ipc::SHMHandle shm {};

//static int * assignSHM(key_t key, size_t size, int id) {
//    int shmID = shmget(key, size, IPC_CREAT | 0666);
//    shmid[id] = shmID;
//    if (shmID == -1) {
//        printf("[!] Couldn't obtain a shared memory segment ID for key:\t%d\n", key);
//        printf("%s\n", strerror(errno));
//        return nullptr;
//    }
//    else return static_cast<int *>(shmat(shmID, nullptr, 0));
//}

void MAXRF_UI::CreateSHMResources() {

  try {
    shm.Init();
  } catch (std::runtime_error & e) {
    std::cout << e.what() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds {1});
    std::exit(EXIT_FAILURE);
  }

  shm0_.initialize(maxrf::ipc::Segments::kSHMCommand);
  shm1_.initialize(maxrf::ipc::Segments::kSHMSpectra);

  shared_memory_cmd = shm0_.data();
  shared_memory     = shm1_.data();


//    key_t key_cmd = 6900, key = 7000;
//    shared_memory_cmd = assignSHM(key_cmd, 2048, 0);
//    shared_memory = assignSHM(key, 409600, 1);

    ////////////////   SHARED MEMORY STARTING CONFIGURATION   ///////////////////////////////////////


    shared_memory_cmd[70] = 0;                   /// VME/ADCXRF STATUS (default=0)
    shared_memory_cmd[71] = 0;                   /// XRF SPECTRUM STATUS (default=0)
    shared_memory_cmd[73] = 0;                   /// Rate mater STATUS (default=0)
    shared_memory_cmd[74] = 0;                   /// Xray Table STATUS (default=0)

    shared_memory_cmd[80] = 10000;               /// PID ADCXRF (default=10000)
    shared_memory_cmd[81] = 10000;               /// PID XRF SPECTRUM (default=10000)
    shared_memory_cmd[83] = 10000;               /// PID Rate Meter (default=10000)
    shared_memory_cmd[84] = 10000;               /// PID Xray Table (default=10000)

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


