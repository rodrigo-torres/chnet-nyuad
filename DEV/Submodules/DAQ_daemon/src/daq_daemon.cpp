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

  if (config.mode_parameters.mode == DAQMode::kDAQInvalid) {
    return status;
  }

  try {
    caen_handle_ = std::make_unique<CAENLibraryHandle>();

    auto writer_ = std::make_shared<FileWriter>();
    writer_->Initialize(config);

    uint pipes_id {0};
    for (auto & board : config.boards_config) {
//      auto ptr = caen_handle_->GetBoardHandle(board);
      pipes_.push_back(DAQPipe {caen_handle_->GetBoardHandle(board), writer_});
      pipes_.back().SetID(pipes_id);
      ++pipes_id;
    }
    status = true;
  } catch (std::exception & e) {
    std::cout << e.what() << std::endl;
  }
//  daq_duration = std::chrono::duration<double>(config.mode_parameters.timeout);
  session_params_ = config.mode_parameters;

  return status;
}

void DAQSession::StartDAQSession() {

  switch (session_params_.mode) {
  case DAQMode::kDAQScan :
    // Set up the extra stuff for the scan
    image_dimensions_[0] =
        session_params_.x_end_coordinate - session_params_.x_start_coordinate;
    image_dimensions_[0] /= session_params_.x_motor_step;
    image_dimensions_[1] =
        session_params_.y_end_coordinate - session_params_.y_start_coordinate;
    image_dimensions_[1] /= session_params_.y_motor_step;
    image_dimensions_[2] = image_dimensions_[0] * image_dimensions_[1];

//    sem_probe = sem_open("/daq_probe", 0);
//    sem_reply = sem_open("/daq_reply", 0);
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
  auto daq_duration   = std::chrono::duration<double> {session_params_.timeout};


  int32_t samples {0};
  microseconds micros {0};


  pipes_.front().OpenPipe();
  auto start_timestamp = steady_clock::now();
  while (daq_enable) {
    if (session_params_.mode == DAQMode::kDAQScan) {
      // Time period is less than the dwell time per pixel
      if (steady_clock::now() - start_timestamp < daq_duration) {
        auto t1 = steady_clock::now();
        pipes_.front().CollectData();
        auto t2 = steady_clock::now();
        micros += duration_cast<microseconds>(t2 - t1);
        ++samples;
        std::this_thread::sleep_for(daq_duration / 10);
        continue;
      }
          micros /= samples;
          std::cout << "Collecting data took (us): " << micros.count() << std::endl;
          micros = microseconds {0};
          samples = 0;
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
    else if (session_params_.mode == DAQMode::kDAQPoint) {
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
  session_params_.mode = DAQMode::kDAQInvalid;
  pipes_.clear();

  // Handle semaphores
  if (sem_reply) {
    sem_close(sem_reply);
  }
  if (sem_probe) {
    sem_close(sem_probe);
  }

  caen_handle_.reset(nullptr);
}

void DAQPipe::SendDataDownPipe() {
  if (!pipe_open_) {
    return;
  }

  uint32_t channel_id {0};
  for (auto & channel : producer_->GetHandles()) {
    auto packet = consumer_->GetEmptyDataPacket();
    packet.channel_id = pipe_id + channel_id;
    packet.pixel_no = pixel++;
    packet.m_buffer.swap(channel.histogram);

    consumer_->PushDataPacketToQueue(std::move(packet));
    ++channel_id;
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

      WriteMode mode = static_cast<WriteMode>(static_cast<int>(config.mode_parameters.mode));
      file->SetWriteMode(mode);
    }
  }

  constexpr int kEmptyBuffersSize = 10;
  for (auto _ = kEmptyBuffersSize; _--; ) {
    DataPacket packet {};
    packet.m_buffer.resize(kSpectralBins, 0);
    empty_packets_pool.push(std::move(packet));
  }

  mode_ = config.mode_parameters.mode;

  scan_line_width_ =
      static_cast<uint32_t>( ( config.mode_parameters.x_end_coordinate -
                              config.mode_parameters.x_start_coordinate ) /
                              config.mode_parameters.x_motor_step);
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
//      using namespace std::chrono;
//      auto t1 = high_resolution_clock::now();
      WriteDataToFiles(current_job);
//      auto t2 = high_resolution_clock::now();
//      std::cout << "Writing to file took (ms): "
//                << duration_cast<microseconds>(t2 - t1).count() << std::endl;
//      For writing spectra is on the order of  a few tenths of microseconds
      // Except! for when the tasks flushes out the line buffer
      // In which case it seems to take ~40-100 microseconds per pixel
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
  using PixelBuffer = std::vector<uchar>;
  thread_local std::vector<PixelBuffer> buffer {};
  thread_local std::vector<FileStats>   stats  {};
//  thread_local std::vector<std::byte>   single_call_buffer {};
  thread_local bool call_once {true};
  if (call_once) {
    // Each data file has it's own file stats data structure
    stats.resize(files_.size());
    // The buffer holds the pixel data of one line for every data file
    buffer.resize(files_.size() * scan_line_width_);
    // The buffer is arranged as such:
    // pos 0 -> buffer of pixel 0 of data file 0
    // pos 1 -> buffer of pixel 1 of data file 0
    // ...
    // pos scan_width - 1 -> buffer of pixel scan_width - 1 of data file 0
    // pos scan_width     -> buffer of pixel 0 of data file 1
    // pos scan_width + 1 -> buffer of pixel scan_width + 1 of data file 1
    // ...
    // pos (scan_width * no_of_files) - 1
    //        -> buffer of pixel scan_width - 1 of data file no_of_files - 1
    // pos (scan_width * no_of_files) -> end iterator of buffer
    for (auto & pixel_buff : buffer) {
      // Every pixel buffer reserves at first 2 pages of memory (tipically ~8K)
      pixel_buff.resize(0x1U << 12, 0);
    }
//    single_call_buffer.reserve(0x1U << 12);
    call_once = false;
  }

  // A useful lambda to reset Queue data packets back to 'zero' state
  auto thread_local ResetDataPacket = [] (Job & _p) {
    _p.pixel_no   = std::numeric_limits<int32_t>::max();
    _p.channel_id = std::numeric_limits<int32_t>::max();

    // Casting the pointers to char * and filling with '\0' forces the
    // memcpy template resolution of std::fill
    auto begin = reinterpret_cast<char *>(&(*_p.m_buffer.begin()));
    auto end   = reinterpret_cast<char *>(&(*_p.m_buffer.end()));
    std::fill(begin, end, '\0');
  };


  auto & call_buffer = buffer.at((packet.pixel_no % scan_line_width_)
                                 * (packet.channel_id + 1));

  // For now we only write channel 0 of the MCA
//  auto & file_object = files_.at(packet.channel_id);
//  auto & file  = file_object->DataFile();
//  int32_t bin {0}, expected_bin {0};
  std::size_t bin {0};
  constexpr uint16_t zero_block_flag {std::numeric_limits<uint16_t>::max()};

  switch (mode_) {
  case DAQMode::kDAQScan : {
    auto ptr = call_buffer.begin();
    auto end = call_buffer.end();
    // The x-coordinate is the modulus of the pixel number over the scan width
    uint32_t coord = packet.pixel_no % scan_line_width_;
    memcpy(ptr.base(), &coord, sizeof (coord));
    ptr += 4;
    coord = packet.pixel_no / scan_line_width_;
    memcpy(ptr.base(), &coord, sizeof (coord));
    ptr += 4;
    for (auto & counts : packet.m_buffer) {
      if (counts == 0) {
        ++bin;
        continue;
      }
      // Checks the size of the buffer is enough
      if (end.base() - ptr.base() < 100) {
        auto offset = ptr - call_buffer.begin();
        call_buffer.resize(call_buffer.size() * 2);
        ptr = call_buffer.begin() + offset;
      }


      if (bin != 0) {
        // Write preceding zeroes
        if (bin < 3) {
         memset(ptr.base(), 0, bin * 2);  // Zeroes use two bytes
         ptr += (bin * 2);
        }
        else {
          memcpy(ptr.base(), &zero_block_flag, sizeof (zero_block_flag));
          ptr += sizeof(zero_block_flag);
          memcpy(ptr.base(), &bin, 2);
          ptr += 2;
        }
        bin = 0;
      }
      // Write value
      memcpy(ptr.base(), &counts, 2);
      ptr += 2;

      // Zeroes the packet after reading the value
      counts = 0;
    }
    // Mark the end of pixel with 32 bits set to 1
    coord = std::numeric_limits<uint32_t>::max();
    memcpy(ptr.base(), &coord, 4);
    ptr += 4;

    // Wrote the buffer
    auto & ch_stats = stats.at(packet.channel_id);

    // If there are exactly scan_width pixels in the buffer, flush the
    // data in the correct order to the data file
    if (++ch_stats.pixels_in_buffer == scan_line_width_) {
      // Trigger writing into file in the correct order
      auto begin = buffer.begin() + scan_line_width_ * packet.channel_id;
      auto & file = files_.at(packet.channel_id)->DataFile();
      file << std::hex << std::uppercase;
      uint32_t bytes_read_4;
      uint16_t bytes_read_2;


      for (auto it = begin; it != begin + scan_line_width_; ++it) {
        ptr = it->begin();
        memcpy(&bytes_read_4, ptr.base(), 4);
        ptr += 4;

        file << bytes_read_4 << ',';  // Write the x coordinate
        memcpy(&bytes_read_4, ptr.base(), 4);
        ptr += 4;
        file << bytes_read_4; // Write the y coordinate

        bool enable {true};
        while (enable) {
          memcpy(&bytes_read_2, ptr.base(), 2);
          ptr += 2;
          switch (bytes_read_2) {
          case std::numeric_limits<uint16_t>::max() :
            // This is either a zero block flag or a pixel stop flag
            bytes_read_4 = static_cast<uint32_t>(bytes_read_2 << 16);
            memcpy(&bytes_read_2, ptr.base(), 2);
            ptr += 2;
            bytes_read_4 |= bytes_read_2;
            if (bytes_read_4 == std::numeric_limits<uint32_t>::max()) {
              // This is a pixel stop flag
              file << std::endl;
              enable = false;
            }
            else {
              // This a zero block flag, the last 2 bytes read tell us the
              // number of zeroes to write
              file << ',' << bytes_read_2 << 'R';
            }
            break;
          case 0:
            // Two zeroes
            file << ',' << 0;
            file << ',' << 0;
            break;
          default:
            // This a normal value
            file << ',' << bytes_read_2;
            break;
          }
        }

        ++ch_stats.pixels_written;
        --ch_stats.pixels_in_buffer;
      }

      // We'll read the data in blocks of 2 bytes

    }

    // After this monster of an operation we should be done

    //    file << std::hex << std::uppercase;
    //    file << packet.pixel_no << ',';
    //    for (auto & counts : packet.m_buffer) {
    //      if (counts == 0) {
    //        ++bin;
    //        continue;
    //      }
    //      if (bin != expected_bin) {
    //        file << bin - 1 << "R,";
    //        expected_bin = bin;
    //      }
    //      file << counts << ',';
    //      ++bin;
    //      ++expected_bin;
    //    }
    //    file.seekp(file.tellp() - std::fstream::off_type {1});
    //    file << '\n';
    break;
  }
  case DAQMode::kDAQPoint :
    files_.at(packet.channel_id)->WritePixel(packet.m_buffer);
    break;
  case DAQMode::kDAQInvalid :
    break;
  }

//  constexpr auto size = sizeof (std::vector<std::byte>);
//  std::vector<std::byte> buffer;  // Reserve two pages of memory ~8Kbytes
//  // 8 bytes begin scan
  // 3 bytes pixel coord, 3 bytes pixel coord, 2 bytes val, 2 bytes val, 1 byte zero, 1 byte zero, 2 bytes stop + 2 bytes number of zeroes following, 3 bytes pixel
  // 8 bytes end scan

//  ResetDataPacket(packet);
  // We have already zeroed the packet buffer
  packet.pixel_no   = std::numeric_limits<int32_t>::max();
  packet.channel_id = std::numeric_limits<int32_t>::max();

  empty_packets_pool.push(std::move(packet));
}


} //namespace maxrf::daq
