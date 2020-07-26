#include "file_converter.h"
#include "pugi/pugixml.hpp"

namespace maxrf {

auto HeaderUtilities::IsHypercube() -> bool
{
  std::string line;
  // Two conditions for being a HyperCube. First, the file is an xml file
  // We check if the first line in the file corresponds to a declaration node
  std::getline(data_file_, line);

  // Care should be placed to make this line match with the header_versions map
  if (line.compare("<?xml version=\"1.0\" encoding=\"UTF-8\"?>") != 0) {
    //  The file is not XML and not a HyperCube
    data_file_.seekg(0, std::ios::beg);
    return false;
  }
  header_.append(line);
  header_.push_back('\n');

  // We skip a line because of a bit of a bug, we want to skip the <XRFAnalysis>
  //  node, which is only closed at the end of the file
  std::getline(data_file_, line);

  // Then we extract the whole header onto a buffer
  while (std::getline(data_file_, line))
  {
    if (line.compare("<Analysis_Data>") == 0) {
      break;
    }
    header_.append(line);
    header_.push_back('\n');
  }
  data_file_.seekg(0, std::ios::beg);

  pugi::xml_document doc;
  // TODO use the parse result to inform the user about invalid formatting
  pugi::xml_parse_result ret = doc.load_string(header_.c_str());
  // We tell pugi to find the node 'Format' and return the value of its first
  // plain character data node. If it exists, the return will be a non-empty
  // string
  line = doc.child("Analysis_Header").child("Format").child_value();

  return line.compare("Hypercube") == 0 ? true : false;
}

} // namespace maxrf

