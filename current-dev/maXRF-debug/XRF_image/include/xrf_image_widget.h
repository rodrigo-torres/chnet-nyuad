#ifndef XRF_IMAGE_WIDGET_H
#define XRF_IMAGE_WIDGET_H

#include <QCheckBox>
#include <QGroupBox>
#include <QFileDialog>
#include <QDockWidget>
#include <QLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QMenu>
#include <QObject>
#include <QScrollArea>
#include <QSlider>
#include <QSpinBox>
#include <QString>
#include <QToolBar>

#include "include/image_display.h"
#include "include/shm_wrapper.h"

namespace xrfimage
{

class XRFImageWidget : public QWidget
{
  Q_OBJECT // A Qt macro enabling Qt's C++ extensions (like signals and slots)
public:
  XRFImageWidget(QWidget * parent = nullptr);
  ~XRFImageWidget();

public slots:
  void ExportImageToPNG();

private:
  void CreateActions();
  void CreateWidget();
  void FilterImageDialog();

private:
  shm::array<double> shared_memory5;

  // STATIC methods. Internal methods only seen in this translation unit
  // TODO variable below must be implemented as a user preference variable
  static QString data_directory;
  static double energy_calibration_param0;
  static double energy_calibration_param1;
  static int const histogram_lowest_bin = 0;
  static int const histogram_highest_bin = 16383;

  ImgLabel * image_label_;
  QToolBar * toolbar_;
  QCustomPlot * image_histogram_;
};
} // namespace xrfimage
#endif // XRFIMAGEWIDGET_H
