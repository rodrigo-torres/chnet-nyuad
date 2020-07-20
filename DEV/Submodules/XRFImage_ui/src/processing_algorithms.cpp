#include "MAXRF/image_display.h"

namespace maxrf {

/// \fn PaintImage
/// \brief Paints a processed 8-bit indexed image on the screen
///
/// Paints the image currently held in the private member variable \see
/// displayed_image_ .
///
void ImgLabel::PaintImage()
{
//  displayed_image_ =  images_.at(active_index_)->processed;
  QCursor cursor (QPixmap::fromImage(displayed_image_));
  cursor.setShape(Qt::PointingHandCursor);

  this->setPixmap(QPixmap::fromImage(displayed_image_));
  this->setCursor(cursor);
  this->resize(displayed_image_.size());

  map_opened_ = true;
}

/**
  *  @brief Handles resizing events and changes in the palette selection
  */
void ImgLabel::ProcessImage()
{
  auto & img = *images_.at(image_index_);
  //displayed_image_ = img.matrix;

  std::copy(img.histogram.begin(), img.histogram.end(),
            displayed_histo_.begin());

  if (img.flags & kPalette)
  {
    auto const & color_table = palettes_.find(selected_palette);
    displayed_image_.setColorTable(color_table->second);
    img.flags &= ~kPalette;
  }
  if (img.flags & kBrighten)
  {
//    AdjustBrightness();
  }
  if (img.flags & kEqualize)
  {
//    EqualizeHistogram();
  }
  if (img.flags & kStretch)
  {
    AdjustContrast();
  }
  if (img.flags & kResize)
  {
//    ResizeImage();
  }
  PaintImage();
  emit UpdateImageHistogram(displayed_histo_);
}

template<typename T>
static auto ShiftVectorRight(QVector<T> & vec,
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
static auto ShiftVectorLeft(QVector<T> & vec,
                             int shift, T fill_val = 0) -> void
{
  static_assert (std::is_integral<T>::value,
                 "Template parameter is not an integral value");
  // Shifting of the vector to the left
  std::move(vec.begin() + shift, vec.end(), vec.begin());
  // Assigning the out-of-range values to fill_val
  std::fill(vec.rbegin(), vec.rbegin() + shift, fill_val);
}
///
/// \brief Resize the displayed image
/// \note Nothing to see here, this method is locked and optimized
///
//void ImgLabel::ResizeImage()
//{
//  auto & img = *images_.at(image_index_);
//  QSize requested_size { img.dimensions.width() * img.pixel_replication,
//                          img.dimensions.height() * img.pixel_replication };

//  displayed_image_ =
//      displayed_image_.scaled(requested_size, Qt::KeepAspectRatio);
//}

///
/// \brief Adjust the brightness levels of the displayed image
/// \note Nothing to see here, this method is locked and optimized
///
//void ImgLabel::AdjustBrightness()
//{
//  auto & image = *images_.at(image_index_);

//  auto palette = palettes_.find(selected_palette)->second;
////  auto palette = displayed_image_.colorTable();
//  auto offset = std::abs(image.brightness);

//  using type = decltype (palette)::value_type;

//  if (image.brightness < 0)
//  {
//    ShiftVectorRight<type>(palette, offset, 0xFF000000);
//    displayed_image_.setColorTable(palette);

//    // We shift the image histogram to the left
//    ShiftVectorLeft<type>(displayed_histo_, offset, 0);
//    displayed_histo_.front() = static_cast<uint>(std::accumulate(image.histogram.begin(),
//                                        image.histogram.begin() + offset, 0));
//  }
//  else
//  {
//    ShiftVectorLeft<type>(palette, offset, 0xFFFFFFFF);
//    displayed_image_.setColorTable(palette);

//    // We shift the image histogram to the right
//    ShiftVectorRight<type>(displayed_histo_, offset, 0);
//    displayed_histo_.back() = static_cast<uint>(std::accumulate(image.histogram.rbegin(),
//                                        image.histogram.rbegin() + offset, 0));}
//}

///
/// \brief Equalize the Histogram
/// \note Nothing to see here, this method is locked and optimized
///
//auto ImgLabel::EqualizeHistogram() -> void
//{

//  // We get a reference to the image being displayed (and thus to its histogram)
//  auto & image = *images_.at(image_index_);

//  // Error checking the image isn't empty -> would lead to division by zero
//  if (QSize{image.dimensions.width(), image.dimensions.height()} == QSize{0, 0})
//  {
//    std::cout << "[!] Can't transform an empty image" << std::endl;
//    return;
//  }

//  // We set up some aliases for better readability
//  using container_type = decltype (displayed_histo_);
//  using type = container_type::value_type;

//  // Error-checking the value_type of the histogram is an integral type
//  static_assert (std::is_integral<type>::value,
//                 "The value_type of the histogram is not an integral type!" );

//  // We'll asign the elements of the equalized histogram in order, but it's not
//  // guaranteed will jump forward in the container.
//  // So we default-initialize its N elements
//  container_type equalized(displayed_histo_.size());

//  // We get a reference to the untransformed look-up table for the image
//  // No copying is involved
//  auto & look_up_table = palettes_.find(selected_palette)->second;

//  // Since we assign the elements of the new palette in order, we construct
//  // a default initialized container, and get a back insert iterator
//  container_type palette;
//  auto palette_inserter = std::back_inserter(palette);

//  // CDF factor is the equalization factor for the cumulative histogram
//  double cdf_factor = 255. / (image.dimensions.width() * image.dimensions.height());
//  // CDF entry is an entry in the cumulative histogram multiplied by cdf_factor
//  double cdf_entry{};

//  std::for_each(displayed_histo_.begin(), displayed_histo_.end(),
//                [&] (type & n) {
//  //std::for_each(image.nhisto.begin(), image.nhisto.end(),  [&] (type & n) {
//      // We construct the CDF as we go, we only use the latest entry at any time
//      // Notice we compute the cumulative histogram, and equalize in the same op
//      cdf_entry += n * cdf_factor;
//      // We reassing the counts on the equalized histogram based on the  CDF
//      equalized[static_cast<int>(cdf_entry)] += n;
//      // We transform the colors of the palette based on the CDF
//      *palette_inserter = look_up_table.at(static_cast<int>(cdf_entry));
//    });
//  equalized.swap(displayed_histo_);
////  equalized.swap(image.nhisto);

//  if (image.brightness > 0) {
//    ShiftVectorLeft<type>(palette, image.brightness, 0xFFFFFFFF);
//  }
//  else if (image.brightness < 0) {
//    ShiftVectorRight<type>(palette, std::abs(image.brightness), 0xFF000000);
//  }
//  displayed_image_.setColorTable(palette);
//}

///
/// \brief ImgLabel::AdjustContrast
///
void ImgLabel::AdjustContrast()
{
  auto & image = *images_.at(image_index_);

  // Find the first non-zero histogram element going from beginning to end
  auto it =  std::find_if(displayed_histo_.begin(), displayed_histo_.end(),
                           [] (uint & n) -> bool {return n != 0;});

  if (it == displayed_histo_.end())
  {
    // The histogram holds only zero values, contrast cannot be increases
    // Attempting to increase contrast would lead to division by zero
    return;
  }

  // Find the first non-zero histogram element going from end to beginning
  auto rit = std::find_if(displayed_histo_.rbegin(), displayed_histo_.rend(),
                          [] (uint & n) -> bool {return n != 0;});

  if (it == rit.base() - 1)
  {
    // The iterators are pointing to the same element
    // Increasing contrast would lead to division by zero
    return;
  }

  auto min = std::distance(displayed_histo_.begin(), it);
  auto max = std::distance(displayed_histo_.begin(), rit.base() - 1);
  auto factor = 255. / (max - min);

  if (factor == 1)
  {
    //  Update Status Bar
    return;
  }

  // We set up some aliases for better readability
  using container_type = decltype (displayed_histo_);
  using type = container_type::value_type;

  // Now we update the palette
  // We get the original palette to assign new color values to the current one
  auto & original = palettes_.find(selected_palette)->second;
  container_type temp(256, 0);
  container_type palette(256, 0);
  type new_i;

  for (long i = max; i >= min; --i)
  {
    new_i = (i - min) * factor;
    temp[new_i] = displayed_histo_.at(i);
    palette[i] = original.at(new_i);
  }
  temp.swap(displayed_histo_);

  // Fill missing values in palette
  for (auto it = palette.begin() + 1; it != palette.end(); ++it)
  {
    if (*it == 0) {
      *it = *(it - 1);
    }
  }

  //image.processed.setColorTable(QVector<uint>::fromStdVector(palette));
  displayed_image_.setColorTable(palette);
}


} // namespace maxrf
