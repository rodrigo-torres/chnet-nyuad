/** MAXRF_DAQ: Manage XRF DAQ sessions with the CHNET built MAXRF Scanner
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

#include "daq_director.h"
#include "mca.h"
#include "file_writer.h"

namespace maxrf::daq {

bool DAQSession::SetupDAQSession(DAQInitParameters const & config) {
  bool status {false};

  try {
    caen_handle_ = std::make_unique<CAENLibraryHandle>();

    auto writer_ = std::make_shared<FileWriter>();
    writer_->Initialize(config);

    for (auto & single_board_config : config.boards_config) {
      auto ptr = caen_handle_->GetBoardHandle(single_board_config);
      pipes_.push_back(DAQPipe {ptr, writer_});
    }
    status = true;
  } catch (std::exception & e) {
    std::cout << e.what() << std::endl;
  }
  daq_duration = std::chrono::duration<double>(config.timeout);
  mode_ = config.mode;
  safe_to_stop.store(false);

  return status;
}

void DAQSession::StartDAQSession() {

  switch (mode_) {
  case DAQMode::kDAQScan :
    // Set up the extra stuff for the scan
    [[fallthrough]];
  case DAQMode::kDAQPoint :
    daq_enable.store(true);
    EnterDAQLoop();
    break;
  case DAQMode::kDAQInvalid :
    break;
  }
}

void DAQSession::EnterDAQLoop() {
  using namespace std::chrono;

  duration<double> update_interval {1.};

  pipes_.front().OpenPipe();
  auto start_timestamp = steady_clock::now();
  while (daq_enable) {
    std::this_thread::sleep_for(update_interval);

    pipes_.front().SendDataDownPipe();

    if (steady_clock::now() - start_timestamp > daq_duration) {
      daq_enable = false;
    }
  }


  CleanupEverything();
}

inline void DAQSession::CleanupEverything() {
  mode_ = DAQMode::kDAQInvalid;
  pipes_.clear();

  // Handle semaphores
  if (sem_reply) {
    sem_close(sem_reply);
  }
  if (sem_probe) {
    sem_close(sem_probe);
  }

  safe_to_stop.store(true);
}

} //namespace maxrf::daq
