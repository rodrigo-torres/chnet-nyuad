#include "include/MAXRF/xrf_image_plot.h"

namespace maxrf {

void XRFImagePlot::AddImageToBuffer()
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


  renderer_.LoadHypercube(filename.toStdString());
  if (renderer_.is_valid()) {
    PaintImage(EnergyFilter::kUnfiltered);
  }
  else
  {
    QMessageBox::critical(this, "Error!",
                          QString::fromStdString(renderer_.err_mesg()));
//      map_opened_ = false;
  }
}

void XRFImagePlot::PaintImage(EnergyFilter filter)
{
//  auto data_ptr = reinterpret_cast<uint8_t *>(image_state->indexed_data.data());
//  for (int j = 0; j < height; ++j) {
//    for (int i = 0; i < width; ++i) {
//      color_map_filter1->data()->setCell(i, j, *data_ptr);
//      ++data_ptr;
//    }
//  }
  color_map_filter1->rescaleDataRange();
  auto map_layer = layer("Colormap 1");
  map_layer->setVisible(true);

  this->rescaleAxes();
  this->replot();
}

auto XRFImagePlot::EqualizeHistogram(bool state) -> void
{
  // Error checking the image isn't empty -> would lead to division by zero
  if (QSize{image_state->dimensions.width(), image_state->dimensions.height()} == QSize{0, 0})
  {
    std::cout << "[!] Can't transform an empty image" << std::endl;
    return;
  }

  // We set up some aliases for better readability
  using container_type = decltype (image_state->histogram);
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
  auto const & lut = palettes_.at(4).colorStops();
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
  double cdf_factor = 255. / (image_state->dimensions.width() * image_state->dimensions.height());
  // CDF entry is an entry in the cumulative histogram multiplied by cdf_factor
  double cdf_entry {0};

  std::for_each(image_state->histogram.begin(), image_state->histogram.end(),
                [&] (type & n) {
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
  color_map_filter1->setGradient(gradient);
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

void XRFImagePlot::MakeWidget()
{
//    plotLayout()->clear();
  axisRect()->setupFullAxesBox(true);
  setInteractions(QCP::iSelectPlottables | QCP::iRangeZoom | QCP::iMultiSelect | QCP::iRangeDrag); // this will also allow rescaling the color scale by dragging/zooming
  setMultiSelectModifier(Qt::ControlModifier);
  //setSelectionRectMode(QCP::srmSelect);

  image_axis_rect = axisRect();

  QImage image {":/images/TT_CHNet_res2.png"};
  image_axis_rect->setBackground(QPixmap::fromImage(image), true,
                                 Qt::KeepAspectRatio);

//    image_axis_rect = new QCPAxisRect{this, false};

//    xAxis->setVisible(false);

//    auto x_axis = image_axis_rect->addAxis(QCPAxis::atTop);
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


  image_axis_rect->setRangeZoom(Qt::Horizontal | Qt::Vertical);
  image_axis_rect->setRangeZoomAxes(image_axis_rect->axis(QCPAxis::atTop),
                                    image_axis_rect->axis(QCPAxis::atLeft));
  image_axis_rect->setRangeZoomFactor(0.5);


  // Set up the color scales
  QCPColorScale * color_scale_filter1 = new QCPColorScale(this);
  color_scale_filter1->setType(QCPAxis::atRight);
  color_scale_filter1->setDataRange(QCPRange{0, 255});

  QCPColorScale *color_scale_filter2 = new QCPColorScale(this);
  color_scale_filter2->setType(QCPAxis::atRight);
  color_scale_filter2->setDataRange(QCPRange{0, 255});

  QCPColorScale * color_scale_filter3 = new QCPColorScale(this);
  color_scale_filter3->setType(QCPAxis::atRight);
  color_scale_filter3->setDataRange(QCPRange{0, 255});
  color_scale_filter3->setLabel("Intensities");


  auto axes = new QCPAxisRect(this, true);
  axes->axis(QCPAxis::atBottom)->setRange(0, 255);
  axes->axis(QCPAxis::atLeft)->setRange(0, 255);

  histogram = new QCPBars(axes->axis(QCPAxis::atBottom), axes->axis(QCPAxis::atLeft));
  histogram->setWidth(1);
  histogram->setPen(Qt::NoPen);
  histogram->setBrush(QColor("Orange"));

  QVector<double> keys(256), value(256, 0.);
  for (int i = 0; i < 256; ++i) {
    keys[i] = i;
  }
  histogram->setData(keys, value, true);


  // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
  QCPMarginGroup *marginGroup = new QCPMarginGroup(this);
  image_axis_rect->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
  color_scale_filter1->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
  color_scale_filter2->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
  color_scale_filter3->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

  QCPMarginGroup *histo_and_image_margins = new QCPMarginGroup{this};
  image_axis_rect->setMarginGroup(QCP::msLeft | QCP::msRight, histo_and_image_margins);
  axes->setMarginGroup(QCP::msLeft | QCP::msRight, histo_and_image_margins);

  this->plotLayout()->addElement(0, 1, color_scale_filter1);
  this->plotLayout()->addElement(0, 2, color_scale_filter2);
  this->plotLayout()->addElement(0, 3, color_scale_filter3);
  this->plotLayout()->addElement(0, 4, axes);

  // Configure the 2nd Energy Filter overlay
  auto alayer = layer("main");
  this->addLayer("Colormap 1", alayer, limAbove);
  alayer = layer("Colormap 1");
  alayer->setVisible(false);
  this->addLayer("Colormap 2", alayer, limAbove);
  alayer = layer("Colormap 2");
  alayer->setVisible(false);
  this->addLayer("Colormap 3", alayer, limAbove);
  alayer = layer("Colormap 3");
  alayer->setVisible(false);

  // Set up the XRF image plot
  color_map_filter1 = new QCPColorMap(x_axis, y_axis);
  color_map_filter1->setSelectable(QCP::stDataRange);
  color_map_filter1->data()->setSize(450, 450);
  color_map_filter1->data()->setRange(x_axis->range(), y_axis->range());
  color_map_filter1->setColorScale(color_scale_filter1); // associate the color map with the color scale
  color_map_filter1->setGradient(palettes_.at(4));
  color_map_filter1->setLayer("Colormap 1");

  color_map_filter2 = new QCPColorMap(x_axis, y_axis);
  color_map_filter2->setSelectable(QCP::stDataRange);
  color_map_filter2->data()->setSize(450, 450);
  color_map_filter2->data()->setRange(x_axis->range(), y_axis->range());
  color_map_filter2->setColorScale(color_scale_filter2); // associate the color map with the color scale
  color_map_filter2->setGradient(palettes_.at(5));
  color_map_filter2->setLayer("Colormap 2");

  color_map_filter3 = new QCPColorMap(x_axis, y_axis);
  color_map_filter3->setSelectable(QCP::stDataRange);
  color_map_filter3->data()->setSize(450, 450);
  color_map_filter3->data()->setRange(x_axis->range(), y_axis->range());
  color_map_filter3->setColorScale(color_scale_filter3); // associate the color map with the color scale
  color_map_filter3->setGradient(palettes_.at(5));
  color_map_filter3->setLayer("Colormap 3");

  filtered_colormaps.at(0) = color_map_filter1;
  filtered_colormaps.at(1) = color_map_filter2;
  filtered_colormaps.at(2) = color_map_filter3;
  // rescale the key (x) and value (y) axes so the whole color map is visible:
  this->rescaleAxes();
  this->replot();
}

} // namespace maxrf
