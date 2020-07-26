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
#include <thread>
#include <vector>

// POSIX libraries
#include <semaphore.h>	// Semaphore with main program

#include "MAXRF/ipc_methods.h"
#include "daq_types.h"
#include "mca.h"

namespace maxrf::daq {

class DAQPipe;
class DAQSession;
class FileWriter;

///
/// \brief The DAQSession class manages a DAQ session.
///
class DAQSession
{
public:
  ~DAQSession() {
    CleanupEverything();
  }

  ///
  /// \brief SetupDAQSession
  /// \param config
  /// \return a boolean value, true if the configuration was succesful, and
  /// false otherwise.
  ///
  bool SetupDAQSession(DAQInitParameters const & config);

  ///
  /// \brief StartDAQSession
  ///
  void StartDAQSession();

protected:

  void EnterDAQLoop();
  void CleanupEverything();

private:
  std::vector<DAQPipe>  pipes_  {};
  std::unique_ptr<CAENLibraryHandle> caen_handle_ {nullptr};

  DAQModeParameters session_params_  {};
  std::array<int, 3> image_dimensions_ {};

  // DAQ flags
  std::atomic_bool daq_enable {false};

  // DAQ semaphores for the scan daq mode
  sem_t * sem_reply {nullptr};
  sem_t * sem_probe {nullptr};
};

class DAQPipe
{
  std::shared_ptr<CAEN_MCAHandle> producer_ {nullptr};
  std::shared_ptr<FileWriter>     consumer_ {nullptr};
  bool pipe_open_ {false};
  uint32_t pixel {0};
  uint32_t pipe_id {0};

public:
  DAQPipe() = delete;
  DAQPipe(std::shared_ptr<CAEN_MCAHandle> producer,
          std::shared_ptr<FileWriter> consumer) :
    producer_ {producer}, consumer_ {consumer} {
  }

  ~DAQPipe() {
    ClosePipe();
    producer_.reset();
    consumer_.reset();
  }

  void SetID(uint id) {
    pipe_id = id;
  }

  void OpenPipe() {
    if (!pipe_open_) {
      producer_->StartAcquisition();
      pipe_open_ = true;
    }
  }

  inline void CollectData() {
    producer_->UpdateHistograms();
  }

  void SendDataDownPipe();


  void ClosePipe() {
    if (pipe_open_) {
      producer_->StopAcquisition();
      pipe_open_ = false;
    }
  }
};

struct FileStats {
//  int32_t width;  ///< Scan width in pixels
//  int32_t height; ///< Scan height in pixels
  uint32_t pixels_in_buffer;   ///< No. of pixels in buffer but not written
  uint32_t pixels_written;     ///< No. of pixels written
  SpectralData sum_spectrum;  ///< Sum spectrum of the scan for this detector
};

class FileWriter
{
  using Job = DataPacket;
  using FileHande = std::shared_ptr<maxrf::HypercubeFile>;

public:
  FileWriter();
  ~FileWriter() {
    daq_enable = false;
    StopWritingTask();
  }

  void Initialize(DAQInitParameters const & config);

  ///
  /// \brief PushDataPacketToQueue thread-safe
  /// \param packet
  ///
  inline void PushDataPacketToQueue(DataPacket && packet) {
    pending_jobs.push(std::move(packet));
  }

  ///
  /// \brief GetEmptyDataPacket thread-safe
  ///
  inline auto GetEmptyDataPacket() {
    return empty_packets_pool.pop();
  }




private:
  void TimestampFiles();
  void WriteTask();
  void WriteDataToFiles(Job & packet);

  inline auto BeginWritingTask() {
    daq_enable = true;
    if (!writing_task_.joinable()) {
      TimestampFiles();
      writing_task_ = std::thread(&FileWriter::WriteTask, this);
    }
  }

  inline void StopWritingTask() {
    daq_enable = false;
    if (writing_task_.joinable()) {
      writing_task_.join();
    }
  }


  std::thread writing_task_;
  std::atomic_bool daq_enable;
  maxrf::ipc::Queue<Job> pending_jobs;
  maxrf::ipc::Queue<Job> empty_packets_pool;

  DAQMode mode_ {DAQMode::kDAQInvalid};
  std::vector<FileHande> files_;
  uint32_t scan_line_width_;
};


}  // namespace maxrf::daq


#endif /* SRC_DIGITIZER_H_ */
