#include "include/mainwindow.h"
#include "include/image_display.h"

#include <QDockWidget>
#include <QToolButton>

// External global variables to be somehow encapsulated
QSpinBox* image_low_limit_slider = nullptr;
QSpinBox * image_high_limit_slider = nullptr;
QLineEdit * image_low_limit_edit = nullptr;
QLineEdit * image_high_limit_edit = nullptr;

static int histogram_lowest_bin = 0;
static int histogram_highest_bin = 16383;

QWidget * MainWindow::MakeSpectrumDisplayDockTab()
{
  QWidget * tab = new QWidget;
  QLabel * dock_label = new QLabel("<center>[!]IN CONSTRUCTION. COMING SOON [!]</center>");

  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(dock_label, Qt::AlignCenter);

  tab->setLayout(layout);
  return tab;
}

QWidget * MainWindow::MakeImageDockTab()
{
  QWidget * tab = new QWidget;
  QImage image(":/images/TT_CHNet_res2.png");

  // CHILD WIDGETS
  image_label_ = new ImgLabel;
  image_label_->setPixmap(QPixmap::fromImage(image));
  image_label_->setBackgroundRole(QPalette::Base);

  scroll_area_ = new QScrollArea;
  scroll_area_->setBackgroundRole(QPalette::Dark);
  scroll_area_->setMinimumSize(450, 450);
  scroll_area_->setWidget(image_label_);
  scroll_area_->setAlignment(Qt::AlignCenter);

  auto image_actions_box = new QGroupBox{tr("Image Controls")};
  image_actions_box->setAlignment(Qt::AlignCenter);
  //image_actions_box->setStyleSheet("QGroupBox {border: 1px solid #000}");

  // CHILD WIDGETS OF ACTIONS GROUPBOX
  auto reload_button = new QToolButton;

  QIcon::setThemeName("app-icons");
  reload_button->setIcon(QIcon{":/icons/reload.ico"});


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
  grid->addWidget(reload_button, 1, 3, 2, 1);
  grid->setSpacing(5);


  image_actions_box->setLayout(grid);

  QVBoxLayout * layout = new QVBoxLayout;
  layout->addWidget(scroll_area_);
  layout->addWidget(image_actions_box);
  tab->setLayout(layout);

  connect(limits_low_slider, QOverload<int>::of(&QSpinBox::valueChanged),
          [=](int i){
  limits_low_edit->setText(QString::number(i));
  limits_high_slider->setMinimum(i);
  });
  connect(limits_low_slider, QOverload<int>::of(&QSpinBox::valueChanged),
          this, &MainWindow::SelectLowIntegralLimit);
  connect(limits_high_slider, QOverload<int>::of(&QSpinBox::valueChanged),
          this, &MainWindow::SelectHighIntegralLimit);
  connect(reload_button, &QToolButton::clicked, this, &MainWindow::DisplayImageLabel);

  image_low_limit_slider = limits_low_slider;
  image_low_limit_edit = limits_low_edit;
  image_high_limit_slider = limits_high_slider;
  image_high_limit_edit = limits_high_edit;

  return tab;
}

QTabWidget * MainWindow::MakeDockWidget()
{
  // Passing the parent parameter is important, as the parent QWidget takes
  //  ownership of the children widgets, and makes sure they are properly
  //  destroed at the end of the program's life.
  //QDockWidget * dock = new QDockWidget {this};
  QTabWidget * tab_widget = new QTabWidget;

  QWidget * tab = MakeImageDockTab();
  tab_widget->addTab(tab, "XRF Image");
  tab = MakeSpectrumDisplayDockTab();
  tab_widget->addTab(tab, "XRF Spectra");

  //tab_widget->adjustSize();
  //tab_widget->setMinimumSize(tab_widget->size());

//  dock->setWidget(tab_widget);
//  dock ->setFeatures(QDockWidget::DockWidgetFloatable |
//                     QDockWidget::DockWidgetMovable);
//  dock ->setAllowedAreas(Qt::AllDockWidgetAreas);

  return tab_widget;
}

