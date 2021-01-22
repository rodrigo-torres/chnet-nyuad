/**
 * @file
 * @author  Rodrigo Torres <rodrigo.torres@nyu.edu>
 * @version 1.1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * The shared memory array class (shmarray) provides a bounds-safe, reentrant
 * wrapper for POSIX shared memory segments. It implements some of the methods
 * found in the std::array container for STL compatibility.
 */


#ifndef IPC_SHM_MAPPING_H
#define IPC_SHM_MAPPING_H

#include <atomic>
#include <cstdint>
#include <cstddef>
#include <limits>
#include <type_traits>

#include <semaphore.h>
#include <sys/types.h>

#include "MAXRF/types.h"

namespace maxrf::ipc {

using maxrf::daq::DAQModeParameters;

#define TYPE_MAX(type) { std::numeric_limits<type>::max() }
#define TYPE_MIN(type) { std::numeric_limits<type>::min() }

struct SHMStructure
{
  SHMStructure() {
    segment_semaphore = sem_t {{0}};
    seq_number  = 0;
    mappings    = 0;

    daq_daemon_enable = false;
    daq_daemon_active = false;
    daq_request_scan_from_motors  = false;
    daq_scan_request_acknowledged = false;

    daq_mca_library_handle  = -1;
    daq_daemon_pid    = -1;
    xrf_image_pid     = -1;
    xrf_spectra_pid   = -1;
    xray_table_pid    = -1;
    rate_pid          = -1;

    daq_session_params = DAQModeParameters {};  // initialize to defaults
  }

  sem_t segment_semaphore;          ///< seqlock semaphore lock
  std::atomic_uint64_t seq_number;  ///< seqlock sequence number

  int32_t mappings; ///< Counts the times the segment has been mapped

//  int     mappings_counter    {0};  ///! Number of times the segment has been attached

  bool    daq_daemon_enable;
  bool    daq_daemon_active;
  bool    daq_request_scan_from_motors;
  bool    daq_scan_request_acknowledged;

  int32_t daq_mca_library_handle;

  pid_t   daq_daemon_pid;
  pid_t   xrf_image_pid;
  pid_t   xrf_spectra_pid;
  pid_t   xray_table_pid;
  pid_t   rate_pid;

  DAQModeParameters daq_session_params;
};


static_assert (std::is_standard_layout_v<SHMStructure>,
               "Standard layout types occupy contiguous bytes of storage, and "
               "add no padding at the beginning of the objects, meaning that "
               "offsets from the beginning of the object to its non-static "
               "members are well defined. "
               "This is a strong requirement for shared memory object safety. "
               "For more info see "
               "https://en.cppreference.com/w/cpp/types/is_standard_layout");


static_assert (std::atomic_uint64_t::is_always_lock_free ,
               "A 64 bit integer atomic is used as the type for the sequence"
               "number of the segment seqlock. It should be lock free. Compile"
               "with GCC");

static_assert (offsetof(SHMStructure, segment_semaphore) == 0 ,
               "The shared memory object semaphore (segment_semaphore) must be "
               "the first member of SHMStructure!");

} // namespace maxrf::ipc

#endif // IPC_SHM_MAPPING_H
