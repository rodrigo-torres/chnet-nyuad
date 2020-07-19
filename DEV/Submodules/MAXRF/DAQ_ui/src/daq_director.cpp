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

  int32_t width, height;
  int32_t total_pixels;
  width = scan_params_.x_end_coordinate - scan_params_.x_start_coordinate;
  width = width / scan_params_.x_motor_step;
  height = scan_params_.y_end_coordinate - scan_params_.y_start_coordinate;
  height = (height/ scan_params_.y_motor_step) + 1;
  total_pixels =  width * height;
  int32_t pixel {0};
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

} //namespace maxrf::daq
