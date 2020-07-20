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
// FOR System V POSIX SHM
#include <sys/ipc.h>
#include <sys/shm.h>
// FOR FD management
#include <sys/types.h>
#include <unistd.h>

#include <QtCore/qglobal.h>

#if defined(MAXRF_LIBRARIES)
#  define MAXRF_LIBRARIES_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAXRF_LIBRARIES_SHARED_EXPORT Q_DECL_IMPORT
#endif

#include "ipc_shm_mapping.h"

namespace maxrf::ipc {

enum class SocketOptions
{
  kServer = 0x0,
  kClient,
};

enum class Segments : int
{
  kSHMCommand = 0,  ///! For control and status flags
  kSHMSpectra,      ///! For passing XRF spectra between processes
};

///
/// \brief The Queue class is a wrapper around an STL queue which uses
/// a mutex and a condition variable to provide a thread-safe FIFO queue for
/// synchronizing jobs across threads
///
template <typename T>
class Queue
{
 public:
  using value_type  = T;

  ///
  /// \brief empty checks if there are items in the queue
  /// \return a boolean value, true if queue is empty, false otherwise
  ///
  bool empty() {
    std::unique_lock<std::mutex> mlock(mutex_);
    return queue_.empty();
  }

  ///
  /// \brief pop attempts to get the first item available on the queue (i.e. the
  /// oldest) and blocks if the queue is empty
  /// \return the oldest element in the queue of type value_type
  ///
  value_type pop()
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

  ///
  /// \brief pop an overload of the method above
  /// \param item
  ///
  void pop(value_type & item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
    item = queue_.front();
    queue_.pop();
  }

  ///
  /// \brief try_pop same as \see Queue::pop but if the call to pop from queue
  /// would block, the function returns false. Otherwise the element is popped
  /// immediately
  /// \param item of type value_type, the element is retreived by copy
  /// \return a boolean value, true if an element was popped, false otherwise
  ///
  bool try_pop(value_type & item)
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



  void push(value_type const & item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(item);
    mlock.unlock();
    cond_.notify_one();
  }

  void push(value_type && item)
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
      shutdown(connection_fd_, SHUT_RDWR);
      shutdown(socket_fd, SHUT_RDWR);
      unlink(socket_path.c_str());
    }
  }

  void EstablishConnectionWithClient() {
    if (is_server) {
      auto ret = accept(socket_fd, nullptr, nullptr);
      if (ret == -1) {
        std::cout << strerror(errno) << std::endl;
        return;
      }
 //     shutdown(socket_fd, SHUT_RDWR);
      connection_fd_ = socket_fd;
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

    rc = listen(socket_fd, 1);
    if (rc == -1) {
      std::cout << "Couldn't bind socket: " << strerror(errno) << std::endl;
      close(socket_fd);
      socket_fd = -1;
      return;
    }
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
  int connection_fd_ {-1};
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
  SHMStructure * segment_start_ {nullptr};
  int shm_fd_ { -1 };

public:
  ~SHMHandle() {
    if (segment_start_) {
      munmap(segment_start_, sizeof (SHMStructure));
      close(shm_fd_);
    }
  }

  void Create() {
    shm_fd_ = shm_open("/maxrf_daq", O_RDWR | O_CREAT | O_EXCL,
                       S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
    if (shm_fd_ == -1) {
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

    segment_start_ = static_cast<SHMStructure *>(mmap(nullptr, sizeof (SHMStructure),
                          PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_, 0));
    if (segment_start_ == MAP_FAILED) {
      close(shm_fd_);
      shm_unlink("/maxrf_daq");
      segment_start_ =  nullptr;
      perror(strerror(errno));
      return;
    }

    SHMStructure default_values {};
    memcpy(segment_start_, &default_values, sizeof (SHMStructure));
  }

  void Open() {
    shm_fd_ = shm_open("/maxrf_daq", O_RDWR, 0);
    if (shm_fd_ == -1) {
      perror(strerror(errno));
      return;
    }

    segment_start_ = static_cast<SHMStructure *>(mmap(nullptr, sizeof (SHMStructure),
                          PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_, 0));
    if (segment_start_ == MAP_FAILED) {
      close(shm_fd_);
      segment_start_ =  nullptr;
      perror(strerror(errno));
      return;
    }

  }

  template <class T>
  auto GetVariable(T SHMStructure::* shm_structure_member) {
    return (*segment_start_).*shm_structure_member;
  }

  template <class T>
  void GetVariable(T SHMStructure::* shm_structure_member, SHMStructure & in) {
    in.*shm_structure_member = (*segment_start_).*shm_structure_member;
  }

  template <class T>
  void WriteVariable(T SHMStructure::* shm_structure_member, T const & val) {
    (*segment_start_).*shm_structure_member = val;
  }

};

template <typename T>
class MAXRF_LIBRARIES_SHARED_EXPORT shmarray
{
public:
  using value_type    = T;
  using pointer       = value_type *;
  using reference     = value_type &;
  using const_pointer = value_type const *;
  using const_reference = value_type const &;
  using size_type = std::size_t;

public:
  shmarray() = default;
  ~shmarray();
  // Deleted move and copy constructors
  shmarray (shmarray const &) = delete;
  shmarray (shmarray &&) = delete;
  // Deleted move and copy assignment operators
  shmarray & operator= (shmarray const &) = delete;
  shmarray & operator= (shmarray &&) = delete;

  // Only way to initialize the array
  auto initialize(Segments segment) noexcept -> bool;

  // Element access -> Access is only permitted with boundary checks
  auto at (size_type pos) -> reference;
  auto at (size_type pos) const -> const_reference;

  // Underlying pointer
  auto data() -> pointer;

  //  Capacity;
  auto empty() const noexcept -> bool;
  auto size() const noexcept -> size_type;
  auto size_bytes() const noexcept -> size_type;
private:
  int shmid_  {-1};
  size_type size_ {0};
  size_type size_bytes_ {0};
  pointer segment_  {nullptr};
};

} // namespace maxrf::ipc

#endif // IPC_METHODS_H
