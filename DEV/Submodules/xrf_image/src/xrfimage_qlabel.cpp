#include "MAXRF/image_display.h"
#include "MAXRF/palettes.h"

ImgLabel::ImgLabel(QWidget * parent) :
  left_mouse_clicked_ {false}, coordinates_found_ {false},
  map_opened_ {false}, renderers_{}, parent_(parent)
{
  shared_memory_cmd.initialize(Segments::SHARED_MEMORY_CMD);
  shared_memory.initialize(Segments::SHARED_MEMORY_1);

  CreatePalettes();
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

  // We construct a default-initialized XRFImage object on the back
  auto image  = new XRFImage;
  connect(image, &XRFImage::UpdateProgressBar,  this,
           &ImgLabel::RelayProgressBarSignal);

  std::unique_ptr<XRFImage> ptr{image};
  renderers_.push_back(std::move(ptr));
  auto & renderer = *renderers_.back();

  renderer.LoadDataFile(filename.toStdString());
  if (renderer.is_valid())
  {
    images_.emplace_back();
    active_index_ = images_.size() - 1;
    RenderImage();

    auto pos = filename.lastIndexOf('/');
    emit AddToComboBox(filename.mid(pos));
    emit EnableImageControls();
  }
  else
  {
    QMessageBox::critical(this, "Error!",
                          QString::fromStdString(renderer.err_mesg()));
    renderers_.pop_back();
    map_opened_ = false;
  }
}

