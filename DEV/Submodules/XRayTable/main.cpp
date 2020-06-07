#include <QtGui>
#include <QSpinBox>
#include "mainwindow.h"
#include "../Header.h"

int Resolution_mode=0; 

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.resize( 580, 490);
    w.setMinimumSize (580,490);
    w.setMaximumSize(580 , 490);

    //w.setWindowTitle(QApplication::translate("childwidget", "X-Ray Energy Table"));
    w.show();

    return a.exec();
}
