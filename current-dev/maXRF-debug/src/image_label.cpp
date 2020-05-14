#include "include/image_display.h"
#include "include/mainwindow.h"

extern int *shared_memory, *shared_memory_cmd;

ImgLabel::ImgLabel() : pixel_dim_ {1}, left_mouse_clicked_ {false},
  coordinates_found_ {false}, map_opened_ {false}
{

}

bool ImgLabel::is_map_opened() const
{
  return map_opened_;
}

QImage ImgLabel::qimage() const
{
  return qimage_;
}

void ImgLabel::set_pixel_dim(int dim)
{
  pixel_dim_ = dim;
}
void ImgLabel::set_map_opened(bool state)
{
  map_opened_ = state;
}

void ImgLabel::set_image_data(XRFImage * data_ptr)
{
  image_data_ = data_ptr;
}

void ImgLabel::DisplayImage(QString palette)
{
  qimage_ = image_data_->ConstructQImage(palette, pixel_dim_);
  QCursor cursor(QPixmap::fromImage(qimage_));
  cursor.setShape(Qt::PointingHandCursor);

  setPixmap(QPixmap::fromImage(qimage_));
  setCursor(cursor);
  resize(sizeHint());

  map_opened_ = true;
}

void ImgLabel::mousePressEvent(QMouseEvent *event) {
  if (!map_opened_)
  {
    return;
  }

  if (event->buttons() == Qt::LeftButton)
  {
    // Left click: Tells me the integral of the point
    left_mouse_clicked_ = true;

    x_image_ = event->x() / pixel_dim_;
    y_image_ = event->y() / pixel_dim_;

    uint pixel;
    if (image_data_->FindPixelWithCoordinates(x_image_, y_image_, &pixel))
    {
      // Pixel coordinates exist on the image
      coordinates_found_ = true;
      //qDebug()<<"[!] Point integral is "<<integral[it];
    }
    else
    {
      coordinates_found_ = false;
      qDebug()<<"[!] First point not found in map";
    }
  }
  else if (event->buttons() == Qt::RightButton)
  {
    // Right pixel: move motors to the position of the pixel
    // TODO implement moving of motors
  }
}

static int largest(int a, int b) {
  int ret;
  a > b ? ret = a : ret = b;
  return  ret;
}

static int smallest(int a, int b) {
  int ret;
  a < b ? ret = a : ret = b;
  return  ret;
}


void ImgLabel::mouseReleaseEvent(QMouseEvent *event) { // Click and release in different pixel -> displays the rectangle's count integral

  if (map_opened_ && left_mouse_clicked_) {
    left_mouse_clicked_ = false;

    uint pixel;
    int x_image2 = event->x() / pixel_dim_;
    int y_image2 = event->y() / pixel_dim_;
    if (image_data_->FindPixelWithCoordinates(x_image2, y_image2, &pixel))
    {
    }
    else {
      qDebug()<<"[!] Second point not found in map";
      return;
    }

    if (!coordinates_found_)
    {
      // First set of coordinates was not found.
      return;
    }


    /* If a rectangle is selected on the map */

    // If a rectangle is selected on the map, generate a list of pixels
    //  contained in that region, and parse the spectra of those pixels
    //  only.
    if (x_image_ != x_image2 || y_image_ != y_image2)
    {
      auto min_x = smallest(x_image_, x_image2);
      auto min_y = smallest(y_image_, y_image2);
      auto max_x = largest(x_image_, x_image2);
      auto max_y = largest(y_image_, y_image2);
      max_x++;
      max_y++;

//      auto length_y = max_y - min_y + 1;
//      auto length_x = max_x - min_x + 1;
//      auto area = length_x * length_y;

      int pixel = 0;
      std::vector<int> pixels_selected;
//      pixels_selected.reserve(area);
      for (int i = min_y; i < max_y; ++i)
      {
        for (int j = min_x; j < max_x; ++j)
        {
          pixel = (i * image_data_->x_length()) + j;
          pixels_selected.push_back(pixel);
        }
      }
      image_data_->ComputeROISpectrum(std::move(pixels_selected));
    }
    else
    {
      // The only remaining logical scenario is that both coordinates
      //  are the same, i.e., a point was clicked.
      auto pixel = y_image2 * (x_image2 + 1);
      std::vector<int> pixel_selected{1, pixel};
      image_data_->ComputeROISpectrum(std::move(pixel_selected));
    }

    int * spectrum_display_ptr = nullptr;
    auto spectrum = image_data_->roi_spectrum();

    switch (shared_memory_cmd[100])
    {
    case 0:
      spectrum_display_ptr = &shared_memory[100];
      break;
    case 1:
      spectrum_display_ptr = &shared_memory[20000];
      break;
    case 2:
      spectrum_display_ptr = &shared_memory[40000];
      break;
    default:
      // TODO warning here
      break;
    }

    int i = 0;
    for (auto val : spectrum)
    {
      spectrum_display_ptr[i] = val;
      ++i;
    }

    shared_memory[99] = 1;
  }
}








