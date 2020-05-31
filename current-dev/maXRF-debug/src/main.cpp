#include "include/mainwindow.h"

// Checking of access rights to COM ports, i.e. stages and MCA USB ports,
// happens at runtime (before attempting to access the port).
// Checking the CAEN USB driver is loaded in the kernel space also happens
// at runtime, before the start of a DAQ session.

// utility_c.h provides an API to perform the checks mentioned above

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  // Resizing and show directives are inside the MainWindow's constructor
  MainWindow w;
  return a.exec();
}






