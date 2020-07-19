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

//enum class SHMMapping : std::size_t
//{
//  kStart            = 0,
//  kDAQDaemonEnable  = offsetof(SHMStructure, daq_daemon_enable),
//  kDAQDaemonPID     = offsetof(SHMStructure, daq_daemon_pid),
//  kDAQSocketFD      = offsetof(SHMStructure, daq_socket_fd),
//  kSize             = sizeof (SHMStructure)
//};


//class SHMMap
//{
//  using TypeSizeIndex = std::byte;
//  std::map<SHMMapping, TypeSizeIndex>  type_of_shm_variable_ {};
//  std::vector<std::size_t>             type_sizes {};

//  std::map<SHMMapping, std::any SHMStructure::*> map;
//  SHMStructure structure {};
//  template <class T>
//  void InsertToMap(SHMMapping var, T const &) {
//    auto it = std::find(type_sizes.begin(), type_sizes.end(), sizeof (T));
//    if (it == type_sizes.end()) {
//      // The size is not already in the vector, we can add it
//      type_sizes.push_back(sizeof (T));
//      type_of_shm_variable_.insert({ var,
//                                     static_cast<std::byte>(type_sizes.size() - 1)});
//    }
//    else {
//      type_of_shm_variable_.insert({var,
//                                    static_cast<std::byte>(*it)});
//    }
//  }


//  template <class T,
//            class U = std::enable_if_t<std::is_standard_layout_v<T>>>
//  void MakeMapping(SHMMapping var, T SHMStructure::* val) {
////    auto offset =
////        reinterpret_cast<std::byte *>(&structure) -
////        reinterpret_cast<std::byte *>(&(structure.*val));

////    map.insert({ var, {offset, sizeof (structure.*val)} });
//    map.insert({var, std::any_cast<std::any SHMStructure::*>(val)});
//  }

//public:

//  SHMMap() {
//    SHMStructure s {};
//    auto ptr = &s;

//    auto member_ptr = &SHMStructure::daq_daemon_enable;
//    auto var = &((*ptr).*member_ptr);

//    MakeMapping(SHMMapping::kDAQDaemonEnable, &SHMStructure::daq_daemon_enable);
//    InsertToMap(SHMMapping::kDAQDaemonEnable, s.daq_daemon_enable);
//    InsertToMap(SHMMapping::kDAQDaemonPID, s.daq_daemon_pid);
//    InsertToMap(SHMMapping::kDAQSocketFD, s.daq_socket_fd);
//  }


//  auto GetSizeOfVar(SHMMapping const var) {
//    auto it = type_of_shm_variable_.find(var);

//    return it == type_of_shm_variable_.end() ?
//          std::size_t {0} :
//          type_sizes[static_cast<std::size_t>(it->second)];
//  }

//};





} // namespace maxrf::ipc

#endif // IPC_SHM_MAPPING_H
