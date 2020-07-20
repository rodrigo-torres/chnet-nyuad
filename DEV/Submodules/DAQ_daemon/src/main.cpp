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


int main()
{
  //  ResourceCleaner safety {SafeTermination};
  return StartFromSocket();
//  return HardcodedValues();
}

int StartFromSocket() {
  using namespace maxrf::daq;
  using namespace maxrf::ipc;

  SHMHandle shm;
  shm.Open();

  auto start = std::chrono::steady_clock::now();
  while (shm.GetVariable(&SHMStructure::daq_daemon_enable) == false) {
    std::this_thread::sleep_for(std::chrono::milliseconds {10});
    if (std::chrono::steady_clock::now() - start > std::chrono::seconds {1}) {
      // Process waiting for DAQ enable from parent process timed out
      std::cout << "Waiting for DAQ enable flag timed out" << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }


  DAQInitParameters params {};
  IPCSocket socket{"/run/maxrf/daq", SocketOptions::kClient};

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
      socket.ReceiveFromSocket(&params.scan_parameters);
      socket.ReceiveFromSocket(&params.mode);
      socket.ReceiveFromSocket(&params.timeout);
      socket.ReceiveFromSocket(path);
      socket.ReceiveFromSocket(filename);
      params.output_path = std::string {path};
      params.base_filename = std::string {filename};
      done = true;
      break;
    }
    default:
      std::cout << "Error in parsing" << std::endl;
      return 1;
    }
  } while (!done);

  std::cout << "Done!" << std::endl;

  DAQSession daq{};
  if (daq.SetupDAQSession(params)) {
    daq.StartDAQSession();
    do {
      std::this_thread::sleep_for(std::chrono::seconds{1});
    } while (!daq.IsSafeToExit());


    return 0;
  }
  else {
    return 1;
  }
}

int HardcodedValues() {
  using namespace maxrf::daq;
  DAQInitParameters params;

  params.boards_config.push_back( MCABoardConfiguration {});
  params.boards_config.front().channels.insert( {0, MCAChannelParameters {}});
  params.base_filename = "test";
  params.output_path =  ".";
  params.mode = DAQMode::kDAQPoint;
  params.timeout = 15.;

  DAQSession daq{};
  if (daq.SetupDAQSession(params)) {
    daq.StartDAQSession();
    do {
      std::this_thread::sleep_for(std::chrono::seconds{1});
    } while (!daq.IsSafeToExit());


    return 0;
  }
  else {
    return 1;
  }
}
