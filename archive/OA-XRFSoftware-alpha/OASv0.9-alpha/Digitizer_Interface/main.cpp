#include <QtGui>
#include <QSpinBox>
#include "mainwindow.h"
#include "../Header.h"

int Resolution_mode=0; 

int main(int argc, char *argv[])
{
     QString ResMod;
     QString text="ScreenResolution";
      QFile file(text);
      if(file.exists())
      {
        file.open(QIODevice::ReadOnly);
        QString line;
         line = file.readLine();
         int numero=line.toInt();
         file.close();
         if (numero >  2400)                    {Resolution_mode=0; ResMod="high";}
         if (numero <  2400 && numero >= 1750)  {Resolution_mode=1; ResMod="medium";}
         if (numero <= 1750)                    {Resolution_mode=2; ResMod="low";}
      }
     else {qDebug()<<"ScreenResolution not defined....!!!\n"; exit ;}


    QApplication a(argc, argv);
    MainWindow w;

//    w.resize(1160, 740);

      switch (Resolution_mode)
       {
        case(0): {w.resize( 1160, 740  ); w.setMinimumSize (1160,740); w.setMaximumSize(1160,740); break;}
        case(1): {w.resize( 870, 555  ); w.setMinimumSize (870,555); w.setMaximumSize(870,555); break;}
        case(2): {w.resize( 580, 370  ); w.setMinimumSize (580,370); w.setMaximumSize(580,370); break;}
       }


    w.setWindowTitle(QApplication::translate("childwidget", "Digitiser Settings"));
    w.show();

    return a.exec();
}