void ImgLabel::ChangeImage(int index)
{
  if (index == 0) {
    // Show the logo
    QImage image(":/images/TT_CHNet_res2.png");
    setPixmap(QPixmap::fromImage(image));
    setBackgroundRole(QPalette::Base);
    resize(image.size());
  }
  else {
    // Change the index
    active_index_ = index - 1;
    PaintImage();
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
  images_.at(active_index_)->pixel_replication = dim;
}

void ImgLabel::ToggleEqualization (bool state)
{
  if (state) {
    images_.at(active_index_)->flags |= kEqualize;
    images_.at(active_index_)->flags &= ~kStretch;
  }
  else {
    images_.at(active_index_)->flags &= ~kEqualize;
  }
  ProcessImage();
}

void ImgLabel::ToggleStretching(bool state)
{
  if (state) {
    images_.at(active_index_)->flags |= kStretch;
    images_.at(active_index_)->flags &= ~kEqualize;
  }
  else {
    images_.at(active_index_)->flags &= ~kStretch;
  }
  ProcessImage();
}

void ImgLabel::set_brightness(int percentage)
{
  // Values are constrained between -50% and +50%
  images_.at(active_index_)->brightness = static_cast<int>(percentage * 2.56);
  if (percentage == 0) {
    images_.at(active_index_)->flags &= ~kBrighten;
  }
  else {
    images_.at(active_index_)->flags |= kBrighten;
  }
  ProcessImage();
}

void ImgLabel::RenderImage()
{
  auto image = renderers_.at(active_index_)->RenderQImage();

  // Get a list of available palettes and ask the user to select one
  QStringList available_palettes{};
  for (auto & it : palettes_)
  {
    available_palettes.append(it.first);
  }
  bool valid {false};
  QString item =
      QInputDialog::getItem(this, "Color Palette", "Using color palette:",
                            available_palettes, 0, false, &valid);

  // Since we just rendered the image, there's no color palette active on the
  //  image. So if the user cancels we assign the default one
  // TODO this could be improved by using the last used value
  selected_palette = (valid == true) ? item : available_palettes.at(0);

  auto & palette = palettes_.find(selected_palette)->second;
  image->processed.setColorTable(palette);
  images_.at(active_index_).swap(image);
}

void ImgLabel::AdjustContrast()
{
  auto & image = *images_.at(active_index_);

  // Find the first non-zero histogram element going from beginning to end
  auto it =  std::find_if(image.nhisto.begin(), image.nhisto.end(),
                           [] (uint & n) -> bool {return n != 0;});

  if (it == image.nhisto.end())
  {
    // The histogram holds only zero values, contrast cannot be increases
    // Attempting to increase contrast would lead to division by zero
    return;
  }

  // Find the first non-zero histogram element going from end to beginning
  auto rit = std::find_if(image.nhisto.rbegin(), image.nhisto.rend(),
                          [] (uint & n) -> bool {return n != 0;});

  if (it == rit.base() - 1)
  {
    // The iterators are pointing to the same element
    // Increasing contrast would lead to division by zero
    return;
  }

  auto min = std::distance(image.nhisto.begin(), it);
  auto max = std::distance(image.nhisto.begin(), rit.base() - 1);
  auto factor = 255. / (max - min);

  if (factor == 1)
  {
    //  Update Status Bar
    return;
  }

  uint new_i;
  std::vector<uint> temp(256, 0);
  // Now we update the palette
  // We get the original palette to assign new color values to the current one
  auto & original = palettes_.find(selected_palette)->second;
  std::vector<uint> palette(256, 0);

  for (long i = max; i >= min; --i)
  {
    new_i = (i - min) * factor;
    temp.at(new_i) = image.nhisto.at(i);
    palette.at(new_i) = original.at(i);
  }
  temp.swap(image.nhisto);

  // Fill missing values in palette
  for (auto it = palette.begin() + 1; it != palette.end(); ++it)
  {
    if (*it == 0) {
      *it = *(it - 1);
    }
  }

  image.processed.setColorTable(QVector<uint>::fromStdVector(palette));

}

template<typename T>
static auto ShiftVectorRight(std::vector<T> & vec,
                             int shift, T fill_val = 0) -> void
{
  static_assert (std::is_integral<T>::value,
                 "Template parameter is not an integral value");
  // Shifting of the vector to the right
  std::move_backward(vec.begin(), vec.end() - shift, vec.end());
  // Assigning the out-of-range values to fill_val
  std::fill(vec.begin(), vec.begin() + shift, fill_val);
}

template<typename T>
static auto ShiftVectorLeft(std::vector<T> & vec,
                             int shift, T fill_val = 0) -> void
{
  // Shifting of the vector to the left
  std::move(vec.begin() + shift, vec.end(), vec.begin());
  // Assigning the out-of-range values to fill_val
  std::fill(vec.rbegin(), vec.rbegin() + shift, fill_val);
}

///
/// \brief Equalize the Histogram
/// \note Nothing to see here, this method is locked and optimized
///
auto ImgLabel::EqualizeHistogram() -> void
{

  // We get a reference to the image being displayed (and thus to its histogram)
  auto & image = *images_.at(active_index_);

  // Error checking the image isn't empty -> would lead to division by zero
  if (image.matrix.size() == QSize{0, 0})
  {
    std::cout << "[!] Can't transform an empty image" << std::endl;
    return;
  }

  // We set up some aliases for better readability
  using container_type = decltype (image.nhisto);
  using type = container_type::value_type;

  // Error-checking the value_type of the histogram is an integral type
  static_assert (std::is_integral<type>::value,
                 "The value_type of the histogram is not an integral type!" );

  // We'll asign the elements of the equalized histogram in order, but it's not
  // guaranteed will jump forward in the container.
  // So we default-initialize its N elements
  container_type equalized(image.nhisto.size());

  // We get a reference to the untransformed look-up table for the image
  // No copying is involved
  auto & look_up_table = palettes_.find(selected_palette)->second;

  // Since we assign the elements of the new palette in order, we construct
  // a default initialized container, and get a back insert iterator
  container_type palette;
  auto palette_inserter = std::back_inserter(palette);

  // CDF factor is the equalization factor for the cumulative histogram
  double cdf_factor = 255. / (image.matrix.width() * image.matrix.height());
  // CDF entry is an entry in the cumulative histogram multiplied by cdf_factor
  double cdf_entry{};

  std::for_each(image.nhisto.begin(), image.nhisto.end(),  [&] (type & n) {
      // We construct the CDF as we go, we only use the latest entry at any time
      // Notice we compute the cumulative histogram, and equalize in the same op
      cdf_entry += n * cdf_factor;
      // We reassing the counts on the equalized histogram based on the  CDF
      equalized.at(static_cast<type>(cdf_entry)) += n;
      // We transform the colors of the palette based on the CDF
      *palette_inserter = look_up_table.at(static_cast<int>(cdf_entry));
    });

  equalized.swap(image.nhisto);

  if (image.brightness > 0) {
    ShiftVectorLeft<type>(palette, image.brightness, 0xFFFFFFFF);
  }
  else if (image.brightness < 0) {
    ShiftVectorRight<type>(palette, std::abs(image.brightness), 0xFF000000);
  }
  image.processed.setColorTable(QVector<type>::fromStdVector(palette));
}



///
/// \brief Adjust the brightness levels of the displayed image
/// \note Nothing to see here, this method is locked and optimized
///
void ImgLabel::AdjustBrightness()
{
  auto & image = *images_.at(active_index_);
  auto & histo = image.nhisto;

  auto palette = image.processed.colorTable().toStdVector();
  auto offset = std::abs(image.brightness);

  using type = decltype (palette)::value_type;

  if (image.brightness < 0)
  {
    ShiftVectorRight<type>(palette, offset, 0xFF000000);
    image.processed.setColorTable(QVector<type>::fromStdVector(palette));

    // We shift the image histogram to the left
    ShiftVectorLeft<type>(histo, offset, 0);
    histo.front() = static_cast<uint>(std::accumulate(image.histogram.begin(),
                                        image.histogram.begin() + offset, 0));
  }
  else
  {
    ShiftVectorLeft<type>(palette, offset, 0xFFFFFFFF);
    image.processed.setColorTable(QVector<type>::fromStdVector(palette));

    // We shift the image histogram to the right
    ShiftVectorRight<type>(histo, offset, 0);
    histo.back() = static_cast<uint>(std::accumulate(image.histogram.rbegin(),
                                        image.histogram.rbegin() + offset, 0));  }
}

///
/// \brief Resize the displayed image
/// \note Nothing to see here, this method is locked and optimized
///
void ImgLabel::ResizeImage()
{
  auto & img = *images_.at(active_index_);
  QSize requested_size { img.matrix.width() * img.pixel_replication,
                          img.matrix.height() * img.pixel_replication };

  img.processed = img.processed.scaled(requested_size, Qt::KeepAspectRatio);
}

/**
  *  @brief Handles resizing events and changes in the palette selection
  */
void ImgLabel::ProcessImage()
{
  auto & img = *images_.at(active_index_);

  std::copy(img.histogram.begin(), img.histogram.end(),
            img.nhisto.begin());
  img.flags |= kPalette;
  if (img.flags & kPalette)
  {
    auto const & color_table = palettes_.find(selected_palette);
    img.processed.setColorTable(color_table->second);
    img.flags &= ~kPalette;
  }
  if (img.flags & kBrighten)
  {
    AdjustBrightness();
  }
  if (img.flags & kEqualize)
  {
    //AdjustContrast();
    EqualizeHistogram();
  }
  if (img.flags & kStretch)
  {
    AdjustContrast();
  }
  if (img.flags & kResize)
  {
    ResizeImage();
  }
  PaintImage();
  emit UpdateImageHistogram(QVector<uint>::fromStdVector(img.nhisto));
}

void ImgLabel::PaintImage()
{

  displayed_image_ =  images_.at(active_index_)->processed;
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
  auto & pixel_dim_ = images_.at(active_index_)->pixel_replication;

  auto delta = event->angleDelta().y();
  delta = (delta > 0) ? 1 : -1;
  images_.at(active_index_)->pixel_replication += delta;

  if (pixel_dim_ < 1)
  {
    pixel_dim_ = 1;
    images_.at(active_index_)->flags &= ~kResize;
    QMessageBox msg_box;
    msg_box.warning(this, "Warning!", "Can't zoom out the image any further!");
    event->ignore();
  }
  else
  {
    images_.at(active_index_)->flags |= kResize;
    ResizeImage();
    PaintImage();
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
    auto &img = *images_.at(active_index_);
    // Left click: Tells me the integral of the point
    left_mouse_clicked_ = true;

    x_image_ = event->x() / img.pixel_replication;
    y_image_ = event->y() / img.pixel_replication;

    //if (img_data->FindPixelWithCoordinates(x_image_, y_image_, &pixel))
    if (renderers_.at(active_index_)->IsPixelOnImage(x_image_, y_image_))
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
    auto &img = *images_.at(active_index_);
    left_mouse_clicked_ = false;

    int x_image2 = event->x() / img.pixel_replication;
    int y_image2 = event->y() / img.pixel_replication;
    if (renderers_.at(active_index_)->IsPixelOnImage(x_image2, y_image2))
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
      renderers_.at(active_index_)->ComputeROISpectrum(std::move(pixels_selected));
    }
    else
    {
      // The only remaining logical scenario is that both coordinates
      //  are the same, i.e., a point was clicked.
      auto pixel = y_image2 * (x_image2 + 1);
      std::vector<int> pixel_selected{1, pixel};
      renderers_.at(active_index_)->ComputeROISpectrum(std::move(pixel_selected));
    }

    int * spectrum_display_ptr = nullptr;
    auto spectrum = renderers_.at(active_index_)->roi_spectrum();

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








