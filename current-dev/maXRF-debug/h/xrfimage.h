#ifndef XRFIMAGE_H
#define XRFIMAGE_H

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <QProgressBar>

#include "viridis.h"

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

  long ComputeXLength();
  long ComputeYLength();
  int ComputeMaxIntegral();
  void UpdateROIIntegrals();

  QImage ConstructQImage(QString mode, int Pixeldim);
private:
  bool valid_;
  std::string err_msg_;
  std::fstream  file_;

  double roi_low;
  double roi_high;

  ImageData image_data_;
  Spectrum  sum_spectrum_;
  Spectrum  roi_spectrum_;
  // Info about the data file associated
};

extern XRFImage xrf_image;



#endif // XRFIMAGE_H
