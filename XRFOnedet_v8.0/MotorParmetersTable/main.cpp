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
        case(0): {w.resize( 1000, 580  ); w.setMinimumSize (1000, 580); w.setMaximumSize(1000, 580); break;}
        case(1): {w.resize( 570, 450  ); w.setMinimumSize (570, 450); w.setMaximumSize(570, 450); break;}
        case(2): {w.resize( 380, 300  ); w.setMinimumSize (380, 300); w.setMaximumSize(380, 300); break;}
//        case(0): {w.resize( 760, 600  ); break;}
//        case(1): {w.resize( 570, 450  ); break;}
//        case(2): {w.resize( 380, 300  ); break;}
       }



    w.setWindowTitle(QApplication::translate("childwidget", "PI motors: parameters"));
    w.show();

    return a.exec();
}
