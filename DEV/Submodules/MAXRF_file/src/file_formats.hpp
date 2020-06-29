#ifndef FILE_FORMATS_HPP
#define FILE_FORMATS_HPP

#include "file_management.hpp"

namespace maxrf {

class MAXRF_LIBRARIES_SHARED_EXPORT NullFile : public MAXRFDataFile
{
public:
  NullFile() : MAXRFDataFile{DataFormat::kInvalid} {}
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
  XMLFile() : MAXRFDataFile{DataFormat::kXMLFile} {}


  /// HEADER MANIPULATION API
  auto ExtractHeader() -> std::string override;
//  auto GetTokenValue(HeaderTokens token) -> std::string override;

  /// FILE CONVERSION API
  auto ConvertToHypercube() -> bool override;
  auto ExtractHistograms() -> bool override;

  /// INTEGRITY CHECKS
  auto CheckIntegrity() -> std::string override;
private:
  std::fstream file_;
};

class MAXRF_LIBRARIES_SHARED_EXPORT MaskedDumpFile : public MAXRFDataFile
{
public:
  MaskedDumpFile() : MAXRFDataFile{DataFormat::kMultiDetectorMaskedDump} {}

  /// HEADER MANIPULATION API
  auto ExtractHeader() -> std::string override;
//  auto GetTokenValue(HeaderTokens token) -> std::string override;

  /// FILE CONVERSION API
  auto ConvertToHypercube() -> bool override;
  auto ExtractHistograms() -> bool override;

  /// INTEGRITY CHECKS
  auto CheckIntegrity() -> std::string override;

private:
  std::fstream file_;
};

//class MAXRF_LIBRARIES_SHARED_EXPORT HypercubeFile : public MAXRFDataFile
//{
//public:

//  HypercubeFile(std::fstream && f);

//  /// DIRECT ACCESS
//  auto DataFile() -> std::fstream & override {
//    return file_;
//  }

//  /// FILE WRITING API
//  auto WritePixel(PixelData & data) -> bool override;

//  /// HEADER MANIPULATION API
//  auto MakeDefaultHeader() ->  bool override {
//    std::cout << "This functionality is not yet implemented"
//              << std::endl;
//    return false;
//  }
//  auto ExtractHeader() -> std::string override;
//  auto GetTokenValue(HeaderTokens token) -> std::string override;
//  auto EditToken(HeaderTokens token, std::string val) -> bool override {
//    std::cout << "This functionality is not yet implemented"
//              << std::endl;
//    return false;
//  }

//  /// FILE CONVERSION API
//  auto ConvertToPyMcaEDF() -> bool override {
//    std::cout << "This functionality is not yet implemented"
//              << std::endl;
//    return false;
//  }
//  auto ConvertToHDF5() -> bool override {
//    std::cout << "This functionality is not yet implemented"
//              << std::endl;
//    return false;
//  }
//  auto ExtractHistograms() -> bool override {
//    std::cout << "This functionality is not yet implemented"
//              << std::endl;
//    return false;
//  }

//  /// LOOK-UP TABLE COMPUTATION
//  auto ComputeLookupTable() -> LookupTable override;

//  /// INTEGRITY CHECKS
//  auto CheckIntegrity() -> std::string override {
//    std::cout << "This functionality is not yet implemented"
//              << std::endl;
//    return "";
//  }
//private:
//  std::fstream file_;
//  std::fstream::pos_type first_datum_pos_;

//  pugi::xml_document header_node_;
//};


}

#endif // FILE_FORMATS_HPP
