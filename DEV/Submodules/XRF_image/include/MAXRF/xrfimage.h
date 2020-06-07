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

#include <sys/stat.h> // For file information

#include "MAXRF/types.h"


class XRFImage : public QObject
{
  Q_OBJECT
  using Buffer  = std::vector<std::unique_ptr<DataFile>>;

public:
  explicit XRFImage(QWidget * parent = nullptr);
  ~XRFImage();

  auto IsPixelOnImage(size_t x, size_t y) -> bool;
  auto FindPixelWithCoordinates(size_t x, size_t y) -> size_t;

  void ComputeROISpectrum(std::vector<int>&& pixels_selected);
  void LoadDataFile(std::string);

  auto RenderQImage() -> std::unique_ptr<UnprocessedImage>;
  void UpdatePixelIntegrals();



  // ACCESOR METHODS
  auto err_mesg() -> std::string;
  auto is_valid() -> bool;
  auto sum_spectrum() -> Histogram &;
  auto roi_spectrum() -> Histogram &;

signals:
  void UpdateProgressBar(int value);

private:
  double roi_low;
  double roi_high;

  Buffer buffer_;
  Buffer::iterator buffer_it_;

  Histogram  roi_spectrum_;
};

#endif // XRFIMAGE_H
