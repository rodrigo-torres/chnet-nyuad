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

#ifndef SRC_DIGITIZER_H_
#define SRC_DIGITIZER_H_

#include <atomic>
#include <memory>
#include <filesystem>
#include <vector>

// POSIX libraries
#include <semaphore.h>	// Semaphore with main program

#include "daq_types.h"
#include "file_writer.h"
#include "mca.h"

namespace maxrf::daq {

class DAQPipe
{
  std::shared_ptr<CAEN_MCAHandle> producer_ {nullptr};
  std::shared_ptr<FileWriter>     consumer_ {nullptr};
  bool pipe_open_ {false};

public:
  DAQPipe() = delete;
  DAQPipe(std::shared_ptr<CAEN_MCAHandle> producer,
          std::shared_ptr<FileWriter> consumer) :
    producer_ {producer}, consumer_ {consumer} {}

  ~DAQPipe() {
    ClosePipe();
  }

  void OpenPipe() {
    if (!pipe_open_) {
      producer_->StartAcquisition();
      pipe_open_ = true;
    }
  }

  void SendDataDownPipe() {
    if (!pipe_open_) {
      return;
    }

    producer_->UpdateHistograms();

    for (auto & channel : producer_->GetHandles()) {
      auto packet = consumer_->GetEmptyDataPacket();
      packet.channel_id = channel.id;
      packet.m_buffer.swap(channel.histogram);

      consumer_->PushDataPacketToQueue(std::move(packet));
    }

  }

  void ClosePipe() {
    if (pipe_open_) {
      producer_->StopAcquisition();
      pipe_open_ = false;
    }
    producer_.reset();
    consumer_.reset();
  }
};

///
/// \brief The DAQBridge class manages a DAQ session and connects the handle for
/// the CAEN MCA with the class responsible for writing the data file
///
class DAQSession
{
public:
  DAQSession() = default;
  DAQSession(DAQInitParameters const & config) {
    if (SetupDAQSession(config) == false) {
      std::exit(1);
    }
  }
  ~DAQSession() { CleanupEverything(); }

  bool SetupDAQSession(DAQInitParameters const & config);

  ///
  /// \brief StartDAQSession
  ///
  void StartDAQSession();

  inline void ForceStopDAQ() {
    if (daq_enable.load() == false) {
      safe_to_stop.store(true);
    }
    daq_enable.store(false);
  }

  inline bool IsSafeToExit() {
    return safe_to_stop.load();
  }


protected:

  void EnterDAQLoop();
  void CleanupEverything();

private:
  std::vector<DAQPipe>  pipes_  {};
  std::unique_ptr<CAENLibraryHandle> caen_handle_ {nullptr};

  DAQMode mode_ {DAQMode::kDAQInvalid};
  std::chrono::duration<double> daq_duration {0.0};

  // DAQ flags
  std::atomic_bool daq_enable {false};
  std::atomic_bool safe_to_stop {false};

  // DAQ semaphores for the scan daq mode
  sem_t * sem_reply {nullptr};
  sem_t * sem_probe {nullptr};
};
}


#endif /* SRC_DIGITIZER_H_ */
