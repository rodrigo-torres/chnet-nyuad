#include "include/image_display.h"
#include "include/viridis.h"

ImgLabel::ImgLabel() : pixel_dim_ {1}, left_mouse_clicked_ {false},
  coordinates_found_ {false}, map_opened_ {false}
{
  shm::TypeDefInitSHM shm_cmd {6900, 4096, IPC_CREAT | 0666};
  shm::TypeDefInitSHM shm {7000, 409600, IPC_CREAT | 0666};

  shared_memory_cmd.initialize(shm_cmd);
  shared_memory.initialize(shm);

  CreatePalettes();
}

ImgLabel::~ImgLabel()
{}

void ImgLabel::AddImageToBuffer()
{
  QString filename = QFileDialog::getOpenFileName(
      nullptr, "Open XRF Image Data File", data_directory);

  if (filename.isEmpty())
  {
    QMessageBox::warning(this, "Warning!",
                         "You have chosen an empty filename.\n"
                         "No image data has been loaded onto memory.");
    return;
  }

  // We create a new XRFImage object in the heap. A raw pointer is used instead
  //  of a smart pointer because we need to take the address of the object to
  //  connect its signals to the main QWidget
  XRFImage  * new_image = new XRFImage{};
  connect(new_image, &XRFImage::UpdateProgressBar,
          this, &ImgLabel::RelayProgressBarSignal);

  // Now, how do we make a smart pointer point to an address of an object
  //  that already exists in the heap?
  // Step 1: Make a std:unique_ptr for a new XRFImage object
  std::unique_ptr<XRFImage> ptr{};
  // Step 2: Replace the managed object of the pointer with the address of the
  //  first XRFImage object (currently 'managed' by a raw pointer)
  ptr.reset(new_image);
  // Step 3: Transfer ownership of the image to the buffer through std::move
  images_.push_back(std::move(ptr));
	images_.back()->LoadDataFile(filename.toStdString());

	if (images_.back()->is_valid())
	{
		// Assign the iterator to point to the last element in the buffer
		image_on_display_ = images_.end() - 1;
		RenderAndPaintImage();
	}
	else
	{
	  // The last element is destroyed. Since each element is a unique pointer,
	  //  the XRFImage object in the heap is destroyed, and all resources freed.
	  images_.pop_back();
		QMessageBox::critical(this, "Error!",
		                      QString::fromStdString(images_.back()->err_msg()));
		map_opened_ = false;
	}
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

void ImgLabel::set_current_palette(QString palette)
{
  selected_palette = palette;
}

void ImgLabel::RenderAndPaintImage()
{
  // TODO RenderQImage has to set x_length and y_length fields to proper values
  auto & img_data = *image_on_display_;
  qimage_ = img_data->RenderQImage();

  QStringList available_palettes{};
  for (auto & it : palettes_)
  {
    available_palettes.append(it.first);
  }

  bool valid {false};
  QString item =
      QInputDialog::getItem(this, "Color Palette", "Using color palette:",
                            available_palettes, 0, false, &valid);
  selected_palette = (valid == true) ? item : available_palettes.at(0);
  RepaintImage();
}

/**
  *  @brief Handles resizing events and changes in the palette selection
  */
void ImgLabel::RepaintImage()
{
  auto & img_data = *image_on_display_;
  QSize requested_size {img_data->x_length() * pixel_dim_,
                        img_data->y_length() * pixel_dim_};

  if (qimage_.size() != requested_size)
  {
    // Resize the existing image to the new pixel dimensions
    // The Qt::FastTransformation flag ensures the image is not transformed
    //  further.
    qimage_ = qimage_.scaled(requested_size,
                   Qt::KeepAspectRatio, Qt::FastTransformation);
  }

  auto color_table = palettes_.find(selected_palette);
  if (color_table == palettes_.end())
  {
    // The palette does not exist.
    // Default to a given palette and inform the user
    color_table = palettes_.begin();
  }
  qimage_.setColorTable(color_table->second);

  // Now actually repaint the displayed image
  QCursor cursor(QPixmap::fromImage(qimage_));
  cursor.setShape(Qt::PointingHandCursor);


  this->setPixmap(QPixmap::fromImage(qimage_));
  this->setCursor(cursor);
  this->resize(qimage_.size());

  map_opened_ = true;
}

void ImgLabel::CreatePalettes()
{
  QColor color;
  ColorTable grayscale;
  // A color-blind friendly palette
  ColorTable viridis_palette;

  // We use 8-bit color palettes
  for (int i = 0; i < 0x100; ++i)
  {
    color.setRgb(i, i, i, 255);
    grayscale.push_back(color.rgb());

    color.setRgb(viridis[i][0], viridis[i][1], viridis[i][2], 255);
    viridis_palette.push_back(color.rgb());
  }

  palettes_.insert(std::make_pair("Grayscale", grayscale));
  palettes_.insert(std::make_pair("Viridis", viridis_palette));
}



void ImgLabel::wheelEvent(QWheelEvent *event)
{
  if (!map_opened_)
  {
    event->ignore();
    return;
  }

  auto delta = event->angleDelta().y();
  delta = (delta > 0) ? 1 : -1;
  pixel_dim_ += delta;

  if (pixel_dim_ < 0)
  {
    pixel_dim_ = 1;
    QMessageBox msg_box;
    msg_box.warning(this, "Warning!", "Can't zoom out the image any further!");
    event->ignore();
  }
  else
  {
    RepaintImage();
    event->accept();
  }

}

void ImgLabel::mousePressEvent(QMouseEvent *event) {
  if (!map_opened_)
  {
    return;
  }

  if (event->buttons() == Qt::LeftButton)
  {
    auto & img_data = *image_on_display_;
    // Left click: Tells me the integral of the point
    left_mouse_clicked_ = true;

    x_image_ = event->x() / pixel_dim_;
    y_image_ = event->y() / pixel_dim_;

    uint pixel;
    if (img_data->FindPixelWithCoordinates(x_image_, y_image_, &pixel))
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
    auto & img_data = *image_on_display_;
    left_mouse_clicked_ = false;

    uint pixel;
    int x_image2 = event->x() / pixel_dim_;
    int y_image2 = event->y() / pixel_dim_;
    if (img_data->FindPixelWithCoordinates(x_image2, y_image2, &pixel))
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
          pixel = (i * img_data->x_length()) + j;
          pixels_selected.push_back(pixel);
        }
      }
      img_data->ComputeROISpectrum(std::move(pixels_selected));
    }
    else
    {
      // The only remaining logical scenario is that both coordinates
      //  are the same, i.e., a point was clicked.
      auto pixel = y_image2 * (x_image2 + 1);
      std::vector<int> pixel_selected{1, pixel};
      img_data->ComputeROISpectrum(std::move(pixel_selected));
    }

    int * spectrum_display_ptr = nullptr;
    auto spectrum = img_data->roi_spectrum();

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








