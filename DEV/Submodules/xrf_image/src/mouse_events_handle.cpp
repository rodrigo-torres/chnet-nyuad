#include "MAXRF/image_display.h"

namespace maxrf {

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


//void ImgLabel::wheelEvent(QWheelEvent *event)
//{
//  if (!map_opened_)
//  {
//    event->ignore();
//    return;
//  }
//  auto & pixel_dim_ = images_.at(image_index_)->pixel_replication;

//  auto delta = event->angleDelta().y();
//  delta = (delta > 0) ? 1 : -1;
//  images_.at(image_index_)->pixel_replication += delta;

//  if (pixel_dim_ < 1)
//  {
//    pixel_dim_ = 1;
//    images_.at(image_index_)->flags &= ~kResize;
//    QMessageBox msg_box;
//    msg_box.warning(this, "Warning!", "Can't zoom out the image any further!");
//    event->ignore();
//  }
//  else
//  {
//    images_.at(image_index_)->flags |= kResize;
//    ResizeImage();
//    PaintImage();
//    event->accept();
//  }

//}

void ImgLabel::mousePressEvent(QMouseEvent *event) {
  if (!map_opened_)
  {
    return;
  }

  if (event->buttons() == Qt::LeftButton)
  {
    //auto & img_data = *image_on_display_;
    auto &img = *images_.at(image_index_);
    // Left click: Tells me the integral of the point
    left_mouse_clicked_ = true;

    x_image_ = event->x() / img.pixel_replication;
    y_image_ = event->y() / img.pixel_replication;

    //if (img_data->FindPixelWithCoordinates(x_image_, y_image_, &pixel))
    if (renderer_.IsPixelOnImage(x_image_, y_image_))
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

void ImgLabel::mouseReleaseEvent(QMouseEvent *event) { // Click and release in different pixel -> displays the rectangle's count integral

  if (map_opened_ && left_mouse_clicked_) {
    //auto & img_data = *image_on_display_;
    auto &img = *images_.at(image_index_);
    left_mouse_clicked_ = false;

    int x_image2 = event->x() / img.pixel_replication;
    int y_image2 = event->y() / img.pixel_replication;
    if (renderer_.IsPixelOnImage(x_image2, y_image2))
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
          pixel = (i * img.dimensions.width()) + j;
          pixels_selected.push_back(pixel);
        }
      }
      renderer_.ComputeROISpectrum(std::move(pixels_selected));
    }
    else
    {
      // The only remaining logical scenario is that both coordinates
      //  are the same, i.e., a point was clicked.
      auto pixel = y_image2 * (x_image2 + 1);
      std::vector<int> pixel_selected{1, pixel};
      renderer_.ComputeROISpectrum(std::move(pixel_selected));
    }

    int * spectrum_display_ptr = nullptr;
    auto spectrum = renderer_.roi_spectrum();

    switch (shared_memory_cmd.at(100))
    {
    case 0:
      spectrum_display_ptr = &shared_memory.at(100);
      break;
    case 1:
      spectrum_display_ptr = &shared_memory.at(20000);
      break;
    case 2:
      spectrum_display_ptr = &shared_memory.at(40000);
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

    shared_memory.at(99)= 1;
  }
}

}
