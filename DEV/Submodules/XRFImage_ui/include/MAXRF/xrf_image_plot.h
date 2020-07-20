#ifndef XRF_IMAGE_PLOT_H
#define XRF_IMAGE_PLOT_H


#include <memory>
#include "qcustomplot.h"
#include "MAXRF/xrfimage.h"
#include "../../src/palettes.h"

// -------------------- USER CONFIGURATION -------------------- //
// These fields will eventually need to be populated from a configuration file
static QString data_directory = "/home/frao/Documents/Workspaces/MAXRF/Data";

namespace maxrf {

class PlotMediator;

class XRFImagePlot : public QCustomPlot
{
  Q_OBJECT
  using ColorTable = std::map<double, QColor>;

  friend PlotMediator;

public:
  explicit XRFImagePlot(QWidget * parent  = nullptr) : QCustomPlot{parent} {
    MakePalettes();
    MakeWidget();
  }
  void AddImageToBuffer();
  void PaintImage(EnergyFilter filter);
  void EqualizeHistogram(bool state);

  void SetEnergyFilter(EnergyFilter filter, int low, int high) {
    renderer_.SetIntegralLimits(filter, low, high);
  }

  auto PrepareColormapPlot(int const width, int const height) {
    axisRect()->setBackground(QPixmap{});

    for (auto & colormap : filtered_colormaps) {
      if (colormap->data()->keySize() != width ||
          colormap->data()->valueSize() != height) {
        colormap->data()->setSize(width, height);
        colormap->data()->setRange({0, width * 1.0}, {0, height * 1.0});
      }
    }

    xAxis->setRange(0, width);
    yAxis->setRange(0, height);
    yAxis->setScaleRatio(xAxis, static_cast<double>(height / width));
  }

  auto PlotColormapFromData(EnergyFilter filter, std::vector<uint32_t> const & packed_data) {
    auto data = reinterpret_cast<uint8_t const *>(packed_data.data());

    QCPColorMap * colormap;
    switch (filter)
    {
    case EnergyFilter::kUnfiltered :
      [[fallthrough]];
    case EnergyFilter::kFilterRange1 :
      colormap = filtered_colormaps.at(0);
      break;
    case EnergyFilter::kFilterRange2 :
      colormap = filtered_colormaps.at(1);
      break;
    case EnergyFilter::kFilterRange3 :
      colormap = filtered_colormaps.at(2);
      break;
    }

    // Range-check
    auto width  = colormap->data()->keySize();
    auto height = colormap->data()->valueSize();
    for (int j = 0; j < height; ++j) {
      for (int i = 0; i < width; ++i) {
        colormap->data()->setCell(i, j, *data);
        ++data;
      }
    }
    colormap->rescaleDataRange();
    colormap->layer()->setVisible(true);

    this->rescaleAxes();
    this->replot();
  }

  void overlay_test() {


//    renderer_.SetIntegralLimits(EnergyFilter::kFilterRange1, 175, 185);
//    auto image1 = renderer_.RenderQImage(EnergyFilter::kFilterRange1);
//    renderer_.SetIntegralLimits(EnergyFilter::kFilterRange2, 230, 250);
//    auto image2 = renderer_.RenderQImage(EnergyFilter::kFilterRange2);

//    QImage output{image1->matrix.width(), image1->matrix.height(), QImage::Format_RGB32};


//    auto copy1 = image1->matrix.bits();
//    auto copy2 = image2->matrix.bits();
//    auto bits = reinterpret_cast<QRgb *>(output.bits());
//    auto width = image1->matrix.width();
//    auto height = image1->matrix.height();
//    auto padding = image1->matrix.bytesPerLine() - width;
//    auto padding2 = output.bytesPerLine() / sizeof (QRgb);
//    padding2 = padding2 - output.width();

//    // These must be set first before set the individual cells data
//    color_map_filter1->data()->setSize(width, height);
//    color_map_filter1->data()->setRange(QCPRange{0, width * 1.0},
//                               QCPRange{0, height * 1.0});

//    color_map_filter2->data()->setSize(width, height);
//    color_map_filter2->data()->setRange(QCPRange{0, width * 1.0},
//                               QCPRange{0, height * 1.0});

//    QColor color1, color2, color3;
//    for (int j = 0; j < height; ++j) {
//      for (int i = 0; i < width; ++i) {
//        color_map_filter1->data()->setCell(i, j, *copy1);
//        color_map_filter2->data()->setCell(i, j, *copy2);
//        // Color 1 is saturated green
//        color1.setHsvF(120. / 360., 1., *copy1 / 255.);
//        // Color 2 is magenta, equal parts saturated red and blue
//        color2.setHsvF(300. / 360., 1., *copy2 / 255.);
//        // Adding the green and magenta is colorblind safe
//        color3.setRgb(color2.red(), color1.green(), color2.blue());
//        *bits = color3.rgb();

//        ++copy1;
//        ++copy2;
//        ++bits;
//        if (i == width - 1) {
//          copy1 += padding;
//          copy2 += padding;
//          bits += padding2;
//        }
//      }
//    }

//    image_axis_rect->setBackground(QPixmap::fromImage(output), true, Qt::KeepAspectRatio);

//    color_map_filter1->setGradient(palettes_.at(5));
//    color_map_filter2->setGradient(palettes_.at(6));

//    auto map_layer = layer("Colormap 1");
//    map_layer->setVisible(false);

//    this->rescaleAxes();
//    this->replot();
  }


private:
 void MakeWidget();
 void MakePalettes()
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
    palettes_.push_back(gradient);
    map = MakePalette(_inferno_data);
    gradient.setColorStops(map);
    palettes_.push_back(gradient);
    map = MakePalette(_viridis_data);
    gradient.setColorStops(map);
    palettes_.push_back(gradient);
    map = MakePalette(_magma_data);
    gradient.setColorStops(map);
    palettes_.push_back(gradient);


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
    palettes_.push_back(gradient);
    map = MakeMonochromaticPalette(green);
    gradient.setColorStops(map);
    palettes_.push_back(gradient);
    map = MakeMonochromaticPalette(magenta);
    gradient.setColorStops(map);
    palettes_.push_back(gradient);
    map = MakeMonochromaticPalette(red);
    gradient.setColorStops(map);
    palettes_.push_back(gradient);
    map = MakeMonochromaticPalette(blue);
    gradient.setColorStops(map);
    palettes_.push_back(gradient);

  }

protected:
// void resizeEvent(QResizeEvent *event) override {
////   this->setViewport(QRect {QPoint {0, 0}, event->size()});
////   this->repaint(QRect {QPoint {0, 0}, event->size()});
////   this->replot();

//   this->resize(event->size());
// }

