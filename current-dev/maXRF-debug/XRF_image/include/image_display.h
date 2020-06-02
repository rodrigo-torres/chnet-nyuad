#ifndef IMAGE_DISPLAY_H
#define IMAGE_DISPLAY_H

#include <memory>

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

// -------------------- USER CONFIGURATION -------------------- //
// These fields will eventually need to be populated from a configuration file
static QString data_directory = "/home/frao/Documents/Workspaces/MAXRF/Data";


class ImgLabel : public QLabel
{
  // INHERITED PUBLIC METHODS
  // Mutator function for the label's QPixmap
  // void setPixmap(const QPixmap &)
  Q_OBJECT

  using ImagePointer	= std::unique_ptr<XRFImage>;
  using ImageBuffer		= std::vector<ImagePointer>;
  using ImageIterator	= ImageBuffer::iterator;

  using ColorTable = QVector<QRgb>;
  using Palettes = std::map<QString, ColorTable>;


signals:
  void UpdateProgressBar(int value);

public slots:
  void RelayProgressBarSignal(int value)
  {
    emit UpdateProgressBar(value);
  }

public:
  ImgLabel();
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
  void RepaintImage();


  // ACCESOR methods
  bool is_map_opened() const;
	QImage qimage() const;

  // MUTATOR methods
  void set_pixel_dim(int);
  void set_map_opened(bool state);
  void set_current_palette(QString palette);

private:
  void CreatePalettes();

private:
  shm::array<int> shared_memory_cmd;
  shm::array<int> shared_memory;

  int pixel_dim_;
  bool left_mouse_clicked_;
  bool coordinates_found_;
  bool map_opened_;

  int x_image_;
  int y_image_;

  QImage qimage_;

  Palettes palettes_;

  QString selected_palette;
  XRFImage * img_data;

  ImageBuffer images_;
  ImageIterator image_on_display_;



  void mousePressEvent(QMouseEvent * event);
  void mouseReleaseEvent(QMouseEvent * event);
  void wheelEvent(QWheelEvent * event);
};

#endif // IMAGE_DISPLAY_H

