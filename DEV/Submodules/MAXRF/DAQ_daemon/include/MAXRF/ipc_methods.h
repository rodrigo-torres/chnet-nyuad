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

#ifndef IPC_METHODS_H
#define IPC_METHODS_H

#include <array>
#include <csignal>
#include <cstring>    //memcpy and the lik
#include <iostream>
#include <string>
#include <type_traits>
#include <queue>

#include <condition_variable>	// Concurrent queue notify
#include <mutex>              // Mutual exclusion
#include <queue>              // Job FIFO queue

// For POSIX SHM
#include <fcntl.h>           // For O_* constants
#include <sys/mman.h>
#include <sys/stat.h>        // For mode constants
// FOR UNIX Domain sockets
#include <sys/socket.h>
#include <sys/un.h>          // UNIX socket address struct
// FOR FD management
#include <sys/types.h>
#include <unistd.h>

#include "ipc_shm_mapping.h"

namespace maxrf::ipc {

template <typename T>
class Queue
{
 public:
  ///
  /// \brief empty checks if there are items in the queue
  /// \return a boolean value, true if queue is empty, false otherwise
  ///

  bool empty() {
    std::unique_lock<std::mutex> mlock(mutex_);
    return queue_.empty();
  }

  T pop()
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
    auto item = queue_.front();
    queue_.pop();
    return item;
  }

  bool try_pop(T& item)
  {
    using namespace std::chrono_literals;

    std::unique_lock<std::mutex> mlock(mutex_);
    if (cond_.wait_for(mlock, 100ms, [&] {return !queue_.empty();})) {
      item = queue_.front();
      queue_.pop();
      return true;
    }
    else {
      return false;
    }
  }

  void pop(T& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
    item = queue_.front();
    queue_.pop();
  }

  void push(const T& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(item);
    mlock.unlock();
    cond_.notify_one();
  }

  void push(T&& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(std::move(item));
    mlock.unlock();
    cond_.notify_one();
  }
 private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
};


enum class SocketOptions
{
  kServer = 0x0,
  kClient
};

///
/// \brief The SocketServer class uses Unix domain sockets to guarantee only
/// one instance of the program runs on a machine at any given time.
///
class IPCSocket
{
public:
  IPCSocket(std::string const & _path, SocketOptions opt) : socket_path{_path} {
    switch (opt) {
    case SocketOptions::kServer:
      CreateServer();
      is_server = true;
      break;
    case SocketOptions::kClient:
      CreateClient();
      break;
    }
  }

  ~IPCSocket() {
    ReleaseSocket();
  }

  inline void ReleaseSocket() {
    if (socket_fd != -1) {
//      close(socket_fd);
    }
    if (is_server) {
      shutdown(socket_fd, SHUT_RDWR);
      unlink(socket_path.c_str());
    }
  }

  void EstablishConnectionWithClient() {
    if (is_server) {
      int addr_len;
      struct sockaddr_un address;
      auto ret = accept(socket_fd,
                        reinterpret_cast<sockaddr *>(&address),
                        reinterpret_cast<socklen_t *>(&addr_len));
      if (ret == -1) {
        std::cout << strerror(errno) << std::endl;
        return;
      }
      shutdown(socket_fd, SHUT_RDWR);
      socket_fd = ret;
    }
  }

  template <class T>
  void ReceiveFromSocket(T * buff) {
    recv(socket_fd, buff, sizeof (T), 0);
  }

  void ReceiveFromSocket(char * buff) {
    do {
      recv(socket_fd, buff, 1, 0); // Read character by character
    } while (*(buff++) != '\0');
  }

  void ReceiveFromSocket(char & buff) {
    recv(socket_fd, &buff, 1, 0);
  }

  template<class T>
  void SendThroughSocket(T const * data) {
    // TODO enable only for arithmetic, enum types and trivial classes
    send(socket_fd, data, sizeof(T), 0);
  }


  void SendThroughSocket(char const * data) {
    // TODO enable only for arithmetic, enum types and trivial classes
    for (uint off = 0; off < strlen(data) + 1; ++off) {
      send(socket_fd, data + off, 1, 0);
    }
  }

  void SendCharThroughSocket(char c) {
    char buff = c;
    send(socket_fd, &buff, 1, 0);
  }


private:
  void CreateServer() {
    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd == -1) {
      std::cout << "Couldn't create process socket: " << strerror(errno)
                << std::endl;
      return;
    }

