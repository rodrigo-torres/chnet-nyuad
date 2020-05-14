#include "include/mainwindow.h"
#include "include/image_display.h"
#include "include/xrfimage.h"

XRFImage xrf_image {};
extern int *shared_memory;
extern double *shared_memory5;

// STATIC methods. Internal methods only seen in this translation unit
// TODO variable below must be implemented as a user preference variable
static QString data_directory = "/home/frao/Documents/Workspaces/MAXRF/Data";
static double energy_calibration_param0;
static double energy_calibration_param1;

static bool UpdateEnergyCalibrationParameters()
{
  bool valid {false};
  QString filename = QDir::currentPath();
  filename.append("/conf/Calibration.txt");

  QFile file {filename};
  if (file.open(QIODevice::ReadOnly))
  {
    auto str = file.readLine();
    energy_calibration_param1 = str.toDouble();
    str = file.readLine();
    energy_calibration_param0 = str.toDouble();
    energy_calibration_param1 == 0 ?
          valid = false : valid = true;
  }
  file.close();
  return valid;
}

static double ConvertEnergyToChannel(double energy)
{
  energy -= energy_calibration_param0;
  energy /= energy_calibration_param1;
  return energy;
}

// //////////////////// GUI INTERFACE METHODS //////////////////// //

extern QSpinBox * image_low_limit_slider;
extern QSpinBox* image_high_limit_slider;
extern QLineEdit * image_low_limit_edit;
extern QLineEdit * image_high_limit_edit;

void MainWindow::SelectLowIntegralLimit(int val)
{
  shared_memory5[100] = val;
  //image_low_limit_edit->setText(QString::number(val));
  //image_high_limit_slider->setMinimum(val);
}

void MainWindow::SelectHighIntegralLimit(int val)
{
  shared_memory5[101] = val;
  image_high_limit_edit->setText(QString::number(val));
  image_low_limit_slider->setMaximum(val);
}


// Function is called from 'Select channel intervals' action button on the menu
void MainWindow::SelectChannels()
{
  bool valid { false };
  double channel { 0 };

  switch (shared_memory[24])
  {
  case 0:
    channel = QInputDialog::getInt(this, tr("Lowest Histogram Bin"),
                                   "Bin Low:", 0, 0, 16383, 1, &valid);
    if (valid)
    {
      qDebug()<<"INFO: New lower channel set to:"<<channel<<'\n';
      shared_memory5[100] = channel;
    }
    channel = QInputDialog::getInt(this, tr("Highest Histogram Bin"),
                                   "Bin High:", 16383, 0, 16383, 1, &valid);
    if (valid)
    {
      qDebug()<<"INFO: New upper channel set to:"<<channel<<'\n';
      shared_memory5[101] = channel;
    }
    break;
  case 1:
    if (!UpdateEnergyCalibrationParameters())
    {
      // Invalid energy calibration parameters
      // TODO warn user and advise to check calibration conf file
      return;
    }

    channel = QInputDialog::getDouble(this, tr("Lowest Spectrum Energy"),
                                   "Energy Low:", 0, 0, 60., 2, &valid);
    if (valid)
    {
      channel = ConvertEnergyToChannel(channel);
      qDebug()<<"INFO: New lower energy set to:"<<channel<<'\n';
      shared_memory5[100] = channel;
    }
    channel = QInputDialog::getDouble(this, tr("Highest Spectrum Energy"),
                                   "Energy High:", 60., 0, 60., 2, &valid);
    if (valid)
    {
      channel = ConvertEnergyToChannel(channel);
      qDebug()<<"INFO: New upper energy set to:"<<channel<<'\n';
      shared_memory5[101] = channel;
    }
    break;
  default:
    qDebug()<<"[!] Invalid value found in memory at shared_memory[24]\n";
    break;
  }
}




void MainWindow::ChangePixelDimension() { // Change pixel dimension
  bool valid {false};
  int px = QInputDialog::getInt(this, "Set Pixel Size",
                                "Pixel side length:", 1, 1, 30, 1, &valid);

  if (valid)
  {
    image_label_->set_pixel_dim(px);
    printf("[!] New pixel dimension: %d\n", px);
  }
  else
  {
    printf("[!] Couldn't obtain new pixel dimensions\n");
  }
}


void MainWindow::LoadImageDataFile()
{
    QString filename =
        QFileDialog::getOpenFileName(this, "Open XRF Image Data File",
                                     data_directory);
    if (filename.isEmpty())
    {
        QMessageBox msg_box;
        msg_box.warning(this, "Warning!", "You have chosen an empty filename.\n"
                         "No image data has been loaded onto memory.");
        return;
    }

    xrf_image.LoadDataFile(filename.toStdString());
    if (xrf_image.is_valid())
    {
      // The function below provided the functionality to pass a spectrum
      //  onto shared memory. With the XRFImage class we can pass straight to
      //  the DisplayImageSHM
      // LoadNewFile_SHM();
      image_label_->set_image_data(&xrf_image);
      DisplayImageLabel();
    }
    else
    {
      QMessageBox msg_box;
      msg_box.critical(this, "Error!",
                       QString::fromStdString(xrf_image.err_msg()));
    }
}

void MainWindow::DisplayImageLabel()
{
  // TODO. The available color palettes should be given directly by the
  //  a method of the class ImgLabel. It'd be less prone to mistakes
  bool valid { false };
  QString item =
      QInputDialog::getItem(this, tr("Color Palette"),
                            tr("Using color palette:"),
                            { tr("Viridis"), tr("Grayscale") },
                            0, false, &valid);

  if (valid)
  {
    // Ask the QLabel handling the XRF image to render the image again
    //  using the color palette specified in 'item' and the pixel size
    //  currently specified in its internal member 'pixel_dimension_'
    image_label_->DisplayImage(item);

    // There's no need to set imageLabel again as the widget in scrollArea
    // The QScrollArea reacts dinamically to the size and contents of its
    //  underlying widget.
  }
}

void MainWindow::ExportImageToPNG()
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


// The 'Reload map' action button on the menu now redirects directly to the
//  DisplayImageLabel method.
[[deprecated]] void MainWindow::LoadNewFile_SHM() { }

// TODO
// Functionality to be implemented through the XRFImage class
[[deprecated]] void MainWindow::displaySumImage_SHM() { }

// Data saving is now handled by the DAQ protocol
[[deprecated]] void MainWindow::saveImageXRFData() {}

// Originally introduced to load the new data format. Now the new data format
//  is loaded by default. And should the format be different, the program
//  triggers a file conversion. A new file with the optimized data format is
//  created while leaving the previous untouched.
[[deprecated]] void MainWindow::load_optimized() {}

// It originally copied the contents of a file onto a shared memory segment
//  in this case, shared_memory2. It also populated the correct scan parameters
// With the new data format this is no longer needed, and load times are
//  optimized. We should only trigger a file conversion from the old format
//  to the new format when the header version is not recognized.
[[deprecated]] void MainWindow::LoadNewFileWithNoCorrection_SHM() { }

// It took the values on shared_memory2 to create an optimized parse
//  of the data in a different shared memory segment, shared_memory3
// It also computed the ROI integrals. This functionality is all replicated
//  in the new xrf_image object through the roi_integrals member of the
//  Pixel structure.
[[deprecated]] void MainWindow::LoadSHM_SumMap() { }

