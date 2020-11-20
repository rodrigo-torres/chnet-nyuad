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
  params.EventAggr = 1;   // Number of events in one aggregate (0 = automatic)
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
  params.WFParams.recordLength = (int32_t)(20000); //Originally 8192, 20000 in CoMPASS
  params.WFParams.preTrigger = (int32_t)(2000); // Originally 500, 2000 in CoMPASS
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
//  for (int ch = 0; ch < 2; ++ch) {
//    params.DCoffset[ch] = PercentageOffsetToInt(20.);
//    params.DPPParams.M[ch] = 4000;        // Signal Decay Time Constant
//    params.DPPParams.m[ch] = 2000;         // Trapezoid Flat Top
//    params.DPPParams.k[ch] = 2000;         // Trapezoid Rise Time
//    params.DPPParams.ftd[ch] = 1600;       // Flat Top Delay
//    params.DPPParams.a[ch] = 2;            // Trigger Filter smoothing factor NOT SAME AS CAEN
//    params.DPPParams.b[ch] = 100;          // Input Signal Rise time
//    params.DPPParams.thr[ch] = 1800;       // Trigger Threshold
//    params.DPPParams.nsbl[ch] = 3;         // Number of Samples for Baseline Mean
//    params.DPPParams.nspk[ch] = 2;         // Number of Samples for Peak Mean Calculation NOT SAME AS CAEN
//    params.DPPParams.pkho[ch] = 960;        // Peak Hold Off
//    params.DPPParams.blho[ch] = 200;      // Base Line Hold Off CAN'T FIND IN CAEN
//    params.DPPParams.dgain[ch] = 3;        // Digital Probe Gain WAS 0
//    params.DPPParams.enf[ch] = 1.0;        // Energy Normalization Factor
//    params.DPPParams.decimation[ch] = 0;   // Decimation of Input Signal
//    params.DPPParams.trgho[ch] = 1300;      // Trigger Hold Off
//  }

  params.DCoffset[0] = PercentageOffsetToInt(30.);
  params.DPPParams.M[0] = 3500;        // Signal Decay Time Constant
  params.DPPParams.m[0] = 2000;         // Trapezoid Flat Top
  params.DPPParams.k[0] = 2000;         // Trapezoid Rise Time
  params.DPPParams.ftd[0] = 1600;       // Flat Top Delay
  params.DPPParams.a[0] = 2;            // Trigger Filter smoothing factor NOT SAME AS CoMPASS
  params.DPPParams.b[0] = 200; //200    // Input Signal Rise time
  params.DPPParams.thr[0] = 1000;  //1200     // Trigger Threshold
  params.DPPParams.nsbl[0] = 3;         // Number of Samples for Baseline Mean
  params.DPPParams.nspk[0] = 16;        // Number of Samples for Peak Mean Calculation NOT SAME AS CoMPASS
  params.DPPParams.pkho[0] = 10000;        // Peak Hold Off
  params.DPPParams.blho[0] = 200;      // Base Line Hold Off CAN'T FIND IN CAEN
  params.DPPParams.dgain[0] = 0;        // Digital Probe Gain WAS 0
  params.DPPParams.enf[0] = 1;        // Energy Normalization Factor
  params.DPPParams.decimation[0] = 0;   // Decimation of Input Signal
  params.DPPParams.trgho[0] = 480;      // Trigger Hold Off

  params.DCoffset[1] = PercentageOffsetToInt(20.);
  params.DPPParams.M[1] = 8000;        // Signal Decay Time Constant
  params.DPPParams.m[1] = 2000;         // Trapezoid Flat Top
  params.DPPParams.k[1] = 2000;         // Trapezoid Rise Time
  params.DPPParams.ftd[1] = 1600;       // Flat Top Delay
  params.DPPParams.a[1] = 2;            // Trigger Filter smoothing factor NOT SAME AS CoMPASS
  params.DPPParams.b[1] = 100;          // Input Signal Rise time
  params.DPPParams.thr[1] = 800;       // Trigger Threshold
  params.DPPParams.nsbl[1] = 3;         // Number of Samples for Baseline Mean
  params.DPPParams.nspk[1] = 1;         // Number of Samples for Peak Mean Calculation NOT SAME AS CoMPASS
  params.DPPParams.pkho[1] = 10000;        // Peak Hold Off
  params.DPPParams.blho[1] = 200;      // Base Line Hold Off CAN'T FIND IN CoMPASS
  params.DPPParams.dgain[1] = 1;        // Digital Probe Gain WAS 0
  params.DPPParams.enf[1] = 1.0;        // Energy Normalization Factor
  params.DPPParams.decimation[1] = 0;   // Decimation of Input Signal
  params.DPPParams.trgho[1] = 480;      // Trigger Hold Off

  // Notes
  // Event aggregate, 0 is for automatic, set to 1
  // The gain for the Florence INFN detector is different
  // Number of samples for the peak seems not to be an enum value
  // Digital probe gain value of 0 actually corresponds to 1? enum value?
  // Record length and pre-trigger should only matter for the waveform mode

  // Additional notes further modifications
  // Peak at 0 channels is an undershoot of the ADC -> DC offset modified to 30 from 20
  // Threshold is still perhaps too low -> pushing up to 1200 from 800
  // Skewness of peaks is likely due to rise time of convolution operation -> input rise time up to 200 from 100


//  DPPParams[b].thr[ch] =  200;     //Trigger Threshold //previously set to 10000
//  DPPParams[b].k[ch] =  1000;    //Trapezoid Rise Time (ns)
//  DPPParams[b].m[ch] = 2500;      //Trapezoid Flat Top  (ns)//previously set to 300000
//  DPPParams[b].M[ch] = 8000;    //Decay Time Constant (ns) HACK-FPEP the one expected from fitting algorithm?
//  DPPParams[b].ftd[ch] = 1200;    //Peaking delay  (ns)
//  DPPParams[b].a[ch] = 4;         //Trigger Filter smoothing factor (position in the menu)
//  DPPParams[b].b[ch] = 800;       //Delay(b)  // Input Signal Rise time (ns)
//  DPPParams[b].trgho[ch] = 1300; //Trigger Hold Off
//  DPPParams[b].nsbl[ch] = 6;      //Baseline mean del trapezio (ordine di comparsa nel menu)
//  DPPParams[b].nspk[ch] = 16;      //Peak mean (numero punti su cui mediare per peaking del trapezio)
//  DPPParams[b].pkho[ch] = 3500;   //Peak holdoff (ns) (min intervallo tra 2 trapezi...minimo=k+m)
//  DPPParams[b].blho[ch] = 10000;    //Baseline holdoff del trapezio (ns) (intervallo dove non calcolare la baseline 0 sta per default=m+2*k)
//  DPPParams[b].enf[ch] = 1;      //Energy Normalization Factor or Trapezoid Gain
//  //DPPParams[b].tsampl[ch] = 10;
//  DPPParams[b].dgain[ch] = 0;
//  DPPParams[b].decimation[ch] = 0;

  // Note it's veeery strange that the return statement was commented outc
  return params;
}

}  // namespace maxrf::daq
