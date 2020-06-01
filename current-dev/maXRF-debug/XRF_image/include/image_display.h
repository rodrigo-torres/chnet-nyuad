#ifndef IMAGE_DISPLAY_H
#define IMAGE_DISPLAY_H

#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>
#include <QWheelEvent>
#include <QTextEdit>

#include "include/xrfimage.h"
#include "include/shm_wrapper.h"

class ImgLabel : public QLabel
{
  // INHERITED PUBLIC METHODS
  // Mutator function for the label's QPixmap
  // void setPixmap(const QPixmap &)
  Q_OBJECT
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
  void DisplayImage();

  // ACCESOR methods
  bool is_map_opened() const;
  QImage qimage() const;

  // MUTATOR methods
  void set_pixel_dim(int);
  void set_map_opened(bool state);
  void set_image_data(XRFImage * data_ptr);
  void set_current_palette(QString palette);

  XRFImage default_image_;
private:
  //int * shared_memory_cmd;
  //int * shared_memory;
  shm::array<int> shared_memory_cmd;
  shm::array<int> shared_memory;

  int pixel_dim_;
  bool left_mouse_clicked_;
  bool coordinates_found_;
  bool map_opened_;

  int x_image_;
  int y_image_;

  QImage qimage_;
  QString current_palette_;
  QTextEdit * debug_console_;
  XRFImage * image_data_;

  void mousePressEvent(QMouseEvent * event);
  void mouseReleaseEvent(QMouseEvent * event);
  void wheelEvent(QWheelEvent * event);
};

#endif // IMAGE_DISPLAY_H

