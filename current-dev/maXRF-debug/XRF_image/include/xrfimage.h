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

  void ComputeROISpectrum(std::vector<int>&& pixels_selected);
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

  void set_shared_memory5(double * ptr)
  {
    shared_memory5 = ptr;
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
  double * shared_memory5;

  std::string err_msg_;
  std::fstream  file_;


  ImageData image_data_;
  Spectrum  sum_spectrum_;
  Spectrum  roi_spectrum_;
  // Info about the data file associated

};

#endif // XRFIMAGE_H
