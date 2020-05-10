#include "h/mainwindow.h"
#include "h/image_display.h"
#include "h/xrfimage.h"

XRFImage xrf_image {};
extern double *shared_memory5;

// TODO variable below must be implemented as a user preference variable
static QString data_directory = "/home/frao/Documents/Workspaces/MAXRF/Data";

void adjust_map_limits() {
    // Called if energy calibration is active in the spectrum visualization program.
    QString dir = QDir::currentPath();
    dir += "/conf/Calibration.txt";
    QFile file(dir);
    file.open(QIODevice::ReadOnly);
    QString a,b;
    a = file.readLine();
    b = file.readLine();
    double const_a = a.toDouble();
    double const_b = b.toDouble();
    file.close();

    for (int i = 0; i < 8; i ++) {
    *(shared_memory5+100+i) -= const_b;
    *(shared_memory5+100+i) /= const_a;
    }
}

void MainWindow::LoadTxt()  {
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
      imageLabel->set_image_data(&xrf_image);
      displayImage_SHM();
    }
    else
    {
      QMessageBox msg_box;
      msg_box.critical(this, "Error!",
                       QString::fromStdString(xrf_image.err_msg()));
    }
}

void MainWindow::displayImage_SHM()
{
  bool valid { false };
  QString item =
      QInputDialog::getItem(this, tr("Color Palette"),
                            tr("Using color palette:"),
                            { tr("Viridis"), tr("Grayscale") },
                            0, false, &valid);

  if (valid)
  {
    //QImage image = xrf_image.ConstructQImage(item, Pixeldim);
    imageLabel->DisplayImage(item);

    scrollArea->setWidget(imageLabel);
    scrollArea->setBackgroundRole(QPalette::Dark);
  }
}

void MainWindow::export_map()
{
  QImage image = imageLabel->qimage();
  if (!image.isNull())
  {
    QString filename =
        QFileDialog::getSaveFileName(this, "Save image as PNG", data_directory,
                                     tr("Images (*.png)"));

    if (!filename.isEmpty())
    {
      if (!filename.endsWith(".png"))
      {
        filename.append(".png");
      }
      QFile file {filename};
      file.open(QIODevice::WriteOnly);
      if (image.save(&file))
      {
        file.close();
        filename.prepend("... File saved in: ");
        // TODO add indication
        //status->showMessage(fileName, 30);
      }
    }
  }
  //else status->showMessage("[!] No image loaded", 30000);
}


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

