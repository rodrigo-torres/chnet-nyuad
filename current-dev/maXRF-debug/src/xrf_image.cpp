#include "h/xrfimage.h"

XRFImage::XRFImage()
{

}
XRFImage::~XRFImage()
{
  if (file_.is_open())
  {
    file_.close();
  }
}

bool XRFImage::is_valid()
{
  return  valid_;
}

std::string XRFImage::err_msg()
{
  return err_msg_;
}

void XRFImage::UpdateROIIntegrals()
{
  if (roi_low != shared_memory5[100] || roi_high != shared_memory5[101])
  {
    roi_low = shared_memory5[100];
    roi_high = shared_memory5[101];

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
        if (file_.eof())
        {
          printf("hey");
        }
      }
    }

  }
  else
  {
    return;
  }
}

QImage XRFImage::ConstructQImage(QString mode, int Pixeldim)
{
  auto x_len = ComputeXLength();
  auto y_len = ComputeYLength();

  UpdateROIIntegrals();
  auto max_i = ComputeMaxIntegral();
  auto map_size = x_len * y_len;
  QImage image(x_len * Pixeldim, y_len * Pixeldim, QImage::Format_RGB32);

  QColor myColor;
  for (long i = 0; i < map_size; ++i)
  {
    double intensity = 0;
    if (max_i)
    {
      intensity = static_cast<double>(image_data_[i]->integral) / max_i;
      //intensity *= scale;
    }


    if (mode == "Colors")
    {
      int temp = int(intensity * 255);

      int color_r = viridis[temp][0];
      int color_g = viridis[temp][1];
      int color_b = viridis[temp][2];
      myColor.setRgb(color_r, color_g, color_b, 255);
    }

    else if ( mode == "Gray Scale" ) {
      intensity *= 255;
      myColor.setRgb(int(intensity),int(intensity),int(intensity),255);
    }

    int x_corner = image_data_[i]->x_coord * Pixeldim;
    int y_corner = image_data_[i]->y_coord * Pixeldim;

    for (int j = 0; j < Pixeldim; j++) {
      for (int i = 0; i < Pixeldim; i++){
        image.setPixel(x_corner + i, y_corner + j, myColor.rgb());
      }
    }
  }
  return image;
}

long XRFImage::ComputeXLength()
{
  // TODO error checking
  for (uint i = 1; i <image_data_.size(); ++i)
  {
    // We start the loop from index 1, as index 0 should already have x_coord  = 0
    if (image_data_.at(i)->x_coord == 0)
    {
      return i; // implicit integer promotion
    }
  }
  return 0;
}

long XRFImage::ComputeYLength()
{
  // TODO error checking
  return image_data_.back()->y_coord + 1;
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
    //sum_spectrum_.shrink_to_fit();
  }

  else
  {
    err_msg_ = "The file cannot be opened.\n"
               "Check format and access priviledges.";
    valid_ = false;
  }
}
