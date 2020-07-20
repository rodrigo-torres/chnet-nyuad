#ifndef TYPES_H
#define TYPES_H

#include <fstream>
#include <memory>
#include <string>

#include <QImage>
#include <QString>

#include "utility.hpp"

// -------------------- USER CONFIGURATION -------------------- //
// These fields will eventually need to be populated from a configuration file
static constexpr int const lowest_bin = 0;
static constexpr int const highest_bin = 16383;
static constexpr int const total_bins = 16384;

using Histogram   = std::array<int32_t, total_bins>;

enum ProcessingFlags : char
{
  kNone       = 0,
  kBrighten   = 0x1,
  kEqualize   = 0x2,
  kStretch    = 0x4,
  kResize     = 0x8,
  kPalette    = 0x10,
};
DECLARE_BITWISE_OPERATORS(ProcessingFlags);

/// \struct
/// \brief The IndexedImage struct
///
/// \var matrix
/// \var dimensions
/// \var flags
/// \var brightness
/// \var pixel_replication
/// \var histogram
///
struct IndexedImage
{
  IndexedImage() : flags{kNone}, brightness{0},
    pixel_replication{1}
  {}
  // An 8-bit grayscale representation of the rendered matrix by XRFImage
//  QImage matrix;
  std::vector<uint32_t> indexed_data;
  QSize dimensions;
  // Attributes necessary to determine how the image will be painted
  ProcessingFlags flags;
  int brightness;
  int pixel_replication;
  std::array<int32_t, 256> histogram;
};


#endif // TYPES_H
