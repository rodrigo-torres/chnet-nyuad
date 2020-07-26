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

CAENLibraryHandle::CallbacksList const CAENLibraryHandle::default_boards_config_ {
  CAENLibraryHandle::ConfigureBoard0
};

///
/// \brief PercentageOffsetToInt helper function to converte a desired DC offset
/// into the correct integral value
/// \param percentage
/// \return
///
static int PercentageOffsetToInt(double percentage){
  static constexpr int kADCMaxChannel = 65535;
  if (std::abs(percentage) > 50.) {
    percentage = 50. * (std::signbit(percentage) ? -1 : 1);
  }
  percentage = (percentage + 50) / 100.;
  return static_cast<int>(kADCMaxChannel * percentage);
};

///
/// \brief ApplyCommonDefaultValues sets the correct values for the parameters
/// that all common to all boards
/// \param params
///
static void ApplyCommonDefaultValues(DigitizerParams & params) {

//  auto & params = config.digitizer_params;
  // set listMode default parameters
  params.ListParams.enabled = TRUE;
  params.ListParams.saveMode = CAENDPP_ListSaveMode_Memory;
  strcpy(params.ListParams.fileName, "UNNAMED");
  params.ListParams.maxBuffNumEvents = kEventsBufferSize; //8192
//#define DUMP_MASK_TTT           (0x1)
//#define DUMP_MASK_ENERGY        (0x2)
//#define DUMP_MASK_EXTRAS        (0x4)
//#define DUMP_MASK_ENERGYSHORT   (0x8)
  params.ListParams.saveMask = 0x2;  // Only the energy

  // default board parameters
  params.EventAggr = 0;
  params.IOlev = CAENDPP_IOLevel_TTL;

  // Generic Writes to Registers
  params.GWn = 0;                                    // Number of Generic Writes
//  memset(Params->GWaddr, 0, MAX_GW*sizeof(uint32_t)); // List of addresses (length = 'GWn')
//  memset(Params->GWdata, 0, MAX_GW*sizeof(uint32_t)); // List of datas (length = 'GWn')
//  memset(Params->GWmask, 0, MAX_GW*sizeof(uint32_t)); // List of masks (length = 'GWn')

  // Waveform parameters default settings
  params.WFParams.dualTraceMode = 1;
  params.WFParams.vp1 = CAENDPP_PHA_VIRTUALPROBE1_Input;
  params.WFParams.vp2 = CAENDPP_PHA_VIRTUALPROBE2_TrapBLCorr;
  params.WFParams.dp1 = CAENDPP_PHA_DigitalProbe1_Peaking;
  params.WFParams.dp2 = CAENDPP_PHA_DigitalProbe2_Trigger;
  params.WFParams.recordLength = (int32_t)(8192);
  params.WFParams.preTrigger = (int32_t)(500);
  params.WFParams.probeSelfTriggerVal = 200;
  params.WFParams.probeTrigger = CAENDPP_PHA_PROBETRIGGER_MainTrig;

  for (int ch = 0; ch < MAX_NUMCHB; ++ch) {
    params.PulsePolarity[ch] = CAENDPP_PulsePolarityPositive;

    // Coicidence parameters between channels. Do not remove this is important
    params.CoincParams[ch].CoincChMask = 0x0;
    params.CoincParams[ch].CoincLogic = CAENDPP_CoincLogic_None;
    params.CoincParams[ch].CoincOp = CAENDPP_CoincOp_OR;
    params.CoincParams[ch].MajLevel = 0;
    params.CoincParams[ch].TrgWin = 0;
  }

}

auto CAENLibraryHandle::ConfigureBoard0() noexcept -> ConfigCallback::result_type {
  DigitizerParams params;
  ApplyCommonDefaultValues(params);

  // The channel mask is computed by the GetBoardHandle method of the
  // CAENLibraryHandle class;
  // params.ChannelMask = 0xF;

  // Set the PHA parameters
  for (int ch = 0; ch < 2; ++ch) {
    params.DCoffset[ch] = PercentageOffsetToInt(40.);
    params.DPPParams.M[ch] = 9000;        // Signal Decay Time Constant
    params.DPPParams.m[ch] = 5000;         // Trapezoid Flat Top
    params.DPPParams.k[ch] = 2000;         // Trapezoid Rise Time
    params.DPPParams.ftd[ch] = 1600;       // Flat Top Delay
    params.DPPParams.a[ch] = 2;            // Trigger Filter smoothing factor
    params.DPPParams.b[ch] = 100;          // Input Signal Rise time
    params.DPPParams.thr[ch] = 300;       // Trigger Threshold
    params.DPPParams.nsbl[ch] = 3;         // Number of Samples for Baseline Mean
    params.DPPParams.nspk[ch] = 1;         // Number of Samples for Peak Mean Calculation
    params.DPPParams.pkho[ch] = 1000;         // Peak Hold Off
    params.DPPParams.blho[ch] = 200;      // Base Line Hold Off
    params.DPPParams.dgain[ch] = 0;        // Digital Probe Gain
    params.DPPParams.enf[ch] = 4.0;        // Energy Normalization Factor
    params.DPPParams.decimation[ch] = 0;   // Decimation of Input Signal
    params.DPPParams.trgho[ch] = 1300;      // Trigger Hold Off
  }

  return params;
}

}  // namespace maxrf::daq
