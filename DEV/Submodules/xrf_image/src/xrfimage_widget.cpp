#include "MAXRF/xrf_image_widget.h"
namespace xrfimage
{

XRFImageWidget::XRFImageWidget(QWidget * parent)
{
  Q_UNUSED(parent)
  shared_memory5.initialize(Segments::SHARED_MEMORY_5);
  image_label_ = new ImgLabel{this};

  CreateActions();
  CreateWidget();
  this->resize(this->sizeHint());
  this->show();
}

XRFImageWidget::~XRFImageWidget()
{

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


  connect(open_action, &QAction::triggered,
          image_label_, &ImgLabel::AddImageToBuffer);
  connect(export_action, &QAction::triggered,
          this, &XRFImageWidget::ExportImageToPNG);
  // TODO pixel_action slot
  connect(reload_button, &QAction::triggered,
          image_label_, &ImgLabel::RenderImage);

}

void XRFImageWidget::FilterImageDialog()
{
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

  QVBoxLayout * layout = new QVBoxLayout;
  layout->addLayout(integral_select_layout);
  layout->addWidget(image_actions_box);


  // TODO setObjectNames for children to avoid pointers

  connect(limits_low_slider, QOverload<int>::of(&QSpinBox::valueChanged), action_sliderlow);
  connect(limits_high_slider, QOverload<int>::of(&QSpinBox::valueChanged), action_sliderhigh);
}



void XRFImageWidget::CreateWidget()
{
  //QWidget * tab = new QWidget;
  QImage image(":/images/TT_CHNet_res2.png");

  // CHILD WIDGETS
  image_label_->setPixmap(QPixmap::fromImage(image));
  image_label_->setBackgroundRole(QPalette::Base);

  auto scroll_area_ = new QScrollArea;
  scroll_area_->setBackgroundRole(QPalette::Dark);
  scroll_area_->setMinimumSize(450, 450);
  scroll_area_->setWidget(image_label_);
  scroll_area_->setAlignment(Qt::AlignCenter);
  \

  auto tab_widget = new QTabWidget;
  tab_widget->setEnabled(false);

  //  // ---------- TAB1 WIDGET DEFINITION ---------- //
  auto tab1 = new QWidget;

  QVector<double> x(256), y(256);
  for (int i=0; i <x.size(); ++i)
  {
    x[i] = i;
    y[i] = i;
  }

  image_histogram_ = new QCustomPlot{};

  auto bars1 = new QCPBars(image_histogram_->xAxis, image_histogram_->yAxis);
  bars1->setWidth(1);
  bars1->setData(x, y, true);
  bars1->setPen(Qt::NoPen);
  bars1->setBrush(QColor("Orange"));


  connect(image_label_, &ImgLabel::UpdateImageHistogram,
          this, [=](QVector<uint> histogram) {
    auto it = histogram.begin();
    for (auto & i : *bars1->data())
    {
      i.value = *it;
      ++it;
    }
    auto max = *std::max_element(histogram.begin(), histogram.end());
    image_histogram_->yAxis->setRange(0, max);
    image_histogram_->replot();
  });


  image_histogram_->yAxis->setRange(0, 255);
  image_histogram_->xAxis->setRange(0, 255);
  image_histogram_->yAxis->setVisible(true);
  image_histogram_->xAxis->setVisible(true);
  image_histogram_->setBackground(QBrush(QColor("white")));

  image_histogram_->replot();

  auto checklist1 = new QCheckBox{"Filter 1"};

  //  BRIGHTNESS CONTROLS

  auto brightness_slider = new QSlider{Qt::Horizontal};
  brightness_slider->setRange(-50, 50);
  brightness_slider->setSingleStep(1);
  brightness_slider->setPageStep(10);
  brightness_slider->setValue(0);

  auto brightness_indicator = new QLabel;
  brightness_indicator->setNum(brightness_slider->value());

  auto brightness_controls = new QHBoxLayout;
  brightness_controls->addWidget(new QLabel{"Adjust Brightness:"});
  brightness_controls->addWidget(brightness_slider);
  brightness_controls->addWidget(brightness_indicator);


  QObject::connect(brightness_slider, &QSlider::valueChanged, this,
                   [=](int value) {
    auto formatted_string = QString::number(value).append("%");
    if (value > 0)
    {
      formatted_string.prepend("+");
    }
    brightness_indicator->setText(formatted_string);
  });

  // CONTRAST CONTROLS

  auto none_button      = new QRadioButton{"None"};
  auto stretch_button   = new QRadioButton{"Stretch"};
  auto equalize_button  = new QRadioButton{"Equalize"};

  auto contrast_opts = new QHBoxLayout;
  contrast_opts->addWidget(new QLabel{"Adjust Contrast:"});
  contrast_opts->addWidget(none_button);
  contrast_opts->addWidget(stretch_button);
  contrast_opts->addWidget(equalize_button);

  QObject::connect(brightness_slider, &QSlider::valueChanged,
                   image_label_, &ImgLabel::set_brightness);
  QObject::connect(equalize_button, &QRadioButton::toggled,
                   image_label_, &ImgLabel::ToggleEqualization);
  QObject::connect(stretch_button,  &QRadioButton::toggled,
                  image_label_, &ImgLabel::ToggleStretching);


  auto tab1_layout = new QVBoxLayout;
  tab1_layout->addWidget(checklist1);
  tab1_layout->addWidget(image_histogram_);
  tab1_layout->setStretchFactor(image_histogram_, 2);
  tab1_layout->addLayout(brightness_controls);
  tab1_layout->addLayout(contrast_opts);

  tab1->setLayout(tab1_layout);
  tab_widget->addTab(tab1, "Scale");
  tab_widget->setCurrentIndex(0);


  auto progress_bar = new QProgressBar;
  progress_bar->setRange(0, 100);
  progress_bar->setValue(0);

  auto image_select = new QComboBox;
  image_select->addItem("Logo");

  auto showing_layout = new QHBoxLayout;
  showing_layout->addWidget(new QLabel{"Currently Showing:"}, Qt::AlignRight);
  showing_layout->addWidget(image_select);

  QVBoxLayout * layout = new QVBoxLayout;
  layout->addWidget(scroll_area_);
  layout->addWidget(toolbar_);
  layout->addLayout(showing_layout);
  layout->addWidget(tab_widget);
  layout->addWidget(progress_bar);
  this->setLayout(layout);

  connect(image_label_, &ImgLabel::UpdateProgressBar,
          progress_bar, &QProgressBar::setValue);
  connect(image_label_, &ImgLabel::EnableImageControls, this,
         [=](){tab_widget->setEnabled(true);});
  connect(image_select, QOverload<int>::of(&QComboBox::currentIndexChanged),
          image_label_, &ImgLabel::ChangeImage);

  connect(image_label_, &ImgLabel::AddToComboBox,
          this, [=](QString name){
    image_select->addItem(name);
    image_select->setCurrentIndex(image_select->findText(name));
  });
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
      QFileDialog::getSaveFileName(this, "Save image as PNG", data_directory,
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
