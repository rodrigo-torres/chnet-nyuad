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

#include <filesystem>

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

  line.erase(line.find('\n'), std::string::npos);

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
  case DataFormat::kMultiDetectorMaskedDump:
    return std::make_shared<MaskedDumpFile>(std::move(file));
  case DataFormat::kHypercubeFiltered:
    // Fallthrough cases have not yet been implemented
    [[fallthrough]];
  case DataFormat::kEDF:
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

auto DataFileHander::GetFileForDAQ(std::string path) -> std::shared_ptr<MAXRFDataFile> {
  std::error_code err;
  if (!std::filesystem::exists(std::filesystem::path {path}, err)) {
    if (err.default_error_condition() == err) {
    // We create the file
    std::ofstream {path};
    }
  }
  std::cout << std::filesystem::current_path() << std::endl;
  // Then we open after closure (guaranteeing the file is created)
  std::fstream file {path, std::fstream::out | std::fstream::trunc | std::fstream::binary };
  if (file.is_open()) {
    return std::make_shared<HypercubeFile>(std::move(file));
  }
  else {
    // The file wasn't created or we don't have access rights
    return std::make_shared<NullFile>();
  }
}

MAXRFDataFile::~MAXRFDataFile() {}

///  CHECK OPERATIONS
auto  MAXRFDataFile::GetFormat() const noexcept ->  DataFormat {
  return format_;
}

bool HypercubeFile::MakeDefaultHeader() {

  header_node_.reset();
//  auto node = doc.append_child(pugi::node_declaration);
//  node.append_attribute("version").set_value("1.0");
//  node.append_attribute("encoding").set_value("UTF-8");

  auto root = header_node_.append_child("Analysis_Header");
  root.append_child("Format").append_child(pugi::node_pcdata).set_value("Hypercube");
  root.append_child("File_Url");

  // OBJECT CATEGORY
  root = root.append_child("Object");
  root.text().set("Test");
  root.append_child("Name");
  root.append_child("Identifier");
  root.append_child("Description");

  root = root.parent().append_child("Analysis_Metadata");
  root.append_child("Analysis_Identifier");
  root.append_child("Analysis_Type");
  root.append_child("DAQ_Start_Timestamp");

  root = root.parent().append_child("Device_Info");
  root.append_child("Tube_Anode").append_child(pugi::node_pcdata).set_value("Molybdenum");
  root.append_child("Tube_Voltage").append_attribute("units").set_value("keV");
  root.append_child("Tube_Current").append_attribute("units").set_value("uA");

  root = root.parent().append_child("Image_Info");
  root.append_child("Width");
  root.append_child("Height");
  root.append_child("Pixels");

  root = root.parent().append_child("Histogram");
  root.append_child("Detector_Identifier");
  root.append_child("Spectral_Bins");
  root.append_child("Calibration_Param_0");
  root.append_child("Calibration_Param_1");
  root.append_child("Calibration_Param_2");

  return true;
}

bool HypercubeFile::WriteHeader() {
  file_ << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  file_ << "<XRFAnalysis>\n";
  header_node_.print(file_);
  file_ << "<Analysis_Data>\n";

  return true;
}


bool HypercubeFile::WritePixel(PixelData & data) {
  static auto const first_datum_pos { file_.tellp() };

  switch (write_mode) {
  case WriteMode::kDAQInvalid :
    // The writing mode must be chosen first
    return false;
  case WriteMode::kDAQPoint:
    file_.seekp(first_datum_pos);

    for (auto & counts : data.histogram) {
      file_ << counts << ',';
    }
    file_.seekp(file_.tellp() - std::fstream::off_type {1});
    file_ << '\n';
    return true;
  case WriteMode::kDAQScan :
    writer->WriteDataToFiles(data);
    return  true;
  }
}


bool HypercubeFile::WriteFooter() {
  if (writer) {
    file_ = writer->ReacquireFileOwnership();
    writer.reset();
    write_mode = WriteMode::kDAQInvalid;
    file_ << "</Analysis_Data>\n";
    // Perhaps write a footer
    file_ << "</XRFAnalysis>\n";
    return true;
  }
  else {
    return false;
  }
}

/// HEADER MANIPULATION API
auto MAXRFDataFile::ExtractHeader() ->std::string {
  std::cout << "This functionality is not implemented in this data format"
            << std::endl;
  return "";
}

/// FILE CONVERSION API
auto MAXRFDataFile::ConvertToHypercube() ->bool {
  std::cout << "This functionality is not implemented in this data format"
            << std::endl;
  return false;
}

auto MAXRFDataFile::ExtractHistograms() ->bool {
  std::cout << "This functionality is not implemented in this data format"
            << std::endl;
  return  false;
}

/// INTEGRITY CHECKS
auto MAXRFDataFile::CheckIntegrity() ->std::string {
  std::cout << "This functionality is not implemented in this data format"
            << std::endl;
  return "";
}

auto HypercubeFile::ConvertToPyMcaEDF() -> bool {
  std::cout << "This functionality is not yet implemented"
            << std::endl;
  return false;
}
auto HypercubeFile::ConvertToHDF5() -> bool {
  std::cout << "This functionality is not yet implemented"
            << std::endl;
  return false;
}
auto HypercubeFile::ExtractHistograms() -> bool {
  std::cout << "This functionality is not yet implemented"
            << std::endl;
  return false;
}

auto HypercubeFile::CheckIntegrity() -> std::string {
  std::cout << "This functionality is not yet implemented"
            << std::endl;
  return "";
}