    struct sockaddr_un address;
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, socket_path.c_str(), 108); // UNIX_PATH_MAX is 108

    int rc = bind(socket_fd, reinterpret_cast<struct sockaddr *>(&address),
                  sizeof (address));
    if (rc == -1) {
      std::cout << "Couldn't bind socket: " << strerror(errno) << std::endl;
      close(socket_fd);
      socket_fd = -1;
      return;
    }

    listen(socket_fd, 1);
  }

  void CreateClient() {
    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd == -1) {
      std::cout << "Couldn't create process socket: " << strerror(errno)
                << std::endl;
      return;
    }

    struct sockaddr_un address;
    address.sun_family  = AF_UNIX;
    strncpy(address.sun_path, socket_path.c_str(), 108);

    auto ret = connect(socket_fd,
                       reinterpret_cast<sockaddr *>(&address), sizeof (address));
    if (ret == -1) {
      std::cout << "Couldn't connect to socket: "
                << strerror(errno) << std::endl;
      close(socket_fd);
      socket_fd = -1;
    }
  }


  std::string const socket_path;
  int socket_fd {-1};
  bool is_server {false};
};

class ResourceCleaner
{
public:
  ResourceCleaner(void (*_callback)(int)) : clean_up_callback {_callback}
  {
    if (clean_up_callback == nullptr) {
      std::cout << "Invalid clean-up callback pointer" << std::endl;
      std::exit(EXIT_FAILURE);
    }

    std::array<int, 5> sigs { SIGINT, SIGTERM, SIGABRT, SIGSEGV, SIGFPE };
    for (auto sig : sigs) {
      if (std::signal(sig, clean_up_callback) == SIG_ERR) {
        std::cout << "Can't guarantee proper cleanup in case of failure!"
                  << std::endl;
        std::exit(EXIT_FAILURE);
      }
    }
  }
private:
  void (*clean_up_callback)(int) {nullptr};
};

class SHMHandle
{
  SHMStructure * shm_segment_ {nullptr};

public:
  ~SHMHandle() {
    if (shm_segment_) {
      munmap(shm_segment_, sizeof (SHMStructure));
    }
  }

  void Create() {
    auto shm_fd_ = shm_open("/maxrf_daq", O_RDWR | O_CREAT | O_EXCL,
                       S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
    if (shm_fd_ == -1) {
      shm_unlink("/maxrf_daq");
      perror(strerror(errno));
      return;
    }

    auto  ret = ftruncate(shm_fd_, sizeof (SHMStructure));
    if (ret == -1) {
      close(shm_fd_);
      shm_unlink("/maxrf_daq");
      perror(strerror(errno));
      return;
    }

    shm_segment_ = static_cast<SHMStructure *>(mmap(nullptr, sizeof (SHMStructure),
                          PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_, 0));
    if (shm_segment_ == MAP_FAILED) {
      close(shm_fd_);
      shm_unlink("/maxrf_daq");
      shm_segment_ =  nullptr;
      perror(strerror(errno));
      return;
    }

    SHMStructure default_values {};
    memcpy(shm_segment_, &default_values, sizeof (SHMStructure));

    shm_unlink("/maxrf_daq");
  }

  void Open() {
    auto shm_fd_ = shm_open("/maxrf_daq", O_RDWR, 0);
    if (shm_fd_ == -1) {
      perror(strerror(errno));
    }


    shm_segment_ = static_cast<SHMStructure *>(mmap(nullptr, sizeof (SHMStructure),
                          PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_, 0));
    if (shm_segment_ == MAP_FAILED) {
      shm_segment_ =  nullptr;
      perror(strerror(errno));
    }

    close(shm_fd_);
  }

  template <typename T>
  auto GetVariable(T SHMStructure::* shm_structure_member) {
    return (*shm_segment_).*shm_structure_member;
  }

  template <class T>
  void GetVariable(T SHMStructure::* shm_structure_member, SHMStructure & in) {
    in.*shm_structure_member = (*shm_segment_).*shm_structure_member;
  }

  template <class T>
  void WriteVariable(T SHMStructure::* shm_structure_member, T const & val) {
    (*shm_segment_).*shm_structure_member = val;
  }

};


}

#endif // IPC_METHODS_H
