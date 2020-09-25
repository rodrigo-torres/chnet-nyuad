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

#include "widgets.h"

template class maxrf::ipc::shmarray<int>;

namespace maxrf {

PlotMediator::PlotMediator() : QObject{nullptr} {
  spectrum_broadcast_.initialize(maxrf::ipc::Segments::kSHMSpectra);

  // Construct the widget
  window = new XRFImageWidget {this};

  // STATUS BAR
  auto status_bar = window->statusBar();

  auto progress_bar = new QProgressBar;
  progress_bar->setRange(0, 100);
  progress_bar->setValue(0);

  status_bar->addPermanentWidget(progress_bar);

  table = new RendersTable {};
  image_plot_ = new XRFImagePlot {this};
  image_plot_->setMinimumSize(600, 300);

  auto sublayout = new QVBoxLayout {};
  sublayout->addWidget(image_plot_, 1);
  sublayout->addWidget(table);

  auto central_widget = new QWidget {};
  central_widget->setLayout(sublayout);
  window->setCentralWidget(central_widget);

  window->resize(window->sizeHint());
  window->show();


  connect(table, &RendersTable::SelectedRenderChanged,
          this, &PlotMediator::ChangeDisplayedRender);

  renderer_   = new Renderer {this};
}

void PlotMediator::LoadDataFile()  {
  auto filename = QFileDialog::getOpenFileName(
        window, "Open XRF Image Data File", data_directory, "*.hyperc");

  if (filename.isEmpty()) {
    QMessageBox::critical(window, "Error!",
                         "You have chosen an empty filename.\n");
    return;
  }

  auto path = std::filesystem::absolute({filename.toStdString()});
  for (auto & state : mementos_) {
    // Check if the file hasn't been opened
    if (state.FilePath().compare(path) == 0) {
      QMessageBox::critical(window, "Error!",
                           "You have chosen a file that's already open.");
    }
  }

  try {
    renderer_->LoadHypercube(filename.toStdString());
    mementos_.push_back(renderer_->SaveState());
    RenderAndPaintImage();
  } catch (std::exception & e) {
    QMessageBox::critical(window, "Error!", e.what());
  }
}

void PlotMediator::RenderAndPaintImage() {
  // Render the default image for this new file (maximum integration limits)
  auto render = renderer_->RenderXRFImage();
  // Then display
  image_plot_->PlotRenderData(render);
  // And add an entry to the available renders table
  table->AddTableEntry(render);
}

void PlotMediator::ExportImageToPNG()
{
  QString filename =
      QFileDialog::getSaveFileName(window, "Save image as PNG", data_directory,
                                   tr("Images (*.png)"));
  if (filename.isEmpty())  {
    // Invalid filename. Warn of choice of filename then do nothing.
    return;
  }
  if (!filename.endsWith(".png"))  {
    // Fix filename extension
    filename.append(".png");
  }

  auto image = image_plot_->ConvertColormap();

  QFile file {filename};
  file.open(QIODevice::WriteOnly);
  if (image.save(&file))
  {
    // TODO add indication
    filename.prepend("... File saved in: ");
    //status->showMessage(fileName, 30);
  }
  file.close();
}

void PlotMediator::ChangePalette() {
  QStringList palette_names {};

  // Get the list of available palettes
  for (auto palette : image_plot_->palettes_) {
    palette_names << QString::fromStdString(palette.first);
  }

  // Ask the user which palette to use
  auto select = QInputDialog::getItem(window, "Palette", "Select a palette:",
                                      palette_names, 0, false);

  auto it = image_plot_->palettes_.find(select.toStdString());
  image_plot_->colormap_->setGradient(it->second);
  image_plot_->Replot();
}


void PlotMediator::PublishROISpectrum(std::array<QPoint, 4> corners) {
  auto spectrum = renderer_->ComputeROISpectrum(corners);

  auto ptr = spectrum_broadcast_.data() + 100;
  for (auto & event : spectrum) {
    *ptr = event;
    ++ptr;
  }

  spectrum_broadcast_.at(99) = 1;
}

} // namespace maxrf
