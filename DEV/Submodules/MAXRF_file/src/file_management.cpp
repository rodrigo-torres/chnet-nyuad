/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "file_management.hpp"
#include "file_formats.hpp"

namespace maxrf {

std::map<std::string, DataFormat> DataFileHander::formats_ =
{
  {"{", DataFormat::kEDF},
  {"ver.001", DataFormat::kMultiDetectorMaskedDump},
  {"<?xml version=\"1.0\" encoding=\"UTF-8\"?>", DataFormat::kXMLFile},
};

DataFileHander::DataFileHander() {}

auto DataFileHander::GetFile(std::string filepath) -> std::shared_ptr<MAXRFDataFile>
{
  std::fstream file{filepath};
  if (!file.is_open()) {
    return std::make_shared<NullFile>();
  }

  constexpr short kMaxSizeFirstLine = 50;
  std::string line(kMaxSizeFirstLine, '\0');
  file.read(line.data(), kMaxSizeFirstLine);

  line.erase(line.rfind('\n'), std::string::npos);

  DataFormat format{DataFormat::kInvalid};
  auto it = formats_.find(line);
  if (it == formats_.end()) {
    return std::make_shared<NullFile>();
  }
  else {
    format = it->second;
  }

  if (format == DataFormat::kXMLFile) {
    // We need to further differentiate the format
    constexpr short kMaxParseLength = 4096;
    short characters_parsed{0};
    while (getline(file,  line) && characters_parsed < kMaxParseLength) {
      characters_parsed += static_cast<short>(file.gcount());

      if (line.find("Hypercube") != std::string::npos) {
        // A 'Format' node containing a PC data child possibly exists
        format = (line.find("HypercubeFiltered") == std::string::npos) ?
              DataFormat::kHypercube : DataFormat::kHypercubeFiltered;
        break;
      }
    }
  }

  // Return to the beginning of the file
  file.seekg(0, std::ios::beg);

  switch (format) {
  case DataFormat::kHypercube:
    return std::make_shared<HypercubeFile>(std::move(file));
  case DataFormat::kHypercubeFiltered:
    // Fallthrough cases have not yet been implemented
    [[fallthrough]];
  case DataFormat::kEDF:
    [[fallthrough]];
  case DataFormat::kMultiDetectorMaskedDump:
    [[fallthrough]];
  case DataFormat::kXMLFile:
    [[fallthrough]];
  case DataFormat::kLegacy:
    [[fallthrough]];
  case DataFormat::kHDF5:
    [[fallthrough]];
  case DataFormat::kInvalid:
    return std::make_shared<NullFile>();
  }
}

MAXRFDataFile::MAXRFDataFile(DataFormat const format) : format_{format} {}
MAXRFDataFile::~MAXRFDataFile() {}

///  CHECK OPERATIONS
auto  MAXRFDataFile::GetFormat() const noexcept ->  DataFormat {
  return format_;
}

/// FILE WRITING API
//auto MAXRFDataFile::WritePixel(PixelData & data) ->bool {
//  std::cout << "This functionality is not implemented in this data format"
//            << std::endl;
//  return false;
//}

/// HEADER MANIPULATION API
//auto MAXRFDataFile::MakeDefaultHeader() -> bool {
//  std::cout << "This functionality is not implemented in this data format"
//            << std::endl;
//  return false;
//}
auto MAXRFDataFile::ExtractHeader() ->std::string {
  std::cout << "This functionality is not implemented in this data format"
            << std::endl;
  return "";
}
//auto MAXRFDataFile::GetTokenValue(HeaderTokens token) ->std::string {
//  std::cout << "This functionality is not implemented in this data format"
//            << std::endl;
//  return "";
//}
//auto MAXRFDataFile::EditToken(HeaderTokens token, std::string val) ->bool {
//  std::cout << "This functionality is not implemented in this data format"
//            << std::endl;
//  return false;
//}

/// FILE CONVERSION API
auto MAXRFDataFile::ConvertToHypercube() ->bool {
  std::cout << "This functionality is not implemented in this data format"
            << std::endl;
  return false;
}
//auto MAXRFDataFile::ConvertToPyMcaSPE() ->bool {
//  std::cout << "This functionality is not implemented in this data format"
//            << std::endl;
//  return false;
//}
//auto MAXRFDataFile::ConvertToPyMcaEDF() ->bool {
//  std::cout << "This functionality is not implemented in this data format"
//            << std::endl;
//  return  false;
//}
//auto MAXRFDataFile::ConvertToHDF5() ->bool {
//  std::cout << "This functionality is not implemented in this data format"
//            << std::endl;
//  return false;
//}
auto MAXRFDataFile::ExtractHistograms() ->bool {
  std::cout << "This functionality is not implemented in this data format"
            << std::endl;
  return  false;
}

/// LOOK-UP TABLE COMPUTATION
//auto MAXRFDataFile::ComputeLookupTable() ->LookupTable {
//  std::cout << "This functionality is not implemented in this data format"
//            << std::endl;
//  return LookupTable{};
//}

/// INTEGRITY CHECKS
auto MAXRFDataFile::CheckIntegrity() ->std::string {
  std::cout << "This functionality is not implemented in this data format"
            << std::endl;
  return "";
}




} //namespace maxrf