  Renderer renderer_;
  std::array<QCPColorMap *, 3> filtered_colormaps;
  QCPColorMap * color_map_filter1;
  QCPColorMap * color_map_filter2;
  QCPColorMap * color_map_filter3;
  QVector<QCPColorGradient> palettes_;
  QCPAxisRect * image_axis_rect;
  QCPBars * histogram;

  std::unique_ptr<IndexedImage> image_state;


};

struct PlotMediatorState
{

};


class PlotMediator : public QWidget
{
  Q_OBJECT

public:
  explicit PlotMediator(QWidget * parent = nullptr) : QWidget{parent} {
    image_plot_ = new XRFImagePlot {this};
    renderer_   = new Renderer     {this};

    this->setMinimumSize(800, 600);
  }

  auto ConnectProgressBar(QProgressBar * progress_bar) {
    if (parentWidget()->isAncestorOf(progress_bar)) {
      QObject::connect(renderer_, &Renderer::UpdateProgressBar,
                       progress_bar, &QProgressBar::setValue);
    }
  }

  auto ConnectStatusBar(QStatusBar * status_bar) {
    Q_UNUSED(status_bar)
//    if (parentWidget()->isAncestorOf(status_bar)) {
//      QObject::connect(renderer_, &Renderer::UpdateStatusBar,
//                       status_bar, &QStatusBar::showMessage);
//    }
  }

  auto LoadDataFile() {
    bool success {false};

    QString filename = QFileDialog::getOpenFileName(
          nullptr, "Open XRF Image Data File", data_directory, "*.hyperc");

    if (filename.isEmpty()) {
      QMessageBox::warning(this, "Warning!",
                           "You have chosen an empty filename.\n");
      return success;
    }

    renderer_->LoadHypercube(filename.toStdString());
    if (renderer_->is_valid()) {
      AddImageToInternalBuffer(EnergyFilter::kUnfiltered);
      success = true;
    }
    else {
      QMessageBox::critical(this, "Error!",
                            QString::fromStdString(renderer_->err_mesg()));
    }
    return success;
  }

  void AddImageToInternalBuffer(EnergyFilter filter) {
    auto renderer_state = renderer_->RenderQImage(filter);
    auto const & width  = renderer_state->dimensions.width();
    auto const & height = renderer_state->dimensions.height();

    image_plot_->PrepareColormapPlot(width, height);
    image_plot_->PlotColormapFromData(filter, renderer_state->indexed_data);
  }

protected:
  auto SetColormapData() {

  }

  void resizeEvent(QResizeEvent *event) override {
    constexpr double aspect_ratio = 1.0;
    image_plot_->resize(event->size());
    image_plot_->yAxis->setScaleRatio(image_plot_->xAxis, aspect_ratio);
    image_plot_->replot();
  }

private:
  XRFImagePlot * image_plot_;
  Renderer * renderer_;


};


} // namespace maxrf

#endif // XRF_IMAGE_PLOT_H
