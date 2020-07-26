#ifndef HEADER_TYPES_H
#define HEADER_TYPES_H

#include <iostream>
#include <string>
#include <vector>

#include "pugixml.hpp"
#include "external/utility.hpp"

enum AvailableHeaderFormats
{

};

enum FieldFormatFlags
{
  SEPARATOR         = 0x01,
  VALUE_GUARDS      = 0x02,
  DESCRIPTOR_GUARDS = 0x04,
  FIELD_GUARDS      = 0x08,
  DESCRIPTOR_FIRST  = 0x10
};

struct FieldFormatPolicy
{
  // Field Format ->
    // Field Begin Guard
    // Field End Guard
    // Descriptor Begin Guard
    // Descriptor End Guard
    // Value Begin Guard
    // Value End Guard
    // Descriptor Value Separator
    // bool Uses Separator?
    // bool Uses Value Guards?
    // bool Uses Descriptor Guards ?
    // bool Uses Field Guards ?
    // bool Descriptor Before Value ?
    // bool Prepend Category Descriptor ?
    // bool Descriptor Can Be Empty?
};

/////////////////////////////////////////////////
/// ... text ...
/////////////////////////////////////////////////

//enum class EntryFlags : int
//{
//  IS_REQUIRED     = 0x01,
//  PREFER_DEFAULT  = 0x02
//};
//DECLARE_BITWISE_OPERATORS(EntryFlags)


/**
 * @brief The HeaderField struct
 */
struct HeaderEntry
{
  using string = std::string;
  ///
  /// \brief HeaderEntry
  /// \param path
  /// \param val
  /// \param units
  /// \param flag
  ///
  HeaderEntry(string path, string val = "", string units = "") :
    path{path}, value{val}, units{units}
  {

  }
  string const path;
  string value;
  string units;

  std::vector<std::string> categories;
  std::string descriptor;

};

struct HeaderSection
{
  using FieldList = std::vector<HeaderEntry>;


  FieldList fields;
};

/**
 * @brief The Header class
 */
class Header
{
  //using FieldList = std::vector<HeaderField>;

public:
  // The Interface
  Header();
  virtual ~Header();

  virtual auto MakeDefault() -> void = 0;
  // auto MakeNewField() -> void;
  // auto ConstructHeaderString() -> void;

  // auto ConvertBetweenFormats() -> void;
  // auto CalculateFieldsFromScanParameters -> void;

private:
  // ---------- PROPERTIES ---------- //
  // Preamble
  // Header Format ->
    // Header Begin Guard
    // Header End Guard
    // Size Constraints
  // Sections Format ->
    // Section Begin Guard
    // Section End Guard

  //FieldFormatPolicy fields_format_;
  //std::vector <HeaderField> fields_;
};


class XMLHeader : public Header
{
  using FieldList = std::vector<HeaderEntry>;
public:
  XMLHeader() {}
  ~XMLHeader() {}

  auto MakeDefault() -> void;

private:
  FieldList fields_;
};

class EDFHeader : public Header
{

};

class LegacyHeader : public Header
{

};





#endif // HEADER_TYPES_H
