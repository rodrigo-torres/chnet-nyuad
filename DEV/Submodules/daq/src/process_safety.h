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

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

///
/// \brief The SocketServer class uses Unix domain sockets to guarantee only
/// one instance of the program runs on a machine at any given time.
///
class SocketServer
{
public:
  SocketServer(std::string const & _path) : socket_path{_path} {
    socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (socket_fd == -1) {
      std::cout << "Couldn't create process socket: " << strerror(errno)
                << std::endl;
      std::exit(EXIT_FAILURE);
    }

    struct sockaddr_un address;
    address.sun_family = AF_UNIX;
    snprintf(address.sun_path, socket_path.size() + 1, "%s",
             socket_path.c_str());

    int rc = bind(socket_fd, reinterpret_cast<struct sockaddr *>(&address),
                  sizeof (address));
    if (rc == -1) {
      close(socket_fd);
      std::cout << "Couldn't bind socket: " << strerror(errno) << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
  ~SocketServer() {
    ReleaseSocket();
  }

  inline void ReleaseSocket() {
    if (socket_fd != -1) {
      close(socket_fd);
      unlink(socket_path.c_str());
    }
  }

private:
  std::string const socket_path;
  int socket_fd {-1};
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
