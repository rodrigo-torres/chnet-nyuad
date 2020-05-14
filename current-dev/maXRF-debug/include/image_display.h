#ifndef IMAGE_DISPLAY_H
#define IMAGE_DISPLAY_H

#include <QLabel>
#include <QTextEdit>

#include "include/xrfimage.h"

class ImgLabel : public QLabel
{
  // INHERITED PUBLIC METHODS
  // Mutator function for the label's QPixmap
  // void setPixmap(const QPixmap &)

public:
  ImgLabel();
  void DisplayImage(QString palette);

  // ACCESOR methods
  bool is_map_opened() const;
  QImage qimage() const;

  // MUTATOR methods
  void set_pixel_dim(int);
  void set_map_opened(bool state);
  void set_image_data(XRFImage * data_ptr);
private:
  int pixel_dim_;
  bool left_mouse_clicked_;
  bool coordinates_found_;
  bool map_opened_;

  int x_image_;
  int y_image_;

  QImage qimage_;
  QTextEdit * debug_console_;
  XRFImage * image_data_;

  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
};

#endif // IMAGE_DISPLAY_H

