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

#include <chrono>

#include "daq_director.h"
#include "user_interface.h"
#include "process_safety.h"

#include <QApplication>

//static SocketServer process_lock {"/run/maxrf/daq.sock"};
//static maxrf::daq::DAQSession daq;

extern "C" [[noreturn]] void SafeTermination([[maybe_unused]] int sig) {
  //  process_lock.ReleaseSocket();

  //  daq.ForceStopDAQ();
  //  while (daq.IsSafeToExit() != true) {
  //    std::this_thread::sleep_for(std::chrono::milliseconds {10});
  //  }

  std::cout << "Cleanup success!" << std::endl;
  std::_Exit(EXIT_FAILURE);
}

template <class T>
void ReadFromSHM(uchar const * & shm, T  * obj) {
  memcpy(obj, shm, sizeof (T));
  shm += sizeof (T);
}

template <>
void ReadFromSHM<char>(uchar const * & shm, char * c_str) {
  strcpy(c_str, (char const *) shm);
  shm += strlen(c_str) + 1;
}

template <class T>
void ReceiveFromSocket(int fd, T * buff) {
  recv(fd, buff, sizeof (T), 0);
}

template <>
void ReceiveFromSocket<char>(int fd, char * buff) {
  do {
    recv(fd, buff, 1, 0); // Read character by character
  } while (*(buff++) != '\0');
}

int StartFromCodedValues();
int StartFromSHM();
int StartFromSocket(int fd);

int main(int argc, char ** argv)
{
  //  ResourceCleaner safety {SafeTermination};

  //  struct sigaction action;
  //  action.sa_flags = 0;
  //  sigfillset(&action.sa_mask);

  //  sigaction(SIGTERM, &action, nullptr);



//  return StartFromCodedValues();
//  return StartFromSHM();
//  int fd = 28;
//  return StartFromSocket(fd);

  //  return 0;


    QApplication app {argc, argv};

    maxrf::daq::Wrapper wrap {};
  //  wrap.StartDAQ();

    return app.exec();
}

int StartFromSocket(int fd) {
  using namespace maxrf::daq;
  DAQInitParameters params {};
  char command {0};


  bool done {false};
  do {
    auto ret = recv(fd, &command, 1, 0);
    if (ret == -1) {
      return 1;
    }
    switch (command)
    {
    case 'B':
      params.boards_config.push_back(MCABoardConfiguration {});
      ReceiveFromSocket(fd, &params.boards_config.back().conn);
      ReceiveFromSocket(fd, &params.boards_config.back().board_id);
      break;
    case 'C': {
      uint8_t id {0};
      MCAChannelParameters channel_params {};
      ReceiveFromSocket(fd, &id);
      ReceiveFromSocket(fd, &channel_params);
      params.boards_config.back().channels.insert({id, channel_params});
      break;
    }
    case 'K': {
      char key [256], val [256];
      ReceiveFromSocket(fd, key);
      ReceiveFromSocket(fd, val);
      params.user_header_fields.insert({std::string {key}, std::string {val}});
      break;
    }
    case 'E': {
      char path [256],  filename [256];
      ReceiveFromSocket(fd, &params.scan_parameters);
      ReceiveFromSocket(fd, &params.mode);
      ReceiveFromSocket(fd, &params.timeout);
      ReceiveFromSocket(fd, path);
      ReceiveFromSocket(fd, filename);
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
  return 0;
}

int StartFromSHM() {
  using namespace maxrf::daq;

  DAQInitParameters params {};

  auto fd = shm_open("/maxrf_daq",  O_RDWR,  0);
  if (fd == -1) {
    perror(strerror(errno));
    return 1;
  }

  struct stat stats {};
  auto ret = fstat(fd, &stats);
  if (ret == -1) {
    perror(strerror(errno));
    return  1;
  }

  auto shm = static_cast<uchar const *>(mmap(nullptr, stats.st_size,
                                             PROT_READ | PROT_WRITE,
                                             MAP_SHARED, fd, 0));

  bool done {false};
  do {
    switch (*(shm++)) {
    case 'B':
      params.boards_config.push_back(MCABoardConfiguration {});
      ReadFromSHM(shm, &params.boards_config.back().conn);
      ReadFromSHM(shm, &params.boards_config.back().board_id);
      break;
    case 'C': {
      uint8_t id {0};
      MCAChannelParameters channel_params {};
      ReadFromSHM(shm, &id);
      ReadFromSHM(shm, &channel_params);
      params.boards_config.back().channels.insert({id, channel_params});
      break;
    }
    case 'K': {
      char key [256], val [256];
      ReadFromSHM(shm, key);
      ReadFromSHM(shm, val);
      params.user_header_fields.insert({std::string {key}, std::string {val}});
      break;
    }
    case 'E': {
      char path [256],  filename [256];
      ReadFromSHM(shm, &params.scan_parameters);
      ReadFromSHM(shm, &params.mode);
      ReadFromSHM(shm, &params.timeout);
      ReadFromSHM(shm, path);
      ReadFromSHM(shm, filename);
      params.output_path = std::string {path};
      params.base_filename = std::string {filename};
      done = true;
      break;
    }
    default:
      std::cout <<  "Error  getting parameters from shm" << std::endl;
      return 1;
    }
  } while (!done);

  std::cout << "Done" << std::endl;
  return 0;
}
