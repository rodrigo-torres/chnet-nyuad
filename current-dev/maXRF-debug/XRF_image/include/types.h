#ifndef TYPES_H
#define TYPES_H

#include <fstream>
#include <memory>
#include <string>

#include <QImage>
#include <QString>

// -------------------- USER CONFIGURATION -------------------- //
// These fields will eventually need to be populated from a configuration file
static constexpr int const lowest_bin = 0;
static constexpr int const highest_bin = 16383;
static constexpr int const total_bins = 16384;

struct Pixel;

using PixelData   = std::unique_ptr<Pixel>;
using ImageData   = std::vector<PixelData>;
using Histogram   = std::array<int32_t, total_bins>;

struct Pixel
{
  std::streamoff first_datum_pos;
  int pixel_no;
  int x_coord;
  int y_coord;
  int integral;
  std::array<int, 3> roi_integrals;
};

struct UnprocessedImage
{
  UnprocessedImage() : matrix{}, brightness{0}, contrast{false},
    pixel_replication{1}, palette{}, intensity_minimum{0}, intensity_maximum{1}
  {}
  // An 8-bit grayscale representation of the rendered matrix by XRFImage
  QImage matrix;
  QImage processed;
  // Attributes necessary to determine how the image will be painted
  int brightness;
  bool contrast;
  int pixel_replication;
  QString palette;
  // Convenience variables to ease computing time
  int intensity_minimum;
  int intensity_maximum;
  std::array<int32_t, 256> histogram;
};

struct DataFile
{
  std::fstream file;
  std::string err_msg;
  bool valid;

  ImageData image_data;
  Histogram sum_spectrum;

  uint x_length_;
  uint y_length_;
  uint pixels_in_image_;

  int minimum_integral_;
  int maximum_integral_;
};

#endif // TYPES_H
