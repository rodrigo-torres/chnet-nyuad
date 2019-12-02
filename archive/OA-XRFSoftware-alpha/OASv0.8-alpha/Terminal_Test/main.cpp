#include <QtGui>
#include <QSpinBox>
#include "../Header.h"

bool First_run=false; 

int main(int argc, char *argv[])
{
    if(!First_run)
     {
      system("./prova &");
      First_run=true;
     }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
