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

#ifndef DAQ_TYPES_H
#define DAQ_TYPES_H

#include <map>

#include "MAXRF/file_management.hpp"

namespace maxrf::daq {
///
/// SpectralData is an alias guaranteeing data packets shared across threads
/// have the same data type, in this case std::vector<int32_t>
///
using SpectralData  = HypercubeFile::PixelData;

constexpr static int kSpectralBins {16384};

enum class MCAConnectionType : char {
  kUSB          = 0x0,
  kOpticalLink  = 0x1,
  kSerial       = 0x3
};

///
/// \brief The DAQMode enum the mode of acquisition for a given DAQ session
///
enum class DAQMode : char {
  kDAQInvalid,
  kDAQPoint,
  kDAQScan
};

enum class MCAInputRange : char {

};

///
/// \brief The RCCR2Smoothing enum
///
enum class RCCR2Smoothing : int {
  k1Sample    = 0x1,
  k2Samples   = 0x2,
  k4Samples   = 0x4,
  k8Samples   = 0x8,
  k16Samples  = 0x10,
  k32Samples  = 0x20
};

///
/// \brief The MCAConnectionParameters struct
///
struct MCAConnectionParameters {
  MCAConnectionType type  {MCAConnectionType::kUSB};
  int32_t device_number     {0};
  int32_t daisy_chain_node  {0};
  uint32_t vme_base_address  {0};
};

///
/// \brief The MCAChannelParameters struct controls the pulse-height analysis
/// parameters of an MCA channel
///
/// \var MCAChannelParameters::trig_threshold
/// The trigger threshold, in ADC bin units ranging from 0 to 65535 inclusive
/// \var MCAChannelParameters::trig_smoothing
/// Number of samples to take for the average filter of the RC-CR2 signal, it
/// can only take a value from \see RCCR2Smoothing
/// \var MCAChannelParameters::trig_holdoff
/// Specifies the duration to block signals after an RC-CR2 trigger
/// \var MCAChannelParameters::sig_rise
///
/// \var MCAChannelParameters::trap_decay
///
/// \var MCAChannelParameters::trap_flat
///
/// \var MCAChannelParameters::trap_peak
///
/// \var MCAChannelParameters::trap_rise
///
/// \var MCAChannelParameters::peak_samples
///
/// \var MCAChannelParameters::peak_holdoff
///
/// \var MCAChannelParameters::baseline_samples
///
/// \var MCAChannelParameters::baseline_holdoff
///
/// \var MCAChannelParameters::decimation
///
/// \var MCAChannelParameters::gain_coarse
///
/// \var MCAChannelParameters::gain_fine
///
/// \var MCAChannelParameters::dc_offset
///
///
struct MCAChannelParameters {
  bool use_defaults {true};
  int32_t trig_threshold;
  int32_t trig_smoothing;
  int32_t trig_holdoff;
  int32_t sig_rise;
  int32_t trap_decay;
  int32_t trap_flat;
  int32_t trap_peak;
  int32_t trap_rise;
  int32_t peak_samples;
  int32_t peak_holdoff;
  int32_t baseline_samples;
  int32_t baseline_holdoff;
  int32_t decimation;
  int32_t gain_coarse;
  double gain_fine;
  double dc_offset;
  MCAInputRange input_range;
};

struct MCABoardConfiguration {
  MCAConnectionParameters conn;
  std::map<uint8_t, MCAChannelParameters> channels;
  uint8_t board_id;
};


struct DAQModeParameters {
  DAQMode mode { DAQMode::kDAQInvalid };
  double timeout {0.};
  int x_start_coordinate;
  int y_start_coordinate;
  int x_end_coordinate;
  int y_end_coordinate;
  int x_motor_step;
  int y_motor_step;
  int motor_velocity;
};

struct DAQInitParameters {
  std::vector<MCABoardConfiguration> boards_config {};
  std::map<std::string, std::string> user_header_fields {};
  DAQModeParameters mode_parameters {};

  // Output file parameters
  std::string output_path   {"."};
  std::string base_filename {"test"};
};

struct DataPacket {
  int32_t pixel_no;
  int32_t channel_id;
  SpectralData m_buffer;
};


} // namespace maxrf::daq

#endif // DAQ_TYPES_H
