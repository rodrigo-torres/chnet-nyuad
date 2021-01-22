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

#ifndef XRF_IMAGE_WIDGET_H
#define XRF_IMAGE_WIDGET_H

#include <QtWidgets>

#include "renderer.h"
#include "MAXRF/ipc_methods.h"
#include "QCP/qcustomplot.h"


// -------------------- USER CONFIGURATION -------------------- //
// These fields will eventually need to be populated from a configuration file
static QString data_directory = "/home/frao/Documents/Workspaces/MAXRF/Data";

extern template class maxrf::ipc::shmarray<int>;

namespace maxrf {

// Add image to buffer
// Add image with filter to buffer
// Remove image from buffer
// Change image from buffer

// Stretching of histogram

// Update image histogram plot
// Update status bar
// Update progress bar

// Move motors method

// Replot method
// Process image

// Export to PNG methods

// File conversion menu


class PlotMediator;

class XRFImagePlot : public QCustomPlot
{
  Q_OBJECT
  using Palette     = QCPColorGradient;
  using PaletteList = std::map<std::string, Palette>;

  friend PlotMediator;

public:
  ///
  /// \brief XRFImagePlot
  /// \param parent
  ///
  explicit XRFImagePlot(PlotMediator *, QWidget * parent  = nullptr);

  ///
  /// \brief PlotRenderData
  /// \param render
  ///
  void PlotRenderData(std::shared_ptr<Render const> render);

  ///
  /// \brief Replot
  ///
  void Replot();

  ///
  /// \brief EqualizeHistogram
  ///
  void EqualizeHistogram();

  ///
  /// \brief ConvertColormap
  /// \return
  ///
  QImage ConvertColormap();



protected:
  void MakePalettes();
  void SelectionRectEvent(QRect const & rect, QMouseEvent * event);

  QCPColorMap * colormap_;
  QCPBars * histogram;
  PaletteList palettes_;

  PlotMediator * mediator_ {nullptr};
  std::shared_ptr<Render const> image_state;
};

class RenderDialog : public QDialog
{
protected:
  void closeEvent(QCloseEvent *) override {
    result_.first = chan_low->value();
    result_.second = chan_high->value();
  }

public:
  explicit RenderDialog(QWidget * parent = nullptr);

  std::pair<int, int> result_;
  QSpinBox * chan_low {nullptr};
  QSpinBox * chan_high{nullptr};

};

///
/// \brief The RendersTable class is a simple widget incorporating a table
/// with the history of rendered images by the program. The program adds
/// new renders to the history as needed
///
class RendersTable : public QWidget
{
  Q_OBJECT

public:
  using TableEntry    = std::shared_ptr<Render const>;
  using RenderHistory = std::vector<TableEntry>;

  explicit RendersTable(QWidget * parent = nullptr);

  void AddTableEntry(TableEntry);
  void RemoveTableEntry();

signals:
  void SelectedRenderChanged(TableEntry);

private:
  RenderHistory entries_ {};
};


class XRFImageWidget : public QMainWindow
{
  Q_OBJECT // A Qt macro enabling Qt's C++ extensions (like signals and slots)

public:
 explicit XRFImageWidget(PlotMediator * mediator_, QWidget * parent = nullptr);

private:
  void CreateActions();
  void NewRenderDialog();

private:
  PlotMediator * mediator_ {nullptr};
};

enum class SelectionMode : char {
  kNone,
  kZoom,
  kROISpectrum,
  kSelect
};



class PlotMediator : public QObject
{
  Q_OBJECT


public:
  using MementoStack  = std::vector<Renderer::Memento>;

  ///
  /// \brief PlotMediator
  ///
  PlotMediator();

  ///
  /// \brief LoadDataFile
  ///
  void LoadDataFile();

  ///
  /// \brief ExportImageToPNG
  ///
  void ExportImageToPNG();

  ///
  /// \brief RenderAndPaintImage
  ///
  void RenderAndPaintImage();

  ///
  /// \brief ChangeLimits
  /// \param limits
  ///
  void ChangeLimits(std::pair<int, int> limits) {
    renderer_->SetIntegralLimits(limits.first, limits.second);
  }

  ///
  /// \brief ChangePalette
  ///
  void ChangePalette();

  ///
  /// \brief PublishROISpectrum
  /// \param corners
  ///
  void PublishROISpectrum(std::array<QPoint, 4> corners);

  ///
  /// \brief ChangeSelectionMode
  /// \param mode
  ///
  void ChangeSelectionMode(SelectionMode mode) {
    switch (mode) {
    case SelectionMode::kNone :
      image_plot_->setSelectionRectMode(QCP::srmNone);
      break;
    case SelectionMode::kZoom :
      image_plot_->setSelectionRectMode(QCP::srmZoom);
      break;
    case SelectionMode::kSelect :
      image_plot_->setSelectionRectMode(QCP::srmSelect);
      break;
    case SelectionMode::kROISpectrum :
      image_plot_->setSelectionRectMode(QCP::srmCustom);
      break;
    }

  }

  void ChangeDisplayedRender(std::shared_ptr<Render const> render) {
    image_plot_->PlotRenderData(render);
  }

  ///
  /// \brief ChangeScale
  ///
  void ChangeScale() {
    auto scale = image_plot_->colormap_->dataScaleType();
    if (scale == QCPAxis::stLinear) {
      image_plot_->colormap_->setDataScaleType(QCPAxis::stLogarithmic);
      image_plot_->colormap_->colorScale()->axis()->ticker()->setTickStepStrategy(QCPAxisTicker::tssReadability);
      image_plot_->colormap_->rescaleDataRange();
    }
    else if (scale == QCPAxis::stLogarithmic) {
      image_plot_->colormap_->setDataScaleType(QCPAxis::stLinear);
      image_plot_->colormap_->rescaleDataRange();
    }
  }


private:
  maxrf::ipc::shmarray<int> spectrum_broadcast_;
  MementoStack mementos_;

  XRFImageWidget * window;
  XRFImagePlot * image_plot_;
  Renderer * renderer_;
  RendersTable * table;


};



} // namespace xrfimage

#endif // XRFIMAGEWIDGET_H
