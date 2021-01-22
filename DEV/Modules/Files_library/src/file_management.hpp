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

#ifndef MAXRF_FILE_MANAGEMENT_H
#define MAXRF_FILE_MANAGEMENT_H

#include <cstring>
#include <iostream>
#include <fstream>

#include <list>
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
  kStartXCoordinate,
  kStartYCoordinate,
  kEndXCoordinate,
  kEndYCoordinate,
  kMotorStepX,
  kMotorStepY,
  kMotorVelocity,
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

struct PixelData
{
  uint32_t coord_x;
  uint32_t coord_y;
  std::vector<uint16_t> histogram;
};

///
/// \brief The FileWriter class takes spectral data from consecutive pixels of
/// an XRF scan line and outputs it to a file in the correct order and with the
/// correct format
///
/// The ordering of the pixels on an MAXRF scan depends on the number of the
/// scan line. Pixels on even lines increase to the right, while pixels on
/// odd lines increase to the left.
///
/// The FileWriter class uses two buffers to accomplish is task: a pixel buffer,
/// and a scan line buffer. The scan line buffer is merely a list of pixel
/// buffers arranged in the correct order. The pixel with column coordinate 0
/// will be at the index 0 of the scan line buffer, and so on.
///
/// The pixel buffer has the following format:
/// Begins with 4 bytes for the pixel x coord, then 4 bytes for the y coord
/// Then all data following can have a layout of either:
///  - 2 bytes (not equal to 0xFFFFU) for the event counts
///  - 2 bytes command flag (0xFFFFU) followed by 2 bytes coding the number
///    of consecutive zeroes in the histogram
/// Finally a 4 byte flag marks the end of pixel (0xFFFF FFFFU)
///
/// The FileWriter class takes the pixel spectral data and codes it in the pixel
/// buffer with the format described above. Then, the FileWriter class writes
/// all the pixel data to the file only when all the pixel buffers in a given
/// line have been written.
///
/// Data can be written to the file in binary or ASCII mode. In case of the
/// former, the data will have the format described above. In case of the latter
/// the format will be the following;
///
/// - All numbers are written in their ASCII uppercase hexadecimal representation
/// - pixel column coordinate followed by a comma
/// - pixel row coordinate followed by a comma, or in case there is no spectral
///   data in the pixel, followed by a std::endl
/// - bin event counts (which can be zero)
/// - should there be more than two consecutive zeroes in the data histogram,
/// they are written as:
///   - number of consecutive zeroes, followed by an 'R', followed by a comma
/// - the end of the pixel is marked by replacing the trailing comma with a
///   std::endl
///
class MAXRF_LIBRARIES_SHARED_EXPORT FileWriter
{

public:
  using Histogram   = std::vector<uint16_t>;
  using PixelBuffer = std::vector<uchar>;

  ///
  /// \brief FileWriter constructor takes ownership of a file descriptor
  /// (managed by a file stream class instance) and setups the scan line
  /// buffer to the specified with
  /// \param f
  /// \param width
  ///
  FileWriter(std::fstream && f, uint32_t width) {
    file_ = std::move(f);
    scan_line_width_ = width;

    SetUpBuffers(width);
  }


  //  void AddOutputFile(std::shared_ptr<HypercubeFile> file) {
  //    file_ = file;
  //  }
  void SetUpBuffers(uint scan_line_width);

  void WriteDataToFiles(PixelData & data);

  auto ReacquireFileOwnership() {
    return std::move(file_);
  }

private:
  //  std::shared_ptr<HypercubeFile> file_ {nullptr};
  std::fstream file_ ;
  std::vector<PixelBuffer> buffer {};
  uint32_t scan_line_width_;

  struct FileStats {
    uint32_t pixels_in_buffer;  ///< No. of pixels in buffer but not written
    uint32_t pixels_written;    ///< No. of pixels written
    Histogram  sum_spectrum;    ///< Detector sum spectrum of scan
  } stats {};
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

  std::unique_ptr<FileWriter> writer {nullptr};
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

  HypercubeFile(std::fstream && f);

  /// DIRECT ACCESS
  auto DataFile() -> std::fstream & {
    return file_;
  }

  /// FILE WRITING API

  ///
  /// \brief SetWriteMode sets the write mode to either Scan DAQ, or Point DAQ
  ///
  inline void SetWriteMode(WriteMode mode, uint width) {
    write_mode = mode;
    switch (write_mode) {
    case WriteMode::kDAQInvalid :
      [[fallthrough]];
    case WriteMode::kDAQPoint :
      if (writer) {
        file_ = writer->ReacquireFileOwnership();
        writer.reset();
      }
      break;
    case WriteMode::kDAQScan :
      writer = std::make_unique<FileWriter>(std::move(file_), width);
      break;
    }
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
  bool WritePixel(PixelData & data);

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

  // Not reentrant! Not suitable for concurrent use
  inline auto GoToPixel(size_t x, size_t y) -> void {
    static auto kInvalidPos {std::fstream::pos_type {std::fstream::off_type {-1}}};
    static std::fstream::pos_type pos {};

    pos = lut->at(x, y).datum_pos;
    if (pos == kInvalidPos) {
      throw std::runtime_error("Hypercubefile reports: Missing pixel data!");
    }
    file_.seekg(pos);
  }
  inline auto GoToPixel(size_t index) -> void {
    static auto kInvalidPos {std::fstream::pos_type {std::fstream::off_type {-1}}};
    static std::fstream::pos_type pos {};

    pos = lut->at(index).datum_pos;
    if (pos == kInvalidPos) {
      throw std::runtime_error("Hypercubefile reports: Missing pixel data!");
    }
    file_.seekg(pos);
  }
  inline auto GoToPixel(std::fstream::pos_type pos) -> void {
    static auto kInvalidPos {std::fstream::pos_type {std::fstream::off_type {-1}}};

    if (pos == kInvalidPos) {
      throw std::runtime_error("Hypercubefile reports: Missing pixel data!");
    }
    file_.seekg(pos);
  }

  template <class T>
  auto ParsePixel(T & parse_instructions) -> void
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
        bin += value; // We skip the number of consecutive zeroes
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

  std::fstream::pos_type first_datum_pos_;

  pugi::xml_document header_node_;
  std::shared_ptr<LookupTable> lut;
};

class MAXRF_LIBRARIES_SHARED_EXPORT MAXRFConfigFile
{

};




}

#endif
