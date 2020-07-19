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

#ifndef PROCESS_SAFETY_H
#define PROCESS_SAFETY_H

#include <array>
#include <csignal>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

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

#endif // PROCESS_SAFETY_H
