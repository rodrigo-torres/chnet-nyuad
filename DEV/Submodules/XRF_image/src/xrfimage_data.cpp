#include "MAXRF/xrfimage.h"

#define MAXRF_DEBUG 1

XRFImage::XRFImage(QWidget* parent) : roi_low{lowest_bin}, roi_high{highest_bin}
{
  Q_UNUSED(parent)
}

XRFImage::~XRFImage() {}


bool XRFImage::is_valid()
{
  return  buffer_.size() > 0 ? (**buffer_it_).valid : false;
}


auto XRFImage::err_mesg() -> std::string
{
  return  buffer_.size() > 0 ? (**buffer_it_).err_msg :
                               "[!] No image is loaded";
}

auto XRFImage::roi_spectrum() -> Histogram &
{
  return roi_spectrum_;
}

auto XRFImage::IsPixelOnImage(size_t x, size_t y) -> bool
{
  if (buffer_.size() == 0)
  {
    return false;
  }

  auto & data_file = **buffer_it_;
  if ( data_file.x_length_ > x || data_file.y_length_ > y)
  {
    return true;
  }
  else
  {
    return false;
  }

}


auto XRFImage::FindPixelWithCoordinates(size_t x, size_t y) -> size_t
{
  // Checks for a valid state of the buffer, i.e., the iterator is valid
  // Checks if pixel is in bounds
  if (IsPixelOnImage(x, y))
  {
    return y * (**buffer_it_).x_length_ + x;
  }
  else
  {
    return std::numeric_limits<size_t>::max();
  }
}

void XRFImage::ComputeROISpectrum(std::vector<int>&& pixels_selected)
{
  std::fill(roi_spectrum_.begin(), roi_spectrum_.end(), 0);
  for (auto val : pixels_selected)
  {
    std::string line;
    auto & p = (**buffer_it_).image_data.at(val);
    auto & file = (**buffer_it_).file;
    int i_line = 0, channel = 0, count = 0;

    file.seekg(p->first_datum_pos);
    getline(file, line);
    while (line.front() != 'P' && !file.eof())
    {
      i_line = stoi(line);
      channel = (i_line & 0xFFFC000) >> 14;
      count = (i_line & 0x3FFF);
      roi_spectrum_.at(channel) += count;
      getline(file, line);
    }
  }
}

void XRFImage::UpdatePixelIntegrals()
{
  auto & maximum_integral_ = (**buffer_it_).maximum_integral_;
  auto & minimum_integral_ = (**buffer_it_).minimum_integral_;
  auto & image_data_ = (**buffer_it_).image_data;
  auto & file_ = (**buffer_it_).file;


  // TODO get values of integral limits from somwehere else other than SHM
  minimum_integral_ = std::numeric_limits<int>::max();
  maximum_integral_ = 0;

  constexpr int32_t bin_mask = 0xFFFC000;
  constexpr int32_t counts_mask = 0x0003FFF;

  // We omit a check for division by zero becase the LoadDataFile method should
  //  check for the validity of the ImageData buffer and abort if needed.
  double progress_step = 100. / image_data_.size();

  std::string line{};
  for (auto & p: image_data_)
  {
    p->integral = 0;

    file_.seekg(p->first_datum_pos);
    while (true)
    {
      getline(file_, line);
      if (line.front() == 'P' || file_.eof())
      {
        break;
      }

      int datum  = stoi(line);
      int bin    = (datum & bin_mask) >> 14;
      int counts = (datum & counts_mask);
      if (bin > roi_low && bin < roi_high)
      {
        p->integral += counts;
      }
    }

    // Update the image intensity maxima if needed
    if (p->integral < minimum_integral_)
    {
      minimum_integral_ = p->integral;
    }
    else if (p->integral > maximum_integral_)
    {
      maximum_integral_ = p->integral;
    }

    emit UpdateProgressBar(static_cast<int>(progress_step));
    progress_step += progress_step;
  }
  emit UpdateProgressBar(0);
}

