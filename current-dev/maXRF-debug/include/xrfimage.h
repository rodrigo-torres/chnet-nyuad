#ifndef XRFIMAGE_H
#define XRFIMAGE_H

#include <algorithm>  // For maximum element algorithm
#include <fstream>
#include <memory>     // Smart pointers
#include <string>
#include <iostream>
#include <vector>

#include <QInputDialog>
#include <QProgressBar>
#include <QString>

#include "viridis.h"

extern std::vector<int> integral;
extern int x_len, y_len, x_step, y_step, min_x, min_y;


// Conversion methods

// Detector alignment methods
extern double *shared_memory5;

struct Pixel;
struct Pixel
{
  std::streamoff first_datum_pos;
  int pixel_no;
  int x_coord;
  int y_coord;
  int integral;
  std::array<int, 3> roi_integrals;
  //void update_integral(int val){ integral = val; }
};
typedef std::vector<int_least32_t> Spectrum;
typedef std::vector<std::unique_ptr<Pixel>> ImageData;

// TODO add ROI limits info

// API
class XRFImage
{
public:
  XRFImage();
  ~XRFImage();
  void LoadDataFile(std::string);
  bool is_valid();
  std::string err_msg();

  int ComputeMaxIntegral();
  void UpdateROIIntegrals();
  bool FindPixelWithCoordinates(uint, uint, uint*);

  void ComputeROISpectrum(std::vector<int>&& pixels_selected)
  {
    roi_spectrum_.resize(16384);
    std::fill(roi_spectrum_.begin(), roi_spectrum_.end(), 0);
    for (auto val : pixels_selected)
    {
      std::string line;
      auto & p = image_data_.at(val);
      int i_line = 0, channel = 0, count = 0;

      file_.seekg(p->first_datum_pos);
      getline(file_, line);
      while (line.front() != 'P' && !file_.eof())
      {
        i_line = stoi(line);
        channel = (i_line & 0xFFFC000) >> 14;
        count = (i_line & 0x3FFF);
        roi_spectrum_.at(channel) += count;
        getline(file_, line);
      }
    }
  }

  QImage ConstructQImage(QString mode, int Pixeldim);

  Spectrum roi_spectrum()
  {
    return roi_spectrum_;
  }
  Spectrum sum_spectrum();

  uint x_length()
  {
    return x_length_;
  }
  uint y_length()
  {
    return y_length_;
  }
private:
  void ComputeXLength();
  void ComputeYLength();

  bool valid_;

  uint x_length_;
  uint y_length_;
  uint pixels_in_image_;

  double roi_low;
  double roi_high;

  std::string err_msg_;
  std::fstream  file_;


  ImageData image_data_;
  Spectrum  sum_spectrum_;
  Spectrum  roi_spectrum_;
  // Info about the data file associated

};

extern XRFImage xrf_image;



#endif // XRFIMAGE_H
