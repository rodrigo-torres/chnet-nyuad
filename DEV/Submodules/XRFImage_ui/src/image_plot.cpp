/** MAXRF_IMAGE: Render and process MAXRF images
 *  Copyright (C) 2020 Rodrigo Torres and contributors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "palettes.h"
#include "widgets.h"

namespace maxrf {


void XRFImagePlot::PlotRenderData(std::shared_ptr<Render const> render) {
  // Update the matrix histogram plot
  double gray_level {0.};
  QVector<double> keys {}, values {};
  for (auto gray_intensity : render->matrix_histogram) {
    keys.push_back(gray_level);
    values.push_back(static_cast<double>(gray_intensity));
    gray_level += 1.0;
  }
  histogram->setData(keys, values, true);
  histogram->rescaleAxes();

  // Resize the Colormap plot
  axisRect()->setBackground(QPixmap {});

  colormap_->data()->setSize(render->dimensions.first,
                             render->dimensions.second);
  colormap_->data()->setRange({0, render->dimensions.first * 1.0},
                              {0, render->dimensions.second * 1.0});
  yAxis->setScaleRatio(xAxis, 1.0);
  colormap_->rescaleAxes();

  std::byte const * data = render->indexed_data.data();

  // Plot the data;
  auto width  = colormap_->data()->keySize();
  auto height = colormap_->data()->valueSize();
  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      colormap_->data()->setCell(i, j, std::to_integer<long>(*data));
      ++data;
    }
  }
  colormap_->rescaleDataRange();
  this->replot();

  image_state = render;
}


void XRFImagePlot::Replot() {
  colormap_->rescaleDataRange();

  this->rescaleAxes();
  this->replot();
}

QImage XRFImagePlot::ConvertColormap() {
  auto const width  = colormap_->data()->keySize();
  auto const height = colormap_->data()->valueSize();

  QImage image {width, height, QImage::Format_Indexed8};

  auto padding  = image.bytesPerLine() - width;
  auto img_data = image.bits();

  // Copy data from colormap to the QImage buffer
  for (auto j = 0; j < height; ++j) {
    for (auto i = 0; i < width; ++i) {
      *img_data = static_cast<unsigned char>(colormap_->data()->cell(i, j));
      ++img_data;
      if (i == width - 1) {
        img_data += padding;
      }
    }
  }

  // And we copy the palette too
  QVector<QRgb> palette {};
  auto it = std::back_inserter(palette);

  for (auto & color : colormap_->gradient().colorStops()) {
    it = color.rgb();
  }
  image.setColorTable(palette);

  return image;
}

void XRFImagePlot::EqualizeHistogram()
{
  // Error checking the image isn't empty -> would lead to division by zero
  if (QSize{image_state->dimensions.first, image_state->dimensions.second} == QSize{0, 0})
  {
    std::cout << "[!] Can't transform an empty image" << std::endl;
    return;
  }

  // We set up some aliases for better readability
  using container_type = decltype (image_state->matrix_histogram);
  using type = container_type::value_type;

  // Error-checking the value_type of the histogram is an integral type
  static_assert (std::is_integral<type>::value,
                 "The value_type of the histogram is not an integral type!" );

  // We'll asign the elements of the equalized histogram in order, but it's not
  // guaranteed will jump forward in the container.
  // So we default-initialize its N elements
  container_type equalized;
  equalized.fill(0);

  // We get a reference to the untransformed look-up table for the image
  // No copying is involved
  auto const & lut = colormap_->gradient().colorStops();
  std::vector<QColor> colors;
  std::vector<double> stops;
  for (auto it = lut.constKeyValueBegin(); it != lut.constKeyValueEnd(); ++it) {
    stops.push_back((*it).first);
    colors.push_back((*it).second);
  }

  // Since we assign the elements of the new palette in order, we construct
  // a default initialized container, and get a back insert iterator
  QMap<double, QColor> palette;
  std::vector<QColor> new_colors;
  //auto palette_inserter = std::back_inserter(palette);

  // CDF factor is the equalization factor for the cumulative histogram
  double cdf_factor = 255. / (image_state->dimensions.first * image_state->dimensions.second);
  // CDF entry is an entry in the cumulative histogram multiplied by cdf_factor
  double cdf_entry {0};

  std::for_each(image_state->matrix_histogram.begin(), image_state->matrix_histogram.end(),
                [&] (type const & n) {
  //std::for_each(image.nhisto.begin(), image.nhisto.end(),  [&] (type & n) {
      // We construct the CDF as we go, we only use the latest entry at any time
      // Notice we compute the cumulative histogram, and equalize in the same op
      cdf_entry += n * cdf_factor;
      // We reassing the counts on the equalized histogram based on the  CDF
      equalized[static_cast<size_t>(cdf_entry)] += n;
      // We transform the colors of the palette based on the CDF
      new_colors.push_back(colors.at(static_cast<size_t>(cdf_entry)));
    });
  //equalized.swap(image_state->histogram);

  auto it = new_colors.begin();
  for (auto & levels : stops) {
    palette.insert(levels, *it);
    ++it;
  }

  QCPColorGradient gradient;
  gradient.setPeriodic(false);
  gradient.setLevelCount(256);
  gradient.setColorStops(palette);
  colormap_->setGradient(gradient);
//  equalized.swap(image.nhisto);

//  if (image.brightness > 0) {
//    ShiftVectorLeft<type>(palette, image.brightness, 0xFFFFFFFF);
//  }
//  else if (image.brightness < 0) {
//    ShiftVectorRight<type>(palette, std::abs(image.brightness), 0xFF000000);
//  }
  auto nit = equalized.begin();
  auto histogram_data = histogram->data();
  for (auto & data : *histogram_data) {
    data.value = *nit;
    ++nit;
  }

  auto max = *std::max_element(equalized.begin(), equalized.end());
  histogram->valueAxis()->setRange(0, max);

  rescaleAxes();
  replot();

}

/////
///// \brief ImgLabel::AdjustContrast
/////
//void ImgLabel::AdjustContrast()
//{
//  auto & image = *images_.at(image_index_);

//  // Find the first non-zero histogram element going from beginning to end
//  auto it =  std::find_if(displayed_histo_.begin(), displayed_histo_.end(),
//                           [] (uint & n) -> bool {return n != 0;});

//  if (it == displayed_histo_.end())
//  {
//    // The histogram holds only zero values, contrast cannot be increases
//    // Attempting to increase contrast would lead to division by zero
//    return;
//  }

//  // Find the first non-zero histogram element going from end to beginning
//  auto rit = std::find_if(displayed_histo_.rbegin(), displayed_histo_.rend(),
//                          [] (uint & n) -> bool {return n != 0;});

//  if (it == rit.base() - 1)
//  {
//    // The iterators are pointing to the same element
//    // Increasing contrast would lead to division by zero
//    return;
//  }

//  auto min = std::distance(displayed_histo_.begin(), it);
//  auto max = std::distance(displayed_histo_.begin(), rit.base() - 1);
//  auto factor = 255. / (max - min);

//  if (factor == 1)
//  {
//    //  Update Status Bar
//    return;
//  }

//  // We set up some aliases for better readability
//  using container_type = decltype (displayed_histo_);
//  using type = container_type::value_type;

//  // Now we update the palette
//  // We get the original palette to assign new color values to the current one
//  auto & original = palettes_.find(selected_palette)->second;
//  container_type temp(256, 0);
//  container_type palette(256, 0);
//  type new_i;

//  for (long i = max; i >= min; --i)
//  {
//    new_i = (i - min) * factor;
//    temp[new_i] = displayed_histo_.at(i);
//    palette[i] = original.at(new_i);
//  }
//  temp.swap(displayed_histo_);

//  // Fill missing values in palette
//  for (auto it = palette.begin() + 1; it != palette.end(); ++it)
//  {
//    if (*it == 0) {
//      *it = *(it - 1);
//    }
//  }

//  //image.processed.setColorTable(QVector<uint>::fromStdVector(palette));
//  displayed_image_.setColorTable(palette);
//}

//void ImgLabel::mouseReleaseEvent(QMouseEvent *event) {
  // Ask the renderer to compute ROI spectrum form 4 corners
  // renderer_.ComputeROISpectrum(std::move(pixels_selected));

  // Or if all 4 corners are the same, move to that position

//    int * spectrum_display_ptr = nullptr;
//    auto spectrum = renderer_.roi_spectrum();

//    switch (shared_memory_cmd.at(100))
//    {
//    case 0:
//      spectrum_display_ptr = &shared_memory.at(100);
//      break;
//    case 1:
//      spectrum_display_ptr = &shared_memory.at(20000);
//      break;
//    case 2:
//      spectrum_display_ptr = &shared_memory.at(40000);
//      break;
//    default:
//      // TODO warning here
//      break;
//    }

//    int i = 0;
//    for (auto val : spectrum)
//    {
//      spectrum_display_ptr[i] = val;
//      ++i;
//    }

//    shared_memory.at(99)= 1;
//  }
//}

void XRFImagePlot::SelectionRectEvent(QRect const & rect, QMouseEvent * event) {
  Q_UNUSED(event)

  // Construct the corners of the rectangle
  std::array<QPointF, 4> corners {};
  corners[0] = QPointF { rect.topLeft() };
  corners[1] = QPointF { rect.topRight() };
  corners[2] = QPointF { rect.bottomLeft() };
  corners[3] = QPointF { rect.bottomRight() };

  std::array<QPoint, 4> pixel_coords{};
  auto it = pixel_coords.begin();

  bool get_roi_spectrum {true};
  int ikey, ival;
  double key, val;
  for (auto & corner : corners) {
    // Check the corners are in the QCPColorMap
    if (colormap_->selectTest(corner, false, nullptr) > 0) {
      // The GUI pixel is inside the QCPColorMap
      colormap_->pixelsToCoords(corner, key, val);
      colormap_->data()->coordToCell(key, val, &ikey, &ival);
      *it = {ikey, ival};
      qDebug() << "Key found at: " << ikey;
      qDebug() << "Value found at: " << ival;
    }
    else {
      // One point was not found inside the map
      get_roi_spectrum = false;
    }
    ++it;
  }

  if (get_roi_spectrum) {
    // Pass the pixel coordinates to the renderer to construct the spectrum
    mediator_->PublishROISpectrum(pixel_coords);
  }

}

XRFImagePlot::XRFImagePlot(PlotMediator * mediator, QWidget * parent)
  : QCustomPlot {parent} {
  mediator_ = mediator;

  MakePalettes();

  this->setInteractions(QCP::iSelectPlottables | QCP::iMultiSelect |
                        QCP::iRangeZoom | QCP::iRangeDrag);
  this->setSelectionRectMode(QCP::srmNone);

  auto colormap_axis = axisRect();
  colormap_axis->setupFullAxesBox(true);

  auto & x_axis = xAxis2;
  x_axis->setTickLabels(true);
  xAxis->setTickLabels(false);
  xAxis->setRange(0., 450.);

  auto & y_axis = yAxis;
//    auto y_axis = image_axis_rect->addAxis(QCPAxis::atLeft);
  yAxis->setRangeReversed(true);
  y_axis->setRangeReversed(true);
  yAxis->setRange(0., 450.);
  y_axis->setScaleRatio(x_axis, 1.0);


  colormap_axis->setRangeZoom(Qt::Horizontal | Qt::Vertical);
  colormap_axis->setRangeZoomAxes(colormap_axis->axis(QCPAxis::atTop),
                                    colormap_axis->axis(QCPAxis::atLeft));
  colormap_axis->setRangeZoomFactor(0.5);

  auto sublayout = new QCPLayoutGrid {};
  this->plotLayout()->addElement(0, 1, sublayout);

  // Set up the color scales
  QCPColorScale * color_scale_filter1 = new QCPColorScale(this);
  color_scale_filter1->setType(QCPAxis::atBottom);
  color_scale_filter1->setDataRange(QCPRange{0, 255});


  auto axes = new QCPAxisRect(this, true);
  axes->axis(QCPAxis::atBottom)->setRange(0, 255);
  axes->axis(QCPAxis::atLeft)->setRange(0, 255);

  histogram = new QCPBars(axes->axis(QCPAxis::atBottom), axes->axis(QCPAxis::atLeft));
  histogram->setWidth(1);
  histogram->setPen(Qt::NoPen);
  histogram->setBrush(QColor("Orange"));
  histogram->setSelectable(QCP::stDataRange);

  QVector<double> keys(256), value(256, 0.);
  for (int i = 0; i < 256; ++i) {
    keys[i] = i;
    value[i] = i % 2 ? 127: 255;
  }
  histogram->setData(keys, value, true);


  // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
  QCPMarginGroup *marginGroup = new QCPMarginGroup(this);
  colormap_axis->setMarginGroup(QCP::msBottom, marginGroup);
  color_scale_filter1->setMarginGroup(QCP::msBottom, marginGroup);
  color_scale_filter1->setGradient(palettes_.begin()->second);


  QCPMarginGroup *histo_and_image_margins = new QCPMarginGroup{this};
  colormap_axis->setMarginGroup(QCP::msTop, histo_and_image_margins);
  axes->setMarginGroup(QCP::msTop, histo_and_image_margins);


  auto margins_3 = new QCPMarginGroup {this};
  axes->setMarginGroup(QCP::msLeft | QCP::msRight, margins_3);
  color_scale_filter1->setMarginGroup(QCP::msLeft | QCP::msRight, margins_3);

  sublayout->addElement(0, 0, axes);
  sublayout->addElement(1, 0, color_scale_filter1);


  // Set up the XRF image plot
  colormap_ = new QCPColorMap { x_axis, y_axis };
  colormap_->setColorScale(color_scale_filter1);

  QImage logo {":/images/TT_CHNet_res2.png"};
  colormap_axis->setBackground(QPixmap::fromImage(logo), true, Qt::KeepAspectRatio);

  auto const size = logo.size();
  colormap_->data()->setSize(size.width(), size.height());
  colormap_->data()->setRange({ 0., static_cast<double>(size.width())},
  {0., static_cast<double>(size.height())});


  colormap_->data()->fillAlpha(0);

  // rescale the key (x) and value (y) axes so the whole color map is visible:
  this->rescaleAxes();
  this->replot();


  connect(this->selectionRect(), &QCPSelectionRect::accepted,
          this, &XRFImagePlot::SelectionRectEvent);
}

void XRFImagePlot::MakePalettes()
{
  using namespace maxrf::palettes;

  auto MakePalette = [](PaletteData const & _palette) -> QMap<double, QColor>
  {
    QColor color;
    QMap<double, QColor> map;
    constexpr double color_stop_step { 1 / 255. };
    double color_stop {0};

    for (auto & components : _palette) {
      color.setRgbF(components[0], components[1], components[2]);
      map.insert(color_stop, color);
      color_stop += color_stop_step;
    }
    return map;
  };


   QCPColorGradient gradient;
   gradient.setLevelCount(256);
   gradient.setPeriodic(false);

   auto map = MakePalette(_plasma_data);
   gradient.setColorStops(map);
   palettes_.insert( {"Plasma (CVD friendly)", gradient} );
   map = MakePalette(_inferno_data);
   gradient.setColorStops(map);
   palettes_.insert( {"Inferno (CVD friendly)", gradient} );
   map = MakePalette(_viridis_data);
   gradient.setColorStops(map);
   palettes_.insert( {"Viridis (CVD friendly)", gradient} );
   map = MakePalette(_magma_data);
   gradient.setColorStops(map);
   palettes_.insert( {"Magma (CVD friendly)", gradient} );


   // Monochromatic scales
   QColor magenta;
   magenta.setHsvF(300. / 360., 1., 1.);

   QColor green;
   green.setHsvF(120 / 360., 1., 1.);

   QColor white;
   white.setHsvF(0., 0., 1.);

   QColor red;
   red.setHsvF(0., 1., 1.);

   QColor blue;
   blue.setHsvF(240. / 360., 1., 1.);


   auto MakeMonochromaticPalette = [](QColor base_color) -> QMap<double, QColor>
   {
     QMap<double, QColor> map;
     QColor color;
     auto const hue = base_color.hueF();
     auto const saturation = base_color.saturationF();
     auto constexpr value_step = 1.0 / 255;

     for (double value = 1.0; !std::signbit(value); value -= value_step) {
       color.setHslF(hue, saturation, value);
       map.insert(value, color.toRgb());
     }
     return map;
   };

   map = MakeMonochromaticPalette(white);
   gradient.setColorStops(map);
   palettes_.insert( {"Grayscale", gradient} );
   map = MakeMonochromaticPalette(red);
   gradient.setColorStops(map);
   palettes_.insert( { "Monochrome Red", gradient} );
   map = MakeMonochromaticPalette(green);
   gradient.setColorStops(map);
   palettes_.insert( { "Monochrome Green", gradient} );
   map = MakeMonochromaticPalette(blue);
   gradient.setColorStops(map);
   palettes_.insert( { "Monochrome Blue", gradient} );
   map = MakeMonochromaticPalette(magenta);
   gradient.setColorStops(map);
   palettes_.insert( { "Monochrome Magenta", gradient} );

 }


} // namespace maxrf
