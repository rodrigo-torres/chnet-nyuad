/*
 * conversion_routines.cpp
 *
 *  Created on: Jun 5, 2020
 *      Author: rt135
 */

#include "MAXRF/conversion_routines.h"
#include "pugi/pugixml.hpp"

FileConverter::FileConverter(std::string const & filename)
  : error{true}, err_message_{"[!] File hasn't been opened"}, data_file_{}
{
  OpenFile(filename);
  header_versions.emplace("{", 2);
  header_versions.emplace("ver.001",  1);
  header_versions.emplace("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", 3);
}

FileConverter::FileConverter()
{
  header_versions.emplace("{", 2);
  header_versions.emplace("ver.001",  1);
  header_versions.emplace("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", 3);
}

auto FileConverter::OpenFile(std::string filename) -> void
{
  filename_  = filename;

  error = filename_.empty();
  if (error)
  {
    err_message_ = "[!] Invalid filename";
  }
  else
  {
    data_file_.open(filename_);
    error = data_file_.is_open();
    if (error)
    {
      err_message_ = "[!] Can't open file";
    }
  }
}

auto FileConverter::err_message() -> std::string
{
  return err_message_;
}

auto FileConverter::converted_filepath() -> std::string
{
  return  filename_;
}

auto FileConverter::file() -> std::ifstream
{
  error = true;
  err_message_ = "[!] Attempting to access an invalid file stream";
  return std::move(data_file_);
}

auto FileConverter::AdvanceToEndOfHeader() -> void
{
  data_file_.seekg(end_of_header_pos_);
}

auto FileConverter::IsHypercube() -> bool
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
  // We return the fstream to its original state
  end_of_header_pos_ = data_file_.tellg();
  data_file_.seekg(0, std::ios::beg);

  pugi::xml_document doc;
  // TODO use the parse result to inform the user about invalid formatting
  pugi::xml_parse_result ret = doc.load_string(header_.c_str());
  // We tell pugi to find the node 'Format' and return the value of its first
  // plain character data node. If it exists, the return will be a non-empty
  // string
  line = doc.child("Analysis_Header").child("Format").child_value();
  if (line.compare("Hypercube") == 0) {
    // The format is correct
    return  true;
  }
  else {
    // We could attempt to change the format
    return false;
  }
}

auto FileConverter::ConvertToHypercube() -> bool
{
  // Get the first line on the file
  std::string buffer;
  std::getline(data_file_, buffer);

  int version{};
  auto it = header_versions.find(buffer);
  if ( it == header_versions.end() ) {
   version = std::numeric_limits<int>::max();
  }
  else {
    version = it->second;
  }

	switch (version)
	{
	case 0:
    error = true;
    LegacyFormatToHypercube();
		break;
	case 1:
    error = true;
		MultiDetectorFormatToHypercube();
		break;
	case 2:
		OptimizedSingleDetectorFormatToHypercube();
		break;
	case 3:
		error = true;
    err_message_ = "[!] The selected file is already a hypercube";
    break;
	default:
		error = true;
    err_message_ = "[!] Unrecognized data format";
		break;

	}
  return error;
}

auto FileConverter::LegacyFormatToHypercube() -> void
{

}

auto FileConverter::MultiDetectorFormatToHypercube() -> void
{

}


auto Tokenize(std::string const & str, std::string const & delims)
  -> std::vector<std::string>
{
  std::vector<std::string> cont;
  std::size_t current, previous = 0;
  current = str.find(delims);
  while (current != std::string::npos) {
      cont.push_back(str.substr(previous, current - previous));
      previous = current + delims.size();
      current = str.find(delims, previous);
  }
  cont.push_back(str.substr(previous, current - previous));

  return cont;
};

auto FileConverter::OptimizedSingleDetectorFormatToHypercube() -> void
{
  pugi::xml_document doc{};
  auto root = doc.append_child("Analysis_Header");
  auto node = root.append_child("Format");
  node.append_child(pugi::node_pcdata).set_value("Hypercube");

  // We extract the header
  // Put entries into xml
  std::vector<std::string> tokens;
  std::string buffer;
  buffer.reserve(128);

  getline(data_file_, buffer);
  while (buffer.front() != '}')
  {
    tokens = Tokenize(buffer, " = ");
    node = root.append_child(tokens.at(0).c_str());
    node.append_child(pugi::node_pcdata).set_value(tokens.at(1).c_str());
    getline(data_file_, buffer);
  }

  //Save the file onto a temporary
  filename_.append(".hyperc");

  //TODO check for errors in saving

  std::ofstream output{filename_};
  if (!output.is_open()) {
    err_message_ = "[!] Can't open output file";
    error = true;
    return;
  }
  //TODO check for errors in opening


  output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
  output << "<XRFAnalysis>" << std::endl;

  // Paste the xml header in the output file
  doc.print(output);

  output << "<Analysis_Data>";
  output << std::hex << std::uppercase;

  end_of_header_pos_ = output.tellp();


  int counts, bin;
  int expected_bin{0};
  getline(data_file_, buffer);
  while (!data_file_.eof())
  {
    if (buffer.front() == 'P')
    {
      // Write pixel front
     output << std::endl;
     tokens = Tokenize(buffer, "\t");

     output << std::stoi(tokens.at(2)) << ',';
     output << std::stoi(tokens.at(3));

     expected_bin = 0;
    }
    else {
      output << ',';

      bin = std::stoi(buffer);
      counts = bin & 0x0003FFF;
      bin    = (bin & 0xFFFC000) >> 14;

      if (bin != expected_bin)
      {
        output << bin - 1 << 'R' << ',';
      }
      output << counts;
      expected_bin = bin + 1;
    }

    getline(data_file_, buffer);
  }
  output << std::endl;
  output << "</Analysis_Data>" << std::endl;
  output << "</XRFAnalysis>" << std::endl;

  // Output file  is closed upon exiting this function
  // File handled by member data_file_ is closed upon destruction of this instance
}

