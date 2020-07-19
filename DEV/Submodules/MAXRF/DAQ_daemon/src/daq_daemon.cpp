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

#include "daq_daemon.h"

namespace maxrf::daq {

bool DAQSession::SetupDAQSession(DAQInitParameters const & config) {
  bool status {false};

  if (config.mode == DAQMode::kDAQInvalid) {
    return status;
  }

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
  scan_params_ = config.scan_parameters;
  mode_ = config.mode;
  safe_to_stop.store(false);

  return status;
}

void DAQSession::StartDAQSession() {

  switch (mode_) {
  case DAQMode::kDAQScan :
    image_dimensions_[0] =
        scan_params_.x_end_coordinate - scan_params_.x_start_coordinate;
    image_dimensions_[0] /= scan_params_.x_motor_step;
    image_dimensions_[1] =
        scan_params_.y_end_coordinate - scan_params_.y_start_coordinate;
    image_dimensions_[1] /= scan_params_.y_motor_step;
    image_dimensions_[2] = image_dimensions_[0] * image_dimensions_[1];


//    sem_probe = sem_open("/daq_probe", 0);
//    sem_reply = sem_open("/daq_reply", 0);

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

  int32_t pixel {0};
  auto & width = image_dimensions_[0];
  auto & height = image_dimensions_[1];
  auto & total_pixels = image_dimensions_[2];


//  int32_t samples {0};
//  microseconds micros {0};


  pipes_.front().OpenPipe();
  auto start_timestamp = steady_clock::now();
  while (daq_enable) {
    if (mode_ == DAQMode::kDAQScan) {
      // Time period is less than the dwell time per pixel
      if (steady_clock::now() - start_timestamp < daq_duration) {
        //      auto t1 = steady_clock::now();
        pipes_.front().CollectData();
        //      auto t2 = steady_clock::now();
        //      micros += duration_cast<microseconds>(t2 - t1);
        //      ++samples;
        std::this_thread::sleep_for(daq_duration / 10);
        continue;
      }
      //    micros /= samples;
      //    std::cout << "Collecting data took (us): " << micros.count() << std::endl;
      //    micros = microseconds {0};
      //    samples = 0;
      //    auto t1 = steady_clock::now();
          pipes_.front().SendDataDownPipe();
      //    auto t2 = steady_clock::now();
      //    std::cout << "Exchange took (us): "
      //              <<  duration_cast<microseconds>(t2  - t1).count() << std::endl;
      ++pixel;

      if (pixel == total_pixels) {
        break;
      }

      if ( (pixel % width) == 0) {
        // Synchronize with the motors at the end of each scan line
        pipes_.front().ClosePipe();
        //      sem_post(sem_probe);
        //      sem_wait(sem_reply);
        pipes_.front().OpenPipe();
      }
      start_timestamp = steady_clock::now();
    }
    else if (mode_ == DAQMode::kDAQPoint) {
      if (steady_clock::now() - start_timestamp > daq_duration) {
        break;
      }
      std::this_thread::sleep_for(500ms);
      pipes_.front().CollectData();
      pipes_.front().SendDataDownPipe();
    }
    else {
      break;
    }
  }

  CleanupEverything();
}

void DAQSession::CleanupEverything() {
  mode_ = DAQMode::kDAQInvalid;
  pipes_.clear();

  // Handle semaphores
  if (sem_reply) {
    sem_close(sem_reply);
  }
  if (sem_probe) {
    sem_close(sem_probe);
  }

  caen_handle_.reset(nullptr);
  safe_to_stop.store(true);
}

void DAQPipe::SendDataDownPipe() {
  if (!pipe_open_) {
    return;
  }

  for (auto & channel : producer_->GetHandles()) {
    auto packet = consumer_->GetEmptyDataPacket();
    packet.channel_id = channel.id;
    packet.pixel_no = pixel++;
    packet.m_buffer.swap(channel.histogram);

    consumer_->PushDataPacketToQueue(std::move(packet));
  }
}

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
    for (auto & channel : board.channels) {
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
      using namespace std::chrono;
      auto t1 = high_resolution_clock::now();
      WriteDataToFiles(current_job);
      auto t2 = high_resolution_clock::now();
      std::cout << "Writing to file took (ms): "
                << duration_cast<milliseconds>(t2 - t1).count() << std::endl;
//      For writing spectra is only a couple of microseconds
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
  auto & file  = file_object->DataFile();
  int32_t bin {0}, expected_bin {0};

  switch (mode_) {
  case DAQMode::kDAQScan :
    file << std::hex << std::uppercase;
    file << packet.pixel_no << ',';
    for (auto & counts : packet.m_buffer) {
      if (counts == 0) {
        ++bin;
        continue;
      }
      if (bin != expected_bin) {
        file << bin - 1 << "R,";
        expected_bin = bin;
      }
      file << counts << ',';
      ++bin;
      ++expected_bin;
    }
    file.seekp(file.tellp() - std::fstream::off_type {1});
    file << '\n';
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


} //namespace maxrf::daq
