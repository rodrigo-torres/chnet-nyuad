#include "include/image_display.h"
#include "include/xrfimage.h"

//XRFImage xrf_image {};
//extern int *shared_memory;

//static bool UpdateEnergyCalibrationParameters()
//{
//  bool valid {false};
//  QString filename = QDir::currentPath();
//  filename.append("/conf/Calibration.txt");

//  QFile file {filename};
//  if (file.open(QIODevice::ReadOnly))
//  {
//    auto str = file.readLine();
//    energy_calibration_param1 = str.toDouble();
//    str = file.readLine();
//    energy_calibration_param0 = str.toDouble();
//    valid = (energy_calibration_param1 == 0) ? false : true;
//  }
//  file.close();
//  return valid;
//}

//static double ConvertEnergyToChannel(double energy)
//{
//  energy -= energy_calibration_param0;
//  energy /= energy_calibration_param1;
//  return energy;
//}

//static double ConvertBinToEnergy(double bin)
//{
//  bin *= energy_calibration_param0;
//  bin += energy_calibration_param1;
//  bin = (bin > 16384) ? 16383 : (bin < 0) ? 0 : floor(bin);
//  return bin;
//}

