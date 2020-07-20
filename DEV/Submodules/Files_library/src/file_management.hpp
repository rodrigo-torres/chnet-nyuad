/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
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
#ifndef MAXRF_FILE_MANAGEMENT_H
#define MAXRF_FILE_MANAGEMENT_H

#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>

#include "maxrf_libraries_config.h"
#include "pugi/pugixml.hpp"
 
 namespace maxrf {

 class MAXRFDataFile;

 enum class MAXRF_LIBRARIES_SHARED_EXPORT DataFormat : int
 {
   kInvalid     =  0,
   kLegacy,
   kEDF,
   kHDF5,
   kMultiDetectorMaskedDump,
   kXMLFile,
   kHypercube,
   kHypercubeFiltered,
 };

 enum class MAXRF_LIBRARIES_SHARED_EXPORT HeaderTokens : int
 {
   kDAQStartTimestamp,
   kImageWidth,
   kImageHeight,
   kImagePixels
 };

 enum class MAXRF_LIBRARIES_SHARED_EXPORT WriteMode : int {
   kDAQInvalid  = 0x0,
   kDAQPoint    = 0x1,
   kDAQScan     = 0x2
};

 // 24 bytes per pixel -> 24 MB for a 1 mega-pixel image
 struct MAXRF_LIBRARIES_SHARED_EXPORT LUTEntry
 {
   LUTEntry() :
     datum_pos{std::numeric_limits<std::fstream::pos_type>::max()},
     integrals {0, 0, 0, 0} {}
   std::streampos datum_pos;
   std::array<int, 4> integrals;
 };

 ///
 /// \brief An interface to navigate a 2D matrix laid out as a 1D vector
 ///
 /// The function provides safer data access to a 2D coordinate on a matrix whose
 /// underlying implementation is a 1D vector, while reducing the overhead of
 /// a std::vector of std::vector
 ///
 class MAXRF_LIBRARIES_SHARED_EXPORT LookupTable
 {
 public:
   using value_type  = LUTEntry;
   using vector_type = std::vector<value_type>;
   using size_type   = vector_type::size_type;

   using value_reference         = value_type &;
   using vector_reference        = vector_type &;
   using value_const_reference   = value_type const &;
   using vector_const_reference  = vector_type const &;

   LookupTable() {}
   LookupTable(vector_type && __vec, size_type __x, size_type __y)
   {
     v_ = std::move(__vec);
     x_len_ = __x;
     y_len_ = __y;
     _init_size_check(x_len_ * y_len_);
   }

   // We provide a simple API consisting of 3 methods:
   // The first, a means to initialize the instance with an existing vector
   auto initialize(vector_type && __vec, size_type __x, size_type __y) -> void
   {
     v_ = std::move(__vec);
     x_len_ = __x;
     y_len_ = __y;
     _init_size_check(x_len_ * y_len_);
   }

   inline auto at(size_type __x, size_type __y) -> value_type  {
     return v_.at(__y * x_len_ + __x);
   }
   inline auto at(size_type __x, size_type __y) const -> value_type  {
     return v_.at(__y * x_len_ + __x);
   }
   // The second, a method to access an index in the vector through a 2D coordinate
   inline auto at(size_type __index) -> value_type  {
     return v_.at(__index);
   }
   inline auto at(size_type __index) const -> value_type  {
     return v_.at(__index);
   }

   // The third to get access to the underlying vector
   inline auto vector() noexcept -> vector_reference  {
     return v_;
   }
   inline auto vector() const noexcept -> vector_const_reference  {
     return v_;
   }
 protected:
   auto _init_size_check(size_type __elems) const -> void
   {
     if (__elems != v_.size())
     {
       throw std::range_error("PixelsNavigator::_init_size_check: __elems");
     }
   }

 private:
   vector_type v_;
   size_type x_len_;
   size_type y_len_;
 };

 class MAXRF_LIBRARIES_SHARED_EXPORT DataFileHander
 {

 public:
   // No copy / move constructors nor assignment operators
   DataFileHander(DataFileHander const &) = delete;
   DataFileHander(DataFileHander &&) = delete;
   DataFileHander & operator= (DataFileHander const &) = delete;
   DataFileHander & operator= (DataFileHander &&) = delete;

   /// FORMAT IDENTIFICATION
   static auto GetFile(std::string filepath) -> std::shared_ptr<MAXRFDataFile>;
   static auto GetFileForDAQ(std::string filepath) -> std::shared_ptr<MAXRFDataFile>;
   /// MULTIDETECTOR SUMMATION METHODS
 private:
   DataFileHander();

   static std::map<std::string, DataFormat> formats_;
 };

class MAXRF_LIBRARIES_SHARED_EXPORT MAXRFDataFile
{
  friend DataFileHander;

protected:
  MAXRFDataFile() = default;

  void SetFormat(DataFormat const _format) {
    format_ = _format;
  }

  void SetFileDevice(std::fstream && _file) {
    file_ = std::move(_file);
  }

  DataFormat format_ { DataFormat::kInvalid };
  std::fstream file_ {};

public:
  virtual ~MAXRFDataFile();

  ///  CHECK OPERATIONS
  auto GetFormat() const noexcept ->  DataFormat;

  /// HEADER MANIPULATION API
  virtual auto ExtractHeader() -> std::string;

  /// FILE CONVERSION API
  virtual auto ConvertToHypercube() -> bool;
  virtual auto ExtractHistograms() -> bool;
  /// INTEGRITY CHECKS
  virtual auto CheckIntegrity() -> std::string;
};

class MAXRF_LIBRARIES_SHARED_EXPORT HypercubeFile : public MAXRFDataFile
{
  friend DataFileHander;

public:
  using PixelData   = std::vector<uint32_t>;

  HypercubeFile(std::fstream && f);

  /// DIRECT ACCESS
  auto DataFile() -> std::fstream & {
    return file_;
  }

  /// FILE WRITING API

  ///
  /// \brief SetWriteMode sets the write mode to either Scan DAQ, or Point DAQ
  ///
  inline void SetWriteMode(WriteMode mode) {
    write_mode = mode;
  }
  ///
  /// \brief MakeDefaultHeader looks at the configuration file and tries to fill
  /// in as much header fields as possible with their default values. No data
  /// is actually written onto the file
  /// \return
  ///
  bool MakeDefaultHeader();
  ///
  /// \brief WriteHeader confirms the user edits of the header fields and
  /// actually writes the header onto the file
  /// \return a boolean value, true if succesful, false otherwise
  /// This function is meant to be called after the user has use the Header API,
  /// such as \see EditToken, to confirm the changes and write the header
  ///
  bool WriteHeader();
  ///
  /// \brief WritePixel write a single spectrum in the specified format
  /// \param data
  /// \return
  ///
  bool WritePixel(PixelData const & data);
  ///
  /// \brief WritePixel overload specific for writing XRF scan data
  /// \param data
  /// \param x
  /// \param y
  /// \return
  ///
  bool WritePixel(PixelData const & data, size_t x, size_t y);
  ///
  /// \brief MakeDefaultFooter
  /// \return
  ///
  bool MakeDefaultFooter() { return false; }
  ///
  /// \brief WriteFooter
  /// \return
  ///
  bool WriteFooter();


  /// HEADER MANIPULATION API
  auto ExtractHeader() -> std::string override;
  auto GetTokenValue(HeaderTokens token) -> std::string;
  auto GetTokenValue(std::string search_term) -> std::string;
  auto EditToken(HeaderTokens token, std::string val) -> bool;
  auto EditToken(std::string search_term, std::string val) -> bool;

  /// FILE CONVERSION API
  auto ConvertToPyMcaEDF() -> bool;
  auto ConvertToHDF5() -> bool;
  auto ExtractHistograms() -> bool override;

  /// LOOK-UP TABLE COMPUTATION
  auto ComputeLookupTable() -> std::shared_ptr<LookupTable>;

  inline auto GoToPixel(size_t x, size_t y) -> void {
    file_.seekg(lut->at(x, y).datum_pos);
  }
  inline auto GoToPixel(size_t index) -> void {
    file_.seekg(lut->at(index).datum_pos);
  }
  inline auto GoToPixel(std::fstream::pos_type pos) -> void {
    file_.seekg(pos);
  }

  template <class T>
  inline auto ParsePixel(T & parse_instructions) -> void
  {
    static bool end_of_line{false};
    static char character;         // We read character by character... sometimes
    static int  bin{0}, value{0};  // Helper variables to assing integrals and spectra

    bin = 0;
    end_of_line = false;

    // We skip the coordinates at the beginning of the pixel
    file_ >> value >> character;
    file_ >> value >> character;
    if (character == '\n') {
      // There's no spectral data in this pixel
      return;
    }

    while (!end_of_line)
    {
      file_ >> value >> character;
      switch (character)
      {
      case '\n':
        end_of_line = true;
        [[fallthrough]];
      case ',':
        end_of_line |= parse_instructions(bin, value);
        ++bin;
        break;
      case 'R':
        bin = value;
        // Extract the trailing comma
        file_ >> character;
        break;
      default:
        std::cout << "Formatting error at position: "
                  << file_.tellg() << std::endl;
        break;
      }
    }
  }


  ///
  /// \brief CheckIntegrity parse the file looking for format errors
  /// \return
  ///
  auto CheckIntegrity() -> std::string override;
private:
  WriteMode write_mode  { WriteMode::kDAQInvalid };

//  std::fstream file_;
  std::fstream::pos_type first_datum_pos_;

  pugi::xml_document header_node_;
  std::shared_ptr<LookupTable> lut;
};

class MAXRF_LIBRARIES_SHARED_EXPORT MAXRFConfigFile
{

};




}

#endif
