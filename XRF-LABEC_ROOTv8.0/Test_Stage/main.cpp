////////////////////////////////////////////////////// FRAO 2016 - December /////////////////////////////////////
//
//         CHNet - INFN (TEST Motori) 
//
//     used to develop plug&play
//                                                                   
//
//



#include <QApplication>
#include <QObject>
#include <QSizePolicy>
#include "mainwindow.h"
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h>
#include <QScreen>
#include <time.h>
#include <QTextStream>
#include <QString>
#include <QFile>

int Resolution_mode=0; // 0 if          WIDTH >  2400px
                       // 1 if 1750px < WIDTH <= 2400 
                       // 2 if          WIDTH <= 1750


int main(int argc, char *argv[])
{


     QString ResMod;
     system("./ScreenDetector");
     Sleeper::msleep(100);

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


      system("su -c 'chmod 777 /dev/ttyUSB*; chown frao:frao /dev/ttyUSB*;'");

      QApplication a(argc, argv);
      MainWindow w;

      switch (Resolution_mode)
       {
        case(0): {w.resize( 1120, 880 ); break;}
        case(1): {w.resize( 1815, 810  ); break;}
        case(2): {w.resize( 1210, 540  ); break;}
       }

       w.show();
    
return a.exec(); 
}






