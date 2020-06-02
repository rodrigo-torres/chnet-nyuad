#include "include/xrfimage.h"
//#include "include/viridis.h"

#include <sys/stat.h> // For file information

bool XRFImage::is_valid()
{
  return  valid_;
}

std::string XRFImage::err_msg()
{
  return err_msg_;
}

bool XRFImage::FindPixelWithCoordinates(uint x, uint y, uint * pixel)
{
  if ( x >= x_length_ || y >= y_length_)
  {
    return false;
  }
  *pixel = y * x_length_;
  // Is the line odd or even?
  y % 2 == 0 ?
        *pixel += x :
      *pixel += (x_length_ - x - 1);

  return true;
}

void XRFImage::ComputeROISpectrum(std::vector<int>&& pixels_selected)
{
  roi_spectrum_.resize(16384);
  std::fill(roi_spectrum_.begin(), roi_spectrum_.end(), 0);
  for (auto val : pixels_selected)
  {
    std::string line;
    auto & p = image_data_.at(val);
    int i_line = 0, channel = 0, count = 0;

    file_.seekg(p->first_datum_pos);
    getline(file_, line);
    while (line.front() != 'P' && !file_.eof())
    {
      i_line = stoi(line);
      channel = (i_line & 0xFFFC000) >> 14;
      count = (i_line & 0x3FFF);
      roi_spectrum_.at(channel) += count;
      getline(file_, line);
    }
  }
}

void XRFImage::UpdateROIIntegrals()
{
  if (roi_low == shared_memory5.at(100) && roi_high == shared_memory5.at(101))
  {
    // ROI limits haven't changed
    return;
  }

  roi_low = shared_memory5.at(100);
  roi_high = shared_memory5.at(101);

  size_t current = 0;
  double size = image_data_.size();

  int i_line = 0, channel = 0, count = 0;
  std::string line;
  for (auto & p: image_data_)
  {
    p->integral = 0;

    file_.seekg(p->first_datum_pos);
    getline(file_, line);
    while (line.front() != 'P' && !file_.eof())
    {
      i_line = stoi(line);
      channel = (i_line & 0xFFFC000) >> 14;
      count = (i_line & 0x3FFF);
      if (channel > roi_low && channel < roi_high)
      {
        p->integral += count;
      }
      getline(file_, line);
    }
    emit UpdateProgressBar(static_cast<int>((current *100) / size));
    ++current;
  }
  emit UpdateProgressBar(0);
}

QImage XRFImage::RenderQImage()
{
  QImage image(x_length_, y_length_, QImage::Format_Indexed8);
  image.setColorCount(256);

  UpdateROIIntegrals();
  auto max_i = ComputeMaxIntegral();
  if (max_i == 0)
  {
    // The maximum integral of any given pixel is 0, so the image is null
    //  We return a black image (which reflects the fact above)
    image.fill(Qt::black);
    return image;
  }

  double intensity = 0;
  for (auto & pixel : image_data_)
  {
    intensity = static_cast<double>(pixel->integral) / max_i;
    image.setPixel(pixel->x_coord, pixel->y_coord, std::ceil(intensity * 255));

  }
  return image;
}

void XRFImage::ComputeXLength()
{
  // TODO error checking
  x_length_ = 0;
  for (uint i = 1; i <image_data_.size(); ++i)
  {
    // We start the loop from index 1, as index 0 should already have x_coord  = 0
    if (image_data_.at(i)->x_coord == 0)
    {
      x_length_ = i;
      break;
    }
  }
}

void XRFImage::ComputeYLength()
{
  // TODO error checking
  y_length_ = image_data_.back()->y_coord + 1;
}

int XRFImage::ComputeMaxIntegral()
{
  std::vector<int> integrals;
  for (auto & p: image_data_)
  {
    integrals.push_back(p->integral);
  }
  return *std::max_element(integrals.begin(), integrals.end());

}


// Starting contents of source file
void XRFImage::LoadDataFile(std::string filename)
{
  // Close previous file
  if (file_.is_open()) {
    file_.close();
  }

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

    roi_low = 0;
    roi_high = 16383;

    getline(file_, line);
    while (line.compare("}") != 0)
    {
      // Parse the header
      // Populate the scan parameters
      getline(file_, line);
    }


    // ImageData is a vector of unique pointers to a a Pixel structure
    // Clearing the vector has the effect of releasing ownership of the Pixels
    // Thus the old image in the heap is cleaned up.
    image_data_.clear();


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
    std::array<uint_least32_t, 0x4000> sum_spectrum;
    sum_spectrum.fill(0);
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
        sum_spectrum.at(channel) += count;
      }
      getline(file_, line);
    }
    image_data_.back()->integral = integral;

    sum_spectrum_.assign(sum_spectrum.begin(), sum_spectrum.end());
    valid_ = true;

    ComputeXLength();
    ComputeYLength();
    pixels_in_image_ = x_length_ * y_length_;
    emit UpdateProgressBar(0);
    //sum_spectrum_.shrink_to_fit();
  }

  else
  {
    err_msg_ = "The file cannot be opened.\n"
               "Check format and access privileges.";
    valid_ = false;
  }
}
