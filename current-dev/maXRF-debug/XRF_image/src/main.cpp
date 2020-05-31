#include "include/xrf_image_widget.h"

#include <QApplication>

int main(int argc, char * argv[])
{
  QApplication app(argc, argv);
  // Resizing and show directives are inside the MainWindow's constructor
  xrfimage::XRFImageWidget window{};
  return app.exec();

}
