#include "widgets.h"

extern template class maxrf::ipc::shmarray<double>;

namespace maxrf
{


RendersTable::RendersTable(QWidget * parent) : QWidget {parent} {
  auto renders_box = new QGroupBox {"Available Renders"};
  auto remove_render  = new QPushButton {"Remove Render"};
  auto display_render = new QPushButton {"Display Render"};

  // AVAILABLE RENDERS TABLE
  auto table = new QTableWidget {0, 4};
  table->setHorizontalHeaderLabels(QStringList {}
                                   << "Filename" << "Energy Label"
                                   << "Integral Limits"
                                   << "Intensity Cutoffs");
  table->setObjectName("Renders Table");

  auto layout  = new QGridLayout {};
  layout->addWidget(remove_render, 0, 1);
  layout->addWidget(display_render, 0, 2);
  layout->addWidget(table, 1, 0, 1, 3);

  renders_box->setLayout(layout);

  auto master = new QVBoxLayout {};
  master->addWidget(renders_box);

  this->setLayout(master);

  connect(remove_render, &QPushButton::clicked,
          this, &RendersTable::RemoveTableEntry);

  connect(display_render, &QPushButton::clicked, this, [=] () {
    auto row = table->currentRow();
    emit SelectedRenderChanged(this->entries_.at(row));
  });
}

void RendersTable::AddTableEntry(TableEntry entry) {
  using namespace std::literals;

  auto table = this->findChild<QTableWidget *>("Renders Table");
  auto row   = table->rowCount();
  table->insertRow(row);

  auto edit = new QLineEdit {entry->filename.c_str()};
  edit->setEnabled(false);

  table->setCellWidget(row, 0, edit);
  // Column 1 is the default QLineEdit, reserved for the user label

  auto format = "[ "s + std::to_string(entry->integral_limits.first) + ", "s
      + std::to_string(entry->integral_limits.second) + " ]"s;
  edit = new QLineEdit {format.c_str()};
  edit->setEnabled(false);
  table->setCellWidget(row, 2, edit);

  format = "[ "s + std::to_string(entry->intensity_cutoffs.first) + ", "s
      + std::to_string(entry->intensity_cutoffs.second) + " ]"s;
  edit = new QLineEdit {format.c_str()};
  edit->setEnabled(false);
  table->setCellWidget(row, 3, edit);

  // Resizing to contents
  table->resizeColumnsToContents();

  entries_.push_back(entry);
}

void RendersTable::RemoveTableEntry() {
  // Entries are appended to the entries vector in the order they appear on the
  // table. So we remove the vector index specified
  auto table = this->findChild<QTableWidget *>("Renders Table");
  auto row   = table->currentRow();


  // Construct the informative text
  QString info, val;
  val = static_cast<QLineEdit *>(table->cellWidget(row, 0))->text();
  info.append("Filename: " + val + "\n");
  val = static_cast<QLineEdit *>(table->cellWidget(row, 2))->text();
  info.append("Integration limits: " + val + "\n");
  val = static_cast<QLineEdit *>(table->cellWidget(row, 3))->text();
  info.append("Intensity cutoffs: "  + val + "\n");

  QMessageBox msgbox {};
  msgbox.setText("You asked to delete an image render.");
  msgbox.setInformativeText("Are you sure you want to delete the rendered image"
                            " with the following properties?\n" + info);
  msgbox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
  msgbox.setDefaultButton(QMessageBox::No);

  switch (msgbox.exec()) {
  case QMessageBox::No :
    // Do nothing
    break;
  case QMessageBox::Yes : {
    table->removeRow(row);

    auto entry = *(entries_.begin() + row);
    entries_.erase(entries_.begin() + row);
    break;
  }
  default:
    // Never reached
    break;
  }
}





XRFImageWidget::XRFImageWidget(PlotMediator *mediator, QWidget * parent)
{
  Q_UNUSED(parent)
  //shared_memory5.initialize(Segments::SHARED_MEMORY_5);
//  image_label_ = new ImgLabel   {this};
  mediator_ = mediator;


  CreateActions();



}




static QString data_directory = "/home/frao/Documents/Workspaces/MAXRF/Data";

void XRFImageWidget::CreateActions()
{
  // CHILD WIDGETS OF ACTIONS GROUPBOX
  auto menubar = this->menuBar();  // A new menu bar is created upon calling

  auto menu = menubar->addMenu("File");

  auto open_action = menu->addAction("Open new XRF Image file",
                                     mediator_, &PlotMediator::LoadDataFile);
  open_action->setIcon(QIcon::fromTheme("document-open"));
  open_action->setToolTip("Open a .hyperc XRF Image data file and add the"
                          "the default rendered image to the buffer");

  auto close_action = menu->addAction("Close XRF Image file");
 //                                     image_plot, &PlotMediator::RemoveFromBuffer);
  close_action->setIcon(QIcon::fromTheme("document-close"));
  close_action->setToolTip("Remove all rendered images associated with a given"
                          "XRF Image data filename from the buffer, and close"
                          "the file.");

  auto export_action = menu->addAction("Export displayed image as a .PNG",
                                       mediator_, &PlotMediator::ExportImageToPNG);
  export_action->setIcon(QIcon::fromTheme("document-save-as"));
  export_action->setToolTip("Export XRF Image into a PNG image");

  menu = menubar->addMenu("Rendering");

  auto rendering_action = menu->addAction("New render",
                                          this, &XRFImageWidget::NewRenderDialog);
  rendering_action->setIcon(QIcon{":/icons/map/integral.png"});
  rendering_action->setToolTip("Compute integral between limits");

  menu = menubar->addMenu("Contrast");

  auto no_adjust  = menu->addAction("No contrast adjustments");
  auto stretch    = menu->addAction("Adjust by stretching");
  auto equalize   = menu->addAction("Adjust by equalization");


  menu = menubar->addMenu("Selection Mode");

  auto no_selection = menu->addAction("Dragging & Scroll Wheel Zooming", this, [=]() {
    mediator_->ChangeSelectionMode(SelectionMode::kNone);
  });

  auto roi_spectra = menu->addAction("Compute ROI Spectrum", this, [=]() {
    mediator_->ChangeSelectionMode(SelectionMode::kROISpectrum);
  });


  auto select_zoom = menu->addAction("Selection Zooming", this, [=]() {
    mediator_->ChangeSelectionMode(SelectionMode::kZoom);
  });

  auto select_data = menu->addAction("Selection Data", this, [=]() {
    mediator_->ChangeSelectionMode(SelectionMode::kSelect);
  });


  menu = menubar->addMenu("View");

  auto palette = menu->addAction("Select palette",
                                mediator_, &PlotMediator::ChangePalette);

  auto log = menu->addAction("Toggle log-scale",
                                mediator_, &PlotMediator::ChangeScale);

  auto zoom = menu->addAction("Restore Zoom",
                                mediator_, &PlotMediator::ChangePalette);

}

void XRFImageWidget::NewRenderDialog() {
  RenderDialog render {this};
  render.exec();

  // Wait for user input then
  mediator_->ChangeLimits(render.result_);
  mediator_->RenderAndPaintImage();

  // Update the available renders table and directory
}

RenderDialog::RenderDialog(QWidget * parent) : QDialog {parent}
{
  static int const histogram_lowest_bin = 0;
  static int const histogram_highest_bin = 16383;

  // ENERGY CALIBRATION

  auto calibration = new QGroupBox {"Energy Calibration"};
  auto cal_layout = new QFormLayout{};

  cal_layout->addRow("Order 0", new QDoubleSpinBox {});
  cal_layout->addRow("Order 1", new QDoubleSpinBox {});
  cal_layout->addRow("Order 2", new QDoubleSpinBox {});

  calibration->setLayout(cal_layout);

  auto cal_source = new QGroupBox {"Calibration Source"};
  auto source_layout = new QVBoxLayout;

  source_layout->addWidget(new QRadioButton {"User"});
  source_layout->addWidget(new QRadioButton {"Data File"});
  source_layout->addWidget(new QRadioButton {"Spectra window"});

  cal_source->setLayout(source_layout);

  //  ENERGY LIMITS SELECTION

  auto energy_low = new QDoubleSpinBox{};
  energy_low->setRange(0., 100.);
  energy_low->setValue(0);

  auto energy_high = new QDoubleSpinBox {};
  energy_high->setRange(0., 100.);
  energy_high->setValue(100.);

  auto energy_layout = new QFormLayout {};
  energy_layout->addRow("Energy 0", energy_low);
  energy_layout->addRow("Energy 1", energy_high);

  auto energy_box = new QGroupBox {};
  energy_box->setCheckable(true);
  energy_box->setChecked(false);
  energy_box->setLayout(energy_layout);

  //  SPECTRAL BINS LIMITS SELECTION

  chan_low = new QSpinBox{};
  chan_low->setRange(histogram_lowest_bin, histogram_highest_bin);
  chan_low->setValue(0);

  chan_high = new QSpinBox {};
  chan_high->setRange(histogram_lowest_bin, histogram_highest_bin);
  chan_high->setValue(histogram_highest_bin);

  auto limits_form  = new QFormLayout {};
  limits_form->addRow("Bin 0", chan_low);
  limits_form->addRow("Bin 1", chan_high);

  auto bin_box = new QGroupBox {};
  bin_box->setCheckable(true);
  bin_box->setChecked(true);
  bin_box->setLayout(limits_form);


  auto limits_layout = new QGridLayout {};
  limits_layout->addWidget(bin_box, 0, 0);
  limits_layout->addWidget(energy_box, 1, 0);
  limits_layout->addWidget(cal_source, 0, 1, 2, 1);
  limits_layout->addWidget(calibration, 0, 2, 2, 1);

  this->setLayout(limits_layout);
}

} // namespace maxrf
