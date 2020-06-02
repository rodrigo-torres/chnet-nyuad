#ifndef XRFIMAGE_H
#define XRFIMAGE_H

#include <algorithm>  // For maximum element algorithm
#include <cmath>      // ceil functions
#include <fstream>
#include <memory>     // Smart pointers
#include <string>
#include <iostream>
#include <vector>

#include <QInputDialog>
#include <QProgressBar>
#include <QObject>
#include <QString>

#include "include/shm_wrapper.h"

// -------------------- USER CONFIGURATION -------------------- //
// These fields will eventually need to be populated from a configuration file
static int const lowest_bin = 0;
static int const highest_bin = 16383;


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
class ImgLabel;

// API
class XRFImage : public QObject
{
  Q_OBJECT
  friend ImgLabel;

signals:
  void UpdateProgressBar(int value);

public:
  explicit XRFImage(QWidget* parent = 0) : valid_{false}, x_length_{0},
  y_length_{0}, pixels_in_image_{0}, roi_low{highest_bin}, roi_high{lowest_bin}
  {
	  shm::TypeDefInitSHM shm5_init;
	  shm5_init.key = 8000;
	  shm5_init.size = 4096;
	  shm5_init.shmflag = shm::SHMFlags::CREATE | shm::SHMPermissions::ALL_ALL;

	  shared_memory5.initialize(shm5_init);
  }
  virtual ~XRFImage()
  {
    if (file_.is_open())
    {
      file_.close();
    }
  }


  void LoadDataFile(std::string);
  bool is_valid();
  std::string err_msg();

  int ComputeMaxIntegral();
  void UpdateROIIntegrals();
  bool FindPixelWithCoordinates(uint, uint, uint*);

  void ComputeROISpectrum(std::vector<int>&& pixels_selected);
  QImage RenderQImage();

  Spectrum roi_spectrum()
  {
    return roi_spectrum_;
  }
  Spectrum sum_spectrum();

  int x_length()
  {
    return x_length_;
  }
  int y_length()
  {
    return y_length_;
  }
protected:
  XRFImage * address()
  {
    return this;
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
  shm::array<double> shared_memory5;

  std::string err_msg_;
  std::fstream  file_;


  ImageData image_data_;
  Spectrum  sum_spectrum_;
  Spectrum  roi_spectrum_;
  // Info about the data file associated

};

#endif // XRFIMAGE_H
