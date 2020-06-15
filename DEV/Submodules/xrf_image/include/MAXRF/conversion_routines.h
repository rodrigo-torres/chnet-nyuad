/*
 * conversion_routines.h
 *
 *  Created on: Jun 5, 2020
 *      Author: rt135
 */

#ifndef INCLUDE_CONVERSION_ROUTINES_H_
#define INCLUDE_CONVERSION_ROUTINES_H_

#include <fstream>
#include <cstdio>
#include <string>
#include <vector>

#include <QObject>

class FileConverter : public QObject
{
	Q_OBJECT
	using ReadFile	=	std::ifstream;


public:
	// Default constructors
  FileConverter();
  FileConverter(std::string const & filename);

  // Check operations
  auto IsHyperCube(std::ifstream & file) -> bool;

	// Conversion methods
	auto ConvertToHypercube() -> std::string;
	auto ConvertToPyMcaSPE() -> std::string;
	auto ConvertToPyMcaEDF() -> std::string;
	auto ComputeHistograms() -> std::string;

	auto err_message() -> std::string;

signals:
  // The Qt Meta-Object compiler (moc) does not like trailing return types
  void UpdateProgressBar(int value);
  void UpdateStatusBar(std::string message);

private:
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


#endif /* INCLUDE_CONVERSION_ROUTINES_H_ */
