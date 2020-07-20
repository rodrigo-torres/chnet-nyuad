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

#ifndef IPC_SHM_MAPPING_H
#define IPC_SHM_MAPPING_H

#include <cstdint>
#include <limits>
#include <type_traits>

#include <sys/types.h>

namespace maxrf::ipc {

struct SHMStructure
{
  bool    daq_daemon_enable { false };
  pid_t   daq_daemon_pid { std::numeric_limits<pid_t>::max() };
  int32_t daq_socket_fd  { std::numeric_limits<int32_t>::max() };
};

static_assert (std::is_standard_layout_v<SHMStructure>,
               "SHMStructure  has to satisfy the requirements of a standard"
               "layout type. See this link:\n"
               "https://en.cppreference.com/w/cpp/named_req/StandardLayoutType");

} // namespace maxrf::ipc

#endif // IPC_SHM_MAPPING_H
