/** MAXRF_DAQ: Manage XRF DAQ sessions with the CHNET built MAXRF Scanner
 *  Copyright (C) 2020 Rodrigo Torres (rodrigo.torres@nyu.edu) and contributors
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

#include <cstring>
#include <chrono>

#include "daq_daemon.h"
#include "MAXRF/ipc_methods.h"

extern "C" [[noreturn]] void SafeTermination([[maybe_unused]] int sig) {
  //  process_lock.ReleaseSocket();

  //  daq.ForceStopDAQ();
  //  while (daq.IsSafeToExit() != true) {
  //    std::this_thread::sleep_for(std::chrono::milliseconds {10});
  //  }

  std::cout << "Cleanup success!" << std::endl;
  std::_Exit(EXIT_FAILURE);
}

int HardcodedValues();
int StartFromSocket();

static maxrf::ipc::SHMHandle shm {};


int main()
{
  //  ResourceCleaner safety {SafeTermination};
//  auto size = sizeof(std::vector<std::byte>);
//  printf("%ld\n", size);
//  try {
//    shm.Init();
//  } catch (std::runtime_error & e) {
//    std::cout << e.what() << std::endl;
//    std::this_thread::sleep_for(std::chrono::milliseconds {500});
//    std::exit(EXIT_FAILURE);
//  }
//  int counter {0};
//  do {
//    StartFromSocket();
//    shm.WriteVariable(&maxrf::ipc::SHMStructure::daq_daemon_active, false);
//    ++counter;
//  } while (counter < 5);

//  return 0;
// return HardcodedValues();
  return StartFromSocket();
}

int StartFromSocket() {
  using namespace maxrf::daq;
  using namespace maxrf::ipc;
  using namespace std::chrono;
  using namespace std::chrono_literals;

  std::cout << "Daemon started" << std::endl;
  shm.Init();
  shm.WriteVariable(&maxrf::ipc::SHMStructure::daq_daemon_active, true);


//  auto start = std::chrono::steady_clock::now();
//  while (shm.GetVariable(&SHMStructure::daq_daemon_enable) == false) {
//    std::this_thread::sleep_for(10ms);
//    if (std::chrono::steady_clock::now() - start > 1s) {
//      // Process waiting for DAQ enable from parent process timed out
//      std::cout << "Waiting for DAQ enable flag timed out" << std::endl;
//      std::this_thread::sleep_for(2s);
//      std::exit(EXIT_FAILURE);
//    }
//  }


  DAQInitParameters params {};
  IPCSocket socket {"/run/maxrf/daq"};
  std::cout << "Opening socket" << std::endl;
  try {
    socket.StartSocket(SocketOptions::kClient);
  } catch (std::exception & e) {
    shm.WriteVariable(&maxrf::ipc::SHMStructure::daq_daemon_enable, false);
    shm.WriteVariable(&maxrf::ipc::SHMStructure::daq_daemon_active, false);
    std::cout << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }

  shm.WriteVariable(&maxrf::ipc::SHMStructure::daq_daemon_active, true);

  char command {0};
  bool done {false};
  do {
    socket.ReceiveFromSocket(command);
    switch (command)
    {
    case 'B':
      params.boards_config.push_back(MCABoardConfiguration {});
      socket.ReceiveFromSocket(&params.boards_config.back().conn);
      socket.ReceiveFromSocket(&params.boards_config.back().board_id);
      break;
    case 'C': {
      uint8_t id {0};
      MCAChannelParameters channel_params {};
      socket.ReceiveFromSocket(&id);
      socket.ReceiveFromSocket(&channel_params);
      params.boards_config.back().channels.insert({id, channel_params});
      break;
    }
    case 'K': {
      char key [256], val [256];
      socket.ReceiveFromSocket(key);
      socket.ReceiveFromSocket(val);
      params.user_header_fields.insert({std::string {key}, std::string {val}});
      break;
    }
    case 'E': {
      char path [256],  filename [256];
      socket.ReceiveFromSocket(&params.mode_parameters);
      socket.ReceiveFromSocket(path);
      socket.ReceiveFromSocket(filename);
      params.output_path = std::string {path};
      params.base_filename = std::string {filename};
      done = true;
      break;
    }
    default:
      shm.WriteVariable(&maxrf::ipc::SHMStructure::daq_daemon_enable, false);
      std::cout << "Error in parsing" << std::endl;
      return 1;
    }
  } while (!done);

  std::cout << "Done!" << std::endl;

  DAQSession daq{};
  if (daq.SetupDAQSession(params)) {
    daq.StartDAQSession();
  }
  else {
    std::cout << "DAQ Session parameters are invalid" << std::endl;
  }
  shm.WriteVariable(&maxrf::ipc::SHMStructure::daq_daemon_enable, false);
  return 0;
}

int HardcodedValues() {
  using namespace std::literals;
  using namespace maxrf::daq;
  DAQInitParameters params;

  params.boards_config.push_back( MCABoardConfiguration {});
  params.boards_config.front().channels.insert( {1, MCAChannelParameters {}});
  params.boards_config.front().board_id = 0;
  params.base_filename = "test";
  params.output_path =  ".";
  params.mode_parameters.mode     = DAQMode::kDAQScan;
  params.mode_parameters.x_motor_step = 1000;
  params.mode_parameters.y_motor_step = 1000;
  params.mode_parameters.motor_velocity = 5000;
  params.mode_parameters.x_start_coordinate = 0;
  params.mode_parameters.x_end_coordinate = 10000;
  params.mode_parameters.y_start_coordinate = 0;
  params.mode_parameters.y_end_coordinate   = 5000;
  params.mode_parameters.timeout  = 0.2;

//  int counter {0};
//  do {
    DAQSession daq{};
//    params.base_filename = "test"s + std::to_string(counter);
      if (daq.SetupDAQSession(params)) {
      daq.StartDAQSession();
    }
//    ++counter;
//  } while (counter < 30);
  return 0;
}
