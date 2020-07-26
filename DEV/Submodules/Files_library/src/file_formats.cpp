#include "file_formats.hpp"

namespace maxrf {

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


} // namespace maxrf
