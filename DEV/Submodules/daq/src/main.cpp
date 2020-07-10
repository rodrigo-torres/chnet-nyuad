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

#include "daq_director.h"
#include "process_safety.h"

static SocketServer process_lock {"/run/maxrf/daq.sock"};
static maxrf::daq::DAQSession daq;

extern "C" [[noreturn]] void SafeTermination([[maybe_unused]] int sig) {
//  process_lock.ReleaseSocket();

  daq.ForceStopDAQ();
  while (daq.IsSafeToExit() != true) {
    std::this_thread::sleep_for(std::chrono::milliseconds {10});
  }

  std::cout << "Cleanup success!" << std::endl;
  std::_Exit(EXIT_FAILURE);
}

int main()
{
//  ResourceCleaner safety {SafeTermination};

//  struct sigaction action;
//  action.sa_flags = 0;
//  sigfillset(&action.sa_mask);

//  sigaction(SIGTERM, &action, nullptr);



  using namespace maxrf::daq;
  DAQInitParameters params;
  params.output_path = ".";
  params.base_filename = "test";
  params.boards_config.push_back(MCABoardConfiguration {});
  auto & board = params.boards_config.front();
  board.board_id = 0;
  board.enabled_channels.insert( {0, MCAChannelParameters {}} );
  board.connection = MCAConnectionParameters {};
  params.timeout = 10.;
  params.mode = DAQMode::kDAQPoint;

  if (daq.SetupDAQSession(params)) {
    // Start a point DAQ with 30 seconds duration
    daq.StartDAQSession();
    // Start a Scan DAQ with 1 second acquisition duration per pixel
    // Not yet implemented
//    daq.StartDAQSession(DAQMode::kDaqScan, 1.);
  }

  return 0;
}
