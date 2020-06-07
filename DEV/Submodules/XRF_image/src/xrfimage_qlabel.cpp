#include "MAXRF/image_display.h"
#include "MAXRF/viridis.h"

ImgLabel::ImgLabel(QWidget * parent) :
  left_mouse_clicked_ {false}, coordinates_found_ {false},
  map_opened_ {false}, renderer{}, parent_(parent)
{
  shared_memory_cmd.initialize(Segments::SHARED_MEMORY_CMD);
  shared_memory.initialize(Segments::SHARED_MEMORY_1);

  CreatePalettes();
  connect(&renderer, &XRFImage::UpdateProgressBar,
          this, &ImgLabel::RelayProgressBarSignal);
}

ImgLabel::~ImgLabel()
{

}

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

  renderer.LoadDataFile(filename.toStdString());
  if (renderer.is_valid())
  {
    RenderAndPaintImage();
  }
  else
  {
    QMessageBox::critical(this, "Error!",
                          QString::fromStdString(renderer.err_mesg()));
    map_opened_ = false;
  }
}

bool ImgLabel::is_map_opened() const
{
  return map_opened_;
}

QImage ImgLabel::qimage() const
{
  return displayed_image_;
  //return qimage_;
}

void ImgLabel::set_pixel_dim(int dim)
{
  (**active_image_).pixel_replication = dim;
}

void ImgLabel::ToggleHistogramStretching(int state)
{
  switch (state)
  {
  case Qt::Unchecked :
    (**active_image_).contrast = false;
    break;
  case Qt::PartiallyChecked :
  case Qt::Checked :
    (**active_image_).contrast = true;
    break;
  }
  ProcessImage();
}

void ImgLabel::set_brightness(int percentage)
{
  // Values are constrained between -50% and +50%
  (**active_image_).brightness = static_cast<int>(percentage * 2.56);
  ProcessImage();
}

void ImgLabel::RenderAndPaintImage()
{
  auto image = renderer.RenderQImage();
  image->processed = image->matrix;
  std::vector<uint> nuisance{image->histogram.begin(), image->histogram.end()};
  emit UpdateImageHistogram(QVector<uint>::fromStdVector(nuisance));

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
  image->palette = selected_palette;

  if (images_.size() == 0)
  {
    images_.push_back(std::move(image));
    active_image_ = images_.end() - 1;
  }
  else
  {
    images_.back().swap(image);
  }
  ProcessImage();
}

void ImgLabel::AdjustContrast()
{
  // TODO implement simple histogram stretching
  auto & image = **active_image_;
  auto rit = std::find_if(image.nhistogram.rbegin(), image.nhistogram.rend(),
                          [] (uint & n) -> bool {return n != 0;});
  auto max = 255 - std::distance(image.nhistogram.rbegin(), rit);
  auto it =  std::find_if(image.nhistogram.begin(), image.nhistogram.end(),
                           [] (uint & n) -> bool {return n != 0;});
  auto min = std::distance(image.nhistogram.begin(), it);

  if (max != min || (max != 256 && min != 256))
  {
    double factor = (max - min);
    factor /= 255.;

    uint new_i;
    std::vector<uint> newvec;
    ColorTable new_palette{};
    ColorTable palette = image.processed.colorTable();
    for (long i = 0; i < 256; ++i)
    {
      new_i = i * factor + min;
      newvec.push_back(image.nhistogram.at(new_i));
      new_palette.push_back(std::move(palette.at(new_i)));
    }
    image.processed.setColorTable(new_palette);
    emit UpdateImageHistogram(QVector<uint>::fromStdVector(newvec));
}

}

