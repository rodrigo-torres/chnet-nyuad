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
#include "mca.h"

namespace maxrf::daq {


CAENLibraryHandle::Handle CAENLibraryHandle::GetBoardHandle(MCABoardConfiguration const & config) {
  int32_t err;
  auto HandleError = [](int32_t error) {
    if (error != CAENDPP_RetCode_Ok) {
      throw std::runtime_error(DecodeCAENRetCode(error));
    }
  };
  // TODO check if board has already been added

  // Check if the CAENDPP library has been initiliazed
  if (CAEN_library_handle == std::numeric_limits<int32_t>::max()) {
    err = CAENDPP_InitLibrary(&CAEN_library_handle);
    HandleError(err);
  }

  // Add the board to the library instance with the selected parameters
  int32_t board_handle;
  CAENDPP_ConnectionParams_t conn_params;
  MapEnumValues(config.connection.conn_type, conn_params.LinkType);
  conn_params.LinkNum         = config.connection.device_number;
  conn_params.ConetNode       = config.connection.daisy_chain_node;
  conn_params.VMEBaseAddress  = config.connection.vme_base_address;

  err = CAENDPP_AddBoard(CAEN_library_handle, conn_params, &board_handle);
  HandleError(err);

  // Get the MCA board info
  CAENDPP_Info_t info;
  err = CAENDPP_GetDPPInfo(CAEN_library_handle, board_handle, &info);
  HandleError(err);
  std::cout << "Board " << info.ModelName << '_' << info.SerialNumber <<
               " added succesfully" << std::endl;

  // Get the default configuration
  DigitizerParams params = default_boards_config_[0]();
  params.ChannelMask = 0;

  std::vector<ChannelHandle> handles;
  for (auto & chan : config.enabled_channels)  {
    handles.emplace_back(chan.first + board_handle);
    // Construct the channel enable mask;
    params.ChannelMask |= static_cast<uint32_t>(1U << chan.first);
    if (chan.second.use_defaults) {
      continue;
    }
    // TODO override the PHA parameters if specified
  }
  err = CAENDPP_SetBoardConfiguration(CAEN_library_handle, board_handle,
                                      CAENDPP_AcqMode_Histogram,
                                      params);
  HandleError(err);

  for (auto & channel : handles)  {
    // TODO get the input range from the user configuration struct
    err = CAENDPP_SetInputRange(CAEN_library_handle, channel.id,
                                CAENDPP_InputRange_0_6Vpp);
    HandleError(err);
  }

  auto hmca = std::shared_ptr<CAEN_MCAHandle>{new CAEN_MCAHandle};
  // Initialize the handle to a valid, reliable state
  hmca->library_handle = CAEN_library_handle;
  hmca->board_handle   = board_handle;
  hmca->channel_handles= std::move(handles);

  // TODO fill out user preferred values if specified

  return hmca;
}

std::string DecodeCAENRetCode(int error_code) noexcept {
  std::string message;
  DecodeCAENRetCode(error_code, message);
  return message;
}

void DecodeCAENRetCode(int error_code, std::string & message) noexcept {
  switch (error_code) {
  case CAENDPP_RetCode_Ok:
    message = "Operation completed successfully";
    break;
  case CAENDPP_RetCode_GenericError:
    message = "Unspecified error";
    break;
  case CAENDPP_RetCode_TooManyInstances:
    message = "Too many instances";
    break;
  case CAENDPP_RetCode_ProcessFail:
    message = "Process fail";
    break;
  case CAENDPP_RetCode_ReadFail:
    message = "Read fail";
    break;
  case CAENDPP_RetCode_WriteFail:
    message = "Write fail";
    break;
  case CAENDPP_RetCode_BadMessage:
    message = "Invalid response";
    break;
  case CAENDPP_RetCode_InvalidHandle:
    message = "Invalid library handle";
    break;
  case CAENDPP_RetCode_ConfigError:
    message = "Configuration error";
    break;
  case CAENDPP_RetCode_BoardInitFail:
    message = "Board Init failed";
    break;
  case CAENDPP_RetCode_TimeoutError:
    message = "Timeout error";
    break;
  case CAENDPP_RetCode_InvalidParameter:
    message = "Invalid parameter";
    break;
  case CAENDPP_RetCode_NotInWaveMode:
    message = "Not in Waveforms Mode";
    break;
  case CAENDPP_RetCode_NotInHistoMode:
    message = "Not in Histogram Mode";
    break;
  case CAENDPP_RetCode_NotInListMode:
    message = "Not in List Mode";
    break;
  case CAENDPP_RetCode_NotYetImplemented:
    message = "Not yet implemented";
    break;
  case CAENDPP_RetCode_BoardNotConfigured:
    message = "Board not configured";
    break;
  case CAENDPP_RetCode_InvalidBoardIndex:
    message = "Invalid board index";
    break;
  case CAENDPP_RetCode_InvalidChannelIndex:
    message = "Invalid channel index";
    break;
  case CAENDPP_RetCode_UnsupportedFirmware:
    message = "Invalid board firmware";
    break;
  case CAENDPP_RetCode_NoBoardsAdded:
    message = "No board added";
    break;
  case CAENDPP_RetCode_AcquisitionRunning:
    message = "Acquisition Status is not compliant with the function called";
    break;
  case CAENDPP_RetCode_OutOfMemory:
    message = "Out of memory";
    break;
  case CAENDPP_RetCode_BoardChannelIndex:
    message = "Invalid board channel index";
    break;
  case CAENDPP_RetCode_HistoAlloc:
    message = "No valid histogram allocated";
    break;
  case CAENDPP_RetCode_OpenDumper:
    message = "Error opening the list dumper";
    break;
  case CAENDPP_RetCode_BoardStart:
    message = "Error starting acquisition for a board";
    break;
  case CAENDPP_RetCode_ChannelEnable:
    message = "The given channel is not enabled";
    break;
  case CAENDPP_RetCode_InvalidCommand:
    message = "Invalid command";
    break;
  case CAENDPP_RetCode_NumBins:
    message = "Invalid number of bins";
    break;
  case CAENDPP_RetCode_HistoIndex:
    message = "Invalid Hitogram Index";
    break;
  case CAENDPP_RetCode_UnsupportedFeature:
    message = "The feature is not supported by the given board/channel";
    break;
  case CAENDPP_RetCode_BadHistoState:
    message = "The given histogram is an invalid state (e.g. 'done' while it shouldn't)";
    break;
  case CAENDPP_RetCode_NoMoreHistograms:
    message = "Cannot switch to ext histo, no more histograms available";
    break;
  case CAENDPP_RetCode_NotHVBoard:
    message = "The selected board doesn't support HV Channels";
    break;
  case CAENDPP_RetCode_InvalidHVChannel:
    message = "Invalid HV channel index";
    break;
  case CAENDPP_RetCode_SocketSend:
    message = "Error Sending Message through Socket";
    break;
  case CAENDPP_RetCode_SocketReceive:
    message = "Error Receiving Message from Socket";
    break;
  case CAENDPP_RetCode_BoardThread:
    message = "Cannot get Board's acquisition thread";
    break;
  case CAENDPP_RetCode_DecodeWaveform:
    message = "Cannot decode waveform from buffer";
    break;
  case CAENDPP_RetCode_OpenDigitizer:
    message = "Error Opening the digitizer";
    break;
  case CAENDPP_RetCode_BoardModel:
    message = "Requested a feature incompatible with board's Manufacture";
    break;
  case CAENDPP_RetCode_AutosetStatus:
    message = "Autoset Status is not compliant with the requested feature";
    break;
  case CAENDPP_RetCode_Autoset:
    message = "Autoset error looking for signal parameters";
    break;
  case CAENDPP_RetCode_Calibration:
    message = "Calibration Error";
    break;
  case CAENDPP_RetCode_EventRead:
    message = "Event read error";
    break;
  default:
    message = "CAEN unknown error";
    break;
  }
}


}   //namespace maxrf::daq