auto XRFImage::RenderQImage() -> std::unique_ptr<UnprocessedImage>
{
  auto & file = **buffer_it_;
  auto width = static_cast<int>(file.x_length_);
  auto height = static_cast<int>(file.y_length_);

  auto image = std::make_unique<UnprocessedImage>();
  image->matrix = QImage{width, height, QImage::Format_Indexed8};
  image->matrix.setColorCount(256);
  std::fill(image->histogram.begin(), image->histogram.end(), 0);



  UpdatePixelIntegrals();
  if (file.maximum_integral_ == 0)
  {
    // The maximum integral of any given pixel is 0, so the image is null
    //  We return a black image (which reflects the fact above)
    image->matrix.fill(Qt::black);
    return image;
  }

  double intensity = 0;
  for (auto & pixel : file.image_data)
  {
    intensity = static_cast<double>(pixel->integral) / file.maximum_integral_;
    intensity = intensity * 255;

    ++image->histogram.at(static_cast<size_t>(intensity));

    image->matrix.setPixel(pixel->x_coord, pixel->y_coord,
                          static_cast<uint>(intensity));
  }
  // We can return by 'value' because of copy elision
  return image;
}


// Starting contents of source file
void XRFImage::LoadDataFile(std::string filename)
{
  auto file_bundle = std::make_unique<DataFile>();
  auto & file_ = file_bundle->file;
  auto & err_msg_ = file_bundle->err_msg;
  auto & valid_ = file_bundle->valid;
  auto & image_data_ = file_bundle->image_data;
  auto & sum_spectrum_ = file_bundle->sum_spectrum;

  // Some information about the file first
  struct stat stat_buf;
  int rc = stat(filename.c_str(), &stat_buf);
  auto size_bytes = (rc == 0) ? stat_buf.st_size : -1;


  file_ = std::fstream{filename, std::fstream::in};
  if (file_.is_open())
  {
    std::string line{};
    getline(file_, line);

    if (line.compare("{") != 0)
    {
      err_msg_ = "The file you have chosen is in a format that cannot be parsed.";
      // TODO trigger a file conversion
      valid_ = false;
      return;
    }

    getline(file_, line);
    while (line.compare("}") != 0)
    {
      // Parse the header
      // Populate the scan parameters
      getline(file_, line);
    }

    int integral = 0;
    auto write_pixel = [&](const QString& pixel_header){
      // The lambda captures the pixel integral, and file position

      // TODO ascert no. of tokens
      auto tokens = pixel_header.split('\t');
      auto pixel = std::make_unique<Pixel>();
      pixel->first_datum_pos = file_.tellg();
      pixel->integral = 0;
      pixel->pixel_no = tokens.at(1).toInt();
      pixel->x_coord = tokens.at(2).toInt();
      pixel->y_coord = tokens.at(3).toInt();

      double progress = static_cast<double>(pixel->first_datum_pos) / size_bytes;
      progress *= 100;

      emit UpdateProgressBar(static_cast<int>(progress));

      image_data_.push_back(std::move(pixel));
      if (image_data_.size() > 1)
      {
        auto& ptr = *(image_data_.rbegin() + 1);
        ptr->integral = integral;
        integral = 0;
      }
    };

    uint channel = 0, count = 0;
    int_least32_t line_int = 0;

    std::fill(sum_spectrum_.begin(), sum_spectrum_.end(), 0);
    getline(file_, line);
    while (!file_.eof())
    {
      if (line.front() == 'P')
      {
        write_pixel(QString::fromStdString(line));
      }
      else
      {
        line_int = stoi(line);
        channel = (line_int & 0xFFFC000) >> 14;
        count = (line_int & 0x3FFF);
        integral += count;
        sum_spectrum_.at(channel) += count;
      }
      getline(file_, line);
    }
    image_data_.back()->integral = integral;

    valid_ = true;


    // COMPUTE X LENGTH
    // TODO error checking
    auto & it  = *file_bundle;
    it.x_length_ = 0;
    for (uint i = 1; i <image_data_.size(); ++i)
    {
      // We start the loop from index 1, as index 0 should already have x_coord  = 0
      if (image_data_.at(i)->x_coord == 0)
      {
        it.x_length_ = i;
        break;
      }
    }

    // COMPUTE X LENGTH
    // TODO error checking
    it.y_length_ = image_data_.back()->y_coord + 1;

    it.pixels_in_image_ = it.x_length_ * it.y_length_;

    std::vector<int> integrals;
    for (auto & i : it.image_data)
    {
      integrals.push_back(i->integral);
    }
    it.maximum_integral_ = *std::max_element(integrals.begin(), integrals.end());
    it.minimum_integral_ = *std::min_element(integrals.begin(), integrals.end());

    buffer_.push_back(std::move(file_bundle));
    buffer_it_ = buffer_.end() - 1;

    emit UpdateProgressBar(0);
  }

  else
  {
    err_msg_ = "The file cannot be opened.\n"
               "Check format and access privileges.";
    valid_ = false;
  }
}