void ImgLabel::AdjustBrightness()
{
  auto & image = **active_image_;

  auto palette = image.processed.colorTable();
  auto offset = std::abs(image.brightness);

  std::vector<uint> nuisance;
  if (image.brightness < 0)
  {
    nuisance.resize(offset, 0);
    std::move(palette.begin(), palette.end() - offset, std::back_inserter(nuisance));
    image.processed.setColorTable(QVector<uint>::fromStdVector(nuisance));
    nuisance.clear();
    std::copy(image.histogram.begin() + offset, image.histogram.end(),
              std::back_inserter(nuisance));
    nuisance.resize(256, 0);
    std::for_each(image.histogram.begin(), image.histogram.begin() + offset,
                  [&](int & n){nuisance.front() += n;});
  }
  else
  {
    std::move(palette.begin() + offset, palette.end(), std::back_inserter(nuisance));
    nuisance.resize(256, 0);
    image.processed.setColorTable(QVector<uint>::fromStdVector(nuisance));
    nuisance.clear();
    nuisance.resize(offset, 0);
    std::copy(image.histogram.begin(), image.histogram.end() - offset,
              std::back_inserter(nuisance));
    std::for_each(image.histogram.end() - offset, image.histogram.end(),
                  [&](int & n){nuisance.back() += n;});
  }
  image.nhistogram = nuisance;
  emit UpdateImageHistogram(QVector<uint>::fromStdVector(nuisance));
}

void ImgLabel::ResizeImage()
{
  auto & img = **active_image_;

  QSize requested_size {img.matrix.width() * img.pixel_replication,
        img.matrix.height() * img.pixel_replication};

  displayed_image_ = img.processed.scaled(requested_size, Qt::KeepAspectRatio);

  PaintImage();
}

/**
  *  @brief Handles resizing events and changes in the palette selection
  */
void ImgLabel::ProcessImage()
{
  auto & img = **active_image_;
  img.nhistogram.assign(img.histogram.begin(), img.histogram.end());

  auto color_table = palettes_.find(selected_palette);
  if (color_table == palettes_.end())
  {
    // The palette does not exist.
    // Default to a given palette and inform the user
    color_table = palettes_.begin();
  }
  //qimage_.setColorTable(color_table->second)
  img.processed.setColorTable(color_table->second);


  if (img.brightness != 0)
  {
    AdjustBrightness();
  }


  if (img.contrast != 0)
  {
    AdjustContrast();
  }

  ResizeImage();
}

void ImgLabel::PaintImage()
{
  // Now actually repaint the displayed image
  QCursor cursor (QPixmap::fromImage(displayed_image_));
  //QCursor cursor(QPixmap::fromImage(qimage_));
  cursor.setShape(Qt::PointingHandCursor);


  //this->setPixmap(QPixmap::fromImage(qimage_));
  this->setPixmap(QPixmap::fromImage(displayed_image_));
  this->setCursor(cursor);
  //this->resize(qimage_.size());
  this->resize(displayed_image_.size());

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
  auto & pixel_dim_ = (**active_image_).pixel_replication;

  auto delta = event->angleDelta().y();
  delta = (delta > 0) ? 1 : -1;
  (**active_image_).pixel_replication += delta;

  if (pixel_dim_ < 1)
  {
    pixel_dim_ = 1;
    QMessageBox msg_box;
    msg_box.warning(this, "Warning!", "Can't zoom out the image any further!");
    event->ignore();
  }
  else
  {
    ResizeImage();
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
    //auto & img_data = *image_on_display_;
    auto &img = (**active_image_);
    // Left click: Tells me the integral of the point
    left_mouse_clicked_ = true;

    x_image_ = event->x() / img.pixel_replication;
    y_image_ = event->y() / img.pixel_replication;

    //if (img_data->FindPixelWithCoordinates(x_image_, y_image_, &pixel))
    if (renderer.IsPixelOnImage(x_image_, y_image_))
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
    //auto & img_data = *image_on_display_;
    auto &img = (**active_image_);
    left_mouse_clicked_ = false;

    uint pixel;
    int x_image2 = event->x() / img.pixel_replication;
    int y_image2 = event->y() / img.pixel_replication;
    if (renderer.IsPixelOnImage(x_image2, y_image2))
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
          pixel = (i * img.matrix.width()) + j;
          pixels_selected.push_back(pixel);
        }
      }
      renderer.ComputeROISpectrum(std::move(pixels_selected));
    }
    else
    {
      // The only remaining logical scenario is that both coordinates
      //  are the same, i.e., a point was clicked.
      auto pixel = y_image2 * (x_image2 + 1);
      std::vector<int> pixel_selected{1, pixel};
      renderer.ComputeROISpectrum(std::move(pixel_selected));
    }

    int * spectrum_display_ptr = nullptr;
    auto spectrum = renderer.roi_spectrum();

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








