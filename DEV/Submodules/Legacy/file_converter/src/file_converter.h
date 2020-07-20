/*
 * conversion_routines.h
 *
 *  Created on: Jun 5, 2020
 *      Author: rt135
 */

#ifndef INCLUDE_CONVERSION_ROUTINES_H_
#define INCLUDE_CONVERSION_ROUTINES_H_

#include <cstdio>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

#include <sys/stat.h>

#include <QObject>

#include "maxrf_libraries_config.h"
#include "pugi/pugixml.hpp"

namespace maxrf
{
class MAXRF_LIBRARIES_SHARED_EXPORT FileConverter : public QObject
{
  Q_OBJECT
  using ReadFile	=	std::ifstream;


public:
  // Default constructors
  FileConverter();
  FileConverter(std::string const & filename);

  // Conversion methods
  auto ConvertToHypercube() -> bool;
  auto ConvertToPyMcaSPE() -> std::string;
  auto ConvertToPyMcaEDF() -> std::string;
  auto ComputeHistograms() -> std::string;

  auto converted_filepath() -> std::string;
  auto err_message() -> std::string;
signals:
  // The Qt Meta-Object compiler (moc) does not like trailing return types
  void UpdateProgressBar(int value);
  void UpdateStatusBar(std::string message);

private:
  // Helper Methods
  auto OpenFile(std::string filename) -> void;

  // XRF Image Data formats to other XRF Image Data formats
  auto LegacyFormatToHypercube() -> void;
  auto MultiDetectorFormatToHypercube() -> void;
  auto OptimizedSingleDetectorFormatToHypercube() -> void;

  // XRF Image Data formats to XRF Spectrum format
  auto HypercubeToHistogram() -> void;

  // Internal data formats to PyMca data formats
  auto HypercubeToPyMcaSPE() -> void;
  auto HypercubeToPyMcaEDF() -> void;

  bool error;
  std::string err_message_;
  std::map<std::string, int> header_versions;
  std::string filename_;
  std::ifstream data_file_;

  std::string header_;
};

class MAXRF_LIBRARIES_SHARED_EXPORT HeaderUtilities
{
public:
  HeaderUtilities();

  auto IsHypercube() -> bool;


private:
  std::fstream data_file_;
  std::fstream::pos_type header_end_;
  std::string header_;
};

} // namespace maxrf

#endif /* INCLUDE_CONVERSION_ROUTINES_H_ */
