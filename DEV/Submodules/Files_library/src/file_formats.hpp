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
