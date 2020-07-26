#ifndef IMAGE_DISPLAY_H
#define IMAGE_DISPLAY_H

#include <memory>
#include <tuple>

#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QString>
#include <QTextEdit>
#include <QWheelEvent>

#include "MAXRF/xrfimage.h"
#include "MAXRF/shared_memory.h"
#include "qcustomplot.h"

extern template class shmarray<int>;

namespace maxrf {


// -------------------- USER CONFIGURATION -------------------- //
// These fields will eventually need to be populated from a configuration file
static QString data_directory = "/home/frao/Documents/Workspaces/MAXRF/Data";

/// \class ImgLabel
/// \brief The ImgLabel class
///
/// \var displayed_image_ holds 32 bit color reprensentation of the image
///
class ImgLabel : public QLabel
{
  // INHERITED PUBLIC METHODS
  // Mutator function for the label's QPixmap
  // void setPixmap(const QPixmap &)
  Q_OBJECT
  using ColorTable = QVector<QRgb>;
  using Palettes = std::map<QString, ColorTable>;


signals:
  void AddToComboBox(QString name);
  void EnableImageControls();
  void UpdateProgressBar(int value);
  void UpdateImageHistogram(QVector<uint> histogram);

public slots:
  void RelayProgressBarSignal(int value)
  {
    emit UpdateProgressBar(value);
  }

public:
  ImgLabel(QWidget * parent);
  ~ImgLabel();
  /**
    *	A method that loads an XRFImage and adds it to the behind-the-curtains
    *	buffer. In practice it creates a new XRFImage in the heap, assigns the
    *	ownership to a smart pointer, itself stored in a vector. This method
    *	allows fast cycling between images loaded in memory.
    */
  void AddImageToBuffer();
  void RemoveImageFromBuffer();
  void ChangeImage(int index);

//  void SelectFilter(EnergyFilter filter, int low, int high) {
//    renderer_.SetIntegralLimits(filter, low, high);
//  }

//  void RenderImage(EnergyFilter filter = EnergyFilter::kUnfiltered);
  void PaintImage();
  void ProcessImage();


  // ACCESOR methods
  bool is_map_opened() const;
  QImage qimage() const;

  // MUTATOR methodsprocessed
//  void set_pixel_dim(int);
  void ToggleEqualization(bool state);
  void ToggleStretching(bool state);
  void set_brightness(int percentage);

private:
//  void CreatePalettes();
  void AdjustContrast();
//  void AdjustBrightness();
//  void EqualizeHistogram();
//  void ResizeImage();

  void mousePressEvent(QMouseEvent * event);
  void mouseReleaseEvent(QMouseEvent * event);
//  void wheelEvent(QWheelEvent * event);

private:
  shmarray<int> shared_memory_cmd;
  shmarray<int> shared_memory;

  bool left_mouse_clicked_;
  bool coordinates_found_;
  bool map_opened_;

  int x_image_;
  int y_image_;

  // Look-up tables for the painted XRF images, which are stored as 8-bit
  //  indexed images
  Palettes palettes_;
  QString selected_palette;

  Renderer renderer_;
  std::vector<std::unique_ptr<IndexedImage>> images_;
  int image_index_;


  QImage displayed_image_;
  QVector<uint> displayed_histo_;

  QWidget * parent_;
};

}

#endif // IMAGE_DISPLAY_H

