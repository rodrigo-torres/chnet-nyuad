#include "include/xrf_image_widget.h"
#include "include/shm_wrapper.h"

namespace xrfimage
{

XRFImageWidget::XRFImageWidget(QWidget * parent) : parent_{parent}
{
  parent_ = this;

  shm::TypeDefInitSHM shm5_init;
  shm5_init.key = 8000;
  shm5_init.size = 4096;
  shm5_init.shmflag = shm::SHMFlags::CREATE | shm::SHMPermissions::ALL_ALL;

  shared_memory5.initialize(shm5_init);
  //shared_memory5 = posix::assignSHM<double>(8000, 4096);

  CreateActions();
  CreateWidget();
  this->resize(this->sizeHint());
  this->show();
}

XRFImageWidget::~XRFImageWidget()
{
  //shmdt(shared_memory5);
}

QWidget * XRFImageWidget::widget()
{
  return widget_;
}

QToolBar * XRFImageWidget::toolbar()
{
  return toolbar_;
}


QString XRFImageWidget::data_directory = "/home/frao/Documents/Workspaces/MAXRF/Data";

void XRFImageWidget::CreateActions()
{
  // CHILD WIDGETS OF ACTIONS GROUPBOX
  toolbar_ = new QToolBar;

  auto open_action = new QAction;
  open_action->setIcon(QIcon::fromTheme("document-open"));
  open_action->setToolTip("Open a new XRF Image file");

  auto export_action = new QAction;
  export_action->setIcon(QIcon::fromTheme("document-save-as"));
  export_action->setToolTip("Export XRF Image into a PNG image");

  auto convert_action = new QAction;
  convert_action->setIcon(QIcon{":/icons/map/file-convert.png"});
  convert_action->setToolTip("Convert an old format into a new format");

  auto pixel_action = new QAction;
  pixel_action->setIcon(QIcon::fromTheme("zoom-in"));
  pixel_action->setToolTip("Set new pixel size");

  auto integral_action = new QAction;
  integral_action->setIcon(QIcon{":/icons/map/integral.png"});
  integral_action->setToolTip("Compute integral between limits");

  auto reload_button = new QAction;
  //reload_button->setIcon(QIcon{":/icons/map/reload.ico"});
  reload_button->setIcon(QIcon::fromTheme("view-refresh"));
  reload_button->setToolTip("Reload current XRF Image with new parameters");

  toolbar_->addAction(open_action);
  toolbar_->addAction(export_action);
  toolbar_->addAction(convert_action);
  toolbar_->addAction(pixel_action);
  toolbar_->addAction(integral_action);
  toolbar_->addAction(reload_button);


  connect(open_action, &QAction::triggered, this, &XRFImageWidget::LoadImageDataFile);
  connect(export_action, &QAction::triggered, this, &XRFImageWidget::ExportImageToPNG);
  // TODO pixel_action slot
  connect(reload_button, &QAction::triggered, this, &XRFImageWidget::DisplayImageLabel);

}



void XRFImageWidget::CreateWidget()
{
  //QWidget * tab = new QWidget;
  QImage image(":/images/TT_CHNet_res2.png");

  // CHILD WIDGETS
  image_label_ = new ImgLabel;
  image_label_->setPixmap(QPixmap::fromImage(image));
  image_label_->setBackgroundRole(QPalette::Base);

  auto scroll_area_ = new QScrollArea;
  scroll_area_->setBackgroundRole(QPalette::Dark);
  scroll_area_->setMinimumSize(450, 450);
  scroll_area_->setWidget(image_label_);
  scroll_area_->setAlignment(Qt::AlignCenter);

  auto image_actions_box = new QGroupBox{tr("Image Controls")};
  image_actions_box->setAlignment(Qt::AlignCenter);
  //image_actions_box->setStyleSheet("QGroupBox {border: 1px solid #000}");


  auto integral_select_layout = new QHBoxLayout;
  auto select_bins = new QCheckBox{"Instrumental Bins"};
  auto select_internal_cal = new QCheckBox{"Energy"};
  auto select_external_cal = new QCheckBox{"Energy (external calibration)"};

  integral_select_layout->addWidget(select_bins);
  integral_select_layout->addWidget(select_internal_cal);
  integral_select_layout->addWidget(select_external_cal);

  auto title_limits = new QLabel{"Integral Limits"};

  auto limits_low_label = new QLabel{"Low "};
  limits_low_label->setFixedSize(limits_low_label->minimumSizeHint());
  auto limits_low_slider = new QSpinBox;
  limits_low_slider->setMinimum(histogram_lowest_bin);
  limits_low_slider->setMaximum(histogram_highest_bin);
  limits_low_slider->setValue(histogram_lowest_bin);
  limits_low_slider->setSingleStep(1);
  auto limits_low_edit = new QLineEdit;
  limits_low_edit->setText(QString::number(limits_low_slider->value()));
  limits_low_edit->setMaximumSize(limits_low_edit->minimumSizeHint());

  auto limits_high_label = new QLabel{"High"};
  limits_high_label->setFixedSize(limits_high_label->minimumSizeHint());
  auto limits_high_slider = new QSpinBox;
  limits_high_slider->setMinimum(histogram_lowest_bin);
  limits_high_slider->setMaximum(histogram_highest_bin);
  limits_high_slider->setValue(histogram_highest_bin);
  limits_high_slider->setSingleStep(1);
  auto limits_high_edit = new QLineEdit;
  limits_high_edit->setText(QString::number(limits_high_slider->value()));
  limits_high_edit->setMaximumSize(limits_low_edit->minimumSizeHint());

  QGridLayout * grid = new QGridLayout;
  grid->addWidget(title_limits, 0, 0, 1, 2);
  grid->addWidget(limits_low_label, 1, 0);
  grid->addWidget(limits_low_slider, 1, 1);
  grid->addWidget(limits_low_edit, 1, 2);
  grid->addWidget(limits_high_label, 2, 0);
  grid->addWidget(limits_high_slider, 2, 1);
  grid->addWidget(limits_high_edit, 2, 2);
  grid->setSpacing(5);

  image_actions_box->setLayout(grid);

  auto progress_bar = new QProgressBar;
  progress_bar->setRange(0, 100);
  progress_bar->setValue(0);

  QVBoxLayout * layout = new QVBoxLayout;
  layout->addWidget(scroll_area_);
  layout->addWidget(toolbar_);
  layout->addLayout(integral_select_layout);
  layout->addWidget(image_actions_box);
  layout->addWidget(progress_bar);
  this->setLayout(layout);
  //tab->setLayout(layout);

  auto action_sliderlow = [=](int i) {
    limits_low_edit->setText(QString::number(i));
    limits_high_slider->setMinimum(i);
    shared_memory5.at(100) = i;
  };

  auto action_sliderhigh = [=](int i) {
    limits_high_edit->setText(QString::number(i));
    limits_low_slider->setMaximum(i);
    shared_memory5.at(101) = i;
  };

  connect(limits_low_slider, QOverload<int>::of(&QSpinBox::valueChanged), action_sliderlow);
  connect(limits_high_slider, QOverload<int>::of(&QSpinBox::valueChanged), action_sliderhigh);

  connect(image_label_, &ImgLabel::UpdateProgressBar, progress_bar, &QProgressBar::setValue);
  widget_ = this;
}


void XRFImageWidget::LoadImageDataFile()
{
  QFileDialog dialog;
  QString filename = dialog.getOpenFileName(nullptr, "Open XRF Image Data File", data_directory);

  if (filename.isEmpty())
  {
    QMessageBox msg_box;
    msg_box.warning(parent_, "Warning!", "You have chosen an empty filename.\n"
                                         "No image data has been loaded onto memory.");
    return;
  }

  image_label_->default_image_.LoadDataFile(filename.toStdString());
  if (image_label_->default_image_.is_valid())
  {
    // The function below provided the functionality to pass a spectrum
    //  onto shared memory. With the XRFImage class we can pass straight to
    //  the DisplayImageSHM
    image_label_->default_image_.set_shared_memory5(shared_memory5.data());
    image_label_->set_image_data(&image_label_->default_image_);
    DisplayImageLabel();
  }
  else
  {
    QMessageBox msg_box;
    msg_box.critical(parent_, "Error!",
                     QString::fromStdString(xrf_image.err_msg()));
  }
}

void XRFImageWidget::DisplayImageLabel()
{
  // TODO. The available color palettes should be given directly by the
  //  a method of the class ImgLabel. It'd be less prone to mistakes
  bool valid { false };
  QString item =
      QInputDialog::getItem(parent_, tr("Color Palette"),
                            tr("Using color palette:"),
  { tr("Viridis"), tr("Grayscale") },
                            0, false, &valid);

  if (valid)
  {
    // Ask the QLabel handling the XRF image to render the image again
    //  using the color palette specified in 'item' and the pixel size
    //  currently specified in its internal member 'pixel_dimension_'
    image_label_->set_current_palette(item);
    image_label_->DisplayImage();

    // There's no need to set imageLabel again as the widget in scrollArea
    // The QScrollArea reacts dinamically to the size and contents of its
    //  underlying widget.
  }
}

void XRFImageWidget::ExportImageToPNG()
{
  QImage image = image_label_->qimage();
  if (image.isNull())
  {
    // NULL image, do nothing. Perhaps warn of uninitialized image
    //status->showMessage("[!] No image loaded", 30000);
    return;
  }

  QString filename =
      QFileDialog::getSaveFileName(parent_, "Save image as PNG", data_directory,
                                   tr("Images (*.png)"));
  if (filename.isEmpty())
  {
    // Invalid filename. Warn of choice of filename then do nothing.
    return;
  }
  if (!filename.endsWith(".png"))
  {
    // Fix filename extension
    filename.append(".png");
  }

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


// -------------------- FUNCTIONALITY TO BE IMPLEMENTED -------------------- //
// TODO false color / composed image methods
// TODO live xrf image functionality
// TODO data format conversion methods

}
