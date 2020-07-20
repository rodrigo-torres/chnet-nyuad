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
#ifndef MCA_CONFIGURATION_H
#define MCA_CONFIGURATION_H

#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <cstring>
#include <memory>
#include <mutex>
#include <functional>

#include <CAENDPPLib.h>
#include "daq_types.h"

namespace maxrf::daq {

// Set the size of the DPP memory buffer for energy events (max is 8192)
constexpr static int kEventsBufferSize = 8192;
static_assert (kEventsBufferSize <= 8192,
               "Maximum possible size for the buffer is 8192");

/// Class declarations
class CAEN_MCAHandle;
class CAENLibraryHandle;


/// Helper functions
extern std::string DecodeCAENRetCode(int32_t code) noexcept;
extern void DecodeCAENRetCode(int32_t code, std::string  & out) noexcept;


struct ChannelHandle
{
  ChannelHandle() = default;
  ChannelHandle(int32_t _id) : id {_id} {}

  int32_t id {std::numeric_limits<int32_t>::max()};
  uint32_t counts {0};
  SpectralData histogram = SpectralData (kSpectralBins, 0);
  std::array<CAENDPP_ListEvent_t, kEventsBufferSize> buffer {};
};

using DigitizerParams = CAENDPP_DgtzParams_t;

class CAENLibraryHandle
{
  // Set the number of MCA boards connections (experimental)
  constexpr static int kNumberOfBoards {1};
public:
  using Handle          = std::shared_ptr<CAEN_MCAHandle>;
  using ConfigCallback  = std::function<DigitizerParams ()>;
  using CallbacksList   = std::array<ConfigCallback const, kNumberOfBoards>;

  CAENLibraryHandle() {
    auto ret = CAENDPP_InitLibrary(&CAEN_library_handle);
    if (ret != CAENDPP_RetCode_Ok) {
      throw std::runtime_error(DecodeCAENRetCode(ret));
    }
  }
  CAENLibraryHandle(CAENLibraryHandle const &) = delete;
  CAENLibraryHandle & operator= (CAENLibraryHandle const &) = delete;

  ~CAENLibraryHandle() {
    CAENDPP_EndLibrary(CAEN_library_handle);
  }

  Handle GetBoardHandle(MCABoardConfiguration const & config);

  template <class T, class U>
  inline void MapEnumValues(T const & val, U & caen_val) {
    caen_val = static_cast<U>(static_cast<int>(val));
  }

private:
  int32_t CAEN_library_handle {std::numeric_limits<int32_t>::max()};

  static ConfigCallback::result_type ConfigureBoard0() noexcept;
//  static ConfigCallback::result_type ConfigureBoard1();
  ///
  /// \brief default_boards_config_ is an array whose entries hold a pointer to
  /// a function which return the a DigitizerParams struct with the correct
  /// values set for a given MCA board.
  ///
  static CallbacksList const default_boards_config_;
};


class CAEN_MCAHandle
{
  friend CAENLibraryHandle;

protected:
  CAEN_MCAHandle() {}

  int32_t library_handle  { std::numeric_limits<int32_t>::max() };
  int32_t board_handle    { std::numeric_limits<int32_t>::max() };
  std::vector<ChannelHandle> channel_handles  {};

public:
  ~CAEN_MCAHandle() {
    StopAcquisition();
  }

  inline void StartAcquisition() {
    for (auto & handle : channel_handles) {
      auto err = CAENDPP_StartAcquisition(library_handle, handle.id);
      std::cout << DecodeCAENRetCode(err) << std::endl;
    }
  }

  inline void StopAcquisition() {
    CAENDPP_AcqStatus_t is_acquiring;

    for (auto & handle : channel_handles) {
      CAENDPP_IsChannelAcquiring(library_handle, handle.id, &is_acquiring);
      if (is_acquiring) {
        CAENDPP_StopAcquisition(library_handle, handle.id);
      }
    }
  }


  inline void UpdateHistograms() {
    int32_t err;

    for (auto & handle : channel_handles) {
      err = CAENDPP_GetListEvents(library_handle, handle.id,
                                  handle.buffer.data(), &handle.counts);
      if (err != CAENDPP_RetCode_Ok) {
        std::cout << DecodeCAENRetCode(err) << std::endl;
        continue;
      }

      std::for_each(handle.buffer.begin(),
                    handle.buffer.begin() + handle.counts,
                    [&] (CAENDPP_ListEvent_t & event) {
        if (event.Energy < kSpectralBins) {
          ++handle.histogram.at(event.Energy);
        }
      });
    }
  }


  auto & GetHandles() {
    return channel_handles;
  }
};

}  //namespace maxrf::daq

#endif // MCA_CONFIGURATION_H
