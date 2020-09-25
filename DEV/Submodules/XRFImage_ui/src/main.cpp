#include "widgets.h"

#include <QApplication>

int main(int argc, char * argv[])
{
  QApplication app(argc, argv);
  // Resizing and show directives are inside the MainWindow's constructor
  maxrf::PlotMediator window{};
  return app.exec();

}
