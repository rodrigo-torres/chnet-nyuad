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

#include "file_writer.h"

namespace maxrf::daq {

FileWriter::FileWriter() {}


void FileWriter::Initialize(DAQInitParameters const & config) {
  using namespace std::filesystem;
  std::error_code err;

  path file_path {config.output_path};
  if (!exists(file_path, err)) {
    // We attempt to create the directory
    if (!create_directory(file_path, err)) {
      throw std::runtime_error(err.message());
    }
  }
  file_path /= config.base_filename;

  std::stringstream filename;
  for (auto & board : config.boards_config) {
    for (auto & channel : board.enabled_channels) {
      // Prepare the filename based  upon the base name
      filename.str("");
      filename << config.base_filename << '_'
               << "b" << std::to_string(board.board_id) << '_'
               << "c" << std::to_string(channel.first) << ".hyperc";
      file_path.replace_filename(filename.str());

      // Ask for a pointer to a File Handle
      auto ptr = maxrf::DataFileHander::GetFileForDAQ(file_path.c_str());
      if (ptr->GetFormat() != DataFormat::kHypercube) {
        throw std::runtime_error("Can't");
      }
      files_.push_back(std::static_pointer_cast<maxrf::HypercubeFile>(ptr));
      auto & file = files_.back();
      file->MakeDefaultHeader();

      // Fill-in the missing header entries
      file->EditToken("Name", "Test");
      file->EditToken("Description", "DAQ tests with new CAENDPPLib");

      file->EditToken("Calibration_Param_0", "0.00");
      file->EditToken("Calibration_Param_1", "0.0113");
      file->EditToken("Calibration_Param_2", "0.00");

      WriteMode mode = static_cast<WriteMode>(static_cast<int>(config.mode));
      file->SetWriteMode(mode);
    }
  }

  constexpr int kEmptyBuffersSize = 10;
  for (auto _ = kEmptyBuffersSize; _--; ) {
    DataPacket packet {};
    packet.m_buffer.resize(kSpectralBins, 0);
    empty_packets_pool.push(std::move(packet));
  }

  mode_ = config.mode;

  BeginWritingTask();
}

void FileWriter::TimestampFiles() {
  std::stringstream sstream;

  using std::chrono::system_clock;
  auto t_c = system_clock::to_time_t(system_clock::now());
  sstream << std::put_time(std::localtime(&t_c), "%Y-%m-%d %H-%M-%S");

  for (auto & file : files_) {
    file->EditToken(HeaderTokens::kDAQStartTimestamp, sstream.str());
    file->WriteHeader();
  }
}

void FileWriter::WriteTask() {
  Job current_job;

  while (daq_enable || !pending_jobs.empty()) {
    if (pending_jobs.try_pop(current_job)) {
      //        using namespace std::chrono;
      //        auto t1 = high_resolution_clock::now();
      WriteDataToFiles(current_job);
      //        auto t2 = high_resolution_clock::now();
      //        std::cout << duration_cast<milliseconds>(t2 - t1).count() << std::endl;
      // For writing spectra is only a couple of microseconds
    }
  }

  for (auto & file_handle : files_) {
    auto & file = file_handle->DataFile();
    file << "</Analysis_Data>\n";
    // Perhaps write a footer
    file << "</XRFAnalysis>\n";
  }
}

void FileWriter::WriteDataToFiles(Job & packet)
{
  auto ResetDataPacket = [] (Job & _p) {
    _p.pixel_no   = std::numeric_limits<int32_t>::max();
    _p.channel_id = std::numeric_limits<int32_t>::max();

    auto begin = reinterpret_cast<char *>(&(*_p.m_buffer.begin()));
    auto end   = reinterpret_cast<char *>(&(*_p.m_buffer.end()));
    std::fill(begin, end, '\0');
  };
  // For now we only write channel 0 of the MCA
  auto & file_object = files_.at(packet.channel_id);

  switch (mode_) {
  case DAQMode::kDAQScan :
    break;
  case DAQMode::kDAQPoint :
    file_object->WritePixel(packet.m_buffer);
    break;
  case DAQMode::kDAQInvalid :
    break;
  }

  ResetDataPacket(packet);
  empty_packets_pool.push(std::move(packet));
}


}	// namespace daq_writer
