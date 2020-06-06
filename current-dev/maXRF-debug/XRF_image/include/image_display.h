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

#include "include/xrfimage.h"
#include "include/shm_wrapper.h"
#include "libs/qcustomplot.h"

// -------------------- USER CONFIGURATION -------------------- //
// These fields will eventually need to be populated from a configuration file
static QString data_directory = "/home/frao/Documents/Workspaces/MAXRF/Data";


class ImgLabel : public QLabel
{
  // INHERITED PUBLIC METHODS
  // Mutator function for the label's QPixmap
  // void setPixmap(const QPixmap &)
  Q_OBJECT

  using Renderer	= std::unique_ptr<XRFImage>;

  using ColorTable = QVector<QRgb>;
  using Palettes = std::map<QString, ColorTable>;


signals:
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


  void RenderAndPaintImage();
  void ProcessImage();


  // ACCESOR methods
  bool is_map_opened() const;
	QImage qimage() const;

  // MUTATOR methods
  void set_pixel_dim(int);
  void ToggleHistogramStretching(int percentage);
  void set_brightness(int percentage);

private:
  void CreatePalettes();
  void AdjustContrast();
  void AdjustBrightness();
  void PaintImage();
  void ResizeImage();

  void mousePressEvent(QMouseEvent * event);
  void mouseReleaseEvent(QMouseEvent * event);
  void wheelEvent(QWheelEvent * event);

private:
  shm::array<int> shared_memory_cmd;
  shm::array<int> shared_memory;

  bool left_mouse_clicked_;
  bool coordinates_found_;
  bool map_opened_;

  int x_image_;
  int y_image_;

  //QImage qimage_;

  // Look-up tables for the painted XRF images, which are stored as 8-bit
  //  indexed images
  Palettes palettes_;
  QString selected_palette;

  XRFImage renderer;
  std::vector<std::unique_ptr<UnprocessedImage>> images_;
  std::vector<std::unique_ptr<UnprocessedImage>>::iterator active_image_;

  QImage displayed_image_;

  QWidget * parent_;
};

#endif // IMAGE_DISPLAY_H