HypercubeFile::HypercubeFile(std::fstream && _f) {
  SetFormat(DataFormat::kHypercube);
  SetFileDevice(std::move(_f));
}

auto HypercubeFile::GetTokenValue(HeaderTokens token) -> std::string
{
  static struct SearchPredicate {
    auto operator() [[maybe_unused]] (pugi::xml_node  _n) const -> bool {
      return strcmp(_n.name(), search_token.c_str()) == 0;
    }
    std::string search_token;
  } search;

  switch (token)
  {
  case HeaderTokens::kImageWidth  :
    search.search_token = "Width";
    break;
  case HeaderTokens::kImageHeight :
    search.search_token = "Height";
    break;
  case HeaderTokens::kImagePixels :
    search.search_token = "Pixels";
    break;
  case HeaderTokens::kDAQStartTimestamp :
    search.search_token = "DAQ_Start_Timestamp";
    break;
  }

  auto node = header_node_.find_node(search);
  if (node.type() == pugi::xml_node_type::node_null) {
    return "";
  }
  else {
    return node.child_value();
  }
}

auto HypercubeFile::GetTokenValue(std::string search_term) -> std::string
{
  static struct SearchPredicate {
    auto operator() [[maybe_unused]] (pugi::xml_node  _n) const -> bool {
      return strcmp(_n.name(), search_token.c_str()) == 0;
    }
    std::string search_token;
  } search;

  search.search_token = search_term;

  auto node = header_node_.find_node(search);
  if (node.type() == pugi::xml_node_type::node_null) {
    return "";
  }
  else {
    return node.child_value();
  }
}

auto HypercubeFile::EditToken(HeaderTokens token, std::string val) -> bool {
  static struct SearchPredicate {
    auto operator() (pugi::xml_node  _n) const -> bool {
      return strcmp(_n.name(), search_token.c_str()) == 0;
    }
    std::string search_token;
  } search;

  switch (token)
  {
  case HeaderTokens::kStartXCoordinate :
    search.search_token = "Width";
    break;
  case HeaderTokens::kStartYCoordinate :
    search.search_token = "Width";
    break;
  case HeaderTokens::kEndXCoordinate :
    search.search_token = "Width";
    break;
  case HeaderTokens::kEndYCoordinate :
    search.search_token = "Width";
    break;
  case HeaderTokens::kMotorStepX :
    search.search_token = "Width";
    break;
  case HeaderTokens::kMotorStepY :
    search.search_token = "Width";
    break;
  case HeaderTokens::kMotorVelocity :
    search.search_token = "Width";
    break;
  case HeaderTokens::kImageWidth  :
    search.search_token = "Width";
    break;
  case HeaderTokens::kImageHeight :
    search.search_token = "Height";
    break;
  case HeaderTokens::kImagePixels :
    search.search_token = "Pixels";
    break;
  case HeaderTokens::kDAQStartTimestamp :
    search.search_token = "DAQ_Start_Timestamp";
    break;
  }

  auto node = header_node_.find_node(search);
  if (node.type() == pugi::xml_node_type::node_null) {
    return false;
  }
  else {
    node.text().set(val.c_str());
    return true;
  }
}

auto HypercubeFile::EditToken(std::string search_term, std::string val) -> bool {
  static struct SearchPredicate {
    auto operator() (pugi::xml_node  _n) const -> bool {
      return strcmp(_n.name(), search_token.c_str()) == 0;
    }
    std::string search_token;
  } search;
  search.search_token = search_term;

  auto node = header_node_.find_node(search);
  if (node.type() == pugi::xml_node_type::node_null) {
    return false;
  }
  else {
    node.text().set(val.c_str());
    return true;
  }
}

auto HypercubeFile::ExtractHeader() -> std::string
{
  static std::string const kDataNodeStartTag = "<Analysis_Data>";

  file_.seekg(0, std::ios::beg);
  // Ignore the XML declaration
  file_.ignore(std::numeric_limits<char>::max(), '\n');
  // Ignore the root element
  file_.ignore(std::numeric_limits<char>::max(), '\n');
  // Local variables for header extraction
  std::string line;
  std::stringstream header;

  file_ >> std::skipws;
  while (file_ >> line)
  {
    if (line.compare(kDataNodeStartTag) == 0) {
      break;
    }
    header << line << '\n';
  }
  // The >> operator does not extract trailing whitespace
  file_.ignore();
  first_datum_pos_ = file_.tellg();

  auto parse_result = header_node_.load(header);
  if (parse_result.status != pugi::xml_parse_status::status_ok) {
    // The header is not valid. We could generate a report
  }

  return header.str();
}

auto HypercubeFile::ComputeLookupTable() -> std::shared_ptr<LookupTable>
{
  std::vector<LUTEntry> lut_data;

  file_.seekg(first_datum_pos_);
  while (file_.peek() != '<')
  {
    lut_data.emplace_back().datum_pos = file_.tellg();
    file_.ignore(std::numeric_limits<short>::max(), '\n');
  }

  size_t const width  = std::stoul(GetTokenValue(HeaderTokens::kImageWidth));
  size_t const height = std::stoul(GetTokenValue(HeaderTokens::kImageHeight));

  try {
    lut = std::make_shared<LookupTable>(std::move(lut_data), width, height);
    return lut;
 //   return LookupTable{std::move(lut_data), width, height};
  } catch (...) {
    std::rethrow_exception(std::current_exception());
  }
}





} //namespace maxrf
