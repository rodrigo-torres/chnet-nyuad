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


#ifndef FILE_FORMATS_HPP
#define FILE_FORMATS_HPP

#include "file_management.hpp"

namespace maxrf {

class MAXRF_LIBRARIES_SHARED_EXPORT NullFile : public MAXRFDataFile
{
public:
  NullFile() {}
  auto CheckIntegrity() -> std::string override {
    std::cout << "This functionality is not yet implemented"
              << std::endl;
    return "";
  }
};

class MAXRF_LIBRARIES_SHARED_EXPORT EDFFile : public MAXRFDataFile
{

};

class MAXRF_LIBRARIES_SHARED_EXPORT XMLFile : public MAXRFDataFile
{
public:
  XMLFile() {}


  /// HEADER MANIPULATION API
  auto ExtractHeader() -> std::string override;
//  auto GetTokenValue(HeaderTokens token) -> std::string override;

  /// FILE CONVERSION API
  auto ConvertToHypercube() -> bool override;
  auto ExtractHistograms() -> bool override;

  /// INTEGRITY CHECKS
  auto CheckIntegrity() -> std::string override;
};

class MAXRF_LIBRARIES_SHARED_EXPORT MaskedDumpFile : public MAXRFDataFile
{
public:
  MaskedDumpFile(std::fstream && f) {
    SetFormat(DataFormat::kMultiDetectorMaskedDump);
    SetFileDevice(std::move(f));
  }

  /// HEADER MANIPULATION API
  auto ExtractHeader() -> std::string override {return std::string {};}

  /// FILE CONVERSION API
  auto ConvertToHypercube() -> bool override;
  auto ExtractHistograms() -> bool override { return false; }

  /// INTEGRITY CHECKS
  auto CheckIntegrity() -> std::string override {return std::string {};}
};

//class MAXRF_LIBRARIES_SHARED_EXPORT HypercubeFile : public MAXRFDataFile

}

#endif // FILE_FORMATS_HPP
