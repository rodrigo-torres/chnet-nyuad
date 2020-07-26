#include "MAXRF/xrf_image_widget.h"

#include <QApplication>

int main(int argc, char * argv[])
{
  QApplication app(argc, argv);
  // Resizing and show directives are inside the MainWindow's constructor
  maxrf::XRFImageWidget window{};
  return app.exec();

}
