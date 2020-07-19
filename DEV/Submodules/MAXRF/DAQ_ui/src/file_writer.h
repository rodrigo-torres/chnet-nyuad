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

#ifndef BUFFER_H_
#define BUFFER_H_

#include <array>
#include <chrono>				// Clock for debug purposes
#include <condition_variable>	// notify
#include <filesystem>
#include <memory>				// Smart pointers
#include <sstream>
#include <string>
#include <thread>				// Event-loop thread
#include <vector>

#include "daq_types.h"
#include "safe_queue.h"

namespace maxrf::daq {

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
  Queue<Job> pending_jobs;
  Queue<Job> empty_packets_pool;

  DAQMode mode_ {DAQMode::kDAQInvalid};
  std::vector<FileHande> files_;
  std::vector<std::stringstream> line_buffer_;
};

}	// namespace daq_writer

#endif /* BUFFER_H_ */
